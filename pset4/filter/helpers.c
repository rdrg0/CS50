#include "helpers.h"
#include <math.h>


// Returns 255 if input is greater then 255. Used in edges.
int maxrgb(float a)
{

    if (round(a) >= 255)
    {
        return 255;
    }
    else
    {
        return round(a);
    }
}

// Convert image to grayscale // Averages all values and assignes the average to all 3 colors
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0; 
            int ravg = round(avg);
            image[i][j].rgbtBlue = ravg;
            image[i][j].rgbtGreen = ravg;
            image[i][j].rgbtRed = ravg;

        }
    }
    return;
}

// Reflect image horizontally. Uses swap0 to store bit info temporarily and swap pixels horizontally.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    float halfw = width / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < halfw ; j++)
        {
            RGBTRIPLE swap0 = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = swap0;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width]; // stores pixels temporarily so that the original image is not changed until all the averages are calculated

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float counter = 0.0; // Counts how many pixels are summed in order to get the average.
            float sumB = 0; 
            float sumG = 0;
            float sumR = 0;

            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    if ((k >= 0) && (k < height) && (l >= 0) && (l < width))
                    {
                        counter++;
                        sumB += image[k][l].rgbtBlue; // Valid pixels are summed on each iteration
                        sumG += image[k][l].rgbtGreen;
                        sumR += image[k][l].rgbtRed;
                    }

                }
            }

            temp[i][j].rgbtBlue = round(sumB / counter); // Blured pixels are stored in temporal array
            temp[i][j].rgbtGreen = round(sumG / counter);
            temp[i][j].rgbtRed = round(sumR / counter);

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue; // Blured pixels are assigned in order into original image
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int counter = 0;
            int blue;
            int green;
            int red;

            int gxB = 0;
            int gxG = 0;
            int gxR = 0;

            int gyB = 0;
            int gyG = 0;
            int gyR = 0;

            int Gx[3][3] =  // 3x3 matrix used to process each pixel aka "kernel"
            {

                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1},
            };

            int Gy[3][3] =  // 3x3 matrix used to process each pixel aka "kernel"
            {

                {-1, -2, -1},
                { 0,  0,  0},
                { 1,  2,  1},
            };

            float rB;
            float rG;
            float rR;

            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    if ((k >= 0) && (k < height) && (l >= 0) && (l < width))
                    {
                        blue = image[k][l].rgbtBlue;
                        green = image[k][l].rgbtGreen;
                        red = image[k][l].rgbtRed;
                        counter++;
                    }
                    else
                    {
                        blue = 0;
                        green = 0;
                        red = 0;
                        counter++;
                    }
// For each pixel, all the inmediatly surrounding pixels and the pixel itself are multiplied by the corresponding number in the kernel
                    gxB = gxB + Gx[k - i + 1][l - j + 1] * blue; 
                    gxG = gxG + Gx[k - i + 1][l - j + 1] * green;
                    gxR = gxR + Gx[k - i + 1][l - j + 1] * red;

                    gyB = gyB + Gy[k - i + 1][l - j + 1] * blue;
                    gyG = gyG + Gy[k - i + 1][l - j + 1] * green;
                    gyR = gyR + Gy[k - i + 1][l - j + 1] * red;


                }
            }

            rB = sqrt(pow(gxB, 2) + pow(gyB, 2)); 
            rG = sqrt(pow(gxG, 2) + pow(gyG, 2));
            rR = sqrt(pow(gxR, 2) + pow(gyR, 2));


            temp[i][j].rgbtBlue = maxrgb(rB); // The processed pixels are stored in a temporary array
            temp[i][j].rgbtGreen = maxrgb(rG);
            temp[i][j].rgbtRed = maxrgb(rR);

        }
    }

    for (int i = 0; i < height; i++)  // Processed pixels are assigned in order to the original image.
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}
