from cs50 import get_int

# asking height of the triangle from the user
height = get_int("Height: ")

# while the height is not in the range that we want we are going to keep asking it from the user
while (height < 1 or height > 8):
    height = get_int("Height: ")

# For loop for rows of our triangle
for i in range(height):

    # first empty spaces
    for k in range(height - i - 1):
        print(" ", end="")

    # first half of the triangle
    for y in range(i + 1):
        print("#", end="")

    # middle empty column
    print("  ", end="")

    # second part of our triangle
    for z in range(i + 1):
        print("#", end="")

    print("")
