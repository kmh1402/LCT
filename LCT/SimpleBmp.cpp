#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "SimpleBmp.h"

#define WIDTHBYTES(bits)  (((bits)+31)/32*4)

/*******************************************************************
BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, 
		int height, long* newsize )


This function takes as input an array of RGB values, it's width
and height.
The buffer gets then transformed to an array that can be used
to write to a windows bitmap file. The size of the array
is returned in newsize, the array itself is the
return value of the function.
Both input and output buffers must be deleted by the
calling function.

The input buffer is expected to consist of width * height
RGB triplets. Thus the total size of the buffer is taken as
width * height * 3.

The function then transforms this buffer so that it can be written 
to a windows bitmap file:
First the RGB triplets are converted to BGR.
Then the buffer is swapped around since .bmps store
images uside-down.
Finally the buffer gets DWORD ( 32bit ) aligned, 
meaning that each scanline ( 3 * width bytes ) gets
padded with 0x00 bytes up to the next DWORD boundary

*******************************************************************/

Image* CreateImage(int width, int height, int channel)
{
	Image* img = new Image;
	img->width = width;
	img->height = height;
	img->channel = channel;
	img->widthBytes = WIDTHBYTES(width * channel * 8);
	img->imageData = new BYTE[width*height*channel];
	return img;
}

void FreeImage(Image* img)
{
	delete[] img->imageData;
	delete img;
	img = NULL;
}

void CopyImage(Image* src, Image* dst)
{
	if (dst == NULL) return;
	dst->width = src->width;
	dst->height = src->height;
	dst->widthBytes = src->widthBytes;
	memcpy(dst->imageData, src->imageData, sizeof(BYTE)*src->width*src->height);
}

BYTE* ConvertRGBToBMPBuffer(BYTE* Buffer, int width, int height, long* newsize)
{
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 ) 
		padding++;

	int psw = scanlinebytes + padding;
	
	*newsize = height * psw;

	BYTE* newbuf = new BYTE[*newsize];
	
	memset ( newbuf, 0, *newsize );

	long bufpos = 0;   
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			bufpos = y * 3 * width + x; 
			newpos = ( height - y - 1 ) * psw + x;   

			newbuf[newpos] = Buffer[bufpos+2];      
			newbuf[newpos + 1] = Buffer[bufpos + 1]; 
			newbuf[newpos + 2] = Buffer[bufpos];    
		}

	return newbuf;
}

/***************************************************************
BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, 
		int width, int height )

This function takes as input the data array
from a bitmap and its width and height.
It then converts the bmp data into an RGB array.
The calling function must delete both the input
and output arrays.
The size of the returned array will be 
width * height * 3
On error the returb value is NULL, else the
RGB array.


The Buffer is expected to be the exact data read out
from a .bmp file.  
The function will swap it around, since images
are stored upside-down in bmps.
The BGR triplets from the image data will
be converted to RGB.
And finally the function removes padding bytes.
The returned arraay consits then of
width * height RGB triplets.

*****************************************************************/

BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height )
{
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;
			
	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )
		padding++;
	int psw = scanlinebytes + padding;

	BYTE* newbuf = new BYTE[width*height*3];

	long bufpos = 0;   
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;     
			bufpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos + 2];       
			newbuf[newpos + 1] = Buffer[bufpos+1]; 
			newbuf[newpos + 2] = Buffer[bufpos];     
		}
	return newbuf;
}

/***********************************************
bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile )

Takes in a device context and the name of a
bitmap to load. If an error occurs the function
returns false, else the contents of the bmp
are blitted to the HDC 

************************************************/


bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile )
{
	if ( ( NULL == hDC  ) || ( NULL == bmpfile ) )
		return false;      

	HANDLE hBmp = LoadImage ( NULL, bmpfile, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE );
	
	if ( NULL == hBmp )
		return false;

	HDC dcmem = CreateCompatibleDC ( NULL );
	if ( NULL == SelectObject ( dcmem, hBmp ) )
	{	
		DeleteDC ( dcmem ); 
		return false; 
	}

	BITMAP bm;
	GetObject ( hBmp, sizeof(bm), &bm );
	if ( BitBlt ( hDC, 0, 0, bm.bmWidth, bm.bmHeight, dcmem, 0, 0, SRCCOPY ) == 0 )
	{	
		DeleteDC ( dcmem ); 
		return false; 
	}
		   	
	DeleteDC ( dcmem );
	
	return true;
}

bool LoadBMPIntoDC2 ( HDC hDC, LPCTSTR bmpfile , int width, int height)
{
	if ( ( NULL == hDC  ) || ( NULL == bmpfile ) )
		return false;      

	HANDLE hBmp = LoadImage ( NULL, bmpfile, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE );
	
	if ( NULL == hBmp )
		return false; 
	HDC dcmem = CreateCompatibleDC ( NULL );

	if ( NULL == SelectObject ( dcmem, hBmp ) )
	{	
		DeleteDC ( dcmem ); 
		return false; 
	}
		
	BITMAP bm;
	GetObject ( hBmp, sizeof(bm), &bm );
	SetStretchBltMode(hDC, COLORONCOLOR);
	if ( StretchBlt ( hDC, 0, 0,width, height, dcmem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY ) == 0 )
	{	
		DeleteDC ( dcmem ); 
		return false; 
	}
		   	
	DeleteDC ( dcmem );
	
	return true;
}

void DrawImage(CWnd*  pWnd, Image* pImage)
{
	CRect rect;
	CDC* pDC;
	pDC = pWnd->GetDC();
	pWnd->GetClientRect(rect);

	DrawRawDataIntoDC(pDC, 0, 0, rect.Width(), rect.Height(),
		pImage->imageData, pImage->width, pImage->height, pImage->channel * 8);
	pWnd->ReleaseDC(pDC);
}

void DrawImage(CWnd*  pWnd, int x, int y, int dstWidth, int dstHeight, Image* pImage)
{	
	CDC* pDC;
	pDC = pWnd->GetDC();
	
	DrawRawDataIntoDC(pDC, x, y, dstWidth, dstHeight,
		pImage->imageData, pImage->width, pImage->height, pImage->channel*8);
	pWnd->ReleaseDC(pDC);
}

SimpleBmp::SimpleBmp()
{
	m_Init = FALSE;
	m_pImage = NULL;
	m_pDC = NULL;
	m_pWnd = NULL;
	isDrawing = FALSE;
	m_pBmpInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFOHEADER)+(256 * sizeof(RGBQUAD))];
}

SimpleBmp::~SimpleBmp()
{
	delete []m_pBmpInfo;
	delete m_pImage;
}

void SimpleBmp::SetHeaderInfo(int width, int height, int bitCount)
{
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biCompression = 0;
	m_pBmpInfo->bmiHeader.biWidth = width;
	m_pBmpInfo->bmiHeader.biHeight = height;
	m_pBmpInfo->bmiHeader.biBitCount = bitCount;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 256;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;
	m_pBmpInfo->bmiHeader.biSizeImage = height * WIDTHBYTES(width * bitCount);

	if (bitCount == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = 0;
		}
	}
}

void SimpleBmp::Draw(CWnd*  pWnd, Image* pImage)
{
	m_pWnd = pWnd;
	if (!m_pDC) {
		m_pDC = pWnd->GetDC();
		pWnd->GetClientRect(m_rect);
	}
	Draw(m_pDC, 0, 0, m_rect.Width(), m_rect.Height(),
		pImage->imageData, pImage->width, pImage->height, pImage->channel * 8);
}

void SimpleBmp::Draw(CDC* pDC, int x, int y, int dstWidth, int dstHeight, BYTE* pViewImg, int width, int height, int bitCount)
{
	if (isDrawing) return;
	isDrawing = TRUE;

	if (pViewImg == NULL) return;
	
	CBitmap bitMap, *pOldBitmap;
	if (!m_Init) {
		SetHeaderInfo(width, height, bitCount); 
		m_memDC.CreateCompatibleDC(pDC);
		bitMap.CreateCompatibleBitmap(pDC, width, height);
		pOldBitmap = (CBitmap*)m_memDC.SelectObject(&bitMap);
		m_memDC.SelectObject(&bitMap);
		CRect rect;
		m_pImage = new BYTE[m_pBmpInfo->bmiHeader.biSizeImage];
		m_Init = TRUE;
	}
	
	int widthBytes = WIDTHBYTES(width * bitCount);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < widthBytes; j++) {
			m_pImage[i*widthBytes + j] = pViewImg[(height - i - 1)* widthBytes + j];
		}
	}

	SetDIBitsToDevice(
		m_memDC.GetSafeHdc(),  
		0, 0,
		width, height,  
		0, 0,  
		0, height, 
		m_pImage, m_pBmpInfo, DIB_RGB_COLORS);

	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(x, y, dstWidth, dstHeight, &m_memDC, 0, 0, width, height, SRCCOPY);

	isDrawing = FALSE;
}

void DrawRawDataIntoDC(CDC* pDC, int x, int y, int dstWidth, int dstHeight, BYTE* pViewImg, int width, int height, int bitCount)
{ 
	BITMAPINFO *pBmpInfo = NULL; 
	if(pBmpInfo == NULL) 
		pBmpInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD))]; 
	
	pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pBmpInfo->bmiHeader.biPlanes = 1; 
	pBmpInfo->bmiHeader.biCompression = 0;
	pBmpInfo->bmiHeader.biWidth = width;
	pBmpInfo->bmiHeader.biHeight = height; 
	pBmpInfo->bmiHeader.biBitCount = bitCount; 
	pBmpInfo->bmiHeader.biXPelsPerMeter = 0; 
	pBmpInfo->bmiHeader.biYPelsPerMeter = 0; 
	pBmpInfo->bmiHeader.biClrUsed = 256; 
	pBmpInfo->bmiHeader.biClrImportant = 0;
	pBmpInfo->bmiHeader.biSizeImage =height * WIDTHBYTES( width * bitCount ); 
	
	if(bitCount==8)
	{
		for(int i = 0; i < 256; i++) 
		{ 
			pBmpInfo->bmiColors[i].rgbBlue = i; 
			pBmpInfo->bmiColors[i].rgbGreen = i; 
			pBmpInfo->bmiColors[i].rgbRed = i; 
			pBmpInfo->bmiColors[i].rgbReserved= 0; 
		} 
	}

	if(pViewImg != NULL) 
	{ 
		CRect rect;
		CDC memDC; 
		CBitmap bitMap, *pOldBitmap; 
		memDC.CreateCompatibleDC(pDC); 
		bitMap.CreateCompatibleBitmap(pDC, width, height); 
		pOldBitmap = (CBitmap*)memDC.SelectObject(&bitMap); 
		memDC.SelectObject(&bitMap); 
		memDC.FillSolidRect(&rect, RGB(255, 255, 255)); 

		int size = pBmpInfo->bmiHeader.biSizeImage;
		LPBYTE pImage = (LPBYTE)malloc(size);
		memcpy(pImage, pViewImg, size);
		int widthBytes = WIDTHBYTES(width * bitCount);
		for (int i = 0; i < height / 2; i++) {
			for (int j = 0; j < widthBytes; j++) {
				char temp = pImage[i*widthBytes + j];
				pImage[i*widthBytes + j] = pImage[(height - i - 1)* widthBytes + j];
				pImage[(height - i - 1)* widthBytes + j] = temp;
			}
		}

		SetDIBitsToDevice( 
			memDC.GetSafeHdc(), 
			0, 0,
			width, height, 
			0, 0,
			0, height, 
			pImage, pBmpInfo, DIB_RGB_COLORS);

		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(x,y, dstWidth, dstHeight, &memDC, 0, 0, width, height, SRCCOPY);

		free(pImage);
	} 


	if(pBmpInfo != NULL) 
	{ 
		delete [] pBmpInfo; 
		pBmpInfo = NULL;
	}
} 

/***************************************************************
bool SaveBMP ( BYTE* Buffer, int width, int height, 
		long paddedsize, LPCTSTR bmpfile )

Function takes a buffer of size <paddedsize> 
and saves it as a <width> * <height> sized bitmap 
under the supplied filename.
On error the return value is false.

***************************************************************/
bool SaveBMP(Image* src, LPCTSTR bmpfile)
{
	return SaveBMP2(
		src->imageData, 
		src->width, src->height, 
		bmpfile, src->channel*8);
}

bool SaveBMP2(BYTE* Buffer, int width, int height, LPCTSTR bmpfile, int bitCount)
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;

	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&info, 0, sizeof(BITMAPINFOHEADER));

	long widthBytes = WIDTHBYTES(width * bitCount);
	long bufSize = widthBytes * height;
	long palSize = ((bitCount == 8) ? 256 * sizeof(RGBQUAD) : 0);

	bmfh.bfType = 0x4d42; 
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bufSize + palSize;
	bmfh.bfOffBits = 0x36 + palSize;

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;		
	info.biBitCount = bitCount;	
	info.biCompression = 0;	
	info.biSizeImage = bufSize;	
	info.biXPelsPerMeter = 0x0ec4;	
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = (bitCount == 8) ? 256 : 0;
	info.biClrImportant = (bitCount == 8) ? 256 : 0;

	HANDLE file = CreateFile(bmpfile, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL)
	{
		CloseHandle(file);
		AfxMessageBox(_T("Error save bmp!(-1)"));
		return false;
	}

	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof(BITMAPFILEHEADER), &bwritten, NULL) == false)
	{
		CloseHandle(file);
		AfxMessageBox(_T("Error save bmp!(-2)"));
		return false;
	}

	if (WriteFile(file, &info, sizeof(BITMAPINFOHEADER), &bwritten, NULL) == false)
	{
		CloseHandle(file);
		AfxMessageBox(_T("Error save bmp!(-3)"));
		return false;
	}

	if (bitCount == 8)
	{
		RGBQUAD *bmiColors = (RGBQUAD*) new BYTE[(256 * sizeof(RGBQUAD))];
		for (int i = 0; i < 256; i++)
		{
			bmiColors[i].rgbBlue = i;
			bmiColors[i].rgbGreen = i;
			bmiColors[i].rgbRed = i;
			bmiColors[i].rgbReserved = 0;
		}
		if (WriteFile(file, bmiColors, (256 * sizeof(RGBQUAD)), &bwritten, NULL) == false)
		{
			CloseHandle(file);
			AfxMessageBox(_T("Error save bmp!(-4)"));
			return false;
		}
		delete bmiColors;
	}

	BYTE temp;
	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < widthBytes; j++) {
			temp = Buffer[i*widthBytes + j];
			Buffer[i*widthBytes + j] = Buffer[(height - i - 1)* widthBytes + j];
			Buffer[(height - i - 1)* widthBytes + j] = temp;
		}
	}

	if (WriteFile(file, Buffer, bufSize, &bwritten, NULL) == false)
	{
		CloseHandle(file);
		CString str;
		str.Format(_T("Error save bmp!(%d)"), GetLastError());
		AfxMessageBox(str);
		return false;
	}
	CloseHandle(file);

	return true;
}

/*******************************************************************
BYTE* LoadBMP ( int* width, int* height, long* size 
		LPCTSTR bmpfile )

The function loads a 24 bit bitmap from bmpfile, 
stores it's width and height in the supplied variables
and the whole size of the data (padded) in <size>
and returns a buffer of the image data 

On error the return value is NULL. 

  NOTE: make sure you [] delete the returned array at end of 
		program!!!
*******************************************************************/
Image* LoadBMP(LPCTSTR bmpfile)
{
	Image* image = new Image;
	image->imageData = LoadBMP2(&image->width, &image->height, &image->channel, bmpfile);
	if (image->imageData == NULL) {
		delete image;
		return NULL;
	}
	image->widthBytes = WIDTHBYTES((image->width) * (image->channel) * 8);

	return image;
}
BYTE* LoadBMP2(int* width, int* height, int* channel, LPCTSTR bmpfile)
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	DWORD bytesread;
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL;

	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	if (bmpheader.bfType != 'MB')
	{
		CloseHandle(file);
		return NULL;
	}

	*width = bmpinfo.biWidth;
	*height = abs(bmpinfo.biHeight);
	*channel = bmpinfo.biBitCount / 8;

	if (bmpinfo.biCompression != BI_RGB)
	{
		CloseHandle(file);
		return NULL;
	}
	if (bmpinfo.biBitCount != 24 && bmpinfo.biBitCount != 8)
	{
		CloseHandle(file);
		return NULL;
	}

	long size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[size];
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	if (ReadFile(file, Buffer, size, &bytesread, NULL) == false)
	{
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}

	CloseHandle(file);

	int widthBytes = WIDTHBYTES((*width) * bmpinfo.biBitCount);
	BYTE temp;
	for (int i = 0; i < *height / 2; i++) {
		for (int j = 0; j < widthBytes; j++) {
			temp = Buffer[i*widthBytes + j];
			Buffer[i*widthBytes + j] = Buffer[(*height - i - 1)* widthBytes + j];
			Buffer[(*height - i - 1)* widthBytes + j] = temp;
		}
	}
	
	return Buffer;
}