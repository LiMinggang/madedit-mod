///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "MadOptionsDialog.h"
#include <wx/fileconf.h>
#include <wx/config.h>
#include <wx/dir.h>
#include <wx/aui/auibook.h>

#include "MadEdit/MadEncoding.h"
#include "MadEdit/MadEditCommand.h"
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadUtils.h"
#include "MadEdit/MadEditPv.h"
#include "SpellCheckerManager.h"

#ifdef MADEDIT_ENABLE_STC
enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};
#endif

const long MadOptionsDialog::ID_WXCHECKBOXDCLICKHIGHLIGHT = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXINSERTPAIRFORSELECTION = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT30 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXENABLEAUTOBACKUP = wxNewId();
const long MadOptionsDialog::ID_WXEDITAUTOSAVETIMEOUT = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXENABLEAUTOSAVE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXFIXWIDTHMODE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXSHOWQSEARCHBAR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTBOOKMARK = wxNewId();
const long MadOptionsDialog::ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 = wxNewId();
const long MadOptionsDialog::ID_MNU___D__M__Y_24_02_2007_1192 = wxNewId();
const long MadOptionsDialog::ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 = wxNewId();
const long MadOptionsDialog::ID_MNU_MENUITEM1_1170 = wxNewId();
const long MadOptionsDialog::ID_MNU___A_FULLWEEKDAYNAME_1171 = wxNewId();
const long MadOptionsDialog::ID_MNU___B_ABBREVIATEDMONTHNAME_1172 = wxNewId();
const long MadOptionsDialog::ID_MNU___B_FULLMONTHNAME_1173 = wxNewId();
const long MadOptionsDialog::ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 = wxNewId();
const long MadOptionsDialog::ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 = wxNewId();
const long MadOptionsDialog::ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 = wxNewId();
const long MadOptionsDialog::ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 = wxNewId();
const long MadOptionsDialog::ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 = wxNewId();
const long MadOptionsDialog::ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 = wxNewId();
const long MadOptionsDialog::ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 = wxNewId();
const long MadOptionsDialog::ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 = wxNewId();
const long MadOptionsDialog::ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 = wxNewId();
const long MadOptionsDialog::ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 = wxNewId();
const long MadOptionsDialog::ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 = wxNewId();
const long MadOptionsDialog::ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 = wxNewId();
const long MadOptionsDialog::ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 = wxNewId();
const long MadOptionsDialog::ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 = wxNewId();
const long MadOptionsDialog::ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 = wxNewId();
const long MadOptionsDialog::ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 = wxNewId();
const long MadOptionsDialog::ID_MNU___Z_TIME_ZONENAME_1193 = wxNewId();
const long MadOptionsDialog::ID_MNU___Z_TIME_ZONEABBREVIATION_1194 = wxNewId();

const long MadOptionsDialog::ID_MNU_MENUITEM1_1110 = wxNewId();
const long MadOptionsDialog::ID_MNU___P__PATHNAME_1111 = wxNewId();
const long MadOptionsDialog::ID_MNU_MENUITEM3_1112 = wxNewId();
const long MadOptionsDialog::ID_MNU___N_PAGE_NUMBER_1113 = wxNewId();
const long MadOptionsDialog::ID_MNU___S__TOTALPAGES_1114 = wxNewId();
const long MadOptionsDialog::ID_MNU_MENUITEM6_1115 = wxNewId();
const long MadOptionsDialog::ID_MNU___D__DATE_1116 = wxNewId();
const long MadOptionsDialog::ID_MNU___T__TIME_1117 = wxNewId();

const long MadOptionsDialog::ID_WXBUTTONCANCEL = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONOK = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT21 = wxNewId();
const long MadOptionsDialog::ID_WXBITMAP_DIR = wxNewId();
const long MadOptionsDialog::ID_WXEDITBITMAPDIR = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT20 = wxNewId();
const long MadOptionsDialog::ID_WXTHESAURI_DIR = wxNewId();
const long MadOptionsDialog::ID_WXEDITTHESAURI = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT19 = wxNewId();
const long MadOptionsDialog::ID_WXDICTIONARY_DIR = wxNewId();
const long MadOptionsDialog::ID_WXEDITDICTIONARYDIR = wxNewId();
const long MadOptionsDialog::ID_WXCHOICEDICTIONARY = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT18 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPERSONALDICT = wxNewId();
const long MadOptionsDialog::ID_WXNOTEBOOKPAGE5 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRESETALLKEYS = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONSHOWINMENU = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONDELETEKEY = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONADDKEY = wxNewId();
const long MadOptionsDialog::ID_WXEDITKEYHINT = wxNewId();
const long MadOptionsDialog::ID_WXEDITKEY = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT15 = wxNewId();
const long MadOptionsDialog::ID_WXLISTBOXKEYS = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT14 = wxNewId();
const long MadOptionsDialog::ID_WXEDITHINT = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXTCOMMANDHINT = wxNewId();
const long MadOptionsDialog::ID_WXTREECTRL1 = wxNewId();
const long MadOptionsDialog::ID_WXNOTEBOOKPAGE4 = wxNewId();

const long MadOptionsDialog::ID_WXCHECKBOXPRINTPAGEFOOTER = wxNewId();
const long MadOptionsDialog::ID_WXEDITHEADERRIGHT = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT9 = wxNewId();
const long MadOptionsDialog::ID_WXEDITHEADERCENTER = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT8 = wxNewId();
const long MadOptionsDialog::ID_WXEDITHEADERLEFT = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT7 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTPAGEHEADER = wxNewId();
const long MadOptionsDialog::ID_WXRADIOBOXPRINTOFFSET = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTSPACECHAR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTTABCHAR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTENDOFLINE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTLINENUMBER = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTSYNTAX = wxNewId();
const long MadOptionsDialog::ID_WXNOTEBOOKPAGE3 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXAUTOFILLCOLUMN = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXTYPEWRITERMODE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXMIDDLEMOUSETOPASTE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXCTRLWITHMOUSE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXMOUSESELECTTOCOPY = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXAUTOCOMPLETEPAIR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXAUTOINDENT = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXTABORSPACES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXDATETIMEINENGLISH = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONDATETIME = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXTDATETIME = wxNewId();
const long MadOptionsDialog::ID_WXEDITDATETIME = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT5 = wxNewId();
const long MadOptionsDialog::ID_WXEDITTABCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT4 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT3 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXLINELENGTH = wxNewId();
const long MadOptionsDialog::ID_WXNOTEBOOKPAGE2 = wxNewId();
const long MadOptionsDialog::ID_PURGEHISTORY = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXDONOTSAVESETTINGS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRESTORECARETPOS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRELOADFILES = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT13 = wxNewId();
const long MadOptionsDialog::ID_WXCOMBOBOXENCODING = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT2 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXTEXTFILESIZE = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT1 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXSIZETOLOAD = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRECORDCARETMOVEMENTS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXSINGLEINSTANCE = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT16 = wxNewId();
const long MadOptionsDialog::ID_WXCOMBOBOXLANGUAGE = wxNewId();
const long MadOptionsDialog::ID_WXNOTEBOOKPAGE1 = wxNewId();
const long MadOptionsDialog::ID_WXNOTEBOOK1 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTAFTERPARENS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKATTACHCLOSINGWHILE = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXT33 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON1 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON2 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON3 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON4 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON5 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON6 = wxNewId();

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

MadOptionsDialog *g_OptionsDialog = nullptr;
extern wxArrayString g_LanguageString;
extern wxString g_MadEditAppDir;

TreeItemData *g_SelectedCommandItem = nullptr;
int g_SelectedKeyId = -1;
TreeItemData *g_CommandItemOfNewKey = nullptr;
#define ENABLE_BITMAP_THUARI 0

class KeyTextCtrl : public wxTextCtrl
{
public:
	KeyTextCtrl( wxWindow *parent, wxWindowID id,
				 const wxString& value = wxEmptyString,
				 const wxPoint& pos = wxDefaultPosition,
				 const wxSize& size = wxDefaultSize,
				 long style = 0,
				 const wxValidator& validator = wxDefaultValidator,
				 const wxString& name = wxTextCtrlNameStr )
		: wxTextCtrl( parent, id, value, pos, size, style, validator, name ) {
		Bind( wxEVT_KEY_DOWN, &KeyTextCtrl::OnKeyDown, this);
		Bind( wxEVT_SET_FOCUS, &KeyTextCtrl::OnSetFocus, this);
		Bind( wxEVT_KILL_FOCUS, &KeyTextCtrl::OnKillFocus, this);
	}

	void OnKeyDown( wxKeyEvent& evt ) {
		int flags = wxACCEL_NORMAL;
		int key = evt.GetKeyCode();

		if( evt.ControlDown() ) { flags |= wxACCEL_CTRL; }

		if( evt.AltDown() )     { flags |= wxACCEL_ALT; }

		if( evt.ShiftDown() )   { flags |= wxACCEL_SHIFT; }

		MadEditShortCut sc = ShortCut( flags, key );
		static MadEditShortCut prevsc = 0;

		if( sc == prevsc ) { return; }

		prevsc = sc;
		wxString scstr = ShortCutToString( sc );
		SetValue( scstr );
		g_OptionsDialog->UpdateKeyHint();
		//evt.Skip();
	}
	void OnSetFocus( wxFocusEvent &evt ) { // for getting Ctrl-Tab
		g_OptionsDialog->SetWindowStyleFlag( g_OptionsDialog->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL );
		g_OptionsDialog->WxNotebook1->wxControl::SetWindowStyleFlag( g_OptionsDialog->WxNotebook1->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL );
		g_OptionsDialog->WxNoteBookPage4->SetWindowStyleFlag( g_OptionsDialog->WxNoteBookPage4->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL );
		g_OptionsDialog->WxButtonCancel->SetId( MadOptionsDialog::ID_WXBUTTONCANCEL );
		evt.Skip();
	}
	void OnKillFocus( wxFocusEvent &evt ) { // restore wxTAB_TRAVERSAL
		g_OptionsDialog->SetWindowStyleFlag( g_OptionsDialog->GetWindowStyleFlag() | wxTAB_TRAVERSAL );
		g_OptionsDialog->WxNotebook1->wxControl::SetWindowStyleFlag( g_OptionsDialog->WxNotebook1->wxControl::GetWindowStyleFlag() | wxTAB_TRAVERSAL );
		g_OptionsDialog->WxNoteBookPage4->SetWindowStyleFlag( g_OptionsDialog->WxNoteBookPage4->GetWindowStyleFlag() | wxTAB_TRAVERSAL );
		g_OptionsDialog->WxButtonCancel->SetId( MadOptionsDialog::ID_WXBUTTONCANCEL );
		evt.Skip();
	}
};

MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_menu_evt_map[] = 
{
	{ ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___D__M__Y_24_02_2007_1192, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU_MENUITEM1_1170, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___A_FULLWEEKDAYNAME_1171, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___B_ABBREVIATEDMONTHNAME_1172, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___B_FULLMONTHNAME_1173, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Z_TIME_ZONENAME_1193, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Z_TIME_ZONEABBREVIATION_1194, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU_MENUITEM1_1110, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___P__PATHNAME_1111, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___N_PAGE_NUMBER_1113, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___S__TOTALPAGES_1114, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___D__DATE_1116, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___T__TIME_1117, &MadOptionsDialog::PrintMarkClick },
};

MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_button_evt_map[] = 
{
	{ ID_WXBUTTON1, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_WXBUTTON2, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_WXBUTTON3, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_WXBUTTON4, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_WXBUTTON5, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_WXBUTTON6, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_WXBUTTONCANCEL, &MadOptionsDialog::WxButtonCancelClick },
	{ ID_WXBUTTONOK, &MadOptionsDialog::WxButtonOKClick },
	//  {ID_WXBITMAP_DIR,&MadOptionsDialog::WxButtonBitmapDirClick},
	//  {ID_WXTHESAURI_DIR,&MadOptionsDialog::WxButtonThesauriDirClick},
	{ ID_WXDICTIONARY_DIR, &MadOptionsDialog::WxButtonDictionaryDirClick },
	{ ID_WXBUTTONSHOWINMENU, &MadOptionsDialog::WxButtonShowInMenuClick },
	{ ID_WXBUTTONDELETEKEY, &MadOptionsDialog::WxButtonDeleteKeyClick },
	{ ID_WXBUTTONADDKEY, &MadOptionsDialog::WxButtonAddKeyClick },
	{ ID_WXBUTTONDATETIME, &MadOptionsDialog::WxButtonDateTimeClick },
};

MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_checkbox_evt_map[] =
{
	{ ID_WXCHECKBOXMOUSESELECTTOCOPY, &MadOptionsDialog::OnMouseAutoCopyClicked },

	{ ID_WXCHECKBOXAUTOCOMPLETEPAIR, &MadOptionsDialog::OnAutoCompletePairClicked },
//	{ ID_WXCHECKBREAKLINES, &MadOptionsDialog::OnFormattingBreakLinesClick },
	{ ID_WXCHECKBOXENABLEAUTOSAVE, &MadOptionsDialog::OnEnableAutoSaveClick },
//	{ ID_WXCHECKBREAKBLOCKS, &MadOptionsDialog::OnPaddingBreakBlocksClick },
};

//----------------------------------------------------------------------------
// MadOptionsDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate Block.
// Code added in  other places will be removed by wx-dvcpp

MadOptionsDialog::MadOptionsDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
	: wxDialog( parent, id, title, position, size, style )
{
	ButtonID = 0;
	CreateGUIControls();

	for(size_t i = 0; i < sizeof(m_menu_evt_map)/sizeof(m_menu_evt_map[0]); ++i)
	{
		Bind( wxEVT_MENU, m_menu_evt_map[i].method, this, m_menu_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_button_evt_map)/sizeof(m_button_evt_map[0]); ++i)
	{
		Bind( wxEVT_BUTTON, m_button_evt_map[i].method, this, m_button_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_checkbox_evt_map)/sizeof(m_checkbox_evt_map[0]); ++i)
	{
		Bind( wxEVT_CHECKBOX, m_checkbox_evt_map[i].method, this, m_checkbox_evt_map[i].evtTag );
	}

	Bind( wxEVT_CLOSE_WINDOW, &MadOptionsDialog::MadOptionsDialogClose , this );
	Bind( wxEVT_ACTIVATE, &MadOptionsDialog::MadOptionsDialogActivate , this );

	Bind( wxEVT_TEXT_ENTER, &MadOptionsDialog::OnDictionaryDirChange , this, ID_WXEDITDICTIONARYDIR );
	Bind( wxEVT_CHOICE, &MadOptionsDialog::OnSelectDictionary , this, ID_WXCHOICEDICTIONARY );
	Bind( wxEVT_LISTBOX, &MadOptionsDialog::WxListBoxKeysSelected , this, ID_WXLISTBOXKEYS );

	Bind( wxEVT_TREE_SEL_CHANGED, &MadOptionsDialog::WxTreeCtrl1SelChanged , this, ID_WXTREECTRL1 );
	//Bind( wxEVT_RADIOBOX, &MadOptionsDialog::OnRadioBoxBracketStyleClick , this, ID_WXRADIOBOXBRACKETSTYLE );
}

MadOptionsDialog::~MadOptionsDialog()
{
	delete WxPopupMenuPrintMark;
}

static void ResizeItem( wxSizer* sizer, wxWindow *item, int ax, int ay )
{
	int x, y;

	if( ax != 0 )
	{
		wxString str = item->GetLabel();
		item->GetTextExtent( str, &x, &y );
	}
	else
	{
		item->GetSize( &x, &y );
	}

	item->SetSize( x += ax, y += ay );
	sizer->SetItemMinSize( item, x, y );
}

inline void UpdateSize( wxSize &s1, const wxSize &s2 )
{
	if( s2.x > s1.x ) { s1.x = s2.x; }

	if( s2.y > s1.y ) { s1.y = s2.y; }
}

// filter '&' and '.'
wxString FilterChar( const wxChar *ws )
{
	wxString str;
	wxChar wc;

	while( ( wc = *ws ) != wxT( '\0' ) )
	{
		if( wc != wxT( '&' ) && wc != wxT( '.' ) )
		{
			str << wc;
		}

		++ws;
	}

	return str;
}

#if defined(__WXMSW__) && (wxMAJOR_VERSION >= 3)
#define SET_CONTROLPARENT(pWin) \
    {\
        LONG exStyle = wxGetWindowExStyle((wxWindow *)(pWin));\
        if ( !(exStyle & WS_EX_CONTROLPARENT) )\
        {\
            wxSetWindowExStyle((wxWindow *)(pWin), exStyle | WS_EX_CONTROLPARENT);\
        }\
    }
#else
#define SET_CONTROLPARENT(pWin)
#endif
void MadOptionsDialog::CreateGUIControls( void )
{
	//Do not add custom Code here
	//wx-devcpp designer will remove them.
	//Add the custom code before or after the Blocks
	////GUI Items Creation Start
	WxBoxSizer1 = new wxBoxSizer( wxVERTICAL );
	this->SetSizer( WxBoxSizer1 );
	this->SetAutoLayout( true );
	WxNotebook1 = new wxAuiNotebook( this, ID_WXNOTEBOOK1, wxPoint( 0, 0 ), wxSize( 800, 492 ), wxNB_DEFAULT );
	WxBoxSizer1->Add( WxNotebook1, 1, wxEXPAND | wxALL, 0 );
	WxNoteBookPage1 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE1, wxPoint( 4, 24 ), wxSize( 792, 464 ) );
	WxNotebook1->AddPage( WxNoteBookPage1, _( "General" ) );
	WxBoxSizer3 = new wxBoxSizer( wxVERTICAL );
	WxNoteBookPage1->SetSizer( WxBoxSizer3 );
	WxNoteBookPage1->SetAutoLayout( true );
	WxBoxSizer27 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer3->Add( WxBoxSizer27, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	//wxArrayString arrayStringFor_WxComboBoxLanguage;
	wxASSERT( g_LanguageString.GetCount() != 0 );
	WxComboBoxLanguage = new wxComboBox( WxNoteBookPage1, ID_WXCOMBOBOXLANGUAGE, wxT( "" ), wxPoint( 0, 3 ), wxSize( 140, 21 ), g_LanguageString, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, wxT( "WxComboBoxLanguage" ) );
	WxBoxSizer27->Add( WxComboBoxLanguage, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxComboBoxLanguage );
	WxComboBoxLanguage->SetValue( g_LanguageString[0] );
	WxStaticText16 = new wxStaticText( WxNoteBookPage1, ID_WXSTATICTEXT16, _( "Language of User Interface (must restart MadEdit)" ), wxPoint( 145, 5 ), wxDefaultSize, 0, wxT( "WxStaticText16" ) );
	WxBoxSizer27->Add( WxStaticText16, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer4 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer3->Add( WxBoxSizer4, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxCheckBoxSingleInstance = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXSINGLEINSTANCE, _( "Single Instance (must restart MadEdit)" ), wxPoint( 2, 2 ), wxSize( 300, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxSingleInstance" ) );
	WxBoxSizer4->Add( WxCheckBoxSingleInstance, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxSingleInstance );
	WxCheckBoxRecordCaretMovements = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXRECORDCARETMOVEMENTS, _( "Record caret movements" ), wxPoint( 2, 26 ), wxSize( 300, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxRecordCaretMovements" ) );
	WxBoxSizer4->Add( WxCheckBoxRecordCaretMovements, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxRecordCaretMovements );
	WxBoxSizer5 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer3->Add( WxBoxSizer5, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditMaxSizeToLoad = new wxTextCtrl( WxNoteBookPage1, ID_WXEDITMAXSIZETOLOAD, wxT( "0" ), wxPoint( 0, 3 ), wxSize( 80, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditMaxSizeToLoad" ) );
	WxBoxSizer5->Add( WxEditMaxSizeToLoad, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditMaxSizeToLoad );
	WxStaticText1 = new wxStaticText( WxNoteBookPage1, ID_WXSTATICTEXT1, _( "Max file size to load whole file into memory" ), wxPoint( 85, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText1" ) );
	WxBoxSizer5->Add( WxStaticText1, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer6 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer3->Add( WxBoxSizer6, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditMaxTextFileSize = new wxTextCtrl( WxNoteBookPage1, ID_WXEDITMAXTEXTFILESIZE, wxT( "0" ), wxPoint( 0, 3 ), wxSize( 80, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditMaxTextFileSize" ) );
	WxBoxSizer6->Add( WxEditMaxTextFileSize, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditMaxTextFileSize );
	WxStaticText2 = new wxStaticText( WxNoteBookPage1, ID_WXSTATICTEXT2, _( "Max file size to load as a text file" ), wxPoint( 85, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText2" ) );
	WxBoxSizer6->Add( WxStaticText2, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer54 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer3->Add( WxBoxSizer54, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditMaxDisplaySize = new wxTextCtrl( WxNoteBookPage1, ID_WXEDITMAXTEXTFILESIZE, wxT( "128" ), wxPoint( 0, 3 ), wxSize( 80, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditMaxDisplaySize" ) );
	WxBoxSizer54->Add( WxEditMaxDisplaySize, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditMaxDisplaySize );
	WxStaticText33 = new wxStaticText( WxNoteBookPage1, ID_WXSTATICTEXT33, _( "Max chars displayed in search results" ), wxPoint( 85, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText33" ) );
	WxBoxSizer54->Add( WxStaticText33, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer17 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer3->Add( WxBoxSizer17, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	wxArrayString arrayStringFor_WxComboBoxEncoding;
	WxComboBoxEncoding = new wxComboBox( WxNoteBookPage1, ID_WXCOMBOBOXENCODING, wxT( "" ), wxPoint( 0, 3 ), wxSize( 140, 21 ), arrayStringFor_WxComboBoxEncoding, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, wxT( "WxComboBoxEncoding" ) );
	WxBoxSizer17->Add( WxComboBoxEncoding, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxComboBoxEncoding );
	WxStaticText13 = new wxStaticText( WxNoteBookPage1, ID_WXSTATICTEXT13, _( "Use this encoding to create new file or when MadEdit cannot determine the encoding of old file" ), wxPoint( 145, 5 ), wxDefaultSize, 0, wxT( "WxStaticText13" ) );
	WxBoxSizer17->Add( WxStaticText13, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer7 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer3->Add( WxBoxSizer7, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxCheckBoxReloadFiles = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXRELOADFILES, _( "Reload files previously open on startup" ), wxPoint( 2, 2 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxReloadFiles" ) );
	WxBoxSizer7->Add( WxCheckBoxReloadFiles, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxReloadFiles );
	WxCheckBoxRestoreCaretPos = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXRESTORECARETPOS, _( "Restore previous caret position when files are opened" ), wxPoint( 2, 26 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxRestoreCaretPos" ) );
	WxBoxSizer7->Add( WxCheckBoxRestoreCaretPos, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxRestoreCaretPos );
	WxCheckBoxShowQSearchBar = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXSHOWQSEARCHBAR, _( "Show quick search bar on startup" ), wxPoint( 2, 50 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxShowQSearchBar" ) );
	WxBoxSizer7->Add( WxCheckBoxShowQSearchBar, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxShowQSearchBar );
	WxCheckBoxDoNotSaveSettings = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXDONOTSAVESETTINGS, _( "Do not save settings to MadEdit.cfg when MadEdit closed (this session only)" ), wxPoint( 2, 74 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxDoNotSaveSettings" ) );
	WxBoxSizer7->Add( WxCheckBoxDoNotSaveSettings, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxDoNotSaveSettings );
	WxCheckBoxPurgeHistory = new wxCheckBox( WxNoteBookPage1, ID_PURGEHISTORY, _( "Purge History while exiting" ), wxPoint( 2, 98 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPurgeHistory" ) );
	WxBoxSizer7->Add( WxCheckBoxPurgeHistory, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxPurgeHistory );
	WxCheckBoxEnableAutoSave = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXENABLEAUTOSAVE, _( "Auto save" ), wxPoint( 2, 122 ), wxSize( 120, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxEnableAutoSave" ) );;
	WxBoxSizer7->Add( WxCheckBoxEnableAutoSave, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxEnableAutoSave );
	WxBoxSizer51 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer7->Add( WxBoxSizer51, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxBoxSizer51->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxStaticText30 = new wxStaticText( WxNoteBookPage1, ID_WXSTATICTEXT30, _( "Timeout(M)" ), wxPoint( 125, 2 ), wxSize( 100, 20 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText30" ) );
	WxBoxSizer51->Add( WxStaticText30, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditAutoSaveTimeout = new wxTextCtrl( WxNoteBookPage1, ID_WXEDITMAXSIZETOLOAD, wxT( "10" ), wxPoint( 190, 2 ), wxSize( 50, 17 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditAutoSaveTimeout" ) );
	WxBoxSizer51->Add( WxEditAutoSaveTimeout, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxEditAutoSaveTimeout );
	WxEditAutoSaveTimeout->Enable( false );
	WxCheckBoxEnableAutoBackup = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXENABLEAUTOBACKUP, _( "Auto backup" ), wxPoint( 2, 146 ), wxSize( 120, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxEnableAutoBackup" ) );;
	WxBoxSizer7->Add( WxCheckBoxEnableAutoBackup, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxEnableAutoBackup );
	WxNoteBookPage2 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE2, wxPoint( 4, 24 ), wxSize( 792, 464 ) );
	WxNotebook1->AddPage( WxNoteBookPage2, _( "Edit" ) );
	WxBoxSizer8 = new wxBoxSizer( wxVERTICAL );
	WxNoteBookPage2->SetSizer( WxBoxSizer8 );
	WxNoteBookPage2->SetAutoLayout( true );
	WxBoxSizer9 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer8->Add( WxBoxSizer9, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditMaxLineLength = new wxTextCtrl( WxNoteBookPage2, ID_WXEDITMAXLINELENGTH, wxT( "0" ), wxPoint( 0, 3 ), wxSize( 60, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditMaxLineLength" ) );
	WxBoxSizer9->Add( WxEditMaxLineLength, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditMaxLineLength );
	WxStaticText3 = new wxStaticText( WxNoteBookPage2, ID_WXSTATICTEXT3, _( "Max line length before Line-Wrap (must restart MadEdit)" ), wxPoint( 65, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText3" ) );
	WxBoxSizer9->Add( WxStaticText3, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer10 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer8->Add( WxBoxSizer10, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditMaxColumns = new wxTextCtrl( WxNoteBookPage2, ID_WXEDITMAXCOLUMNS, wxT( "0" ), wxPoint( 0, 3 ), wxSize( 60, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditMaxColumns" ) );
	WxBoxSizer10->Add( WxEditMaxColumns, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditMaxColumns );
	WxStaticText4 = new wxStaticText( WxNoteBookPage2, ID_WXSTATICTEXT4, _( "Columns of Wrap-By-Column" ), wxPoint( 65, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText4" ) );
	WxBoxSizer10->Add( WxStaticText4, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer11 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer8->Add( WxBoxSizer11, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditTabColumns = new wxTextCtrl( WxNoteBookPage2, ID_WXEDITTABCOLUMNS, wxT( "0" ), wxPoint( 0, 3 ), wxSize( 60, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditTabColumns" ) );
	WxBoxSizer11->Add( WxEditTabColumns, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditTabColumns );
	WxStaticText5 = new wxStaticText( WxNoteBookPage2, ID_WXSTATICTEXT5, _( "Columns of Tab" ), wxPoint( 65, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText5" ) );
	WxBoxSizer11->Add( WxStaticText5, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer13 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer8->Add( WxBoxSizer13, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditIndentColumns = new wxTextCtrl( WxNoteBookPage2, ID_WXEDITTABCOLUMNS, wxT( "0" ), wxPoint( 0, 3 ), wxSize( 60, 21 ), 0, wxTextValidator( wxFILTER_NUMERIC ), wxT( "WxEditIndentColumns" ) );
	WxBoxSizer13->Add( WxEditIndentColumns, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditIndentColumns );
	WxStaticText6 = new wxStaticText( WxNoteBookPage2, ID_WXSTATICTEXT5, _( "Columns of Indent" ), wxPoint( 65, 5 ), wxSize( 300, 17 ), wxST_NO_AUTORESIZE, wxT( "WxStaticText6" ) );
	WxBoxSizer13->Add( WxStaticText6, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer23 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer8->Add( WxBoxSizer23, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditDateTime = new wxTextCtrl( WxNoteBookPage2, ID_WXEDITDATETIME, wxT( "" ), wxPoint( 0, 3 ), wxSize( 160, 21 ), 0, wxDefaultValidator, wxT( "WxEditDateTime" ) );
	WxBoxSizer23->Add( WxEditDateTime, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	SET_CONTROLPARENT( WxEditDateTime );
	WxStaticTextDateTime = new wxStaticText( WxNoteBookPage2, ID_WXSTATICTEXTDATETIME, _( "Format of Date/Time" ), wxPoint( 165, 5 ), wxDefaultSize, 0, wxT( "WxStaticTextDateTime" ) );
	WxBoxSizer23->Add( WxStaticTextDateTime, 0, wxALIGN_CENTER | wxALL, 5 );
	WxButtonDateTime = new wxButton( WxNoteBookPage2, ID_WXBUTTONDATETIME, wxT( ">>" ), wxPoint( 273, 1 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButtonDateTime" ) );
	WxBoxSizer23->Add( WxButtonDateTime, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButtonDateTime );
	WxCheckBoxDateTimeInEnglish = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXDATETIMEINENGLISH, _( "Use English instead of current locale" ), wxPoint( 301, 3 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxDateTimeInEnglish" ) );
	WxBoxSizer23->Add( WxCheckBoxDateTimeInEnglish, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxDateTimeInEnglish );
	WxBoxSizer12 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer8->Add( WxBoxSizer12, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxCheckBoxTabOrSpaces = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXTABORSPACES, _( "Insert Space char instead of Tab char" ), wxPoint( 24, 2 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxTabOrSpaces" ) );
	WxBoxSizer12->Add( WxCheckBoxTabOrSpaces, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxTabOrSpaces );
	WxCheckBoxAutoIndent = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXAUTOINDENT, _( "Auto Indent" ), wxPoint( 24, 26 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxAutoIndent" ) );
	WxBoxSizer12->Add( WxCheckBoxAutoIndent, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxAutoIndent );
	WxCheckBoxAutoCompletePair = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXAUTOCOMPLETEPAIR, _( "Auto complete character pair" ), wxPoint( 24, 50 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxAutoCompletePair" ) );
	WxBoxSizer12->Add( WxCheckBoxAutoCompletePair, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxAutoCompletePair );
	WxBoxSizer52 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer52->Add(10,0,0, wxALL, 0);
	WxCheckBoxInsertPairForSelection = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXINSERTPAIRFORSELECTION, _("Insert pair instead of replace selection"), wxPoint( 15, 2 ), wxSize( 260, 20 ), 0, wxDefaultValidator, _T("WxCheckBoxInsertPairForSelection"));
	WxCheckBoxInsertPairForSelection->SetValue(false);
	WxBoxSizer52->Add(WxCheckBoxInsertPairForSelection, 0, wxALL|wxEXPAND, 2);
	SET_CONTROLPARENT( WxCheckBoxInsertPairForSelection );
	WxBoxSizer12->Add(WxBoxSizer52, 0,  wxALIGN_LEFT | wxALL|wxEXPAND, 0);
	WxBoxSizer28 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer12->Add( WxBoxSizer28, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	WxCheckBoxMouseSelectToCopy = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXMOUSESELECTTOCOPY, _( "Auto copy the mouse-selected text to clipboard" ), wxPoint( 2, 2 ), wxSize( 260, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxMouseSelectToCopy" ) );
	WxBoxSizer28->Add( WxCheckBoxMouseSelectToCopy, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxMouseSelectToCopy );
	WxCheckBoxCtrlWithMouseToSelect = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXCTRLWITHMOUSE, _( "when pressing Ctrl key" ), wxPoint( 254, 3 ), wxSize( 160, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxCtrlWithMouseToSelect" ) );
	WxBoxSizer28->Add( WxCheckBoxCtrlWithMouseToSelect, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxCtrlWithMouseToSelect );
	WxCheckBoxMiddleMouseToPaste = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXMIDDLEMOUSETOPASTE, _( "Paste text from clipboard when pressing middle mouse button" ), wxPoint( 24, 98 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxMiddleMouseToPaste" ) );
	WxBoxSizer12->Add( WxCheckBoxMiddleMouseToPaste, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxMiddleMouseToPaste );
	WxCheckBoxAutoFillColumnPaste = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXAUTOFILLCOLUMN, _( "Auto fill in column paste" ), wxPoint( 24, 122 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxAutoFillColumnPaste" ) );
	WxBoxSizer12->Add( WxCheckBoxAutoFillColumnPaste, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxAutoFillColumnPaste );
	WxCheckBoxLDClickHighlight = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXDCLICKHIGHLIGHT, _( "Left double click to highlight" ), wxPoint( 24, 146 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxLDClickHighlight" ) );
	WxBoxSizer12->Add( WxCheckBoxLDClickHighlight, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxLDClickHighlight );
	WxCheckBoxTypewriterMode = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXTYPEWRITERMODE, _( "Typewriter mode" ), wxPoint( 24, 170 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxTypewriterMode" ) );
	WxBoxSizer12->Add( WxCheckBoxTypewriterMode, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxTypewriterMode );
	WxCheckBoxFixWidthMode = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXFIXWIDTHMODE, _( "Fix width mode" ), wxPoint( 24, 194 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxFixWidthMode" ) );
	WxBoxSizer12->Add( WxCheckBoxFixWidthMode, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxFixWidthMode );
	WxNoteBookPage3 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE3, wxPoint( 4, 24 ), wxSize( 792, 464 ) );
	WxNotebook1->AddPage( WxNoteBookPage3, _( "Print" ) );
	WxBoxSizer14 = new wxBoxSizer( wxHORIZONTAL );
	WxNoteBookPage3->SetSizer( WxBoxSizer14 );
	WxNoteBookPage3->SetAutoLayout( true );
	WxBoxSizer15 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer14->Add( WxBoxSizer15, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, _( "Text Mode" ) );
	WxStaticBoxSizer1 = new wxStaticBoxSizer( WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL );
	WxBoxSizer15->Add( WxStaticBoxSizer1, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 4 );
	WxCheckBoxPrintSyntax = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTSYNTAX, _( "Print Syntax Highlighter" ), wxPoint( 6, 16 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintSyntax" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintSyntax, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintSyntax );
	WxCheckBoxPrintLineNumber = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTLINENUMBER, _( "Print Line Number" ), wxPoint( 6, 35 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintLineNumber" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintLineNumber, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintLineNumber );
	WxCheckBoxPrintBookmark = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTBOOKMARK, _( "Print Bookmark" ), wxPoint( 6, 54 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintBookmark" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintBookmark, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintBookmark );
	WxCheckBoxPrintEndOfLine = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTENDOFLINE, _( "Print End of Line" ), wxPoint( 6, 73 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintEndOfLine" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintEndOfLine, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintEndOfLine );
	WxCheckBoxPrintTabChar = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTTABCHAR, _( "Print Tab Char" ), wxPoint( 6, 92 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintTabChar" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintTabChar, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintTabChar );
	WxCheckBoxPrintSpaceChar = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTSPACECHAR, _( "Print Space Char" ), wxPoint( 6, 111 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintSpaceChar" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintSpaceChar, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintSpaceChar );
	wxStaticBox* WxStaticBoxSizer2_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, _( "Hex Mode" ) );
	WxStaticBoxSizer2 = new wxStaticBoxSizer( WxStaticBoxSizer2_StaticBoxObj, wxVERTICAL );
	WxBoxSizer15->Add( WxStaticBoxSizer2, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 4 );
	wxArrayString arrayStringFor_WxRadioBoxPrintOffset;
	arrayStringFor_WxRadioBoxPrintOffset.Add( _( "None" ) );
	arrayStringFor_WxRadioBoxPrintOffset.Add( _( "First Page Only" ) );
	arrayStringFor_WxRadioBoxPrintOffset.Add( _( "Every Page" ) );
	WxRadioBoxPrintOffset = new wxRadioBox( WxNoteBookPage3, ID_WXRADIOBOXPRINTOFFSET, _( "Print Offset Header" ), wxPoint( 6, 16 ), wxDefaultSize, arrayStringFor_WxRadioBoxPrintOffset, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, wxT( "WxRadioBoxPrintOffset" ) );
	WxRadioBoxPrintOffset->InvalidateBestSize();
	WxRadioBoxPrintOffset->SetSelection( 0 );
	WxStaticBoxSizer2->Add( WxRadioBoxPrintOffset, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxRadioBoxPrintOffset );
	WxBoxSizer16 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer14->Add( WxBoxSizer16, 1, wxEXPAND | wxALL, 2 );
	wxStaticBox* WxStaticBoxSizer3_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, _( "Page Header" ) );
	WxStaticBoxSizer3 = new wxStaticBoxSizer( WxStaticBoxSizer3_StaticBoxObj, wxVERTICAL );
	WxBoxSizer16->Add( WxStaticBoxSizer3, 1, wxEXPAND | wxALL, 4 );
	WxCheckBoxPrintPageHeader = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTPAGEHEADER, _( "Print Page Header" ), wxPoint( 86, 16 ), wxSize( 200, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintPageHeader" ) );
	WxStaticBoxSizer3->Add( WxCheckBoxPrintPageHeader, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintPageHeader );
	WxFlexGridSizer1 = new wxFlexGridSizer( 0, 3, 0, 0 );
	WxStaticBoxSizer3->Add( WxFlexGridSizer1, 1, wxEXPAND | wxALL, 2 );
	WxStaticText7 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT7, _( "Left:" ), wxPoint( 8, 5 ), wxDefaultSize, 0, wxT( "WxStaticText7" ) );
	WxFlexGridSizer1->Add( WxStaticText7, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditHeaderLeft = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERLEFT, wxT( "" ), wxPoint( 44, 3 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditHeaderLeft" ) );
	WxFlexGridSizer1->Add( WxEditHeaderLeft, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditHeaderLeft );
	WxButton1 = new wxButton( WxNoteBookPage3, ID_WXBUTTON1, wxT( ">>" ), wxPoint( 287, 1 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButton1" ) );
	WxFlexGridSizer1->Add( WxButton1, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton1 );
	WxStaticText8 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT8, _( "Center:" ), wxPoint( 2, 32 ), wxDefaultSize, 0, wxT( "WxStaticText8" ) );
	WxFlexGridSizer1->Add( WxStaticText8, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditHeaderCenter = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERCENTER, wxT( "" ), wxPoint( 44, 30 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditHeaderCenter" ) );
	WxFlexGridSizer1->Add( WxEditHeaderCenter, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditHeaderCenter );
	WxButton2 = new wxButton( WxNoteBookPage3, ID_WXBUTTON2, wxT( ">>" ), wxPoint( 287, 28 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButton2" ) );
	WxFlexGridSizer1->Add( WxButton2, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton2 );
	WxStaticText9 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT9, _( "Right:" ), wxPoint( 5, 59 ), wxDefaultSize, 0, wxT( "WxStaticText9" ) );
	WxFlexGridSizer1->Add( WxStaticText9, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditHeaderRight = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERRIGHT, wxT( "" ), wxPoint( 44, 57 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditHeaderRight" ) );
	WxFlexGridSizer1->Add( WxEditHeaderRight, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditHeaderRight );
	WxButton3 = new wxButton( WxNoteBookPage3, ID_WXBUTTON3, wxT( ">>" ), wxPoint( 287, 55 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButton3" ) );
	WxFlexGridSizer1->Add( WxButton3, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton3 );
	wxStaticBox* WxStaticBoxSizer4_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, _( "Page Footer" ) );
	WxStaticBoxSizer4 = new wxStaticBoxSizer( WxStaticBoxSizer4_StaticBoxObj, wxVERTICAL );
	WxBoxSizer16->Add( WxStaticBoxSizer4, 1, wxEXPAND | wxALL, 4 );
	WxCheckBoxPrintPageFooter = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTPAGEFOOTER, _( "Print Page Footer" ), wxPoint( 86, 16 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintPageFooter" ) );
	WxStaticBoxSizer4->Add( WxCheckBoxPrintPageFooter, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintPageFooter );
	WxFlexGridSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	WxStaticBoxSizer4->Add( WxFlexGridSizer2, 1, wxEXPAND | wxALL, 2 );
	WxStaticText10 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT7, _( "Left:" ), wxPoint( 8, 5 ), wxDefaultSize, 0, wxT( "WxStaticText10" ) );
	WxFlexGridSizer2->Add( WxStaticText10, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditFooterLeft = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERLEFT, wxT( "" ), wxPoint( 44, 3 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditFooterLeft" ) );
	WxFlexGridSizer2->Add( WxEditFooterLeft, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditFooterLeft );
	WxButton4 = new wxButton( WxNoteBookPage3, ID_WXBUTTON4, wxT( ">>" ), wxPoint( 287, 1 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButton4" ) );
	WxFlexGridSizer2->Add( WxButton4, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton4 );
	WxStaticText11 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT8, _( "Center:" ), wxPoint( 2, 32 ), wxDefaultSize, 0, wxT( "WxStaticText11" ) );
	WxFlexGridSizer2->Add( WxStaticText11, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditFooterCenter = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERCENTER, wxT( "" ), wxPoint( 44, 30 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditFooterCenter" ) );
	WxFlexGridSizer2->Add( WxEditFooterCenter, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditFooterCenter );
	WxButton5 = new wxButton( WxNoteBookPage3, ID_WXBUTTON5, wxT( ">>" ), wxPoint( 287, 28 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButton5" ) );
	WxFlexGridSizer2->Add( WxButton5, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton5 );
	WxStaticText12 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT9, _( "Right:" ), wxPoint( 5, 59 ), wxDefaultSize, 0, wxT( "WxStaticText12" ) );
	WxFlexGridSizer2->Add( WxStaticText12, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditFooterRight = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERRIGHT, wxT( "" ), wxPoint( 44, 57 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditFooterRight" ) );
	WxFlexGridSizer2->Add( WxEditFooterRight, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditFooterRight );
	WxButton6 = new wxButton( WxNoteBookPage3, ID_WXBUTTON6, wxT( ">>" ), wxPoint( 287, 55 ), wxSize( 40, 25 ), 0, wxDefaultValidator, wxT( "WxButton6" ) );
	WxFlexGridSizer2->Add( WxButton6, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton6 );
	WxNoteBookPage4 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE4, wxPoint( 4, 24 ), wxSize( 673, 314 ) );
	WxNotebook1->AddPage( WxNoteBookPage4, _( "Keys" ) );
	WxBoxSizer18 = new wxBoxSizer( wxHORIZONTAL );
	WxNoteBookPage4->SetSizer( WxBoxSizer18 );
	WxNoteBookPage4->SetAutoLayout( true );
	WxBoxSizer19 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer18->Add( WxBoxSizer19, 2, wxEXPAND | wxALL, 4 );
	WxTreeCtrl1 = new wxTreeCtrl( WxNoteBookPage4, ID_WXTREECTRL1, wxPoint( 3, 3 ), wxSize( 250, 240 ), wxTR_HAS_BUTTONS | wxTR_DEFAULT_STYLE, wxDefaultValidator, wxT( "WxTreeCtrl1" ) );
	WxBoxSizer19->Add( WxTreeCtrl1, 1, wxEXPAND | wxALL, 3 );
	SET_CONTROLPARENT( WxTreeCtrl1 );
	WxBoxSizer20 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer18->Add( WxBoxSizer20, 3, wxEXPAND | wxALL, 2 );
	WxBoxSizer24 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer20->Add( WxBoxSizer24, 0, wxEXPAND | wxALL, 2 );
	WxStaticTextCommandHint = new wxStaticText( WxNoteBookPage4, ID_WXSTATICTEXTCOMMANDHINT, _( "Hint:" ), wxPoint( 5, 7 ), wxDefaultSize, 0, wxT( "WxStaticTextCommandHint" ) );
	WxBoxSizer24->Add( WxStaticTextCommandHint, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditCommandHint = new wxTextCtrl( WxNoteBookPage4, ID_WXEDITHINT, wxT( "" ), wxPoint( 41, 5 ), wxSize( 300, 21 ), wxTE_READONLY, wxDefaultValidator, wxT( "WxEditCommandHint" ) );
	WxEditCommandHint->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	WxBoxSizer24->Add( WxEditCommandHint, 1, wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxEditCommandHint );
	WxBoxSizer21 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer20->Add( WxBoxSizer21, 1, wxEXPAND | wxALL, 2 );
	WxBoxSizer22 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer21->Add( WxBoxSizer22, 0, wxEXPAND | wxALL, 2 );
	WxStaticText14 = new wxStaticText( WxNoteBookPage4, ID_WXSTATICTEXT14, _( "Assigned Keys:" ), wxPoint( 34, 5 ), wxDefaultSize, 0, wxT( "WxStaticText14" ) );
	WxBoxSizer22->Add( WxStaticText14, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	wxArrayString arrayStringFor_WxListBoxKeys;
	WxListBoxKeys = new wxListBox( WxNoteBookPage4, ID_WXLISTBOXKEYS, wxPoint( 2, 29 ), wxSize( 140, 200 ), arrayStringFor_WxListBoxKeys, wxLB_SINGLE );
	WxBoxSizer22->Add( WxListBoxKeys, 1, wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxListBoxKeys );
	WxBoxSizer25 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer21->Add( WxBoxSizer25, 5, wxEXPAND | wxALL, 1 );
	WxStaticText15 = new wxStaticText( WxNoteBookPage4, ID_WXSTATICTEXT15, _( "New Key:" ), wxPoint( 70, 5 ), wxDefaultSize, 0, wxT( "WxStaticText15" ) );
	WxBoxSizer25->Add( WxStaticText15, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditKey = new KeyTextCtrl( WxNoteBookPage4, ID_WXEDITKEY, wxT( "" ), wxPoint( 5, 32 ), wxSize( 180, 21 ), wxWANTS_CHARS | wxTE_PROCESS_TAB, wxDefaultValidator, wxT( "WxEditKey" ) );
	WxBoxSizer25->Add( WxEditKey, 0, wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxEditKey );
	WxEditKeyHint = new wxTextCtrl( WxNoteBookPage4, ID_WXEDITKEYHINT, wxT( "" ), wxPoint( 5, 63 ), wxSize( 180, 21 ), wxTE_READONLY, wxDefaultValidator, wxT( "WxEditKeyHint" ) );
	WxEditKeyHint->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	WxBoxSizer25->Add( WxEditKeyHint, 1, wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxEditKeyHint );
	WxButtonAddKey = new wxButton( WxNoteBookPage4, ID_WXBUTTONADDKEY, _( "<== Add Key" ), wxPoint( 15, 94 ), wxSize( 160, 28 ), 0, wxDefaultValidator, wxT( "WxButtonAddKey" ) );
	WxBoxSizer25->Add( WxButtonAddKey, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonAddKey );
	WxButtonDeleteKey = new wxButton( WxNoteBookPage4, ID_WXBUTTONDELETEKEY, _( "==> Delete Key" ), wxPoint( 15, 132 ), wxSize( 160, 28 ), 0, wxDefaultValidator, wxT( "WxButtonDeleteKey" ) );
	WxBoxSizer25->Add( WxButtonDeleteKey, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonDeleteKey );
	WxButtonShowInMenu = new wxButton( WxNoteBookPage4, ID_WXBUTTONSHOWINMENU, _( "Show the Key in Menu" ), wxPoint( 15, 170 ), wxSize( 160, 28 ), 0, wxDefaultValidator, wxT( "WxButtonShowInMenu" ) );
	WxBoxSizer25->Add( WxButtonShowInMenu, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonShowInMenu );
	WxBoxSizer26 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer20->Add( WxBoxSizer26, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxCheckBoxResetAllKeys = new wxCheckBox( WxNoteBookPage4, ID_WXCHECKBOXRESETALLKEYS, _( "Reset all keys to default (must restart MadEdit)" ), wxPoint( 5, 5 ), wxSize( 250, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxResetAllKeys" ) );
	WxBoxSizer26->Add( WxCheckBoxResetAllKeys, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxCheckBoxResetAllKeys );
	WxNoteBookPage5 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE5, wxPoint( 4, 24 ), wxSize( 673, 314 ) );
	WxNotebook1->AddPage( WxNoteBookPage5, _( "SpellChecker" ) );
	WxBoxSizer29 = new wxBoxSizer( wxVERTICAL );
	WxNoteBookPage5->SetSizer( WxBoxSizer29 );
	WxNoteBookPage5->SetAutoLayout( true );
	WxBoxSizer33 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer29->Add( WxBoxSizer33, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxCheckBoxPersonalDict = new wxCheckBox( WxNoteBookPage5, ID_WXCHECKBOXPERSONALDICT, _( "Enable Personal Dictionary" ), wxPoint( 5, 5 ), wxSize( 400, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPersonalDict" ) );
	WxBoxSizer33->Add( WxCheckBoxPersonalDict, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxCheckBoxPersonalDict );
	wxStaticBox* WxStaticBoxSizer5_StaticBoxObj = new wxStaticBox( WxNoteBookPage5, wxID_ANY, _( "Language" ) );
	WxStaticBoxSizer5 = new wxStaticBoxSizer( WxStaticBoxSizer5_StaticBoxObj, wxHORIZONTAL );
	WxBoxSizer29->Add( WxStaticBoxSizer5, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxStaticText18 = new wxStaticText( WxNoteBookPage5, ID_WXSTATICTEXT18, _( "Dictionary:" ), wxPoint( 10, 22 ), wxDefaultSize, 0, wxT( "WxStaticText18" ) );
	WxStaticBoxSizer5->Add( WxStaticText18, 0, wxALIGN_CENTER | wxALL, 5 );
	wxArrayString arrayStringFor_WxChoiceDictionary;
	WxChoiceDictionary = new wxChoice( WxNoteBookPage5, ID_WXCHOICEDICTIONARY, wxPoint( 74, 20 ), wxSize( 320, 30 ), arrayStringFor_WxChoiceDictionary, 0, wxDefaultValidator, wxT( "" ) );
	WxChoiceDictionary->SetSelection( -1 );
	WxStaticBoxSizer5->Add( WxChoiceDictionary, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxChoiceDictionary );
	wxStaticBox* WxStaticBoxSizer6_StaticBoxObj = new wxStaticBox( WxNoteBookPage5, wxID_ANY, _( "Path Setting" ) );
	WxStaticBoxSizer6 = new wxStaticBoxSizer( WxStaticBoxSizer6_StaticBoxObj, wxVERTICAL );
	WxBoxSizer29->Add( WxStaticBoxSizer6, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer30 = new wxBoxSizer( wxHORIZONTAL );
	WxStaticBoxSizer6->Add( WxBoxSizer30, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditDictionaryDir = new wxTextCtrl( WxNoteBookPage5, ID_WXEDITDICTIONARYDIR, wxT( "" ), wxPoint( 5, 8 ), wxSize( 240, 25 ), 0, wxTextValidator( wxFILTER_NONE, nullptr ), wxT( "WxEditDictionaryDir" ) );
	WxBoxSizer30->Add( WxEditDictionaryDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxEditDictionaryDir );
	WxButtonDictionaryDir = new wxButton( WxNoteBookPage5, ID_WXDICTIONARY_DIR, wxT( "..." ), wxPoint( 255, 5 ), wxSize( 75, 25 ), 0, wxDefaultValidator, wxT( "WxButtonDictionaryDir" ) );
	WxBoxSizer30->Add( WxButtonDictionaryDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonDictionaryDir );
	WxStaticText19 = new wxStaticText( WxNoteBookPage5, ID_WXSTATICTEXT19, _( "Dictionary" ), wxPoint( 340, 9 ), wxDefaultSize, 0, wxT( "WxStaticText19" ) );
	WxBoxSizer30->Add( WxStaticText19, 0, wxALIGN_CENTER | wxALL, 5 );
#if ENABLE_BITMAP_THUARI
	WxBoxSizer31 = new wxBoxSizer( wxHORIZONTAL );
	WxStaticBoxSizer6->Add( WxBoxSizer31, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditThesauriDir = new wxTextCtrl( WxNoteBookPage5, ID_WXEDITTHESAURI, wxT( "" ), wxPoint( 5, 8 ), wxSize( 240, 19 ), 0, wxTextValidator( wxFILTER_NONE, nullptr ), wxT( "WxEditThesauriDir" ) );
	WxBoxSizer31->Add( WxEditThesauriDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxEditThesauriDir );
	WxButtonThesauriDir = new wxButton( WxNoteBookPage5, ID_WXTHESAURI_DIR, wxT( "..." ), wxPoint( 255, 5 ), wxSize( 75, 25 ), 0, wxDefaultValidator, wxT( "WxButtonThesauriDir" ) );
	WxBoxSizer31->Add( WxButtonThesauriDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonThesauriDir );
	WxStaticText20 = new wxStaticText( WxNoteBookPage5, ID_WXSTATICTEXT20, _( "Thesauri" ), wxPoint( 340, 9 ), wxDefaultSize, 0, wxT( "WxStaticText20" ) );
	WxBoxSizer31->Add( WxStaticText20, 0, wxALIGN_CENTER | wxALL, 5 );
	WxBoxSizer32 = new wxBoxSizer( wxHORIZONTAL );
	WxStaticBoxSizer6->Add( WxBoxSizer32, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditBitMapDir = new wxTextCtrl( WxNoteBookPage5, ID_WXEDITBITMAPDIR, wxT( "" ), wxPoint( 5, 8 ), wxSize( 240, 19 ), 0, wxTextValidator( wxFILTER_NONE, nullptr ), wxT( "WxEditBitMapDir" ) );
	WxBoxSizer32->Add( WxEditBitMapDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxEditBitMapDir );
	WxButtonBitmapDir = new wxButton( WxNoteBookPage5, ID_WXBITMAP_DIR, wxT( "..." ), wxPoint( 255, 5 ), wxSize( 75, 25 ), 0, wxDefaultValidator, wxT( "WxButtonBitmapDir" ) );
	WxBoxSizer32->Add( WxButtonBitmapDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonBitmapDir );
	WxStaticText21 = new wxStaticText( WxNoteBookPage5, ID_WXSTATICTEXT21, _( "BitMap" ), wxPoint( 340, 9 ), wxDefaultSize, 0, wxT( "WxStaticText21" ) );
	WxBoxSizer32->Add( WxStaticText21, 0, wxALIGN_CENTER | wxALL, 5 );
#endif
	WxBoxSizer2 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer1->Add( WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5 );
	WxButtonOK = new wxButton( this, ID_WXBUTTONOK, _( "&OK" ), wxPoint( 5, 5 ), wxSize( 85, 30 ), 0, wxDefaultValidator, wxT( "WxButtonOK" ) );
	WxBoxSizer2->Add( WxButtonOK, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonOK );
	WxButtonCancel = new wxButton( this, ID_WXBUTTONCANCEL, _( "&Cancel" ), wxPoint( 100, 5 ), wxSize( 90, 30 ), 0, wxDefaultValidator, wxT( "WxButtonCancel" ) );
	WxBoxSizer2->Add( WxButtonCancel, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonCancel );
	WxPopupMenuDateTimeMark = new wxMenu( wxT( "" ) );
	WxPopupMenuDateTimeMark->Append( ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, _( "[%Y/%m/%d %I:%M:%S %p] 2007/02/24 08:30:55 AM" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___D__M__Y_24_02_2007_1192, wxT( "[%d/%m/%Y] 24/02/2007" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, _( "[%a, %d %b %Y %H:%M:%S %z] RFC822 timestamp" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->AppendSeparator();
	WxPopupMenuDateTimeMark->Append( ID_MNU_MENUITEM1_1170, _( "[%a] Abbreviated weekday name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___A_FULLWEEKDAYNAME_1171, _( "[%A] Full weekday name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___B_ABBREVIATEDMONTHNAME_1172, _( "[%b] Abbreviated month name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___B_FULLMONTHNAME_1173, _( "[%B] Full month name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, _( "[%c] Date and time representation appropriate for locale" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, _( "[%d] Day of month as decimal number (01 - 31)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, _( "[%H] Hour in 24-hour format (00 - 23)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, _( "[%I] Hour in 12-hour format (01 - 12)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, _( "[%j] Day of year as decimal number (001 - 366)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, _( "[%m] Month as decimal number (01 - 12)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, _( "[%M] Minute as decimal number (00 - 59)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, _( "[%p] Current locale's A.M./P.M. indicator for 12-hour clock" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, _( "[%S] Second as decimal number (00 - 59)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, _( "[%U] Week of year as decimal number, with Sunday as first day of week (00 - 53)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, _( "[%w] Weekday as decimal number (0 - 6; Sunday is 0)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, _( "[%W] Week of year as decimal number, with Monday as first day of week (00 - 53)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, _( "[%x] Date representation for current locale" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, _( "[%X] Time representation for current locale" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, _( "[%y] Year without century, as decimal number (00 - 99)" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, _( "[%Y] Year with century, as decimal number" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___Z_TIME_ZONENAME_1193, _( "[%z] Time-zone name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuDateTimeMark->Append( ID_MNU___Z_TIME_ZONEABBREVIATION_1194, _( "[%Z] Time-zone abbreviation" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuPrintMark = new wxMenu( wxT( "" ) );
	WxPopupMenuPrintMark->Append( ID_MNU_MENUITEM1_1110, _( "[%f] &File Name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuPrintMark->Append( ID_MNU___P__PATHNAME_1111, _( "[%p] &Path Name" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuPrintMark->AppendSeparator();
	WxPopupMenuPrintMark->Append( ID_MNU___N_PAGE_NUMBER_1113, _( "[%n] Page &Number" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuPrintMark->Append( ID_MNU___S__TOTALPAGES_1114, _( "[%s] Total Page&s" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuPrintMark->AppendSeparator();
	WxPopupMenuPrintMark->Append( ID_MNU___D__DATE_1116, _( "[%d] &Date" ), wxT( "" ), wxITEM_NORMAL );
	WxPopupMenuPrintMark->Append( ID_MNU___T__TIME_1117, _( "[%t] &Time" ), wxT( "" ), wxITEM_NORMAL );
	SetTitle( _( "Options" ) );
	SetIcon( wxNullIcon );
	GetSizer()->Layout();
	GetSizer()->Fit( this );
	////GUI Items Creation End
	wxString systemenc( _( "System Default" ) );
	WxComboBoxEncoding->Append( systemenc );
	size_t cnt = MadEncoding::GetEncodingsCount();
	size_t i;

	for( i = 0; i < cnt; ++i )
	{
		WxComboBoxEncoding->Append( MadEncoding::GetEncodingName( i ) ); //enc+des);
	}

	WxComboBoxEncoding->SetValue( systemenc );
	/*for(i=0; i<g_LanguageString.GetCount(); ++i)
	{
	    WxComboBoxLanguage->Append(g_LanguageString[i]);
	}
	WxComboBoxLanguage->SetValue(g_LanguageString[0]);*/
	ResizeItem( WxBoxSizer27, WxStaticText16, 2, 2 );
	ResizeItem( WxBoxSizer4, WxCheckBoxSingleInstance, 25, 4 );
	ResizeItem( WxBoxSizer4, WxCheckBoxRecordCaretMovements, 25, 4 );
	ResizeItem( WxBoxSizer5, WxStaticText1, 2, 2 );
	ResizeItem( WxBoxSizer6, WxStaticText2, 2, 2 );
	ResizeItem( WxBoxSizer7, WxCheckBoxDoNotSaveSettings, 25, 4 );
	ResizeItem( WxBoxSizer7, WxCheckBoxReloadFiles, 25, 4 );
	ResizeItem( WxBoxSizer7, WxCheckBoxRestoreCaretPos, 25, 4 );
	ResizeItem( WxBoxSizer7, WxCheckBoxShowQSearchBar, 25, 4 );
	ResizeItem( WxBoxSizer7, WxCheckBoxPurgeHistory, 25, 4 );
	ResizeItem( WxBoxSizer17, WxStaticText13, 2, 2 );
	ResizeItem( WxBoxSizer9, WxStaticText3, 2, 2 );
	ResizeItem( WxBoxSizer10, WxStaticText4, 2, 2 );
	ResizeItem( WxBoxSizer11, WxStaticText5, 2, 2 );
	ResizeItem( WxBoxSizer13, WxStaticText6, 2, 2 );
	ResizeItem( WxBoxSizer12, WxCheckBoxTabOrSpaces, 25, 4 );
	ResizeItem( WxBoxSizer12, WxCheckBoxAutoIndent, 25, 4 );
	ResizeItem( WxBoxSizer12, WxCheckBoxAutoCompletePair, 25, 4 );
	ResizeItem( WxBoxSizer28, WxCheckBoxMouseSelectToCopy, 25, 4 );
	ResizeItem( WxBoxSizer28, WxCheckBoxCtrlWithMouseToSelect, 25, 4 );
	ResizeItem( WxBoxSizer12, WxCheckBoxMiddleMouseToPaste, 25, 4 );
	ResizeItem( WxBoxSizer23, WxStaticTextDateTime, 2, 2 );
	ResizeItem( WxBoxSizer23, WxCheckBoxDateTimeInEnglish, 25, 4 );
	ResizeItem( WxStaticBoxSizer1, WxCheckBoxPrintSyntax, 25, 4 );
	ResizeItem( WxStaticBoxSizer1, WxCheckBoxPrintLineNumber, 25, 4 );
	ResizeItem( WxStaticBoxSizer1, WxCheckBoxPrintEndOfLine, 25, 4 );
	ResizeItem( WxStaticBoxSizer1, WxCheckBoxPrintTabChar, 25, 4 );
	ResizeItem( WxStaticBoxSizer1, WxCheckBoxPrintSpaceChar, 25, 4 );
	int strx = 0, stry = 0;
	wxString str = WxRadioBoxPrintOffset->GetLabel();
	WxRadioBoxPrintOffset->GetTextExtent( str, &strx, &stry );

	for( i = 0; i < WxRadioBoxPrintOffset->GetCount(); ++i )
	{
		int x;
		str = WxRadioBoxPrintOffset->GetString( ( unsigned int )i );
		WxRadioBoxPrintOffset->GetTextExtent( str, &x, &stry );

		if( x > strx ) { strx = x; }
	}

	stry = ( stry * ( ( int )WxRadioBoxPrintOffset->GetCount() + 2 ) ) * 4 / 3;
	WxRadioBoxPrintOffset->SetSize( strx += 25, stry );
	WxStaticBoxSizer2->SetItemMinSize( WxRadioBoxPrintOffset, strx, stry );
	ResizeItem( WxStaticBoxSizer3, WxCheckBoxPrintPageHeader, 25, 4 );
	ResizeItem( WxStaticBoxSizer3, WxStaticText7, 2, 2 );
	ResizeItem( WxStaticBoxSizer3, WxStaticText8, 2, 2 );
	ResizeItem( WxStaticBoxSizer3, WxStaticText9, 2, 2 );
	ResizeItem( WxStaticBoxSizer3, WxEditHeaderLeft, 0, 2 );
	ResizeItem( WxStaticBoxSizer3, WxEditHeaderCenter, 0, 2 );
	ResizeItem( WxStaticBoxSizer3, WxEditHeaderRight, 0, 2 );
	ResizeItem( WxStaticBoxSizer4, WxCheckBoxPrintPageFooter, 25, 4 );
	ResizeItem( WxStaticBoxSizer4, WxStaticText10, 2, 2 );
	ResizeItem( WxStaticBoxSizer4, WxStaticText11, 2, 2 );
	ResizeItem( WxStaticBoxSizer4, WxStaticText12, 2, 2 );
	ResizeItem( WxStaticBoxSizer4, WxEditFooterLeft, 0, 2 );
	ResizeItem( WxStaticBoxSizer4, WxEditFooterCenter, 0, 2 );
	ResizeItem( WxStaticBoxSizer4, WxEditFooterRight, 0, 2 );
	ResizeItem( WxBoxSizer22, WxStaticText14, 2, 2 );
	ResizeItem( WxBoxSizer25, WxStaticText15, 2, 2 );
	ResizeItem( WxBoxSizer24, WxStaticTextCommandHint, 2, 2 );
	ResizeItem( WxBoxSizer26, WxCheckBoxResetAllKeys, 25, 4 );
#ifdef __WXMSW__
	WxCheckBoxRightClickMenu = new wxCheckBox( WxNoteBookPage1, -1, _( "Add MadEdit to the RightClickMenu of Explorer(Deselect to Remove the Entry from Windows Registry)" ), wxPoint( 5, 5 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxRightClickMenu" ) );
	WxBoxSizer7->Add( WxCheckBoxRightClickMenu, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	ResizeItem( WxBoxSizer7, WxCheckBoxRightClickMenu, 25, 4 );
	SET_CONTROLPARENT( WxCheckBoxRightClickMenu );
#endif
	wxSize size = WxBoxSizer1->GetMinSize();
	wxSize size1 = WxBoxSizer3->GetMinSize();
	wxSize size2 = WxBoxSizer8->GetMinSize();
	wxSize size3 = WxBoxSizer14->GetMinSize();
	wxSize size4 = WxBoxSizer18->GetMinSize();
	UpdateSize( size, size1 );
	UpdateSize( size, size2 );
	UpdateSize( size, size3 );
	UpdateSize( size, size4 );
	wxSize size5 = WxBoxSizer2->GetMinSize();
	size.y += ( size5.y + 30 );
	this->SetClientSize( size );
	Center();
	// build Command Tree
	WxTreeCtrl1->SetImageList( g_MainFrame->m_ImageList );
	wxTreeItemId root = WxTreeCtrl1->AddRoot( _( "Commands" ) );
	wxTreeItemId menuRoot = WxTreeCtrl1->AppendItem( root, _( "Menu" ) );
	wxTreeItemId editorRoot = WxTreeCtrl1->AppendItem( root, _( "Editor" ) );
	list<wxTreeItemId> tree_stack;
	CommandData *cd = &CommandTable[0];
	wxTreeItemId tid;

	// Menu command
	do
	{
		if( cd->menu_level == 0 )
		{
			tid = WxTreeCtrl1->AppendItem( menuRoot, FilterChar( wxGetTranslation( cd->text ) ) );
			tree_stack.clear();
			tree_stack.push_back( tid );
			++cd;
			continue;
		}
		else
			if( cd->menu_level < int( tree_stack.size() ) )
			{
				do
				{
					tree_stack.pop_back();
				}
				while( cd->menu_level < int( tree_stack.size() ) );
			}

		if( cd->menu_ptr != 0 )
		{
			tid = WxTreeCtrl1->AppendItem( tree_stack.back(), FilterChar( wxGetTranslation( cd->text ) ) );
			tree_stack.push_back( tid );
		}
		else
			if( cd->kind != wxITEM_SEPARATOR )
			{
				TreeItemData *data = new TreeItemData;
				data->cmddata = cd;
				TreeItemDataList.push_back( data );
				tid = WxTreeCtrl1->AppendItem( tree_stack.back(), FilterChar( wxGetTranslation( cd->text ) ), cd->image_idx, cd->image_idx, data );
				WxTreeCtrl1->SetItemBold( tid, true );
			}

		++cd;
	}
	while( cd->menu_level >= 0 );

	// Editor command
	do
	{
		TreeItemData *data = new TreeItemData;
		data->cmddata = cd;
		TreeItemDataList.push_back( data );
		tid = WxTreeCtrl1->AppendItem( editorRoot, MadKeyBindings::CommandToText( cd->command ), cd->image_idx, cd->image_idx, data );
		WxTreeCtrl1->SetItemBold( tid, true );
		cd++;
	}
	while( cd->command > 0 );

	WxTreeCtrl1->Expand(root);
	WxTreeCtrl1->Expand(menuRoot);

	WxNotebook1->SetWindowStyleFlag( wxAUI_NB_TOP | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );
	WxNotebook1->SetSelection( 0 );
	WxButtonCancel->SetId( wxID_CANCEL );
	SetDefaultItem( WxButtonOK );
	WxButtonOK->SetFocus();
}

void MadOptionsDialog::MadOptionsDialogClose( wxCloseEvent& event )
{
	// --> Don't use Close with a wxDialog,
	// use Destroy instead.
	if( event.CanVeto() )
	{
		event.Veto();
		Show( false );
		return;
	}

	g_OptionsDialog = nullptr;
	Destroy();
}


/*
 * MadOptionsDialogActivate
 */
void MadOptionsDialog::MadOptionsDialogActivate( wxActivateEvent& event )
{
	if( event.GetActive() )
	{
		if( FindFocus() == nullptr )
		{
			SetReturnCode( wxID_CANCEL );
			WxButtonCancel->SetFocus();
		}
	}

	event.Skip();
}

void MadOptionsDialog::LoadOptions( void )
{
	wxConfigBase *cfg = wxConfigBase::Get( false );
	wxString oldpath = cfg->GetPath();
	cfg->SetPath( wxT( "/MadEdit" ) );
	long ll;
	bool bb;
	wxString ss;
	// General page
	ss = g_LanguageString[0];
	cfg->Read( wxT( "Language" ), &ss );
	WxComboBoxLanguage->SetValue( ss );
	cfg->Read( wxT( "SingleInstance" ), &bb, false );
	WxCheckBoxSingleInstance->SetValue( bb );
	cfg->Read( wxT( "RecordCaretMovements" ), &bb, true );
	WxCheckBoxRecordCaretMovements->SetValue( bb );
	cfg->Read( wxT( "MaxSizeToLoad" ), &ll );
	WxEditMaxSizeToLoad->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxTextFileSize" ), &ll );
	WxEditMaxTextFileSize->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxDisplaySize" ), &ll );
	WxEditMaxDisplaySize->SetValue( wxString() << ll );
	ss = _( "System Default" );
	cfg->Read( wxT( "DefaultEncoding" ), &ss );
	WxComboBoxEncoding->SetValue( ss );
	cfg->Read( wxT( "AutoSaveTimeout" ), &ll, 0 );
	bb = ((ll >= 10) && (ll <= 30));
	WxCheckBoxEnableAutoSave->SetValue( bb );
	if(!bb) ll = 10;
	WxEditAutoSaveTimeout->Enable(bb);
	WxEditAutoSaveTimeout->SetValue( wxString() << ll );
	cfg->Read( wxT( "AutoBackup" ), &bb, false );
	WxCheckBoxEnableAutoBackup->SetValue( bb );
#ifdef __WXMSW__
	wxRegKey *pRegKey = new wxRegKey( g_MadEditRegkeyPath + wxT( "*\\shell\\MadEdit-Mod\\command" ) );

	if( pRegKey->Exists() )
	{
		wxString str;

		if( pRegKey->QueryValue( wxEmptyString, str ) )
		{
			wxString exepath = GetExecutablePath();
			WxCheckBoxRightClickMenu->SetValue( str.Upper().Find( exepath.Upper() ) >= 0 );
		}
	}

	delete pRegKey;
#endif
	// Edit page
	cfg->Read( wxT( "MaxLineLength" ), &ll );
	WxEditMaxLineLength->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxColumns" ), &ll );
	WxEditMaxColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "TabColumns" ), &ll );
	WxEditTabColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "IndentColumns" ), &ll );
	WxEditIndentColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "DateTimeFormat" ), &ss, wxT( "%c" ) );
	WxEditDateTime->SetValue( ss );
	cfg->Read( wxT( "DateTimeInEnglish" ), &bb, false );
	WxCheckBoxDateTimeInEnglish->SetValue( bb );
	cfg->Read( wxT( "InsertSpacesInsteadOfTab" ), &bb, false );
	WxCheckBoxTabOrSpaces->SetValue( bb );
	cfg->Read( wxT( "AutoIndent" ), &bb, false );
	WxCheckBoxAutoIndent->SetValue( bb );
	cfg->Read( wxT( "AutoCompletePair" ), &bb, false );
	WxCheckBoxAutoCompletePair->SetValue( bb );
	WxCheckBoxInsertPairForSelection->Enable(bb);
	cfg->Read( wxT( "InsertPairForSelction" ), &bb, true );
	WxCheckBoxInsertPairForSelection->SetValue( bb );
	cfg->Read( wxT( "MouseSelectToCopy" ), &bb, false );
	WxCheckBoxMouseSelectToCopy->SetValue( bb );
	WxCheckBoxCtrlWithMouseToSelect->Enable( bb );
	cfg->Read( wxT( "MouseSelectToCopyWithCtrlKey" ), &bb, true );
	WxCheckBoxCtrlWithMouseToSelect->SetValue( bb );
	cfg->Read( wxT( "MiddleMouseToPaste" ), &bb, false );
	WxCheckBoxMiddleMouseToPaste->SetValue( bb );
	cfg->Read( wxT( "AutoFillColumnPaste" ), &bb, true );
	WxCheckBoxAutoFillColumnPaste->SetValue( bb );
	cfg->Read( wxT( "LDoubleClickHighlight" ), &bb, true );
	WxCheckBoxLDClickHighlight->SetValue( bb );
	cfg->Read( wxT( "TypewriterMode" ), &bb, false );
	WxCheckBoxTypewriterMode->SetValue( bb );
	cfg->Read( wxT( "FixedWidthMode" ), &bb, false );
	WxCheckBoxFixWidthMode->SetValue( bb );
	extern bool g_DoNotSaveSettings;
	WxCheckBoxDoNotSaveSettings->SetValue( g_DoNotSaveSettings );
	cfg->Read( wxT( "ReloadFiles" ), &bb, true );
	WxCheckBoxReloadFiles->SetValue( bb );
	cfg->Read( wxT( "PurgeHistory" ), &bb, false );
	WxCheckBoxPurgeHistory->SetValue( bb );
	cfg->Read( wxT( "ShowQSearchBarOnStart" ), &bb, true );
	WxCheckBoxShowQSearchBar->SetValue( bb );
	cfg->Read( wxT( "RestoreCaretPos" ), &bb, true );
	WxCheckBoxRestoreCaretPos->SetValue( bb );
	// Print page
	cfg->Read( wxT( "PrintSyntax" ), &bb );
	WxCheckBoxPrintSyntax->SetValue( bb );
	cfg->Read( wxT( "PrintLineNumber" ), &bb );
	WxCheckBoxPrintLineNumber->SetValue( bb );
	cfg->Read( wxT( "PrintBookmark" ), &bb );
	WxCheckBoxPrintBookmark->SetValue( bb );
	cfg->Read( wxT( "PrintEndOfLine" ), &bb );
	WxCheckBoxPrintEndOfLine->SetValue( bb );
	cfg->Read( wxT( "PrintTabChar" ), &bb );
	WxCheckBoxPrintTabChar->SetValue( bb );
	cfg->Read( wxT( "PrintSpaceChar" ), &bb );
	WxCheckBoxPrintSpaceChar->SetValue( bb );
	cfg->Read( wxT( "PrintOffsetHeader" ), &ll );
	WxRadioBoxPrintOffset->SetSelection( ll );
	cfg->Read( wxT( "PrintPageHeader" ), &bb );
	WxCheckBoxPrintPageHeader->SetValue( bb );
	cfg->Read( wxT( "PageHeaderLeft" ), &ss );
	WxEditHeaderLeft->SetValue( ss );
	cfg->Read( wxT( "PageHeaderCenter" ), &ss );
	WxEditHeaderCenter->SetValue( ss );
	cfg->Read( wxT( "PageHeaderRight" ), &ss );
	WxEditHeaderRight->SetValue( ss );
	cfg->Read( wxT( "PrintPageFooter" ), &bb );
	WxCheckBoxPrintPageFooter->SetValue( bb );
	cfg->Read( wxT( "PageFooterLeft" ), &ss );
	WxEditFooterLeft->SetValue( ss );
	cfg->Read( wxT( "PageFooterCenter" ), &ss );
	WxEditFooterCenter->SetValue( ss );
	cfg->Read( wxT( "PageFooterRight" ), &ss );
	WxEditFooterRight->SetValue( ss );
	bb = SpellCheckerManager::Instance().GetEnablePersonalDictionary();
	WxCheckBoxPersonalDict->SetValue( bb );
	WxEditDictionaryDir->SetValue( SpellCheckerManager::Instance().GetDictionaryPath() );
	InitDictionaryChoice();
	//cfg->Read(wxT("ThesPath"), &ss, dictDir);
	//WxEditThesauriDir->SetValue(ss);
	//cfg->Read(wxT("BitmPath"), &ss, dictDir);
	//WxEditBitMapDir->SetValue(ss);
	extern bool g_ResetAllKeys;
	WxCheckBoxResetAllKeys->SetValue( g_ResetAllKeys );
	ChangedTreeItemDataList.clear();
	// get all keys of each command and srore them in TreeItemDataList
	list<TreeItemData*>::iterator tidit = TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend = TreeItemDataList.end();

	do
	{
		CommandData *cd = ( *tidit )->cmddata;
		( *tidit )->keys.Clear();
		MadEdit::ms_KeyBindings.GetKeys( cd->menu_id, cd->command, ( *tidit )->keys );
	}
	while( ++tidit != tiditend );

	g_SelectedCommandItem = nullptr;
	g_SelectedKeyId = -1;
	WxEditCommandHint->Clear();
	WxListBoxKeys->Clear();
	UpdateKeyHint();
	wxTreeItemId selid = WxTreeCtrl1->GetSelection();

	if( selid.IsOk() )
	{
		WxTreeCtrl1->Unselect();
		WxTreeCtrl1->SelectItem( selid, true );
	}
#if 0
	cfg->SetPath( wxT( "/astyle" ) );
	WxRadioBoxBracketStyle->SetSelection( cfg->ReadLong( wxT( "style" ), aspsAllman ) );
	WxCheckAttachClasses->SetValue( cfg->ReadBool( wxT( "attach_classes" ), false ) );
	WxCheckAttachExternC->SetValue( cfg->ReadBool( wxT( "attach_extern_c" ), true ) );
	WxCheckAttachNamespaces->SetValue( cfg->ReadBool( wxT( "attach_namespaces" ), true ) );
	WxCheckAttachInlines->SetValue( cfg->ReadBool( wxT( "attach_inlines" ), true ) );
	WxCheckAttachClosingWhile->SetValue( cfg->ReadBool( wxT( "attach_closing_while" ), false ) );
	WxCheckForceUseTabs->SetValue( cfg->ReadBool( wxT( "force_tabs" ), false ) );
	WxSpinIndentation->SetValue( cfg->ReadLong( wxT( "indentation" ), 4 ) );
	WxCheckUseTab->SetValue( cfg->ReadBool( wxT( "use_tabs" ), false ) );
	WxCheckIndentCase->SetValue( cfg->ReadBool( wxT( "indent_case" ), true ) );
	WxCheckIndentClasses->SetValue( cfg->ReadBool( wxT( "indent_classes" ), false ) );
	WxCheckIndentLabels->SetValue( cfg->ReadBool( wxT( "indent_labels" ), false ) );
	WxCheckIndentModifiers->SetValue( cfg->ReadBool( wxT( "indent_modifiers" ), false ) );
	WxCheckIndentNamespaces->SetValue( cfg->ReadBool( wxT( "indent_namespaces" ), true ) );
	WxCheckIndentSwitches->SetValue( cfg->ReadBool( wxT( "indent_switches" ), false ) );
	WxCheckIndentPreprocBlock->SetValue( cfg->ReadBool( wxT( "indent_preproc_block" ), true ) );
	WxCheckIndentPreprocDefine->SetValue( cfg->ReadBool( wxT( "indent_preproc_define" ), false ) );
	WxCheckIndentPreprocCond->SetValue( cfg->ReadBool( wxT( "indent_preproc_cond" ), false ) );
	WxCheckIndentCol1Comments->SetValue( cfg->ReadBool( wxT( "indent_col1_comments" ), true ) );
	WxCheckIndentAfterParens->SetValue( cfg->ReadBool( wxT( "indent_after_parens" ), false ) );
	WxSpinMinConditionalEvent->SetValue( cfg->ReadLong( wxT( "min_conditional_indent" ), 2 ) );
	WxEditMaxInStatementIndent->SetValue( wxString() << cfg->ReadLong( wxT( "max_continuation_indent" ), 40 ) );
	WxEditIndentContinuation->SetValue( wxString() << cfg->ReadLong( wxT( "indent_continuation" ), 1 ) );
	WxCheckBreakClosing->SetValue( cfg->ReadBool( wxT( "break_closing" ), true ) );
	WxCheckBreakElseIfs->SetValue( cfg->ReadBool( wxT( "break_elseifs" ), true ) );
	WxCheckAddBraces->SetValue( cfg->ReadBool( wxT( "add_brackets" ), false ) );
	WxCheckAddOneLineBraces->SetValue( cfg->ReadBool( wxT( "add_one_line_brackets" ), true ) );
	WxCheckRemoveBraces->SetValue( cfg->ReadBool( wxT( "remove_brackets" ), false ) );
	WxCheckKeepBlocks->SetValue( cfg->ReadBool( wxT( "keep_blocks" ), true ) );
	WxCheckConvertTabs->SetValue( cfg->ReadBool( wxT( "convert_tabs" ), true ) );
	WxCheckCloseTemplates->SetValue( cfg->ReadBool( wxT( "close_templates" ), false ) );
	WxCheckRemoveCommentPrefix->SetValue( cfg->ReadBool( wxT( "remove_comment_prefix" ), false ) );
	bb = cfg->ReadBool( wxT( "break_lines" ), false );
	WxCheckBreakLines->SetValue( bb );

	if( bb )
	{
		WxEditSFMaxLineLength->Enable( true );
		WxCheckBreakAfterLogical->Enable( true );
		WxEditSFMaxLineLength->SetValue( cfg->Read( wxT( "max_line_length" ), wxString( wxT( "200" ) ) ) );
		WxCheckBreakAfterLogical->SetValue( cfg->ReadBool( wxT( "break_after_mode" ), false ) );
	}
	else
	{
		WxEditSFMaxLineLength->Enable( false );
		WxCheckBreakAfterLogical->Enable( false );
	}

	WxCheckBreakBlocks->SetValue( cfg->ReadBool( wxT( "break_blocks" ), true ) );

	if( WxCheckBreakBlocks->GetValue() )
	{
		WxCheckBreakBlocksAll->Enable( true );
		WxCheckBreakBlocksAll->SetValue( cfg->ReadBool( wxT( "break_blocks_all" ), false ) );
	}
	else
	{
		WxCheckBreakBlocksAll->Enable( false );
	}

	WxCheckPadOperators->SetValue( cfg->ReadBool( wxT( "pad_operators" ), true ) );
	WxCheckPadParensOut->SetValue( cfg->ReadBool( wxT( "pad_parentheses_out" ), false ) );
	WxCheckPadParensIn->SetValue( cfg->ReadBool( wxT( "pad_parentheses_in" ), true ) );
	WxCheckPadHeader->SetValue( cfg->ReadBool( wxT( "pad_header" ), false ) );
	WxCheckUnpadParens ->SetValue( cfg->ReadBool( wxT( "unpad_parentheses" ), true ) );
	WxCheckDelEmptyLine->SetValue( cfg->ReadBool( wxT( "delete_empty_lines" ), true ) );
	WxCheckFillEmptyLines->SetValue( cfg->ReadBool( wxT( "fill_empty_lines" ), false ) );
	wxString pointerAlign = cfg->Read( wxT( "pointer_align" ), wxEmptyString );

	if( pointerAlign == wxT( "Type" ) )
	{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_TYPE ); }
	else
		if( pointerAlign == wxT( "Middle" ) )
		{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_MIDDLE ); }
		else
			if( pointerAlign == wxT( "Name" ) )
			{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_NAME ); }
			else
			{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_NONE ); }

	wxString referenceAlign = cfg->Read( wxT( "reference_align" ), wxEmptyString );

	if( referenceAlign == wxT( "Type" ) )
	{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_TYPE ); }
	else
		if( referenceAlign == wxT( "Middle" ) )
		{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_MIDDLE ); }
		else
			if( referenceAlign == wxT( "Name" ) )
			{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_NAME ); }
			else
			{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_NONE ); }

	cfg->SetPath( wxT( "/xml" ) );
	WxEditXmlIndentSize->SetValue( wxString() << cfg->ReadLong( wxT( "indentation" ), 4 ) );
	WxEditXMLversion->SetValue( cfg->Read( wxT( "version" ), wxString( wxT( "1.0" ) ) ) );
#endif
	cfg->SetPath( oldpath );
}

/*
 * WxButtonOKClick
 */
void MadOptionsDialog::WxButtonOKClick( wxCommandEvent& event )
{
	long lo;
	bool error = false;
	wxString errtext( _( "Invalid value of \"%s(%s)\"" ) );

	if( !WxEditMaxSizeToLoad->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext + _(": Should be greater than zero"), WxStaticText1->GetLabel().c_str(), WxEditMaxSizeToLoad->GetValue().c_str() );
		error = true;
	}

	if( !WxEditMaxTextFileSize->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext + _(": Should be greater than zero"), WxStaticText2->GetLabel().c_str(), WxEditMaxTextFileSize->GetValue().c_str() );
		error = true;
	}

#if PATCH_MAXLINELENGTH == 1
	if( !WxEditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 || lo > 4096 )
#else
	if( !WxEditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 )
#endif
	{
		wxLogError( errtext + _(": Should be 1~4096"), WxStaticText3->GetLabel().c_str(), WxEditMaxLineLength->GetValue().c_str() );
		error = true;
	}

	if( !WxEditMaxColumns->GetValue().ToLong( &lo ) || lo <= 0 || lo > 4096 )
	{
		wxLogError( errtext + _(": Should be 1~4096"), WxStaticText4->GetLabel().c_str(), WxEditMaxColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditTabColumns->GetValue().ToLong( &lo ) || lo <= 0 || lo > 256 )
	{
		wxLogError( errtext + _(": Should be 1~256"), WxStaticText5->GetLabel().c_str(), WxEditTabColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditIndentColumns->GetValue().ToLong( &lo ) || lo <= 0 || lo > 256 )
	{
		wxLogError( errtext + _(": Should be 1~256"), WxStaticText6->GetLabel().c_str(), WxEditIndentColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditAutoSaveTimeout->GetValue().ToLong( &lo ) || ( lo < 10 || lo > 30 ))
	{
		wxLogError( errtext + _(": Should be 10~30"), WxStaticText30->GetLabel().c_str(), WxEditAutoSaveTimeout->GetValue().c_str() );
		error = true;
	}

	if( !error ) { EndModal( wxID_OK ); }
}

/*
 * WxButtonCancelClick
 */
void MadOptionsDialog::WxButtonCancelClick( wxCommandEvent& event )
{
	EndModal( wxID_CANCEL );
}

void MadOptionsDialog::PrintMarkButtonClick( wxCommandEvent& event )
{
	ButtonID = event.GetId();
	PopupMenu( WxPopupMenuPrintMark );
}

void MadOptionsDialog::PrintMarkClick( wxCommandEvent& event )
{
	wxString str = WxPopupMenuPrintMark->GetLabel( event.GetId() );
	wxTextCtrl *edit = nullptr;

	if (ButtonID == ID_WXBUTTON1) { edit = WxEditHeaderLeft; }
	else if (ButtonID == ID_WXBUTTON2) { edit = WxEditHeaderCenter; }
	else if (ButtonID == ID_WXBUTTON3) { edit = WxEditHeaderRight; }
	else if (ButtonID == ID_WXBUTTON4) { edit = WxEditFooterLeft; }
	else if (ButtonID == ID_WXBUTTON5) { edit = WxEditFooterCenter; }
	else if (ButtonID == ID_WXBUTTON6) { edit = WxEditFooterRight; }
	else { ; }

	if( edit != nullptr && str[0] == wxT( '[' ) && str[3] == wxT( ']' ) )
	{
		wxString text = edit->GetValue();
		edit->SetValue( text + str.Mid( 1, 2 ) );
	}

	ButtonID = 0;
}


/*
 * WxTreeCtrl1SelChanged
 */
void MadOptionsDialog::WxTreeCtrl1SelChanged( wxTreeEvent& event )
{
	wxTreeItemId id = event.GetItem();

	if( !id.IsOk() ) { return; }

	g_SelectedCommandItem = ( TreeItemData* )WxTreeCtrl1->GetItemData( id );
	g_SelectedKeyId = -1;

	if( g_SelectedCommandItem == nullptr )
	{
		WxListBoxKeys->Clear();
		WxEditCommandHint->SetValue( _( "Cannot assign key to this item" ) );
	}
	else
	{
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );
		wxString hint = wxT( '[' );

		if( g_SelectedCommandItem->cmddata->menuid_name > 0 )
		{
			hint += wxString( g_SelectedCommandItem->cmddata->menuid_name );
		}

		if( g_SelectedCommandItem->cmddata->command > 0 )
		{
			wxString text = MadKeyBindings::CommandToText( g_SelectedCommandItem->cmddata->command );

			if( !text.IsEmpty() )
			{
				if( hint.Length() > 1 )
				{
					hint += wxT( ',' );
				}

				hint += text;
			}
		}

		hint += wxT( "] " );
		hint += wxString( wxGetTranslation( g_SelectedCommandItem->cmddata->hint ) );
		WxEditCommandHint->SetValue( hint );
	}
}

/*
 * WxListBoxKeysSelected
 */
void MadOptionsDialog::WxListBoxKeysSelected( wxCommandEvent& event )
{
	g_SelectedKeyId = event.GetSelection();
}

// find key in TreeItemDataList
TreeItemData* MadOptionsDialog::FindKeyInList( const wxString &key )
{
	list<TreeItemData*>::iterator tidit = TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend = TreeItemDataList.end();
	wxString lkey = key.Lower();

	while( tidit != tiditend )
	{
		TreeItemData *tid = ( *tidit );

		if( !tid->keys.IsEmpty() )
		{
			size_t count = tid->keys.GetCount();
			size_t idx = 0;

			for( ; idx < count; ++idx )
			{
				if( tid->keys[idx].Lower() == lkey )
				{
					return tid;
				}
			}
		}

		++tidit;
	}

	return nullptr;
}

bool MadOptionsDialog::FindItemInList( TreeItemData* tid, const list<TreeItemData*> &tlist )
{
	list<TreeItemData*>::const_iterator tidit = tlist.begin();
	list<TreeItemData*>::const_iterator tiditend = tlist.end();

	while( tidit != tiditend )
	{
		if( ( *tidit ) == tid )
		{
			return true;
		}

		++tidit;
	}

	return false;
}

void MadOptionsDialog::UpdateKeyHint()
{
	wxString scstr = WxEditKey->GetValue();
	g_CommandItemOfNewKey = nullptr;

	if( scstr.IsEmpty() )
	{
		g_OptionsDialog->WxEditKeyHint->Clear();
	}
	else
	{
		// find the key is assigned to which command
		TreeItemData *tid = g_OptionsDialog->FindKeyInList( scstr );

		if( tid == nullptr )
		{
			g_OptionsDialog->WxEditKeyHint->SetValue( _( "This key is not assigned" ) );
		}
		else
		{
			g_CommandItemOfNewKey = tid;
			wxString cmd;

			if( tid->cmddata->menuid_name > 0 )
			{
				cmd += wxString( tid->cmddata->menuid_name );
			}

			if( tid->cmddata->command > 0 )
			{
				wxString text = MadKeyBindings::CommandToText( tid->cmddata->command );

				if( !text.IsEmpty() )
				{
					if( cmd.Length() > 1 )
					{
						cmd += wxT( ',' );
					}

					cmd += text;
				}
			}

			wxString str = wxString::Format( _( "This key is assigned to [%s]" ), cmd.c_str() );
			g_OptionsDialog->WxEditKeyHint->SetValue( str );
		}
	}
}

/*
 * WxButtonAddKeyClick
 */
void MadOptionsDialog::WxButtonAddKeyClick( wxCommandEvent& event )
{
	if( g_SelectedCommandItem != nullptr && g_SelectedCommandItem != g_CommandItemOfNewKey )
	{
		wxString key = WxEditKey->GetValue();

		if( g_CommandItemOfNewKey != nullptr ) // new key is assigned to another command
		{
			/*
			wxMessageDialog dlg(this, key +wxT(": ") +WxEditKeyHint->GetValue() +wxT("\n\n") + wxString(_("Do you want to reassign this key?")),
			    wxT("MadEdit-Mod"), wxYES_NO|wxICON_QUESTION );
			if(dlg.ShowModal()!=wxID_YES)
			{
			    return;
			}
			*/
			//delete the key from g_CommandItemOfNewKey
			int idx = g_CommandItemOfNewKey->keys.Index( key.c_str(), false );
			wxASSERT( idx >= 0 );
			g_CommandItemOfNewKey->keys.RemoveAt( idx );

			if( FindItemInList( g_CommandItemOfNewKey, ChangedTreeItemDataList ) == false )
			{
				ChangedTreeItemDataList.push_back( g_CommandItemOfNewKey );
			}
		}

		// add the key to g_SelectedCommandItem
		g_SelectedCommandItem->keys.Add( key );
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );

		if( FindItemInList( g_SelectedCommandItem, ChangedTreeItemDataList ) == false )
		{
			ChangedTreeItemDataList.push_back( g_SelectedCommandItem );
		}

		g_SelectedKeyId = -1;
		UpdateKeyHint();
	}
}

/*
 * WxButtonDeleteKeyClick
 */
void MadOptionsDialog::WxButtonDeleteKeyClick( wxCommandEvent& event )
{
	if( g_SelectedCommandItem != nullptr && g_SelectedKeyId >= 0 )
	{
		g_SelectedCommandItem->keys.RemoveAt( g_SelectedKeyId );
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );

		if( FindItemInList( g_SelectedCommandItem, ChangedTreeItemDataList ) == false )
		{
			ChangedTreeItemDataList.push_back( g_SelectedCommandItem );
		}

		g_SelectedKeyId = -1;
		UpdateKeyHint();
	}
}

/*
 * WxButtonShowInMenuClick
 */
void MadOptionsDialog::WxButtonShowInMenuClick( wxCommandEvent& event )
{
	// move the selected key to first element of WxListBoxKeys
	if( g_SelectedCommandItem != nullptr && g_SelectedCommandItem->cmddata->menu_id > 0 && g_SelectedKeyId > 0 )
	{
		wxString key = g_SelectedCommandItem->keys[g_SelectedKeyId];
		g_SelectedCommandItem->keys.RemoveAt( g_SelectedKeyId );
		g_SelectedCommandItem->keys.Insert( key, 0 );
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );

		if( FindItemInList( g_SelectedCommandItem, ChangedTreeItemDataList ) == false )
		{
			ChangedTreeItemDataList.push_back( g_SelectedCommandItem );
		}

		g_SelectedKeyId = -1;
	}
}


/*
 * WxButtonDateTimeClick
 */
void MadOptionsDialog::WxButtonDateTimeClick( wxCommandEvent& event )
{
	PopupMenu( WxPopupMenuDateTimeMark );
}

void MadOptionsDialog::DateTimeMarkClick( wxCommandEvent& event )
{
	wxString str = WxPopupMenuDateTimeMark->GetLabel( event.GetId() );
	int pos = str.Find( wxT( ']' ) );

	if( pos != wxNOT_FOUND && str[0] == wxT( '[' ) )
	{
		str = str.Mid( 1, pos - 1 );
	}

	wxString text = WxEditDateTime->GetValue();
	WxEditDateTime->SetValue( text + str );
}

#if ENABLE_BITMAP_THUARI
void MadOptionsDialog::WxButtonBitmapDirClick( wxCommandEvent& event )
{
}
void MadOptionsDialog::WxButtonThesauriDirClick( wxCommandEvent& event )
{
	wxString defdir = SpellCheckerManager::Instance().GetThesaurusPath();

	if( defdir.IsEmpty() ) { defdir = wxGetCwd(); }

	wxDirDialog dlg( this, _( "Choose a directory" ), defdir );

	if( dlg.ShowModal() == wxID_OK )
	{
		WxEditThesauriDir->SetValue( dlg.GetPath() );
		SpellCheckerManager::Instance().SetThesaurusPath( dlg.GetPath() );
		InitDictionaryChoice();
	}
}
#endif

void MadOptionsDialog::WxButtonDictionaryDirClick( wxCommandEvent& event )
{
	wxString defdir = SpellCheckerManager::Instance().GetDictionaryPath();

	if( defdir.IsEmpty() ) { defdir = wxGetCwd(); }

	wxDirDialog dlg( this, _( "Choose a directory" ), defdir );

	if( dlg.ShowModal() == wxID_OK )
	{
		WxEditDictionaryDir->SetValue( dlg.GetPath() );
		InitDictionaryChoice( dlg.GetPath() );
	}
}

void MadOptionsDialog::InitDictionaryChoice( const wxString &path/* = wxEmptyString*/ )
{
	if( !path.IsEmpty() )
	{
		SpellCheckerManager::Instance().SetDictionaryPath( path );
		SpellCheckerManager::Instance().ScanForDictionaries();
	}

	std::vector<wxString> dics = SpellCheckerManager::Instance().GetPossibleDictionaries();
	int sel = SpellCheckerManager::Instance().GetSelectedDictionaryNumber();
	WxChoiceDictionary->Clear();

	for( unsigned int i = 0 ; i < dics.size(); i++ )
	{ WxChoiceDictionary->AppendString( SpellCheckerManager::Instance().GetLanguageName( dics[i] ) ); }

	if( sel != -1 )
	{ WxChoiceDictionary->Select( sel ); }
}

void MadOptionsDialog::OnDictionaryDirChange( wxCommandEvent& event )
{
	wxString path = WxEditDictionaryDir->GetValue();

	if( wxDir::Exists( path ) )
	{
		InitDictionaryChoice( path );
	}
	else
	{
		WxChoiceDictionary->Clear();
	}
}

void MadOptionsDialog::OnSelectDictionary( wxCommandEvent& event )
{
	wxString dictDesc = WxChoiceDictionary->GetString( WxChoiceDictionary->GetSelection() );
	wxString dictName = SpellCheckerManager::Instance().GetDictionaryName( dictDesc );

	if( !dictName.IsEmpty() )
	{
		SpellCheckerManager::Instance().SetDictionaryName( dictName );
	}
}

void MadOptionsDialog::OnMouseAutoCopyClicked( wxCommandEvent& event )
{
	WxCheckBoxCtrlWithMouseToSelect->Enable( WxCheckBoxMouseSelectToCopy->GetValue() );
}

void MadOptionsDialog::OnAutoCompletePairClicked( wxCommandEvent& event )
{
	WxCheckBoxInsertPairForSelection->Enable( WxCheckBoxAutoCompletePair->GetValue() );
}

void MadOptionsDialog::OnEnableAutoSaveClick( wxCommandEvent& event )
{
	WxEditAutoSaveTimeout->Enable( WxCheckBoxEnableAutoSave->GetValue() );
}

#ifdef MADEDIT_ENABLE_STC
void MadOptionsDialog::OnMarginClick( wxStyledTextEvent &event )
{
	if( event.GetMargin() == MARGIN_FOLD )
	{
		int lineClick = WxTextSample->LineFromPosition( event.GetPosition() );
		int levelClick = WxTextSample->GetFoldLevel( lineClick );

		if( ( levelClick & wxSTC_FOLDLEVELHEADERFLAG ) > 0 )
		{
			WxTextSample->ToggleFold( lineClick );
		}
	}
}
#endif
