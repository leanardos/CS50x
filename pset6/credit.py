from cs50 import get_int
import sys


def main():
    number = get_int("Number: ")

    # American Express: 15-digit numbers   -   numbers start with 34 or 37
    # MasterCard: 16-digit numbers   -   numbers start with 51, 52, 53, 54, or 55
    # Visa uses 13- and 16-digit numbers.   -   numbers start with 4

    # if it number length is less than 13 we directly print invalid and exit the program
    if (len(str(number)) < 13):
        print("INVALID")
        sys.exit()

    # if not, we fist validate if the number is valid by luhn's algorithm and if so we proceed to identify the kind of card number
    if validate_number(str(number)):

        # if firs two number begins with 34 or 37 then it is American Express
        if (str(number)[:2] in ["34", "37"]):
            print("AMEX")
            sys.exit()

        # if firs two number begins with 51, 52, 53, 54, or 55 then it is Mastercard
        elif (str(number)[:2] in ["51", "52", "53", "54", "55"]):
            print("MASTERCARD")
            sys.exit()

        # if first digit begins with 4 then it is Visa
        elif (str(number)[0] == "4"):
            print("VISA")
            sys.exit()

        # if none of these, then it is invalid
        else:
            print("INVALID")
            sys.exit()

    else:
        print("INVALID")
        sys.exit()


def validate_number(number):
    total = 0
    # we reverse the number - algorithm takes it from the back
    number = number[::-1]

    # then we take every other digit after skiping the first digit
    for num in number[1::2]:

        # we multiply that number by 2
        multiplied = int(num) * 2

        # if the result is two digit number we add each of these digit and add it to the total
        if multiplied >= 10:
            total += 1 + multiplied % 10

        # if not, we are just adding the multiplied result to the total
        else:
            total += multiplied

    # then we go through remaining digits and adding it to the total
    for num in number[::2]:
        total += int(num)

    # lastly, we return true if the result is ending with 0 and false otherwise
    return str(total)[1] == "0" if True else False

main()