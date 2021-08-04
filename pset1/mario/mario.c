#include <stdio.h>
#include <cs50.h>

void mario_recursive(int block_size);

int main(void)
{

    // Ask for the block size until it is in the valid range.
    int user_input;
    do
    {
        user_input = get_int("Please enter block size: ");
    }
    while (user_input < 1 || user_input > 8);


    // mario_recursive(user_input);
    for (int i = user_input; i > 0; i--)
    {
        // First triangle
        for (int y = 0; y < i - 1; y++)
        {
            printf("-");
        }

        for (int k = 0; k < user_input - i + 1; k++)
        {
            printf("#");
        }

        // Gap between the second triangle
        printf("^^");

        // Second triangle
        for (int z = 0; z < user_input - i + 1; z++)
        {
            printf("#");
        }

        printf("\n");
    }
}

void mario_recursive(int block_size)
{
    int const_block_size = block_size;

    if (block_size != 1)
    {
        mario_recursive(block_size - 1);
    }
    
    for (int y = 0; y < block_size - 1; y++)
    {
        printf("-");
    }

    for (int k = 0; k < block_size -1; k++)
    {
        printf("#");
    }

    // Gap between the second triangle
    printf("  ");

    // Second triangle
    for (int z = 0; z < block_size  + 1; z++)
    {
        printf("#");
    }

    printf("\n");
}