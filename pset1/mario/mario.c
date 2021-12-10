#include <cs50.h>
#include <stdio.h>

int get_user_input(void); // Get user input for height of pyramid
void build_pyramid(int h); // Build pyramid from user's input of height

int main(void)
{
    // Get user input for height of pyramid
    int height = get_user_input();

    // Build pyramid
    build_pyramid(height);
}

int get_user_input(void)
{
    // Restrict height to height_min < height < height_max
    int height;
    int height_min = 1;
    int height_max = 8;
    do
    {
        height = get_int("Height: ");
    }
    while ((height < height_min) || (height > height_max));

    return height;
}

void build_pyramid(int height)
{
    // input: height -- user's input of height

    // building blocks
    char brick = '#';
    char gap = ' ';

    // gap distance
    int gap_dist = 2;

    /* i: for each row
     * j: for each column
     */
    for (int i = 0; i < height; i++)
    {
        // build left tower
        for (int j = 0; j < height; j++)
        {
            if (j >= (height - i - 1))
            {
                printf("%c", brick);
            }
            else
            {
                printf("%c", gap);
            }
        }

        // build gap
        for (int j = 0; j < gap_dist; j++)
        {
            printf("%c", gap);
        }

        // build right tower
        for (int j = 0; j < height; j++)
        {
            if (j <= i)
            {
                printf("%c", brick);
            }
            //else
            //{
            //    printf("%c", gap);
            //}
        }
        printf("\n");
    }
}