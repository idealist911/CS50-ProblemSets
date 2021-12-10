#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        // Loop through every pixel
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
                // swap values
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
    // copies image into another array
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

    // copies back into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image2[h][w];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // copies image into another array
    RGBTRIPLE G[height][width];

    // make the change in image2
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int GxRed = 0;
            int GxGreen = 0;
            int GxBlue = 0;

            int GyRed = 0;
            int GyGreen = 0;
            int GyBlue = 0;

            if (h - 1 >= 0)
            {
                GyRed += image[h - 1][w].rgbtRed * -2;
                GyGreen += image[h - 1][w].rgbtGreen * -2;
                GyBlue += image[h - 1][w].rgbtBlue * -2;
            }

            if (h + 1 < height)
            {
                GyRed += image[h + 1][w].rgbtRed * 2;
                GyGreen += image[h + 1][w].rgbtGreen * 2;
                GyBlue += image[h + 1][w].rgbtBlue * 2;
            }

            if (w - 1 >= 0)
            {
                GxRed += image[h][w - 1].rgbtRed * -2;
                GxGreen += image[h][w - 1].rgbtGreen * -2;
                GxBlue += image[h][w - 1].rgbtBlue * -2;
            }

            if (w + 1 < width)
            {
                GxRed += image[h][w + 1].rgbtRed * 2;
                GxGreen += image[h][w + 1].rgbtGreen * 2;
                GxBlue += image[h][w + 1].rgbtBlue * 2;
            }

            // top left
            if ((h - 1 >= 0) && (w - 1 >= 0))
            {
                GxRed += image[h - 1][w - 1].rgbtRed * -1;
                GxGreen += image[h - 1][w - 1].rgbtGreen * -1;
                GxBlue += image[h - 1][w - 1].rgbtBlue * -1;

                GyRed += image[h - 1][w - 1].rgbtRed * -1;
                GyGreen += image[h - 1][w - 1].rgbtGreen * -1;
                GyBlue += image[h - 1][w - 1].rgbtBlue * -1;
            }

            // top right
            if ((h - 1 >= 0) && (w + 1 < width))
            {
                GxRed += image[h - 1][w + 1].rgbtRed * 1;
                GxGreen += image[h - 1][w + 1].rgbtGreen * 1;
                GxBlue += image[h - 1][w + 1].rgbtBlue * 1;

                GyRed += image[h - 1][w + 1].rgbtRed * -1;
                GyGreen += image[h - 1][w + 1].rgbtGreen * -1;
                GyBlue += image[h - 1][w + 1].rgbtBlue * -1;
            }

            // bot left
            if ((h + 1 < height) && (w - 1 >= 0))
            {
                GxRed += image[h + 1][w - 1].rgbtRed * -1;
                GxGreen += image[h + 1][w - 1].rgbtGreen * -1;
                GxBlue += image[h + 1][w - 1].rgbtBlue * -1;

                GyRed += image[h + 1][w - 1].rgbtRed * 1;
                GyGreen += image[h + 1][w - 1].rgbtGreen * 1;
                GyBlue += image[h + 1][w - 1].rgbtBlue * 1;
            }

            // bot right
            if ((h + 1 < height) && (w + 1 < width))
            {
                GxRed += image[h + 1][w + 1].rgbtRed * 1;
                GxGreen += image[h + 1][w + 1].rgbtGreen * 1;
                GxBlue += image[h + 1][w + 1].rgbtBlue * 1;

                GyRed += image[h + 1][w + 1].rgbtRed * 1;
                GyGreen += image[h + 1][w + 1].rgbtGreen * 1;
                GyBlue += image[h + 1][w + 1].rgbtBlue * 1;
            }

            int GRed = round(pow(
                                 pow(GxRed, 2) + pow(GyRed, 2),
                                 0.5));

            int GGreen = round(pow(
                                   pow(GxGreen, 2) + pow(GyGreen, 2),
                                   0.5));

            int GBlue = round(pow(
                                  pow(GxBlue, 2) + pow(GyBlue, 2),
                                  0.5));

            if (GRed > 255)
            {
                GRed = 255;
            }

            if (GGreen > 255)
            {
                GGreen = 255;
            }

            if (GBlue > 255)
            {
                GBlue = 255;
            }

            G[h][w].rgbtRed = (BYTE) GRed;
            G[h][w].rgbtGreen = (BYTE) GGreen;
            G[h][w].rgbtBlue = (BYTE) GBlue;
        }
    }

    // copies back into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = G[h][w];
        }
    }

    return;
}

// Detect edges
// void edges(int height, int width, RGBTRIPLE image[height][width])
// {
//     typedef struct
//     {
//         int  rgbtBlue;
//         int  rgbtGreen;
//         int  rgbtRed;
//     }
//     RGBTRIPLEINT;

//     // creates a variable to store the sum when multiplying with kernel
//     RGBTRIPLEINT Ix;
//     RGBTRIPLEINT Iy;
//     RGBTRIPLEINT I;


//     int Gx[3][3];
//     Gx[0][0] = -1;
//     Gx[0][1] = 0;
//     Gx[0][2] = 1;
//     Gx[1][0] = -2;
//     Gx[1][1] = 0;
//     Gx[1][2] = 2;
//     Gx[2][0] = -1;
//     Gx[2][1] = 0;
//     Gx[2][2] = 1;

//     int Gy[3][3];
//     Gy[0][0] = -1;
//     Gy[0][1] = -2;
//     Gy[0][2] = -1;
//     Gy[1][0] = 0;
//     Gy[1][1] = 0;
//     Gy[1][2] = 0;
//     Gy[2][0] = 1;
//     Gy[2][1] = 2;
//     Gy[2][2] = 1;

//     RGBTRIPLE image2[height][width];


//     for (int h = 0; h < height; h++)
//     {
//         for (int w = 0; w < height; w++)
//         {
//             Ix.rgbtBlue = 0;
//             Ix.rgbtGreen = 0;
//             Ix.rgbtRed = 0;

//             Iy.rgbtBlue = 0;
//             Iy.rgbtGreen = 0;
//             Iy.rgbtRed = 0;

//             I.rgbtBlue = 0;
//             I.rgbtGreen = 0;
//             I.rgbtRed = 0;

//             for (int i = 0; i < 3; i++)
//             {
//                 for (int j = 0; j < 3; j++)
//                 {

//                     if (h - 1 + i >= 0 && h - 1 + i < height && w - 1 + j >= 0 && w - 1 + j < width)
//                     {
//                         Ix.rgbtRed += Gx[i][j] * image[h - 1 + i][w - 1 + j].rgbtRed;
//                         Ix.rgbtGreen += Gx[i][j] * image[h - 1 + i][w - 1 + j].rgbtGreen;
//                         Ix.rgbtBlue += Gx[i][j] * image[h - 1 + i][w - 1 + j].rgbtBlue;

//                         Iy.rgbtRed += Gy[i][j] * image[h - 1 + i][w - 1 + j].rgbtRed;
//                         Iy.rgbtGreen += Gy[i][j] * image[h - 1 + i][w - 1 + j].rgbtGreen;
//                         Iy.rgbtBlue += Gy[i][j] * image[h - 1 + i][w - 1 + j].rgbtBlue;

//                     }
//                 }
//             }

//             I.rgbtRed = round(pow(pow(Ix.rgbtRed, 2) + pow(Iy.rgbtRed, 2),
//                         0.5));
//             I.rgbtGreen = round(pow(pow(Ix.rgbtGreen, 2) + pow(Iy.rgbtGreen, 2),
//                         0.5));
//             I.rgbtBlue = round(pow(pow(Ix.rgbtBlue, 2) + pow(Iy.rgbtBlue, 2),
//                         0.5));


//             if (I.rgbtRed > 255)
//                 I.rgbtRed = 255;

//             if (I.rgbtGreen > 255)
//                 I.rgbtGreen = 255;

//             if (I.rgbtBlue > 255)
//                 I.rgbtBlue = 255;

//             image2[h][w].rgbtRed = (BYTE) I.rgbtRed;
//             image2[h][w].rgbtGreen = (BYTE) I.rgbtGreen;
//             image2[h][w].rgbtBlue = (BYTE) I.rgbtBlue;
//         }
//     }


//     // copies back into image
//     for (int h = 0; h < height; h++)
//     {
//         for (int w = 0; w < width; w++)
//         {
//             image[h][w] = image2[h][w];
//         }
//     }

//     return;
// }