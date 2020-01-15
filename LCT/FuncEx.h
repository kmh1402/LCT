extern CString GetRunDirectory();
extern int FileDelete(CString csFileName);
extern int FileCopy(CString objectFile, CString sourceFile, bool overWrite);
extern int DirectoryCreate(CString dirPath);
extern void MotionDataLoad();
extern void GetCurDay(CString *csYear, CString *csMonth, CString *csDay);
extern void GetCurTime(CString *csHour, CString *csMin, CString *csSec);

