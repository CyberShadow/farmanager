#pragma once

/*
dialog.hpp

����� ������� Dialog.

������������ ��� ����������� ��������� ��������.
�������� ����������� �� ������ Frame.
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

#include "frame.hpp"
#include "plugin.hpp"
#include "vmenu.hpp"
#include "bitflags.hpp"
#include "CriticalSections.hpp"
#include "macro.hpp"

class History;

// ����� �������� ������ �������
enum DIALOG_MODES
{
	DMODE_INITOBJECTS           =0x00000001, // �������� ����������������?
	DMODE_CREATEOBJECTS         =0x00000002, // ������� (Edit,...) �������?
	DMODE_WARNINGSTYLE          =0x00000004, // Warning Dialog Style?
	DMODE_DRAGGED               =0x00000008, // ������ ���������?
	DMODE_ISCANMOVE             =0x00000010, // ����� �� ������� ������?
	DMODE_ALTDRAGGED            =0x00000020, // ������ ��������� �� Alt-�������?
	DMODE_SMALLDIALOG           =0x00000040, // "�������� ������"
	DMODE_DRAWING               =0x00001000, // ������ ��������?
	DMODE_KEY                   =0x00002000, // ���� ������� ������?
	DMODE_SHOW                  =0x00004000, // ������ �����?
	DMODE_MOUSEEVENT            =0x00008000, // ����� �������� MouseMove � ����������?
	DMODE_RESIZED               =0x00010000, //
	DMODE_ENDLOOP               =0x00020000, // ����� ����� ��������� �������?
	DMODE_BEGINLOOP             =0x00040000, // ������ ����� ��������� �������?
	DMODE_ISMENU                =0x00080000, // ������ �������� ����������� VMenu2
	DMODE_NODRAWSHADOW          =0x00100000, // �� �������� ����?
	DMODE_NODRAWPANEL           =0x00200000, // �� �������� ��������?
	DMODE_FULLSHADOW            =0x00400000,
	DMODE_NOPLUGINS             =0x00800000,
	DMODE_KEEPCONSOLETITLE      =0x10000000, // �� �������� ��������� �������
	DMODE_CLICKOUTSIDE          =0x20000000, // ���� ������� ���� ��� �������?
	DMODE_MSGINTERNAL           =0x40000000, // ���������� Message?
	DMODE_OLDSTYLE              =0x80000000, // ������ � ������ (�� 1.70) �����
};

//#define DIMODE_REDRAW       0x00000001 // ��������� �������������� ���������� �����?

#define MakeDialogItemsEx(Data,Item) \
	DialogItemEx Item[ARRAYSIZE(Data)]; \
	ItemToItemEx(Data,Item,ARRAYSIZE(Data));

// ���������, ����������� ������������� ��� DIF_AUTOMATION
// �� ������ ����� - ����������� - ����������� ������ � ��������� ��� CheckBox
struct DialogItemAutomation
{
	WORD ID;                    // ��� ����� ��������...
	FARDIALOGITEMFLAGS Flags[3][2];          // ...��������� ��� ��� �����
	// [0] - Unchecked, [1] - Checked, [2] - 3Checked
	// [][0] - Set, [][1] - Skip
};

class DlgUserControl
{
	public:
		COORD CursorPos;
		bool CursorVisible;
		DWORD CursorSize;

	public:
		DlgUserControl():
			CursorVisible(false),
			CursorSize(static_cast<DWORD>(-1))
		{
			CursorPos.X=CursorPos.Y=-1;
		}
		~DlgUserControl() {};
};

/*
��������� ���� ������� ������� - ��������� �������������.
��� �������� ��� FarDialogItem
*/
struct DialogItemEx: public FarDialogItem
{
	int ListPos;
	string strHistory;
	string strMask;
	string strData;

	int ID;
	BitFlags IFlags;
	DialogItemAutomation* AutoPtr;
	size_t AutoCount;
	void *ObjPtr;
	VMenu *ListPtr;
	DlgUserControl *UCData;
	intptr_t SelStart;
	intptr_t SelEnd;

	DialogItemEx() {Clear();}

	void Clear()
	{
		ClearStruct(*static_cast<FarDialogItem*>(this));

		ListPos=0;
		strHistory.Clear();
		strMask.Clear();
		strData.Clear();
		ID=0;
		IFlags.ClearAll();
		AutoCount=0;
		AutoPtr=nullptr;
		UserData=0;
		ObjPtr=nullptr;
		ListPtr=nullptr;
		UCData=nullptr;
		SelStart=0;
		SelEnd=0;
	}

	DialogItemEx &operator=(const FarDialogItem &Other)
	{
		*static_cast<FarDialogItem*>(this) = Other;
		return *this;
	}

	void Indent(int Delta)
	{
		X1 += Delta;
		X2 += Delta;
	}

	bool AddAutomation(int id,
		FARDIALOGITEMFLAGS UncheckedSet,FARDIALOGITEMFLAGS UncheckedSkip,
		FARDIALOGITEMFLAGS CheckedSet,FARDIALOGITEMFLAGS CheckedSkip,
		FARDIALOGITEMFLAGS Checked3Set,FARDIALOGITEMFLAGS Checked3Skip)
	{
		DialogItemAutomation *Auto;

		if ((Auto=(DialogItemAutomation*)xf_realloc(AutoPtr,sizeof(DialogItemAutomation)*(AutoCount+1))) )
		{
			AutoPtr=Auto;
			Auto=AutoPtr+AutoCount;
			Auto->ID=id;
			Auto->Flags[0][0]=UncheckedSet;
			Auto->Flags[0][1]=UncheckedSkip;
			Auto->Flags[1][0]=CheckedSet;
			Auto->Flags[1][1]=CheckedSkip;
			Auto->Flags[2][0]=Checked3Set;
			Auto->Flags[2][1]=Checked3Skip;
			AutoCount++;
			return true;
		}
		return false;
	}
};

class DlgEdit;
class ConsoleTitle;

class DialogOwner
{
public:
	intptr_t Handler(HANDLE hDlg, intptr_t Msg, intptr_t Param1, void* Param2);
};
typedef int (DialogOwner::*DialogHandlerFunction)(HANDLE hDlg, intptr_t Msg, intptr_t Param1, void* Param2);

class Dialog: public Frame
{
		friend class DlgEdit;
		friend intptr_t WINAPI SendDlgMessage(HANDLE hDlg,intptr_t Msg,intptr_t Param1,void* Param2);
		friend intptr_t WINAPI DefDlgProc(HANDLE hDlg,intptr_t Msg,intptr_t Param1,void* Param2);

	private:
		bool bInitOK;               // ������ ��� ������� ���������������
		class Plugin* PluginOwner;       // ������, ��� ������������ HelpTopic
		unsigned FocusPos;               // ������ �������� ����� ������� � ������
		unsigned PrevFocusPos;           // ������ �������� ����� ������� ��� � ������
		int IsEnableRedraw;         // ��������� ����������� �������? ( 0 - ���������)
		BitFlags DialogMode;        // ����� �������� ������ �������

		void* DataDialog;        // ������, ������������� ��� ����������� ���������� ������� (������������� ����� ��������, ���������� � �����������)

		DialogItemEx **Item; // ������ ��������� �������
		DialogItemEx * pSaveItemEx; // ���������������� ������ ��������� �������

		unsigned ItemCount;         // ���������� ��������� �������

		ConsoleTitle *OldTitle;     // ���������� ���������
		MACROMODEAREA PrevMacroMode;          // ���������� ����� �����

		DialogOwner *OwnerClass;
		union
		{
			DialogHandlerFunction DialogHandler;
			FARWINDOWPROC RealDlgProc;      // ������� ��������� �������
		};
		// ���������� ��� ����������� �������
		int OldX1,OldX2,OldY1,OldY2;

		wchar_t *HelpTopic;

		volatile int DropDownOpened;// �������� ������ ���������� � �������: TRUE - ������, FALSE - ������.

		CriticalSection CS;

		int RealWidth, RealHeight;

		GUID Id;
		bool IdExist;

		MOUSE_EVENT_RECORD PrevMouseRecord;

	protected:
		unsigned InitDialogObjects(unsigned ID=(unsigned)-1);

	private:
		void Construct(DialogItemEx* SrcItem, size_t SrcItemCount, DialogOwner* OwnerClass, DialogHandlerFunction HandlerFunction, void* InitParam=nullptr);
		void Init(DialogOwner* OwnerClass, DialogHandlerFunction HandlerFunction, FARWINDOWPROC DlgProc, void* InitParam);
		virtual void DisplayObject();
		void DeleteDialogObjects();
		int  LenStrItem(int ID, const wchar_t *lpwszStr = nullptr);

		void ShowDialog(unsigned ID=(unsigned)-1);  //    ID=-1 - ���������� ���� ������

		intptr_t CtlColorDlgItem(FarColor Color[4], int ItemPos,int Type,int Focus,int Default,FARDIALOGITEMFLAGS Flags);
		/* $ 28.07.2000 SVS
		   + �������� ����� ����� ����� ����� ����������.
		     ������� �������� ��� ����, ����� ���������� DMSG_KILLFOCUS & DMSG_SETFOCUS
		*/
		void ChangeFocus2(unsigned SetFocusPos);

		unsigned ChangeFocus(unsigned FocusPos,int Step,int SkipGroup);
		BOOL SelectFromEditHistory(DialogItemEx *CurItem,DlgEdit *EditLine,const wchar_t *HistoryName,string &strStr);
		int SelectFromComboBox(DialogItemEx *CurItem,DlgEdit*EditLine,VMenu *List);
		int AddToEditHistory(DialogItemEx* CurItem, const wchar_t *AddStr);

		void ProcessLastHistory(DialogItemEx *CurItem, int MsgIndex);  // ��������� DIF_USELASTHISTORY

		int ProcessHighlighting(int Key,unsigned FocusPos,int Translate);
		int CheckHighlights(WORD Chr,int StartPos=0);

		void SelectOnEntry(unsigned Pos,BOOL Selected);

		void CheckDialogCoord();
		BOOL GetItemRect(unsigned I,SMALL_RECT& Rect);
		bool ItemHasDropDownArrow(const DialogItemEx *Item);

		// ���������� ��������� ������� (����� ������� ������ ��� ������)
		const wchar_t *GetDialogTitle();

		BOOL SetItemRect(unsigned ID,SMALL_RECT *Rect);

		/* $ 23.06.2001 KM
		   + ������� ������������ ��������/�������� ���������� � �������
		     � ��������� ������� ����������/���������� ���������� � �������.
		*/
		volatile void SetDropDownOpened(int Status) { DropDownOpened=Status; }
		volatile int GetDropDownOpened() { return DropDownOpened; }

		void ProcessCenterGroup();
		unsigned ProcessRadioButton(unsigned);

		int ProcessOpenComboBox(FARDIALOGITEMTYPES Type,DialogItemEx *CurItem,unsigned CurFocusPos);
		int ProcessMoveDialog(DWORD Key);

		int Do_ProcessTab(int Next);
		int Do_ProcessNextCtrl(int Next,BOOL IsRedraw=TRUE);
		int Do_ProcessFirstCtrl();
		int Do_ProcessSpace();
		void SetComboBoxPos(DialogItemEx* Item=nullptr);
		void CalcComboBoxPos(DialogItemEx* CurItem, intptr_t ItemCount, int &X1, int &Y1, int &X2, int &Y2);

		intptr_t CallDlgProc(int nMsg, int nParam1, void* Param2);

		void ProcessKey(int Key, unsigned ItemPos);

	public:
		template<class T, typename Y>
		Dialog(T* OwnerClass, Y HandlerFunction, void* InitParam, DialogItemEx* SrcItem, size_t SrcItemCount)
		{
			Construct(SrcItem, SrcItemCount, reinterpret_cast<DialogOwner*>(OwnerClass), reinterpret_cast<DialogHandlerFunction>(HandlerFunction), InitParam);
		}
		Dialog(DialogItemEx *SrcItem, size_t SrcItemCount,
		       FARWINDOWPROC DlgProc=nullptr,void* InitParam=nullptr);
		Dialog(const FarDialogItem *SrcItem, size_t SrcItemCount,
		       FARWINDOWPROC DlgProc=nullptr,void* InitParam=nullptr);
		bool InitOK() {return bInitOK;}
		virtual ~Dialog();

	public:
		virtual int ProcessKey(int Key);
		virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
		virtual __int64 VMProcess(int OpCode,void *vParam=nullptr,__int64 iParam=0);
		virtual void Show();
		virtual void Hide();
		void FastShow() {ShowDialog();}

		void GetDialogObjectsData();

		void SetDialogMode(DWORD Flags) { DialogMode.Set(Flags); }
		bool CheckDialogMode(DWORD Flags) { return DialogMode.Check(Flags)!=FALSE; }

		// ����� ��� ����������� �������
		void AdjustEditPos(int dx,int dy);

		int IsMoving() {return DialogMode.Check(DMODE_DRAGGED);}
		void SetModeMoving(bool IsMoving) { DialogMode.Change(DMODE_ISCANMOVE,IsMoving);}
		int  GetModeMoving() {return DialogMode.Check(DMODE_ISCANMOVE);}
		void SetDialogData(void* NewDataDialog);
		void* GetDialogData() {return DataDialog;};

		void InitDialog();
		void Process();
		void SetPluginOwner(Plugin* NewPluginAddress) {PluginOwner = ((NewPluginAddress == INVALID_HANDLE_VALUE)? nullptr : NewPluginAddress);}
		Plugin* GetPluginOwner() const {return PluginOwner;}

		void SetHelp(const wchar_t *Topic);
		void ShowHelp();
		int Done() { return DialogMode.Check(DMODE_ENDLOOP); }
		void ClearDone();
		virtual void SetExitCode(int Code);

		intptr_t CloseDialog();

		virtual int GetTypeAndName(string &strType, string &strName);
		virtual int GetType() { return MODALTYPE_DIALOG; }
		virtual const wchar_t *GetTypeName() {return L"[Dialog]";};

		virtual MACROMODEAREA GetMacroMode();

		/* $ ������� ��� ���� CtrlAltShift OT */
		virtual int FastHide();
		virtual void ResizeConsole();
//    virtual void OnDestroy();

		// For MACRO
		const DialogItemEx **GetAllItem() {return (const DialogItemEx**)Item;};
		unsigned GetAllItemCount() {return ItemCount;};             // ���������� ��������� �������
		unsigned GetDlgFocusPos() {return FocusPos;};


		int SetAutomation(WORD IDParent,WORD id,
		                  FARDIALOGITEMFLAGS UncheckedSet,FARDIALOGITEMFLAGS UncheckedSkip,
		                  FARDIALOGITEMFLAGS CheckedSet,FARDIALOGITEMFLAGS CheckedSkip,
		                  FARDIALOGITEMFLAGS Checked3Set=DIF_NONE,FARDIALOGITEMFLAGS Checked3Skip=DIF_NONE);

		intptr_t WINAPI DlgProc(HANDLE hDlg,intptr_t Msg,intptr_t Param1,void* Param2);

		virtual void SetPosition(int X1,int Y1,int X2,int Y2);

		BOOL IsInited();
		bool ProcessEvents();

		void SetId(const GUID& Id);
		const GUID& GetId(){return Id;}

		friend class History;
};

intptr_t WINAPI SendDlgMessage(HANDLE hDlg,intptr_t Msg,intptr_t Param1,void* Param2);

intptr_t WINAPI DefDlgProc(HANDLE hDlg,intptr_t Msg,intptr_t Param1,void* Param2);

bool IsKeyHighlighted(const wchar_t *Str,int Key,int Translate,int AmpPos=-1);

void ItemToItemEx(const FarDialogItem *Data, DialogItemEx *Item, size_t Count, bool Short = false);
