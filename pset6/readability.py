from cs50 import get_string

# Getting text input from the user
text = get_string("Text: ")

# counting sentences by looking for dot, question and exclenemation mark
sentences = text.count(".") + text.count("!") + text.count("?")

# whitespaces
white_spaces = text.count(" ")

# would be better just to check with ascii values but this one works as well
letters = len(text) - white_spaces - sentences - text.count(",") - text.count("'")

# including the last word that does not end with a space so + 1
words = white_spaces + 1

# Cloeman liao algorithm
hundered_words_ratio = 100 / words
index = hundered_words_ratio * 0.0588 * letters - hundered_words_ratio * 0.296 * sentences - 15.8

# displaying result
if index > 15:
    print("Grade 16+")

elif index < 1:
    print("Before Grade 1")

else:
    print(f"Grade {round(index)}")