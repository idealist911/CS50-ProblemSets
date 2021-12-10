#include <cs50.h> // for get_long()
#include <stdio.h>

int get_length_long(long number);
int get_firsttwo_long(long number, int length);
int get_checksum(long credit, int credit_length);

int main(void)
{
    // Get user's credit card number (without hypens)
    long credit;
    do
    {
        credit = get_long("Number: ");
    }
    while (credit < 0);

    int credit_length = get_length_long(credit); // get length of number
    int start = get_firsttwo_long(credit, credit_length);

    //printf("length: %i\n", credit_length);
    //printf("start: %i\n", start);

    int invalid = 1; // turns 0 if valid
    int amex_length = 15;
    int amex_start_1 = 34;
    int amex_start_2 = 37;
    int master_length = 16;
    int master_start_min = 51;
    int master_start_max = 55;
    int visa_length_1 = 13;
    int visa_length_2 = 16;
    int visa_start = 4;
    char type;

    // check start
    if (credit_length == amex_length)
    {
        // check amex
        if (start == amex_start_1 || start == amex_start_2)
        {
            invalid = 0;
            type = 'a';
        }
    }
    if (credit_length == master_length)
    {
        // check master
        if (start >= master_start_min && start <= master_start_max)
        {
            invalid = 0;
            type = 'm';
        }
    }
    if ((credit_length == visa_length_1) || (credit_length == visa_length_2))
    {
        // check visa
        int first = start / 10;
        if (first == visa_start)
        {
            invalid = 0;
            type = 'v';
        }
    }

    // skip this step if already invalidated
    if (invalid == 0)
    {
        int checksum = get_checksum(credit, credit_length);
        //printf("checksum: %i\n", checksum);

        // failure criterion
        int failed = checksum % 10;

        if (failed)
        {
            invalid = 1; // invalidate if failed checksum
        }
        else
        {
            if (type == 'v')
            {
                printf("VISA\n");
            }
            if (type == 'm')
            {
                printf("MASTERCARD\n");
            }
            if (type == 'a')
            {
                printf("AMEX\n");
            }
        }
    }

    if (invalid)
    {
        printf("INVALID\n");
    }

}

int get_length_long(long number)
{
    // Get number of digits in a long
    int num_digits = 1;
    if (number < 0)
    {
        number = -1 * number;
    }
    while (number > 9)
    {
        number /= 10;
        num_digits++;
    }
    return num_digits;
}

int get_firsttwo_long(long number, int length)
{
    int poweroften = length - 2;
    for (int i = 0; i < poweroften; i++)
    {
        number = number / 10;
    }
    int firsttwo = (int) number;
    return firsttwo;
}

int get_checksum(long credit, int credit_length)
{
    // Get checksum of credit card using Luhn's algorithm

    int oddsum = 0;
    int evensum = 0;
    for (int i = 0; i < credit_length; i++)
    {
        int digit = credit % 10;
        credit /= 10;

        // different operations for odd and even powers of 10
        if (i % 2)
        {
            digit *= 2;
            if (digit > 9)
            {
                int digit1 = digit % 10;
                int digit10 = digit / 10;
                oddsum = oddsum + digit1 + digit10;
            }
            else
            {
                oddsum = oddsum + digit;
            }
        }
        else
        {
            evensum = evensum + digit;
        }
    }
    int checksum = oddsum + evensum;
    return checksum;
}