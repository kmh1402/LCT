#pragma once
#include "SimpleBmp.h"
#include "afxwin.h"
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h>
#endif


using namespace Pylon;
using namespace GenApi;
using namespace std;


// CFrm_Main 폼 뷰입니다.

class CFrm_Main : public CFormView
{
	DECLARE_DYNCREATE(CFrm_Main)

protected:
	CFrm_Main();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFrm_Main();

public:
	enum { IDD = IDD_FRM_MAIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
	CStatic m_Display;

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	void Rect1(CDC* pDC);
	void ImageCalc();
	void LoadBitMap(CDC* pDC);
	
	afx_msg void OnBnClickedSetBtn();
	int m_GridON;
	BYTE m_Array[960][1280];
	BYTE m_Array_Blue[960][1280];
	//BYTE m_Array_Black[1280][1024];
	afx_msg void OnBnClickedGrid1(UINT msg);
	int m_nGrab;	
	int m_ScoreON1;
	int m_Score_Mode1;
	int m_Result1;
	void FileSave(CString csCnt);
	afx_msg void OnBnClickedScoreOn1(UINT msg);
	afx_msg void OnBnClickedResultOn1(UINT msg);
	void RectSkip(CDC* pDC);
	CStatic m_Disp_Result1;
	CStatic m_Disp_Result2;
	CStatic m_Skip_Disp;
	void SkipDataSave();
	void ContGrab();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void Rect_Result1(CDC* pDC);
	void Rect_Result2(CDC* pDC);
	CStatic m_DispLogo;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_Model_Name;
	afx_msg void OnBnClickedModelName();
	
	void OnCamInit();
	void ImageCalc2();
	void RectRoi(CDC* pDC);
	void Test();
	void Rect_Test(CDC* pDC);
	void LotSave();
	int m_nResultDisp;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};


