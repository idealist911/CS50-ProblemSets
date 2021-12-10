#include <cs50.h> // for get_float()
#include <math.h> // for round()
#include <stdio.h> // for printf()

float get_change(void);// Get user's input for change

int main(void)
{
    // Get user's input for change
    // Change dollars to cents to get rid of decimals
    int change = round(get_change() * 100);

    int coin_ctr = 0;

    while (change > 0)
    {
        if (change >= 25)
        {
            change -= 25;
        }
        else if (change >= 10)
        {
            change -= 10;
        }
        else if (change >= 5)
        {
            change -= 5;
        }
        else
        {
            // the only other denomination left is 1 cent
            change -= 1;
        }
        coin_ctr++;
    }
    printf("%i\n", coin_ctr);
}

float get_change(void)
{
    // Ask user for change
    // Make sure it is non-negative
    float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);
    return change;
}