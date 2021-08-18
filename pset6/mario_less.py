from cs50 import get_int

# Getting input from user
height = get_int("Height: ")

# Checking if the input is in the range and if not we are keep asking
while (height < 1 or height > 8):
    height = get_int("Height: ")

# Print loop
for i in range(height):

    # Printing empty part of it
    for k in range(height - i - 1):
        print(" ", end="")

    # Printing
    for y in range(i + 1):

        print("#", end="")
    print()