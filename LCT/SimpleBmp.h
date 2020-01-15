#include <windows.h>
#include <stdio.h>

typedef struct image{
	int width;
	int height;
	int channel;
	int widthBytes;
	BYTE* imageData;
}Image;

Image* CreateImage(int width, int height, int channel);
BYTE* CreateByte(int width, int height);

void FreeImage(Image* img);
void DrawImage(CWnd* pWnd, int x, int y, int dstWidth, int dstHeight, Image* pImage);
void DrawImage(CWnd* pWnd, Image* pImage);

BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, int height, long* newsize );
BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height );

bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile );
bool SaveBMP(Image* src, LPCTSTR bmpfile);
bool SaveBMP2(BYTE* Buffer, int width, int height, LPCTSTR bmpfile, int bitCount);

Image* LoadBMP(LPCTSTR bmpfile);
BYTE* LoadBMP2(int* width, int* height, int* channel, LPCTSTR bmpfile);

bool LoadBMPIntoDC2 ( HDC hDC, LPCTSTR bmpfile , int width, int height);

void DrawRawDataIntoDC(CDC* pDC, int x,int y, int dstWidth,int dstHeight, BYTE* pViewImg, int width,int height,int bitCount);


class SimpleBmp
{
	BITMAPINFO *m_pBmpInfo;
	CDC m_memDC;
	LPBYTE m_pImage;
	BOOL m_Init;
	CDC* m_pDC;
	CWnd* m_pWnd;
	CRect m_rect;
	BOOL isDrawing;

public:
	SimpleBmp();
	~SimpleBmp();

	void Draw(CWnd*  pWnd, Image* pImage);
	void Draw(CDC* pDC, int x, int y, int dstWidth, int dstHeight, BYTE* pViewImg, int width, int height, int bitCount);
	void SetHeaderInfo(int width, int height, int bitCount);
};
