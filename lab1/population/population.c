#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int start_min = 9;
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < start_min);

    // Prompt for end size
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);

    // Calculate number of years until we reach threshold
    int year = 0;
    int pop = start;
    while (pop < end)
    {
        float birth = (float) pop / 3; // birth rate
        float death = (float) pop / 4; // death rate
        pop = pop + (int) birth - (int) death;
        year++;
    }

    // Print number of years
    printf("Years: %i\n", year);
}