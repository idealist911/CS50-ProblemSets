#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Typedef
typedef uint8_t BYTE;

// Constants
const int BLOCK_SIZE = 512; // Number of bytes in block
const BYTE BYTE_0 = 0xff;
const BYTE BYTE_1 = 0xd8;
const BYTE BYTE_2 = 0xff;
const BYTE BYTE_3_START = 0xe0;
const BYTE BYTE_3_END = 0xef;

// Function Prototypes
int meet_condition(BYTE *buffer);

int main(int argc, char *argv[])
{
    // Reject wrong usage
    if (argc != 2)
    {
        printf("Usage: ./recover [file]");
        return 1;
    }

    // Save filename
    char *filename = argv[1];

    // Open file
    FILE *file = fopen(filename, "r");

    BYTE *buffer = malloc(BLOCK_SIZE);
    int n = 0; // index of out files
    FILE *outfile = NULL;

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file))
    {
        if (meet_condition(buffer) == 0)
        {
            if (n > 0)
            {
                fclose(outfile);
            }

            char *outfilename = malloc(7);
            sprintf(outfilename, "%03i.jpg", n);
            outfile = fopen(outfilename, "w");
            free(outfilename);


            if (outfile == NULL)
            {
                printf("Could not open file %s.\n", outfilename);
                return 1;
            }

            n++;
        }

        // if n is still 0, then it has not open first file
        if (n > 0)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, outfile);
        }
    }

    free(buffer);
    fclose(outfile); // close last file


}

int meet_condition(BYTE *buffer)
{
    if (buffer[0] == BYTE_0 && buffer[1] == BYTE_1 && buffer[2] == BYTE_2)
    {
        for (BYTE i = BYTE_3_START; i <= BYTE_3_END; i++)
        {
            if (buffer[3] == i)
            {
                return 0;
            }
        }
    }

    return 1;
}