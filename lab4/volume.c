// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h> // for fread(), fwrite()
#include <stdlib.h> // for atof()

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
const int BUFFER_SIZE = 10;


// Typedefs
typedef uint8_t BYTE;
typedef int16_t TWO_BYTE;

int main(int argc, char *argv[])
{
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
    BYTE header[HEADER_SIZE];
    fread(&header, sizeof(BYTE), HEADER_SIZE, input);
    fwrite(&header, sizeof(BYTE), HEADER_SIZE, output);

    // TODO: Read samples from input file and write updated data to output file
    TWO_BYTE *buffer = malloc(BUFFER_SIZE);

    while (fread(buffer, sizeof(TWO_BYTE), BUFFER_SIZE, input))
    {
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            buffer[i] = buffer[i] * factor;
        }

        fwrite(buffer, sizeof(TWO_BYTE), BUFFER_SIZE, output);
    }
    free(buffer);

    // Close files
    fclose(input);
    fclose(output);
}
