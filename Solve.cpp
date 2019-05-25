#define _CRT_SECURE_NO_WARNINGS
#include "Bitmap.h"


// cai dat pixel
int SetPixel(Bitmap &bmp, int row, int col, Color color)
{
	if(row < 0 || row >= bmp.height
		 || col < 0 || col >= bmp.width)
		return 0;
	
	// color: B, G, R
	int offset = (bmp.height - 1 - row)*bmp.rowSize + col*3;
	bmp.pixels[offset] = color.B;
	bmp.pixels[offset + 1] = color.G;
	bmp.pixels[offset + 2] = color.R;
}

//lay pixel
int GetPixel(const Bitmap &bmp, int row, int col, Color &color)
{
	if(row < 0 || row >= bmp.height || col < 0 || col >= bmp.width)
		return 0;
	
	// color: B, G, R
	//Trong day se luu nguoc lai RGB -> BGR
	int offset = (bmp.height - 1 - row)*bmp.rowSize + col*3;
	color.B = bmp.pixels[offset];
	color.G = bmp.pixels[offset + 1];
	color.R = bmp.pixels[offset + 2];
}

void Enlarge(const Bitmap &inbmp, Bitmap &BmpOut)
{
	BmpOut.width = 2 * inbmp.width;
	BmpOut.height = 2 * inbmp.height;
	BmpOut.rowSize = ((3 * BmpOut.width + 3)/4)*4;
	BmpOut.pixels = new unsigned char[BmpOut.rowSize * BmpOut.height];
	for(int row = 0; row < inbmp.height; row++)
		for(int col = 0; col < inbmp.width; col++)
		{
			Color color;
			GetPixel(inbmp, row, col, color);
			SetPixel(BmpOut, 2*row, 2*col, color);
			SetPixel(BmpOut, 2*row, 2*col + 1, color);
			SetPixel(BmpOut, 2*row + 1, 2*col, color);
			SetPixel(BmpOut, 2*row + 1, 2*col + 1, color);
		}
}

void AdjustBrightness(Bitmap bmp, double factor)
{
	for(int row = 0; row < bmp.height; row++)
		for(int col = 0; col < bmp.width; col++)
		{
			Color color;
			GetPixel(bmp, row, col, color);
			
			if(color.R*factor > 255)
				color.R = 255;
			else
				color.R = color.R*factor;
			
			if(color.G*factor > 255)
				color.G = 255;
			else
				color.G = color.G*factor;
			
			if(color.B*factor > 255)
				color.B = 255;
			else
				color.B = color.B*factor;
			
			SetPixel(bmp, row, col, color);
		}
}


void grayscale(Bitmap bmp)
{
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			Color color;
			GetPixel(bmp, row, col, color);
			int _val = (color.R + color.G + color.B) / 3;
			color = {unsigned char(_val),unsigned char(_val),unsigned char(_val) };
			SetPixel(bmp, row, col, color);
		}
}

//Lam mo anh
void blur(Bitmap &bmp, int radius)
{
	float rs = ceil(radius * 2.57);
	for (int i = 0; i < bmp.height; ++i)
	{
		for (int j = 0; j < bmp.width; ++j)
		{
			double r = 0, g = 0, b = 0;
			double count = 0;

			for (int iy = i - rs; iy < i + rs + 1; ++iy)
			{
				for (int ix = j - rs; ix < j + rs + 1; ++ix)
				{
					auto x = min((bmp.width) - 1,max(0, ix));
					auto y = min((bmp.height) - 1, max(0, iy));

					auto dsq = ((ix - j) * (ix - j)) + ((iy - i) * (iy - i));
					auto wght = exp(-dsq / (2.0 * radius * radius)) / (_PI * 2.0 * radius * radius);

					Color color;

					GetPixel(bmp, y, x,color);

					r += color.R * wght;
					g += color.G * wght;
					b += color.B * wght;
					count += wght;
				}
			}
			Color color;

			GetPixel(bmp, i, j, color);
			color.R = round(r / count);
			color.G = round(g / count);
			color.B = round(b / count);
			SetPixel(bmp, i, j, color);
			
		}
	}
}

//Mau anh ban
void Amban(Bitmap& bmp) {

	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			Color color;
			GetPixel(bmp, row, col, color);
			color.R = 255 - color.R;
			color.B = 255 - color.B;
			color.G = 255 - color.G;
			
			SetPixel(bmp, row, col, color);
		}
}

//Loc mau
//1: mau do
//2: mau xanh(Green)
//3: mau xanh Blue
void Filter(Bitmap& bmp, int _MainColor) {
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			Color color;
			GetPixel(bmp, row, col, color);
			
			switch (_MainColor) {
			case 1: color.B = 255 - color.B; color.G = 255 - color.G;
				break;
			case 2: color.R = 255 - color.R; color.B = 255 - color.B;
				break;
			case 3: color.G = 255 - color.G; color.R = 255 - color.R;
				break;
			
			}
		
			SetPixel(bmp, row, col, color);
		}
	
}

//Cat anh
void CatAnh(Bitmap& bmp, Bitmap& _BmpOut, int _firstx, int _firsty, int _secondx, int _secondy) {
	_BmpOut.height = abs(_secondy - _firsty);
	_BmpOut.width = abs(_secondx - _firstx);
	_BmpOut.rowSize = ((3 * _BmpOut.width + 3) / 4) * 4;
	_BmpOut.pixels = new unsigned char[_BmpOut.rowSize * _BmpOut.height];

	int _NewCol = 0;
	int _NewRow = 0;

	for (int row = _firsty; row < _secondy; row++) {
		for (int col = _firstx; col < _secondx; col++)
		{

			Color color;
			GetPixel(bmp, row, col, color);
			SetPixel(_BmpOut, _NewRow, _NewCol, color);
			_NewCol++;
		}
		_NewCol = 0;
		_NewRow++;
	}
	bmp = _BmpOut;
}


//Phong to
void ZoomIn(Bitmap& bmp, int _firstx, int _firsty, int _secondx, int _secondy, int ratio) {

	Bitmap _CatAnh,_ZoomBmp;
	CatAnh(bmp, _CatAnh, _firstx, _firsty, _secondx, _secondy);
	
	_ZoomBmp.width = ratio * _CatAnh.width;
	_ZoomBmp.height = ratio * _CatAnh.height;
	_ZoomBmp.rowSize = ((3 * _ZoomBmp.width + 3) / 4) * 4;
	_ZoomBmp.pixels = new unsigned char[_ZoomBmp.rowSize * _ZoomBmp.height];
	int _NewRow = 0;
	int _NewCol = 0;
	for (int row = 0; row < _CatAnh.height; row++) {
		for (int col = 0; col < _CatAnh.width; col++)
		{
			Color color;
			GetPixel(_CatAnh, row, col, color);
			for (int i = 0; i < ratio; i++)
				for (int j = 0; j < ratio; j++) {
					SetPixel(_ZoomBmp, ratio * row + i, ratio * col + j, color);

				}
			
		}
		
	}
	bmp = _ZoomBmp;
}

// ham doi chieu anh
void DoiChieuAnh(Bitmap& bmp) {
	
	for (int row = 0; row < bmp.height; row++) {
		for (int col = 0; col < bmp.width / 2; col++) {
			Color _colorleft, _colorright;

			GetPixel(bmp, row, col, _colorleft);
			GetPixel(bmp, row, bmp.width - col, _colorright);

			SetPixel(bmp, row, bmp.width - col, _colorleft);
			SetPixel(bmp, row, col, _colorright);

		}
	}
}

void filter(Bitmap& bmp, double z) {
	double mask[] = { 0,   -z  ,   0,
					 -z,  1 + 4 * z,  -z,
					  0,   -z  ,   0 };
	byte R, G, B;
	Color color;
	for (int row = 1; row < bmp.height - 1; row++) {
		for (int col = 1; col < bmp.width - 1; col++) {
			double linc_r = 0, linc_g = 0, linc_b = 0;
			for (int k = 0; k <= 2; k++) {
				for (int l = 0; l <= 2; l++) {
					GetPixel(bmp, row + k - 1, col + l - 1, color);
					
					linc_r += color.R * mask[k * 3 + l];
					linc_g += color.G * mask[k * 3 + l];
					linc_b += color.B * mask[k * 3 + l];
				}
			}
			
		

			color.R = min(linc_r,255);
			color.G = min(linc_g, 255);
			color.B = min(linc_b, 255);

			SetPixel(bmp, row, col, color);
		}
	}
}

void ProcessImageWithFilter(Bitmap& bmp)
{
	
	int filterWidth = 3;
	int filterHeight = 3;
	
	Color* _newColor = new Color[bmp.height * bmp.width];
	double filter[3][3] = { {-1, -1, -1}, { -1,9,-1 }, { -1,-1,-1 } };
	

	double factor = 1.0;
	double bias = 0.0;

	//Color[, ] result = new Color[img.Width, img.Height];

	for (int col = 0; col < bmp.width; ++col)
	{
		for (int row = 0; row < bmp.height; ++row)
		{
			double red = 0.0, green = 0.0, blue = 0.0;


			for (int filterX = 0; filterX < filterWidth; filterX++)
			{
				for (int filterY = 0; filterY < filterHeight; filterY++)
				{
					int imageX = (col - filterWidth / 2 + filterX + bmp.width) % bmp.width;
					int imageY = (row - filterHeight / 2 + filterY + bmp.height) % bmp.height;

					
					Color color;
					GetPixel(bmp, imageY, imageX, color);
					
					red += color.R * filter[filterX][filterY];
					green += color.G * filter[filterX][filterY];
					blue += color.B * filter[filterX][filterY];

				}

				int r = min(max((int)(factor * red + bias), 0), 255);
				int g = min(max((int)(factor * green + bias), 0), 255);
				int b = min(max((int)(factor * blue + bias), 0), 255);
				
				_newColor[row * bmp.width + col].R = r;
				_newColor[row * bmp.width + col].G = g;
				_newColor[row * bmp.width + col].B = b;
				


			}
		}
	}

	for (int row = 0; row < bmp.height; ++row)
	{
		for (int col = 0; col < bmp.width; ++col)
		{
			
			if (_newColor[row * bmp.width + col].R == 0 && _newColor[row * bmp.width + col].G == 0 && _newColor[row * bmp.width + col].B == 0)
			{
				
			}
			else
			{
				Color color = _newColor[row * bmp.width + col];
				SetPixel(bmp,row, col, color);
			}
		}
	}
}



void rotate(Bitmap& bmp, double degree) {
	
	Bitmap _Imagetmp;

	_Imagetmp.width = bmp.width;
	_Imagetmp.height = bmp.height;
	_Imagetmp.rowSize = ((3 * bmp.width + 3) / 4) * 4;
	_Imagetmp.pixels = new unsigned char[_Imagetmp.rowSize * _Imagetmp.height];


	double rads = (degree * 3.14159265) / 180.0;
	int sinf = (int)sin(rads);
	int cosf = (int)cos(rads);

	//Chon goc toa do o giua anh
	double c0 = 0.5 * (bmp.width );     
	double r0 = 0.5 * (bmp.height);     


	// rotation
	for (int col = 0; col < bmp.width; col++) {
		for (int row = 0; row < bmp.height; row++) {
			
			int _newRow = (int)(r0 + ((row - r0) * cos(rads)) - ((col - c0) * sin(rads)));
			int _newCol = (int)(c0 + ((row - r0) * sin(rads)) + ((col - c0) * cos(rads)));

			

			//Neu no van con nam trong hinh
			if (_newCol >= 0 && _newCol < bmp.width && _newRow >= 0 && _newRow < bmp.height) {
				Color color;
				GetPixel(bmp, row, col, color);
				SetPixel(_Imagetmp, _newRow, _newCol, color);
			
			}
		}
	}
	bmp = _Imagetmp;
	
}


//Neu nhan 0: phan anh hinh anh tu truc thang dung
//Neu nhan 1: phan anh hinh anh tu truc ngang
void reflectImage(Bitmap& bmp, bool flag)
/*Reflects the Image based on users input*/
{
	int rows = bmp.height;
	int cols = bmp.width;
	Bitmap _newImage = bmp;
	if (flag == true) //horizontal reflection
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++) {
				Color color;
				GetPixel(bmp, i, j, color);
				SetPixel(_newImage, rows - (i + 1), j, color);
				
			}
		}
	}
	else //vertical reflection
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++) {
				Color color;
				GetPixel(bmp, i, j, color);
				SetPixel(_newImage, i, cols - (j + 1), color);
			}
				
		}
	}

	bmp = _newImage;
}

