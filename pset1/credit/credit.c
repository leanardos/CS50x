#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool is_valid_number(string user_input);
string get_credit_kind(string user_input);
bool is_american_express(int first_two_digits);
bool is_mastercard(int first_two_digits);
bool is_visa(int first_digit);
int get_first_n_digits(string user_input, int n_digit);
bool is_legit_with_luhn_algo(string user_input);

int main(void)
{
    string user_input;

    do
    {
        user_input = get_string("Please enter credit card number: ");
    }
    while(!is_valid_number(user_input));

    if (strlen(user_input) > 12)
    {
        printf("%s\n", get_credit_kind(user_input));
    }
    else
    {
        printf("INVALID\n");
    }
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

string get_credit_kind(string user_input)
{
    int first_two_digits = get_first_n_digits(user_input, 2);
    bool is_legit = is_legit_with_luhn_algo(user_input);

    if (is_legit)
    {
        if (strlen(user_input) == 13)
        {
            if (is_visa(get_first_n_digits(user_input, 1)))
            {
                return "VISA";
            }
        }
        if (strlen(user_input) == 15)
        {
            if (is_american_express(first_two_digits))
            {
                return "AMEX";
            }
        }
        if (strlen(user_input) == 16)
        {
            if (is_mastercard(first_two_digits))
            {
                return "MASTERCARD";
            }
            if (is_visa(get_first_n_digits(user_input, 1)))
            {
                return "VISA";
            }
        }
        return "INVALID";
    }
    return "INVALID";
}

bool is_american_express(int first_two_digits)
{

    if (first_two_digits == 34 || first_two_digits == 37)
    {
        return true;
    }
    return false;
}

bool is_mastercard(int first_two_digits)
{

    if (first_two_digits > 50 && first_two_digits < 56)
    {
        return true;
    }
    return false;
}

bool is_visa(int first_digit)
{

    if (first_digit == 4)
    {
        return true;
    }
    return false;
}

int get_first_n_digits(string user_input, int n_digit)
{
    static char result[3];
    strncpy(result, user_input, n_digit);
    result[n_digit] = '\0';
    return atoi(result);
}

bool is_legit_with_luhn_algo(string user_input)
{
    long long int long_input = atoll(user_input);

    int multiply_total = 0;
    int sum_total = 0;

    int cycle = 0;
    while (long_input > 0)
    {
        int current_digit = long_input % 10;

        if (cycle % 2 != 0)
        {
            int multiply_by_two = current_digit * 2;
            if (multiply_by_two > 9)
            {
                int modulo_by_10 = multiply_by_two % 10;
                multiply_total += 1 + modulo_by_10;
            }
            else
            {
                multiply_total += multiply_by_two;
            }
        }
        else
        {
            sum_total += current_digit;
        }

        long_input /= 10;
        cycle++;
    }

    if ((multiply_total + sum_total) % 10 == 0)
    {
        return true;
    }
    return false;
}
