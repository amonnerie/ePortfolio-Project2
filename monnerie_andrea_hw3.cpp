/*
name: Andrea Monnerie
data: 4-5-21
description: This program creates sierpinski carpet
             with the given number of layers (n) from
             the user, then it will make an output file
             of the carpet. [Bonus: it will make a
             bitmap file of the carpet]
*/

#include <iostream>
#include <math.h>
#include <fstream>
#include <cmath>
#include <cstdint>


using namespace std;

//prototypes
void sierpinski(char**, int, int);  //this will create the carpet
void printArray(char**, int, int);  //this will print and make the output file
void carpetAlgorithm(char**, int, int); //this is the helper function for sierpinski()
void bitmap(char**, int);
int main()
{
  int layers;  //declared layers

  //step 1: get value of layers from user
  cout << "****Sierpinski's Carpet Maker****" << endl;
  do{
    cout << "\nHow many layers (max is 6):" << endl;      //6 layers will be the max
    cin >> layers;                                        //get the value
    if (layers < 0 || layers > 6)              //check to make sure that layers is a positive value <= 7
      cout << "invalid number (0 < layers <= 6):" << endl;  //error message is display if invalid
  }while(layers < 0 || layers > 6);              //repeat until user gives a valid value

  //step 2: create the 2D dynamic array
  int row, col;                           //initialized the diminisions
  row = (int) pow(3, layers);              //get dim for x
  col = (int) pow(3, layers);             //make a copy to y
  char** array = new char*[col];           //create the 1D array with the new number of columns

  for(int i = 0; i < row; i++)
    {
      array[i] = new char[row];            //create an array[row] for each column, now array is 2D
    }

  //step 3: make the sierpinski carpet
  sierpinski(array, row, col);             //call the sierpinski function: create the carpet

  //print and make output file
  printArray(array, row, col);         //print the array (it layers <= 3) and creat the output file
  bitmap(array, row);                   //make the bitmap file
  delete[] array;                            //done using array so delete it
  return 0;                                 //done
}//end of main()

/*Pattern I noticed: The index of the row (i) and col (j) of the array for the
blank spaces eventually gets 1 = i mod 3 and 1 = j mod 3(if you keep dividing both by 3)
while everywhere else does not*/
void sierpinski(char** array,int row,int col)
{
  for(int i=0; i < row; i++)             //nested for loop so I can analyze
    {                                    //each position's x and y indexes
      for(int j=0; j < col; j++)
	  {
	  carpetAlgorithm(array, i, j);    //helper function which would print the right char
	  }
    }
}//end of sierpinski()

void printArray(char** array,int row,int col)
{
  ofstream outFile; //an ofstream object declared
  bool print = false;
  outFile.open("carpet.txt");    //the output file name will always be "carpet.txt"
  for (int i=0;i<row;i++)        //nested for loop to print each element from array
    {
     for(int j=0;j<col;j++)  //j<col would ensure '\0' is not included
	{
	  if (col<= 27) //if layers <= 3 aka row <=27 && col <=27 then print on screen
	    {
	    cout << array[i][j];
            print = true;   //print "\n" since it reached the end of the line
	    }
	  outFile << array[i][j];  //print the array regardless of size
	}
     if(print)
       {
	 cout << endl;
       }
     outFile << endl;
    }
  outFile.close();
  cout << "\nfile named \"carpet.txt\" has been created" << endl; //a message to confirm that the file has been created
}//end of printArray()

void carpetAlgorithm(char** array, int i, int j)
{
   array[i][j] = '#'; //print '#' for every element
  int r = i, c = j;//store the original i and j
  while(i > 0 && j > 0)
   {
    if(i % 3 == 1 && j % 3 == 1) //if valid then it found the center
     {
       array[r][c] = ' ';        //replace '#' with a ' '
       break;                    //leave the while loop
     }
    i=i/3;                      //if condition failed then divide by 3 and try again
    j=j/3;
   }
}

void bitmap(char** array, int row)
{
  ofstream bmpFile;                             //declared the bitmap file: bmp file
  bmpFile.open("bitmap.bmp",ios::binary);        //opened it and noted that it is a binary file

  uint16_t twoBytesHeader;                        //declared some variables - unsigned integers
  uint32_t fourBytesHeader;
  uint32_t width = (uint32_t) row;                //unsigned intger (32 bits)

  //****now I need to insert all the information for
  //****the header before printing the array in file

  //********************file header****************************
  twoBytesHeader = 19778; //default for bmp file 'B' and 'M'
                          //19778 is deciaml for 'M' and 'B' since the most significant bit is at right
  bmpFile.write((char*)&twoBytesHeader,sizeof(twoBytesHeader)); //print it in file

  //since there is a thing called padding, I needed to determine
  //total size with paddings from the given width (as the padding
  //depends on the width ... in which the width varies
  uint32_t widthSize = (uint32_t)4*(ceil((double)width/32.00));  //# of bytes for row with padding
                                                                 //which must be a multiple of four
  fourBytesHeader = widthSize*width+64;    //calculating the total size of bmp file (including header)
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));  //print it in file

  fourBytesHeader = 0; //reserved bytes
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader)); //print it in file

  fourBytesHeader = 62;  //offset from beginning from file to image data
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader)); //print it in file

  //***********image header*************************************
  fourBytesHeader = 40; //default for total bytes for  image header
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader)); //print it in file

  //width and height (but for this scenario it is a square so both will be width
  fourBytesHeader = width;              //x component of the size of image
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));  //print it in file
  fourBytesHeader = width;             //y component of the size of image
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));   //print it in file

  //reserved bytes - print 0x01 0x00 two times
  twoBytesHeader = 1;      //this is reserved
  bmpFile.write((char*)&twoBytesHeader,sizeof(twoBytesHeader));  //print it in file
  bmpFile.write((char*)&twoBytesHeader,sizeof(twoBytesHeader));   //print it in file

  //reserved bytes - print 0x00 six times
  fourBytesHeader = 0;
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));   //print theses in file
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));

  //color table (R G B)(but for this scenario it is monochromic)
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader)); //0x00 - this prints black
  fourBytesHeader = 16777215;                             //decimal for 0xff 0xff 0xff - this is for white
  bmpFile.write((char*)&fourBytesHeader,sizeof(fourBytesHeader));  //print it in file

  char imageByte = 0;    //this is for getting the bits in sequence for displaying color
  uint32_t pos =0; //position of pixel of current row
  for(uint32_t j = 0; j < width; j++)
    {
      pos = 0;
      for(uint32_t i = 0; i < widthSize; i++) //for each byte
        {
           for(int bit = 0; bit < 8; bit++)  //for each bit/pixel
            {
              if(pos < width)
                {
		  if(array[pos][j] == '#')
                   imageByte=(imageByte<<1)+0; //logical shift left - have bit of 0 for black
		  else
		    imageByte = (imageByte<<1)+1; //logical shift left - 1 for white
                }
              else
                {
                  imageByte=(imageByte<<1); //otherwise logical shift left
                }
              pos++; //go to next position
            }
          bmpFile.write((char*)&imageByte,sizeof(imageByte)); //print the byte in file
        }
    }
bmpFile.close();    //close the file
  cout << "file named \"bitmap.bmp\" has been created" << endl;  //display message that file is complete
  if(row <= 27)
    {
      cout << "Note: the bitmap image size is very small" << endl; //display note that image would be small
    }
}
