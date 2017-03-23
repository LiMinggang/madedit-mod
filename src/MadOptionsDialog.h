///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
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
#include <wx/spinctrl.h>
#include <wx/menu.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/aui/auibook.h>
#include <wx/choice.h>
#include <wx/listbox.h>
#include <wx/treectrl.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>

#define MADEDIT_ENABLE_MADEDIT

#ifdef MADEDIT_ENABLE_STC
#include <wx/stc/stc.h>
#endif
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
#define MadOptionsDialog_STYLE wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
////Dialog Style End


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
	wxCheckBox *WxCheckPadComma;
	wxCheckBox *WxCheckPadReturnType;
	wxCheckBox *WxCheckUnpadReturnType;
	wxCheckBox *WxCheckPadParamType;
	wxCheckBox *WxCheckUnpadParamType;
	wxCheckBox *WxCheckBreakOneLineHeaders;
	wxTextCtrl *WxEditIndentContinuation;
	wxStaticText *WxStaticText32;
	wxBoxSizer *WxBoxSizer53;
	wxCheckBox *WxCheckBoxInsertPairForSelection;
	wxBoxSizer *WxBoxSizer52;
	wxCheckBox *WxCheckBoxEnableAutoBackup;
	wxCheckBox *WxCheckBoxEnableAutoSave;
	wxTextCtrl *WxEditAutoSaveTimeout;
	wxStaticText *WxStaticText30;
	wxBoxSizer *WxBoxSizer51;
	wxCheckBox *WxCheckBoxShowQSearchBar;
	wxTextCtrl *WxEditXmlIndentSize;
	wxStaticText *WxStaticText29;
	wxBoxSizer *WxBoxSizer50;
	wxTextCtrl *WxEditXMLversion;
	wxStaticText *WxStaticText28;
	wxBoxSizer *WxBoxSizer49;
	wxBoxSizer *WxBoxSizer48;
	wxPanel *WxAuiNoteBookPage6;
	wxTextCtrl *WxEditMaxInStatementIndent;
	wxStaticText *WxStaticText23;
	wxBoxSizer *WxBoxSizer41;
	wxSpinCtrl *WxSpinMinConditionalEvent;
	wxStaticText *WxStaticText22;
	wxBoxSizer *WxBoxSizer40;
	wxCheckBox *WxCheckIndentCol1Comments;
	wxCheckBox *WxCheckIndentPreprocCond;
	wxCheckBox *WxCheckIndentPreprocDefine;
	wxCheckBox *WxCheckIndentPreprocBlock;
	wxCheckBox *WxCheckIndentSwitches;
	wxCheckBox *WxCheckIndentNamespaces;
	wxCheckBox *WxCheckIndentModifiers;
	wxCheckBox *WxCheckIndentLabels;
	wxCheckBox *WxCheckIndentClasses;
	wxCheckBox *WxCheckIndentCase;
	wxCheckBox *WxCheckForceUseTabs;
	wxCheckBox *WxCheckUseTab;
	wxBoxSizer *WxBoxSizer38;
	wxSpinCtrl *WxSpinIndentation;
	wxStaticText *WxStaticText17;
	wxBoxSizer *WxBoxSizer37;
	wxBoxSizer *WxBoxSizer39;
	wxCheckBox *WxCheckBoxPrintBookmark;
	wxMenu *WxPopupMenuDateTimeMark;
	wxMenu *WxPopupMenuPrintMark;
	wxButton *WxButtonCancel;
	wxButton *WxButtonOK;
	wxBoxSizer *WxBoxSizer2;
	wxChoice *WxChoiceReferenceAlign;
	wxStaticText *WxStaticText26;
	wxChoice *WxChoicePointerAlign;
	wxStaticText *WxStaticText25;
	wxBoxSizer *WxBoxSizer46;
	wxCheckBox *WxCheckFillEmptyLines;
	wxCheckBox *WxCheckDelEmptyLine;
	wxCheckBox *WxCheckUnpadParens;
	wxCheckBox *WxCheckPadHeader;
	wxCheckBox *WxCheckPadParensIn;
	wxCheckBox *WxCheckPadParensOut;
	wxCheckBox *WxCheckPadOperators;
	wxCheckBox *WxCheckBreakBlocksAll;
	wxCheckBox *WxCheckBreakBlocks;
	wxBoxSizer *WxBoxSizer45;
	wxPanel *WxAuiNoteBookPage5;
	wxCheckBox *WxCheckBreakAfterLogical;
	wxBoxSizer *WxBoxSizer44;
	wxTextCtrl *WxEditSFMaxLineLength;
	wxStaticText *WxStaticText24;
	wxBoxSizer *WxBoxSizer43;
	wxCheckBox *WxCheckBreakLines;
	wxCheckBox *WxCheckRemoveCommentPrefix;
	wxCheckBox *WxCheckCloseTemplates;
	wxCheckBox *WxCheckConvertTabs;
	wxCheckBox *WxCheckKeepBlocks;
	wxCheckBox *WxCheckRemoveBrackets;
	wxCheckBox *WxCheckAddOneLineBrackets;
	wxCheckBox *WxCheckAddBrackets;
	wxCheckBox *WxCheckBreakElseIfs;
	wxCheckBox *WxCheckBreakClosing;
	wxBoxSizer *WxBoxSizer42;
	wxPanel *WxAuiNoteBookPage4;
	wxPanel *WxAuiNoteBookPage3;
	wxCheckBox *WxCheckAttachInlines;
	wxCheckBox *WxCheckAttachNamespaces;
	wxCheckBox *WxCheckAttachExternC;
	wxCheckBox *WxCheckAttachClasses;
	wxBoxSizer *WxBoxSizer35;
	wxPanel *WxAuiNoteBookPage2;
#ifdef MADEDIT_ENABLE_MADEDIT
	MadEdit *WxTextSample;
#elif MADEDIT_ENABLE_STC
	wxStyledTextCtrl *WxTextSample;
#else
	wxTextCtrl *WxTextSample;
#endif
	wxStaticText *WxStaticText27;
	wxStaticBoxSizer *WxStaticBoxSizer7;
	wxRadioBox *WxRadioBoxBracketStyle;
	wxBoxSizer *WxBoxSizer47;
	wxPanel *WxAuiNoteBookPage1;
	wxAuiNotebook *WxAuiNotebook1;
	wxBoxSizer *WxBoxSizer34;
	wxPanel *WxNoteBookPage6;
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
	wxCheckBox *WxCheckBoxAutoFillColumnPaste;
	wxCheckBox *WxCheckBoxTypewriterMode;
	wxCheckBox *WxCheckBoxMiddleMouseToPaste;
	wxCheckBox *WxCheckBoxCtrlWithMouseToSelect;
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
	wxCheckBox* WxCheckBoxFixWidthMode;
	wxCheckBox* WxCheckBoxLDClickHighlight; 
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
  static const long ID_WXBUTTONCANCEL;

private:
	enum{
		ID_WXBUTTON1 = 197453,
		ID_WXBUTTON2,
		ID_WXBUTTON3,
		ID_WXBUTTON4,
		ID_WXBUTTON5,
		ID_WXBUTTON6,
	};
	typedef struct 
	{
		const long evtTag;
		void (MadOptionsDialog::*method)( wxCommandEvent &);
	} wxCmdEvtHandlerMap_t;
	static wxCmdEvtHandlerMap_t m_menu_evt_map[];
	static wxCmdEvtHandlerMap_t m_button_evt_map[];
	static wxCmdEvtHandlerMap_t m_checkbox_evt_map[];

	static const long ID_WXCHECKPADCOMMA;
	static const long ID_WXCHECKPADRETURNTYPE;
	static const long ID_WXCHECKUNPADRETURNTYPE;
	static const long ID_WXCHECKPADPARAMTYPE;
	static const long ID_WXCHECKUNPADPARAMTYPE;
	static const long ID_WXCHECKBREAKONELINEHEADERS;
	static const long ID_WXSTATICTEXT32;
	static const long ID_WXCHECKINDENTCONTINUATION;
	static const long ID_WXCHECKBOXDCLICKHIGHLIGHT;
	static const long ID_WXCHECKBOXINSERTPAIRFORSELECTION;
	static const long ID_WXSTATICTEXT31;
	static const long ID_WXCHECKBOXENABLEAUTOBACKUP;
	static const long ID_WXEDITAUTOSAVETIMEOUT;
	static const long ID_WXSTATICTEXT30;
	static const long ID_WXCHECKBOXENABLEAUTOSAVE;
	static const long ID_WXCHECKBOXFIXWIDTHMODE;
	static const long ID_WXCHECKBOXSHOWQSEARCHBAR;
	static const long ID_WXEDITXMLINDENTSIZE;
	static const long ID_WXSTATICTEXT29;
	static const long ID_WXEDITXMLVERSION;
	static const long ID_WXSTATICTEXT28;
	static const long ID_WXAUINOTEBOOKPAGE6;
	static const long ID_WXEDITMAXINSTATEMENTINDENT;
	static const long ID_WXSTATICTEXT23;
	static const long ID_WXSPINCTRLMININDENT;
	static const long ID_WXSTATICTEXT22;
	static const long ID_WXCHECKINDENETCPPCOMMENTS;
	static const long ID_WXCHECKINDENTPREPCOND;
	static const long ID_WXCHECKINDENTMULTIPREP;
	static const long ID_WXCHECKINDENTPREP;
	static const long ID_WXCHECKINDENTSWITCHS;
	static const long ID_WXCHECKINDENTNS;
	static const long ID_WXCHECKINDENTMODIFIERS;
	static const long ID_WXCHECKINDENTLABELS;
	static const long ID_WXCHECKINDENETCLASSES;
	static const long ID_WXCHECKINDENTCASE;
	static const long ID_WXCHECKFORCETABS;
	static const long ID_WXCHECKUSETABOVERSPACES;
	static const long ID_WXSPINCTRLINDENTSIZE;
	static const long ID_WXSTATICTEXT17;
	static const long ID_WXCHECKBOXPRINTBOOKMARK;
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

	static const long ID_WXBUTTONOK;
	static const long ID_WXCHOICEREFERENCEALIGN;
	static const long ID_WXSTATICTEXT26;
	static const long ID_WXCHOICEPOINTERALIGN;
	static const long ID_WXSTATICTEXT25;
	static const long ID_WXCHECKFILLEMPTYLINES;
	static const long ID_WXCHECKDELEMPTYLINE;
	static const long ID_WXCHECKUNPADPARENS;
	static const long ID_WXCHECKPADHEADER;
	static const long ID_WXCHECKPADPARENSIN;
	static const long ID_WXCHECKPADPARENSOUT;
	static const long ID_WXCHECKPADOPERATORS;
	static const long ID_WXCHECKBREAKBLOCKSALL;
	static const long ID_WXCHECKBREAKBLOCKS;
	static const long ID_WXAUINOTEBOOKPAGE5;
	static const long ID_WXCHECKBREAKAFTERLOGICAL;
	static const long ID_WXEDITSFMAXLINELENGTH;
	static const long ID_WXSTATICTEXT24;
	static const long ID_WXCHECKBREAKLINES;
	static const long ID_WXCHECKREMOVECOMMENTPREFIX;
	static const long ID_WXCHECKCLOSETEMPLATES;
	static const long ID_WXCHECKCONVERTTABS;
	static const long ID_WXCHECKKEEPBLOCKS;
	static const long ID_WXCHECKBOX17;
	static const long ID_WXCHECKADDONELINEBRACKETS;
	static const long ID_WXCHECKADDBRACKETS;
	static const long ID_WXCHECKBREAKELSEIFS;
	static const long ID_WXCHECKBREAKCLOSING;
	static const long ID_WXAUINOTEBOOKPAGE4;
	static const long ID_WXAUINOTEBOOKPAGE3;
	static const long ID_WXCHECKATTACHINLINES;
	static const long ID_WXCHECKATTACHNAMESPACES;
	static const long ID_WXCHECKATTACHEXTERNC;
	static const long ID_WXCHECKATTACHCLASSES;
	static const long ID_WXAUINOTEBOOKPAGE2;
	static const long ID_WXRICHTEXTSAMPLE;
	static const long ID_WXSTATICTEXT27;
	static const long ID_WXRADIOBOXBRACKETSTYLE;
	static const long ID_WXAUINOTEBOOKPAGE1;
	static const long ID_WXAUINOTEBOOK1;
	static const long ID_WXNOTEBOOKPAGE6;
	static const long ID_WXSTATICTEXT21;
	static const long ID_WXBITMAP_DIR;
	static const long ID_WXEDITBITMAPDIR;
	static const long ID_WXSTATICTEXT20;
	static const long ID_WXTHESAURI_DIR;
	static const long ID_WXEDITTHESAURI;
	static const long ID_WXSTATICTEXT19;
	static const long ID_WXDICTIONARY_DIR;
	static const long ID_WXEDITDICTIONARYDIR;
	static const long ID_WXCHOICEDICTIONARY;
	static const long ID_WXSTATICTEXT18;
	static const long ID_WXCHECKBOXPERSONALDICT;
	static const long ID_WXNOTEBOOKPAGE5;
	static const long ID_WXCHECKBOXRESETALLKEYS;
	static const long ID_WXBUTTONSHOWINMENU;
	static const long ID_WXBUTTONDELETEKEY;
	static const long ID_WXBUTTONADDKEY;
	static const long ID_WXEDITKEYHINT;
	static const long ID_WXEDITKEY;
	static const long ID_WXSTATICTEXT15;
	static const long ID_WXLISTBOXKEYS;
	static const long ID_WXSTATICTEXT14;
	static const long ID_WXEDITHINT;
	static const long ID_WXSTATICTEXTCOMMANDHINT;
	static const long ID_WXTREECTRL1;
	static const long ID_WXNOTEBOOKPAGE4;
	static const long ID_WXCHECKBOXPRINTPAGEFOOTER;
	static const long ID_WXEDITHEADERRIGHT;
	static const long ID_WXSTATICTEXT9;
	static const long ID_WXEDITHEADERCENTER;
	static const long ID_WXSTATICTEXT8;
	static const long ID_WXEDITHEADERLEFT;
	static const long ID_WXSTATICTEXT7;
	static const long ID_WXCHECKBOXPRINTPAGEHEADER;
	static const long ID_WXRADIOBOXPRINTOFFSET;
	static const long ID_WXCHECKBOXPRINTSPACECHAR;
	static const long ID_WXCHECKBOXPRINTTABCHAR;
	static const long ID_WXCHECKBOXPRINTENDOFLINE;
	static const long ID_WXCHECKBOXPRINTLINENUMBER;
	static const long ID_WXCHECKBOXPRINTSYNTAX;
	static const long ID_WXNOTEBOOKPAGE3;
	static const long ID_WXCHECKBOXAUTOFILLCOLUMN;
	static const long ID_WXCHECKBOXTYPEWRITERMODE;
	static const long ID_WXCHECKBOXMIDDLEMOUSETOPASTE;
	static const long ID_WXCHECKBOXCTRLWITHMOUSE;
	static const long ID_WXCHECKBOXMOUSESELECTTOCOPY;
	static const long ID_WXCHECKBOXAUTOCOMPLETEPAIR;
	static const long ID_WXCHECKBOXAUTOINDENT;
	static const long ID_WXCHECKBOXTABORSPACES;
	static const long ID_WXCHECKBOXDATETIMEINENGLISH;
	static const long ID_WXBUTTONDATETIME;
	static const long ID_WXSTATICTEXTDATETIME;
	static const long ID_WXEDITDATETIME;
	static const long ID_WXSTATICTEXT5;
	static const long ID_WXEDITTABCOLUMNS;
	static const long ID_WXSTATICTEXT4;
	static const long ID_WXEDITMAXCOLUMNS;
	static const long ID_WXSTATICTEXT3;
	static const long ID_WXEDITMAXLINELENGTH;
	static const long ID_WXNOTEBOOKPAGE2;
	static const long ID_PURGEHISTORY;
	static const long ID_WXCHECKBOXDONOTSAVESETTINGS;
	static const long ID_WXCHECKBOXRESTORECARETPOS;
	static const long ID_WXCHECKBOXRELOADFILES;
	static const long ID_WXSTATICTEXT13;
	static const long ID_WXCOMBOBOXENCODING;
	static const long ID_WXSTATICTEXT2;
	static const long ID_WXEDITMAXTEXTFILESIZE;
	static const long ID_WXSTATICTEXT1;
	static const long ID_WXEDITMAXSIZETOLOAD;
	static const long ID_WXCHECKBOXRECORDCARETMOVEMENTS;
	static const long ID_WXCHECKBOXSINGLEINSTANCE;
	static const long ID_WXSTATICTEXT16;
	static const long ID_WXCOMBOBOXLANGUAGE;
	static const long ID_WXNOTEBOOKPAGE1;
	static const long ID_WXNOTEBOOK1;

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
	void OnMouseAutoCopyClicked(wxCommandEvent& event);
    void OnAutoCompletePairClicked( wxCommandEvent& event );
    void OnRadioBoxBracketStyleClick(wxCommandEvent& event);
	void OnFormattingBreakLinesClick(wxCommandEvent& event);
	void OnPaddingBreakBlocksClick(wxCommandEvent& event);
	void OnEnableAutoSaveClick(wxCommandEvent& event);
#ifdef MADEDIT_ENABLE_STC
	void OnMarginClick(wxStyledTextEvent &event);
#endif
};

extern MadOptionsDialog *g_OptionsDialog;

#endif
 
 
 
 
