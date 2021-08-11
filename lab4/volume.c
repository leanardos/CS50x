// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
typedef int16_t TWO_BYTE;


// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    BYTE header_bytes[HEADER_SIZE];
    TWO_BYTE sample;

    // In order to change the volume, multiply the sample by a number

    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    fread(&header_bytes, HEADER_SIZE, 1, input);
    fwrite(&header_bytes, HEADER_SIZE, 1, output);

    while (fread(&sample, sizeof sample, 1, input))
    {
        sample *= factor;
        fwrite(&sample, sizeof sample, 1, output);
    }
    // TODO: Read samples from input file and write updated data to output file

    // Close files
    fclose(input);
    fclose(output);
}
