#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

bool is_contains_digit(string user_input);
bool is_character_repeated(string user_input);

int main(int argc, string argv[])
{
    string user_input;

    if (argc != 2)
    {
        printf("Usage: %s key \n", argv[0]);
        return 1;
    }

    if (argc == 2)
    {
        if (strlen(argv[1]) < 26)
        {
            printf("Key must contain 26 characters. \n");
            return 1;
        }

        if ((strlen(argv[1]) == 26) &&  is_contains_digit(argv[1]))
        {
            printf("Key must contain only alphabetic characters. \n");
            return 1;
        }

        if (is_character_repeated(argv[1]))
        {
            printf("Key must not contain repeated characters \n");
            return 1;
        }

        else
        {
            user_input = get_string("plaintext: ");
        }
    }


    printf("ciphertext: ");

    for (int i = 0, input_length = strlen(user_input); i < input_length; i++)
    {
        if (user_input[i] >= 'a' && user_input[i] <= 'z')
        {
            int diff = user_input[i] - 'a';
            if (isupper(argv[1][diff]))
            {
                printf("%c", argv[1][diff] + 32);
            }
            else
            {
                printf("%c", argv[1][diff]);
            }
        }
        else if (user_input[i] >= 'A' && user_input[i] <= 'Z')
        {
            int diff = user_input[i] - 'A';
            if (isupper(argv[1][diff]))
            {
                printf("%c", argv[1][diff]);
            }
            else
            {
                printf("%c", argv[1][diff] - 32);
            }
        }
        else
        {
            printf("%c", user_input[i]);
        }
    }
    printf("\n");
}

bool is_contains_digit(string user_input)
{
    for (int i = 0, input_length = strlen(user_input); i < input_length; i++)
    {
        if (isdigit(user_input[i]))
        {
            return true;
        }
    }
    return false;
}

bool is_character_repeated(string user_input)
{
    char temp_input[strlen(user_input)];
    for (int i = 0, input_length = strlen(user_input); i < input_length; i++)
    {
        if(strchr(temp_input, user_input[i]) != NULL)
        {
            return true;
        }

        temp_input[i] = user_input[i];
    }
    return false;
}