#pragma once

/*
treelist.hpp

Tree panel
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

#include "panel.hpp"
#include "macro.hpp"

enum
{
	MODALTREE_ACTIVE  =1,
	MODALTREE_PASSIVE =2,
	MODALTREE_FREE    =3
};

struct TreeItem
{
	string strName;
	int *Last;
	size_t LastCount;
	int Depth;             // ������� �����������

	TreeItem()
	{
		LastCount=MAX_PATH/2;
		Last=static_cast<int*>(xf_malloc(LastCount*sizeof(*Last)));
		Clear();
	}
	~TreeItem()
	{
		xf_free(Last);
	}

	void Clear()
	{
		strName.Clear();
		memset(Last,0,LastCount*sizeof(*Last));
		Depth=0;
	}

	TreeItem& operator=(const TreeItem &tiCopy)
	{
		if (this != &tiCopy)
		{
			strName=tiCopy.strName;
			*Last=*tiCopy.Last;
			LastCount = tiCopy.LastCount;
			Depth=tiCopy.Depth;
		}

		return *this;
	}
};

enum TREELIST_FLAGS
{
	FTREELIST_TREEISPREPARED          = 0x00010000,
	FTREELIST_UPDATEREQUIRED          = 0x00020000,
	FTREELIST_ISPANEL                 = 0x00040000,
};

class TreeListCache
{
public:
	TreeListCache()
	{
		ListName=nullptr;
		TreeCount=0;
		TreeSize=0;
	}

	void Resize()
	{
		if (TreeCount==TreeSize)
		{
			TreeSize+=TreeSize?TreeSize>>2:32;
			wchar_t **NewPtr=(wchar_t**)xf_realloc(ListName,sizeof(wchar_t*)*TreeSize);

			if (!NewPtr)
				return;

			ListName=NewPtr;
		}
	}

	void Add(const wchar_t* name)
	{
		Resize();
		ListName[TreeCount++]=xf_wcsdup(name);
	}

	void Insert(int idx,const wchar_t* name)
	{
		Resize();
		memmove(ListName+idx+1,ListName+idx,sizeof(wchar_t*)*(TreeCount-idx));
		ListName[idx]=xf_wcsdup(name);
		TreeCount++;
	}

	void Delete(int idx)
	{
		if (ListName[idx]) xf_free(ListName[idx]);

		memmove(ListName+idx,ListName+idx+1,sizeof(wchar_t*)*(TreeCount-idx-1));
		TreeCount--;
	}

	void Clean()
	{
		if (!TreeSize)return;

		for (int i=0; i<TreeCount; i++)
		{
			if (ListName[i]) xf_free(ListName[i]);
		}

		if (ListName) xf_free(ListName);

		ListName=nullptr;
		TreeCount=0;
		TreeSize=0;
		strTreeName.Clear();
	}

	//TODO: ���������� ��������������!
	void Copy(TreeListCache *Dest)
	{
		Dest->Clean();

		for (int I=0; I < TreeCount; I++)
			Dest->Add(ListName[I]);
	}

	//BUGBUG
//private:
	string strTreeName;
	wchar_t **ListName;
	int TreeCount;
	int TreeSize;
};

class TreeList: public Panel
{
	private:
		MACROMODEAREA PrevMacroMode;
		TreeItem **ListData;
		string strRoot;
		long TreeCount;
		long WorkDir;
		long GetSelPosition;
		int NumericSort;
		int CaseSensitiveSort;
		int ExitCode; // ��������� ������ ��� ������, ���������� �� ������!

		struct TreeItem *SaveListData;
		long SaveTreeCount;
		long SaveWorkDir;

	private:
		void SetMacroMode(int Restore = FALSE);
		virtual void DisplayObject();
		void DisplayTree(int Fast);
		void DisplayTreeName(const wchar_t *Name,int Pos);
		void Up(int Count);
		void Down(int Count);
		void Scroll(int Count);
		void CorrectPosition();
		bool FillLastData();
		UINT CountSlash(const wchar_t *Str);
		int SetDirPosition(const wchar_t *NewDir);
		void GetRoot();
		Panel* GetRootPanel();
		void SyncDir();
		void SaveTreeFile();
		int ReadTreeFile();
		virtual size_t GetSelCount();
		void DynamicUpdateKeyBar();
		int GetNextNavPos();
		int GetPrevNavPos();
		static string &MkTreeFileName(const wchar_t *RootDir,string &strDest);
		static string &MkTreeCacheFolderName(const wchar_t *RootDir,string &strDest);
		static string &CreateTreeFileName(const wchar_t *Path,string &strDest);

		bool SaveState();
		bool RestoreState();

	private:
		static int MsgReadTree(int TreeCount,int &FirstCall);
		static int GetCacheTreeName(const string& Root, string& strName,int CreateDir);

	public:
		TreeList(bool IsPanel=true);
	private:
		virtual ~TreeList();

	public:
		virtual int ProcessKey(int Key);
		virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
		virtual __int64 VMProcess(int OpCode,void *vParam=nullptr,__int64 iParam=0);
//    virtual void KillFocus();
		virtual void Update(int Mode);
		int  ReadTree();

		virtual BOOL SetCurDir(const string& NewDir,int ClosePanel,BOOL IsUpdated=TRUE);

		void SetRootDir(const wchar_t *NewRootDir);

		virtual int GetCurDir(string &strCurDir);

		virtual int GetCurName(string &strName, string &strShortName);

		virtual void UpdateViewPanel();
		virtual void MoveToMouse(MOUSE_EVENT_RECORD *MouseEvent);
		virtual int FindPartName(const wchar_t *Name,int Next,int Direct=1,int ExcludeSets=0);
		virtual bool GetPlainString(string& Dest,int ListPos);

		virtual int GoToFile(long idxItem);
		virtual int GoToFile(const wchar_t *Name,BOOL OnlyPartName=FALSE);
		virtual long FindFile(const wchar_t *Name,BOOL OnlyPartName=FALSE);

		void ProcessEnter();

		virtual long FindFirst(const wchar_t *Name);
		virtual long FindNext(int StartPos, const wchar_t *Name);

		int GetExitCode() {return ExitCode;}
		virtual long GetFileCount() {return TreeCount;}
		virtual int GetFileName(string &strName,int Pos,DWORD &FileAttr);

		virtual void SetTitle();
		virtual string &GetTitle(string &Title,int SubLen=-1,int TruncSize=0);
		virtual void SetFocus();
		virtual void KillFocus();
		virtual BOOL UpdateKeyBar();
		virtual BOOL GetItem(int Index,void *Dest);
		virtual int GetCurrentPos();

		virtual int GetSelName(string *strName,DWORD &FileAttr,string *ShortName=nullptr,FAR_FIND_DATA_EX *fd=nullptr);

	public:
		static void AddTreeName(const wchar_t *Name);
		static void DelTreeName(const wchar_t *Name);
		static void RenTreeName(const string& SrcName, const string& DestName);
		static void ReadSubTree(const string& Path);
		static void ClearCache(int EnableFreeMem);
		static void ReadCache(const string& TreeRoot);
		static void FlushCache();

		static int MustBeCached(const wchar_t *Root); // $ 16.10.2000 tran - �������, ������������� ������������� ����������� �����
		static void PR_MsgReadTree();
};
