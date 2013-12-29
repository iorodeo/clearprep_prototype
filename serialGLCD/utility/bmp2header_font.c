/*
Copyright (c) 2010 Jennifer Holt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

//bmp2header_font 
// by Jenn Holt
//converts a bitmap picture containing N character images in the horizontal direction to a c header file of font data for Jenn Holt's sparkfun serial graphic LCD firmware
//the input bitmap should be the height of your characters in the y direction and N*(width of your characters) in the x direction, where N is the number of characters in the font
//Usage:  bmp2header_font [width of character] [font space] [bitmap.bmp] > [file.h]
//height of character is calculated from image, number of characters is int(bitmapwidth/characterwidth], if your image is a non-integral number of character widths wide, the extra will be ignored
//outputs to stdout, use redirection to catch it in a file

#include <stdlib.h>
#include <stdio.h>
#include <bmp.h>
#include <string.h>


int main(int argc, char **argv)
{
	bitmap bmp;
	unsigned int y,w,h,Nrows,i,j,k,Nchars, charnum,char_w, char_bytes,font_size,font_space,firstbyte;
	unsigned char buff[128]; 	//buffer to hold a row of image data

	atexit(bmp_gc);

  if(argc==4)
  {	

	if (!(bmp = bmp_load(argv[3]))) {
                fprintf(stderr,"Error creating image!\n");
		return -2;
         }       

	/* get the height and width of the image. */

        w = bmp_get_width(bmp);
        h = bmp_get_height(bmp);
	//get font size data
		char_w = atoi(argv[1]);
		font_space = atoi(argv[2]);

	//verify font parameters
		//is size of character valid?
		//is number og bytes/character <=128?

	
	//check to make sure the character is not larger than the display
	if ( (char_w >128) || (h>64) )
	{
		fprintf(stderr,"character is too large. max size is 128x64\n");
		return -1;
	}
	
	//calculate the number of rows necessary(each row is 8 pixels tall)
	Nrows = h/8;			//number of whole rows necessary
	if (h%8!=0)
		Nrows++;		//a partial row counts too

	//calculate the number of characters in the image
	Nchars = (w/char_w);

	//calculate # of bytes per character
	char_bytes = Nrows*char_w;

	//calculate total number of bytes for font

	font_size = char_bytes*Nchars+3;	//char_bytes repeated Nchars times +3 for font width,height,space

	//build font
	
	printf("static char text_array[%d] PROGMEM = {0x%.2x, 0x%.2x, 0x%.2x,\n",font_size,char_w,h,font_space);	//print the array definition

	//now build image data

	firstbyte=0;					//to get the right number of comma's in the array initilization
	for(charnum=0;charnum<Nchars;charnum++)		//loop for each character in the image	
	{	
		for (i=0;i<Nrows;i++)	//loop for each row
		{
			memset(buff,0x00,128);	//clear all bytes in buffer for each new row
			for (j=0;j<char_w;j++)	//loop for each byte in row
			{
				for(k=0;k<8;k++) //loop for each pixel in byte
				{
					//calculate the y coordinate of this bit(x coordinate is j+charnum*char_w)
					y = i*8+k;	
					if (GetPixel(bmp,j+charnum*char_w,y)==0)	//check if pixel is set
					{
						buff[j] |= ((unsigned char)1 << k);		//set the pixel
					}
			 	}
			}
			//output row of data
			for (y=0;y<char_w;y++)
			{		
				if (firstbyte==0)
					printf("0x%.2x",buff[y]); //write w bytes(1 for each x coordinate)
				else
					printf(", 0x%.2x",buff[y]); //write w bytes(1 for each x coordinate)
				firstbyte=1;
			}
		}
		printf("\n");	//print a newline to make the file pretty	
	}
	printf("};\n");		//finish array definition 
  }
  else
  {
	printf("Usage: bmp2header_font [character width in pixels] [spaceing in pixels] [infile.bmp] > header.h\n");
	printf("Converts a 24bbp bitmap image with characters in the x direction into a header file array definition\n");
	printf("for Jenn Holt's SparkFun graphical LCD firmware.\n");
	printf("any pixel in the source identically equal to 0 is considered set in the font.\n");
  }
	return 0;
}

int GetPixel(bitmap bmp,int x,int y)	//returns pixel value from a bitmap 
{
	int pix;	//integer to store pixel value 0x00RRGGBB
	int w,h,offset,padding;
	char *img;
	/* get the height and width of the image. */

        w = bmp_get_width(bmp);
        h = bmp_get_height(bmp);
	/* get pointer to image data */

	  if (!(img = bmp_get_img(bmp))) {
                fprintf(stderr,"Cannot get image. Object does not exist\n");
        }       


	/*calculate offset to pixel of interest */
	padding = (w % 4);			//figure number of padding bytes
	offset = ((w*3+padding)*y + x*3);		//3 bytes/pixel 
	
	memcpy(&pix,(char*)(img+offset),3);	//get data	
	
	return pix;	
}


