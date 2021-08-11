#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*TODO
    Open memory card
    Look for beginning of a JPEG
    Open a new JPEG file
    Write 512 Bytes until a new JPEG is found
*/

#define BUFFER_SIZE 512

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{

    // ------- Asking input from the user, validating it and checking if the file exists

    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    FILE *card_raw = fopen(filename, "r"); // One of the ways of opening a file is to use fopen:

    if(card_raw == NULL)
    {
        printf("Error occured while reading the file\n");
        return 1;
    }




    // ------- Reading from the file

    // BYTE *buffer[BUFFER_SIZE]; // variable to store the data that we read from the file into 512 bytes of block
    unsigned char buffer[BUFFER_SIZE];
    char *img_filename = malloc(8);
    int img_counter = 0;
    FILE *img = NULL;
    while(fread(&buffer, BUFFER_SIZE, 1, card_raw) == 1)
    {
        // Every JPEG file starts with a same header squence. First 3 bytes of that squence always the same. They are: 0xff, 0xd8, 0xff
        // And the last byte of this header can be something from 0xe0 to 0exf.
        // Bitwise arithmetic => Where we are taking bitwise & of buffer[3] and 0xf0. What that's going to do is it's going to say, ...
        // .. just look at the first four bits of this 8-bit byte and set the remaining four bits to 0
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (img_counter != 0)
            {
                fclose(img);
            }
            sprintf(img_filename, "%03i.jpg", img_counter);
            img = fopen(img_filename, "w");
            //fwrite(&buffer, 512, 1, img);
            img_counter++;
        }

        if (img_counter != 0)
        {
            fwrite(&buffer, 512, 1, img);
        }

    }

    fclose(card_raw);

    return 0;
}