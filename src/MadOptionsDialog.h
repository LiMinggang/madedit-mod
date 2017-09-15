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
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/aui/aui.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
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
		wxStaticText* StaticText10;
		wxComboBox* ComboBoxEncoding;
		wxStaticText* StaticText9;
		wxTextCtrl* EditAutoSaveTimeout;
#ifdef __WXMSW__
		wxCheckBox* CheckBoxRightClickMenu;
#endif
		wxTextCtrl* EditDefaultFontSize;
		wxCheckBox* CheckBoxSkipAutoEncoding;
		wxCheckBox* CheckBoxRecordCaretMovements;
		wxPanel* Panel5;
		wxMenu PopupMenuDateTimeMark;
		wxCheckBox* CheckBoxAutoIndent;
		wxCheckBox* CheckBoxNewDocEncUTF8WithBOM;
		wxButton* Button4;
		wxCheckBox* CheckBoxRestoreCaretPos;
		wxCheckBox* CheckBoxAutoFillColumnPaste;
		wxTextCtrl* EditTabColumns;
		wxComboBox* ComboBoxNewDocFont;
		wxStaticText* StaticText13;
		wxStaticText* StaticText2;
		wxPanel* Panel4;
		wxListBox* ListBoxKeys;
		wxTextCtrl* EditMaxLineLength;
		wxStaticText* StaticText14;
		wxTextCtrl* EditMaxColumns;
		wxRadioButton* RadioButtonNewDocEncSystemDefault;
		wxTextCtrl* EditCommandHint;
		wxButton* Button1;
		wxStaticText* StaticText6;
		wxCheckBox* CheckBoxPrintPageHeader;
		wxComboBox* ComboBoxLanguage;
		wxMenu PopupMenuPrintMark;
		wxButton* ButtonOK;
		wxCheckBox* CheckBoxDateTimeInEnglish;
		wxStaticText* StaticTextAutoSaveTimeout;
		wxRadioBox* RadioBoxPrintOffset;
		wxButton* ButtonDateTime;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxTextCtrl* EditFooterCenter;
		wxCheckBox* CheckBoxPrintTabChar;
		wxButton* ButtonDeleteKey;
		wxTextCtrl* EditHeaderLeft;
		wxRadioButton* RadioButtonNewDocLineEndingLF;
		wxPanel* Panel1;
		wxCheckBox* CheckBoxInsertPairForSelection;
		wxStaticText* StaticText1;
		wxCheckBox* CheckBoxPrintPageFooter;
		wxCheckBox* CheckBoxDoNotSaveSettings;
		wxStaticText* StaticText3;
		wxButton* ButtonShowInMenu;
		wxRadioButton* RadioButtonNewDocLineEndingDefault;
		wxButton* Button2;
		wxPanel* Panel6;
		wxPanel* Panel3;
		wxTextCtrl* EditMaxSizeToLoad;
		wxButton* Button6;
		wxTextCtrl* EditIndentColumns;
		wxCheckBox* CheckBoxSingleInstance;
		wxButton* ButtonDictionaryDir;
		wxStaticText* StaticTextDateTime;
		wxCheckBox* CheckBoxPersonalDict;
		wxCheckBox* CheckBoxPrintSyntax;
		wxButton* ButtonCancel;
		wxButton* Button5;
		wxRadioButton* RadioButtonNewDocEncUTF8;
		wxRadioButton* RadioButtonNewDocLineEndingCRLF;
		wxCheckBox* CheckBoxMiddleMouseToPaste;
		wxButton* ButtonAddKey;
		wxButton* Button3;
		wxTextCtrl* EditMaxDisplaySize;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxCheckBox* CheckBoxTypewriterMode;
		wxCheckBox* CheckBoxCtrlWithMouseToSelect;
		wxRadioButton* RadioButtonNewDocEncOther;
		wxTreeCtrl* TreeCtrl1;
		wxCheckBox* CheckBoxFixWidthMode;
		wxCheckBox* CheckBoxShowQSearchBar;
		wxAuiNotebook* AuiNotebook1;
		wxRadioButton* RadioButtonNewDocLineEndingCR;
		wxCheckBox* CheckBoxTabOrSpaces;
		wxTextCtrl* EditDictionaryDir;
		wxChoice* ChoiceDictionary;
		wxTextCtrl* EditDateTime;
		wxStaticText* StaticText15;
		wxStaticText* StaticText12;
		wxTextCtrl* EditFooterLeft;
		wxCheckBox* CheckBoxPrintBookmark;
		wxCheckBox* CheckBoxReloadFiles;
		wxPanel* Panel2;
		wxTextCtrl* EditNewDocFontSize;
		wxCheckBox* CheckBoxLDClickHighlight;
		wxCheckBox* CheckBoxEnableAutoBackup;
		wxComboBox* ComboBoxNewDocSyntax;
		wxCheckBox* CheckBoxAutoCompletePair;
		wxCheckBox* CheckBoxPrintEndOfLine;
		wxCheckBox* CheckBoxPrintSpaceChar;
		wxCheckBox* CheckBoxPrintLineNumber;
		wxStaticText* StaticText17;
		wxTextCtrl* EditHeaderRight;
		wxStaticText* StaticText4;
		KeyTextCtrl* EditKey;
		wxComboBox* ComboBoxNewDocEncOther;
		wxComboBox* ComboBoxDefaultFont;
		wxCheckBox* CheckBoxResetAllKeys;
		wxCheckBox* CheckBoxPurgeHistory;
		wxCheckBox* CheckBoxMouseSelectToCopy;
		wxStaticText* StaticText16;
		wxStaticText* StaticTextCommandHint;
		wxTextCtrl* EditMaxTextFileSize;
		wxTextCtrl* EditKeyHint;
		wxTextCtrl* EditFooterRight;
		wxTextCtrl* EditHeaderCenter;
		wxCheckBox* CheckBoxEnableAutoSave;
		//*)

	protected:
		friend class KeyTextCtrl;
		//(*Identifiers(MadOptionsDialog)
		static const long ID_COMBOBOXLANGUAGE;
		static const long ID_STATICTEXT16;
		static const long ID_COMBOBOXENCODING;
		static const long ID_STATICTEXT13;
		static const long ID_CHECKBOXSKIPAUTOENCODING;
		static const long ID_COMBOBOXDEFAULTFONT;
		static const long ID_TEXTCTRLDEFAULTFONTSIZE;
		static const long ID_EDITMAXSIZETOLOAD;
		static const long ID_EDITMAXTEXTFILESIZE;
		static const long ID_MAXDISPLAYSIZE;
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
		static const long ID_RADIOBUTTONNEWDOCLINEENDINGDEFAULT;
		static const long ID_RADIOBUTTONNEWDOCLINEENDINGCRLF;
		static const long ID_RADIOBUTTONNEWDOCLINEENDINGLF;
		static const long ID_RADIOBUTTONNEWDOCLINEENDINGCR;
		static const long ID_COMBOBOXNEWDOCSYNTAX;
		static const long ID_RADIOBUTTONNEWDOCENCSYSTEMDEFAULT;
		static const long ID_RADIOBUTTONNEWDOCENCUTF8;
		static const long ID_CHECKBOXNEWDOCENCUTF8WITHBOM;
		static const long ID_RADIOBUTTONNEWDOCENCOTHER;
		static const long ID_COMBOBOXNEWDOCENCOTHER;
		static const long ID_COMBOBOXNEWDOCFONT;
		static const long ID_TEXTCTRLNEWDOCFONTSIZE;
		static const long ID_PANEL6;
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
		void CheckBoxMouseSelectToCopyClick(wxCommandEvent& event);
		void ButtonDictionaryDirClick(wxCommandEvent& event);
		void CheckBoxAutoCompletePairClick(wxCommandEvent& event);
		void EnableAutoSaveClick(wxCommandEvent& event);
		void ChoiceDictionarySelect(wxCommandEvent& event);
		void EditDictionaryDirTextEnter(wxCommandEvent& event);
		void RadioButtonNewDocEncSelect(wxCommandEvent& event);
		void RadioButtonNewDocLineEndingSelect(wxCommandEvent& event);
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
	static wxCmdEvtHandlerMap_t m_radiobutton_evt_map[];

public:
	void LoadOptions(void);

	int ButtonID;
	list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
	list<TreeItemData*> ChangedTreeItemDataList;
	
	TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
	bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
	void UpdateKeyHint();
	void InitDictionaryChoice(const wxString &path = wxEmptyString);
	int m_NewDocLineEnding;
	wxString m_NewDocEncoding;

private:
};


extern MadOptionsDialog *g_OptionsDialog;

#endif
