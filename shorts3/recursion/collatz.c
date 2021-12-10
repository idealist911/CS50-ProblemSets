#include <cs50.h>
#include <stdio.h>

// PROTOTYPE
int collatz(int n);

int main(void)
{
    int n = get_int("Give integer: ");
    int steps = collatz(n);
    printf("Steps: %i\n", steps);
}

int collatz(int n)
{
    // base case
    if (n == 1)
    {
        return 0;
    }

    // if even
    if (!(n%2))
    {
        return 1 + collatz(n/2);
    }
    // if odd
    else
    {
        return 1 + collatz(3*n+1);
    }
}