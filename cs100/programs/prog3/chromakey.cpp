//--------------------------------------------------------
// File:	 chromakey.cpp
//
// Author:	 Mark Redekopp
//
// Date:	 2008-Jul-23
//
// Usage:	 chromakey greenscreen.bmp background.bmp DIST meth1.bmp meth2.bmp
//
// Notes:
// This program performs the chroma key operation  on an input 
// using two different methods.
//
// Method 1 Take average pixel value of first row/column and then
//			take a user-defined distance threshold from that
//			pixel value as a discriminator
//
// Method 2 Up to you?
//
//--------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// Place the #include for bmplib.h here
#include "bmplib.h"
// Place other necessary #includes here 
#include <math.h>


#define NUM_METHODS 2
#define SIZE 256
#define RGB 3
using namespace std;
// Declare your image arrays here
// image data array

unsigned char OutputImage[SIZE][SIZE][RGB];
unsigned char InputImage[SIZE][SIZE][RGB];
unsigned char BgrndImage[SIZE][SIZE][RGB];

// Mask arrays (one for each method)
//	you can define more
unsigned char Mask[NUM_METHODS][SIZE][SIZE];

int main(int argc, char *argv[])
{
  // declare any temporary or other variables here
  int h,i,j,k;

  if(argc < 6){
	//	  printf("usage: program_name input.bmp background.bmp dist_threshold output1.bmp output2.bmp\n");
	cout << "usage: program_name input.bmp background.bmp dist_threshold output1.bmp output2.bmp" << endl;
	return 0;
  }
	
  if(readRGBBMP(argv[1], InputImage) ){
	//	  printf("Error reading file: %s\n", argv[1]);
	cout << "Error reading file: " << argv[1] << endl;
	exit(1);
  }

  // read in the background image here
  if(readRGBBMP(argv[2], BgrndImage) ){
	//	  printf("Error reading file: %s\n", argv[1]);
	cout << "Error reading file: " << argv[2] << endl;
	exit(1);
  }

 
  // Write code to find the chroma key reference point here
  int reference_point[3] = {0, 0, 0};
  for(j=0; j<SIZE; j++){
    for(k=0; k<3; k++){
	  reference_point[k] += InputImage[1][j][k];
	}
  }

  reference_point[0] = reference_point[0] / SIZE;
  reference_point[1] = reference_point[1] / SIZE;
  reference_point[2] = reference_point[2] / SIZE;

 
  // For debug, print out the chroma key reference RGB value here
  cout << "RGB : " << reference_point[0] << "," << reference_point[1] << "," << reference_point[2] <<endl;


  // Method 1: Create the mask using the distance threshold method
  //		   in Mask[0][i][j]
  for(i=0; i<SIZE; i++){	
	for(j=0; j<SIZE; j++){
		if ((double) sqrt(pow((InputImage[i][j][0] - reference_point[0]), 2) + 
					 pow((InputImage[i][j][1] - reference_point[1]), 2) + 
					 pow((InputImage[i][j][2] - reference_point[2]), 2)
			) < (double) atof(argv[3]) ){
		  Mask[0][i][j] = 0;
		} else {
		  Mask[0][i][j] = 1;
		}
	}
  }


  // Method 2: Write code for your devised method 2 here and create
  //		   the necessary mask in Mask[1][i][j]	  
  double distance = 0;
  for(i=1; i<SIZE; i++){	
    for(j=1; j<SIZE; j++){

      // filter all pixels which are really close to referance point
	  distance = sqrt(pow((InputImage[i][j][0] - reference_point[0]), 2) + 
					  pow((InputImage[i][j][1] - reference_point[1]), 2) + 
					  pow((InputImage[i][j][2] - reference_point[2]), 2));
	  Mask[1][i][j] = 1;
	  if (distance < 40){
	    Mask[1][i][j] = 0;
	  }
	  
	  // filter pixel if it is close to pixel at right
	  if (!Mask[1][i][j-1]){
	    distance =  sqrt(pow(InputImage[i][j][0] - InputImage[i][j-1][0], 2) + 
					     pow(InputImage[i][j][1] - InputImage[i][j-1][1], 2) + 
					     pow(InputImage[i][j][2] - InputImage[i][j-1][2], 2));
	    if (distance < 20){
	      Mask[1][i][j] = 0;
	    }
      }
      
      // filter pixel if it is close to pixel on top
      if (!Mask[1][i-1][j]){
	    distance =  sqrt(pow(InputImage[i][j][0] - InputImage[i-1][j][0], 2) + 
					     pow(InputImage[i][j][1] - InputImage[i-1][j][1], 2) + 
					     pow(InputImage[i][j][2] - InputImage[i-1][j][2], 2));
	    if (distance < 20){
		  Mask[1][i][j] = 0;
	    }
	  }
	}
  }
  
  for(i=SIZE; i>0; i--){	
	for(j=SIZE; j>0; j--){
	  if (Mask[1][i][j]){
		
	    // filter pixel if it is close to pixel at left
	    if (!Mask[1][i][j+1]){
		  distance =  sqrt(pow(InputImage[i][j][0] - InputImage[i][j+1][0], 2) + 
					       pow(InputImage[i][j][1] - InputImage[i][j+1][1], 2) + 
					       pow(InputImage[i][j][2] - InputImage[i][j+1][2], 2));
		  if (distance < 20){
		    Mask[1][i][j] = 0;
		  }
		}
		
		// filter pixel if it is close to pixel at bottom
		if (!Mask[1][i+1][j]){
		  distance =  sqrt(pow(InputImage[i][j][0] - InputImage[i+1][j][0], 2) + 
					       pow(InputImage[i][j][1] - InputImage[i+1][j][1], 2) + 
					       pow(InputImage[i][j][2] - InputImage[i+1][j][2], 2));
		  if (distance < 20){
		    Mask[1][i][j] = 0;
		  }
		}
	  }
	}
  }
  
  
  // Create an output image for each method by checking
  // Uses the masks created above (0=Replace w/ background image /
  //  1 = Input Image)
  // This code is complete and need not be altered provided you created
  ///  the mask arrays correctly.
  for(h=0; h<NUM_METHODS; h++){
	for(i=0; i<SIZE; i++){
	  for(j=0; j<SIZE; j++){
		if(Mask[h][i][j] == 1){
		  for(k=0; k<3; k++){
			OutputImage[i][j][k] = InputImage[i][j][k];
		  }
		}
		// Otherwise select the background image pixel
		else {
		  for(k=0; k<3; k++){
			OutputImage[i][j][k] = BgrndImage[i][j][k];
		  }
		}
	  }
	}
	
	if (writeRGBBMP(argv[4+h], OutputImage) ){
	  //	  printf("Error writing file: %s\n", argv[4+h]);
	  cout << "Error writing file: " << argv[4+h] << endl;
	  exit(1);
	}	
  }

  return 0;
}
