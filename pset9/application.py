import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


"""----------------------------- R E G I S T E R -------------------------------"""


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        if not username:
            return apology("Please fill in the username.")

        same_username = db.execute("select username from users where username = ?", request.form.get("username"))
        if same_username:
            return apology("This username is already exists.")

        password = request.form.get("password")
        if not password:
            return apology("Please fill in the password.")

        password_confirmation = request.form.get("confirmation")
        if not password:
            return apology("Please type your password again for confirmation.")

        if password != password_confirmation:
            return apology("Passwords does not match.")

        hashed_password = generate_password_hash(password)
        db.execute("insert into users (username, hash) values(?, ?)", username, hashed_password)

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        session["user_id"] = rows[0]["id"]

    return redirect("/")


"""----------------------------- L O G I N -------------------------------"""


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


"""----------------------------- L O G O U T -------------------------------"""


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


"""----------------------------- I N D E X -------------------------------"""


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    balance = db.execute("select cash from users where id = ?", session["user_id"])
    balance = "{:.2f}".format((balance[0]["cash"]))
    purchases = db.execute("select * from purchases where user_id = ?", session["user_id"])

    total_balance = 0
    for item in purchases:
        total_balance += item["total"]
    total_balance += float(balance)

    return render_template("index.html", purchases=purchases, balance=balance, total_balance=total_balance)


"""----------------------------- Q U O T E -------------------------------"""


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        response = lookup(symbol)
        if not response:
            return apology("Stock does not exists.")

        return render_template("quote.html", response=response)


"""----------------------------- B U Y -------------------------------"""


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    else:
        user_cash_balance = db.execute("select cash from users where id = ?", session["user_id"])
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("shares must be a posative integer", 400)
        except ValueError:
            return apology("shares must be a posative integer", 400)

        symbol = request.form.get("symbol")

        stock_details = lookup(symbol)
        if not stock_details:
            return apology("Stock does not exist")

        converted_cash = float(user_cash_balance[0]["cash"])
        current_purchase_cost = shares * stock_details["price"]
        if current_purchase_cost > converted_cash:
            return apology("Insufficient cash balance")
        else:
            converted_cash = converted_cash - current_purchase_cost
            previous_purchases = db.execute("select * from purchases where symbol = ? and user_id = ?", symbol, session["user_id"])
            total_shares = shares
            total_value = current_purchase_cost
            if previous_purchases:
                total_shares = int(shares) + int(previous_purchases[0]["shares"])
                total_value = usd(current_purchase_cost + float(previous_purchases[0]["total"]))
                db.execute("update purchases set shares = ?, total = ? where symbol = ? and user_id = ?",
                           total_shares, total_value, symbol, session["user_id"])
            else:
                db.execute("insert into purchases (user_id, shares, symbol, name, price, total) values(?, ?, ?, ?, ?, ?)",
                           session["user_id"], total_shares, symbol, stock_details["name"], stock_details["price"], total_value)

            db.execute("update users set cash = ? where id = ?", converted_cash, session["user_id"])

            now = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
            db.execute("insert into history (user_id, shares, symbol, name, price, transacted) values(?, ?, ?, ?, ?, ?)",
                       session["user_id"], total_shares, symbol, stock_details["name"], stock_details["price"], now)
            return redirect("/")


"""----------------------------- S E L L -------------------------------"""


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    purchases = db.execute("select symbol, shares, total from purchases where user_id = ?", session["user_id"])
    symbols = []
    for item in purchases:
        symbols.append(item["symbol"])
    if request.method == "GET":
        return render_template("sell.html", symbols=symbols)
    else:
        symbol = request.form.get("symbol")
        if not symbol in symbols:
            return apology(f"Symbol name: {symbol} does not match")
        shares = float(request.form.get("shares"))
        if not shares:
            return apology("Please fill in the shares")

        shares_details = db.execute(
            "select shares, price, name from purchases where symbol = ? and user_id = ?", symbol, session["user_id"])
        if shares > float(shares_details[0]["shares"]):
            return apology(f"You dont have {shares} shares of {symbol}")

        live_stock_details = lookup(symbol)
        updated_shares = shares_details[0]["shares"] - shares
        updated_total = updated_shares * live_stock_details["price"]
        db.execute("update purchases set shares = ?, total = ? where symbol = ? and user_id = ?",
                   updated_shares, updated_total, symbol, session["user_id"])

        previous_cash = db.execute("select cash from users where id = ?", session["user_id"])
        previous_cash = "{:.2f}".format((previous_cash[0]["cash"]))
        sell_total = shares * live_stock_details["price"]
        updated_cash = float(previous_cash) + float(sell_total)
        db.execute("update users set cash = ? where id = ?", updated_cash, session["user_id"])
        now = datetime.now().strftime("%d-%m-%Y %H:%M:%S")
        sell_shares = shares * (-1)
        sell_total = sell_shares * live_stock_details["price"]
        db.execute("insert into history (user_id, shares, symbol, name, price, total, transacted) values(?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], sell_shares, symbol, live_stock_details["name"], live_stock_details["price"], sell_total, now)
        return redirect("/")


"""----------------------------- H I S T O R Y -------------------------------"""


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("select * from history where user_id = ?", session["user_id"])

    return render_template("history.html", history=history)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
