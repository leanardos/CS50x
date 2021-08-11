#include "helpers.h"
#include "stdio.h"
#include "math.h"
#include <stdbool.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Getting the average of pixel values and making sure we are rounding it so that we have the correctly rounded int in the end
            int average_pixel_value = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = average_pixel_value;
            image[h][w].rgbtGreen = average_pixel_value;
            image[h][w].rgbtBlue = average_pixel_value;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Storing the reflected image in the temp.
            temp[h][width - w - 1].rgbtRed = image[h][w].rgbtRed;
            temp[h][width - w - 1].rgbtGreen = image[h][w].rgbtGreen;
            temp[h][width - w - 1].rgbtBlue = image[h][w].rgbtBlue;
        }

        for (int w = 0; w < width; w++)
        {
            // Updating the original image pixels with the reflected version of it.
            image[h][w].rgbtRed = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
            image[h][w].rgbtBlue = temp[h][w].rgbtBlue;
        }
    }

    return;
}

// Struct for our neighbour. I used it more like tuple purposes
typedef struct
{
    int height;
    int width;
} Neighbour;

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Similar to Tuple to go all the possible neighbours
    Neighbour neighbours[9] =
    {
        { .height = 0, .width = 0 },
        { .height = 0, .width = 1 },
        { .height = 1, .width = 0 },
        { .height = 1, .width = 1 },
        { .height = -1, .width = 0 },
        { .height = 0, .width = -1 },
        { .height = -1, .width = -1 },
        { .height = -1, .width = 1 },
        { .height = 1, .width = -1 },
    };

    RGBTRIPLE temp[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Every iteration I need to reset my pixel counter and calculated height and width variables so that it wont collide with the previous values.
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;

            int newHeight = 0;
            int newWidth = 0;

            // Made it float since the calculated value will be float as well
            float neighbour_count = 0;
            for (int i = 0; i < 9; i++)
            {
                bool is_outside = (h + neighbours[i].height) < 0 || (h + neighbours[i].height) > (height - 1) || (w + neighbours[i].width) < 0
                                  || (w + neighbours[i].width) > (width - 1);

                if (!is_outside)
                {
                    newHeight = h + neighbours[i].height;
                    newWidth =  w + neighbours[i].width;
                    neighbour_count++;

                    totalRed += image[newHeight][newWidth].rgbtRed;
                    totalGreen += image[newHeight][newWidth].rgbtGreen;
                    totalBlue += image[newHeight][newWidth].rgbtBlue;
                }
            }

            // First assigning it to the temp image because I would need the original pixel values for the other pixels as well.
            // Making sure that we are using round method to round the floating values
            temp[h][w].rgbtRed = round(totalRed / neighbour_count);
            temp[h][w].rgbtGreen = round(totalGreen / neighbour_count);
            temp[h][w].rgbtBlue = round(totalBlue / neighbour_count);
        }
    }

    // Finally assigning temp to original image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
            image[h][w].rgbtBlue = temp[h][w].rgbtBlue;
        }
    }

    return;
}


typedef struct
{
    int height;
    int width;
    int gx;
    int gy;
} Gx_Gy_Neighbour;

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    Gx_Gy_Neighbour neighbours[9] =
    {
        { .height = 0, .width = 0, .gx = 0, .gy = 0 },
        { .height = 0, .width = 1, .gx = 2, .gy = 0 },
        { .height = 1, .width = 0, .gx = 0, .gy = 2 },
        { .height = 1, .width = 1, .gx = 1, .gy = 1 },
        { .height = -1, .width = 0, .gx = 0, .gy = -2 },
        { .height = 0, .width = -1, .gx = -2, .gy = 0 },
        { .height = -1, .width = -1, .gx = -1, .gy = -1 },
        { .height = -1, .width = 1, .gx = 1, .gy = -1 },
        { .height = 1, .width = -1, .gx = -1, .gy = 1 },
    };

    RGBTRIPLE temp[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Every iteration I need to reset my pixel and calculated height and width variables so that it wont collide with the previous values.
            double gxRed = 0;
            double gxGreen = 0;
            double gxBlue = 0;

            double gyRed = 0;
            double gyGreen = 0;
            double gyBlue = 0;


            int newHeight = 0;
            int newWidth = 0;

            for (int i = 0; i < 9; i++)
            {
                // Here I am checking if I am outside of the image when moving to the neighbour pixels
                bool is_outside = (h + neighbours[i].height) < 0 || (h + neighbours[i].height) > (height - 1) || (w + neighbours[i].width) < 0
                                  || (w + neighbours[i].width) > (width - 1);

                // I am calculating the new pixel values only if I am inside the image borders
                if (!is_outside)
                {
                    newHeight = h + neighbours[i].height;
                    newWidth =  w + neighbours[i].width;

                    gxRed += image[newHeight][newWidth].rgbtRed * neighbours[i].gx;
                    gxGreen += image[newHeight][newWidth].rgbtGreen * neighbours[i].gx;
                    gxBlue += image[newHeight][newWidth].rgbtBlue * neighbours[i].gx;


                    gyRed += image[newHeight][newWidth].rgbtRed * neighbours[i].gy;
                    gyGreen += image[newHeight][newWidth].rgbtGreen * neighbours[i].gy;
                    gyBlue += image[newHeight][newWidth].rgbtBlue * neighbours[i].gy;
                }
            }

            // First assigning it to the temp image because I would need the original pixel values for the other pixels as well.
            // Making sure that we are using round method to round the floating values

            double calculatedRed = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            double calculatedGreen = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            double calculatedBlue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            temp[h][w].rgbtRed = calculatedRed > 255 ? 255 : calculatedRed;
            temp[h][w].rgbtGreen = calculatedGreen > 255 ? 255 : calculatedGreen;
            temp[h][w].rgbtBlue = calculatedBlue > 255 ? 255 : calculatedBlue;
        }
    }

    // Finally assigning temp to original image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
            image[h][w].rgbtBlue = temp[h][w].rgbtBlue;
        }
    }

    return;
}
