/*********************************************************************
File:     chromakey.cpp


Author:   Haotong Fu


Email address: fum@usc.edu



Notes:
This program performs the chroma key operation on an input
using two different methods.


Method 1 Utilize a user-defined distance threshold from the
         chromakey value as a discriminator


Method 2 Devise a method that to determine the chromakey mask
         that doesn't require a user-input threshold


********************************************************************/


#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "bmplib.h"


using namespace std;


void method1(unsigned char inImage[][SIZE][RGB],
      bool mask[][SIZE],
      double threshold);


void method2(unsigned char inImage[][SIZE][RGB],
      bool mask[][SIZE]);


void replace(bool mask[][SIZE],
      unsigned char inImage[][SIZE][RGB],
      unsigned char bgImage[][SIZE][RGB],
      unsigned char outImage[][SIZE][RGB]);


int main(int argc, char *argv[])
{

 // Image data array
 static unsigned char inputImage[SIZE][SIZE][RGB];
 static unsigned char bgrndImage[SIZE][SIZE][RGB];
 static unsigned char outputImage[SIZE][SIZE][RGB];
 static bool chromaMask[SIZE][SIZE];


 double threshold;


 if (argc < 6) {
   cerr << "usage: program_name in.bmp background.bmp dist_threshold "
        << "out1.bmp out2.bmp" << endl;
   return 0;
 }
  if (readRGBBMP(argv[1], inputImage)) {
   cerr << "Error reading file: " << argv[1] << endl;
   return 1;
 }


 if (readRGBBMP(argv[2], bgrndImage)) {
   cout << "Error reading file: " << argv[2] << endl;
   return 1;
 }
  // Write code to convert the threshold (argv[3])
 //  from string format to a double and assign the 'threshold'
 threshold = atof(argv[3]);


 // Call Method 1 Function
 method1(inputImage, chromaMask, threshold);


 // Produce the output by calling replace()
 replace(chromaMask, inputImage, bgrndImage, outputImage);


 // Write the output image to a file using the filename argv[4]
 if (writeRGBBMP(argv[4], outputImage)) {
   cout << "Error writing file: " << argv[4] << endl;
   exit(1);
 }


 // Call Method 2 Function
 method2(inputImage, chromaMask);


 // Produce the output by calling replace()
 replace(chromaMask, inputImage, bgrndImage, outputImage);


 // Write the output image to a file using the filename argv[5]
 if (writeRGBBMP(argv[5], outputImage)) {
   cout << "Error writing file: " << argv[5] << endl;
   exit(1);
 }


 return 0;
}






// Use user-provided threshold for chroma-key distance
// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method1(unsigned char inImage[][SIZE][RGB],
      bool mask[][SIZE],
      double threshold)
{
 //below is the number of pixels that I will use to
 //calculate the average for each color
 const int num = 256 + 256;


 //calculating the average value for red for the first row
 //and first column on the first plane
 int red_row_sum = 0;
 int red_col_sum = 0;
 for(int i = 0; i < SIZE; i++)
 {
   red_row_sum += int(inImage[0][i][0]);
   red_col_sum += int(inImage[i][0][0]);
 }
 int red_sum = red_row_sum + red_col_sum;
 double red_average = red_sum / (double) num;


 //calculating the average value for green for the first row
 //and first column on the second plane
 int green_row_sum = 0;
 int green_col_sum = 0;
 for(int i = 0; i < SIZE; i++)
 {
   green_row_sum += int(inImage[0][i][1]);
   green_col_sum += int(inImage[i][0][1]);
 }
 int green_sum = green_row_sum + green_col_sum;
 double green_average = green_sum / (double) num;


 //calculating the average value for blue for the first row
 //and first column on the thrid plane
 int blue_row_sum = 0;
 int blue_col_sum = 0;
 for(int i = 0; i < SIZE; i++)
 {
   blue_row_sum += int(inImage[0][i][2]);
   blue_col_sum += int(inImage[i][0][2]);
 }
 int blue_sum = blue_row_sum + blue_col_sum;
 double blue_average = blue_sum / (double) num;


 //the code below calculates the distance between each pixel's
 //corresponding RGB values and the average values for each color
 //with the distance formula: sqrt((x-x0)^2+(y-y0)^2+(z-z0)^2)
 //Then, by comparing the distance of each pixel with the threshold,
 //we can assign either 1 or 0 to the mask 2d array
 for(int i = 0; i < SIZE; i++)
 {
   for(int j = 0; j < SIZE; j++)
   {
     double currentred = inImage[i][j][0];
     double currentgreen = inImage[i][j][1];
     double currentblue = inImage[i][j][2];
     double distance = sqrt((pow(red_average-currentred, 2))+(pow(green_average-currentgreen, 2)) + (pow(blue_average-currentblue, 2)));
     if(distance >= threshold)
     {
       mask[i][j] = 1;
     }
     else{
       mask[i][j] = 0;
     }
   }
 }
}


// Devise a method to automatically come up with a threshold
//  for the chroma key determination
// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method2(unsigned char inImage[][SIZE][RGB],
      bool mask[][SIZE]) 
{
 const double num = (256 + 256)*10;




 //calculating the average value for red for the first ten rows
 //and first ten columns on the first plane
 int red_row_sum = 0;
 int red_col_sum = 0;
 for(int i = 0; i < 10; i++)
 {
   for(int j = 0; j < SIZE; j++)
   {
     red_row_sum += int(inImage[i][j][0]);
     red_col_sum += int(inImage[j][i][0]);
   }
  
 }
 int red_sum = red_row_sum + red_col_sum;
 double red_average = red_sum / num;




 //calculating the average value for green for the first ten rows
 //and first ten columns on the second plane
 int green_row_sum = 0;
 int green_col_sum = 0;
 for(int i = 0; i < 10; i++)
 {
   for(int j = 0; j < SIZE; j++)
   {
     green_row_sum += int(inImage[i][j][1]);
     green_col_sum += int(inImage[j][i][1]);
   }
  
 }
 int green_sum = green_row_sum + green_col_sum;
 double green_average = green_sum / num;




 //calculating the average value for blue for the first ten rows
 //and first ten columns on the thrid plane
 int blue_row_sum = 0;
 int blue_col_sum = 0;
 for(int i = 0; i < 10; i++)
 {
   for(int j = 0; j < SIZE; j++)
   {
     blue_row_sum += int(inImage[i][j][2]);
     blue_col_sum += int(inImage[j][i][2]);
   }
 }
 int blue_sum = blue_row_sum + blue_col_sum;
 double blue_average = blue_sum / num;




 //the code below calculates the distance between each pixel's
 //corresponding RGB values and the average values for each color calculated above
 //with the distance formula: sqrt((x-x0)^2+(y-y0)^2+(z-z0)^2)
 //Then, the total distance value is summed up
 int total_distance = 0;
 for(int i = 0; i < 10; i++)
 {
   for(int j = 0; j < SIZE; j++)
   {
     double currentred = inImage[i][j][0];
     double currentgreen = inImage[i][j][1];
     double currentblue = inImage[i][j][2];
     double distance = sqrt((pow(red_average-currentred, 2))+(pow(green_average-currentgreen, 2)) + (pow(blue_average-currentblue, 2)));
     total_distance += distance;
   }
 }
 for(int i = 0; i < SIZE; i++)
 {
   for(int j = 0; j < 10; j++)
   {
     double currentred = inImage[i][j][0];
     double currentgreen = inImage[i][j][1];
     double currentblue = inImage[i][j][2];
     double distance = sqrt((pow(red_average-currentred, 2))+(pow(green_average-currentgreen, 2)) + (pow(blue_average-currentblue, 2)));
     total_distance += distance;
   }
 }


 //the value for threshold is calculated using total distance divides
 //the total number of pixels that are used in this method (5120).
 //then, a fudge factor of 2.5 is used to scale the threshold value.
 //a 0.1 is added the end because in the minion image, all background have
 //the same exact color meaning that the RGB values are the same. So I would
 //get a 0 when I am calculating the distance. By adding a 0.1, any discrepancy of pixel value
 //more than 0.1 will be in effect.
 double threshold = (total_distance / num)*2.5 + 0.1;




 //assigning values to the mask array
 for(int i = 0; i < SIZE; i++)
 {
   for(int j = 0; j < SIZE; j++)
   {
     double currentred = inImage[i][j][0];
     double currentgreen = inImage[i][j][1];
     double currentblue = inImage[i][j][2];
     double distance = sqrt((pow(red_average-currentred, 2))+(pow(green_average-currentgreen, 2)) + (pow(blue_average-currentblue, 2)));
     if(distance >= threshold)
     {
       mask[i][j] = 1;
     }
     else{
       mask[i][j] = 0;
     }
   }
 }


}


// If mask[i][j] = 1 use the input image pixel for the output image
// Else if mask[i][j] = 0 use the background image pixel
// Create the output image using the mask to determine
//  whether to use the pixel from the Input or Background image
void replace(bool mask[SIZE][SIZE],
      unsigned char inImage[SIZE][SIZE][RGB],
      unsigned char bgImage[SIZE][SIZE][RGB],
      unsigned char outImage[SIZE][SIZE][RGB])
      {
        for(int i = 0; i < SIZE; i++)
        {
          for(int j = 0; j < SIZE; j++)
          {
            if(mask[i][j] == 1)
            {
              outImage[i][j][0] = inImage[i][j][0];
              outImage[i][j][1] = inImage[i][j][1];
              outImage[i][j][2] = inImage[i][j][2];
            }
            else{
              outImage[i][j][0] = bgImage[i][j][0];
              outImage[i][j][1] = bgImage[i][j][1];
              outImage[i][j][2] = bgImage[i][j][2];
              }
          }
        }


      }
 








