#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //variables
    int start_size, end_size, year = 0;

    //Getting inputs from user until it satisfies the condtion
    do
    {
        start_size = get_int("Start size: ");
    }
    while (start_size < 9);

    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < 9 || end_size < start_size);

    while (start_size < end_size)
    {
        //Calculating the difference
        int diff = start_size / 3 - start_size / 4;
        start_size += diff;
        year++;
    }

    //Printing result.
    printf("Years: %i\n", year);
}
