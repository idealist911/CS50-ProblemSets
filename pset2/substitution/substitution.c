#include <cs50.h> // for type string, for get_string()
#include <ctype.h> // for tolower()
#include <stdio.h> // for printf()
#include <string.h> // for strlen()

// PROTOTYPE
int check_arg(int argc, string argv[]);
void print_ciphertext(string plaintext, string key);
string renormalize_string(string word);

int main(int argc, string argv[])
{
    // check if argument fit condition
    int checkval = check_arg(argc, argv);

    if (checkval)
    {
        return 1;
    }

    // get user input for plaintext
    string plaintext = get_string("plaintext: ");

    string key = argv[1];
    key = renormalize_string(key);
    printf("ciphertext: ");
    print_ciphertext(plaintext, key);
}

int check_arg(int argc, string argv[])
{
    // check if argument fit condition:
    // 1. 2 arguments (including name)
    // 2. all alphabets
    // 3. 26 characters

    int failed = 0;

    // it must be if-else condition, otherwise, will produce seg fault when argc == 1
    if (argc != 2)
    {
        failed = 1;
    }
    else
    {
        int length = strlen(argv[1]);

        // check if exists 26 chars
        if (length != 26)
        {
            failed = 2;
        }
        // check if all 26 chars are alphabets
        else
        {
            for (int i = 0; i < length; i++)
            {
                int small = ((argv[1][i] >= 'a') && (argv[1][i] <= 'z')) ? 1 : 0;
                int capital = ((argv[1][i] >= 'A') && (argv[1][i] <= 'Z')) ? 1 : 0;

                if (!small && !capital)
                {
                    failed = 3;
                    break;
                }
            }
        }

        if (!failed)
        {
            // Check repeats
            for (int i = 0; i < length; i++)
            {
                char character = tolower(argv[1][i]);
                for (int j = 0; j < length; j++)
                {
                    if ((character == tolower(argv[1][j])) && (i != j))
                    {
                        failed = 4;
                        break;
                    }
                }
            }
        }

    }

    switch (failed)
    {
        case 1:
        {
            printf("Usage: %s key\n", argv[0]);
            break;
        }
        case 2:
        {
            printf("Key must contain 26 characters\n");
            break;
        }
        case 3:
        {
            printf("Key must only contain alphabetic characters\n");
            break;
        }
        case 4:
        {
            printf("Key must not contained repeated characters\n");
            break;
        }
        default:
        {
            return 0;
        }
    }
    return 1;
}

void print_ciphertext(string plaintext, string key)
{
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // check if char is small letter
        int small = ((plaintext[i] >= 'a') && (plaintext[i] <= 'z')) ? 1 : 0;

        // check if char is capital letter
        int capital = ((plaintext[i] >= 'A') && (plaintext[i] <= 'Z')) ? 1 : 0;

        if (small)
        {
            int index = (plaintext[i] - 'a');
            printf("%c", key[index]);
        }
        else if (capital)
        {
            int index = (plaintext[i] - 'A');
            printf("%c", (key[index] - 'a' + 'A'));
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }

    printf("\n");
}

string renormalize_string(string word)
{

    int length = strlen(word);

    // string is alphabetic only
    for (int i = 0; i < length; i++)
    {
        // check if char is small letter
        int small = ((word[i] >= 'a') && (word[i] <= 'z')) ? 1 : 0;

        // check if char is capital letter
        int capital = ((word[i] >= 'A') && (word[i] <= 'Z')) ? 1 : 0;

        if (small || capital)
        {
            word[i] = tolower(word[i]);
        }
    }

    return word;

}