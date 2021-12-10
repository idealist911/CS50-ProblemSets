#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through every pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Gray pixels have same values for their R, G, and B values
            // Find average of R, G, and B values
            BYTE average = round((image[h][w].rgbtRed
                                  + image[h][w].rgbtGreen
                                  + image[h][w].rgbtBlue) / (float) 3);

            // Fill the R, G, and B values with the average
            image[h][w].rgbtRed = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtBlue = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            /* Set type as int
             * because BYTE has max value of 255
             * so if type is BYTE, number will loop
             * if exceed 255
             */

            // Use an algorithm to calculate sepia RGB from current RGB
            int sepiaRed = round(.393 * image[h][w].rgbtRed
                                 + .769 * image[h][w].rgbtGreen
                                 + .189 * image[h][w].rgbtBlue);

            int sepiaGreen = round(.349 * image[h][w].rgbtRed
                                   + .686 * image[h][w].rgbtGreen
                                   + .168 * image[h][w].rgbtBlue);

            int sepiaBlue = round(.272 * image[h][w].rgbtRed
                                  + .534 * image[h][w].rgbtGreen
                                  + .131 * image[h][w].rgbtBlue);

            // RGB values cannot exceed 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // Fill the R, G, and B values with sepia versions
            image[h][w].rgbtRed = (BYTE) sepiaRed;
            image[h][w].rgbtGreen = (BYTE) sepiaGreen;
            image[h][w].rgbtBlue = (BYTE) sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Find the halfway vertical line
    // If width is odd, left half is smaller than right half
    int half = (width % 2 == 0) ? width / 2 : (width - 1) / 2;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < half; w++)
        {
            int equalRed = (image[h][w].rgbtRed
                            == image[h][width - 1 - w].rgbtRed)
                           ? 1 : 0;

            int equalGreen = (image[h][w].rgbtGreen
                              == image[h][width - 1 - w].rgbtGreen)
                             ? 1 : 0;

            int equalBlue = (image[h][w].rgbtBlue
                             == image[h][width - 1 - w].rgbtBlue)
                            ? 1 : 0;

            // If R, G, and B values are equal, we don't have to swap
            if (!(equalRed && equalGreen && equalBlue))
            {
                // Swap values
                RGBTRIPLE tmp = image[h][w];
                image[h][w] = image[h][width - 1 - w];
                image[h][width - 1 - w] = tmp;
            }
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copies image into another array
    RGBTRIPLE image2[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image2[h][w] = image[h][w];
        }
    }


    // Make the changes in image2
    // Reference original image for the RGB values
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int valueRed = image[h][w].rgbtRed;
            int valueGreen = image[h][w].rgbtGreen;
            int valueBlue = image[h][w].rgbtBlue;
            int num = 1; // number of boxes considered

            if (h - 1 >= 0)
            {
                valueRed += image[h - 1][w].rgbtRed;
                valueGreen += image[h - 1][w].rgbtGreen;
                valueBlue += image[h - 1][w].rgbtBlue;
                num++;
            }

            if (h + 1 < height)
            {
                valueRed += image[h + 1][w].rgbtRed;
                valueGreen += image[h + 1][w].rgbtGreen;
                valueBlue += image[h + 1][w].rgbtBlue;
                num++;
            }

            if (w - 1 >= 0)
            {
                valueRed += image[h][w - 1].rgbtRed;
                valueGreen += image[h][w - 1].rgbtGreen;
                valueBlue += image[h][w - 1].rgbtBlue;
                num++;
            }

            if (w + 1 < width)
            {
                valueRed += image[h][w + 1].rgbtRed;
                valueGreen += image[h][w + 1].rgbtGreen;
                valueBlue += image[h][w + 1].rgbtBlue;
                num++;
            }

            // top left
            if ((h - 1 >= 0) && (w - 1 >= 0))
            {
                valueRed += image[h - 1][w - 1].rgbtRed;
                valueGreen += image[h - 1][w - 1].rgbtGreen;
                valueBlue += image[h - 1][w - 1].rgbtBlue;
                num++;
            }

            // top right
            if ((h - 1 >= 0) && (w + 1 < width))
            {
                valueRed += image[h - 1][w + 1].rgbtRed;
                valueGreen += image[h - 1][w + 1].rgbtGreen;
                valueBlue += image[h - 1][w + 1].rgbtBlue;
                num++;
            }

            // bot left
            if ((h + 1 < height) && (w - 1 >= 0))
            {
                valueRed += image[h + 1][w - 1].rgbtRed;
                valueGreen += image[h + 1][w - 1].rgbtGreen;
                valueBlue += image[h + 1][w - 1].rgbtBlue;
                num++;
            }

            // bot right
            if ((h + 1 < height) && (w + 1 < width))
            {
                valueRed += image[h + 1][w + 1].rgbtRed;
                valueGreen += image[h + 1][w + 1].rgbtGreen;
                valueBlue += image[h + 1][w + 1].rgbtBlue;
                num++;
            }

            image2[h][w].rgbtRed = (BYTE) round(valueRed / (float) num);
            image2[h][w].rgbtGreen = (BYTE) round(valueGreen / (float) num);
            image2[h][w].rgbtBlue = (BYTE) round(valueBlue / (float) num);
        }
    }

    // Copies back into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image2[h][w];
        }
    }

    return;
}
