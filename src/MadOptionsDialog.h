///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADOPTIONSDIALOG_H_
#define _MADOPTIONSDIALOG_H_

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadOptionsDialog)
#include <wx/aui/aui.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <list>
#include <map>
using std::list;

#if __cplusplus <= 199711L
#ifndef nullptr
#define nullptr (0)
#endif
#endif

#include "MadCommand.h"

class TreeItemData: public wxTreeItemData
{
public:
	CommandData *cmddata;
	wxArrayString keys;
};

class KeyTextCtrl;
class wxAuiNotebook;
#ifdef MADEDIT_ENABLE_MADEDIT
class MadEdit;
#endif

class MadOptionsDialog: public wxDialog
{
	public:

		MadOptionsDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~MadOptionsDialog();

		//(*Declarations(MadOptionsDialog)
		KeyTextCtrl* EditKey;
		wxAuiNotebook* AuiNotebook1;
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button3;
		wxButton* Button4;
		wxButton* Button5;
		wxButton* Button6;
		wxButton* ButtonAddKey;
		wxButton* ButtonCancel;
		wxButton* ButtonDateTime;
		wxButton* ButtonDeleteKey;
		wxButton* ButtonDictionaryDir;
		wxButton* ButtonOK;
		wxButton* ButtonShowInMenu;
		wxCheckBox* CheckBoxAutoCompletePair;
		wxCheckBox* CheckBoxAutoFillColumnPaste;
		wxCheckBox* CheckBoxAutoIndent;
		wxCheckBox* CheckBoxCtrlWithMouseToSelect;
		wxCheckBox* CheckBoxDateTimeInEnglish;
		wxCheckBox* CheckBoxDoNotSaveSettings;
		wxCheckBox* CheckBoxEnableAutoBackup;
		wxCheckBox* CheckBoxEnableAutoSave;
		wxCheckBox* CheckBoxFixWidthMode;
		wxCheckBox* CheckBoxInsertPairForSelection;
		wxCheckBox* CheckBoxLDClickHighlight;
		wxCheckBox* CheckBoxMiddleMouseToPaste;
		wxCheckBox* CheckBoxMouseSelectToCopy;
		wxCheckBox* CheckBoxPersonalDict;
		wxCheckBox* CheckBoxPrintBookmark;
		wxCheckBox* CheckBoxPrintEndOfLine;
		wxCheckBox* CheckBoxPrintLineNumber;
		wxCheckBox* CheckBoxPrintPageFooter;
		wxCheckBox* CheckBoxPrintPageHeader;
		wxCheckBox* CheckBoxPrintSpaceChar;
		wxCheckBox* CheckBoxPrintSyntax;
		wxCheckBox* CheckBoxPrintTabChar;
		wxCheckBox* CheckBoxPurgeHistory;
		wxCheckBox* CheckBoxRecordCaretMovements;
		wxCheckBox* CheckBoxReloadFiles;
		wxCheckBox* CheckBoxResetAllKeys;
		wxCheckBox* CheckBoxRestoreCaretPos;
#ifdef __WXMSW__
		wxCheckBox *CheckBoxRightClickMenu;
#endif
		wxCheckBox* CheckBoxShowQSearchBar;
		wxCheckBox* CheckBoxSingleInstance;
		wxCheckBox* CheckBoxTabOrSpaces;
		wxCheckBox* CheckBoxTypewriterMode;
		wxChoice* ChoiceDictionary;
		wxComboBox* ComboBoxEncoding;
		wxComboBox* ComboBoxLanguage;
		wxListBox* ListBoxKeys;
		wxMenu PopupMenuDateTimeMark;
		wxMenu PopupMenuPrintMark;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* Panel4;
		wxPanel* Panel5;
		wxRadioBox* RadioBoxPrintOffset;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText15;
		wxStaticText* StaticText16;
		wxStaticText* StaticText17;
		wxStaticText* StaticText18;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* StaticTextAutoSaveTimeout;
		wxStaticText* StaticTextCommandHint;
		wxStaticText* StaticTextDateTime;
		wxTextCtrl* EditAutoSaveTimeout;
		wxTextCtrl* EditCommandHint;
		wxTextCtrl* EditDateTime;
		wxTextCtrl* EditDictionaryDir;
		wxTextCtrl* EditFooterCenter;
		wxTextCtrl* EditFooterLeft;
		wxTextCtrl* EditFooterRight;
		wxTextCtrl* EditHeaderCenter;
		wxTextCtrl* EditHeaderLeft;
		wxTextCtrl* EditHeaderRight;
		wxTextCtrl* EditIndentColumns;
		wxTextCtrl* EditKeyHint;
		wxTextCtrl* EditMaxColumns;
		wxTextCtrl* EditMaxDisplaySize;
		wxTextCtrl* EditMaxLineLength;
		wxTextCtrl* EditMaxSizeToLoad;
		wxTextCtrl* EditMaxTextFileSize;
		wxTextCtrl* EditTabColumns;
		wxTreeCtrl* TreeCtrl1;
		//*)

	protected:
		friend class KeyTextCtrl;
		//(*Identifiers(MadOptionsDialog)
		static const long ID_COMBOBOXLANGUAGE;
		static const long ID_STATICTEXT16;
		static const long ID_COMBOBOXENCODING;
		static const long ID_STATICTEXT13;
		static const long ID_EDITMAXSIZETOLOAD;
		static const long ID_STATICTEXT1;
		static const long ID_EDITMAXTEXTFILESIZE;
		static const long ID_STATICTEXT2;
		static const long ID_MAXDISPLAYSIZE;
		static const long ID_STATICTEXT18;
		static const long ID_CHECKBOXSINGLEINSTANCE;
		static const long ID_CHECKBOXRELOADFILES;
		static const long ID_CHECKBOXRECORDCARETMOVEMENTS;
		static const long ID_CHECKBOXRESTORECARETPOS;
		static const long ID_CHECKBOXSHOWQSEARCHBAR;
		static const long ID_CHECKBOXDONOTSAVESETTINGS;
		static const long ID_CHECKBOXPURGEHISTORY;
		static const long ID_CHECKBOXENABLEAUTOSAVE;
		static const long ID_TEXTCTRLAUTOSAVETIMEOUT;
		static const long ID_CHECKBOX1;
		static const long ID_WSCHECKBOXMSRIGHTCCLICKMENU;
		static const long ID_PANEL1;
		static const long ID_EDITMAXLINELENGTH;
		static const long ID_STATICTEXT3;
		static const long ID_EDITMAXCOLUMNS;
		static const long ID_STATICTEXT4;
		static const long ID_EDITTABCOLUMNS;
		static const long ID_STATICTEXT5;
		static const long ID_EDITINDENTCOLUMNS;
		static const long ID_STATICTEXT6;
		static const long ID_EDITDATETIME;
		static const long ID_STATICTEXTDATETIME;
		static const long ID_BUTTONDATETIME;
		static const long ID_CHECKBOXDATETIMEINENGLISH;
		static const long ID_CHECKBOXTABORSPACES;
		static const long ID_CHECKBOXAUTOINDENT;
		static const long ID_CHECKBOXAUTOCOMPLETEPAIR;
		static const long ID_CHECKBOXINSERTPAIRFORSELECTION;
		static const long ID_CHECKBOXMOUSESELECTTOCOPY;
		static const long ID_CHECKBOXWHENPRESSCTRLKEY;
		static const long ID_CHECKBOXMIDDLEMOUSETOPASTE;
		static const long ID_CHECKBOXAUTOFILLCOLUMN;
		static const long ID_CHECKBOXDCLICKHIGHLIGHT;
		static const long ID_CHECKBOXLOCKCARETYPOS;
		static const long ID_CHECKBOXFIXWIDTHMODE;
		static const long ID_PANEL2;
		static const long ID_CHECKBOXPRINTSYNTAX;
		static const long ID_CHECKBOXPRINTLINENUMBER;
		static const long ID_CHECKBOXPRINTBOOKMARK;
		static const long ID_CHECKBOXPRINTENDOFLINE;
		static const long ID_CHECKBOXPRINTTABCHAR;
		static const long ID_CHECKBOXPRINTSPACECHAR;
		static const long ID_RADIOBOXPRINTOFFSET;
		static const long ID_CHECKBOXPRINTPAGEHEADER;
		static const long ID_STATICTEXT7;
		static const long ID_EDITHEADERLEFT;
		static const long ID_BUTTON1;
		static const long ID_STATICTEXT8;
		static const long ID_EDITHEADERCENTER;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT9;
		static const long ID_EDITHEADERRIGHT;
		static const long ID_BUTTON3;
		static const long ID_CHECKBOXPRINTPAGEFOOTER;
		static const long ID_STATICTEXT10;
		static const long ID_EDITFOOTERLEFT;
		static const long ID_BUTTON4;
		static const long ID_STATICTEXT11;
		static const long ID_EDITFOOTERLEFTCENTER;
		static const long ID_BUTTON5;
		static const long ID_STATICTEXT12;
		static const long ID_EDITFOOTERRIGHT;
		static const long ID_BUTTON6;
		static const long ID_PANEL3;
		static const long ID_TREECTRL1;
		static const long ID_STATICTEXTCOMMANDHINT;
		static const long ID_EDITHINT;
		static const long ID_STATICTEXT14;
		static const long ID_LISTBOXKEYS;
		static const long ID_STATICTEXT15;
		static const long ID_EDITKEY;
		static const long ID_EDITKEYHINT;
		static const long ID_BUTTONADDKEY;
		static const long ID_BUTTONDELETEKEY;
		static const long ID_BUTTONSHOWINMENU;
		static const long ID_CHECKBOXRESETALLKEYS;
		static const long ID_PANEL4;
		static const long ID_CHECKBOXPERSONALDICT;
		static const long ID_CHOICEDICTIONARY;
		static const long ID_STATICTEXT17;
		static const long ID_EDITDICTIONARYDIR;
		static const long ID_DICTIONARY_DIR;
		static const long ID_PANEL5;
		static const long ID_AUINOTEBOOK1;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

		static const long ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191;
		static const long ID_MNU___D__M__Y_24_02_2007_1192;
		static const long ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195;
		static const long ID_MNU_MENUITEM1_1170;
		static const long ID_MNU___A_FULLWEEKDAYNAME_1171;
		static const long ID_MNU___B_ABBREVIATEDMONTHNAME_1172;
		static const long ID_MNU___B_FULLMONTHNAME_1173;
		static const long ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174;
		static const long ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175;
		static const long ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176;
		static const long ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177;
		static const long ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178;
		static const long ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179;
		static const long ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180;
		static const long ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181;
		static const long ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182;
		static const long ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183;
		static const long ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184;
		static const long ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185;
		static const long ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186;
		static const long ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187;
		static const long ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188;
		static const long ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189;
		static const long ID_MNU___Z_TIME_ZONENAME_1193;
		static const long ID_MNU___Z_TIME_ZONEABBREVIATION_1194;

		static const long ID_MNU_MENUITEM1_1110;
		static const long ID_MNU___P__PATHNAME_1111;
		static const long ID_MNU_MENUITEM3_1112;
		static const long ID_MNU___N_PAGE_NUMBER_1113;
		static const long ID_MNU___S__TOTALPAGES_1114;
		static const long ID_MNU_MENUITEM6_1115;
		static const long ID_MNU___D__DATE_1116;
		static const long ID_MNU___T__TIME_1117;

	private:

		//(*Handlers(MadOptionsDialog)
		void MadOptionsDialogClose(wxCloseEvent& event);
		void ButtonAddKeyClick(wxCommandEvent& event);
		void ButtonCancelClick(wxCommandEvent& event);
		void ButtonDateTimeClick(wxCommandEvent& event);
		void ButtonDeleteKeyClick(wxCommandEvent& event);
		void ButtonOKClick(wxCommandEvent& event);
		void ButtonShowInMenuClick(wxCommandEvent& event);
		void ListBoxKeysSelected(wxCommandEvent& event);
		void TreeCtrl1SelChanged(wxTreeEvent& event);
		void ButtonCheckNowClick(wxCommandEvent& event);
		void CheckBoxMouseSelectToCopyClick(wxCommandEvent& event);
		void ButtonDictionaryDirClick(wxCommandEvent& event);
		void CheckBoxAutoCompletePairClick(wxCommandEvent& event);
		void EnableAutoSaveClick(wxCommandEvent& event);
		void ChoiceDictionarySelect(wxCommandEvent& event);
		void EditDictionaryDirTextEnter(wxCommandEvent& event);
		//*)

		void MadOptionsDialogActivate(wxActivateEvent& event);


		void PrintMarkClick(wxCommandEvent& event);
		void PrintMarkButtonClick(wxCommandEvent& event);
		void DateTimeMarkClick(wxCommandEvent& event);
private:
	typedef struct 
	{
		const long evtTag;
		void (MadOptionsDialog::*method)( wxCommandEvent &);
	} wxCmdEvtHandlerMap_t;
	static wxCmdEvtHandlerMap_t m_menu_evt_map[];
	static wxCmdEvtHandlerMap_t m_button_evt_map[];
	static wxCmdEvtHandlerMap_t m_checkbox_evt_map[];

public:
	void LoadOptions(void);

    int ButtonID;
    list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
    list<TreeItemData*> ChangedTreeItemDataList;
    
    TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
    bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
    void UpdateKeyHint();
    void InitDictionaryChoice(const wxString &path = wxEmptyString);

private:
};


extern MadOptionsDialog *g_OptionsDialog;

#endif
