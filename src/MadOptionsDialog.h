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
#include <wx/spinctrl.h>
#include <wx/aui/aui.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>

#define MADEDIT_ENABLE_MADEDIT
#ifdef MADEDIT_ENABLE_STC
#include <wx/stc/stc.h>
#endif

//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include "MadCommand.h"
#include <list>
using std::list;


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
		wxTextCtrl* WxEditFooterRight;
		wxStaticText* StaticText10;
		wxChoice* WxChoicePointerAlign;
		wxStaticText* StaticText9;
		wxCheckBox* WxCheckBoxPrintTabChar;
		wxCheckBox* WxCheckBoxWhenPressCtrlKey;
		wxStaticText* StaticText20;
		wxCheckBox* WxCheckBoxAutoCompletePair;
		wxTextCtrl* WxEditIndentColumns;
		wxAuiNotebook* AuiNotebook2;
		wxCheckBox* WxCheckForceUseTabs;
		wxPanel* Panel5;
		wxCheckBox* WxCheckBreakLines;
		wxTextCtrl* WxEditMaxColumns;
		wxStaticText* WxStaticTextCommandHint;
		wxCheckBox* WxCheckBoxPrintLineNumber;
		wxTextCtrl* WxEditMaxInStatementIndent;
		wxCheckBox* WxCheckBoxAutoFillColumnPaste;
#ifdef __WXMSW__
		wxCheckBox* WxCheckBoxRightClickMenu;
#endif
		wxCheckBox* WxCheckBoxDoNotSaveSettings;
		wxTextCtrl* WxEditHeaderLeft;
		wxButton* WxButton5;
		wxCheckBox* WxCheckRemoveCommentPrefix;
		wxCheckBox* WxCheckAttachExternC;
		wxButton* WxButtonAddKey;
#ifdef MADEDIT_ENABLE_MADEDIT
		MadEdit *WxTextSample;
#elif MADEDIT_ENABLE_STC
		wxStyledTextCtrl *WxTextSample;
#else
		wxTextCtrl *WxTextSample;
#endif
		wxStaticText* StaticText13;
		wxCheckBox* WxCheckBoxPersonalDict;
		wxListBox* WxListBoxKeys;
		wxStaticText* StaticText2;
		wxPanel* Panel4;
		wxCheckBox* WxCheckPadHeader;
		wxButton* WxButton4;
		wxCheckBox* WxCheckIndentPreprocDefine;
		wxCheckBox* WxCheckAddOneLineBrackets;
		wxStaticText* StaticText14;
		wxButton* WxButton6;
		KeyTextCtrl* WxEditKey;
		wxCheckBox* WxCheckKeepBlocks;
		wxCheckBox* WxCheckIndentPreprocCond;
		wxButton* WxButtonDictionaryDir;
		wxTextCtrl* WxEditDictionaryDir;
		wxCheckBox* WxCheckIndentCase;
		wxCheckBox* WxCheckKeepComplex;
		wxButton* WxButton1;
		wxCheckBox* WxCheckIndentPreprocBlock;
		wxTextCtrl* WxEditHeaderRight;
		wxStaticText* StaticText26;
		wxStaticText* StaticText6;
		wxTextCtrl* WxEditMaxSizeToLoad;
		wxCheckBox* WxCheckBoxPrintPageHeader;
		wxTextCtrl* WxEditKeyHint;
		wxTextCtrl* WxEditMaxTextFileSize;
		wxCheckBox* WxCheckRemoveBrackets;
		wxCheckBox* WxCheckAttachInlines;
		wxPanel* Panel11;
		wxCheckBox* WxCheckBoxPrintPageFooter;
		wxCheckBox* WxCheckIndentCol1Comments;
		wxCheckBox* WxCheckBoxResetAllKeys;
		wxPanel* Panel9;
		wxStaticText* StaticText19;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxStaticText* StaticText18;
		wxButton* WxButtonShowInMenu;
		wxPanel* Panel8;
		wxStaticText* StaticText31;
		wxPanel* Panel1;
		wxCheckBox* WxCheckBoxPrintBookmark;
		wxCheckBox* WxCheckBoxTabOrSpaces;
		wxStaticText* StaticText1;
		wxStaticText* StaticText27;
		wxStaticText* StaticText3;
		wxCheckBox* WxCheckBreakBlocks;
		wxCheckBox* WxCheckIndentSwitches;
		wxTextCtrl* WxEditMaxLineLength;
		wxCheckBox* WxCheckAttachClasses;
		wxTextCtrl* WxEditFooterCenter;
		wxTextCtrl* WxEditXmlIndentSize;
		wxButton* WxButtonDeleteKey;
		wxCheckBox* WxCheckIndentNamespaces;
		wxPanel* Panel6;
		wxCheckBox* WxCheckBoxShowQSearchBar;
		wxPanel* Panel3;
		wxButton* WxButton3;
		wxCheckBox* WxCheckPadOperators;
		wxStaticText* StaticText23;
		wxSpinCtrl* WxSpinIndentation;
		wxStaticText* StaticText24;
		wxCheckBox* WxCheckFillEmptyLines;
		wxComboBox* WxComboBoxEncoding;
		wxCheckBox* WxCheckBoxReloadFiles;
		wxCheckBox* WxCheckCloseTemplates;
		wxButton* WxButtonDateTime;
		wxRadioBox* WxRadioBoxBracketStyle;
		wxCheckBox* WxCheckBoxMiddleMouseToPaste;
		wxCheckBox* WxCheckBreakBlocksAll;
		wxCheckBox* WxCheckBoxRecordCaretMovements;
		wxChoice* WxChoiceReferenceAlign;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxButton* WxButtonCancel;
		wxTextCtrl* WxEditXMLversion;
		wxCheckBox* WxCheckBoxRestoreCaretPos;
		wxCheckBox* WxCheckAttachNamespaces;
		wxPanel* Panel7;
		wxCheckBox* WxCheckBoxSingleInstance;
		wxTextCtrl* WxEditTabColumns;
		wxCheckBox* WxCheckBreakAfterLogical;
		wxTextCtrl* WxEditCommandHint;
		wxCheckBox* WxCheckIndentClasses;
		wxTextCtrl* WxEditHeaderCenter;
		wxCheckBox* WxCheckDelEmptyLine;
		wxTextCtrl* WxEditFooterLeft;
		wxAuiNotebook* AuiNotebook1;
		wxCheckBox* WxCheckPadParensIn;
		wxMenu WxPopupMenuDateTimeMark;
		wxMenu WxPopupMenuPrintMark;
		wxButton* WxButtonOK;
		wxCheckBox* WxCheckUseTab;
		wxStaticText* StaticText28;
		wxCheckBox* WxCheckAddBrackets;
		wxChoice* WxChoiceDictionary;
		wxCheckBox* WxCheckPadParensOut;
		wxCheckBox* WxCheckBoxAutoIndent;
		wxStaticText* StaticText15;
		wxStaticText* StaticText12;
		wxComboBox* WxComboBoxLanguage;
		wxCheckBox* WxCheckBoxPrintEndOfLine;
		wxPanel* Panel2;
		wxCheckBox* WxCheckUnpadParens;
		wxPanel* Panel10;
		wxCheckBox* WxCheckBreakClosing;
		wxButton* WxButton2;
		wxCheckBox* WxCheckIndentModifiers;
		wxCheckBox* WxCheckBoxMouseSelectToCopy;
		wxStaticText* StaticText25;
		wxStaticText* WxStaticTextDateTime;
		wxCheckBox* WxCheckBoxPurgeHistory;
		wxCheckBox* WxCheckBoxPrintSyntax;
		wxTextCtrl* WxEditSFMaxLineLength;
		wxStaticText* StaticText17;
		wxStaticText* StaticText4;
		wxCheckBox* WxCheckBoxPrintSpaceChar;
		wxCheckBox* WxCheckBoxDateTimeInEnglish;
		wxPanel* Panel12;
		wxCheckBox* WxCheckIndentLabels;
		wxRadioBox* WxRadioBoxPrintOffset;
		wxTextCtrl* WxEditDateTime;
		wxCheckBox* WxCheckConvertTabs;
		wxCheckBox* WxCheckBreakElseIfs;
		wxTreeCtrl* WxTreeCtrlCmdList;
		wxStaticText* StaticText16;
		wxSpinCtrl* WxSpinMinConditionalEvent;
		//*)

	protected:
		friend class KeyTextCtrl;
		//(*Identifiers(MadOptionsDialog)
		static const long ID_WXCOMBOBOXLANGUAGE;
		static const long ID_STATICTEXT16;
		static const long ID_WXCHECKBOXSINGLEINSTANCE;
		static const long ID_WXCHECKBOXRELOADFILES;
		static const long ID_WXEDITMAXSIZETOLOAD;
		static const long ID_STATICTEXT1;
		static const long ID_WXEDITMAXTEXTFILESIZE;
		static const long ID_STATICTEXT2;
		static const long ID_WXCOMBOBOXENCODING;
		static const long ID_STATICTEXT13;
		static const long ID_WXCHECKBOXRECORDCARETMOVEMENTS;
		static const long ID_WXCHECKBOXRESTORECARETPOS;
		static const long ID_WXCHECKBOXSHOWQSEARCHBAR;
		static const long ID_WXCHECKBOXDONOTSAVESETTINGS;
		static const long ID_WXCHECKBOXPURGEHISTORY;
		static const long ID_WSCHECKBOXMSRIGHTCCLICKMENU;
		static const long ID_PANEL1;
		static const long ID_WXEDITMAXLINELENGTH;
		static const long ID_STATICTEXT3;
		static const long ID_WXEDITMAXCOLUMNS;
		static const long ID_STATICTEXT4;
		static const long ID_WXEDITTABCOLUMNS;
		static const long ID_STATICTEXT5;
		static const long IID_WXEDITINDENTCOLUMNS;
		static const long ID_STATICTEXT6;
		static const long ID_WXEDITDATETIME;
		static const long ID_WXSTATICTEXTDATETIME;
		static const long ID_WXBUTTONDATETIME;
		static const long ID_WXCHECKBOXDATETIMEINENGLISH;
		static const long ID_WXCHECKBOXTABORSPACES;
		static const long ID_WXCHECKBOXAUTOINDENT;
		static const long ID_WXCHECKBOXAUTOCOMPLETEPAIR;
		static const long ID_WXCHECKBOXMOUSESELECTTOCOPY;
		static const long ID_WXCHECKBOXWHENPRESSCTRLKEY;
		static const long ID_WXCHECKBOXMIDDLEMOUSETOPASTE;
		static const long ID_WXCHECKBOXAUTOFILLCOLUMN;
		static const long ID_PANEL2;
		static const long ID_WXCHECKBOXPRINTSYNTAX;
		static const long ID_WXCHECKBOXPRINTLINENUMBER;
		static const long ID_WXCHECKBOXPRINTBOOKMARK;
		static const long ID_WXCHECKBOXPRINTENDOFLINE;
		static const long ID_WXCHECKBOXPRINTTABCHAR;
		static const long ID_WXCHECKBOXPRINTSPACECHAR;
		static const long ID_WXRADIOBOXPRINTOFFSET;
		static const long ID_WXCHECKBOXPRINTPAGEHEADER;
		static const long ID_STATICTEXT7;
		static const long ID_WXEDITHEADERLEFT;
		static const long ID_WXBUTTON1;
		static const long ID_STATICTEXT8;
		static const long ID_WXEDITHEADERCENTER;
		static const long ID_WXBUTTON2;
		static const long ID_STATICTEXT9;
		static const long ID_WXEDITHEADERRIGHT;
		static const long ID_WXBUTTON3;
		static const long ID_WXCHECKBOXPRINTPAGEFOOTER;
		static const long ID_STATICTEXT10;
		static const long ID_WXEDITFOOTERLEFT;
		static const long ID_WXBUTTON4;
		static const long ID_STATICTEXT11;
		static const long ID_WXEDITFOOTERLEFTCENTER;
		static const long ID_WXBUTTON5;
		static const long ID_STATICTEXT12;
		static const long ID_WXEDITFOOTERRIGHT;
		static const long ID_WXBUTTON6;
		static const long ID_PANEL3;
		static const long ID_WXREECTRLCMDLIST;
		static const long ID_WXSTATICTEXTCOMMANDHINT;
		static const long ID_WXEDITHINT;
		static const long ID_STATICTEXT14;
		static const long ID_WXLISTBOXKEYS;
		static const long ID_STATICTEXT15;
		static const long ID_WXEDITKEY;
		static const long ID_WXEDITKEYHINT;
		static const long ID_WXBUTTONADDKEY;
		static const long ID_WXBUTTONDELETEKEY;
		static const long ID_WXBUTTONSHOWINMENU;
		static const long ID_WXCHECKBOXRESETALLKEYS;
		static const long ID_PANEL4;
		static const long ID_WXCHECKBOXPERSONALDICT;
		static const long ID_STATICTEXT17;
		static const long ID_WXCHOICEDICTIONARY;
		static const long ID_STATICTEXT18;
		static const long ID_WXEDITDICTIONARYDIR;
		static const long ID_WXDICTIONARY_DIR;
		static const long ID_PANEL5;
		static const long ID_WXRADIOBOXBRACKETSTYLE;
		static const long ID_STATICTEXT19;
		static const long ID_WXTEXTSAMPLE;
		static const long ID_PANEL7;
		static const long ID_WXCHECKATTACHCLASSES;
		static const long ID_WXCHECKATTACHEXTERNC;
		static const long ID_WXCHECKATTACHNAMESPACES;
		static const long ID_WXCHECKATTACHINLINES;
		static const long ID_PANEL8;
		static const long ID_STATICTEXT20;
		static const long ID_WXSPINCTRLINDENTSIZE;
		static const long ID_WXCHECKUSETABOVERSPACES;
		static const long ID_WXCHECKFORCETABS;
		static const long ID_WXCHECKINDENTCASE;
		static const long ID_WXCHECKINDENETCLASSES;
		static const long ID_WXCHECKINDENTLABELS;
		static const long ID_WXCHECKINDENTMODIFIERS;
		static const long ID_WXCHECKINDENTNS;
		static const long ID_WXCHECKINDENTSWITCHS;
		static const long ID_WXCHECKINDENTPREP;
		static const long ID_WXCHECKINDENTMULTIPREP;
		static const long ID_WXCHECKINDENTPREPCOND;
		static const long ID_WXCHECKINDENETCPPCOMMENTS;
		static const long ID_STATICTEXT24;
		static const long ID_WXSPINCTRLMININDENT;
		static const long ID_STATICTEXT31;
		static const long ID_WXEDITMAXINSTATEMENTINDENT;
		static const long ID_PANEL9;
		static const long ID_WXCHECKBREAKCLOSING;
		static const long ID_WXCHECKBREAKELSEIFS;
		static const long ID_WXCHECKADDBRACKETS;
		static const long ID_WXCHECKADDONELINEBRACKETS;
		static const long ID_WXCHECKBOXREMOVEBRACKETS;
		static const long ID_WXCHECKKEEPBLOCKS;
		static const long ID_WXCHECKKEEPCOMPLEX;
		static const long ID_WXCHECKCONVERTTABS;
		static const long ID_WXCHECKCLOSETEMPLATES;
		static const long ID_WXCHECKREMOVECOMMENTPREFIX;
		static const long ID_WXCHECKBREAKLINES;
		static const long ID_STATICTEXT23;
		static const long ID_WXEDITSFMAXLINELENGTH;
		static const long ID_WXCHECKBREAKAFTERLOGICAL;
		static const long ID_PANEL10;
		static const long ID_WXCHECKBREAKBLOCKS;
		static const long ID_WXCHECKBREAKBLOCKSALL;
		static const long ID_WXCHECKPADOPERATORS;
		static const long ID_WXCHECKPADPARENSOUT;
		static const long ID_WXCHECKPADPARENSIN;
		static const long ID_WXCHECKPADHEADER;
		static const long ID_WXCHECKUNPADPARENS;
		static const long ID_WXCHECKDELEMPTYLINE;
		static const long ID_WXCHECKFILLEMPTYLINES;
		static const long ID_STATICTEXT26;
		static const long ID_WXCHOICEPOINTERALIGN;
		static const long ID_STATICTEXT27;
		static const long ID_WXCHOICEREFERENCEALIGN;
		static const long ID_PANEL11;
		static const long ID_STATICTEXT28;
		static const long ID_WXEDITXMLVERSION;
		static const long ID_STATICTEXT25;
		static const long ID_WXEDITXMLINDENTSIZE;
		static const long ID_PANEL12;
		static const long ID_AUINOTEBOOK2;
		static const long ID_PANEL6;
		static const long ID_AUINOTEBOOK1;
		static const long ID_WXBUTTONOK;
		static const long ID_WXBUTTONCANCEL;
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
		void WxButtonAddKeyClick(wxCommandEvent& event);
		void WxButtonCancelClick(wxCommandEvent& event);
		void WxButtonDateTimeClick(wxCommandEvent& event);
		void WxButtonDeleteKeyClick(wxCommandEvent& event);
		void WxButtonOKClick(wxCommandEvent& event);
		void WxButtonShowInMenuClick(wxCommandEvent& event);
		void WxListBoxKeysSelected(wxCommandEvent& event);
		void WxButtonCheckNowClick(wxCommandEvent& event);
		void WxCheckBoxMouseSelectToCopyClick(wxCommandEvent& event);
		void WxButtonDictionaryDirClick(wxCommandEvent& event);
		void OnWxEditDictionaryDirText(wxCommandEvent& event);
		void OnSelectDictionary(wxCommandEvent& event);
		void OnWxRadioBoxBracketStyleSelect(wxCommandEvent& event);
		void OnFormattingBreakLinesClick(wxCommandEvent& event);
		void OnPaddingBreakBlocksClick(wxCommandEvent& event);
		void OnMouseAutoCopyClicked(wxCommandEvent& event);
		void WxTreeCtrlCmdListSelectionChanged(wxTreeEvent& event);
		//*)

		void MadOptionsDialogActivate(wxActivateEvent& event);

		void PrintMarkClick(wxCommandEvent& event);
		void PrintMarkButtonClick(wxCommandEvent& event);
		void DateTimeMarkClick(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()

public:

	void LoadOptions(void);

	TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
	bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
	void UpdateKeyHint();
    void InitDictionaryChoice(const wxString &path = wxEmptyString);

	int ButtonID;
	list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
	list<TreeItemData*> ChangedTreeItemDataList;

};


extern MadOptionsDialog *g_OptionsDialog;

#endif
