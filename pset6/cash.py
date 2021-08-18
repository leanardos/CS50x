from cs50 import get_float

# asking input from the user
cash = get_float("Cash owed: ")

# while it is negative we are going to keep asking it
while (cash < 0):
    cash = get_float("Cash owed: ")

# multiply both the cash and the cents by 100 to simplyfi the calculation for myself
cash = cash * 100
count = 0

# first diving it to 25 and re-assigning the left cash to the cash variable
count += int(cash / 25)
cash = cash % 25

# then to 10..
count += int(cash / 10)
cash = cash % 10

# same for 5..
count += int(cash / 5)
cash = cash % 5

# lastly for 1..
count += int(cash / 1)
cash = cash % 1

print(count)