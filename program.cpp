#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Bitmap.h"

int main()
{
	const char *inFileName = "PICTURE.BMP";
	const char *outFileName= "out.bmp";
	Bitmap bmp,bmpOut;
	
	if(LoadBitmap(inFileName, bmp))
	{
		printf("%dx%d\n", bmp.width, bmp.height);
		
		//AdjustBrightness(bmp, 1.5);
		//Enlarge(bmp, bmpOut);
		//grayscale(bmp);
		//CatAnh(bmp, bmpOut, 150, 84, 491, 431);
		//blur(bmp, 6);
		//Amban(bmp);
		//Filter(bmp, 1);
		//ZoomIn(bmp, 150, 84, 491, 431, 4);
		//DoiChieuAnh(bmp);
		//filter(bmp, 1);
		//ProcessImageWithFilter(bmp);
		//reflectImage(bmp, 1);
		rotate(bmp, 45);
		
		if(SaveBitmap(outFileName, bmp))
		{
		}
		else
			printf("Can not save the bitmap file!!!\n");
		printf("%dx%d\n", bmp.width, bmp.height);
		DisposeBitmap(bmp);
	}
	else
		printf("Can not load the bitmap file!!!\n");
	
	printf("Bye!\n");
	
	return 0;
}
