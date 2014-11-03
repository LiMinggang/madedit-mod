///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADOPTIONSDIALOG_H_
#define _MADOPTIONSDIALOG_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/choice.h>
#include <wx/menu.h>
#include <wx/listbox.h>
#include <wx/treectrl.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
////Header Include End

#include <wx/dialog.h>

#include "MadCommand.h"
#include <list>
using std::list;


//Compatibility for 2.4.x code
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadOptionsDialog_STYLE
#define MadOptionsDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End


class TreeItemData: public wxTreeItemData
{
public:
    CommandData *cmddata;
    wxArrayString keys;
};

class KeyTextCtrl;
class wxAuiNotebook;

class MadOptionsDialog : public wxDialog
{
public:
    MadOptionsDialog( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Options"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = MadOptionsDialog_STYLE);
    virtual ~MadOptionsDialog();

public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
		wxStaticText *WxStaticText21;
		wxButton *WxButtonBitmapDir;
		wxTextCtrl *WxEditBitMapDir;
		wxBoxSizer *WxBoxSizer32;
		wxStaticText *WxStaticText20;
		wxButton *WxButtonThesauriDir;
		wxTextCtrl *WxEditThesauriDir;
		wxBoxSizer *WxBoxSizer31;
		wxStaticText *WxStaticText19;
		wxButton *WxButtonDictionaryDir;
		wxTextCtrl *WxEditDictionaryDir;
		wxBoxSizer *WxBoxSizer30;
		wxStaticBoxSizer *WxStaticBoxSizer6;
		wxChoice *WxChoiceDictionary;
		wxStaticText *WxStaticText18;
		wxStaticBoxSizer *WxStaticBoxSizer5;
		wxCheckBox *WxCheckBoxPersonalDict;
		wxBoxSizer *WxBoxSizer33;
		wxMenu *WxPopupMenuPrintMark;
		wxMenu *WxPopupMenuDateTimeMark;
		wxButton *WxButtonCancel;
		wxButton *WxButtonOK;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer29;
		wxPanel *WxNoteBookPage5;
		wxCheckBox *WxCheckBoxResetAllKeys;
		wxBoxSizer *WxBoxSizer26;
		wxButton *WxButtonShowInMenu;
		wxButton *WxButtonDeleteKey;
		wxButton *WxButtonAddKey;
		wxTextCtrl *WxEditKeyHint;
		KeyTextCtrl *WxEditKey;
		wxStaticText *WxStaticText15;
		wxBoxSizer *WxBoxSizer25;
		wxListBox *WxListBoxKeys;
		wxStaticText *WxStaticText14;
		wxBoxSizer *WxBoxSizer22;
		wxBoxSizer *WxBoxSizer21;
		wxTextCtrl *WxEditCommandHint;
		wxStaticText *WxStaticTextCommandHint;
		wxBoxSizer *WxBoxSizer24;
		wxBoxSizer *WxBoxSizer20;
		wxTreeCtrl *WxTreeCtrl1;
		wxBoxSizer *WxBoxSizer19;
		wxBoxSizer *WxBoxSizer18;
		wxPanel *WxNoteBookPage4;
		wxButton *WxButton6;
		wxTextCtrl *WxEditFooterRight;
		wxStaticText *WxStaticText12;
		wxButton *WxButton5;
		wxTextCtrl *WxEditFooterCenter;
		wxStaticText *WxStaticText11;
		wxButton *WxButton4;
		wxTextCtrl *WxEditFooterLeft;
		wxStaticText *WxStaticText10;
		wxFlexGridSizer *WxFlexGridSizer2;
		wxCheckBox *WxCheckBoxPrintPageFooter;
		wxStaticBoxSizer *WxStaticBoxSizer4;
		wxButton *WxButton3;
		wxTextCtrl *WxEditHeaderRight;
		wxStaticText *WxStaticText9;
		wxButton *WxButton2;
		wxTextCtrl *WxEditHeaderCenter;
		wxStaticText *WxStaticText8;
		wxButton *WxButton1;
		wxTextCtrl *WxEditHeaderLeft;
		wxStaticText *WxStaticText7;
		wxFlexGridSizer *WxFlexGridSizer1;
		wxCheckBox *WxCheckBoxPrintPageHeader;
		wxStaticBoxSizer *WxStaticBoxSizer3;
		wxBoxSizer *WxBoxSizer16;
		wxRadioBox *WxRadioBoxPrintOffset;
		wxStaticBoxSizer *WxStaticBoxSizer2;
		wxCheckBox *WxCheckBoxPrintSpaceChar;
		wxCheckBox *WxCheckBoxPrintTabChar;
		wxCheckBox *WxCheckBoxPrintEndOfLine;
		wxCheckBox *WxCheckBoxPrintLineNumber;
		wxCheckBox *WxCheckBoxPrintSyntax;
		wxStaticBoxSizer *WxStaticBoxSizer1;
		wxBoxSizer *WxBoxSizer15;
		wxBoxSizer *WxBoxSizer14;
		wxPanel *WxNoteBookPage3;
		wxCheckBox *WxCheckBoxMiddleMouseToPaste;
		wxStaticText *WxStaticText17;
		wxRadioButton *WxRadioButtonDisable;
		wxRadioButton *WxRadioButtonEnable;
		wxCheckBox *WxCheckBoxMouseSelectToCopy;
		wxBoxSizer *WxBoxSizer28;
		wxCheckBox *WxCheckBoxAutoCompletePair;
		wxCheckBox *WxCheckBoxAutoIndent;
		wxCheckBox *WxCheckBoxTabOrSpaces;
		wxBoxSizer *WxBoxSizer12;
		wxCheckBox *WxCheckBoxDateTimeInEnglish;
		wxButton *WxButtonDateTime;
		wxStaticText *WxStaticTextDateTime;
		wxTextCtrl *WxEditDateTime;
		wxBoxSizer *WxBoxSizer23;
		wxStaticText *WxStaticText6;
		wxTextCtrl *WxEditIndentColumns;
		wxBoxSizer *WxBoxSizer13;
		wxStaticText *WxStaticText5;
		wxTextCtrl *WxEditTabColumns;
		wxBoxSizer *WxBoxSizer11;
		wxStaticText *WxStaticText4;
		wxTextCtrl *WxEditMaxColumns;
		wxBoxSizer *WxBoxSizer10;
		wxStaticText *WxStaticText3;
		wxTextCtrl *WxEditMaxLineLength;
		wxBoxSizer *WxBoxSizer9;
		wxBoxSizer *WxBoxSizer8;
		wxPanel *WxNoteBookPage2;
		wxCheckBox *WxCheckBoxPurgeHistory;
		wxCheckBox *WxCheckBoxDoNotSaveSettings;
		wxCheckBox *WxCheckBoxRestoreCaretPos;
		wxCheckBox *WxCheckBoxReloadFiles;
		wxBoxSizer *WxBoxSizer7;
		wxStaticText *WxStaticText13;
		wxComboBox *WxComboBoxEncoding;
		wxBoxSizer *WxBoxSizer17;
		wxStaticText *WxStaticText2;
		wxTextCtrl *WxEditMaxTextFileSize;
		wxBoxSizer *WxBoxSizer6;
		wxStaticText *WxStaticText1;
		wxTextCtrl *WxEditMaxSizeToLoad;
		wxBoxSizer *WxBoxSizer5;
		wxCheckBox *WxCheckBoxRecordCaretMovements;
		wxCheckBox *WxCheckBoxSingleInstance;
		wxBoxSizer *WxBoxSizer4;
		wxStaticText *WxStaticText16;
		wxComboBox *WxComboBoxLanguage;
		wxBoxSizer *WxBoxSizer27;
		wxBoxSizer *WxBoxSizer3;
		wxPanel *WxNoteBookPage1;
		wxAuiNotebook *WxNotebook1;
		wxBoxSizer *WxBoxSizer1;
  ////GUI Control Declaration End

private:
    DECLARE_EVENT_TABLE()

public:
    //Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
    //#defines may replace a numeric value for the enums names.
    //Try copy pasting the below block in your old Form header Files.
	enum {
////GUI Enum Control ID Start
			ID_DUMMY_START = 1000,
			ID_MNU_MENUITEM1_1110,
			ID_MNU___P__PATHNAME_1111,
			ID_MNU_MENUITEM3_1112,
			ID_MNU___N_PAGE_NUMBER_1113,
			ID_MNU___S__TOTALPAGES_1114,
			ID_MNU_MENUITEM6_1115,
			ID_MNU___D__DATE_1116,
			ID_MNU___T__TIME_1117,
			
			ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191,
			ID_MNU___D__M__Y_24_02_2007_1192,
			ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195,
			ID_MNU_MENUITEM1_1170,
			ID_MNU___A_FULLWEEKDAYNAME_1171,
			ID_MNU___B_ABBREVIATEDMONTHNAME_1172,
			ID_MNU___B_FULLMONTHNAME_1173,
			ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174,
			ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175,
			ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176,
			ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177,
			ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178,
			ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179,
			ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180,
			ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181,
			ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182,
			ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183,
			ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184,
			ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185,
			ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186,
			ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187,
			ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188,
			ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189,
			ID_MNU___Z_TIME_ZONENAME_1193,
			ID_MNU___Z_TIME_ZONEABBREVIATION_1194,
			
			ID_WXBUTTONCANCEL,
			ID_WXBUTTONOK,
			ID_WXSTATICTEXT21,
			ID_WXBITMAP_DIR,
			ID_WXEDITBITMAPDIR,
			ID_WXSTATICTEXT20,
			ID_WXTHESAURI_DIR,
			ID_WXEDITTHESAURI,
			ID_WXSTATICTEXT19,
			ID_WXDICTIONARY_DIR,
			ID_WXEDITDICTIONARYDIR,
			ID_WXCHOICEDICTIONARY,
			ID_WXSTATICTEXT18,
			ID_WXCHECKBOXPERSONALDICT,
			ID_WXNOTEBOOKPAGE5,
			ID_WXCHECKBOXRESETALLKEYS,
			ID_WXBUTTONSHOWINMENU,
			ID_WXBUTTONDELETEKEY,
			ID_WXBUTTONADDKEY,
			ID_WXEDITKEYHINT,
			ID_WXEDITKEY,
			ID_WXSTATICTEXT15,
			ID_WXLISTBOXKEYS,
			ID_WXSTATICTEXT14,
			ID_WXEDITHINT,
			ID_WXSTATICTEXTCOMMANDHINT,
			ID_WXTREECTRL1,
			ID_WXNOTEBOOKPAGE4,
			ID_WXBUTTON6,
			ID_WXBUTTON5,
			ID_WXBUTTON4,
			ID_WXCHECKBOXPRINTPAGEFOOTER,
			ID_WXBUTTON3,
			ID_WXEDITHEADERRIGHT,
			ID_WXSTATICTEXT9,
			ID_WXBUTTON2,
			ID_WXEDITHEADERCENTER,
			ID_WXSTATICTEXT8,
			ID_WXBUTTON1,
			ID_WXEDITHEADERLEFT,
			ID_WXSTATICTEXT7,
			ID_WXCHECKBOXPRINTPAGEHEADER,
			ID_WXRADIOBOXPRINTOFFSET,
			ID_WXCHECKBOXPRINTSPACECHAR,
			ID_WXCHECKBOXPRINTTABCHAR,
			ID_WXCHECKBOXPRINTENDOFLINE,
			ID_WXCHECKBOXPRINTLINENUMBER,
			ID_WXCHECKBOXPRINTSYNTAX,
			ID_WXNOTEBOOKPAGE3,
			ID_WXCHECKBOXMIDDLEMOUSETOPASTE,
			ID_WXSTATICTEXT17,
			ID_WXRADIOBUTTONDISABLE,
			ID_WXRADIOBUTTONENABLE,
			ID_WXCHECKBOXMOUSESELECTTOCOPY,
			ID_WXCHECKBOXAUTOCOMPLETEPAIR,
			ID_WXCHECKBOXAUTOINDENT,
			ID_WXCHECKBOXTABORSPACES,
			ID_WXCHECKBOXDATETIMEINENGLISH,
			ID_WXBUTTONDATETIME,
			ID_WXSTATICTEXTDATETIME,
			ID_WXEDITDATETIME,
			ID_WXSTATICTEXT5,
			ID_WXEDITTABCOLUMNS,
			ID_WXSTATICTEXT4,
			ID_WXEDITMAXCOLUMNS,
			ID_WXSTATICTEXT3,
			ID_WXEDITMAXLINELENGTH,
			ID_WXNOTEBOOKPAGE2,
			ID_PURGEHISTORY,
			ID_WXCHECKBOXDONOTSAVESETTINGS,
			ID_WXCHECKBOXRESTORECARETPOS,
			ID_WXCHECKBOXRELOADFILES,
			ID_WXSTATICTEXT13,
			ID_WXCOMBOBOXENCODING,
			ID_WXSTATICTEXT2,
			ID_WXEDITMAXTEXTFILESIZE,
			ID_WXSTATICTEXT1,
			ID_WXEDITMAXSIZETOLOAD,
			ID_WXCHECKBOXRECORDCARETMOVEMENTS,
			ID_WXCHECKBOXSINGLEINSTANCE,
			ID_WXSTATICTEXT16,
			ID_WXCOMBOBOXLANGUAGE,
			ID_WXNOTEBOOKPAGE1,
			ID_WXNOTEBOOK1,
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    int ButtonID;
    list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
    list<TreeItemData*> ChangedTreeItemDataList;
    
    TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
    bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
    void UpdateKeyHint();
    void InitDictionaryChoice(const wxString &path = wxEmptyString);

#ifdef __WXMSW__
    wxCheckBox *WxCheckBoxRightClickMenu;
#endif
public:
	void MadOptionsDialogClose(wxCloseEvent& event);
	void CreateGUIControls(void);
	void MadOptionsDialogActivate(wxActivateEvent& event);
	void LoadOptions(void);
	void WxButtonOKClick(wxCommandEvent& event);
	void WxButtonCancelClick(wxCommandEvent& event);
	void PrintMarkClick(wxCommandEvent& event);
	void PrintMarkButtonClick(wxCommandEvent& event);
	void DateTimeMarkClick(wxCommandEvent& event);
	void WxTreeCtrl1SelChanged(wxTreeEvent& event);
	void WxButtonAddKeyClick(wxCommandEvent& event);
	void WxListBoxKeysSelected(wxCommandEvent& event);
	void WxButtonDeleteKeyClick(wxCommandEvent& event);
	void WxButtonDateTimeClick(wxCommandEvent& event);
	void WxButtonShowInMenuClick(wxCommandEvent& event);
//	void WxButtonBitmapDirClick(wxCommandEvent& event);
//	void WxButtonThesauriDirClick(wxCommandEvent& event);
	void WxButtonDictionaryDirClick(wxCommandEvent& event);
	void OnDictionaryDirChange(wxCommandEvent& event);
	void OnSelectDictionary(wxCommandEvent& event);
};


extern MadOptionsDialog *g_OptionsDialog;

#endif
 
 
 
 
