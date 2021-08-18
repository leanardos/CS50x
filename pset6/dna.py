# libraries

import sys
import csv
import numpy as np
from collections import Counter

# print(sys.argv[1])

# path for the database and sequences txt file
database_path = sys.argv[1]
sequences_path = sys.argv[2]

# database dictionary array
db_dictionary = []

# variable for storing the str sequences
sequence_data = ""

# storing the repeated str counts in the sequence
counts = []


def main():

    # opening the database csv file and adding the lines as a dictionary to our array
    with open(database_path) as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
            db_dictionary.append(dict(row))

    # Doing the same for the sequence
    with open(sequences_path) as f:
        lines = f.readlines()
        sequence_data = lines[0]

    # print(sequence_data)

    # checking the the key is not "name" then it is STR so we are calling our str_counts() function which finds the consicutively repeated counts
    for key in db_dictionary[0]:
        if key != "name":
            counts.append(str_counts(key, sequence_data))
        # print(key)

    # we then declare a variable for our user's STR counts
    user_counts = []
    user = ""

    # and we iterate through the dictionary but this time not for the values but for the keys
    for elements in db_dictionary:

        # and we iterate through each value in each values element
        for values in elements.values():

            # if the value is numeric then it is the count of user's STR so we are adding it to our array
            if values.isnumeric():
                user_counts.append(int(values))

            # if not we are adding it to user variable which holds the name of that user.
            # We need this variable since we have to print the name of the user if we find a match
            else:
                user = values

        # If these two arrays match each other then we print the user and return from the main
        if bool(np.asarray(user_counts == counts).all()):
            print(user)
            return

        user_counts = []

    print("No match")


def str_counts(str_characters, sequences):

    # these are our cursor variable
    i = 0
    j = len(str_characters)

    length = len(str_characters)
    count = 0
    temp_count = 0

    # while we reach end of the word we repeat the process
    while j < len(sequences):

        # if the part of a sequence is equeal to the STR characters
        if sequences[i:j] == str_characters:
            #print(f"Found at index {i}")

            # we increase the temp count
            temp_count += 1

            # we shift the cursor by the length of a STR
            i += length
            j += length

            # we update the count if we find greater number
            if count < temp_count:
                count = temp_count

        # if we break the consecutiveness we set the temp count to 0 and just increase the cursor by one
        else:
            i += 1
            j += 1
            temp_count = 0

    #print(f"{str_characters} repeated {count} times")

    return count


main()
