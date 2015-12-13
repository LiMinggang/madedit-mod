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
#include "astyle/astyle.h"
#include "astylepredefinedstyles.h"

#ifdef MADEDIT_ENABLE_STC
enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};
#endif


//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

MadOptionsDialog *g_OptionsDialog = NULL;
extern wxArrayString g_LanguageString;
extern wxString g_MadEditAppDir;

TreeItemData *g_SelectedCommandItem = NULL;
int g_SelectedKeyId = -1;
TreeItemData *g_CommandItemOfNewKey = NULL;
#define ENABLE_BITMAP_THUARI 0
static wxString bracket_style[aspsCustom + 1] =
{
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isBar)\n    {\n        bar();\n        return 1;\n    }\n    else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n    } else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isBar) {\n        bar();\n        return 1;\n    } else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n     if (isBar) {\n          bar();\n          return 1;\n     } else\n          return 0;\n}" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n    {\n    if (isBar)\n        {\n        bar();\n        return 1;\n        }\n    else\n        return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n    {\n        if (isBar)\n            {\n            bar();\n            return 1;\n            }\n        else\n            return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n        }\n    else\n        return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n  if (isBar)\n    {\n      bar();\n      return 1;\n    }\n  else\n    return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n        if (isBar) {\n                bar();\n                return 1;\n        } else\n                return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1;\n   }\n   else\n      return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isFoo) {\n        bar();\n        return 1;\n    } else {\n        return 0;\n    }\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n    }\n    else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1; }\n    else\n      return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
};
class KeyTextCtrl : public wxTextCtrl
{
	DECLARE_EVENT_TABLE()
public:
	KeyTextCtrl( wxWindow *parent, wxWindowID id,
				 const wxString& value = wxEmptyString,
				 const wxPoint& pos = wxDefaultPosition,
				 const wxSize& size = wxDefaultSize,
				 long style = 0,
				 const wxValidator& validator = wxDefaultValidator,
				 const wxString& name = wxTextCtrlNameStr )
		: wxTextCtrl( parent, id, value, pos, size, style, validator, name ) {
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
		g_OptionsDialog->WxButtonCancel->SetId( wxID_CANCEL );
		evt.Skip();
	}
};

BEGIN_EVENT_TABLE( KeyTextCtrl, wxTextCtrl )
	EVT_KEY_DOWN( KeyTextCtrl::OnKeyDown )
	EVT_SET_FOCUS( KeyTextCtrl::OnSetFocus )
	EVT_KILL_FOCUS( KeyTextCtrl::OnKillFocus )
END_EVENT_TABLE()


//----------------------------------------------------------------------------
// MadOptionsDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate Block.
// Code added in  other places will be removed by wx-dvcpp
////Event Table Start
BEGIN_EVENT_TABLE( MadOptionsDialog, wxDialog )
	////Manual Code Start
	EVT_BUTTON( ID_WXBUTTON1, MadOptionsDialog::PrintMarkButtonClick )
	EVT_BUTTON( ID_WXBUTTON2, MadOptionsDialog::PrintMarkButtonClick )
	EVT_BUTTON( ID_WXBUTTON3, MadOptionsDialog::PrintMarkButtonClick )
	EVT_BUTTON( ID_WXBUTTON4, MadOptionsDialog::PrintMarkButtonClick )
	EVT_BUTTON( ID_WXBUTTON5, MadOptionsDialog::PrintMarkButtonClick )
	EVT_BUTTON( ID_WXBUTTON6, MadOptionsDialog::PrintMarkButtonClick )
	EVT_BUTTON( wxID_CANCEL, MadOptionsDialog::WxButtonCancelClick )
	////Manual Code End

	EVT_CLOSE( MadOptionsDialog::MadOptionsDialogClose )
	EVT_ACTIVATE( MadOptionsDialog::MadOptionsDialogActivate )
	EVT_MENU( ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___D__M__Y_24_02_2007_1192, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU_MENUITEM1_1170, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___A_FULLWEEKDAYNAME_1171, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___B_ABBREVIATEDMONTHNAME_1172, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___B_FULLMONTHNAME_1173, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___Z_TIME_ZONENAME_1193, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU___Z_TIME_ZONEABBREVIATION_1194, MadOptionsDialog::DateTimeMarkClick )
	EVT_MENU( ID_MNU_MENUITEM1_1110, MadOptionsDialog::PrintMarkClick )
	EVT_MENU( ID_MNU___P__PATHNAME_1111, MadOptionsDialog::PrintMarkClick )
	EVT_MENU( ID_MNU___N_PAGE_NUMBER_1113, MadOptionsDialog::PrintMarkClick )
	EVT_MENU( ID_MNU___S__TOTALPAGES_1114, MadOptionsDialog::PrintMarkClick )
	EVT_MENU( ID_MNU___D__DATE_1116, MadOptionsDialog::PrintMarkClick )
	EVT_MENU( ID_MNU___T__TIME_1117, MadOptionsDialog::PrintMarkClick )
	EVT_BUTTON( ID_WXBUTTONCANCEL, MadOptionsDialog::WxButtonCancelClick )
	EVT_BUTTON( ID_WXBUTTONOK, MadOptionsDialog::WxButtonOKClick )
	//  EVT_BUTTON(ID_WXBITMAP_DIR,MadOptionsDialog::WxButtonBitmapDirClick)
	//  EVT_BUTTON(ID_WXTHESAURI_DIR,MadOptionsDialog::WxButtonThesauriDirClick)
	EVT_BUTTON( ID_WXDICTIONARY_DIR, MadOptionsDialog::WxButtonDictionaryDirClick )
	EVT_TEXT_ENTER( ID_WXEDITDICTIONARYDIR, MadOptionsDialog::OnDictionaryDirChange )
	EVT_CHOICE( ID_WXCHOICEDICTIONARY, MadOptionsDialog::OnSelectDictionary )
	EVT_BUTTON( ID_WXBUTTONSHOWINMENU, MadOptionsDialog::WxButtonShowInMenuClick )
	EVT_BUTTON( ID_WXBUTTONDELETEKEY, MadOptionsDialog::WxButtonDeleteKeyClick )
	EVT_BUTTON( ID_WXBUTTONADDKEY, MadOptionsDialog::WxButtonAddKeyClick )
	EVT_LISTBOX( ID_WXLISTBOXKEYS, MadOptionsDialog::WxListBoxKeysSelected )

	EVT_TREE_SEL_CHANGED( ID_WXTREECTRL1, MadOptionsDialog::WxTreeCtrl1SelChanged )
	EVT_CHECKBOX( ID_WXCHECKBOXMOUSESELECTTOCOPY, MadOptionsDialog::OnMouseAutoCopyClicked )
	EVT_BUTTON( ID_WXBUTTONDATETIME, MadOptionsDialog::WxButtonDateTimeClick )

	EVT_RADIOBOX( ID_WXRADIOBOXBRACKETSTYLE, MadOptionsDialog::OnRadioBoxBracketStyleClick )
	EVT_CHECKBOX( ID_WXCHECKBREAKLINES, MadOptionsDialog::OnFormattingBreakLinesClick )
	EVT_CHECKBOX( ID_WXCHECKBREAKBLOCKS, MadOptionsDialog::OnPaddingBreakBlocksClick )
END_EVENT_TABLE()
////Event Table End



MadOptionsDialog::MadOptionsDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
	: wxDialog( parent, id, title, position, size, style )
{
	ButtonID = 0;
	CreateGUIControls();
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
	WxNotebook1 = new wxAuiNotebook( this, ID_WXNOTEBOOK1, wxPoint( 0, 0 ), wxSize( 700, 492 ), wxNB_DEFAULT );
	WxBoxSizer1->Add( WxNotebook1, 1, wxEXPAND | wxALL, 0 );
	WxNoteBookPage1 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE1, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
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
	WxCheckBoxRecordCaretMovements = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXRECORDCARETMOVEMENTS, _( "Record caret movements in undo list" ), wxPoint( 2, 26 ), wxSize( 300, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxRecordCaretMovements" ) );
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
	WxBoxSizer7->Add( WxCheckBoxShowQSearchBar, 0, wxALIGN_LEFT | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxShowQSearchBar );
	WxCheckBoxDoNotSaveSettings = new wxCheckBox( WxNoteBookPage1, ID_WXCHECKBOXDONOTSAVESETTINGS, _( "Do not save settings to MadEdit.cfg when MadEdit closed (this session only)" ), wxPoint( 2, 74 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxDoNotSaveSettings" ) );
	WxBoxSizer7->Add( WxCheckBoxDoNotSaveSettings, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxDoNotSaveSettings );
	WxCheckBoxPurgeHistory = new wxCheckBox( WxNoteBookPage1, ID_PURGEHISTORY, _( "Purge History while exiting" ), wxPoint( 2, 98 ), wxSize( 400, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPurgeHistory" ) );
	WxBoxSizer7->Add( WxCheckBoxPurgeHistory, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxPurgeHistory );
	WxNoteBookPage2 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE2, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
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
	WxButtonDateTime = new wxButton( WxNoteBookPage2, ID_WXBUTTONDATETIME, wxT( ">>" ), wxPoint( 273, 1 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButtonDateTime" ) );
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
	WxBoxSizer28 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer12->Add( WxBoxSizer28, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0 );
	WxCheckBoxMouseSelectToCopy = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXMOUSESELECTTOCOPY, _( "Auto copy the mouse-selected text to clipboard" ), wxPoint( 2, 2 ), wxSize( 260, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxMouseSelectToCopy" ) );
	WxBoxSizer28->Add( WxCheckBoxMouseSelectToCopy, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxMouseSelectToCopy );
	WxCheckBoxCtrlWithMouseToSelect = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXCTRLWITHMOUSE, _( "when pressing Ctrl key" ), wxPoint( 254, 3 ), wxSize( 136, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxCtrlWithMouseToSelect" ) );
	WxBoxSizer28->Add( WxCheckBoxCtrlWithMouseToSelect, 0, wxALIGN_CENTER | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxCtrlWithMouseToSelect );
	WxCheckBoxMiddleMouseToPaste = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXMIDDLEMOUSETOPASTE, _( "Paste text from clipboard when pressing middle mouse button" ), wxPoint( 24, 98 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxMiddleMouseToPaste" ) );
	WxBoxSizer12->Add( WxCheckBoxMiddleMouseToPaste, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxMiddleMouseToPaste );
	WxCheckBoxAutoFillColumnPaste = new wxCheckBox( WxNoteBookPage2, ID_WXCHECKBOXAUTOFILLCOLUMN, _( "Auto fill in column paste" ), wxPoint( 24, 122 ), wxSize( 480, 20 ), 0, wxDefaultValidator, wxT( "WxCheckBoxAutoFillColumnPaste" ) );
	WxBoxSizer12->Add( WxCheckBoxAutoFillColumnPaste, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBoxAutoFillColumnPaste );
	WxNoteBookPage3 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE3, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxNotebook1->AddPage( WxNoteBookPage3, _( "Print" ) );
	WxBoxSizer14 = new wxBoxSizer( wxHORIZONTAL );
	WxNoteBookPage3->SetSizer( WxBoxSizer14 );
	WxNoteBookPage3->SetAutoLayout( true );
	WxBoxSizer15 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer14->Add( WxBoxSizer15, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, _( "Text Mode" ) );
	WxStaticBoxSizer1 = new wxStaticBoxSizer( WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL );
	WxBoxSizer15->Add( WxStaticBoxSizer1, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 4 );
	WxCheckBoxPrintSyntax = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTSYNTAX, _( "Print Syntax Highlighter" ), wxPoint( 6, 16 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintSyntax" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintSyntax, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintSyntax );
	WxCheckBoxPrintLineNumber = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTLINENUMBER, _( "Print Line Number" ), wxPoint( 6, 35 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintLineNumber" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintLineNumber, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintLineNumber );
	WxCheckBoxPrintBookmark = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTBOOKMARK, _( "Print Bookmark" ), wxPoint( 6, 54 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintBookmark" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintBookmark, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintBookmark );
	WxCheckBoxPrintEndOfLine = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTENDOFLINE, _( "Print End of Line" ), wxPoint( 6, 73 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintEndOfLine" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintEndOfLine, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintEndOfLine );
	WxCheckBoxPrintTabChar = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTTABCHAR, _( "Print Tab Char" ), wxPoint( 6, 92 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintTabChar" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintTabChar, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintTabChar );
	WxCheckBoxPrintSpaceChar = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTSPACECHAR, _( "Print Space Char" ), wxPoint( 6, 111 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintSpaceChar" ) );
	WxStaticBoxSizer1->Add( WxCheckBoxPrintSpaceChar, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintSpaceChar );
	wxStaticBox* WxStaticBoxSizer2_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, wxT( "Hex Mode" ) );
	WxStaticBoxSizer2 = new wxStaticBoxSizer( WxStaticBoxSizer2_StaticBoxObj, wxVERTICAL );
	WxBoxSizer15->Add( WxStaticBoxSizer2, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 4 );
	wxArrayString arrayStringFor_WxRadioBoxPrintOffset;
	arrayStringFor_WxRadioBoxPrintOffset.Add( _( "None" ) );
	arrayStringFor_WxRadioBoxPrintOffset.Add( _( "First Page Only" ) );
	arrayStringFor_WxRadioBoxPrintOffset.Add( _( "Every Page" ) );
	WxRadioBoxPrintOffset = new wxRadioBox( WxNoteBookPage3, ID_WXRADIOBOXPRINTOFFSET, _( "Print Offset Header" ), wxPoint( 6, 16 ), wxSize( 150, 130 ), arrayStringFor_WxRadioBoxPrintOffset, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, wxT( "WxRadioBoxPrintOffset" ) );
	WxRadioBoxPrintOffset->SetSelection( 0 );
	WxStaticBoxSizer2->Add( WxRadioBoxPrintOffset, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxRadioBoxPrintOffset );
	WxBoxSizer16 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer14->Add( WxBoxSizer16, 1, wxEXPAND | wxALL, 2 );
	wxStaticBox* WxStaticBoxSizer3_StaticBoxObj = new wxStaticBox( WxNoteBookPage3, wxID_ANY, _( "Page Header" ) );
	WxStaticBoxSizer3 = new wxStaticBoxSizer( WxStaticBoxSizer3_StaticBoxObj, wxVERTICAL );
	WxBoxSizer16->Add( WxStaticBoxSizer3, 1, wxEXPAND | wxALL, 4 );
	WxCheckBoxPrintPageHeader = new wxCheckBox( WxNoteBookPage3, ID_WXCHECKBOXPRINTPAGEHEADER, _( "Print Page Header" ), wxPoint( 86, 16 ), wxSize( 150, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBoxPrintPageHeader" ) );
	WxStaticBoxSizer3->Add( WxCheckBoxPrintPageHeader, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBoxPrintPageHeader );
	WxFlexGridSizer1 = new wxFlexGridSizer( 0, 3, 0, 0 );
	WxStaticBoxSizer3->Add( WxFlexGridSizer1, 1, wxEXPAND | wxALL, 2 );
	WxStaticText7 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT7, _( "Left:" ), wxPoint( 8, 5 ), wxDefaultSize, 0, wxT( "WxStaticText7" ) );
	WxFlexGridSizer1->Add( WxStaticText7, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditHeaderLeft = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERLEFT, wxT( "" ), wxPoint( 44, 3 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditHeaderLeft" ) );
	WxFlexGridSizer1->Add( WxEditHeaderLeft, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditHeaderLeft );
	WxButton1 = new wxButton( WxNoteBookPage3, ID_WXBUTTON1, wxT( ">>" ), wxPoint( 287, 1 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButton1" ) );
	WxFlexGridSizer1->Add( WxButton1, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton1 );
	WxStaticText8 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT8, _( "Center:" ), wxPoint( 2, 32 ), wxDefaultSize, 0, wxT( "WxStaticText8" ) );
	WxFlexGridSizer1->Add( WxStaticText8, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditHeaderCenter = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERCENTER, wxT( "" ), wxPoint( 44, 30 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditHeaderCenter" ) );
	WxFlexGridSizer1->Add( WxEditHeaderCenter, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditHeaderCenter );
	WxButton2 = new wxButton( WxNoteBookPage3, ID_WXBUTTON2, wxT( ">>" ), wxPoint( 287, 28 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButton2" ) );
	WxFlexGridSizer1->Add( WxButton2, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton2 );
	WxStaticText9 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT9, _( "Right:" ), wxPoint( 5, 59 ), wxDefaultSize, 0, wxT( "WxStaticText9" ) );
	WxFlexGridSizer1->Add( WxStaticText9, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditHeaderRight = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERRIGHT, wxT( "" ), wxPoint( 44, 57 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditHeaderRight" ) );
	WxFlexGridSizer1->Add( WxEditHeaderRight, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditHeaderRight );
	WxButton3 = new wxButton( WxNoteBookPage3, ID_WXBUTTON3, wxT( ">>" ), wxPoint( 287, 55 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButton3" ) );
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
	WxButton4 = new wxButton( WxNoteBookPage3, ID_WXBUTTON4, wxT( ">>" ), wxPoint( 287, 1 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButton4" ) );
	WxFlexGridSizer2->Add( WxButton4, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton4 );
	WxStaticText11 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT8, _( "Center:" ), wxPoint( 2, 32 ), wxDefaultSize, 0, wxT( "WxStaticText11" ) );
	WxFlexGridSizer2->Add( WxStaticText11, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditFooterCenter = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERCENTER, wxT( "" ), wxPoint( 44, 30 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditFooterCenter" ) );
	WxFlexGridSizer2->Add( WxEditFooterCenter, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditFooterCenter );
	WxButton5 = new wxButton( WxNoteBookPage3, ID_WXBUTTON5, wxT( ">>" ), wxPoint( 287, 28 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButton5" ) );
	WxFlexGridSizer2->Add( WxButton5, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxButton5 );
	WxStaticText12 = new wxStaticText( WxNoteBookPage3, ID_WXSTATICTEXT9, _( "Right:" ), wxPoint( 5, 59 ), wxDefaultSize, 0, wxT( "WxStaticText12" ) );
	WxFlexGridSizer2->Add( WxStaticText12, 0, wxALIGN_RIGHT | wxALL, 2 );
	WxEditFooterRight = new wxTextCtrl( WxNoteBookPage3, ID_WXEDITHEADERRIGHT, wxT( "" ), wxPoint( 44, 57 ), wxSize( 240, 21 ), 0, wxDefaultValidator, wxT( "WxEditFooterRight" ) );
	WxFlexGridSizer2->Add( WxEditFooterRight, 0, 0 | wxALL, 2 );
	SET_CONTROLPARENT( WxEditFooterRight );
	WxButton6 = new wxButton( WxNoteBookPage3, ID_WXBUTTON6, wxT( ">>" ), wxPoint( 287, 55 ), wxSize( 25, 25 ), 0, wxDefaultValidator, wxT( "WxButton6" ) );
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
	WxChoiceDictionary = new wxChoice( WxNoteBookPage5, ID_WXCHOICEDICTIONARY, wxPoint( 74, 20 ), wxSize( 320, 25 ), arrayStringFor_WxChoiceDictionary, 0, wxDefaultValidator, wxT( "" ) );
	WxChoiceDictionary->SetSelection( -1 );
	WxStaticBoxSizer5->Add( WxChoiceDictionary, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxChoiceDictionary );
	wxStaticBox* WxStaticBoxSizer6_StaticBoxObj = new wxStaticBox( WxNoteBookPage5, wxID_ANY, _( "Path Setting" ) );
	WxStaticBoxSizer6 = new wxStaticBoxSizer( WxStaticBoxSizer6_StaticBoxObj, wxVERTICAL );
	WxBoxSizer29->Add( WxStaticBoxSizer6, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxBoxSizer30 = new wxBoxSizer( wxHORIZONTAL );
	WxStaticBoxSizer6->Add( WxBoxSizer30, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditDictionaryDir = new wxTextCtrl( WxNoteBookPage5, ID_WXEDITDICTIONARYDIR, wxT( "" ), wxPoint( 5, 8 ), wxSize( 240, 25 ), 0, wxTextValidator( wxFILTER_NONE, NULL ), wxT( "WxEditDictionaryDir" ) );
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
	WxEditThesauriDir = new wxTextCtrl( WxNoteBookPage5, ID_WXEDITTHESAURI, wxT( "" ), wxPoint( 5, 8 ), wxSize( 240, 19 ), 0, wxTextValidator( wxFILTER_NONE, NULL ), wxT( "WxEditThesauriDir" ) );
	WxBoxSizer31->Add( WxEditThesauriDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxEditThesauriDir );
	WxButtonThesauriDir = new wxButton( WxNoteBookPage5, ID_WXTHESAURI_DIR, wxT( "..." ), wxPoint( 255, 5 ), wxSize( 75, 25 ), 0, wxDefaultValidator, wxT( "WxButtonThesauriDir" ) );
	WxBoxSizer31->Add( WxButtonThesauriDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonThesauriDir );
	WxStaticText20 = new wxStaticText( WxNoteBookPage5, ID_WXSTATICTEXT20, _( "Thesauri" ), wxPoint( 340, 9 ), wxDefaultSize, 0, wxT( "WxStaticText20" ) );
	WxBoxSizer31->Add( WxStaticText20, 0, wxALIGN_CENTER | wxALL, 5 );
	WxBoxSizer32 = new wxBoxSizer( wxHORIZONTAL );
	WxStaticBoxSizer6->Add( WxBoxSizer32, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxEditBitMapDir = new wxTextCtrl( WxNoteBookPage5, ID_WXEDITBITMAPDIR, wxT( "" ), wxPoint( 5, 8 ), wxSize( 240, 19 ), 0, wxTextValidator( wxFILTER_NONE, NULL ), wxT( "WxEditBitMapDir" ) );
	WxBoxSizer32->Add( WxEditBitMapDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxEditBitMapDir );
	WxButtonBitmapDir = new wxButton( WxNoteBookPage5, ID_WXBITMAP_DIR, wxT( "..." ), wxPoint( 255, 5 ), wxSize( 75, 25 ), 0, wxDefaultValidator, wxT( "WxButtonBitmapDir" ) );
	WxBoxSizer32->Add( WxButtonBitmapDir, 0, wxALIGN_CENTER | wxALL, 5 );
	SET_CONTROLPARENT( WxButtonBitmapDir );
	WxStaticText21 = new wxStaticText( WxNoteBookPage5, ID_WXSTATICTEXT21, _( "BitMap" ), wxPoint( 340, 9 ), wxDefaultSize, 0, wxT( "WxStaticText21" ) );
	WxBoxSizer32->Add( WxStaticText21, 0, wxALIGN_CENTER | wxALL, 5 );
#endif
	WxNoteBookPage6 = new wxPanel( WxNotebook1, ID_WXNOTEBOOKPAGE6, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxNotebook1->AddPage( WxNoteBookPage6, _( "Source Formatter" ) );
	WxBoxSizer34 = new wxBoxSizer( wxHORIZONTAL );
	WxNoteBookPage6->SetSizer( WxBoxSizer34 );
	WxNoteBookPage6->SetAutoLayout( true );
	WxAuiNotebook1 = new wxAuiNotebook( WxNoteBookPage6, ID_WXAUINOTEBOOK1, wxPoint( 5, 5 ), wxSize( 668, 398 ), wxNB_DEFAULT );
	WxBoxSizer34->Add( WxAuiNotebook1, 0, wxEXPAND | wxALL, 5 );
	WxAuiNoteBookPage1 = new wxPanel( WxAuiNotebook1, ID_WXAUINOTEBOOKPAGE1, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxAuiNotebook1->AddPage( WxAuiNoteBookPage1, _( "Style" ) );
	WxBoxSizer47 = new wxBoxSizer( wxHORIZONTAL );
	WxAuiNoteBookPage1->SetSizer( WxBoxSizer47 );
	WxAuiNoteBookPage1->SetAutoLayout( true );
	wxArrayString arrayStringFor_WxRadioBoxBracketStyle;
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Allman(ANSI)" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Java" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Kr" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Stroustrup" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Whitesmith" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "VTK" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Banner" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Gnu" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Linux" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Horstmann" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "1TBS" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Google" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Pico" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Lisp" ) );
	arrayStringFor_WxRadioBoxBracketStyle.Add( wxT( "Custom" ) );
	WxRadioBoxBracketStyle = new wxRadioBox( WxAuiNoteBookPage1, ID_WXRADIOBOXBRACKETSTYLE, _( "Bracket Style" ), wxPoint( 2, 28 ), wxSize( 124, 400 ), arrayStringFor_WxRadioBoxBracketStyle, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, wxT( "WxRadioBoxBracketStyle" ) );
	WxRadioBoxBracketStyle->SetSelection( 0 );
	WxBoxSizer47->Add( WxRadioBoxBracketStyle, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 2 );
	SET_CONTROLPARENT( WxRadioBoxBracketStyle );
	wxStaticBox* WxStaticBoxSizer7_StaticBoxObj = new wxStaticBox( WxAuiNoteBookPage1, wxID_ANY, _( "Sample" ) );
	WxStaticBoxSizer7 = new wxStaticBoxSizer( WxStaticBoxSizer7_StaticBoxObj, wxVERTICAL );
	WxBoxSizer47->Add( WxStaticBoxSizer7, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5 );
	WxStaticText27 = new wxStaticText( WxAuiNoteBookPage1, ID_WXSTATICTEXT27, _( "Sample of the bracket style option chosen to use:" ), wxPoint( 10, 20 ), wxDefaultSize, 0, wxT( "WxStaticText27" ) );
	WxStaticBoxSizer7->Add(WxStaticText27, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);
#ifdef MADEDIT_ENABLE_MADEDIT
	WxTextSample = new MadEdit(WxAuiNoteBookPage1, ID_WXRICHTEXTSAMPLE, wxPoint(19, 47), wxSize(300, 300));
	WxTextSample->SetEncoding(wxT("UTF-32LE"));
	WxTextSample->SetFixedWidthMode(false);
	WxTextSample->SetRecordCaretMovements(false);
	WxTextSample->SetInsertSpacesInsteadOfTab(false);
	WxTextSample->SetWantTab(false);
	WxTextSample->SetMaxLineLength(DEFAULT_MAX_LINELEN);
	WxTextSample->SetReadOnly(true);
	WxTextSample->SetDisplayBookmark(false);
	WxTextSample->SetShowSpaceChar(false);
	WxTextSample->SetShowTabChar(false);
	WxTextSample->SetShowEndOfLine(false);
	WxTextSample->SetSpellCheck(false);
	WxTextSample->SetSyntax(wxT("C/C++"));
	WxTextSample->SetText(bracket_style[aspsAllman]);
#elif MADEDIT_ENABLE_STC
	WxTextSample = new wxStyledTextCtrl( WxAuiNoteBookPage1, ID_WXRICHTEXTSAMPLE, wxPoint( 19, 47 ), wxSize( 219, 300 ) );
	WxTextSample->StyleClearAll();
	WxTextSample->SetLexer( wxSTC_LEX_CPP );
	WxTextSample->SetMarginWidth( MARGIN_LINE_NUMBERS, 50 );
	WxTextSample->StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( 75, 75, 75 ) );
	WxTextSample->StyleSetBackground( wxSTC_STYLE_LINENUMBER, wxColour( 220, 220, 220 ) );
	WxTextSample->SetMarginType( MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER );
	// ---- Enable code folding
	WxTextSample->SetMarginType( MARGIN_FOLD, wxSTC_MARGIN_SYMBOL );
	WxTextSample->SetMarginWidth( MARGIN_FOLD, 15 );
	WxTextSample->SetMarginMask( MARGIN_FOLD, wxSTC_MASK_FOLDERS );
	WxTextSample->StyleSetBackground( MARGIN_FOLD, wxColor( 200, 200, 200 ) );
	WxTextSample->SetMarginSensitive( MARGIN_FOLD, true );
	// Properties found from http://www.scintilla.org/SciTEDoc.html
	WxTextSample->SetProperty( wxT( "fold" ),         wxT( "1" ) );
	WxTextSample->SetProperty( wxT( "fold.comment" ), wxT( "1" ) );
	WxTextSample->SetProperty( wxT( "fold.compact" ), wxT( "1" ) );
	wxColor grey( 100, 100, 100 );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERSUB, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERSUB, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_ARROW );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, _T( "WHITE" ) );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, _T( "WHITE" ) );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	// ---- End of code folding part
	WxTextSample->SetWrapMode( wxSTC_WRAP_WORD ); // other choice is wxSCI_WRAP_NONE
	WxTextSample->SetText( bracket_style[aspsAllman] );
	WxTextSample->StyleSetForeground( wxSTC_C_STRING,            wxColour( 150, 0, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_PREPROCESSOR,      wxColour( 165, 105, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_IDENTIFIER,        wxColour( 40, 0, 60 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_NUMBER,            wxColour( 0, 150, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_CHARACTER,         wxColour( 150, 0, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_WORD,              wxColour( 0, 0, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_WORD2,             wxColour( 0, 150, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENT,           wxColour( 150, 150, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTLINE,       wxColour( 150, 150, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTDOC,        wxColour( 150, 150, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORD, wxColour( 0, 0, 200 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour( 0, 0, 200 ) );
	WxTextSample->StyleSetBold( wxSTC_C_WORD, true );
	WxTextSample->StyleSetBold( wxSTC_C_WORD2, true );
	WxTextSample->StyleSetBold( wxSTC_C_COMMENTDOCKEYWORD, true );
	// a sample list of keywords, I haven't included them all to keep it short...
	WxTextSample->SetKeyWords( 0, wxT( "if else return for while break continue" ) );
	WxTextSample->SetKeyWords( 1, wxT( "const int float void char double bool" ) );
	WxTextSample->SetReadOnly( true );
	WxTextSample->Connect( wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler( MadOptionsDialog::OnMarginClick ), NULL, this );
#else
	WxTextSample = new wxTextCtrl( WxAuiNoteBookPage1, ID_WXRICHTEXTSAMPLE, wxT( "" ), wxPoint( 19, 47 ), wxSize( 219, 300 ), wxTE_MULTILINE | wxVSCROLL | wxHSCROLL | wxTE_READONLY, wxDefaultValidator, wxT( "WxTextSample" ) );
	//WxTextSample->SetMaxLength(0);
	WxTextSample->AppendText( bracket_style[aspsAllman] );
#endif
	WxTextSample->SetFocus();
#ifndef MADEDIT_ENABLE_MADEDIT
	WxTextSample->SetInsertionPointEnd();
	SET_CONTROLPARENT( WxTextSample );
#endif
	WxStaticBoxSizer7->Add( WxTextSample, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	WxAuiNoteBookPage2 = new wxPanel( WxAuiNotebook1, ID_WXAUINOTEBOOKPAGE2, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxAuiNotebook1->AddPage( WxAuiNoteBookPage2, _( "Brackets" ) );
	WxBoxSizer35 = new wxBoxSizer( wxVERTICAL );
	WxAuiNoteBookPage2->SetSizer( WxBoxSizer35 );
	WxAuiNoteBookPage2->SetAutoLayout( true );
	WxCheckAttachClasses = new wxCheckBox( WxAuiNoteBookPage2, ID_WXCHECKATTACHCLASSES, _( "Attach Classes" ), wxPoint( 2, 2 ), wxSize( 250, 17 ), 0, wxDefaultValidator, wxT( "WxCheckAttachClasses" ) );
	WxBoxSizer35->Add( WxCheckAttachClasses, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckAttachClasses );
	WxCheckAttachExternC = new wxCheckBox( WxAuiNoteBookPage2, ID_WXCHECKATTACHEXTERNC, _( "Attach Extern C" ), wxPoint( 2, 23 ), wxSize( 250, 17 ), 0, wxDefaultValidator, wxT( "WxCheckAttachExternC" ) );
	WxBoxSizer35->Add( WxCheckAttachExternC, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckAttachExternC );
	WxCheckAttachNamespaces = new wxCheckBox( WxAuiNoteBookPage2, ID_WXCHECKATTACHNAMESPACES, _( "Attach Namespaces" ), wxPoint( 2, 44 ), wxSize( 250, 17 ), 0, wxDefaultValidator, wxT( "WxCheckAttachNamespaces" ) );
	WxBoxSizer35->Add( WxCheckAttachNamespaces, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxCheckAttachInlines = new wxCheckBox( WxAuiNoteBookPage2, ID_WXCHECKATTACHINLINES, _( "Attach Inlines" ), wxPoint( 2, 65 ), wxSize( 250, 17 ), 0, wxDefaultValidator, wxT( "WxCheckAttachInlines" ) );
	WxBoxSizer35->Add( WxCheckAttachInlines, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckAttachInlines );
	WxAuiNoteBookPage3 = new wxPanel( WxAuiNotebook1, ID_WXAUINOTEBOOKPAGE3, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxAuiNotebook1->AddPage( WxAuiNoteBookPage3, _( "Indentation" ) );
	WxBoxSizer39 = new wxBoxSizer( wxVERTICAL );
	WxAuiNoteBookPage3->SetSizer( WxBoxSizer39 );
	WxAuiNoteBookPage3->SetAutoLayout( true );
	WxBoxSizer37 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer39->Add( WxBoxSizer37, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxStaticText17 = new wxStaticText( WxAuiNoteBookPage3, ID_WXSTATICTEXT17, _( "Indentation size (in spaces):" ), wxPoint( 1, 3 ), wxDefaultSize, 0, wxT( "WxStaticText17" ) );
	WxBoxSizer37->Add( WxStaticText17, 0, wxALIGN_LEFT | wxALIGN_BOTTOM | wxALL, 1 );
	WxSpinIndentation = new wxSpinCtrl( WxAuiNoteBookPage3, ID_WXSPINCTRLINDENTSIZE, wxT( "4" ), wxPoint( 138, 1 ), wxSize( 60, 19 ), wxSP_ARROW_KEYS, 2, 8, 4 );
	WxBoxSizer37->Add( WxSpinIndentation, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxSpinIndentation );
	WxBoxSizer38 = new wxBoxSizer( wxVERTICAL );
	WxBoxSizer39->Add( WxBoxSizer38, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxCheckUseTab = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKUSETABOVERSPACES, _( "Use TABs instead of spaces" ), wxPoint( 2, 2 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckUseTab" ) );
	WxBoxSizer38->Add( WxCheckUseTab, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckUseTab );
	WxCheckForceUseTabs = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKFORCETABS, _( "Force using TABs" ), wxPoint( 2, 23 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckForceUseTabs" ) );
	WxBoxSizer38->Add( WxCheckForceUseTabs, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckForceUseTabs );
	WxCheckIndentCase = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTCASE, _( "Indent case: statement in switches (commands under case:)" ), wxPoint( 7, 44 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentCase" ) );
	WxBoxSizer38->Add( WxCheckIndentCase, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentCase );
	WxCheckIndentClasses = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENETCLASSES, _( "Indent classes (keywords public:, protected: and private:)" ), wxPoint( 7, 65 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentClasses" ) );
	WxBoxSizer38->Add( WxCheckIndentClasses, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentClasses );
	WxCheckIndentLabels = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTLABELS, _( "Indent labels" ), wxPoint( 7, 86 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentLabels" ) );
	WxBoxSizer38->Add( WxCheckIndentLabels, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentLabels );
	WxCheckIndentModifiers = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTMODIFIERS, _( "Indent modifiers" ), wxPoint( 7, 107 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentModifiers" ) );
	WxBoxSizer38->Add( WxCheckIndentModifiers, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentModifiers );
	WxCheckIndentNamespaces = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTNS, _( "Indent namespaces" ), wxPoint( 7, 128 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentNamespaces" ) );
	WxBoxSizer38->Add( WxCheckIndentNamespaces, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentNamespaces );
	WxCheckIndentSwitches = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTSWITCHS, _( "Indent switches" ), wxPoint( 7, 149 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentSwitches" ) );
	WxBoxSizer38->Add( WxCheckIndentSwitches, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentSwitches );
	WxCheckIndentPreprocBlock = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTPREP, _( "Indent preprocessor blocks at bracket level zero, and within a namespace" ), wxPoint( 7, 170 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentPreprocBlock" ) );
	WxBoxSizer38->Add( WxCheckIndentPreprocBlock, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentPreprocBlock );
	WxCheckIndentPreprocDefine = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTMULTIPREP, _( "Indent multi-line preprocessor definitions ending with a backslash" ), wxPoint( 7, 191 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentPreprocDefine" ) );
	WxBoxSizer38->Add( WxCheckIndentPreprocDefine, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentPreprocDefine );
	WxCheckIndentPreprocCond = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENTPREPCOND, _( "Indent preprocessor conditionals" ), wxPoint( 7, 212 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentPreprocCond" ) );
	WxBoxSizer38->Add( WxCheckIndentPreprocCond, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxCheckIndentCol1Comments = new wxCheckBox( WxAuiNoteBookPage3, ID_WXCHECKINDENETCPPCOMMENTS, _( "Indent C++ comments beginning in column one" ), wxPoint( 7, 233 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckIndentCol1Comments" ) );
	WxBoxSizer38->Add( WxCheckIndentCol1Comments, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckIndentCol1Comments );
	WxBoxSizer40 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer39->Add( WxBoxSizer40, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxStaticText22 = new wxStaticText( WxAuiNoteBookPage3, ID_WXSTATICTEXT22, _( "Minimal indent added when a header is built of multiple lines:" ), wxPoint( 1, 3 ), wxDefaultSize, 0, wxT( "WxStaticText22" ) );
	WxBoxSizer40->Add( WxStaticText22, 0, wxALIGN_LEFT | wxALIGN_BOTTOM | wxALL, 1 );
	WxSpinMinConditionalEvent = new wxSpinCtrl( WxAuiNoteBookPage3, ID_WXSPINCTRLMININDENT, wxT( "2" ), wxPoint( 290, 1 ), wxSize( 60, 19 ), wxSP_ARROW_KEYS, 0, 3, 2 );
	WxBoxSizer40->Add( WxSpinMinConditionalEvent, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxSpinMinConditionalEvent );
	WxBoxSizer41 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer39->Add( WxBoxSizer41, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxStaticText23 = new wxStaticText( WxAuiNoteBookPage3, ID_WXSTATICTEXT23, _( "Maximum of # spaces to indent a continuation line (max. 120):" ), wxPoint( 1, 2 ), wxDefaultSize, 0, wxT( "WxStaticText23" ) );
	WxBoxSizer41->Add( WxStaticText23, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxEditMaxInStatementIndent = new wxTextCtrl( WxAuiNoteBookPage3, ID_WXEDITMAXINSTATEMENTINDENT, wxT( "40" ), wxPoint( 298, 1 ), wxSize( 60, 19 ), 0, wxTextValidator( wxFILTER_NUMERIC, NULL ), wxT( "WxEditMaxInStatementIndent" ) );
	WxBoxSizer41->Add( WxEditMaxInStatementIndent, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxEditMaxInStatementIndent );
	WxAuiNoteBookPage4 = new wxPanel( WxAuiNotebook1, ID_WXAUINOTEBOOKPAGE4, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxAuiNotebook1->AddPage( WxAuiNoteBookPage4, _( "Formatting" ) );
	WxBoxSizer42 = new wxBoxSizer( wxVERTICAL );
	WxAuiNoteBookPage4->SetSizer( WxBoxSizer42 );
	WxAuiNoteBookPage4->SetAutoLayout( true );
	WxCheckBreakClosing = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKBREAKCLOSING, _( "Break closing headers from their immediately preceding closing brackets" ), wxPoint( 2, 2 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBreakClosing" ) );
	WxBoxSizer42->Add( WxCheckBreakClosing, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBreakClosing );
	WxCheckBreakElseIfs = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKBREAKELSEIFS, _( "Break 'else if()' header combinations into seperate lines" ), wxPoint( 2, 23 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBreakElseIfs" ) );
	WxBoxSizer42->Add( WxCheckBreakElseIfs, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBreakElseIfs );
	WxCheckAddBrackets = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKADDBRACKETS, _( "Add brackets to unbracketed one line conditional statements" ), wxPoint( 2, 44 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckAddBrackets" ) );
	WxBoxSizer42->Add( WxCheckAddBrackets, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckAddBrackets );
	WxCheckAddOneLineBrackets = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKADDONELINEBRACKETS, _( "Add one line brackets to unbracketed one line conditional statements" ), wxPoint( 2, 65 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckAddOneLineBrackets" ) );
	WxBoxSizer42->Add( WxCheckAddOneLineBrackets, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckAddOneLineBrackets );
	WxCheckRemoveBrackets = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKBOX17, _( "Remove brackets from conditional statements (single statement on a single line)" ), wxPoint( 2, 86 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckRemoveBrackets" ) );
	WxBoxSizer42->Add( WxCheckRemoveBrackets, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckRemoveBrackets );
	WxCheckKeepBlocks = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKKEEPBLOCKS, _( "Don't break one-line blocks" ), wxPoint( 2, 107 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckKeepBlocks" ) );
	WxBoxSizer42->Add( WxCheckKeepBlocks, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckKeepBlocks );
	WxCheckKeepComplex = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKKEEPCOMPLEX, _( "Don't break complex statements and multiple statements residing in a single line" ), wxPoint( 2, 128 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckKeepComplex" ) );
	WxBoxSizer42->Add( WxCheckKeepComplex, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckKeepComplex );
	WxCheckConvertTabs = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKCONVERTTABS, _( "Convert TABs to spaces" ), wxPoint( 2, 149 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckConvertTabs" ) );
	WxBoxSizer42->Add( WxCheckConvertTabs, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckConvertTabs );
	WxCheckCloseTemplates = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKCLOSETEMPLATES, _( "Closes whitespace in the angle brackets of template definitions" ), wxPoint( 2, 170 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckCloseTemplates" ) );
	WxBoxSizer42->Add( WxCheckCloseTemplates, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckCloseTemplates );
	WxCheckRemoveCommentPrefix = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKREMOVECOMMENTPREFIX, _( "Remove the preceding '*' in a multi-line comment that begins a line" ), wxPoint( 2, 191 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckRemoveCommentPrefix" ) );
	WxBoxSizer42->Add( WxCheckRemoveCommentPrefix, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckRemoveCommentPrefix );
	WxCheckBreakLines = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKBREAKLINES, _( "Enable line breaking" ), wxPoint( 2, 212 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBreakLines" ) );
	WxBoxSizer42->Add( WxCheckBreakLines, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBreakLines );
	WxBoxSizer43 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer42->Add( WxBoxSizer43, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxBoxSizer43->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxStaticText24 = new wxStaticText( WxAuiNoteBookPage4, ID_WXSTATICTEXT24, _( "Break lines after amount of chars (range: 50-200)" ), wxPoint( 15, 2 ), wxDefaultSize, 0, wxT( "WxStaticText24" ) );
	WxBoxSizer43->Add( WxStaticText24, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxEditSFMaxLineLength = new wxTextCtrl( WxAuiNoteBookPage4, ID_WXEDITSFMAXLINELENGTH, wxT( "200" ), wxPoint( 247, 1 ), wxSize( 60, 19 ), 0, wxTextValidator( wxFILTER_NUMERIC, NULL ), wxT( "WxEditSFMaxLineLength" ) );
	WxEditSFMaxLineLength->Enable( false );
	WxBoxSizer43->Add( WxEditSFMaxLineLength, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxEditSFMaxLineLength );
	WxBoxSizer44 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer42->Add( WxBoxSizer44, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	WxBoxSizer44->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBreakAfterLogical = new wxCheckBox( WxAuiNoteBookPage4, ID_WXCHECKBREAKAFTERLOGICAL, _( "Break logical conditionals to be placed last on the previous line" ), wxPoint( 15, 1 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBreakAfterLogical" ) );
	WxCheckBreakAfterLogical->Enable( false );
	WxBoxSizer44->Add( WxCheckBreakAfterLogical, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	SET_CONTROLPARENT( WxCheckBreakAfterLogical );
	WxAuiNoteBookPage5 = new wxPanel( WxAuiNotebook1, ID_WXAUINOTEBOOKPAGE5, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxAuiNotebook1->AddPage( WxAuiNoteBookPage5, _( "Padding" ) );
	WxBoxSizer45 = new wxBoxSizer( wxVERTICAL );
	WxAuiNoteBookPage5->SetSizer( WxBoxSizer45 );
	WxAuiNoteBookPage5->SetAutoLayout( true );
	WxCheckBreakBlocks = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKBREAKBLOCKS, _( "Pad empty lines around header blocks (e.g. 'if', 'while'...)" ), wxPoint( 167, 2 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBreakBlocks" ) );
	WxBoxSizer45->Add( WxCheckBreakBlocks, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBreakBlocks );
	wxBoxSizer *BoxSizer01 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer45->Add( BoxSizer01, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 1 );
	BoxSizer01->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBreakBlocksAll = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKBREAKBLOCKSALL, _( "Treat closing header blocks (e.g. 'else', 'catch') as stand-alone blocks" ), wxPoint( 177, 23 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckBreakBlocksAll" ) );
	WxCheckBreakBlocksAll->Enable( false );
	BoxSizer01->Add( WxCheckBreakBlocksAll, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckBreakBlocksAll );
	WxCheckPadOperators = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKPADOPERATORS, _( "Insert space padding around operators" ), wxPoint( 167, 44 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckPadOperators" ) );
	WxBoxSizer45->Add( WxCheckPadOperators, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckPadOperators );
	WxCheckPadParensOut = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKPADPARENSOUT, _( "Insert space padding around parenthesies on the outside" ), wxPoint( 167, 65 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckPadParensOut" ) );
	WxBoxSizer45->Add( WxCheckPadParensOut, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckPadParensOut );
	WxCheckPadParensIn = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKPADPARENSIN, _( "Insert space padding around parenthesies on the inside" ), wxPoint( 167, 86 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckPadParensIn" ) );
	WxBoxSizer45->Add( WxCheckPadParensIn, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckPadParensIn );
	WxCheckPadHeader = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKPADHEADER, _( "Insert space padding between a header and the following parenthesis" ), wxPoint( 167, 107 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckPadHeader" ) );
	WxBoxSizer45->Add( WxCheckPadHeader, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckPadHeader );
	WxCheckUnpadParens = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKUNPADPARENS, _( "Remove extra space padding around parenthesis" ), wxPoint( 167, 128 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckUnpadParens" ) );
	WxBoxSizer45->Add( WxCheckUnpadParens, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckUnpadParens );
	WxCheckDelEmptyLine = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKDELEMPTYLINE, _( "Delete empty lines within a function or method" ), wxPoint( 167, 149 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckDelEmptyLine" ) );
	WxBoxSizer45->Add( WxCheckDelEmptyLine, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckDelEmptyLine );
	WxCheckFillEmptyLines = new wxCheckBox( WxAuiNoteBookPage5, ID_WXCHECKFILLEMPTYLINES, _( "Fill empty lines with the whitespace of their previous lines" ), wxPoint( 167, 170 ), wxSize( 600, 17 ), 0, wxDefaultValidator, wxT( "WxCheckFillEmptyLines" ) );
	WxBoxSizer45->Add( WxCheckFillEmptyLines, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxCheckFillEmptyLines );
	WxBoxSizer46 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer45->Add( WxBoxSizer46, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxStaticText25 = new wxStaticText( WxAuiNoteBookPage5, ID_WXSTATICTEXT25, _( "Pointer alignment:" ), wxPoint( 5, 7 ), wxDefaultSize, 0, wxT( "WxStaticText25" ) );
	WxBoxSizer46->Add( WxStaticText25, 0, wxALIGN_CENTER | wxALL, 2 );
	wxArrayString arrayStringFor_WxChoicePointerAlign;
	arrayStringFor_WxChoicePointerAlign.Add( _( "None" ) );
	arrayStringFor_WxChoicePointerAlign.Add( _( "Type" ) );
	arrayStringFor_WxChoicePointerAlign.Add( _( "Middle" ) );
	arrayStringFor_WxChoicePointerAlign.Add( _( "Name" ) );
	WxChoicePointerAlign = new wxChoice( WxAuiNoteBookPage5, ID_WXCHOICEPOINTERALIGN, wxPoint( 103, 5 ), wxSize( 104, 21 ), arrayStringFor_WxChoicePointerAlign, 0, wxDefaultValidator, wxT( "WxChoicePointerAlign" ) );
	WxChoicePointerAlign->SetSelection( 0 );
	WxBoxSizer46->Add( WxChoicePointerAlign, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxChoicePointerAlign );
	WxStaticText26 = new wxStaticText( WxAuiNoteBookPage5, ID_WXSTATICTEXT26, _( "Reference alignment" ), wxPoint( 217, 7 ), wxDefaultSize, 0, wxT( "WxStaticText26" ) );
	WxBoxSizer46->Add( WxStaticText26, 0, wxALIGN_CENTER | wxALL, 5 );
	wxArrayString arrayStringFor_WxChoiceReferenceAlign;
	arrayStringFor_WxChoiceReferenceAlign.Add( _( "None" ) );
	arrayStringFor_WxChoiceReferenceAlign.Add( _( "Type" ) );
	arrayStringFor_WxChoiceReferenceAlign.Add( _( "Middle" ) );
	arrayStringFor_WxChoiceReferenceAlign.Add( _( "Name" ) );
	WxChoiceReferenceAlign = new wxChoice( WxAuiNoteBookPage5, ID_WXCHOICEREFERENCEALIGN, wxPoint( 329, 5 ), wxSize( 87, 21 ), arrayStringFor_WxChoiceReferenceAlign, 0, wxDefaultValidator, wxT( "WxChoiceReferenceAlign" ) );
	WxChoiceReferenceAlign->SetSelection( 0 );
	WxBoxSizer46->Add( WxChoiceReferenceAlign, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5 );
	SET_CONTROLPARENT( WxChoiceReferenceAlign );
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
	WxAuiNoteBookPage6 = new wxPanel( WxAuiNotebook1, ID_WXAUINOTEBOOKPAGE6, wxPoint( 4, 24 ), wxSize( 692, 464 ) );
	WxAuiNotebook1->AddPage( WxAuiNoteBookPage6, wxT( "XML" ) );
	WxBoxSizer48 = new wxBoxSizer( wxVERTICAL );
	WxAuiNoteBookPage6->SetSizer( WxBoxSizer48 );
	WxAuiNoteBookPage6->SetAutoLayout( true );
	WxBoxSizer49 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer48->Add( WxBoxSizer49, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxStaticText28 = new wxStaticText( WxAuiNoteBookPage6, ID_WXSTATICTEXT28, _( "XML version:" ), wxPoint( 5, 6 ), wxDefaultSize, 0, wxT( "WxStaticText28" ) );
	WxBoxSizer49->Add( WxStaticText28, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditXMLversion = new wxTextCtrl( WxAuiNoteBookPage6, ID_WXEDITXMLVERSION, wxT( "1.0" ), wxPoint( 81, 5 ), wxSize( 49, 19 ), 0, wxTextValidator( wxFILTER_ASCII, NULL ), wxT( "WxEditXMLversion" ) );
	WxBoxSizer49->Add( WxEditXMLversion, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxEditXMLversion );
	WxBoxSizer50 = new wxBoxSizer( wxHORIZONTAL );
	WxBoxSizer48->Add( WxBoxSizer50, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxStaticText29 = new wxStaticText( WxAuiNoteBookPage6, ID_WXSTATICTEXT29, _( "Indentation size(in spaces):" ), wxPoint( 5, 6 ), wxDefaultSize, 0, wxT( "WxStaticText29" ) );
	WxBoxSizer50->Add( WxStaticText29, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	WxEditXmlIndentSize = new wxTextCtrl( WxAuiNoteBookPage6, ID_WXEDITXMLINDENTSIZE, wxT( "4" ), wxPoint( 147, 5 ), wxSize( 49, 19 ), 0, wxTextValidator( wxFILTER_NUMERIC, NULL ), wxT( "WxEditXmlIndentSize" ) );
	WxBoxSizer50->Add( WxEditXmlIndentSize, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2 );
	SET_CONTROLPARENT( WxEditXmlIndentSize );
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

	g_OptionsDialog = NULL;
	Destroy();
}


/*
 * MadOptionsDialogActivate
 */
void MadOptionsDialog::MadOptionsDialogActivate( wxActivateEvent& event )
{
	if( event.GetActive() )
	{
		if( FindFocus() == NULL )
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
	cfg->Read( wxT( "SingleInstance" ), &bb );
	WxCheckBoxSingleInstance->SetValue( bb );
	cfg->Read( wxT( "RecordCaretMovements" ), &bb );
	WxCheckBoxRecordCaretMovements->SetValue( bb );
	cfg->Read( wxT( "MaxSizeToLoad" ), &ll );
	WxEditMaxSizeToLoad->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxTextFileSize" ), &ll );
	WxEditMaxTextFileSize->SetValue( wxString() << ll );
	ss = _( "System Default" );
	cfg->Read( wxT( "DefaultEncoding" ), &ss );
	WxComboBoxEncoding->SetValue( ss );
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
	cfg->Read( wxT( "InsertSpacesInsteadOfTab" ), &bb );
	WxCheckBoxTabOrSpaces->SetValue( bb );
	cfg->Read( wxT( "AutoIndent" ), &bb );
	WxCheckBoxAutoIndent->SetValue( bb );
	cfg->Read( wxT( "AutoCompletePair" ), &bb );
	WxCheckBoxAutoCompletePair->SetValue( bb );
	cfg->Read( wxT( "MouseSelectToCopy" ), &bb );
	WxCheckBoxMouseSelectToCopy->SetValue( bb );
	WxCheckBoxCtrlWithMouseToSelect->Enable( bb );
	cfg->Read( wxT( "MouseSelectToCopyWithCtrlKey" ), &bb );
	WxCheckBoxCtrlWithMouseToSelect->SetValue( bb );
	cfg->Read( wxT( "MiddleMouseToPaste" ), &bb );
	WxCheckBoxMiddleMouseToPaste->SetValue( bb );
	cfg->Read( wxT( "AutoFillColumnPaste" ), &bb );
	WxCheckBoxAutoFillColumnPaste->SetValue( bb );
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

	g_SelectedCommandItem = NULL;
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

	cfg->SetPath( wxT( "/astyle" ) );
	WxRadioBoxBracketStyle->SetSelection( cfg->ReadLong( wxT( "style" ), aspsAllman ) );
	WxCheckAttachClasses->SetValue( cfg->ReadBool( wxT( "attach_classes" ), false ) );
	WxCheckAttachExternC->SetValue( cfg->ReadBool( wxT( "attach_extern_c" ), true ) );
	WxCheckAttachNamespaces->SetValue( cfg->ReadBool( wxT( "attach_namespaces" ), true ) );
	WxCheckAttachInlines->SetValue( cfg->ReadBool( wxT( "attach_inlines" ), true ) );
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
	WxSpinMinConditionalEvent->SetValue( cfg->ReadLong( wxT( "min_conditional_indent" ), 2 ) );
	WxEditMaxInStatementIndent->SetValue( wxString() << cfg->ReadLong( wxT( "max_instatement_indent" ), 40 ) );
	WxCheckBreakClosing->SetValue( cfg->ReadBool( wxT( "break_closing" ), true ) );
	WxCheckBreakElseIfs->SetValue( cfg->ReadBool( wxT( "break_elseifs" ), true ) );
	WxCheckAddBrackets->SetValue( cfg->ReadBool( wxT( "add_brackets" ), false ) );
	WxCheckAddOneLineBrackets->SetValue( cfg->ReadBool( wxT( "add_one_line_brackets" ), true ) );
	WxCheckKeepComplex->SetValue( cfg->ReadBool( wxT( "keep_complex" ), true ) );
	WxCheckRemoveBrackets->SetValue( cfg->ReadBool( wxT( "remove_brackets" ), false ) );
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
		wxLogError( errtext, WxStaticText1->GetLabel().c_str(), WxEditMaxSizeToLoad->GetValue().c_str() );
		error = true;
	}

	if( !WxEditMaxTextFileSize->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext, WxStaticText2->GetLabel().c_str(), WxEditMaxTextFileSize->GetValue().c_str() );
		error = true;
	}

#if PATCH_MAXLINELENGTH == 1

	if( !WxEditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 || lo > 4096 )
#else
	if( !WxEditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 )
#endif
	{
		wxLogError( errtext, WxStaticText3->GetLabel().c_str(), WxEditMaxLineLength->GetValue().c_str() );
		error = true;
	}

	if( !WxEditMaxColumns->GetValue().ToLong( &lo ) || lo <= 0 )
	{
		wxLogError( errtext, WxStaticText4->GetLabel().c_str(), WxEditMaxColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditTabColumns->GetValue().ToLong( &lo ) || lo <= 0 )
	{
		wxLogError( errtext, WxStaticText5->GetLabel().c_str(), WxEditTabColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditIndentColumns->GetValue().ToLong( &lo ) || lo <= 0 )
	{
		wxLogError( errtext, WxStaticText6->GetLabel().c_str(), WxEditIndentColumns->GetValue().c_str() );
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
	wxTextCtrl *edit = NULL;

	switch( ButtonID )
	{
	case ID_WXBUTTON1: edit = WxEditHeaderLeft; break;

	case ID_WXBUTTON2: edit = WxEditHeaderCenter; break;

	case ID_WXBUTTON3: edit = WxEditHeaderRight; break;

	case ID_WXBUTTON4: edit = WxEditFooterLeft; break;

	case ID_WXBUTTON5: edit = WxEditFooterCenter; break;

	case ID_WXBUTTON6: edit = WxEditFooterRight; break;
	}

	if( edit != NULL && str[0] == wxT( '[' ) && str[3] == wxT( ']' ) )
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

	if( g_SelectedCommandItem == NULL )
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

	return NULL;
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
	g_CommandItemOfNewKey = NULL;

	if( scstr.IsEmpty() )
	{
		g_OptionsDialog->WxEditKeyHint->Clear();
	}
	else
	{
		// find the key is assigned to which command
		TreeItemData *tid = g_OptionsDialog->FindKeyInList( scstr );

		if( tid == NULL )
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
	if( g_SelectedCommandItem != NULL && g_SelectedCommandItem != g_CommandItemOfNewKey )
	{
		wxString key = WxEditKey->GetValue();

		if( g_CommandItemOfNewKey != NULL ) // new key is assigned to another command
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
	if( g_SelectedCommandItem != NULL && g_SelectedKeyId >= 0 )
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
	if( g_SelectedCommandItem != NULL && g_SelectedCommandItem->cmddata->menu_id > 0 && g_SelectedKeyId > 0 )
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

void MadOptionsDialog::OnRadioBoxBracketStyleClick( wxCommandEvent& event )
{
	long style = WxRadioBoxBracketStyle->GetSelection();
#ifdef MADEDIT_ENABLE_STC
	WxTextSample->SetReadOnly( false );
#endif

	switch( style )
	{
	case aspsAllman: // Allman (ANSI)
	case aspsJava: // Java
	case aspsKr: // K&R
	case aspsStroustrup: // Stroustrup
	case aspsWhitesmith: // Whitesmith
	case aspsVTK: // VTK
	case aspsBanner: // Banner
	case aspsGnu: // GNU
	case aspsLinux: // Linux
	case aspsHorstmann: // Horstmann
	case asps1TBS: // 1TBS
	case aspsGoogle: // Google
	case aspsPico: // Pico
	case aspsLisp: // Lisp
#ifndef MADEDIT_ENABLE_MADEDIT
		WxTextSample->SetValue( bracket_style[style] );
#else
		WxTextSample->SetText(bracket_style[style]);
#endif
		break;

	default: // Custom
#ifndef MADEDIT_ENABLE_MADEDIT
		WxTextSample->SetValue(bracket_style[aspsCustom]);
#else
		WxTextSample->SetText(bracket_style[aspsCustom]);
#endif
		break;
	}

#ifdef MADEDIT_ENABLE_STC
	WxTextSample->SetReadOnly( true );
#endif
}

void MadOptionsDialog::OnFormattingBreakLinesClick( wxCommandEvent& event )
{
	if( WxCheckBreakLines->GetValue() )
	{
		WxEditSFMaxLineLength->Enable( true );
		WxCheckBreakAfterLogical->Enable( true );
	}
	else
	{
		WxEditSFMaxLineLength->Enable( false );
		WxCheckBreakAfterLogical->Enable( false );
	}
}

void MadOptionsDialog::OnPaddingBreakBlocksClick( wxCommandEvent& event )
{
	if( WxCheckBreakBlocks->GetValue() )
	{
		WxCheckBreakBlocksAll->Enable( true );
	}
	else
	{
		WxCheckBreakBlocksAll->Enable( false );
	}
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
