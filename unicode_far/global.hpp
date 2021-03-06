#pragma once

/*
global.hpp

�������� ���������� ����������
�������� ���������.
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

class global
{
public:
	global();
	~global();

	clock_t StartIdleTime;

	OSVERSIONINFO WinVer;

	int WaitInMainLoop;
	int WaitInFastFind;

	string g_strFarModuleName;
	string g_strFarINI;
	string g_strFarPath;

	string strGlobalSearchString;
	string strInitTitle;

	// BUGBUG
	string strMsgHelpTopic;

	bool GlobalSearchCase;
	bool GlobalSearchWholeWords; // �������� "Whole words" ��� ������
	bool GlobalSearchHex; // �������� "Search for hex" ��� ������

	bool GlobalSearchReverse;

	int ScreenSaverActive;

	int CloseFAR, CloseFARMenu, AllowCancelExit;

	int DisablePluginsOutput;

	BOOL IsProcessAssignMacroKey;
	BOOL IsRedrawFramesInProcess;

	size_t PluginPanelsCount;

	const wchar_t* Version;
	const wchar_t* Copyright;

	int WidthNameForMessage;

	BOOL ProcessException;

	BOOL ProcessShowClock;

	const wchar_t *HelpFileMask;
	const wchar_t *HelpFormatLinkModule;

#if defined(SYSLOG)
	BOOL StartSysLog;
	long CallNewDelete;
	long CallMallocFree;
#endif

#ifdef DIRECT_RT
	bool DirectRT;
#endif

	class SaveScreen *GlobalSaveScrPtr;

	int CriticalInternalError;

	int KeepUserScreen;
	string g_strDirToSet; //RAVE!!!

	int Macro_DskShowPosType; // ��� ����� ������ �������� ���� ������ ������ (0 - ������� �� ��������, 1 - ����� (AltF1), 2 - ������ (AltF2))

	SYSTEM_INFO SystemInfo;

	DWORD ErrorMode;

	LARGE_INTEGER FarUpTime;

	HANDLE MainThreadHandle;
	DWORD MainThreadId;

	class ImportedFunctions* ifn;
	class console* Console;
	class ScreenBuf* ScrBuf;
	class TaskBarCore* TBC;
	class consoleicons* ConsoleIcons;
	class FormatScreen FS;
	class TPreRedrawFunc* PreRedraw;
	class WindowHandler *Window;
	class Options *Opt;
	class Language *Lang;
	class Language *OldLang;
	class elevation *Elevation;
	class TreeListCache* TreeCache;
	class TreeListCache* tempTreeCache;
	class PluginSynchro* PluginSynchroManager;
	class codepages* CodePages;
};

extern global* Global;

#define MSG(ID) Global->Lang->GetMsg(ID)

inline bool IsPtr(const void* Address)
{
	return reinterpret_cast<uintptr_t>(Address)>=reinterpret_cast<uintptr_t>(Global->SystemInfo.lpMinimumApplicationAddress) && reinterpret_cast<uintptr_t>(Address)<=reinterpret_cast<uintptr_t>(Global->SystemInfo.lpMaximumApplicationAddress);
}


inline bool MainThread() {return GetCurrentThreadId() == Global->MainThreadId;}

// VersionConstant: LOWBYTE - minor, HIBYTE - major
inline bool operator< (const OSVERSIONINFO& OsVersionInfo, WORD VersionConstant) {return MAKEWORD(OsVersionInfo.dwMinorVersion, OsVersionInfo.dwMajorVersion) < VersionConstant;}
inline bool operator> (const OSVERSIONINFO& OsVersionInfo, WORD VersionConstant) {return MAKEWORD(OsVersionInfo.dwMinorVersion, OsVersionInfo.dwMajorVersion) > VersionConstant;}
inline bool operator<= (const OSVERSIONINFO& OsVersionInfo, WORD VersionConstant) {return MAKEWORD(OsVersionInfo.dwMinorVersion, OsVersionInfo.dwMajorVersion) <= VersionConstant;}
inline bool operator>= (const OSVERSIONINFO& OsVersionInfo, WORD VersionConstant) {return MAKEWORD(OsVersionInfo.dwMinorVersion, OsVersionInfo.dwMajorVersion) >= VersionConstant;}
