#include <cs50.h> // for type string and function get_string()
#include <stdio.h>
#include <stdlib.h> // for atoi()
#include <string.h> // for strlen()

// PROTOTYPE
int check_arg(int argc, string argv[]);
void print_ciphertext(string plaintext, int key);

int main(int argc, string argv[])
{
    // check if command line arguments provided are numeric
    int check_return = check_arg(argc, argv);

    // if not numeric, then exit
    if (check_return)
    {
        return check_return;
    }

    int key = atoi(argv[1]);

    // get user input for plaintext
    string plaintext = get_string("plaintext: ");

    printf("ciphertext: ");
    print_ciphertext(plaintext, key);

}

int check_arg(int argc, string argv[])
{
    if (argc != 2) // check if number of arguments provided (including program name) is 2 (no more, no less)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else
    {
        // Check if argument is a number
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            int isDigit = ((argv[1][i] >= '1') && (argv[1][i] <= '9')) ? 1 : 0;
            if (!isDigit)
            {
                printf("Usage: %s key\n", argv[0]);
                return 1;
            }
        }
    }
    return 0;
}

void print_ciphertext(string plaintext, int key)
{
    // Check if plaintext contains a char
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // check if char is small letter
        int small = ((plaintext[i] >= 'a') && (plaintext[i] <= 'z')) ? 1 : 0;

        // check if char is capital letter
        int capital = ((plaintext[i] >= 'A') && (plaintext[i] <= 'Z')) ? 1 : 0;

        if (small)
        {
            printf("%c", ((plaintext[i] - 'a' + key) % 26) + 'a');
        }
        else if (capital)
        {
            printf("%c", ((plaintext[i] - 'A' + key) % 26) + 'A');
        }
        else
        {
            // print char as it is if char is non-alphabetic
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}