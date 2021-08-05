#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
string to_lower(string word);

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words

    int score1 = compute_score(to_lower(word1));
    int score2 = compute_score(to_lower(word2));

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
        return 0;
    }
    else if (score1 == score2)
    {
        printf("Tie!");
    }
    printf("Player 2 wins!\n");
}

string to_lower(string word)
{
     for(int i = 0; word[i]; i++){
      word[i] = tolower(word[i]);
    }
    return word;
}

int compute_score(string word)
{
    int total = 0;
    for (int i = 0, word_length = strlen(word); i < word_length; i++)
    {
        if (word[i] >= 'a' && word[i] <='z')
        {
            int letter_index = word[i] - 'a';
            total += POINTS[letter_index];
        }
    }
    return total;
}