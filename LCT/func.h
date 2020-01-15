#define GOOD			1
#define FAIL			-1

#include "share.h"
CString GetRunDirectory()
{
	char buf[128];
	CString csreturnDir;
	GetCurrentDirectory(128,buf);
	csreturnDir.Format("%s",buf);
	return (csreturnDir);
}

int FileDelete(CString csFileName)
{
	if(DeleteFile(csFileName) != 0)
		return(GOOD);
	else
		return(FAIL);
}

int FileCopy(CString objectFile, CString sourceFile, bool overWrite)
{
	if(CopyFile(sourceFile, objectFile, !overWrite != 0))
		return(GOOD);
	else
		return(FAIL);
}

int DirectoryCreate(CString dirPath)
{
	int returnVal;
	returnVal = CreateDirectory(dirPath, NULL);
	return(returnVal);
}

void MotionDataLoad()
{
	CString csFileName;	


	char MData[20];//불러올변수저장공간및 확보
	char MRData[50];//불러올변수저장공간및 확보
	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Model.txt" ;

	GetPrivateProfileString("MODEL", "NAME", "Default", MRData, 50, csFileName);
	gcsModelName = MRData;

	
	csFileName = "D:\\LCT\\LCT";
	csFileName = csFileName + "\\Setting\\";
	csFileName = csFileName + gcsModelName;
	csFileName = csFileName + ".txt" ;

	GetPrivateProfileString("SET", "X CNT", "Default", MData, 20, csFileName);
	gnXCnt = atol(MData) ;

	GetPrivateProfileString("SET", "Y CNT", "Default", MData, 20, csFileName);
	gnYCnt = atol(MData) ;

	GetPrivateProfileString("SET", "SCORE", "Default", MData, 20, csFileName);
	gnScore2 = atol(MData) ;

	GetPrivateProfileString("SET", "BLACK VALUE", "Default", MData, 20, csFileName);
	gnBlackValue = atol(MData) ;

	GetPrivateProfileString("GRIDSET", "X SIZE", "Default", MData, 20, csFileName);
	gGrid.nSize_X = atol(MData) ;

	GetPrivateProfileString("GRIDSET", "Y SIZE", "Default", MData, 20, csFileName);
	gGrid.nSize_Y = atol(MData) ;

	GetPrivateProfileString("GRIDSET", "X GAP", "Default", MData, 20, csFileName);
	gGrid.nGap_X = atof(MData) ;

	GetPrivateProfileString("GRIDSET", "Y GAP", "Default", MData, 20, csFileName);
	gGrid.nGap_Y = atof(MData) ;

	GetPrivateProfileString("GRIDSET", "X POS", "Default", MData, 20, csFileName);
	gGrid.nPos_X = atol(MData) ;

	GetPrivateProfileString("GRIDSET", "Y POS", "Default", MData, 20, csFileName);
	gGrid.nPos_Y = atol(MData) ;

	GetPrivateProfileString("SET", "GRID", "Default", MData, 20, csFileName);
	gnGridON = atol(MData) ;

	GetPrivateProfileString("SET", "SCOREON", "Default", MData, 20, csFileName);
	gnScoreON = atol(MData) ;

	GetPrivateProfileString("SET", "RESULT MODE", "Default", MData, 20, csFileName);
	gnScoreMode = atol(MData) ;

	GetPrivateProfileString("SET", "BLUE R", "Default", MData, 20, csFileName);
	gBlueC.nR = atol(MData) ;

	GetPrivateProfileString("SET", "BLUE G", "Default", MData, 20, csFileName);
	gBlueC.nG = atol(MData) ;

	GetPrivateProfileString("SET", "BLUE B", "Default", MData, 20, csFileName);
	gBlueC.nB = atol(MData) ;

	

	GetPrivateProfileString("SET", "BLACK R", "Default", MData, 20, csFileName);
	gBlackC.nR = atol(MData) ;

	GetPrivateProfileString("SET", "BLACK G", "Default", MData, 20, csFileName);
	gBlackC.nG = atol(MData) ;

	GetPrivateProfileString("SET", "BLACK B", "Default", MData, 20, csFileName);
	gBlackC.nB = atol(MData) ;

	GetPrivateProfileString("SET", "LENS IN SCORE", "Default", MData, 20, csFileName);
	gnLensINScore = atol(MData) ;

	GetPrivateProfileString("SET", "INBOX SIZE", "Default", MData, 20, csFileName);
	gnInBoxSize = atol(MData) ;

	GetPrivateProfileString("SET", "INBOX SIZE Y", "Default", MData, 20, csFileName);
	gnInBoxSizeY = atol(MData) ;

	GetPrivateProfileString("SET", "OK RANGE MIN", "Default", MData, 20, csFileName);
	gnOKRangeMin = atol(MData) ;

	GetPrivateProfileString("SET", "OK RANGE MAX", "Default", MData, 20, csFileName);
	gnOKRangeMax = atol(MData) ;

	GetPrivateProfileString("SET", "BIG R", "Default", MData, 20, csFileName);
	gBigC.nR = atol(MData) ;

	GetPrivateProfileString("SET", "BIG G", "Default", MData, 20, csFileName);
	gBigC.nG = atol(MData) ;

	GetPrivateProfileString("SET", "SET MODE", "Default", MData, 20, csFileName);
	gnSetMode = atol(MData) ;


	GetPrivateProfileString("SET", "NG THR", "Default", MData, 20, csFileName);
	gnNGval = atol(MData) ;

	GetPrivateProfileString("SET", "BLUE CNT", "Default", MData, 20, csFileName);
	gnBlueCntSet = atol(MData) ;



	GetPrivateProfileString("SET", "BLUE R CHK", "Default", MData, 20, csFileName);
	gnBlueRChk = atol(MData) ;

	GetPrivateProfileString("SET", "BLUE G CHK", "Default", MData, 20, csFileName);
	gnBlueGChk = atol(MData) ;

	GetPrivateProfileString("SET", "BLUE B CHK", "Default", MData, 20, csFileName);
	gnBlueBChk = atol(MData) ;
	
	
	CString csData;
	for(int nY = 0; nY < gnYCnt; nY++)
	{
		for(int nX = 0; nX < gnXCnt; nX++)
		{
			csData.Format("X%d_Y%d", nX,nY);
			GetPrivateProfileString("SKIP", csData, "Default", MData, 20, csFileName);
			gnSkip[nX][nY] = atol(MData) ;
		}
	}
}

void GetCurDay(CString *csYear, CString *csMonth, CString *csDay)
{
	CTime CurTime;
	CString csData;
	int iYear,iMonth,iDay;

	CurTime = CTime::GetCurrentTime();
	iMonth = CurTime.GetMonth();
	csData.Format("%dM",iMonth);
	*csMonth = csData;

	iDay = CurTime.GetDay();
	csData.Format("%dD",iDay);
	*csDay = csData;

	iYear = CurTime.GetYear();
	csData.Format("%dY",iYear);
	*csYear = csData;
}

void GetCurTime(CString *csHour, CString *csMin, CString *csSec)
{
	CTime CurTime;
	CString csData;
	int iHour,iMinute,iSecond;

	CurTime = CTime::GetCurrentTime();
	iHour = CurTime.GetHour();
	csData.Format("%d시",iHour);
	*csHour = csData;

	iMinute = CurTime.GetMinute();
	csData.Format("%d분",iMinute);
	*csMin = csData;

	iSecond = CurTime.GetSecond();
	csData.Format("%d초",iSecond);
	*csSec = csData;
}