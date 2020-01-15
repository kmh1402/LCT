// Frm_Main.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LCT.h"
#include "Frm_Main.h"
#include "Dlg_Set.h"
#include "VarEx.h"
#include "Var.h"
#include "FuncEx.h"
#include "func.h"
#include "Dlg_Model.h"


// CFrm_Main
CInstantCamera *m_pCamera;
// This smart pointer will receive the grab result data.
CGrabResultPtr ptrGrabResult;

int m_nLiveON;
Image* m_pImage;
BOOL m_bThreadFlag;
UINT ThreadImageCaptureFunc(LPVOID param);

IMPLEMENT_DYNCREATE(CFrm_Main, CFormView)

CFrm_Main::CFrm_Main()
	: CFormView(CFrm_Main::IDD)
	
	, m_GridON(0)
	, m_ScoreON1(0)
	, m_Result1(0)
{

}

CFrm_Main::~CFrm_Main()
{
}

void CFrm_Main::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISPLAY, m_Display);
	DDX_Radio(pDX, IDC_GRID1, m_GridON);
	DDX_Radio(pDX, IDC_SCORE_ON1, m_ScoreON1);
	DDX_Radio(pDX, IDC_RESULTV1, m_Result1);
	DDX_Control(pDX, IDC_DISP_RESULT1, m_Disp_Result1);
	DDX_Control(pDX, IDC_DISP_RESULT2, m_Disp_Result2);
	DDX_Control(pDX, IDC_SKIP, m_Skip_Disp);
	DDX_Control(pDX, IDC_DISP_LOGO, m_DispLogo);
	DDX_Control(pDX, IDC_MODEL_NAME, m_Model_Name);
	

	
}

BEGIN_MESSAGE_MAP(CFrm_Main, CFormView)
	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_BUTTON3, &CFrm_Main::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFrm_Main::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFrm_Main::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_SET_BTN, &CFrm_Main::OnBnClickedSetBtn)
	//ON_BN_CLICKED(IDC_GRID1, &CFrm_Main::OnBnClickedGrid1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_GRID1 , IDC_GRID2, OnBnClickedGrid1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SCORE_ON1 , IDC_SCORE_ON2, OnBnClickedScoreOn1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RESULTV1 , IDC_RESULTV3 , OnBnClickedResultOn1)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON7, &CFrm_Main::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CFrm_Main::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CFrm_Main::OnBnClickedButton10)
	
	ON_BN_CLICKED(IDC_MODEL_NAME, &CFrm_Main::OnBnClickedModelName)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CFrm_Main::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFrm_Main::OnBnClickedButton2)
END_MESSAGE_MAP()


// CFrm_Main 진단입니다.

#ifdef _DEBUG
void CFrm_Main::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFrm_Main::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFrm_Main 메시지 처리기입니다.


void CFrm_Main::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DirectoryCreate("D:\\LCT\\LCT\\Setting\\");
	for(int nY = 0; nY < 20; nY++)
	{
		for(int nX = 0; nX < 20; nX++)
		{
			gnSkip[nX][nY] = 0;
		}
	}

	m_nResultDisp = 0;
	gDispCnt.lEmpty = 0;
	gDispCnt.lOK = 0;
	gDispCnt.lNG = 0;
	gDispCnt.lTotal = 0;
	gnLogo = 0;
	m_nLiveON = 0;
	m_nGrabCnt = 0;
	m_nDispON = 0;
	MotionDataLoad();
	SetDlgItemText(IDC_MODEL_NAME, gcsModelName);
	m_GridON = gnGridON;
	m_ScoreON1 = gnScoreON;
	m_Result1 = gnScoreMode;
	
	UpdateData(FALSE);
	OnCamInit();	
	
	SetTimer(1,1,NULL);
	SetTimer(2,1000,NULL);
	SetTimer(3,200,NULL);
}

void CFrm_Main::RectSkip(CDC* pDC)
{
	int nX = 0;
	int nY = 0;
	int nOffSetX = 0;
	int nOffSetY = 0;
	int nJ = 0;
	int nI = 0;
	int nXPos = 0;
	int nYPos = 0;

	bool bChange = 0;
	int nConvX;
	int nStatus;
	CString csData;

	CPen myPen;
	CPen* pOldPen;
	
	myPen.CreatePen(0,1, RGB(0,0,0));
	pOldPen = pDC->SelectObject(&myPen);

	nOffSetX = 16;
	nOffSetY = 25;

	nX = 910;
	nY = 110;

	for(int nK = 0; nK < gnYCnt; nK ++)
	{
		for(int nL = 0; nL < gnXCnt; nL ++)
		{
			//Dir_Convert(nX, &nConvX);//오른족에서 부터 0으로 시작할경우
			nConvX = nL;//왼쪽에서 부터 0 으로 시작할경우 
			nXPos = nX + (nOffSetX * nL) + (2 * nL);
			nYPos = nY + (nOffSetY * nK) + (2 * nK);
			
			
			nStatus = gnSkip[nL][nK];
				
			if(nStatus == 0)
			{
				pDC->FillSolidRect(nXPos, nYPos, nOffSetX, nOffSetY,RGB(0,0,255));
			}
			else
			{
				pDC->FillSolidRect(nXPos, nYPos, nOffSetX, nOffSetY,RGB(50,50,50));
			}		
		}
	}

	myPen.DeleteObject();
	pOldPen->DeleteObject();
}

void CFrm_Main::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//return;
	if(nIDEvent == 1)
	{
		
		if(m_nLiveON == 0)
		{
			//if (m_pImage != NULL && m_bThreadFlag == FALSE) 
			if (m_ngrab == 1 && m_bThreadFlag == FALSE) 
			{
				//m_pImage = NULL;
				
				CDC* pDC;
				pDC = m_Display.GetDC();
				ImageCalc2();
				DrawImage(&m_Display, m_pImage);
				Rect1(pDC);
				ReleaseDC(pDC);
				//m_pImage = NULL;
				m_ngrab = 0;
				
				/*
				CDC* pDCBit;
				pDCBit = m_DispLogo.GetDC();
				LoadBitMap(pDCBit);
				ReleaseDC(pDCBit);
			*/
				CDC* pDCResult1;
				pDCResult1 = m_Disp_Result1.GetDC();
				Rect_Result1(pDCResult1);
				ReleaseDC(pDCResult1);
			
				
				
				CDC* pDCResult2;
				pDCResult2 = m_Disp_Result2.GetDC();
				Rect_Result2(pDCResult2);
				ReleaseDC(pDCResult2);
			}
		}
		else
		{
			
			if (m_pImage != NULL && m_bThreadFlag == TRUE) 
			{			
				CDC* pDCR;
				pDCR = m_Display.GetDC();
				//ImageCalc2();
				DrawImage(&m_Display, m_pImage);
				//Rect1(pDCR);
				RectRoi(pDCR);
				ReleaseDC(pDCR);
				
			}
		}

		if(m_nResultDisp == 1)
		{
			CDC* pDCResult1;
			pDCResult1 = m_Disp_Result1.GetDC();
			Rect_Result1(pDCResult1);
			ReleaseDC(pDCResult1);
			
			CDC* pDCResult2;
			pDCResult2 = m_Disp_Result2.GetDC();
			Rect_Result2(pDCResult2);
			ReleaseDC(pDCResult2);
		}
		
		
	}

	if(nIDEvent == 2)
	{
		CDC* pDCBit;
		pDCBit = m_DispLogo.GetDC();
		LoadBitMap(pDCBit);
		ReleaseDC(pDCBit);
		KillTimer(2);
	}

	if(nIDEvent == 3)
	{
		CDC* pDC;
		pDC = GetDC();
		RectSkip(pDC);
		ReleaseDC(pDC);
	}
	CFormView::OnTimer(nIDEvent);
}

void CFrm_Main::RectRoi(CDC* pDC)
{
	CPen myPen;
	CPen* pOldPen;
	RECT rt;
	int var = 0;	
	CString csData;

	UpdateData(TRUE);	
	myPen.CreatePen(PS_SOLID,1, RGB(255,0,0));
	pOldPen = pDC->SelectObject(&myPen);

	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			////////////////////////////////////////////////////////////////
			if(gnGridON == 0) //ROI BOX그려주기
			{
				if(nX == 0 && nY == 0) //좌표계산
				{
					if(gnSkip[nX][nY] == 0)
					{
						pDC->Draw3dRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y, RGB(150,150,255), RGB(0,0,255));
					}
					else
					{
						pDC->FillSolidRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y,RGB(0,0,0));			
					}
				}
				else
				{
					if(gnSkip[nX][nY] == 0)
					{
						pDC->Draw3dRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y, RGB(150,150,255), RGB(0,0,255));
					}
					else
					{
						pDC->FillSolidRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y,RGB(0,0,0));
					}
				}
			}			
		}
	}
	myPen.DeleteObject();
	pOldPen->DeleteObject();
}

void CFrm_Main::Rect1(CDC* pDC)
{
	CPen myPen;
	CPen* pOldPen;
	RECT rt;
	int var = 0;
	int nX1 = 0;
	int nX2 = 0;
	int nY1 = 0;
	int nY2 = 0;
	int nXConvert = 0;
	int nYConvert = 0;
	int nCnt = 0;
	
	int nBlueCnt[20][20];
	int nBlackCnt = 0;
	int nXDir = 0;
	int nYDir = 0;
	int nData = 0;
	int nMax = 0;
	int nMin = 10000;
	int nMinX[20][20];
	int nMaxX[20][20];
	int nMinY[20][20];
	int nMaxY[20][20];
	int nAVGX[20][20];
	int nAVGY[20][20];
	CString csData;

	
	UpdateData(TRUE);
	
	if(m_nLiveON == 0)
	{
		gDispCnt.lEmpty = 0;
		gDispCnt.lOK = 0;
		gDispCnt.lNG = 0;
		gDispCnt.lTotal = 0;
	}

	for(int nY = 0; nY < 960; nY++)
	{
		for(int nX = 0; nX < 1280; nX++)
		{
			nArray[nX][nY] = 0;
		}
	}

	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			
			nMinX[nX][nY] = 10000;
			nMaxX[nX][nY] = 0;
			nMinY[nX][nY] = 10000;
			nMaxY[nX][nY] = 0;
			nAVGX[nX][nY] = 0;
			nAVGY[nX][nY] = 0;
			
		}
	}

	
	for (int nY = 60; nY < 960; nY++)//카메라 영역기준-> 화면영역으로 변환   
	{
		for (int nX = 20; nX < 1260; nX++) 
		{
			nXDir = nX * 0.7;
			nYDir = nY * 0.64;
			gPixelConv[nYDir][nXDir].R = gPixel[nY][nX].R;
			gPixelConv[nYDir][nXDir].G = gPixel[nY][nX].G;
			gPixelConv[nYDir][nXDir].B = gPixel[nY][nX].B;
			
			//매칭 TEST
			/*
			if(gPixelConv[nYDir][nXDir].R > 250 && gPixelConv[nYDir][nXDir].G < 120 && gPixelConv[nYDir][nXDir].B < 120)
			{
				pDC->SetPixel(nXDir, nYDir,RGB(255,0,0));
				//nXDir = nX * 0.7;
				//nYDir = nY * 0.6;
				//m_Array[nYDir][nXDir] = 1;
			}
			
			
			var = (BYTE) ((gPixelConv[nY][nX].R+gPixelConv[nY][nX].G+gPixelConv[nY][nX].B) / 3.0);
			if(var > gnBlackValue)
			{
				m_Array[nY][nX] = 1;
			}
			*/
		}
	}	
	myPen.CreatePen(PS_SOLID,1, RGB(255,0,0));
	pOldPen = pDC->SelectObject(&myPen);

	int nCnt1 = 0;
	int nCnt2 = 0;
	long lData1 = 0;
	long lData2 = 0;
	int nRelayCnt = 0;
	int nTCnt = 0;
	int nTCnt2 = 0;
	int nChkBoxCnt = 0;
	int nLineCnt[20][20];
	bool bCheck = false;
	int nCheckCnt = 0;
	int nPair = 0;
	int nWhiteCheck = 0;
	
	//Lens 의 중심을 찾는작업 
	for(int nYIndex = 0; nYIndex < gnYCnt;nYIndex++)
	{
		for(int nXIndex = 0; nXIndex < gnXCnt;nXIndex++)
		{
			nCnt = 0; 
			for(int nY = gGrid.nPos_Y + (gGrid.nSize_Y * nYIndex) + (gGrid.nGap_Y * nYIndex); nY < gGrid.nPos_Y + (gGrid.nSize_Y * (nYIndex + 1)) + (gGrid.nGap_Y * nYIndex); nY++)
			{	
				for(int nX = gGrid.nPos_X + (gGrid.nSize_X * nXIndex) + (gGrid.nGap_X * nXIndex); nX < gGrid.nPos_X + (gGrid.nSize_X * (nXIndex + 1)) + (gGrid.nGap_X * nXIndex); nX++)
				{//정방향으로 Check 
					//pDC->SetPixel(nX, nY,RGB(255,0,0));	
					
					var = (BYTE) ((gPixelConv[nY][nX].R+gPixelConv[nY][nX].G+gPixelConv[nY][nX].B) / 3.0);
					//if(gPixelConv[nY][nX].B > 180 && gPixelConv[nY][nX].G > 180 && gPixelConv[nY][nX].R > 180)
					if(var > 20) //트레이보다 밝은 값을 선정하여 Lens검출
					{
						if(nMinX[nXIndex][nYIndex] > nX)
						{
							nMinX[nXIndex][nYIndex] = nX;
						}
						//nX = gGrid.nPos_X + (gGrid.nSize_X * (nXIndex + 1)) + (gGrid.nGap_X * nXIndex);
					}
				}

				for(int nX = gGrid.nPos_X + (gGrid.nSize_X * (nXIndex + 1)) + (gGrid.nGap_X * nXIndex); nX > gGrid.nPos_X + (gGrid.nGap_X * nXIndex) + (gGrid.nSize_X * nXIndex); nX--)
				{//정방향으로 Check 
					//pDC->SetPixel(nX, nY,RGB(255,0,0));					
					var = (BYTE) ((gPixelConv[nY][nX].R+gPixelConv[nY][nX].G+gPixelConv[nY][nX].B) / 3.0);
					//if(gPixelConv[nY][nX].B > 180 && gPixelConv[nY][nX].G > 180 && gPixelConv[nY][nX].R > 180)
					if(var > 20)
					{
						if(nMaxX[nXIndex][nYIndex] < nX)
						{
							nMaxX[nXIndex][nYIndex] = nX;
						}
						//nX = gGrid.nPos_X + (gGrid.nGap_X * nXIndex) + (gGrid.nGap_X * nXIndex);
					}
				}
				nCnt++;
			}

			nCnt = 0; 
			for(int nX = gGrid.nPos_X + (gGrid.nSize_X * nXIndex) + (gGrid.nGap_X * nXIndex); nX < gGrid.nPos_X + (gGrid.nSize_X * (nXIndex + 1)) + (gGrid.nGap_X * nXIndex); nX++)
			{
				for(int nY = gGrid.nPos_Y + (gGrid.nSize_Y * nYIndex) + (gGrid.nGap_Y * nYIndex); nY < gGrid.nPos_Y + (gGrid.nSize_Y * (nYIndex + 1)) + (gGrid.nGap_Y * nYIndex); nY++)
				{//정방향으로 Check 
					//pDC->SetPixel(nX, nY,RGB(255,0,0));					
					var = (BYTE) ((gPixelConv[nY][nX].R+gPixelConv[nY][nX].G+gPixelConv[nY][nX].B) / 3.0);
					//if(gPixelConv[nY][nX].B > 180 && gPixelConv[nY][nX].G > 180 && gPixelConv[nY][nX].R > 180)
					if(var > 20)
					{
						if(nMinY[nXIndex][nYIndex] > nY)
						{
							nMinY[nXIndex][nYIndex] = nY;
						}
						//nY = gGrid.nPos_Y + (gGrid.nSize_Y * (nYIndex + 1)) + (gGrid.nGap_Y * nYIndex);
					}
				}

				for(int nY = gGrid.nPos_Y + (gGrid.nSize_Y * (nYIndex + 1)) + (gGrid.nGap_Y * nYIndex); nY > gGrid.nPos_Y + (gGrid.nGap_Y * nYIndex) + (gGrid.nSize_Y * nYIndex); nY--)
				{//정방향으로 Check 
					//pDC->SetPixel(nX, nY,RGB(255,0,0));					
					var = (BYTE) ((gPixelConv[nY][nX].R+gPixelConv[nY][nX].G+gPixelConv[nY][nX].B) / 3.0);
					//if(gPixelConv[nY][nX].B > 180 && gPixelConv[nY][nX].G > 180 && gPixelConv[nY][nX].R > 180)
					if(var > 20)
					{
						if(nMaxY[nXIndex][nYIndex] < nY)
						{
							nMaxY[nXIndex][nYIndex] = nY;
						}
						//nY = gGrid.nPos_Y + (gGrid.nGap_Y * nYIndex) + (gGrid.nGap_Y * nYIndex);
					}
				}
				nCnt++;
			}
		}
		
	}
	
	//찾은 Lens중심의 Box를 그려준다 
	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			nAVGX[nX][nY] = (nMaxX[nX][nY] + nMinX[nX][nY]) / 2;
			nAVGY[nX][nY] = (nMaxY[nX][nY] + nMinY[nX][nY]) / 2;

			if(gnScoreON == 0)
			{
				pDC->Draw3dRect(nAVGX[nX][nY] - gnInBoxSize+1 , nAVGY[nX][nY] - gnInBoxSizeY-2 + 2, (gnInBoxSize * 2)+1, (gnInBoxSizeY * 2) + 2 +2, RGB(50,50,255), RGB(0,0,255));
			}
		}
	}

	//Lens 중심의 수치를 계산한다. 
	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			nLineCnt[nX][nY] = 0;
			gnEmptyChk[nX][nY] = 0;
			
			nX1 = nAVGX[nX][nY] - gnInBoxSize;
			nX2 = nAVGX[nX][nY] + gnInBoxSize;
			nY1 = nAVGY[nX][nY] - gnInBoxSizeY;
			nY2 = nAVGY[nX][nY] + gnInBoxSizeY;

			if(nX1 < 1000 && nX2 < 1000 && nY1 < 1000 && nY2 < 1000)
			{
				nTCnt = 0;
				bCheck = false;
				for(int nXX = nX1+1; nXX < nX2+1; nXX++)
				{
					nRelayCnt = 0;
					bCheck = false;
					for(int nYY = nY1 -1; nYY < nY2+3; nYY++)
					{
						//pDC->SetPixel(nXX, nYY,RGB(255,0,0));	
						var = (BYTE) ((gPixelConv[nYY][nXX].R+gPixelConv[nYY][nXX].G+gPixelConv[nYY][nXX].B) / 3.0);
						
						if(gPixelConv[nYY][nXX].R > gBlueC.nR && gPixelConv[nYY][nXX].G < gBlueC.nG	&& gPixelConv[nYY][nXX].B < gBlueC.nB)
						{
							nLineCnt[nX][nY]++;
						}

						if(gPixelConv[nYY][nXX].R > gBlackC.nR && gPixelConv[nYY][nXX].G > gBlackC.nG && gPixelConv[nYY][nXX].B > gBlackC.nB)
						{
							nLineCnt[nX][nY]++;
						}

						/*
						if(gnSetMode == 1)
						{
						
							if((nX == gnXCnt -1 && nY == 0) || (nX == 0 && nY == 0) || (nX == 0 && nY == gnYCnt -1)
								|| (nX == gnXCnt -1 && nY == gnYCnt -1) || (nX == gnXCnt -1 && nY == 1) || (nX == 0 && nY == 1)
								|| (nX == gnXCnt -1 && nY == gnYCnt -2) || (nX == 0 && nY == gnYCnt -2)
								|| (nX == gnXCnt -1 && nY == gnYCnt -3) || (nX == 0 && nY == gnYCnt -3)
								|| (nX == gnXCnt -1 && nY == 2) || (nX == 0 && nY == 2)
								|| (nX == gnXCnt -1 && nY == 3) || (nX == 0 && nY == 3)
								|| (nX == gnXCnt -1 && nY == 4) || (nX == 0 && nY == 4)
								|| (nX == gnXCnt -1 && nY == 5) || (nX == 0 && nY == 5)
								|| (nX == gnXCnt -1 && nY == 6) || (nX == 0 && nY == 6)
								|| (nX == gnXCnt -1 && nY == 7) || (nX == 0 && nY == 7))
							{
								if(var > gnBlackValue - 50)
								{
									nCnt++;
								}
							}
							else
							{
								if(var > gnBlackValue)
								{
									nCnt++;
								}
							}
						}
						*/
					}
				}
			}
		}
	}
	int nNGCnt = 0;
	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			/*
			if(gnSetMode == 1)
			{
				nLineCnt[nX][nY] = 0;
				nBlueCnt[nX][nY] = 0;
			}
			else
			{
				nBlueCnt[nX][nY] = 0;
			}
			*/
			nBlueCnt[nX][nY] = 0;

			
			if(nX == 0 && nY == 0) //좌표계산
			{
				nX1 = gGrid.nPos_X;
				nX2 = gGrid.nPos_X + gGrid.nSize_X;
				nY1 = gGrid.nPos_Y;
				nY2 = gGrid.nPos_Y + gGrid.nSize_Y;
			}
			else
			{
				nX1 = gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X));
				nX2 = gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) + gGrid.nSize_X;
				nY1 = gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y));
				nY2 = gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)) + gGrid.nSize_Y;
			}

			nX1 = nAVGX[nX][nY] - gnInBoxSize;
			nX2 = nAVGX[nX][nY] + gnInBoxSize;
			nY1 = nAVGY[nX][nY] - gnInBoxSizeY;
			nY2 = nAVGY[nX][nY] + gnInBoxSizeY;
			
			nCnt = 0;
			
			nBlackCnt = 0;
			nChkBoxCnt = 0;
			nNGCnt = 0;
			if(nX1 < 1000 && nX2 < 1000 && nY1 < 1000 && nY2 < 1000)
			{
				for(int nYY = nY1; nYY < nY2; nYY++)
				{
					for(int nXX = nX1; nXX < nX2; nXX++)
					{
					
						var = (BYTE) ((gPixelConv[nYY][nXX].R+gPixelConv[nYY][nXX].G+gPixelConv[nYY][nXX].B) / 3.0);
						//pDC->SetPixel(nXX, nYY,RGB(255,0,0));

					
						if((nX == gnXCnt -1 && nY == 0) || (nX == 0 && nY == 0) || (nX == 0 && nY == gnYCnt -1)
							|| (nX == gnXCnt -1 && nY == gnYCnt -1) || (nX == gnXCnt -1 && nY == 1) || (nX == 0 && nY == 1)
							|| (nX == gnXCnt -1 && nY == gnYCnt -2) || (nX == 0 && nY == gnYCnt -2)
							|| (nX == gnXCnt -1 && nY == gnYCnt -3) || (nX == 0 && nY == gnYCnt -3)
							|| (nX == gnXCnt -1 && nY == 2) || (nX == 0 && nY == 2)
							|| (nX == gnXCnt -1 && nY == 3) || (nX == 0 && nY == 3)
							|| (nX == gnXCnt -1 && nY == 4) || (nX == 0 && nY == 4)
							|| (nX == gnXCnt -1 && nY == 5) || (nX == 0 && nY == 5)
							|| (nX == gnXCnt -1 && nY == 6) || (nX == 0 && nY == 6)
							|| (nX == gnXCnt -1 && nY == 7) || (nX == 0 && nY == 7))
						{
							if(var > gnBlackValue - 50)
							{
								nCnt++;
							}
						}
						else
						{
							if(var > gnBlackValue)
							{
								nCnt++;
							}
						}
					

						if(gPixelConv[nYY][nXX].B > gnBlueBChk && gPixelConv[nYY][nXX].G < gnBlueGChk && gPixelConv[nYY][nXX].R < gnBlueRChk)
						{
							nBlueCnt[nX][nY]++;
						}
					
					}
				}
			}
			////////////////////////////////////////////////////////////////
			if(gnGridON == 0) //ROI BOX그려주기
			{
				if(nX == 0 && nY == 0) //좌표계산
				{
					if(gnSkip[nX][nY] == 0)
					{
						
						if(gnScore2 < nCnt)
						{					
							if(m_nLiveON == 0)
							{
								if(nLineCnt[nX][nY] > gnLensINScore)
								{
									gDispCnt.lEmpty++;
									pDC->Draw3dRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y, RGB(0,0,0), RGB(0,0,255));
								}
								else
								{
									if(nBlueCnt[nX][nY] < gnBlueCntSet)
									{
										gDispCnt.lOK++;
										pDC->Draw3dRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y, RGB(1,1,1), RGB(40,150,40));
									}
									else
									{
										gDispCnt.lNG++;
										pDC->Draw3dRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y, RGB(0,0,0), RGB(255,0,0));
									}
								}
							}
						}
						else
						{
							if(m_nLiveON == 0)
							{
								if(nLineCnt[nX][nY] > gnLensINScore)
								{
									gDispCnt.lEmpty++;
									pDC->Draw3dRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y, RGB(0,0,0), RGB(0,0,255));
								}
								else
								{
										
									gDispCnt.lNG++;
									pDC->Draw3dRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y, RGB(0,0,0), RGB(255,0,0));
										
								}
							}
						}
					}
					else
					{
						pDC->FillSolidRect(gGrid.nPos_X , gGrid.nPos_Y, gGrid.nSize_X, gGrid.nSize_Y,RGB(0,0,0));				
					}
				}
				else
				{
					if(gnSkip[nX][nY] == 0)
					{
						
						if(gnScore2 < nCnt)
						{
							
							if(m_nLiveON == 0)
							{
								if(nLineCnt[nX][nY] > gnLensINScore)
								{
									gDispCnt.lEmpty++;
									pDC->Draw3dRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y, RGB(50,50,255), RGB(0,0,255));	
								}
								else
								{
									if(nBlueCnt[nX][nY] < gnBlueCntSet)
									{
										gDispCnt.lOK++;	
										pDC->Draw3dRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y, RGB(1,1,1), RGB(40,150,40));
									}
									else
									{
										gDispCnt.lNG++;
										pDC->Draw3dRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y, RGB(255,50,50), RGB(255,0,0));	
									}
								}
							}
						}
						else
						{
							if(m_nLiveON == 0)
							{
								if(nLineCnt[nX][nY] > gnLensINScore)
								{
									gDispCnt.lEmpty++;
									pDC->Draw3dRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y, RGB(50,50,255), RGB(0,0,255));	
								}
								else
								{
										
									gDispCnt.lNG++;
									pDC->Draw3dRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y, RGB(255,50,50), RGB(255,0,0));	
										
								}
							}
						}
					}
					else
					{
						pDC->FillSolidRect(gGrid.nPos_X + ((gGrid.nGap_X * nX) + (nX * gGrid.nSize_X)) , gGrid.nPos_Y + ((gGrid.nGap_Y * nY) + (nY * gGrid.nSize_Y)), gGrid.nSize_X, gGrid.nSize_Y,RGB(0,0,0));
					}
				}
			}
			
			if(gnScoreON == 0)
			{
				
				if(gnScoreMode == 0)
				{
					if(gnSkip[nX][nY] == 0)
					{
						csData.Format("%d", nCnt);
						pDC->SetTextColor(RGB(255,255,0));
						pDC->TextOutA(nX1,nY1 ,csData);//, DT_VCENTER);
					}
				}
				else if(gnScoreMode == 1)
				{
					if(gnSkip[nX][nY] == 0)
					{
						
						csData.Format("%d", nLineCnt[nX][nY]);
						//csData.Format("%d", nBlueCnt[nX][nY]);
						pDC->SetTextColor(RGB(255,255,0));
						pDC->TextOutA(nX1,nY1 ,csData);//, DT_VCENTER);
					}
				}
				else if(gnScoreMode == 2)
				{
					if(gnSkip[nX][nY] == 0)
					{
						csData.Format("%d", nBlueCnt[nX][nY]);
						pDC->SetTextColor(RGB(255,255,0));
						pDC->TextOutA(nX1,nY1 ,csData);//, DT_VCENTER);
					}
				}
			}
		}
	}
	
	if(m_nLiveON == 0)
	{
		gDispCnt.lTotal = gDispCnt.lOK + gDispCnt.lNG;
		gSaveCnt.lOK = gSaveCnt.lOK + gDispCnt.lOK;
		gSaveCnt.lNG = gSaveCnt.lNG + gDispCnt.lNG;
		gSaveCnt.lEmpty = gSaveCnt.lEmpty + gDispCnt.lEmpty;
		gSaveCnt.lTotal = gSaveCnt.lOK + gSaveCnt.lNG;
	}
	
	myPen.DeleteObject();
	pOldPen->DeleteObject();

	CString csCnt;
	csCnt.Format("%d, %d, %d, %d", gDispCnt.lTotal, gDispCnt.lOK, gDispCnt.lNG, gDispCnt.lEmpty);
	FileSave(csCnt);
}

void CFrm_Main::ImageCalc()
{
	BYTE B,G,R;
	int len = min(m_pImage->width, m_pImage->height);
	int x = 0, y = 0;
	//int var;

	if (m_pImage->channel == 3) 
	{      // 칼라 영상
		/*
		for (int i = 0; i < len / 2; i++, x++, y++) 
		{
			B = m_pImage->imageData[y*m_pImage->widthBytes + x * 3 + 0];// = 255; // B
			G = m_pImage->imageData[y*m_pImage->widthBytes + x * 3 + 1];// = 0; // G
			R = m_pImage->imageData[y*m_pImage->widthBytes + x * 3 + 2];// = 0; // R
			gPixel[x][y].B = B;
			gPixel[x][y].G = G;
			gPixel[x][y].R = R;
		}
		*/
		for (int nY = 0; nY < m_pImage->height; nY++) 
		{
			for (int nX = 0; nX < m_pImage->width; nX++) 
			{
				B = m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 0];// = 255; // B
				G = m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 1];// = 0; // G
				R = m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 2];// = 0; // R
				gPixel[nY][nX].B = B;
				gPixel[nY][nX].G = G;
				gPixel[nY][nX].R = R;
				/*
				var = (BYTE) ((R+G+B) / 3.0);
				if(nX > (gGrid.nPos_X / 0.6932) && nX < (gGrid.nPos_X / 0.6932) + (gGrid.nSize_X / 0.6932) && nY > (gGrid.nPos_Y / 0.57714) && nY < (gGrid.nPos_Y / 0.57714) + (gGrid.nSize_Y / 0.57714))
				{
					m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 0] = 0; // B
					m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 1] = 0; // G
					m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 2] = 255; // R
				}
				*/
			}
		}
	}
}

void CFrm_Main::ImageCalc2()
{
	BYTE B,G,R;
	int len = min(m_pImage->width, m_pImage->height);
	int x = 0, y = 0;
	//int var;

	if (m_pImage->channel == 3) 
	{      // 칼라 영상
		for (int nY = 0; nY < m_pImage->height; nY++) 
		{
			for (int nX = 0; nX < m_pImage->width; nX++) 
			{
				B = m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 0];// = 255; // B
				G = m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 1];// = 0; // G
				R = m_pImage->imageData[nY*m_pImage->widthBytes + nX * 3 + 2];// = 0; // R
				gPixel[nY][nX].B = B;
				gPixel[nY][nX].G = G;
				gPixel[nY][nX].R = R;
			}
		}
	}
}

void CFrm_Main::OnCamInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PylonInitialize();

	CTlFactory& tlFactory = CTlFactory::GetInstance();
	DeviceInfoList_t devices;

	if (tlFactory.EnumerateDevices(devices) == 0)
	{
		MessageBox("카메라를 찾을 수 없습니다.");
		//AfxGetMainWnd()->PostMessage(WM_CLOSE);
	}
	else 
	{

		m_pCamera = new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());
		// Print the model name of the m_pCamera->
		std::cout << "Using device " << m_pCamera->GetDeviceInfo().GetModelName() << std::endl;
		INodeMap *m_pNodeMap;

		try
		{
			if (m_pCamera->IsOpen())
			{
				MessageBox("카메라가 이미 연결됨!");
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
			}
			else 
			{
				m_pCamera->GrabCameraEvents = true;
				m_pCamera->Attach(tlFactory.CreateDevice(devices[0]));

				if (m_pCamera->IsPylonDeviceAttached())
				{
					m_pCamera->Open();

					m_pCamera->MaxNumBuffer = 5;
					m_pNodeMap = &m_pCamera->GetNodeMap();
					CIntegerPtr offsetX(m_pNodeMap->GetNode("OffsetX"));
					CIntegerPtr offsetY(m_pNodeMap->GetNode("OffsetY"));
					CIntegerPtr width(m_pNodeMap->GetNode("Width"));
					CIntegerPtr height(m_pNodeMap->GetNode("Height"));
				}
				//MessageBox(_T("카메라 연결 성공!"));
			}
		}
		catch (GenICam::GenericException &e) //Error Handling
		{
			// Error handling
			CString strTrace;
			strTrace.Format(_T("Open_Camera - GenericException : %s\n"), (CString)e.GetDescription());
			AfxMessageBox(strTrace);
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
		}
	}
}

void CFrm_Main::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pCamera == NULL) {
		MessageBox("카메라 연결 실패");
		return;
	}

	if(m_nLiveON == 1)
	{
		return;
	}
	
	m_nLiveON = 1;
	m_pCamera->StartGrabbing();

	m_bThreadFlag = TRUE;
	CWinThread *pThread = ::AfxBeginThread(ThreadImageCaptureFunc, this);
	
	
}


void CFrm_Main::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pCamera == NULL) {
		MessageBox("카메라 연결 실패");
		return;
	}
	m_nLiveON = 0;
	m_pCamera->StopGrabbing();
	m_bThreadFlag = FALSE;

	
}


UINT ThreadImageCaptureFunc(LPVOID param)
{
	int nCnt = 0;
	//CFrm_Main *pDlg = (CFrm_Main *)param;
	if (m_pCamera == NULL) return 0;

	CImageFormatConverter fc;
	fc.OutputPixelFormat = PixelType_BGR8packed;
	CPylonImage image;
	while (m_bThreadFlag)
	{
		
		try
		{
			if (m_pCamera->IsGrabbing())
			{
				m_pCamera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

				if (ptrGrabResult->GrabSucceeded())
				{
					if(m_nLiveON == 0)
					{
						nCnt++;
						if(nCnt > 1)
						{
							m_pCamera->StopGrabbing();
							m_bThreadFlag = FALSE;
							nCnt = 0;
						}
					}
					
					const uint8_t *pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();

#ifdef PYLON_WIN_BUILD
					// Display the grabbed image.
					//Pylon::DisplayImage(0, ptrGrabResult);
#endif
					if (m_pImage == NULL) {
						m_pImage = CreateImage(ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(), 3);
					}
					fc.Convert(image, ptrGrabResult);
					memcpy(m_pImage->imageData, (uint8_t*)image.GetBuffer(), ptrGrabResult->GetWidth() * ptrGrabResult->GetHeight() * 3);
					m_ngrab = 1;
				}
				//pDlg->Invalidate(FALSE);
				
				/*
				if(m_nLiveON == 0 && nCnt > 3)
				{
					nCnt = 0;
					m_pCamera->StopGrabbing();
					m_bThreadFlag = FALSE;
					
				}
				*/
			}

		}
		catch (GenICam::GenericException &e)
		{
			// Error handling.
			cerr << "An exception occurred." << endl << e.GetDescription() << endl;
		}


	}
	
	
	return 0;
}

void CFrm_Main::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag = FALSE;
	PylonTerminate();
	//delete(m_pImage);
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CFrm_Main::LoadBitMap(CDC* pDC)
{
	CString csFileName;
	CDC memDC;
	CBitmap bmp, *pOldBmp;
	BITMAP bm;
	CRect rect;

	if(gnLogo == 0)
	{
		csFileName = "D:\\LCT\\LCT\\LOGO1.bmp";
	}
	else
	{
		csFileName = "D:\\LCT\\LCT\\LOGO2.bmp";
	}
	bmp.m_hObject = (HBITMAP)LoadImage(NULL,csFileName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(bmp.m_hObject == NULL) return;

	bmp.GetBitmap(&bm);
	memDC.CreateCompatibleDC(pDC);
	pOldBmp = memDC.SelectObject(&bmp);

	pDC->BitBlt(1, 1, bm.bmWidth, bm.bmHeight - 20, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
}

void CFrm_Main::Rect_Result1(CDC* pDC)
{
	
	
	CString csFileName;
	CDC memDC;
	CBitmap bmp, *pOldBmp;
	BITMAP bm;
	CRect rect;
	
	
	csFileName = "D:\\LCT\\LCT\\BACK.bmp";
	
	bmp.m_hObject = (HBITMAP)LoadImage(NULL,csFileName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(bmp.m_hObject == NULL) return;

	bmp.GetBitmap(&bm);
	memDC.CreateCompatibleDC(pDC);
	pOldBmp = memDC.SelectObject(&bmp);

	pDC->BitBlt(1, 1, bm.bmWidth, bm.bmHeight - 20, &memDC, 0, 0, SRCCOPY);
	


	CString csData;
	CFont	font;
	font.CreateFontA(30,20,0,0,FW_HEAVY,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"system");

	pDC->SelectObject(&font);

	
	csData.Format("TOTAL    = %d", gDispCnt.lTotal);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOutA(10,20 ,csData);//, DT_VCENTER);

	csData.Format("OK          = %d", gDispCnt.lOK);
	pDC->SetTextColor(RGB(0,255,0));
	pDC->TextOutA(10,70 ,csData);//, DT_VCENTER);

	csData.Format("NG          = %d", gDispCnt.lNG);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOutA(10,120 ,csData);//, DT_VCENTER);

	csData.Format("EMPTY   = %d", gDispCnt.lEmpty);
	pDC->SetTextColor(RGB(0,0,255));
	pDC->TextOutA(10,170 ,csData);//, DT_VCENTER);
	
	memDC.SelectObject(pOldBmp);
	font.DeleteObject();
}

void CFrm_Main::Rect_Result2(CDC* pDC)
{
	CString csFileName;
	CDC memDC;
	CBitmap bmp, *pOldBmp;
	BITMAP bm;
	CRect rect;
	
	
	csFileName = "D:\\LCT\\LCT\\BACK.bmp";
	
	bmp.m_hObject = (HBITMAP)LoadImage(NULL,csFileName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(bmp.m_hObject == NULL) return;

	bmp.GetBitmap(&bm);
	memDC.CreateCompatibleDC(pDC);
	pOldBmp = memDC.SelectObject(&bmp);

	pDC->BitBlt(1, 1, bm.bmWidth, bm.bmHeight - 20, &memDC, 0, 0, SRCCOPY);

	CString csData;
	CFont	font;
	font.CreateFontA(30,20,0,0,FW_HEAVY,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"system");

	pDC->SelectObject(&font);
	csData.Format("TOTAL    = %d", gSaveCnt.lTotal);
	
	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOutA(10,20 ,csData);//, DT_VCENTER);

	csData.Format("OK          = %d", gSaveCnt.lOK);
	pDC->SetTextColor(RGB(0,255,0));
	pDC->TextOutA(10,70 ,csData);//, DT_VCENTER);

	csData.Format("NG          = %d", gSaveCnt.lNG);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOutA(10,120 ,csData);//, DT_VCENTER);

	csData.Format("EMPTY   = %d", gSaveCnt.lEmpty);
	pDC->SetTextColor(RGB(0,0,255));
	pDC->TextOutA(10,170 ,csData);//, DT_VCENTER);

	memDC.SelectObject(pOldBmp);
	font.DeleteObject();

	m_nResultDisp = 0;
}

void CFrm_Main::OnBnClickedSetBtn()
{
	CDlg_Set	*dlg = new CDlg_Set;//동적으로 생성해준다
	dlg -> Create(IDD_DLG_SET,this);//모달리스는 반드시 Create로 띄워야한다. 

	dlg->m_bFirst = TRUE;
	CRect rcFrame;
/*	GetWindowRect(&rcFrame);*/
	dlg->GetWindowRect(&rcFrame);


/*	dlg->MoveWindow(x, y, rcFrame.Width(), rcFrame.Height());*/
	dlg ->ShowWindow(SW_SHOW);

	dlg->m_bFirst = FALSE;
}

void CFrm_Main::OnBnClickedGrid1(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nData;
	UpdateData(TRUE);
	nData = m_GridON;
	gnGridON = m_GridON;

	CString csFileName;
	CString csRet;
	CString csData;
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;
	
	
	csData.Format("%d", gnGridON);
	WritePrivateProfileString("SET", "GRID", csData, csFileName);
}

void CFrm_Main::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nX = 0;
	int nY = 0;
	nX = ((point.x - 910) / 18);
	nY = ((point.y - 110) / 27);
	
	if(point.x > 910 && point.y > 110 && point.x < 1235 && point.y < 519)
	{
		if(gnSkip[nX][nY] == 0)
		{
			gnSkip[nX][nY] = 1;
		}
		else
		{
			gnSkip[nX][nY] = 0;
		}

		SkipDataSave();
	}

	CFormView::OnLButtonDown(nFlags, point);
}

void CFrm_Main::SkipDataSave()
{
	CString csFileName;
	CString csRet;
	CString csData;
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;
	
	
	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			csData.Format("X%d_Y%d", nX,nY);
			csRet.Format("%d", gnSkip[nX][nY]);
			WritePrivateProfileString("SKIP", csData, csRet, csFileName);
		}
	}



	
}

void CFrm_Main::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (m_pCamera == NULL) {
		MessageBox("카메라 연결 실패");
		return;
	}

	if(m_nGrabCnt > 0)
	{
		return;
	}

	
	if(m_nLiveON == 1)
	{
		m_nLiveON = 0;
		m_pCamera->StopGrabbing();
		m_bThreadFlag = FALSE;
		return;
	}
	
	if(m_bThreadFlag == TRUE)
	{
		return;
	}
	m_nLiveON = 0;
	m_pCamera->StartGrabbing();
	
	m_bThreadFlag = TRUE;
	CWinThread *pThread = ::AfxBeginThread(ThreadImageCaptureFunc, this);
	
}

void CFrm_Main::ContGrab()
{
	if (m_pCamera == NULL) {
		MessageBox("카메라 연결 실패");
		return;
	}

	if(m_nGrabCnt > 0)
	{
		return;
	}

	
	
	if(m_bThreadFlag == TRUE)
	{
		return;
	}
	
	m_pCamera->StartGrabbing();

	m_bThreadFlag = TRUE;
	CWinThread *pThread = ::AfxBeginThread(ThreadImageCaptureFunc, this);
}


void CFrm_Main::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gDispCnt.lEmpty = 0;
	gDispCnt.lOK = 0;
	gDispCnt.lNG = 0;
	gDispCnt.lTotal = 0;

	CDC* pDCResult1;
	pDCResult1 = m_Disp_Result1.GetDC();
	Rect_Result1(pDCResult1);
	ReleaseDC(pDCResult1);
}


void CFrm_Main::OnBnClickedButton10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gSaveCnt.lEmpty = 0;
	gSaveCnt.lOK = 0;
	gSaveCnt.lNG = 0;
	gSaveCnt.lTotal = 0;

	CDC* pDCResult2;
	pDCResult2 = m_Disp_Result2.GetDC();
	Rect_Result2(pDCResult2);
	ReleaseDC(pDCResult2);
}

BOOL CFrm_Main::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	switch(pMsg->message)
	{
		//키가눌렸을때
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			OnBnClickedButton7();
			
			return TRUE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CFrm_Main::OnBnClickedModelName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csFileName;

	m_nLiveON = 0;
	m_pCamera->StopGrabbing();
	m_bThreadFlag = FALSE;

	CDlg_Model	Dlg;
	Dlg.DoModal();

	csFileName = "D:\\LCT\\LCT\\Model.txt";
	WritePrivateProfileString("MODEL", "NAME", gcsModelName, csFileName);
	SetDlgItemText(IDC_MODEL_NAME, gcsModelName);

	MotionDataLoad();
	Invalidate();
	m_GridON = gnGridON;
	m_ScoreON1 = gnScoreON;
	m_Result1 = gnScoreMode;
	
	UpdateData(FALSE);
	m_nResultDisp = 1;
}

void CFrm_Main::OnBnClickedResultOn1(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nData;
	UpdateData(TRUE);
	nData = m_Result1;
	gnScoreMode = nData;

	CString csFileName;
	CString csRet;
	CString csData;
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;
	
	csData.Format("%d", gnScoreMode);
	WritePrivateProfileString("SET", "RESULT MODE", csData, csFileName);
}

void CFrm_Main::OnBnClickedScoreOn1(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nData;
	UpdateData(TRUE);
	nData = m_ScoreON1;
	gnScoreON = nData;

	CString csFileName;
	CString csRet;
	CString csData;
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;
	
	csData.Format("%d", gnScoreON);
	WritePrivateProfileString("SET", "SCOREON", csData, csFileName);
}

void CFrm_Main::Test()
{
	CDC* pDC;
	pDC = m_Display.GetDC();
	Rect_Test(pDC);
	ReleaseDC(pDC);

}

void CFrm_Main::Rect_Test(CDC* pDC)
{
	CPen myPen;
	CPen* pOldPen;
	RECT rt;
	int var = 0;
	int nX1 = 0;
	int nX2 = 0;
	int nY1 = 0;
	int nY2 = 0;
	int nXConvert = 0;
	int nYConvert = 0;
	int nCnt = 0;
	int nBlueCnt = 0;
	int nBlackCnt = 0;
	int nXDir = 0;
	int nYDir = 0;
	int nData = 0;
	CString csData;
	int nArround_XMin = 0;
	int nArround_XMax = 0;
	int nArround_YMin = 0;
	int nArround_YMax = 0;
	bool bFirst = false;
	int nLoop = 0;
	int nArray[200][200];

	for (int nY = 0; nY < 200; nY++)//카메라 영역기준-> 화면영역으로 변환   
	{
		for (int nX = 0; nX < 200; nX++) 
		{
			nArray[nX][nY] = 1;
		}
	}
		
	for (int nY = 0; nY < 200; nY++)//카메라 영역기준-> 화면영역으로 변환   
	{
		for (int nX = 0; nX < 200; nX++) 
		{
			if(nArray[nX][nY] == 1)
			{
				pDC->SetPixel(nX, nY,RGB(255,0,0));
			}
			else
			{
				pDC->SetPixel(nX, nY,RGB(255,255,255));
			}
		}
	}	

	while(nLoop != 1)
	{
		for (int nY = 0; nY < 200; nY++)//카메라 영역기준-> 화면영역으로 변환   
		{
			for (int nX = 0; nX < 200; nX++) 
			{
				if(nArray[nX][nY] == 0)
				{
					if(bFirst == false)
					{
						bFirst = true;
						nArray[nX][nY] = 1;
						nArround_XMin = nX - 1;
						nArround_XMax = nX + 1;
						nArround_YMin = nY - 1;
						nArround_YMax = nY + 1;

						pDC->SetPixel(nX, nY,RGB(0,0,255));
					}
					else
					{
						if(nArround_XMin - 2 <= nX && nArround_XMax + 2 >= nX && nArround_YMin - 2 <= nY && nArround_YMax + 2 >= nY)
						{
							nArray[nX][nY] = 1;
							nCnt++;

							if(nArround_XMin > nX -1)
							{
								nArround_XMin = nX -1;
							}

							if(nArround_XMax < nX + 1)
							{
								nArround_XMax = nX + 1;
							}

							if(nArround_YMin > nY -1)
							{
								nArround_YMin = nY -1;
							}

							if(nArround_YMax < nY + 1)
							{
								nArround_YMax = nY + 1;
							}
						}
					}
				}
			}
		}

		if(nCnt < 1)
		{
			nLoop = 1;
		}
		else
		{
			nCnt = 0;
			bFirst = false;
		}
	}

	nLoop = 0;
	myPen.CreatePen(PS_SOLID,1, RGB(255,0,0));
	pOldPen = pDC->SelectObject(&myPen);
}

void CFrm_Main::FileSave(CString csCnt)
{
	FILE* pFile;
	CString csYear;
	CString csMonth;
	CString csDay;
	CString csFileName;
	GetCurDay(&csYear, &csMonth, &csDay);
	csFileName.Format("D:\\LOG\\%s_%s%s%s.txt", gcsModelName, csYear, csMonth, csDay);

	CTime CurTime;
	CString csEventTime, csCreateName;
	CString csH,csM,csS;
	CString csData;
	int nH,nM,nS;

	CurTime = CTime::GetCurrentTime();
	nH = CurTime.GetHour();
	nM = CurTime.GetMinute();
	nS = CurTime.GetSecond();
	csH.Format("[%02d:",nH);
	csM.Format("%02d:",nM);
	csS.Format("%02d]",nS);
	csEventTime = csH + csM + csS;
	csData.Format("::[[%s]] :,%s\n",gcsModelName,csCnt);
	csEventTime = csEventTime + csData;
	
	pFile = _fsopen(csFileName, "a+", _SH_DENYNO);
	if(pFile != NULL)
	{
		fprintf(pFile, csEventTime);
	}
	fclose(pFile);
	
}

void CFrm_Main::LotSave()
{
	
	FILE* pFile;
	CString csYear;
	CString csMonth;
	CString csDay;
	CString csFileName;
	GetCurDay(&csYear, &csMonth, &csDay);
	csFileName.Format("D:\\LOG\\%s_%s%s%s.txt", gcsModelName, csYear, csMonth, csDay);

	CTime CurTime;
	CString csEventTime, csLog;
	CString csH,csM,csS;
	CString csData;
	int nH,nM,nS;

	CurTime = CTime::GetCurrentTime();
	nH = CurTime.GetHour();
	nM = CurTime.GetMinute();
	nS = CurTime.GetSecond();
	csH.Format("[%02d:",nH);
	csM.Format("%02d:",nM);
	csS.Format("%02d]::==============================================\n",nS);
	csEventTime = csH + csM + csS;
	csData.Format("[%02d:%02d:%02d]::[[LOT SAVE]] : ,%d, %d, %d, %d\n",nH,nM,nS, gSaveCnt.lTotal,gSaveCnt.lOK,gSaveCnt.lNG,gSaveCnt.lEmpty);
	csLog = csEventTime + csData + csEventTime;
	
	pFile = _fsopen(csFileName, "a+", _SH_DENYNO);
	if(pFile != NULL)
	{
		fprintf(pFile, csLog);
	}
	fclose(pFile);
	
}

void CFrm_Main::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gSaveCnt.lEmpty = gSaveCnt.lEmpty - gDispCnt.lEmpty;
	gSaveCnt.lOK = gSaveCnt.lOK - gDispCnt.lOK;
	gSaveCnt.lNG = gSaveCnt.lNG - gDispCnt.lNG;
	gSaveCnt.lTotal = gSaveCnt.lTotal - gDispCnt.lTotal;

	gDispCnt.lEmpty = 0;
	gDispCnt.lOK = 0;
	gDispCnt.lNG = 0;
	gDispCnt.lTotal = 0;

	CDC* pDCResult1;
	pDCResult1 = m_Disp_Result1.GetDC();
	Rect_Result1(pDCResult1);
	ReleaseDC(pDCResult1);

	CDC* pDCResult2;
	pDCResult2 = m_Disp_Result2.GetDC();
	Rect_Result2(pDCResult2);
	ReleaseDC(pDCResult2);
}


void CFrm_Main::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LotSave();
}
