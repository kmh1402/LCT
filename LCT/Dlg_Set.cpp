// Dlg_Set.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LCT.h"
#include "Dlg_Set.h"
#include "afxdialogex.h"
#include "VarEx.h"
#include "FuncEx.h"
// CDlg_Set 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Set, CDialog)

CDlg_Set::CDlg_Set(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Set::IDD, pParent)
	, m_X_Cnt(_T(""))
	, m_Y_Cnt(_T(""))
	, m_Score(_T(""))
	, m_XSize(_T(""))
	, m_YSize(_T(""))
	, m_XGap(_T(""))
	, m_YGap(_T(""))
	, m_XPos(_T(""))
	, m_YPos(_T(""))
	, m_EdtPitch(_T(""))
	, m_Black_Value(_T(""))
	
	
	
	
	, m_Blue_R(_T(""))
	, m_Blue_G(_T(""))
	, m_Blue_B(_T(""))
	
	, m_Black_R(_T(""))
	, m_Black_G(_T(""))
	, m_Black_B(_T(""))
	
	, m_LensINScore(_T(""))
	, m_In_Box_Size(_T(""))
	, m_OK_Range_Min(_T(""))
	, m_OK_Range_Max(_T(""))
	, m_Big_R(_T(""))
	, m_Big_G(_T(""))
	, m_NGThr(_T(""))
	, m_Blue_Score(_T(""))
	, m_BlueRChk(_T(""))
	, m_BlueGChk(_T(""))
	, m_BlueBChk(_T(""))
	, m_In_Box_SizeY(_T(""))
{

}

CDlg_Set::~CDlg_Set()
{
}

void CDlg_Set::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_XCNT, m_X_Cnt);
	DDX_Text(pDX, IDC_EDT_YCNT, m_Y_Cnt);
	DDX_Text(pDX, IDC_EDT_SCORE, m_Score);
	DDX_Text(pDX, IDC_EDT_XSIZE, m_XSize);
	DDX_Text(pDX, IDC_EDT_YSIZE, m_YSize);
	DDX_Text(pDX, IDC_EDT_XGAP, m_XGap);
	DDX_Text(pDX, IDC_EDT_YGAP, m_YGap);
	DDX_Text(pDX, IDC_EDT_XPOS, m_XPos);
	DDX_Text(pDX, IDC_EDT_YPOS, m_YPos);
	DDX_Text(pDX, IDC_EDT_PITCH, m_EdtPitch);
	DDX_Text(pDX, IDC_BLACK_VALUE, m_Black_Value);

	DDX_Text(pDX, IDC_EDT_BLUE_R, m_Blue_R);
	DDX_Text(pDX, IDC_EDT_BLUE_G, m_Blue_G);
	DDX_Text(pDX, IDC_EDT_BLUE_B, m_Blue_B);

	DDX_Text(pDX, IDC_EDT_BLACK_R, m_Black_R);
	DDX_Text(pDX, IDC_EDT_BLACK_G, m_Black_G);
	DDX_Text(pDX, IDC_EDT_BLACK_B, m_Black_B);

	DDX_Text(pDX, IDC_LENS_IN_SCORE, m_LensINScore);
	DDX_Text(pDX, IDC_IN_BOX_SIZE, m_In_Box_Size);
	DDX_Text(pDX, IDC_EDT_OK_RANGE_MIN, m_OK_Range_Min);
	DDX_Text(pDX, IDC_EDT_OK_RANGE_MAX, m_OK_Range_Max);
	DDX_Text(pDX, IDC_EDT_BIG_R, m_Big_R);
	DDX_Text(pDX, IDC_IDC_EDT_BIG_G, m_Big_G);
	DDX_Radio(pDX, IDC_SET_MODE1, m_SetMode1);
	DDX_Text(pDX, IDC_EDT_NG_THR, m_NGThr);
	DDX_Text(pDX, IDC_EDT_BLUE_SCORE, m_Blue_Score);
	DDX_Text(pDX, IDC_EDT_BLUE_RCHK, m_BlueRChk);
	DDX_Text(pDX, IDC_EDT_BLUE_GCHK, m_BlueGChk);
	DDX_Text(pDX, IDC_EDT_BLUE_BCHK, m_BlueBChk);
	DDX_Text(pDX, IDC_IN_BOX_SIZEY, m_In_Box_SizeY);
}


BEGIN_MESSAGE_MAP(CDlg_Set, CDialog)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlg_Set::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_JOG_LEFT, &CDlg_Set::OnBnClickedJogLeft)
	ON_BN_CLICKED(IDC_JOG_RIGHT, &CDlg_Set::OnBnClickedJogRight)
	ON_BN_CLICKED(IDC_JOG_UP, &CDlg_Set::OnBnClickedJogUp)
	ON_BN_CLICKED(IDC_JOG_DOWN, &CDlg_Set::OnBnClickedJogDown)
	ON_BN_CLICKED(IDC_JOG_DOWN2, &CDlg_Set::OnBnClickedJogDown2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SET_MODE1 , IDC_SET_MODE2 , OnBnClickedSetMode1)
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlg_Set 메시지 처리기입니다.

void CDlg_Set::OnBnClickedSetMode1(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nData;
	UpdateData(TRUE);
	nData = m_SetMode1;
	gnSetMode = nData;

	CString csFileName;
	CString csRet;
	CString csData;
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;
	
	csData.Format("%d", gnSetMode);
	WritePrivateProfileString("SET", "SET MODE", csData, csFileName);
}

void CDlg_Set::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
}

BOOL CDlg_Set::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString csData;
	csData = "1";
	DataLoadSetCaption();
	m_EdtPitch = csData;
	m_SetMode1 = gnSetMode;
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlg_Set::DataSave()
{
	UpdateData(TRUE);

	CString csFileName;
	CString csRet;
	CString csData;
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;
	
	csData = m_X_Cnt;
	gnXCnt = atol(csData);
	WritePrivateProfileString("SET", "X CNT", csData, csFileName);

	csData = m_Y_Cnt;
	gnYCnt = atol(csData);
	WritePrivateProfileString("SET", "Y CNT", csData, csFileName);

	csData = m_Score;
	gnScore2 = atol(csData);
	WritePrivateProfileString("SET", "SCORE", csData, csFileName);

	csData = m_LensINScore;
	gnLensINScore = atol(csData);
	WritePrivateProfileString("SET", "LENS IN SCORE", csData, csFileName);

	csData = m_Black_Value;
	gnBlackValue = atol(csData);
	WritePrivateProfileString("SET", "BLACK VALUE", csData, csFileName);
	
	csData = m_XSize;
	gGrid.nSize_X = atol(csData);
	WritePrivateProfileString("GRIDSET", "X SIZE", csData, csFileName);

	csData = m_YSize;
	gGrid.nSize_Y = atol(csData);
	WritePrivateProfileString("GRIDSET", "Y SIZE", csData, csFileName);
	
	csData = m_XGap;
	gGrid.nGap_X = atof(csData);
	WritePrivateProfileString("GRIDSET", "X GAP", csData, csFileName);

	csData = m_YGap;
	gGrid.nGap_Y = atof(csData);
	WritePrivateProfileString("GRIDSET", "Y GAP", csData, csFileName);

	csData = m_XPos;
	gGrid.nPos_X = atol(csData);
	WritePrivateProfileString("GRIDSET", "X POS", csData, csFileName);

	csData = m_YPos;
	gGrid.nPos_Y = atol(csData);
	WritePrivateProfileString("GRIDSET", "Y POS", csData, csFileName);

	csData = m_Blue_R;
	gBlueC.nR = atol(csData);
	WritePrivateProfileString("SET", "BLUE R", csData, csFileName);

	csData = m_Blue_G;
	gBlueC.nG = atol(csData);
	WritePrivateProfileString("SET", "BLUE G", csData, csFileName);

	csData = m_Blue_B;
	gBlueC.nB = atol(csData);
	WritePrivateProfileString("SET", "BLUE B", csData, csFileName);

	
	csData = m_Black_R;
	gBlackC.nR = atol(csData);
	WritePrivateProfileString("SET", "BLACK R", csData, csFileName);

	csData = m_Black_G;
	gBlackC.nG = atol(csData);
	WritePrivateProfileString("SET", "BLACK G", csData, csFileName);

	csData = m_Black_B;
	gBlackC.nB = atol(csData);
	WritePrivateProfileString("SET", "BLACK B", csData, csFileName);

	csData = m_In_Box_Size;
	gnInBoxSize = atol(csData);
	WritePrivateProfileString("SET", "INBOX SIZE", csData, csFileName);

	csData = m_In_Box_SizeY;
	gnInBoxSizeY = atol(csData);
	WritePrivateProfileString("SET", "INBOX SIZE Y", csData, csFileName);

	
	csData = m_OK_Range_Min;
	gnOKRangeMin = atol(csData);
	WritePrivateProfileString("SET", "OK RANGE MIN", csData, csFileName);

	csData = m_OK_Range_Max;
	gnOKRangeMax = atol(csData);
	WritePrivateProfileString("SET", "OK RANGE MAX", csData, csFileName);

	csData = m_Big_R;
	gBigC.nR = atol(csData);
	WritePrivateProfileString("SET", "BIG R", csData, csFileName);

	csData = m_Big_G;
	gBigC.nG = atol(csData);
	WritePrivateProfileString("SET", "BIG G", csData, csFileName);

	csData.Format("%d", gnSetMode);
	WritePrivateProfileString("SET", "SET MODE", csData, csFileName);

	csData = m_NGThr;
	gnNGval = atol(csData);
	WritePrivateProfileString("SET", "NG THR", csData, csFileName);


	csData = m_Blue_Score;
	gnBlueCntSet = atol(csData);
	WritePrivateProfileString("SET", "BLUE CNT", csData, csFileName);


	csData = m_BlueRChk;
	gnBlueRChk = atol(csData);
	WritePrivateProfileString("SET", "BLUE R CHK", csData, csFileName);

	csData = m_BlueGChk;
	gnBlueGChk = atol(csData);
	WritePrivateProfileString("SET", "BLUE G CHK", csData, csFileName);

	csData = m_BlueBChk;
	gnBlueBChk = atol(csData);
	WritePrivateProfileString("SET", "BLUE B CHK", csData, csFileName);

}

void CDlg_Set::DataLoadSetCaption()
{
	CString csData;

	csData.Format("%d", gnXCnt);
	m_X_Cnt = csData;

	csData.Format("%d", gnYCnt);
	m_Y_Cnt = csData;

	csData.Format("%d", gnScore2);
	m_Score = csData;

	csData.Format("%d", gnBlackValue);
	m_Black_Value = csData;
	
	csData.Format("%d", gGrid.nSize_X);
	m_XSize = csData;

	csData.Format("%d", gGrid.nSize_Y);
	m_YSize = csData;

	csData.Format("%.2f", gGrid.nGap_X);
	m_XGap = csData;

	csData.Format("%.2f", gGrid.nGap_Y);
	m_YGap = csData;

	csData.Format("%d", gGrid.nPos_X);
	m_XPos = csData;

	csData.Format("%d", gGrid.nPos_Y);
	m_YPos = csData;

	
	csData.Format("%d", gBlueC.nR);
	m_Blue_R = csData;

	csData.Format("%d", gBlueC.nG);
	m_Blue_G = csData;

	csData.Format("%d", gBlueC.nB);
	m_Blue_B = csData;

	

	csData.Format("%d", gBlackC.nR);
	m_Black_R = csData;

	csData.Format("%d", gBlackC.nG);
	m_Black_G = csData;

	csData.Format("%d", gBlackC.nB);
	m_Black_B = csData;

	csData.Format("%d", gnLensINScore);
	m_LensINScore = csData;	

	csData.Format("%d", gnInBoxSize);
	m_In_Box_Size = csData;

	csData.Format("%d", gnInBoxSizeY);
	m_In_Box_SizeY = csData;

	csData.Format("%d", gnOKRangeMin);
	m_OK_Range_Min = csData;	

	csData.Format("%d", gnOKRangeMax);
	m_OK_Range_Max = csData;	

	csData.Format("%d", gBigC.nR);
	m_Big_R = csData;	

	csData.Format("%d", gBigC.nG);
	m_Big_G = csData;	

	csData.Format("%d",gnNGval);
	m_NGThr = csData;	

	csData.Format("%d",gnBlueCntSet);
	m_Blue_Score = csData;	


	csData.Format("%d",gnBlueRChk);
	m_BlueRChk = csData;	

	csData.Format("%d",gnBlueGChk);
	m_BlueGChk = csData;	

	csData.Format("%d",gnBlueBChk);
	m_BlueBChk = csData;	
	UpdateData(FALSE);
}

void CDlg_Set::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialog::PostNcDestroy();
}


BOOL CDlg_Set::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::DestroyWindow();
}


void CDlg_Set::OnBnClickedJogLeft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int nPitch = 0;
	CString csData;

	csData = m_EdtPitch;

	if(csData == "")
	{
		nPitch = 1;
	}

	nPitch = atol(csData);

	if(gGrid.nPos_X < 1)
	{
		gGrid.nPos_X = 0;
	}
	else
	{
		gGrid.nPos_X = gGrid.nPos_X - nPitch;			
	}

	csData.Format("%d", gGrid.nPos_X);
	m_XPos = csData;
	UpdateData(FALSE);
	DataSave();
}


void CDlg_Set::OnBnClickedJogRight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPitch = 0;
	UpdateData(TRUE);
	CString csData;

	csData = m_EdtPitch;

	if(csData == "")
	{
		nPitch = 1;
	}

	nPitch = atol(csData);

	if(gGrid.nPos_X > 500)
	{
		gGrid.nPos_X = 500;
	}
	else
	{
		gGrid.nPos_X = gGrid.nPos_X + nPitch;			
	}

	csData.Format("%d", gGrid.nPos_X);
	m_XPos = csData;
	UpdateData(FALSE);
	DataSave();
}


void CDlg_Set::OnBnClickedJogUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPitch = 0;
	UpdateData(TRUE);
	CString csData;

	csData = m_EdtPitch;

	if(csData == "")
	{
		nPitch = 1;
	}

	nPitch = atol(csData);

	if(gGrid.nPos_Y < 1)
	{
		gGrid.nPos_Y = 0;
	}
	else
	{
		gGrid.nPos_Y = gGrid.nPos_Y - nPitch;			
	}

	csData.Format("%d", gGrid.nPos_Y);
	m_YPos = csData;
	UpdateData(FALSE);
	DataSave();
}


void CDlg_Set::OnBnClickedJogDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPitch = 0;
	UpdateData(TRUE);
	CString csData;

	csData = m_EdtPitch;

	if(csData == "")
	{
		nPitch = 1;
	}

	nPitch = atol(csData);

	if(gGrid.nPos_Y > 500)
	{
		gGrid.nPos_Y = 500;
	}
	else
	{
		gGrid.nPos_Y = gGrid.nPos_Y + nPitch;			
	}

	csData.Format("%d", gGrid.nPos_Y);
	m_YPos = csData;
	UpdateData(FALSE);
	DataSave();
}


void CDlg_Set::OnBnClickedJogDown2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DataSave();
	AfxMessageBox("Data Save Complete");
}


void CDlg_Set::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	/*static BOOL bFirst = TRUE;*/

	if (!m_bFirst)
	{
		CRect rcFrame;
		GetWindowRect(&rcFrame);

		AfxGetApp()->WriteProfileInt("CDlg_Set", "mov_X", rcFrame.left);
		AfxGetApp()->WriteProfileInt("CDlg_Set", "mov_y", rcFrame.top);
	}

/*	bFirst = FALSE;*/
}


void CDlg_Set::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		int x = AfxGetApp()->GetProfileInt("CDlg_Set", "mov_X", 1280 / 2);
		int y = AfxGetApp()->GetProfileInt("CDlg_Set", "mov_y", 1024 / 2);

		CRect rcFrame;
		GetWindowRect(&rcFrame);
		MoveWindow(x, y, rcFrame.Width(), rcFrame.Height());
	}
}
