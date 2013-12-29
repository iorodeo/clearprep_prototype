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

//bmp2bin_sprite 
// by Jenn Holt
//converts a bitmap picture to a binary file of sprite data for Jenn Holt's sparkfun serial graphic LCD firmware
//outputs to stdout, use redirection to catch it in a file

#include <stdlib.h>
#include <stdio.h>
#include <bmp.h>
#include <string.h>


int main(int argc, char **argv)
{
	bitmap bmp;
	unsigned int y,w,h,Nrows,i,j,k;
	unsigned char buff[128]; 	//buffer to hold a row of image data

	atexit(bmp_gc);

  if(argc==2)
  {	

	if (!(bmp = bmp_load(argv[1]))) {
                fprintf(stderr,"Error creating image!\n");
		return -2;
         }       

	/* get the height and width of the image. */

        w = bmp_get_width(bmp);
        h = bmp_get_height(bmp);

	if ((w>128) || (h>64))
	{
		fprintf(stderr,"Image is too large. max size is 128x64\n");
		return -1;
	}
	
	//calculate the number of rows necessary(each row is 8 pixels tall)
	Nrows = h/8;			//number of whole rows necessary
	if (h%8!=0)
		Nrows++;		//a partial row counts too

	//build sprite

	//first 2 bytes are width and height		
	buff[0]=w;
	buff[1]=h;
 
	fwrite(buff,2,1, stdout);

	//now build image data
	for (i=0;i<Nrows;i++)	//loop for each row
	{
		memset(buff,0x00,128);	//clear all bytes in buffer for each new row
		for (j=0;j<w;j++)	//loop for each byte in row
		{
			for(k=0;k<8;k++) //loop for each pixel in byte
			{
				//calculate the y coordinate of this bit(x coordinate is j)
				y = i*8+k;	
				if (GetPixel(bmp,j,y)!=0)	//check if pixel is set
				{
					buff[j] |= ((unsigned char)1 << k);		//set the pixel
				}
		 	}
		}
		//output row of data
		fwrite(buff,w,1,stdout); //write w bytes(1 for each x coordinate)	
	}
  }
  else
  {
	printf("Usage: bmp2bin_sprite [infile.bmp] > sprite.bin\n");
	printf("Converts a 24bbp bitmap image into a sprite\n");
	printf("for Jenn Holt's SparkFun graphical LCD firmware.\n");
	printf("any pixel in the source not equal to 0 is considered set in the sprite.\n");
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


