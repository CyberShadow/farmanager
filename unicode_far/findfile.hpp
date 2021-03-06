#pragma once

/*
findfile.hpp

����� (Alt-F7)
*/
/*
Copyright � 1996 Eugene Roshal
Copyright � 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

enum FINDAREA
{
	FINDAREA_ALL,
	FINDAREA_ALL_BUTNETWORK,
	FINDAREA_INPATH,
	FINDAREA_ROOT,
	FINDAREA_FROM_CURRENT,
	FINDAREA_CURRENT_ONLY,
	FINDAREA_SELECTED,
};

class FindFiles
{
public:
	FindFiles();
	~FindFiles();

private:
	void InitInFileSearch();
	void ReleaseInFileSearch();
	string &PrepareDriveNameStr(string &strSearchFromRoot);
	bool IsWordDiv(const wchar_t symbol);
	void SetPluginDirectory(const wchar_t *DirName,HANDLE hPlugin,bool UpdatePanel=false);
	intptr_t AdvancedDlgProc(HANDLE hDlg, intptr_t Msg, intptr_t Param1, void* Param2);
	void AdvancedDialog();
	intptr_t MainDlgProc(HANDLE hDlg, intptr_t Msg, intptr_t Param1, void* Param2);
	bool GetPluginFile(size_t ArcIndex, const FAR_FIND_DATA_EX& FindData, const wchar_t *DestPath, string &strResultName);
	const int FindStringBMH(const wchar_t* searchBuffer, size_t searchBufferCount);
	const int FindStringBMH(const unsigned char* searchBuffer, size_t searchBufferCount);
	int LookForString(const string& Name);
	bool IsFileIncluded(PluginPanelItem* FileItem, const wchar_t *FullName, DWORD FileAttr);
	intptr_t FindDlgProc(HANDLE hDlg, intptr_t Msg, intptr_t Param1, void* Param2);
	void AddMenuRecord(HANDLE hDlg,const wchar_t *FullName, const FAR_FIND_DATA_EX& FindData, void* Data, FARPANELITEMFREECALLBACK FreeData);
	void AddMenuRecord(HANDLE hDlg,const wchar_t *FullName, PluginPanelItem& FindData);
	void DoPreparePluginList(HANDLE hDlg, bool Internal);
	void ArchiveSearch(HANDLE hDlg, const wchar_t *ArcName);
	void DoScanTree(HANDLE hDlg, string& strRoot);
	void ScanPluginTree(HANDLE hDlg, HANDLE hPlugin, UINT64 Flags, int& RecurseLevel);
	void DoPrepareFileList(HANDLE hDlg);
	unsigned int ThreadRoutine(LPVOID Param);
	bool FindFilesProcess();

private:
	// BUGBUG
	bool AnySetFindList;
	string strFindMask;
	string strFindStr;
	bool CmpCase;
	bool WholeWords;
	bool SearchInArchives;
	bool SearchHex;
	int SearchMode;
	string strLastDirName;
	string strPluginSearchPath;
	bool UseFilter;
	uintptr_t CodePage;
	UINT64 SearchInFirst;
	char *readBufferA;
	wchar_t *readBuffer;
	int codePagesCount;
	unsigned char *hexFindString;
	size_t hexFindStringSize;
	wchar_t *findString;
	wchar_t *findStringBuffer;
	size_t *skipCharsTable;
	int favoriteCodePages;
	bool InFileSearchInited;
	class CFileMask* FileMaskForFindFile;
	class FileFilter *Filter;

	// ������������ ��� �������� ������ �� ��������� ������.
	// ������ �������� �������� � ������ � ���� ��� ��������.
	DWORD FindExitIndex;
	bool FindFoldersChanged;
	bool SearchFromChanged;
	bool FindPositionChanged;
	bool Finalized;
	bool PluginMode;
	class TaskBar *TB;
};
