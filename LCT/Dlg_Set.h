#pragma once


// CDlg_Set 대화 상자입니다.

class CDlg_Set : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Set)

public:
	CDlg_Set(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Set();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton8();
	virtual BOOL OnInitDialog();
	void DataSave();
	void DataLoadSetCaption();
	CString m_X_Cnt;
	CString m_Y_Cnt;
	CString m_Score;
	CString m_XSize;
	CString m_YSize;
	CString m_XGap;
	CString m_YGap;
	CString m_XPos;
	CString m_YPos;
	virtual void PostNcDestroy();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedJogLeft();
	afx_msg void OnBnClickedJogRight();
	afx_msg void OnBnClickedJogUp();
	afx_msg void OnBnClickedJogDown();
	afx_msg void OnBnClickedJogDown2();
	CString m_Score_Empty;
	CString m_EdtPitch;
	BOOL m_bFirst;	
	CString m_Black_Value;
	CString m_Line_Length;
	CString m_Line_Count;
	CString m_InBox_Size_X;
	CString m_Blue_R;
	CString m_Blue_G;
	CString m_Blue_B;
	CString m_Blue_Count;
	CString m_Black_R;
	CString m_Black_G;
	CString m_Black_B;
	CString m_Black_Count;
	CString m_NG_Size;
	CString m_LensINScore;
	CString m_In_Box_Size;
	CString m_OK_Range_Min;
	CString m_OK_Range_Max;
	CString m_Big_R;
	CString m_Big_G;
	int m_SetMode1;
	void OnBnClickedSetMode1(UINT msg);
	CString m_NGThr;
	CString m_Blue_Score;
	CString m_BlueRChk;
	CString m_BlueGChk;
	CString m_BlueBChk;
	CString m_In_Box_SizeY;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
