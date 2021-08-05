#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>  // for strtol

bool is_valid_number(string user_input);
int main(int argc, string argv[])
{
    if (argc != 2 || !is_valid_number(argv[1]))
    {
        printf("Usage: %s key \n", argv[0]);
        return 1;
    }
    char *p;
    long conv = strtol(argv[1], &p, 10);

    string user_input = get_string("plaintext: ");
    printf("ciphertext: ");

    for (int i = 0, input_length = strlen(user_input); i < input_length; i++)
    {
        if ((user_input[i] >= 'A' && user_input[i] <= 'Z') || (user_input[i] >= 'a' && user_input[i] <= 'z'))
        {
            if (user_input[i] >='a' && user_input[i] <= 'z' && user_input[i] + conv > 'z' )
            {
                int diff = conv - ('z' - user_input[i]);
                int shift = diff % 26;
                if (shift % 26 == 0)
                {
                    shift = 26;
                }
                printf("%c", 'a' + shift - 1);
            }

            else if (user_input[i] >='A' && user_input[i] <= 'Z' && user_input[i] + conv > 'Z' )
            {
                int shift = (user_input[i] + conv) % 'Z';
                printf("%c", 'A' + shift - 1);
            }
            else
            {
                printf("%c", user_input[i] + (int)conv);
            }
        }
        else
        {
            printf("%c", user_input[i]);
        }
    }

    printf("\n");
}

bool is_valid_number(string user_input)
{
    for (int i = 0; i < strlen(user_input); i++)
    {
        if (!isdigit(user_input[i]))
        {
            return false;
        }
    }

    return true;
}