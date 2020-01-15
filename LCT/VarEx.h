struct	GRIDSET	{
		float nGap_X;
		float nGap_Y;
		int nSize_X;
		int nSize_Y;
		int nPos_X;
		int nPos_Y;
};
extern GRIDSET gGrid;
extern int gnXCnt;
extern int gnYCnt;
extern int gnGridON;
extern int gnScoreON;
extern int gnScoreMode;

struct	GPIXEL	{
		BYTE R;
		BYTE G;
		BYTE B;
};
extern GPIXEL gPixel[960][1280];
extern GPIXEL gPixelConv[960][1280];

extern int gnSkip[25][25];
extern int gnScore2;
extern int gnBlackValue;

extern int gnLogo;

struct	DISP_CNT	{
		long lTotal;
		long lOK;
		long lNG;
		long lEmpty;
};
extern DISP_CNT gDispCnt;
extern DISP_CNT gSaveCnt;
extern CString gcsModelName;
extern int m_nDispON;
extern int m_nGrabCnt;
extern int gnEmptyChk[20][20]; 

struct	BLUEC	{
		int nR;
		int nG;
		int nB;
};
extern BLUEC gBlueC;
extern BLUEC gBlackC;
extern BLUEC gBigC;
extern int nArray[1280][960];
extern int gnInBoxSize;
extern int gnInBoxSizeY;
extern int gnLensINScore;
extern int gnOKRangeMin;
extern int gnOKRangeMax;
extern int gnSetMode;
extern int gnNGval;
extern int gnBlueCntSet;

extern int gnBlueRChk;
extern int gnBlueGChk;
extern int gnBlueBChk;

