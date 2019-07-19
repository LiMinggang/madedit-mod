///////////////////////////////////////////////////////////////////////////////
// Name:        MadSearchReplaceDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>
#include <wx/longlong.h>
#include <wx/progdlg.h>
#include <wx/fileconf.h>

#include "EmbeddedPython.hpp"
#include "MadEditFrame.h"
#include "MadSearchReplaceDialog.h"
#include "MadRecentList.h"
#include "MadUtils.h"

#include "MadEdit/MadEdit.h"
#include "MadNumberDlg.h"
#include "MadSortDialog.h"

extern MadEdit *g_ActiveMadEdit;
extern void OnEditMouseRightUp( MadEdit * madedit );
extern wxMenu *g_Menu_EditPop;
extern MadNumberDlg * g_MadNumberDlg;

#ifdef _MSC_VER
	# pragma warning( push )
	# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(MadSearchReplaceDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
	# pragma warning( pop )
#endif

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

MadSearchReplaceDialog::wxCmdEvtHandlerMap_t MadSearchReplaceDialog::m_menu_evt_map[] =
{
	// edit
	{ menuUndo, &MadSearchReplaceDialog::OnEditUndo },
	{ menuRedo, &MadSearchReplaceDialog::OnEditRedo },
	{ menuCut, &MadSearchReplaceDialog::OnEditCut },
	{ menuCopy, &MadSearchReplaceDialog::OnEditCopy },
	{ menuPaste, &MadSearchReplaceDialog::OnEditPaste },
	{ menuDelete, &MadSearchReplaceDialog::OnEditDelete },
	{ menuCutLine, &MadSearchReplaceDialog::OnEditCutLine },
	{ menuDeleteLine, &MadSearchReplaceDialog::OnEditDeleteLine },
	{ menuSelectAll, &MadSearchReplaceDialog::OnEditSelectAll },
	{ menuStartEndSelction, &MadSearchReplaceDialog::OnEditStartEndSelction },
	{ menuInsertTabChar, &MadSearchReplaceDialog::OnEditInsertTabChar },
	{ menuInsertDateTime, &MadSearchReplaceDialog::OnEditInsertDateTime },
	{ menuSortAscending, &MadSearchReplaceDialog::OnEditSortAscending },
	{ menuSortDescending, &MadSearchReplaceDialog::OnEditSortDescending },
	{ menuSortAscendingCase, &MadSearchReplaceDialog::OnEditSortAscendingCase },
	{ menuSortDescendingCase, &MadSearchReplaceDialog::OnEditSortDescendingCase },
	{ menuSortByOptions, &MadSearchReplaceDialog::OnEditSortByOptions },
	{ menuSortOptions, &MadSearchReplaceDialog::OnEditSortOptions },
	{ menuCopyAsHexString, &MadSearchReplaceDialog::OnEditCopyAsHexString },
	{ menuCopyAsHexStringWithSpace, &MadSearchReplaceDialog::OnEditCopyAsHexStringWithSpace },
	{ menuCopyRevertHex, &MadSearchReplaceDialog::OnEditCopyRevertHex },
	{ menuIncreaseIndent, &MadSearchReplaceDialog::OnEditIncIndent },
	{ menuDecreaseIndent, &MadSearchReplaceDialog::OnEditDecIndent },
	{ menuComment, &MadSearchReplaceDialog::OnEditComment },
	{ menuUncomment, &MadSearchReplaceDialog::OnEditUncomment },
	{ menuWordWrapToNewLine, &MadSearchReplaceDialog::OnEditWordWrapToNewLine },
	{ menuNewLineToWordWrap, &MadSearchReplaceDialog::OnEditNewLineToWordWrap },
	{ menuToUpperCase, &MadSearchReplaceDialog::OnEditToUpperCase },
	{ menuToLowerCase, &MadSearchReplaceDialog::OnEditToLowerCase },
	{ menuInvertCase, &MadSearchReplaceDialog::OnEditInvertCase },
	{ menuCapitalize, &MadSearchReplaceDialog::OnEditCapitalize },
	{ menuToHalfWidth, &MadSearchReplaceDialog::OnEditToHalfWidth },
	{ menuToHalfWidthByOptions, &MadSearchReplaceDialog::OnEditToHalfWidthByOptions },
	{ menuToFullWidth, &MadSearchReplaceDialog::OnEditToFullWidth },
	{ menuToFullWidthByOptions, &MadSearchReplaceDialog::OnEditToFullWidthByOptions },
	{ menuTabToSpace, &MadSearchReplaceDialog::OnEditTabToSpace },
	{ menuSpaceToTab, &MadSearchReplaceDialog::OnEditSpaceToTab },
	{ menuTrimTrailingSpaces, &MadSearchReplaceDialog::OnEditTrimTrailingSpaces },
	{ menuTrimLeadingSpaces, &MadSearchReplaceDialog::OnEditTrimLeadingSpaces },
	{ menuDeleteEmptyLines, &MadSearchReplaceDialog::OnEditDeleteEmptyLines },
	{ menuDeleteEmptyLinesWithSpaces, &MadSearchReplaceDialog::OnEditDeleteEmptyLinesWithSpaces },
	{ menuJoinLines, &MadSearchReplaceDialog::OnEditJoinLines },
	{ menuInsertNumbers, &MadSearchReplaceDialog::OnEditInsertNumbers },
	{ menuColumnAlignLeft, &MadSearchReplaceDialog::OnEditColumnAlignLeft },
	{ menuColumnAlignRight, &MadSearchReplaceDialog::OnEditColumnAlignRight },
};

MadSearchReplaceDialog::wxCmdEvtHandlerRangeMap_t MadSearchReplaceDialog::m_menu_evt_range_map[] =
{
	{ menuSpellOption1, menuSpellOption99, &MadSearchReplaceDialog::OnEditSpellCheck },
	{ menuMadScrip1, menuMadScrip200, &MadSearchReplaceDialog::OnToolsMadScriptList },
};

extern EmbeddedPython *g_EmbeddedPython;
extern MadEditFrame *g_MainFrame;
extern wxArrayString g_SpellSuggestions;
extern wxString g_MadEditAppDir;
extern wxString g_MadEditHomeDir;
extern wxMenu *g_Menu_MadMacro_Scripts;

extern int MadMessageBox( const wxString& message,
						  const wxString& caption = wxMessageBoxCaptionStr,
						  long style = wxOK | wxCENTRE,
						  wxWindow *parent = nullptr,
						  int x = wxDefaultCoord, int y = wxDefaultCoord );

#include "../images/down.xpm"

extern wxStatusBar *g_StatusBar;   // add: gogo, 19.09.2009
extern MadRecentList * g_RecentFindText;
MadSearchReplaceDialog *g_SearchReplaceDialog = nullptr;
extern bool IsMacroRecording();
extern void RecordAsMadMacro( MadEdit *, const wxString&, bool = false );
extern MadEdit *g_ActiveMadEdit;
extern MadEdit *g_CurrentMadEdit;
wxProgressDialog *g_SearchProgressDialog = nullptr;

bool OnSearchProgressUpdate( int value, const wxString &newmsg = wxEmptyString, bool *skip = nullptr )
{
	if( g_SearchProgressDialog == nullptr )
		return true;

	return g_SearchProgressDialog->Update( value, newmsg, skip );
}
//(*IdInit(MadSearchReplaceDialog)
//*)

const long MadSearchReplaceDialog::ID_RECENTFINDTEXT1 = MadUniqueIDReserver::Instance().RecentFindTextID1();    // must be the same with MadSearchDialog
const long MadSearchReplaceDialog::ID_RECENTFINDTEXT20 = MadUniqueIDReserver::Instance().RecentFindTextID20();
const long MadSearchReplaceDialog::ID_RECENTREPLACETEXT1 = MadUniqueIDReserver::Instance().RecentReplaceTextID1();
const long MadSearchReplaceDialog::ID_RECENTREPLACETEXT20 = MadUniqueIDReserver::Instance().RecentReplaceTextID20();

//----------------------------------------------------------------------------
// MadSearchReplaceDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate Block.
// Code added in  other places will be removed by wx-dvcpp
////Event Table Start

inline void MadSearchEscapeString(wxString & expr, bool bRegex)
{
	const wxUniChar esp('\\');	
	if(!bRegex)
	{
		expr.Replace( wxT( "\\" ), wxT( "\\\\" ) );
	}
	else
		expr.Replace( wxT( "\\\\" ), wxT( "\\\\\\\\" ) );

	for( size_t i  = 0; i < expr.Length(); ++i )
	{
		if((wxUint32)(expr[i].GetValue()) == (wxUint32)('"'))
		{
			expr.insert( i, 1, esp );
			if (bRegex && (i > 0) && ((wxUint32)(expr[i - 1].GetValue()) == (wxUint32)('\\')))
			{
				expr.insert(i, 1, esp);
				++i;
			}

			++i;
		}
	}
}

MadSearchReplaceDialog::MadSearchReplaceDialog( wxWindow* parent, wxWindowID id, const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size) )
{
	m_EnableTransparency = true;/*CanSetTransparent();*/
	//(*Initialize(MadSearchReplaceDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize( wxDefaultSize );
	Move( wxDefaultPosition );
	BoxSizer1 = new wxBoxSizer( wxVERTICAL );
	BoxSizer8 = new wxBoxSizer( wxHORIZONTAL );
	BoxSizer2 = new wxBoxSizer( wxVERTICAL );
	BoxSizerSearch = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizerSearch, 1, wxALL|wxEXPAND, 0);
	BoxSizerReplace = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizerReplace, 1, wxALL|wxEXPAND, 0);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxMoveFocus = new wxCheckBox(this, wxID_ANY, _("&Move Focus to Editor Window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxMoveFocus->SetValue(false);
	BoxSizer6->Add(WxCheckBoxMoveFocus, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxCaseSensitive = new wxCheckBox(this, wxID_ANY, _("&Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxCaseSensitive->SetValue(false);
	BoxSizer6->Add(WxCheckBoxCaseSensitive, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxWholeWord = new wxCheckBox(this, wxID_ANY, _("&Whole Word Only"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxWholeWord->SetValue(false);
	BoxSizer6->Add(WxCheckBoxWholeWord, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxPanChinese = new wxCheckBox(this, wxID_ANY, _("Pan Chinese"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxPanChinese->SetValue(false);
	BoxSizer6->Add(WxCheckBoxPanChinese, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxRegex = new wxCheckBox(this, wxID_ANY, _("Use Regular E&xpressions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxRegex->SetValue(false);
	BoxSizer6->Add(WxCheckBoxRegex, 0, wxALL|wxEXPAND, 2);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer7->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBoxDotMatchNewLine = new wxCheckBox(this, wxID_ANY, _("&. Matches Newline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxDotMatchNewLine->SetValue(false);
	BoxSizer7->Add(WxCheckBoxDotMatchNewLine, 0, wxALL|wxEXPAND, 2);
	BoxSizer6->Add(BoxSizer7, 0, wxALL|wxEXPAND, 0);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	wxCheckBoxReplaceNoDoubleCheck = new wxCheckBox(this, wxID_ANY, _("Replace Directly(Fix Regex dilema)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	wxCheckBoxReplaceNoDoubleCheck->SetValue(false);
	BoxSizer4->Add(wxCheckBoxReplaceNoDoubleCheck, 0, wxALL|wxEXPAND, 2);
	BoxSizer6->Add(BoxSizer4, 0, wxALL|wxEXPAND, 0);
	WxCheckBoxFindHex = new wxCheckBox(this, wxID_ANY, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxFindHex->SetValue(false);
	BoxSizer6->Add(WxCheckBoxFindHex, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxSearchInSelection = new wxCheckBox(this, wxID_ANY, _("Search In &Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxSearchInSelection->SetValue(false);
	BoxSizer6->Add(WxCheckBoxSearchInSelection, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxWrapAround = new wxCheckBox(this, wxID_ANY, _("Wrap aroun&d"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxWrapAround->SetValue(false);
	BoxSizer6->Add(WxCheckBoxWrapAround, 0, wxALL|wxEXPAND, 2);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Bookmark in Searching"));
	WxCheckBoxBookmarkLine = new wxCheckBox(this, wxID_ANY, _("Bookmark line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxBookmarkLine->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxBookmarkLine, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxBookmarkOnly = new wxCheckBox(this, wxID_ANY, _("Bookmark only(Find All in Current)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxBookmarkOnly->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxBookmarkOnly, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxPurgeBookmark = new wxCheckBox(this, wxID_ANY, _("Purge mark for each search session"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	WxCheckBoxPurgeBookmark->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPurgeBookmark, 0, wxALL|wxEXPAND, 2);
	BoxSizer9->Add(StaticBoxSizer1, 0, wxALL|wxALIGN_TOP, 2);

	if( m_EnableTransparency )
	{
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Transparency"));
	WxRadioLosingFocus = new wxRadioButton(this, wxID_ANY, _("On Losing Focus"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	StaticBoxSizer2->Add(WxRadioLosingFocus, 0, wxALL|wxEXPAND, 2);
	WxRadioAlways = new wxRadioButton(this, wxID_ANY, _("Always"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	StaticBoxSizer2->Add(WxRadioAlways, 0, wxALL|wxEXPAND, 2);
		WxSliderTransDegree = new wxSlider( this, wxID_ANY, 255, 25, 255, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXSLIDERTRANSDEGREE" ) );
	StaticBoxSizer2->Add(WxSliderTransDegree, 0, wxALL|wxEXPAND, 2);
	BoxSizer9->Add(StaticBoxSizer2, 0, wxALL|wxALIGN_TOP, 2);
	}
	else
	{
		StaticBoxSizer2 = nullptr;
		WxRadioLosingFocus = nullptr;
		WxRadioAlways = nullptr;
		WxSliderTransDegree = nullptr;
	}

	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Input Sizer"));
	WxSliderInputSizer = new wxSlider(this, wxID_ANY, 1, 1, 5, wxDefaultPosition, wxSize(69,-1), 0, wxDefaultValidator, _T("ID_WXSLIDERINPUTSIZER"));
	WxSliderInputSizer->SetPageSize(1);
	WxSliderInputSizer->SetLineSize(1);
	StaticBoxSizer3->Add(WxSliderInputSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer9->Add(StaticBoxSizer3, 0, wxALL|wxALIGN_TOP, 2);

	BoxSizer6->Add( BoxSizer9, 1, wxALL | wxEXPAND, 2 );
	BoxSizer2->Add( BoxSizer6, 0, wxALL | wxEXPAND, 0 );
	BoxSizer8->Add( BoxSizer2, 1, wxALL | wxALIGN_TOP, 0 );
	BoxSizer3 = new wxBoxSizer( wxVERTICAL );
	WxButtonFindNext = new wxButton( this, wxID_ANY, _( "Find &Next" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONFINDNEXT" ) );
	WxButtonFindNext->SetDefault();
	BoxSizer3->Add( WxButtonFindNext, 0, wxALL | wxEXPAND, 2 );
	WxButtonFindPrev = new wxButton( this, wxID_ANY, _( "Find &Previous" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONFINDPREV" ) );
	BoxSizer3->Add( WxButtonFindPrev, 0, wxALL | wxEXPAND, 2 );
	WxButtonFindAll = new wxButton( this, wxID_ANY, _( "Find A&ll" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONFINDALL" ) );
	BoxSizer3->Add( WxButtonFindAll, 0, wxALL | wxEXPAND, 2 );
	WxButtonFindAllInAll = new wxButton( this, wxID_ANY, _( "Find All in All\nOpened" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONFINDALLINALL" ) );
	BoxSizer3->Add( WxButtonFindAllInAll, 0, wxALL | wxEXPAND, 2 );
	WxButtonReplace = new wxButton( this, wxID_ANY, _( "&Replace" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONREPLACE" ) );
	BoxSizer3->Add( WxButtonReplace, 0, wxALL | wxEXPAND, 2 );
	WxButtonReplaceAll = new wxButton( this, wxID_ANY, _( "Replace &All" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONREPLACEALL" ) );
	BoxSizer3->Add( WxButtonReplaceAll, 0, wxALL | wxEXPAND, 2 );
	WxButtonReplaceAllInAll = new wxButton( this, wxID_ANY, _( "Repl All in All\nOpened" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONREPLACEALLINALL" ) );
	BoxSizer3->Add( WxButtonReplaceAllInAll, 0, wxALL | wxEXPAND, 2 );
	WxButtonCount = new wxButton( this, wxID_ANY, _( "C&ount" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONCOUNT" ) );
	BoxSizer3->Add( WxButtonCount, 0, wxALL | wxEXPAND, 2 );
	WxButtonReplaceExpand = new wxButton( this, wxID_ANY, _( "R&eplace >>" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "ID_WXBUTTONREPLACEEXPAND" ) );
	BoxSizer3->Add( WxButtonReplaceExpand, 0, wxALL | wxEXPAND, 2 );
	WxButtonClose = new wxButton( this, wxID_CANCEL, _( "Close" ), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T( "wxID_CANCEL" ) );
	BoxSizer3->Add( WxButtonClose, 0, wxALL | wxEXPAND, 2 );
	BoxSizer8->Add( BoxSizer3, 0, wxALL | wxEXPAND, 0 );
	BoxSizer1->Add( BoxSizer8, 1, wxALL | wxEXPAND, 0 );
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

#if 0
	Connect(ID_WXCHECKBOXREGEX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxCheckBoxRegexClick);
	Connect(ID_WXCHECKBOXFINDHEX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxCheckBoxFindHexClick);
	Connect(ID_WXCHECKBOXSEARCHINSELECTION,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxCheckBoxSearchInSelectionClick);
	Connect(ID_WXCHECKBOXBOOKMARKLINE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxCheckBoxBookmarkLineClick);
	Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxRadioLosingFocusSelect);
	Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxRadioAlwaysSelect);
	Connect(ID_WXSLIDERTRANSDEGREE,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxSliderTransDegreeScroll);
	Connect(ID_WXSLIDERINPUTSIZER,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&MadSearchReplaceDialog::OnWxSliderInputSizerCmdScroll);
	Connect(ID_WXBUTTONFINDNEXT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonFindNextClick);
	Connect(ID_WXBUTTONFINDPREV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonFindPrevClick);
	Connect(ID_WXBUTTONFINDALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonFindAllClick);
	Connect(ID_WXBUTTONFINDALLINALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonFindAllInAllClick);
	Connect(ID_WXBUTTONREPLACE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonReplaceClick);
	Connect(ID_WXBUTTONREPLACEALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonReplaceAllClick);
	Connect(ID_WXBUTTONREPLACEALLINALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonReplaceAllInAllClick);
	Connect(ID_WXBUTTONCOUNT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonCountClick);
	Connect(ID_WXBUTTONREPLACEEXPAND,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonReplaceExpandClick);
	Connect(wxID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSearchReplaceDialog::WxButtonCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadSearchReplaceDialog::MadSearchReplaceDialogClose);
	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown);
#endif
	//*)
	int bw, bh;
	// find
	WxButtonFindNext->GetSize( &bw, &bh );
	m_OriginInputSize = wxSize( 400, bh );
	m_FindText = new MadEdit( this, wxID_ANY, wxPoint( 0, 0 ), m_OriginInputSize );
	m_FindText->SetSingleLineMode( true );

	if( g_ActiveMadEdit )
		m_FindText->SetEncoding( g_ActiveMadEdit->GetEncodingName() );
	else
		m_FindText->SetEncoding( wxT( "UTF-32LE" ) );

	m_FindText->SetFixedWidthMode( false );
	m_FindText->SetRecordCaretMovements( false );
	m_FindText->SetInsertSpacesInsteadOfTab( false );
	m_FindText->SetWantTab( false );
	MadSyntax* syn = m_FindText->GetSyntax();
	syn->DuplicateAttributes();
	m_FindText->LoadDefaultSyntaxScheme();
	m_FindText->SetMaxLineLength( DEFAULT_MAX_LINELEN );
	m_FindText->SetShowEndOfLine(false);
	m_FindText->SetShowTabChar( true );
	BoxSizerSearch->Add( m_FindText, 1, wxEXPAND | wxALIGN_LEFT | wxALL, 2 );
	BoxSizerSearch->SetItemMinSize( m_FindText, 400, bh );
	wxBitmap WxBitmapButtonRecentFindText_BITMAP( down_xpm );
	WxBitmapButtonRecentFindText = new wxBitmapButton( this, wxID_ANY, WxBitmapButtonRecentFindText_BITMAP, wxPoint( 0, 0 ), wxSize( bh, bh ), wxBU_AUTODRAW, wxDefaultValidator, _T( "WxBitmapButtonRecentFindText" ) );
	BoxSizerSearch->Add( WxBitmapButtonRecentFindText, 0, wxALL, 2 );
	// replace
	WxButtonReplace->GetSize( &bw, &bh );
	m_ReplaceText = new MadEdit( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 400, bh ) );
	m_ReplaceText->SetSingleLineMode( true );

	if( g_ActiveMadEdit )
		m_ReplaceText->SetEncoding( g_ActiveMadEdit->GetEncodingName() );
	else
		m_ReplaceText->SetEncoding( wxT( "UTF-32LE" ) );

	m_ReplaceText->SetFixedWidthMode( false );
	m_ReplaceText->SetRecordCaretMovements( false );
	m_ReplaceText->SetInsertSpacesInsteadOfTab( false );
	m_ReplaceText->SetWantTab( false );
	syn = m_ReplaceText->GetSyntax();
	syn->DuplicateAttributes();
	m_ReplaceText->LoadDefaultSyntaxScheme();
	m_ReplaceText->SetMaxLineLength( DEFAULT_MAX_LINELEN );
	m_ReplaceText->SetShowEndOfLine(false);
	m_ReplaceText->SetShowTabChar( true );
	BoxSizerReplace->Add( m_ReplaceText, 1, wxEXPAND | wxALIGN_LEFT | wxALL, 2 );
	BoxSizerReplace->SetItemMinSize( m_ReplaceText, 400, bh );
	WxBitmapButtonRecentReplaceText = new wxBitmapButton( this, wxID_ANY, WxBitmapButtonRecentFindText_BITMAP, wxPoint( 0, 0 ), wxSize( bh, bh ), wxBU_AUTODRAW, wxDefaultValidator, _T( "WxBitmapButtonRecentReplaceText" ) );
	BoxSizerReplace->Add( WxBitmapButtonRecentReplaceText, 0, wxALL, 2 );

	// connect to KeyDown event handler
	m_FindText->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	m_ReplaceText->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxBitmapButtonRecentFindText->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxBitmapButtonRecentReplaceText->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxMoveFocus->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxCaseSensitive->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxWholeWord->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxRegex->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxFindHex->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxWrapAround->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxSearchInSelection->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxBookmarkLine->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxBookmarkOnly->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxCheckBoxPurgeBookmark->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	////WxEditFrom->Bind(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	////WxEditTo->Bind(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	//WxButtonFindNext->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxButtonFindPrev->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxButtonFindAll->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxButtonReplace->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxButtonReplaceAll->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxButtonCount->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	//WxButtonClose->Bind( wxEVT_KEY_DOWN, &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this );
	m_RecentFindText = g_RecentFindText; //new MadRecentList(20, ID_RECENTFINDTEXT1, true);
	m_RecentFindText->UseMenu( &WxPopupMenuRecentFindText );
	m_RecentFindText->AddFilesToMenu();
	m_RecentReplaceText = new MadRecentList( 20, ID_RECENTREPLACETEXT1, true, false );
	m_RecentReplaceText->UseMenu( &WxPopupMenuRecentReplaceText );
	wxConfigBase *m_Config = wxConfigBase::Get( false );
	wxString oldpath = m_Config->GetPath();
	// add: gogo, 19.09.2009
	long x = 480, y = 100;
	m_Config->Read( wxT( "/UIView/SearchWinLeft" ), &x );
	m_Config->Read( wxT( "/UIView/SearchWinTop" ), &y );
	m_Config->SetPath( wxT( "/RecentReplaceText" ) );
	m_RecentReplaceText->Load( *m_Config );
	m_Config->SetPath( oldpath );
	SetPosition( wxPoint( x, y ) );
	bool bb;
	// I prefer to read it here instead in ReadWriteSettings(), because here
	// it will be read once per session instead of every 'Ctrl + F'
	m_Config->Read( wxT( "/MadEdit/SearchWrapAround" ), &bb,   false );
	WxCheckBoxWrapAround->SetValue( bb );

	//--------
	if( m_EnableTransparency )
	{
		wxASSERT( WxRadioAlways != 0 );
		wxASSERT( WxRadioLosingFocus != 0 );
		wxASSERT( WxSliderTransDegree != 0 );
		long trans = 30;

		m_Config->Read( wxT( "/UIView/AlwaysTransparent" ), &bb, false );
		m_Config->Read( wxT( "/UIView/Transparency" ), &trans );

		WxRadioAlways->SetValue( bb );
		WxRadioLosingFocus->SetValue( !bb );
		WxSliderTransDegree->SetValue( trans );

		if( bb )
		{
			SetTransparent( trans );
		}
	}

	if( m_RecentFindText->GetCount() > 0 )
	{
		wxString text = m_RecentFindText->GetHistoryFile( 0 );

		if( !text.IsEmpty() )
		{
			m_FindText->SetText( text );
		}
	}

	if( m_RecentReplaceText->GetCount() > 0 )
	{
		wxString text = m_RecentReplaceText->GetHistoryFile( 0 );

		if( !text.IsEmpty() )
		{
			m_ReplaceText->SetText( text );
		}
	}

	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,     &MadSearchReplaceDialog::WxCheckBoxRegexClick, this,             WxCheckBoxRegex->GetId() );
	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,     &MadSearchReplaceDialog::WxCheckBoxFindHexClick, this,           WxCheckBoxFindHex->GetId() );
	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,     &MadSearchReplaceDialog::WxCheckBoxSearchInSelectionClick, this, WxCheckBoxSearchInSelection->GetId() );
	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED,     &MadSearchReplaceDialog::WxCheckBoxBookmarkLineClick, this,      WxCheckBoxBookmarkLine->GetId() );
	if( m_EnableTransparency )
	{
		Bind( wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MadSearchReplaceDialog::WxRadioLosingFocusSelect, this,         WxRadioLosingFocus->GetId() );
		Bind( wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MadSearchReplaceDialog::WxRadioAlwaysSelect, this,              WxRadioAlways->GetId() );
		Bind( wxEVT_SLIDER,       &MadSearchReplaceDialog::WxSliderTransDegreeScroll, this,        WxSliderTransDegree->GetId() );
	}
	Bind( wxEVT_SLIDER,       &MadSearchReplaceDialog::OnWxSliderInputSizerCmdScroll, this,    WxSliderInputSizer->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonFindNextClick, this,            WxButtonFindNext->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonFindPrevClick, this,            WxButtonFindPrev->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonFindAllClick, this,             WxButtonFindAll->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonFindAllInAllClick, this,        WxButtonFindAllInAll->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonReplaceClick, this,             WxButtonReplace->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonReplaceAllClick, this,          WxButtonReplaceAll->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonReplaceAllInAllClick, this,     WxButtonReplaceAllInAll->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonCountClick, this,               WxButtonCount->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonReplaceExpandClick, this,       WxButtonReplaceExpand->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED,       &MadSearchReplaceDialog::WxButtonCloseClick, this,               wxID_CANCEL                     );
	Bind( wxEVT_CLOSE_WINDOW,                 &MadSearchReplaceDialog::MadSearchReplaceDialogClose, this);
	Bind( wxEVT_KEY_DOWN,                     &MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown, this);
	
	Bind( wxEVT_ACTIVATE, &MadSearchReplaceDialog::MadSearchReplaceDialogActivate, this );

	Bind( wxEVT_BUTTON, &MadSearchReplaceDialog::WxBitmapButtonRecentFindTextClick, this, WxBitmapButtonRecentFindText->GetId() );
	Bind( wxEVT_BUTTON, &MadSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick, this, WxBitmapButtonRecentReplaceText->GetId() );
	Bind( wxEVT_MENU, &MadSearchReplaceDialog::OnRecentFindText, this, ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20 );
	Bind( wxEVT_MENU, &MadSearchReplaceDialog::OnRecentReplaceText, this, ID_RECENTREPLACETEXT1, ID_RECENTREPLACETEXT20 );

	for(size_t i = 0; i < sizeof(m_menu_evt_map)/sizeof(m_menu_evt_map[0]); ++i)
	{
		Bind( wxEVT_MENU, m_menu_evt_map[i].method, this, m_menu_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_menu_evt_range_map)/sizeof(m_menu_evt_range_map[0]); ++i)
	{
		Bind( wxEVT_MENU, m_menu_evt_range_map[i].method, this, m_menu_evt_range_map[i].evtStartTag, m_menu_evt_range_map[i].evtEndTag );
	}

	m_FindText->SetOnMouseRightUp( &OnEditMouseRightUp );
	m_FindText->SetRightClickMenu( g_Menu_EditPop );
	m_ReplaceText->SetOnMouseRightUp( &OnEditMouseRightUp );
	m_ReplaceText->SetRightClickMenu( g_Menu_EditPop );
}

MadSearchReplaceDialog::~MadSearchReplaceDialog()
{
	//(*Destroy(MadSearchReplaceDialog)
	//*)
	delete m_RecentReplaceText;
	delete m_RecentFindText;
}

void MadSearchReplaceDialog::MadSearchReplaceDialogClose( wxCloseEvent& event )
{
	if( event.CanVeto() && (nullptr != dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow())))
	{
		event.Veto();
		wxCommandEvent evt;
		return WxButtonCloseClick( evt );
	}

	if( g_CurrentMadEdit == m_ReplaceText || g_CurrentMadEdit == m_FindText ) g_CurrentMadEdit = nullptr;
	g_SearchReplaceDialog = nullptr;
	Destroy();
}

void MadSearchReplaceDialog::WxButtonCloseClick( wxCommandEvent& WXUNUSED(event) )
{
	Show( false );
	wxFrame * mframe = dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow());
	if( mframe != nullptr )
	{
		mframe->Raise();

		if( g_ActiveMadEdit != nullptr )
		{
			g_ActiveMadEdit->Refresh( false );
			g_ActiveMadEdit->SetFocus();
		}
	}
}

void MadSearchReplaceDialog::WxButtonFindNextClick( wxCommandEvent& WXUNUSED(event) )
{
	// add: gogo, 19.09.2009
	g_StatusBar->SetStatusText( wxEmptyString, 0 );
	bool ThrEndOfFile = false;
	wxString text, expr;
	m_FindText->GetText( text, true );

	if( text.Len() > 0 )
	{
		m_RecentFindText->AddFileToHistory( text );
		MadSearchResult sr;
		wxFileOffset selend = g_ActiveMadEdit->GetSelectionEndPos();
		// moved here: gogo, 19.09.2009
		wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
		//wxInt64 from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;

		if( WxCheckBoxSearchInSelection->IsChecked() )
		{
			rangeTo = m_SearchTo;

			// removed: gogo, 19.09.2009
			//wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
			if( caretpos <= m_SearchFrom || caretpos >= m_SearchTo )
				rangeFrom = m_SearchFrom;
		}

		g_ActiveMadEdit->ToggleBookmarkInSearch( WxCheckBoxBookmarkLine->IsChecked() );

		for( ;; )
		{
			bool bHex = WxCheckBoxFindHex->GetValue();
			bool bRegex = ((!bHex) && WxCheckBoxRegex->GetValue());
			if( bHex )
			{
				sr = g_ActiveMadEdit->FindHexNext( text, rangeFrom, rangeTo );

				if( sr != SR_EXPR_ERROR )
				{
					if( IsMacroRecording() )
						RecordAsMadMacro( g_ActiveMadEdit, wxString::Format( wxT( "FindHexNext(\"%s\", %s, %s)" ), text.c_str(),
										  ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
				}
			}
			else
			{
				bool bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
				bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

				if( bRegex ) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}

				sr = g_ActiveMadEdit->FindTextNext( text,
													bRegex,
													bCaseSensitive,
													bWholeWord,
													bDotMatchNewline,
													rangeFrom, rangeTo );

				if( sr != SR_EXPR_ERROR )
				{
					if( IsMacroRecording() )
					{
						expr = text;
						MadSearchEscapeString(expr, bRegex);

						wxString fnstr( wxString::Format( wxT( "FindTextNext(\"%s\", bRegex=%s, bCaseSensitive=%s, bWholeWord=%s, bDotMatchNewline=%s, bPanChinese=%s, rangeFrom=%s, rangeTo=%s)" ), expr.c_str(),
														  bRegex ? wxT( "True" ) : wxT( "False" ),
														  bCaseSensitive ? wxT( "True" ) : wxT( "False" ),
														  bWholeWord ? wxT( "True" ) : wxT( "False" ),
														  bDotMatchNewline ? wxT( "True" ) : wxT( "False" ),
														  bPanChinese ? wxT( "True" ) : wxT( "False" ),
														  ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
						RecordAsMadMacro( g_ActiveMadEdit, fnstr );
					}
				}
			}

			if( sr != SR_NO )
			{
				if( sr == SR_YES && g_ActiveMadEdit->GetCaretPosition() == selend )
				{
					selend = -1;
					continue;
				}

				// add: gogo, 19.09.2009
				bool c_find = true;
				if( ThrEndOfFile )
					g_StatusBar->SetStatusText( _( "Passed the end of the file" ), 0 );
				else if(bRegex && g_ActiveMadEdit->IsZeroSelected())
				{
					g_ActiveMadEdit->ShowZeroLenSelIndicator();
					c_find = g_ActiveMadEdit->MoveToNextRegexSearchingPos( text );
				}
				if(c_find)
					break;
			}

			// add: gogo, 19.09.2009
			if( WxCheckBoxWrapAround->IsChecked() && ! WxCheckBoxSearchInSelection->IsChecked() )
			{
				if( rangeTo == caretpos )
				{
					g_StatusBar->SetStatusText( _( "Cannot find the matched string" ), 0 );
					break;
				}

				rangeTo = caretpos;
				rangeFrom = 0;
				ThrEndOfFile = true;
				continue;
			}

			//--------------
			wxString msg( _( "Cannot find the matched string." ) );
			msg += wxT( "\n\n" );
			msg += WxCheckBoxSearchInSelection->IsChecked() ?
				   _( "Do you want to find from begin of selection?" ) :
				   _( "Do you want to find from begin of file?" );

			if( wxCANCEL == MadMessageBox( msg, _( "Find Next" ), wxOK | wxCANCEL
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION > 9)
										   | wxCANCEL_DEFAULT
#endif
										   | wxICON_QUESTION ) )
			{
				break;
			}

			rangeFrom = WxCheckBoxSearchInSelection->IsChecked() ? m_SearchFrom : 0;
			rangeTo = WxCheckBoxSearchInSelection->IsChecked() ? m_SearchTo : g_ActiveMadEdit->GetFileSize();

			if( WxCheckBoxSearchInSelection->IsChecked() )
			{
				g_ActiveMadEdit->SetSelection( m_SearchFrom, m_SearchTo );
			}

			if( WxCheckBoxPurgeBookmark->IsChecked() )
				g_ActiveMadEdit->ClearAllBookmarks();
		}
	}
	else
	{
		g_StatusBar->SetStatusText( _( "Empty expression" ), 0 );
	}

	if( WxCheckBoxMoveFocus->GetValue() )
	{
		wxFrame * mframe = dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow());
		if( mframe != nullptr )
		{
			mframe->Raise();
			g_ActiveMadEdit->SetFocus();
		}
	}
}

void MadSearchReplaceDialog::WxButtonFindPrevClick( wxCommandEvent& WXUNUSED(event) )
{
	extern MadEdit *g_ActiveMadEdit;

	if( g_ActiveMadEdit == nullptr )
		return;

	// add: gogo, 19.09.2009
	g_StatusBar->SetStatusText( wxEmptyString, 0 );
	bool ThrEndOfFile = false;
	wxString text, expr;
	m_FindText->GetText( text, true );

	if( text.Len() > 0 )
	{
		m_RecentFindText->AddFileToHistory( text );
		MadSearchResult sr;
		wxFileOffset selbeg = g_ActiveMadEdit->GetSelectionBeginPos();
		// moved here: gogo, 19.09.2009
		wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
		wxFileOffset rangeFrom = -1, rangeTo = -1;

		if( WxCheckBoxSearchInSelection->IsChecked() )
		{
			rangeFrom = m_SearchFrom;

			// removed: gogo, 19.09.2009
			//wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
			if( caretpos <= m_SearchFrom || caretpos >= m_SearchTo )
				rangeFrom = m_SearchFrom;
		}

		g_ActiveMadEdit->ToggleBookmarkInSearch( WxCheckBoxBookmarkLine->IsChecked() );

		for( ;; )
		{
			if( WxCheckBoxFindHex->GetValue() )
			{
				sr = g_ActiveMadEdit->FindHexPrevious( text, rangeTo, rangeFrom );

				if( IsMacroRecording() )
					RecordAsMadMacro( g_ActiveMadEdit, wxString::Format( wxT( "FindHexPrevious(\"%s\", %s, %s)" ), text.c_str(), ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
			}
			else
			{
				bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
				bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

				if( bRegex ) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}

				sr = g_ActiveMadEdit->FindTextPrevious( text,
														bRegex,
														bCaseSensitive,
														bWholeWord,
														bDotMatchNewline,
														rangeTo, rangeFrom );

				if( IsMacroRecording() )
				{
					expr = text;
					MadSearchEscapeString(expr, bRegex);

					wxString fnstr( wxString::Format( wxT( "FindTextPrevious(\"%s\", bRegex=%s, bCaseSensitive=%s, bWholeWord=%s, bDotMatchNewline=%s, bPanChinese=%s, rangeFrom=%s, rangeTo=%s)" ), expr.c_str(),
													  bRegex ? wxT( "True" ) : wxT( "False" ),
													  bCaseSensitive ? wxT( "True" ) : wxT( "False" ),
													  bWholeWord ? wxT( "True" ) : wxT( "False" ),
													  bDotMatchNewline ? wxT( "True" ) : wxT( "False" ),
													  bPanChinese ? wxT( "True" ) : wxT( "False" ),
													  ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
					RecordAsMadMacro( g_ActiveMadEdit, fnstr );
				}
			}

			if( sr != SR_NO )
			{
				if( sr == SR_YES && g_ActiveMadEdit->GetCaretPosition() == selbeg )
				{
					selbeg = -1;
					continue;
				}

				// add: gogo, 19.09.2009
				if( ThrEndOfFile )
					g_StatusBar->SetStatusText( _( "Passed the end of the file" ), 0 );

				break;
			}

			// add: gogo, 19.09.2009
			if( WxCheckBoxWrapAround->IsChecked() && ! WxCheckBoxSearchInSelection->IsChecked() )
			{
				if( rangeFrom == caretpos )
				{
					g_StatusBar->SetStatusText( _( "Cannot find the matched string" ), 0 );
					break;
				}

				rangeTo = g_ActiveMadEdit->GetFileSize();
				rangeFrom = caretpos;
				ThrEndOfFile = true;
				continue;
			}

			//--------------
			wxString msg( _( "Cannot find the matched string." ) );
			msg += wxT( "\n\n" );
			msg += WxCheckBoxSearchInSelection->IsChecked() ?
				   _( "Do you want to find from end of selection?" ) :
				   _( "Do you want to find from end of file?" );

			if( wxCANCEL == MadMessageBox( msg, _( "Find Previous" ), wxOK | wxCANCEL | wxICON_QUESTION ) )
			{
				break;
			}

			rangeTo = WxCheckBoxSearchInSelection->IsChecked() ? m_SearchTo : g_ActiveMadEdit->GetFileSize();
			rangeFrom = WxCheckBoxSearchInSelection->IsChecked() ? m_SearchFrom : 0;

			if( WxCheckBoxPurgeBookmark->IsChecked() )
				g_ActiveMadEdit->ClearAllBookmarks();
		}
	}
	else
	{
		g_StatusBar->SetStatusText( _( "Empty expression" ), 0 );
	}

	if( WxCheckBoxMoveFocus->GetValue() )
	{
		wxFrame * mframe = dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow());
		if( mframe != nullptr )
		{
			mframe->Raise();
			g_ActiveMadEdit->SetFocus();
		}
	}
}


/*
 * MadSearchReplaceDialogKeyDown
 */
void MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown( wxKeyEvent& event )
{
	int key = event.GetKeyCode();

	//SetTitle(wxString()<<key);

	switch( key )
	{
	case WXK_ESCAPE:
		Show( false );
		return;

	case WXK_RETURN:
	case WXK_NUMPAD_ENTER:
		//if( this->GetClassInfo()->GetClassName() != wxString( wxT( "wxButton" ) ) )
		{
			wxCommandEvent e;
			wxButton* default_btn = static_cast<wxButton*>( GetDefaultItem() );

			if( default_btn == WxButtonReplace )
				return WxButtonReplaceClick( e ); // no skip

			return WxButtonFindNextClick( e ); // no skip
		}

		break;

	case WXK_DOWN:
		wxWindow * win = FindFocus();
		if( win == (wxWindow *)m_FindText )
		{
			int x, y, w, h;
			m_FindText->GetPosition( &x, &y );
			m_FindText->GetSize( &w, &h );
			PopupMenu( &WxPopupMenuRecentFindText, x, y + h );
			return;
		}
		else
			if( win == (wxWindow *)m_ReplaceText )
			{
				int x, y, w, h;
				m_ReplaceText->GetPosition( &x, &y );
				m_ReplaceText->GetSize( &w, &h );
				PopupMenu( &WxPopupMenuRecentReplaceText, x, y + h );
				return;
			}

		break;
	}

	extern wxAcceleratorEntry g_AccelFindNext, g_AccelFindPrev;
	int flags = wxACCEL_NORMAL;

	if( event.m_altDown ) flags |= wxACCEL_ALT;

	if( event.m_shiftDown ) flags |= wxACCEL_SHIFT;

	if( event.m_controlDown ) flags |= wxACCEL_CTRL;

	if( g_AccelFindNext.GetKeyCode() == key && g_AccelFindNext.GetFlags() == flags )
	{
		wxCommandEvent e;
		WxButtonFindNextClick( e );
		return; // no skip
	}

	if( g_AccelFindPrev.GetKeyCode() == key && g_AccelFindPrev.GetFlags() == flags )
	{
		wxCommandEvent e;
		WxButtonFindPrevClick( e );
		return; // no skip
	}

	event.Skip();
}

/*
 * WxBitmapButtonRecentFindTextClick
 */
void MadSearchReplaceDialog::WxBitmapButtonRecentFindTextClick( wxCommandEvent& WXUNUSED(event) )
{
	PopupMenu( &WxPopupMenuRecentFindText );
}

void MadSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick( wxCommandEvent& WXUNUSED(event) )
{
	PopupMenu( &WxPopupMenuRecentReplaceText );
}

void MadSearchReplaceDialog::OnRecentFindText( wxCommandEvent& event )
{
	int idx = event.GetId() - ID_RECENTFINDTEXT1;
	wxString text = m_RecentFindText->GetHistoryFile( idx );

	if( !text.IsEmpty() )
	{
		m_FindText->SetText( text );
		m_FindText->SetFocus();
	}
}

void MadSearchReplaceDialog::OnRecentReplaceText( wxCommandEvent& event )
{
	int idx = event.GetId() - ID_RECENTREPLACETEXT1;
	wxString text = m_RecentReplaceText->GetHistoryFile( idx );

	if( !text.IsEmpty() )
	{
		m_ReplaceText->SetText( text );
		m_ReplaceText->SetFocus();
	}
}

void MadSearchReplaceDialog::ReadWriteSettings( bool bRead )
{
	extern MadEdit *g_ActiveMadEdit;
	wxConfigBase *m_Config = wxConfigBase::Get( false );
	if(m_Config == nullptr) return;
	wxString oldpath = m_Config->GetPath();
	if( m_EnableTransparency && WxRadioLosingFocus->GetValue() )
	{
		SetTransparent( ( wxByte )WxSliderTransDegree->GetValue() );
	}

	if( bRead )
	{
		bool bb;
		m_Config->Read( wxT( "/MadEdit/SearchMoveFocus" ), &bb, false );
		WxCheckBoxMoveFocus->SetValue( bb );
		m_Config->Read( wxT( "/MadEdit/SearchCaseSensitive" ), &bb, false );
		WxCheckBoxCaseSensitive->SetValue( bb );
		m_Config->Read( wxT( "/MadEdit/SearchWholeWord" ), &bb, false );
		WxCheckBoxWholeWord->SetValue( bb );
		m_Config->Read( wxT( "/MadEdit/SearchRegex" ), &bb, false );
		WxCheckBoxRegex->SetValue( bb );
		m_Config->Read( wxT( "/MadEdit/SearchHex" ), &bb, false );
		WxCheckBoxFindHex->SetValue( bb );
		UpdateCheckBoxByCBHex( bb );
		m_Config->Read( wxT( "/MadEdit/SearchInSelection" ), &bb, false );
		WxCheckBoxSearchInSelection->SetValue( bb );
		UpdateSearchInSelection( bb );
		m_Config->Read( wxT( "/MadEdit/SearchBookmarkLines" ), &bb, false );
		WxCheckBoxBookmarkLine->SetValue( bb );
		m_Config->Read( wxT( "/MadEdit/SearchBookmarkOnly" ), &bb, false );
		WxCheckBoxBookmarkOnly->SetValue( bb );
		if(WxCheckBoxBookmarkLine->GetValue() )
		{
			WxCheckBoxBookmarkOnly->Enable();
		}
		else
		{
			WxCheckBoxBookmarkOnly->Disable();
		}

		m_Config->Read( wxT( "/MadEdit/SearchPurgeBookmark" ), &bb, false );
		WxCheckBoxPurgeBookmark->SetValue( bb );
		if(m_EnableTransparency)
		{
			long transparency = 30;
			m_Config->Read( wxT( "/MadEdit/SearchTransparency" ), &transparency, 255 );
			if(transparency < 30) transparency = 30;
			else if(transparency > 255) transparency = 255;
			WxSliderTransDegree->SetValue(transparency);
		}
	}
	else
	{
		m_Config->Write( wxT( "/MadEdit/SearchMoveFocus" ), WxCheckBoxMoveFocus->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchCaseSensitive" ), WxCheckBoxCaseSensitive->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchWholeWord" ), WxCheckBoxWholeWord->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchRegex" ), WxCheckBoxRegex->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchHex" ), WxCheckBoxFindHex->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchInSelection" ), WxCheckBoxSearchInSelection->GetValue() );
		//m_Config->Write(wxT("/MadEdit/SearchFrom"), (wxLongLong(m_SearchFrom)).ToString());
		//m_Config->Write(wxT("/MadEdit/SearchTo"), (wxLongLong(m_SearchTo)).ToString());
		m_Config->Write( wxT( "/MadEdit/SearchBookmarkLines" ), WxCheckBoxBookmarkLine->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchBookmarkOnly" ), WxCheckBoxBookmarkOnly->GetValue() );
		m_Config->Write( wxT( "/MadEdit/SearchPurgeBookmark" ), WxCheckBoxPurgeBookmark->GetValue() );
		
		if(m_EnableTransparency)
		{
			m_Config->Write( wxT( "/MadEdit/SearchTransparency" ), WxSliderTransDegree->GetValue() );
		}
	}

	m_Config->SetPath( oldpath );
}

static inline ucs4_t ToHex( int d ) // 0 <= d <= 15
{
	if( d < 10 )
		return '0' + d;

	return 'A' + d - 10;
}

void MadSearchReplaceDialog::UpdateCheckBoxByCBHex( bool check )
{
	if( check )
	{
		WxCheckBoxCaseSensitive->Disable();
		WxCheckBoxWholeWord->Disable();
		WxCheckBoxRegex->Disable();
		WxCheckBoxDotMatchNewLine->Disable();
		wxCheckBoxReplaceNoDoubleCheck->Disable();
		WxCheckBoxPanChinese->Disable();
	}
	else
	{
		WxCheckBoxCaseSensitive->Enable();
		WxCheckBoxRegex->Enable();

		if( WxCheckBoxRegex->GetValue() )
		{
			WxCheckBoxDotMatchNewLine->Enable();
			wxCheckBoxReplaceNoDoubleCheck->Enable();
			WxCheckBoxPanChinese->Enable();
			WxCheckBoxWholeWord->Disable();
		}
		else
		{
			WxCheckBoxWholeWord->Enable();
			WxCheckBoxDotMatchNewLine->Disable();
			WxCheckBoxPanChinese->Disable();
			wxCheckBoxReplaceNoDoubleCheck->Disable();
		}
	}
}

/*
 * WxCheckBoxFindHexClick
 */
void MadSearchReplaceDialog::WxCheckBoxFindHexClick( wxCommandEvent& event )
{
	extern MadEdit *g_ActiveMadEdit;
	bool checked = event.IsChecked();
	wxString text, ws;

	if( checked )
	{
		m_FindText->GetText( text, true );

		if( text.IsEmpty() )
		{
			if( g_ActiveMadEdit != nullptr )
			{
				g_ActiveMadEdit->GetSelHexString( ws, true );
				m_FindText->SetText( ws );
			}
		}
		else
		{
			m_FindText->SelectAll();
			m_FindText->GetSelHexString( ws, true );
			m_FindText->SetText( ws );
		}
	}
	else
	{
		m_FindText->GetText( text, true );

		if( text.IsEmpty() )
		{
			if( g_ActiveMadEdit != nullptr )
			{
				g_ActiveMadEdit->GetSelText( ws );
				m_FindText->SetText( ws );
			}
		}
	}

	UpdateCheckBoxByCBHex( event.IsChecked() );
}

/*
 * WxButtonReplaceClick
 */
void MadSearchReplaceDialog::WxButtonReplaceClick( wxCommandEvent& WXUNUSED(event) )
{
	if( g_ActiveMadEdit == nullptr )
		return;

	wxString text;
	m_FindText->GetText( text, true );

	if( text.Len() > 0 )
	{
		wxString reptext, expr, target;
		m_ReplaceText->GetText( reptext, true );
		m_RecentFindText->AddFileToHistory( text );

		if( reptext.Len() > 0 )
		{
			m_RecentReplaceText->AddFileToHistory( reptext );
		}

		wxFileOffset rangeFrom = -1, rangeTo = -1;
		wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();

		if( WxCheckBoxSearchInSelection->IsChecked() )
		{
			rangeTo = m_SearchTo;

			if( caretpos <= m_SearchFrom || caretpos > m_SearchTo )
				rangeFrom = m_SearchFrom;
		}
		else
		{
			rangeFrom = caretpos;
		}

		wxString func;
		for( ;; )
		{
			MadReplaceResult ret = RR_EXPR_ERROR;

			if( WxCheckBoxFindHex->GetValue() )
			{
				ret = g_ActiveMadEdit->ReplaceHex( text, reptext, rangeFrom, rangeTo );

				if( ( ret == RR_REP_NEXT || ret == RR_NREP_NEXT ) && ( IsMacroRecording() ) )
				{
					RecordAsMadMacro( g_ActiveMadEdit, wxString::Format( wxT( "ReplaceHex(\"%s\", \"%s\", %s, %s)" ), text.c_str(), reptext.c_str(), ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
				}
			}
			else
			{
				bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
				bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

				if( bRegex ) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}

				if(bRegex && wxCheckBoxReplaceNoDoubleCheck->GetValue())
				{
					ret = g_ActiveMadEdit->ReplaceTextNoDoubleCheck( text, reptext,
																 bRegex,
																 bCaseSensitive,
																 bWholeWord,
																 bDotMatchNewline,
																 rangeFrom, rangeTo );
					func = wxT("ReplaceTextNoDoubleCheck");
				}
				else
				{
					ret = g_ActiveMadEdit->ReplaceText( text, reptext,
														bRegex,
														bCaseSensitive,
														bWholeWord,
														bDotMatchNewline,
														rangeFrom, rangeTo );					
					func = wxT("ReplaceText");
				}

				if( ( ret == RR_REP_NEXT || ret == RR_NREP_NEXT ) && ( IsMacroRecording() ) )
				{
					expr = text;
					target = reptext;
					MadSearchEscapeString(expr, bRegex);
					MadSearchEscapeString(target, bRegex);

					wxString fnstr( wxString::Format( wxT( "%s(\"%s\", \"%s\", bRegex=%s, bCaseSensitive=%s, bWholeWord=%s, bDotMatchNewline=%s, bPanChinese=%s, rangeFrom=%s, rangeTo=%s)" ), func.c_str(), expr.c_str(), target.c_str(),
													  bRegex ? wxT( "True" ) : wxT( "False" ),
													  bCaseSensitive ? wxT( "True" ) : wxT( "False" ),
													  bWholeWord ? wxT( "True" ) : wxT( "False" ),
													  bDotMatchNewline ? wxT( "True" ) : wxT( "False" ),
													  bPanChinese ? wxT( "True" ) : wxT( "False" ),
													  ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
					RecordAsMadMacro( g_ActiveMadEdit, fnstr );
				}
			}

			switch( ret )
			{
			case RR_REP_NNEXT:
			case RR_NREP_NNEXT:
				// handle latter
				break;

			case RR_REP_NEXT:
			case RR_NREP_NEXT:
				if( WxCheckBoxMoveFocus->GetValue() )
				{
					wxFrame * mframe = dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow());
					if( mframe == nullptr )
					{
						return;
					}

					mframe->Raise();
					wxASSERT(g_ActiveMadEdit != nullptr);
					g_ActiveMadEdit->SetFocus();

					if( IsMacroRecording() )
					{
						if( WxCheckBoxFindHex->GetValue() )
						{
							RecordAsMadMacro( g_ActiveMadEdit, wxString::Format( wxT( "ReplaceHex(\"%s\", \"%s\", %s, %s)" ), text.c_str(), reptext.c_str(), ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
						}
						else
						{
							bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
							bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

							if( bRegex ) bWholeWord = false;
							else
							{
								bDotMatchNewline = false;
								bPanChinese = false;
							}
							expr = text;
							target = reptext;
							MadSearchEscapeString(expr, bRegex);
							MadSearchEscapeString(target, bRegex);

							wxString fnstr( wxString::Format( wxT( "ReplaceText(\"%s\", \"%s\", bRegex=%s, bCaseSensitive=%s, bWholeWord=%s, bDotMatchNewline=%s, bPanChinese=%s, rangeFrom=%s, rangeTo=%s)" ), expr.c_str(), target.c_str(),
															  bRegex ? wxT( "True" ) : wxT( "False" ),
															  bCaseSensitive ? wxT( "True" ) : wxT( "False" ),
															  bWholeWord ? wxT( "True" ) : wxT( "False" ),
															  bDotMatchNewline ? wxT( "True" ) : wxT( "False" ),
															  bPanChinese ? wxT( "True" ) : wxT( "False" ),
															  ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
							RecordAsMadMacro( g_ActiveMadEdit, fnstr );
						}
					}
				}

				break;

			default:
				break;
			}

			if( ret == RR_REP_NNEXT || ret == RR_NREP_NNEXT )
			{
				wxString msg( _( "Cannot find the matched string.\nReplace is finished." ) );
				msg += wxT( "\n\n" );
				msg += WxCheckBoxSearchInSelection->IsChecked() ?
					   _( "Do you want to find from begin of selection?" ) :
					   _( "Do you want to find from begin of file?" );

				if( wxCANCEL == MadMessageBox( msg, _( "Find Next" ), wxOK | wxCANCEL
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION > 9)
											   | wxCANCEL_DEFAULT
#endif
											   | wxICON_QUESTION ) )
				{
					m_FindText->SetFocus();
					break;
				}

				if( WxCheckBoxSearchInSelection->IsChecked() )
				{
					rangeFrom = m_SearchFrom;
					rangeTo = m_SearchTo;
					g_ActiveMadEdit->SetSelection( m_SearchFrom, m_SearchTo );
				}
				else
				{
					rangeFrom = 0;
					rangeTo = -1;
				}
			}
			else
			{
				// Not done, just pause
				m_FindText->SetFocus();
				break;
			}
		}
	}
}

/*
 * WxButtonReplaceAllClick
 */
void MadSearchReplaceDialog::WxButtonReplaceAllClick( wxCommandEvent& WXUNUSED(event) )
{
	if( g_ActiveMadEdit != nullptr )
		ReplaceAll( g_ActiveMadEdit );
}

void MadSearchReplaceDialog::WxButtonReplaceAllInAllClick( wxCommandEvent& WXUNUSED(event) )
{
	size_t count = ( ( wxAuiNotebook* )g_MainFrame->m_Notebook )->GetPageCount();

	for( size_t id = 0; id < count; ++id )
	{
		MadEdit *madedit = ( MadEdit* )( ( wxAuiNotebook* )g_MainFrame->m_Notebook )->GetPage( id );
		ReplaceAll( madedit, madedit == g_ActiveMadEdit );
	}
}

/*
 * MadSearchReplaceDialogActivate
 */
void MadSearchReplaceDialog::MadSearchReplaceDialogActivate( wxActivateEvent& event )
{
	ReadWriteSettings( event.GetActive() );

	if( event.GetActive() )
	{
		if( g_ActiveMadEdit )
		{
			wxString fname;
			int fsize;
			g_ActiveMadEdit->GetFont( fname, fsize );
			m_FindText->SetEncoding( g_ActiveMadEdit->GetEncodingName() );
			m_FindText->SetFont( fname, 14 );
			m_FindText->SetSpellCheck( g_ActiveMadEdit->GetSpellCheckStatus() );
			m_ReplaceText->SetEncoding( g_ActiveMadEdit->GetEncodingName() );
			m_ReplaceText->SetFont( fname, 14 );
			m_ReplaceText->SetSpellCheck( g_ActiveMadEdit->GetSpellCheckStatus() );
		}

		UpdateCheckBoxByCBHex( WxCheckBoxFindHex->GetValue() );

		if( m_EnableTransparency && WxRadioLosingFocus->GetValue() )
		{
			SetTransparent( wxIMAGE_ALPHA_OPAQUE );
		}

		GetSizer()->Fit( this );
		GetSizer()->Layout();
	}
}

/*
 * WxButtonReplaceClick
 */
void MadSearchReplaceDialog::WxButtonReplaceExpandClick( wxCommandEvent& WXUNUSED(event) )
{
	if( !m_ReplaceDlgUi )
	{
		wxString fname;
		int fsize;
		m_FindText->GetFont( fname, fsize );
		m_FindText->SetFont( fname, 14 );
		m_ReplaceText->SetFont( fname, 14 );
		ShowReplaceUI();
	}
	else
	{
		ShowFindUI();
	}

	m_FindText->SelectAll();
	m_FindText->SetFocus();
}

void MadSearchReplaceDialog::ShowReplaceUI()
{
	m_ReplaceDlgUi = true;
	WxButtonFindAllInAll->Show( false );
	WxButtonFindAll->Show( false );
	WxButtonReplaceExpand->SetLabel( _( "Search <<" ) );
	BoxSizerReplace->Show( true );
	WxButtonReplace->Show( true );
	WxButtonReplaceAll->Show( true );
	WxButtonReplaceAllInAll->Show( true );
	WxButtonReplace->SetDefault();
	BoxSizer4->Show(true);
	GetSizer()->Fit( this );
	GetSizer()->SetSizeHints( this );
	SetTitle( _( "Replace" ) );
	Show( true );
}

void MadSearchReplaceDialog::ShowFindUI()
{
	m_ReplaceDlgUi = false;
	BoxSizerReplace->Show( false );
	WxButtonReplace->Show( false );
	WxButtonReplaceAll->Show( false );
	WxButtonReplaceAllInAll->Show( false );
	BoxSizer4->Show(false);
	WxButtonFindAllInAll->Show( true );
	WxButtonFindAll->Show( true );
	WxButtonReplaceExpand->SetLabel( _( "R&eplace >>" ) );
	WxButtonFindNext->SetDefault();
	GetSizer()->Fit( this );
	GetSizer()->SetSizeHints( this );
	SetTitle( _( "Search" ) );
	Show( true );
}

void MadSearchReplaceDialog::UpdateSearchInSelection( bool check )
{
	//WxEditFrom->Enable(check);
	//WxEditTo->Enable(check);
	extern MadEdit *g_ActiveMadEdit;

	if( check && g_ActiveMadEdit != nullptr )
	{
		m_SearchFrom = g_ActiveMadEdit->GetSelectionBeginPos();
		m_SearchTo = g_ActiveMadEdit->GetSelectionEndPos();
	}
	else
	{
		m_SearchFrom = -1;
		m_SearchTo = -1;
	}

	// add: gogo, 19.09.2009
	if( check )
		WxCheckBoxWrapAround->Disable();
	else
		WxCheckBoxWrapAround->Enable();
}

void MadSearchReplaceDialog::WxCheckBoxSearchInSelectionClick( wxCommandEvent& event )
{
	UpdateSearchInSelection( event.IsChecked() );
}

void MadSearchReplaceDialog::WxButtonCountClick( wxCommandEvent& WXUNUSED(event) )
{
	extern MadEdit *g_ActiveMadEdit;

	if( g_ActiveMadEdit == nullptr )
		return;

	int count = 0;
	wxString text;
	m_FindText->GetText( text, true );

	if( text.Len() > 0 )
	{
		m_RecentFindText->AddFileToHistory( text );
		//wxInt64 from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;

		if( WxCheckBoxSearchInSelection->IsChecked() )
		{
			rangeTo = m_SearchTo;
			rangeFrom = m_SearchFrom;
		}

		if( WxCheckBoxFindHex->GetValue() )
		{
			count = g_ActiveMadEdit->FindHexAll( text, false, nullptr, nullptr, rangeFrom, rangeTo );
		}
		else
		{
			bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
			bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

			if( bRegex ) bWholeWord = false;
			else
			{
				bDotMatchNewline = false;
				bPanChinese = false;
			}

			count = g_ActiveMadEdit->FindTextAll( text,
												  bRegex,
												  bCaseSensitive,
												  bWholeWord,
												  bDotMatchNewline,
												  false,
												  false,
												  nullptr, nullptr,
												  rangeFrom, rangeTo );
		}
	}

	if( count >= 0 )
	{
		wxString msg;
		msg.Printf( _( "'%s' was found %d times." ), text.c_str(), count );
		MadMessageBox( msg, wxT( "MadEdit-Mod" ), wxOK );
	}
}

void DisplayFindAllResult( wxTreeItemId &myroot, vector<wxFileOffset> &begpos, vector<wxFileOffset> &endpos, MadEdit *madedit, bool expandresults = true, OnProgressUpdatePtr updater = nullptr )
{
	int ResultCount = 0;
#if USE_GENERIC_TREECTRL
	wxGenericTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
#else
	wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
#endif

	if( !begpos.empty() ) // found data
	{
		int pid = ( ( wxAuiNotebook* )g_MainFrame->m_Notebook )->GetPageIndex( madedit );
		int maxdisplay = madedit->GetMaxDisplaySize();
		wxString fmt, filename = madedit->GetFileName();

		if( filename.IsEmpty() )
		{
			if( pid != wxNOT_FOUND )
			{
				filename = ( ( wxAuiNotebook* )g_MainFrame->m_Notebook )->GetPageText( pid );

				if( filename[filename.Len() - 1] == wxT( '*' ) )
					filename.Truncate( filename.Len() - 1 );
			}
		}

		if( !filename.IsEmpty() )
		{
			size_t count = begpos.size(), idx = 0;
			int line = -1, endline = -1;
			wxString linetext, loc;
			results->Freeze();
			wxString status = _( "Preparing %s of %s results..." );
			status += wxT( " 							   \n" );
			wxString lend;
			bool multiline = false;
			switch( madedit->GetNewLineType() )
			{
				case nltDOS:  lend = wxT( "\r\n" ); break;
			
				case nltUNIX: lend = wxT( "\n" ); break;
			
				case nltMAC: lend = wxT( "\r" ); break;
			
				default: break;
			}

			do
			{
				multiline = false;
				if( madedit->IsTextFile() )
				{
					line = madedit->GetLineByPos( begpos[idx] );
					endline = madedit->GetLineByPos( endpos[idx] );

					linetext.Empty();
					if( line != endline )
					{
						madedit->GetLine( linetext, line, 0xFFFF );
						if((linetext.Len() < 0xFFFF)) // matched multipile lines
						{
							int linett = line + 1;

							while((linetext.Len() < 0xFFFF) && (linett <= endline))
							{
								linetext += lend;
								madedit->GetLine( linetext, linett, (0xFFFF - linetext.Len()) );
								++linett;
							}
						}
						multiline = true;
						loc.Printf( _( "Line(%d-%d)" ), line + 1,  endline + 1 );
					}
					else
					{
						madedit->GetLine( linetext, line, 0xFFFF );
						loc.Printf( _( "Line(%d)" ), line + 1 );
					}
				}
				else
				{
					loc.Printf( _( "Offset(%s)" ), wxLongLong( begpos[idx] ).ToString().c_str() );
					linetext = _( "Binary file matches" );
				}

				loc += wxT(": "); //As delimiter

				fmt = loc + linetext;
				if(fmt.Len() > (size_t)maxdisplay)
				{
					fmt.Remove(maxdisplay-4);
					fmt += wxT("...");
				}
				if(multiline)
				{
					switch( madedit->GetNewLineType() )
					{
						case nltDOS:  fmt.Replace(wxT( "\r\n" ), wxT( "\\r\\n" )); break;
					
						case nltUNIX: fmt.Replace(wxT( "\n" ), wxT( "\\n" )); break;
					
						case nltMAC: fmt.Replace(wxT( "\r" ), wxT( "\\r" )); break;
					
						default: break;
					}
				}
				if( madedit->IsTextFile() )
				{
					linetext += lend;
				}
				g_MainFrame->AddItemToFindInFilesResults( myroot, fmt, linetext, idx, filename, pid, begpos[idx], endpos[idx] );
				++ResultCount;

				if( updater != nullptr && ( count >= 2000 ) )
				{
					if( updater( idx, wxString::Format( status, ( wxLongLong( idx ).ToString().c_str() ), ( wxLongLong( count ).ToString().c_str() ) ), nullptr ) == false ) break;
				}
			}
			while( ++idx < count );

			results->Thaw();

			if( results->GetCount() )
			{
				if( expandresults )
				{
					wxTreeItemId rtid = results->GetRootItem();
					wxASSERT(rtid.IsOk());
					wxTreeItemId id = results->GetLastChild(rtid);
					if(id.IsOk())
						results->Expand(id);
				}

				g_MainFrame->m_AuiManager.GetPane( g_MainFrame->m_InfoNotebook ).Show();
				g_MainFrame->m_AuiManager.Update();
			}
		}
	}

	if( !ResultCount )
	{
		g_StatusBar->SetStatusText( _( "Cannot find the matched string" ), 0 );
	}
	else
	{
		wxString smsg( wxLongLong( begpos.size() ).ToString() + _( " results" ) );
		g_StatusBar->SetStatusText( smsg, 0 );
	}
}

void MadSearchReplaceDialog::WxButtonFindAllClick( wxCommandEvent& WXUNUSED(event) )
{
	extern MadEdit *g_ActiveMadEdit;

	if( g_ActiveMadEdit )
		SearchAll( g_ActiveMadEdit );
}

void MadSearchReplaceDialog::WxButtonFindAllInAllClick( wxCommandEvent& WXUNUSED(event) )
{
	extern MadEdit *g_ActiveMadEdit;
	size_t count = ( ( wxAuiNotebook* )g_MainFrame->m_Notebook )->GetPageCount();

	for( size_t id = 0; id < count; ++id )
	{
		MadEdit *madedit = ( MadEdit* )( ( wxAuiNotebook* )g_MainFrame->m_Notebook )->GetPage( id );
		SearchAll( madedit, madedit == g_ActiveMadEdit );
	}
}

void MadSearchReplaceDialog::PurgeRecentFindTexts()
{
	int n = ( int ) m_RecentFindText->GetCount();

	for( int i = n - 1; i >= 0; --i )
		m_RecentFindText->RemoveFileFromHistory( ( size_t )i );
}

void MadSearchReplaceDialog::SearchAll( MadEdit * madedit, bool needRec/*=true*/ )
{
	vector<wxFileOffset> begpos, endpos;
	// get all matched data in madedit
	wxString expr;
	int ok;
	m_FindText->GetText( expr );

	if( expr.Len() > 0 )
	{
		wxString strtobesearch = _("Search \"") + expr + wxT("\" ");
		begpos.reserve( 128 * 1024 );
		endpos.reserve( 128 * 1024 );
		m_RecentFindText->AddFileToHistory( expr );
		madedit->ToggleBookmarkInSearch( WxCheckBoxBookmarkLine->IsChecked() );

		if( WxCheckBoxPurgeBookmark->IsChecked() )
			madedit->ClearAllBookmarks();

		// moved here: gogo, 19.09.2009
		wxFileOffset caretpos = madedit->GetCaretPosition();
		//wxInt64 from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;

		if( WxCheckBoxSearchInSelection->IsChecked() )
		{
			rangeTo = m_SearchTo;

			// removed: gogo, 19.09.2009
			//wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
			if( caretpos <= m_SearchFrom || caretpos >= m_SearchTo )
				rangeFrom = m_SearchFrom;
		}

#if USE_GENERIC_TREECTRL
		wxGenericTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
#else
		wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
#endif
		// Root
		wxTreeItemId rtid = results->GetRootItem();
		wxASSERT(rtid.IsOk());
		wxTreeItemIdValue rtck;
		// First Level----search results summary
		wxTreeItemId id = results->GetFirstChild(rtid, rtck);
		if(id.IsOk())
		{
			// Second Level File results
			wxTreeItemIdValue cookie;
			wxTreeItemId lstid = results->GetFirstChild(id, cookie);
			if(lstid.IsOk() && results->IsExpanded(lstid))
				results->Collapse(lstid);
		}

		if( WxCheckBoxFindHex->GetValue() )
		{
			ok = madedit->FindHexAll( expr, false, &begpos, &endpos, rangeFrom, rangeTo );

			if( needRec && IsMacroRecording() )
				RecordAsMadMacro( madedit, wxString::Format( wxT( "FindHexAll(\"%s\")" ), expr.c_str() ) );
		}
		else
		{
			bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
			bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

			if( bRegex ) bWholeWord = false;
			else
			{
				bDotMatchNewline = false;
				bPanChinese = false;
			}

			ok = madedit->FindTextAll( expr,
									   bRegex,
									   WxCheckBoxCaseSensitive->GetValue(),
									   bWholeWord,
									   bDotMatchNewline,
									   false,
									   false,
									   &begpos, &endpos, rangeFrom, rangeTo );

			if( needRec && ( IsMacroRecording() ) )
			{
				MadSearchEscapeString(expr, bRegex);
				wxString fnstr( wxString::Format( wxT( "FindTextAll(\"%s\", bRegex=%s, bCaseSensitive=%s, bWholeWord=%s, bDotMatchNewline=%s, bPanChinese=%s, False)" ),  expr.c_str(),
												  bRegex ? wxT( "True" ) : wxT( "False" ),
												  bCaseSensitive ? wxT( "True" ) : wxT( "False" ),
												  bWholeWord ? wxT( "True" ) : wxT( "False" ),
												  bDotMatchNewline ? wxT( "True" ) : wxT( "False" ),
												  bPanChinese ? wxT( "True" ) : wxT( "False" )));
				RecordAsMadMacro( madedit, fnstr );
			}
		}

		if( ok < 0 ) return;

		Show( false );

		wxTreeItemId myroot; 

		if( (!WxCheckBoxBookmarkLine->IsChecked()) || !WxCheckBoxBookmarkOnly->IsChecked() )
		{
			strtobesearch += wxString::Format( _("(%s hits in 1 file)"), ( wxLongLong( ok ).ToString().c_str() ) );
			myroot = g_MainFrame->NewSearchSession(strtobesearch);
		}

		OnProgressUpdatePtr progressUpdatePtr = nullptr;

		if( ok > 2000 )
		{
			wxString msg = _( "Found %s matched texts..." );
			msg += wxT( "								\n" );
			wxProgressDialog dialog( _( "Preparing Results" ),
									 wxString::Format( msg, ( wxLongLong( ok ).ToString().c_str() ) ),
									 ok,    // range
									 this,   // parent
									 wxPD_CAN_ABORT |
									 wxPD_AUTO_HIDE |
									 wxPD_APP_MODAL );
			g_SearchProgressDialog = &dialog;

			progressUpdatePtr = &OnSearchProgressUpdate;

			dialog.Update( ok );
			g_SearchProgressDialog = nullptr;
		}

		if( (!WxCheckBoxBookmarkLine->IsChecked()) || !WxCheckBoxBookmarkOnly->IsChecked() )
		{
			static wxString text( _( "Search Results" ) );
			int pid = g_MainFrame->m_InfoNotebook->GetPageIndex( g_MainFrame->m_FindInFilesResults );
			wxASSERT(pid != wxNOT_FOUND);
			g_MainFrame->m_InfoNotebook->SetPageText( pid, text );
			DisplayFindAllResult( myroot, begpos, endpos, madedit, true, progressUpdatePtr );
		}

		if( !ok )
		{
			g_StatusBar->SetStatusText( _( "Cannot find the matched string" ), 0 );
		}
		else
		{
			wxString smsg;
			smsg.Printf( _( "%s results" ), ( wxLongLong( ok ).ToString().c_str() ) );
			g_StatusBar->SetStatusText( smsg, 0 );

			// Root
			// First Level----search results summary
			if( (!WxCheckBoxBookmarkLine->IsChecked()) || !WxCheckBoxBookmarkOnly->IsChecked() )
			{
				wxTreeItemIdValue cookie;
				id = results->GetFirstChild(rtid, cookie);
				if(id.IsOk())
				{
					if(!results->IsExpanded(id))
						results->Expand(id);
					// Second Level File results
					wxTreeItemIdValue ck;
					wxTreeItemId lstid = results->GetFirstChild(id, ck);
					if(lstid.IsOk() && !results->IsExpanded(lstid))
						results->Expand(lstid);
				}
			}
			if(WxCheckBoxBookmarkLine->IsChecked())
			{
				if( g_ActiveMadEdit != nullptr )
				{
					g_ActiveMadEdit->Refresh( false );					
					g_ActiveMadEdit->SetFocus();
				}
			}
		}
	}
	else
	{
		g_StatusBar->SetStatusText( _( "Empty expression" ), 0 );
	}
}

void MadSearchReplaceDialog::PurgeRecentReplaceTexts()
{
	int n = ( int ) m_RecentReplaceText->GetCount();

	for( int i = n - 1; i >= 0; --i )
		m_RecentReplaceText->RemoveFileFromHistory( ( size_t )i );
}

void MadSearchReplaceDialog::ReplaceAll( MadEdit * madedit, bool needRec/*=true*/ )
{
	wxString text;
	m_FindText->GetText( text, true );

	if( text.Len() > 0 )
	{
		wxString reptext;
		m_ReplaceText->GetText( reptext, true );
		m_RecentFindText->AddFileToHistory( text );

		if( reptext.Len() > 0 )
		{
			m_RecentReplaceText->AddFileToHistory( reptext );
		}

		wxFileOffset rangeFrom = -1, rangeTo = -1;

		if( WxCheckBoxSearchInSelection->IsChecked() )
		{
			rangeTo = m_SearchTo;
			rangeFrom = m_SearchFrom;
		}

		int count = 0;

		if( WxCheckBoxFindHex->GetValue() )
		{
			count = madedit->ReplaceHexAll( text, reptext, nullptr, nullptr, rangeFrom, rangeTo );

			if( needRec && count && ( IsMacroRecording() ) )
				RecordAsMadMacro( madedit, wxString::Format( wxT( "ReplaceHexAll(\"%s\", \"%s\", %s, %s)" ), text.c_str(),  reptext.c_str(), ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
		}
		else
		{
			bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
			bool bPanChinese = WxCheckBoxPanChinese->GetValue(), bCaseSensitive = WxCheckBoxCaseSensitive->GetValue();

			if( bRegex ) bWholeWord = false;
			else
			{
				bDotMatchNewline = false;
				bPanChinese = false;
			}

			count = madedit->ReplaceTextAll( text, reptext,
											 bRegex,
											 bCaseSensitive,
											 bWholeWord,
											 bDotMatchNewline,
											 bPanChinese,
											 nullptr, nullptr, rangeFrom, rangeTo );

			if( needRec && count && ( IsMacroRecording() ) )
			{
				MadSearchEscapeString(text, bRegex);
				MadSearchEscapeString(reptext, bRegex);
				wxString fnstr( wxString::Format( wxT( "ReplaceTextAll(\"%s\", \"%s\", bRegex=%s, bCaseSensitive=%s, bWholeWord=%s, bDotMatchNewline=%s, bPanChinese=%s, rangeFrom=%s, rangeTo=%s)" ), text.c_str(), reptext.c_str(),
												  bRegex ? wxT( "True" ) : wxT( "False" ),
												  bCaseSensitive ? wxT( "True" ) : wxT( "False" ),
												  bWholeWord ? wxT( "True" ) : wxT( "False" ),
												  bDotMatchNewline ? wxT( "True" ) : wxT( "False" ),
												  bPanChinese ? wxT( "True" ) : wxT( "False" ),
												  ( wxLongLong( rangeFrom ).ToString() ).c_str(), ( wxLongLong( rangeTo ).ToString() ).c_str() ) );
				RecordAsMadMacro( madedit, fnstr );
			}
		}

		if(( count >= 0 )&& (g_ActiveMadEdit == madedit))
		{
			wxString msg;

			if( count == 0 )
				msg = wxString( _( "Cannot find any matched string." ) );
			else
				msg = wxString::Format( _( "%s string(s) were replaced." ), ( wxLongLong( count ).ToString().c_str() ) );

			g_StatusBar->SetStatusText( msg, 0 );
		}

		m_FindText->SetFocus();
	}
}

/*
 * WxRadioLosingFocusClick
 */
void MadSearchReplaceDialog::WxRadioLosingFocusSelect( wxCommandEvent& WXUNUSED(event) )
{
	// insert your code here
	if( m_EnableTransparency )
	{
		SetTransparent( wxIMAGE_ALPHA_OPAQUE );
	}
}

/*
 * WxRadioAlwaysClick
 */
void MadSearchReplaceDialog::WxRadioAlwaysSelect( wxCommandEvent& WXUNUSED(event) )
{
	// insert your code here
	if( m_EnableTransparency )
	{
		SetTransparent( ( wxByte )WxSliderTransDegree->GetValue() );
	}
}

/*
 * MadSearchReplaceDialogKillFocus
 */
void MadSearchReplaceDialog::SetTransparency()
{
	// insert your code here
	if( m_EnableTransparency && WxRadioLosingFocus->GetValue() )
	{
		SetTransparent( ( wxByte )WxSliderTransDegree->GetValue() );
	}
}

/*
 * WxSliderTransDegreeScroll
 */
void MadSearchReplaceDialog::WxSliderTransDegreeScroll( wxCommandEvent& WXUNUSED(event) )
{
	// insert your code here
	wxByte trans = wxIMAGE_ALPHA_OPAQUE;

	if( m_EnableTransparency && WxRadioAlways->GetValue() )
	{
		trans = ( wxByte )WxSliderTransDegree->GetValue();
	}

	SetTransparent( trans );
}

/*
 * WxCheckBoxRegexClick
 */
void MadSearchReplaceDialog::WxCheckBoxRegexClick( wxCommandEvent& event )
{
	// insert your code here
	if( event.IsChecked() )
	{
		WxCheckBoxWholeWord->Disable();
		WxCheckBoxFindHex->Disable();
		WxCheckBoxDotMatchNewLine->Enable();
		WxCheckBoxPanChinese->Enable();
		wxCheckBoxReplaceNoDoubleCheck->Enable();
	}
	else
	{
		WxCheckBoxWholeWord->Enable();
		WxCheckBoxFindHex->Enable();
		WxCheckBoxDotMatchNewLine->Disable();
		WxCheckBoxPanChinese->Disable();
		wxCheckBoxReplaceNoDoubleCheck->Disable();
	}
}

void MadSearchReplaceDialog::OnWxSliderInputSizerCmdScroll(wxCommandEvent& WXUNUSED(event))
{
	int times = WxSliderInputSizer->GetValue();
	int width, height = 0;
	m_FindText->GetSize (&width, &height);
	BoxSizerSearch->SetItemMinSize( m_FindText, width, m_OriginInputSize.GetHeight()*times );

	if(times == 1)
	{
		m_FindText->SetWordWrapMode(wwmNoWrap);
		m_ReplaceText->SetWordWrapMode(wwmNoWrap);
	}
	else
	{
		m_FindText->SetWordWrapMode(wwmWrapByWindow);
		int w, h;
		m_ReplaceText->GetSize(&w, &h);
		if(m_OriginInputSize.GetHeight() < h )
			m_ReplaceText->SetWordWrapMode(wwmWrapByWindow);
	}
	GetSizer()->Fit( this );
	GetSizer()->Layout();
}

void MadSearchReplaceDialog::WxCheckBoxBookmarkLineClick(wxCommandEvent& event)
{
	if( event.IsChecked() )
	{
		WxCheckBoxBookmarkOnly->Enable();
	}
	else
	{
		WxCheckBoxBookmarkOnly->Disable();
	}
}

void MadSearchReplaceDialog::OnEditUndo( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && ( !g_CurrentMadEdit->IsReadOnly() ) )
	{
		g_CurrentMadEdit->Undo();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditRedo( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && ( !g_CurrentMadEdit->IsReadOnly() ) )
	{
		g_CurrentMadEdit->Redo();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditCut( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CutToClipboard();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditCopy( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CopyToClipboard();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditPaste( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->PasteFromClipboard();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditDelete( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->Delete();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditCutLine( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CutLine();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditDeleteLine( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->DeleteLine();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditSelectAll( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->SelectAll();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditStartEndSelction( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->StartEndSelction();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditInsertTabChar( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->InsertTabChar();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditInsertDateTime( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->InsertDateTime();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditSortAscending( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId( begin, end );
		g_CurrentMadEdit->SortLines( sfAscending, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditSortDescending( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId( begin, end );
		g_CurrentMadEdit->SortLines( sfDescending, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditSortAscendingCase( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId( begin, end );
		g_CurrentMadEdit->SortLines( sfAscending | sfCaseSensitive, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditSortDescendingCase( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId( begin, end );
		g_CurrentMadEdit->SortLines( sfDescending | sfCaseSensitive, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditSortByOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		wxConfigBase *m_Config = wxConfigBase::Get(false);
		wxString oldpath = m_Config->GetPath();
		m_Config->SetPath( wxT( "/MadEdit" ) );
		int order;
		bool cs, num, rem;
		m_Config->Read( wxT( "SortOrder" ), &order, sfAscending );
		m_Config->Read( wxT( "SortCaseSensitive" ), &cs, false );
		m_Config->Read( wxT( "SortNumeric" ), &num, false );
		m_Config->Read( wxT( "SortRemoveDup" ), &rem, false );
		m_Config->SetPath( oldpath );
		MadSortFlags flags = order |
							 ( cs ? sfCaseSensitive : 0 ) |
							 ( num ? sfNumericSort : 0 ) |
							 ( rem ? sfRemoveDuplicate : 0 ) ;
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId( begin, end );
		g_CurrentMadEdit->SortLines( flags, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditSortOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == nullptr || g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit->GetEditMode() == emHexMode )
	{
		g_CurrentMadEdit = nullptr;
		return;
	}
	wxConfigBase *m_Config = wxConfigBase::Get(false);
	MadSortDialog dialog( this );
	wxString oldpath = m_Config->GetPath();
	m_Config->SetPath( wxT( "/MadEdit" ) );
	int order;
	bool cs, num, rem;
	m_Config->Read( wxT( "SortOrder" ), &order, sfAscending );
	dialog.WxRadioBoxOrder->SetSelection( order );
	m_Config->Read( wxT( "SortCaseSensitive" ), &cs, false );
	dialog.WxCheckBoxCase->SetValue( cs );
	m_Config->Read( wxT( "SortNumeric" ), &num, false );
	dialog.WxCheckBoxNumeric->SetValue( num );
	m_Config->Read( wxT( "SortRemoveDup" ), &rem, false );
	dialog.WxCheckBoxRemoveDup->SetValue( rem );
	// Hide Modaless Dialog
	//HideModalessDialogs();

	if( dialog.ShowModal() == wxID_OK )
	{
		order = dialog.WxRadioBoxOrder->GetSelection();
		cs = dialog.WxCheckBoxCase->GetValue();
		num = dialog.WxCheckBoxNumeric->GetValue();
		rem = dialog.WxCheckBoxRemoveDup->GetValue();
		m_Config->Write( wxT( "SortOrder" ), order );
		m_Config->Write( wxT( "SortCaseSensitive" ), cs );
		m_Config->Write( wxT( "SortNumeric" ), num );
		m_Config->Write( wxT( "SortRemoveDup" ), rem );
		int flags = order |
					( cs ? sfCaseSensitive : 0 ) |
					( num ? sfNumericSort : 0 ) |
					( rem ? sfRemoveDuplicate : 0 ) ;
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId( begin, end );
		g_CurrentMadEdit->SortLines( flags, begin, end );
	}

	m_Config->SetPath( oldpath );
	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditCopyAsHexString( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CopyAsHexString( false );
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditCopyAsHexStringWithSpace( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CopyAsHexString( true );
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditCopyRevertHex( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		static wxString strDelimiters = wxT( " .,?!@#\t$%^&*()-=_+[]{}\\|;:\"'`<>/~" );
		wxString str = wxGetTextFromUser( _( "Delimiters:" ), _( "Revert Hex String" ), strDelimiters );

		if( !str.IsEmpty() )
		{
			strDelimiters = str;
		}

		g_CurrentMadEdit->CopyRevertHex( str );
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditIncIndent( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->IncreaseDecreaseIndent( true );
		g_CurrentMadEdit = nullptr;
	}
}
void MadSearchReplaceDialog::OnEditDecIndent( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->IncreaseDecreaseIndent( false );
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditComment( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CommentUncomment( true );

		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditUncomment( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->CommentUncomment( false );
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditWordWrapToNewLine( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ConvertWordWrapToNewLine();
		g_CurrentMadEdit = nullptr;
	}
}
void MadSearchReplaceDialog::OnEditNewLineToWordWrap( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ConvertNewLineToWordWrap();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditToUpperCase( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ToUpperCase();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditToLowerCase( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ToLowerCase();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditInvertCase( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->InvertCase();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditCapitalize( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->Capitalize();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditToHalfWidth( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ToHalfWidth();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditToHalfWidthByOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit == nullptr ) { return; }

	static wxArrayInt selections;
	static bool inited = false;

	if( !inited )
	{
		selections.Add( 0 );
		selections.Add( 1 );
		selections.Add( 2 );
		selections.Add( 3 );
		inited = true;
	}

	wxString choices[4] = { _( "ASCII characters" ), _( "Japanese characters" ),
							_( "Korean characters" ), _( "other characters" )
						  };
#if (wxMAJOR_VERSION == 2)
	size_t sels = wxGetSelectedChoices( selections,
										_( "Choose the characters you want to convert:" ), _( "To Halfwidth by Options..." ),
										4, choices, this );
#else
	int sels = wxGetSelectedChoices( selections,
									 _( "Choose the characters you want to convert:" ), _( "To Halfwidth by Options..." ),
									 4, choices, this );
#endif

	if( sels > 0 )
	{
		bool ascii = false, japanese = false, korean = false, other = false;

		for( size_t i = 0; i < (size_t)sels; ++i )
		{
			switch( selections[i] )
			{
			case 0: ascii = true; break;

			case 1: japanese = true; break;

			case 2: korean = true; break;

			case 3: other = true; break;
			}
		}

		g_CurrentMadEdit->ToHalfWidth( ascii, japanese, korean, other );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditToFullWidth( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ToFullWidth();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditToFullWidthByOptions( wxCommandEvent& WXUNUSED(event) )
{
	if(( g_CurrentMadEdit == g_ActiveMadEdit ) ||( g_CurrentMadEdit == nullptr ) ) return;

	static wxArrayInt selections;
	static bool inited = false;

	if( !inited )
	{
		selections.Add( 0 );
		selections.Add( 1 );
		selections.Add( 2 );
		selections.Add( 3 );
		inited = true;
	}

	wxString choices[4] = { _( "ASCII characters" ), _( "Japanese characters" ),
							_( "Korean characters" ), _( "other characters" )
						  };
#if (wxMAJOR_VERSION == 2)
	size_t sels = wxGetSelectedChoices( selections,
										_( "Choose the characters you want to convert:" ), _( "To Fullwidth by Options..." ),
										4, choices, this );
#else
	int sels = wxGetSelectedChoices( selections,
									 _( "Choose the characters you want to convert:" ), _( "To Fullwidth by Options..." ),
									 4, choices, this );
#endif

	if( sels > 0 )
	{
		bool ascii = false, japanese = false, korean = false, other = false;

		for( size_t i = 0; i < (size_t)sels; ++i )
		{
			switch( selections[i] )
			{
			case 0: ascii = true; break;

			case 1: japanese = true; break;

			case 2: korean = true; break;

			case 3: other = true; break;
			}
		}

		g_CurrentMadEdit->ToFullWidth( ascii, japanese, korean, other );
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditTabToSpace( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ConvertTabToSpace();
		g_CurrentMadEdit = nullptr;
	}

}
void MadSearchReplaceDialog::OnEditSpaceToTab( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit )
	{
		g_CurrentMadEdit->ConvertSpaceToTab();
		g_CurrentMadEdit = nullptr;
	}
}

void MadSearchReplaceDialog::OnEditTrimTrailingSpaces( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->TrimTrailingSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditTrimLeadingSpaces( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->TrimLeadingSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditDeleteEmptyLines( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->DeleteEmptyLines();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditDeleteEmptyLinesWithSpaces( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->DeleteEmptyLinesWithSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditJoinLines( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->JoinLines();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditInsertNumbers( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() == emColumnMode )
	{
		// Hide Modaless Dialog
		//HideModalessDialogs();

		if( g_MadNumberDlg == nullptr ) { g_MadNumberDlg = new MadNumberDlg( this ); }

		if( g_MadNumberDlg->ShowModal() == wxID_OK )
		{
			MadNumberingStepType numStepType = mnstLinear;
			MadNumberFormat numFormat = nfDEC;
			MadNumberAlign numAlign = naLeft;
			int sel = g_MadNumberDlg->WxChoiceNumberStepType->GetSelection();

			switch( sel )
			{
			case 1: numStepType = mnstExponential; break;

			default: break;
			}

			sel = g_MadNumberDlg->WxChoiceFormat->GetSelection();

			switch( sel )
			{
			case 1:
				{
					numFormat = nfHEX;
				}
				break;

			case 2:
				{
					numFormat = nfBIN;
				}
				break;

			case 3:
				{
					numFormat = nfOCT;
				}
				break;

			default:
				break;
			}

			sel = g_MadNumberDlg->WxChoiceAlign->GetSelection();

			switch( sel )
			{
			case 1:
				{
					numAlign = naRight;
				}
				break;

			default:
				break;
			}

			long initialNum = 0, numStep = 0, totalChar = 0;
			g_MadNumberDlg->WxEditNumberOfChars->GetValue().ToLong( &totalChar );
			g_MadNumberDlg->WxEditNumberingStep->GetValue().ToLong( &numStep );
			g_MadNumberDlg->WxEditInitialNumber->GetValue().ToLong( &initialNum );
			wxString prefix, postfix;

			if( g_MadNumberDlg->WxCheckPrefix->GetValue() )
			{ prefix = g_MadNumberDlg->WxEditPrefix->GetValue(); }

			if( g_MadNumberDlg->WxCheckPostfix->GetValue() )
			{ postfix = g_MadNumberDlg->WxEditPostfix->GetValue(); }

			g_CurrentMadEdit->InsertIncrementalNumber( initialNum, numStep, totalChar, numStepType, numFormat, numAlign, g_MadNumberDlg->WxPadChar->GetValue(), prefix, postfix );
			g_CurrentMadEdit->Refresh( false );
		}
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditColumnAlignLeft( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->ColumnAlignLeft();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditColumnAlignRight( wxCommandEvent& WXUNUSED(event) )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->ColumnAlignRight();
	}

	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnEditSpellCheck( wxCommandEvent& event )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit && g_CurrentMadEdit->GetEditMode() != emHexMode )
	{
		g_CurrentMadEdit->ReplaceWordFromCaretPos( g_SpellSuggestions[event.GetId() - menuSpellOption1] );
	}
	g_CurrentMadEdit = nullptr;
}

void MadSearchReplaceDialog::OnToolsMadScriptList( wxCommandEvent& event )
{
	if( g_CurrentMadEdit == g_ActiveMadEdit ) return;
	if( g_CurrentMadEdit != nullptr )
	{ 
		wxString scriptdir = g_MadEditAppDir + wxT( "scripts" ) + wxFILE_SEP_PATH;
		int menuId = event.GetId();
		wxString filename = g_Menu_MadMacro_Scripts->GetLabelText( menuId ) + wxT( ".mpy" );
		wxString scripfile = scriptdir + filename;
		if(!wxFileExists(scripfile)) scripfile = g_MadEditHomeDir + wxT( "scripts" ) + wxFILE_SEP_PATH + filename;
		if(!wxFileExists(scripfile)) 
		{
			scripfile = 
#if defined (DATA_DIR)
				wxT( DATA_DIR"/madedit-mod/scripts/" ) +
#else
				wxT( "/usr/share/madedit-mod/scripts/" ) +
#endif
			filename;
		}

		wxTextFile scriptfile( scripfile );
		scriptfile.Open( wxConvFile );

		if( scriptfile.IsOpened() )
		{
			if( !g_EmbeddedPython )
			{
				try
				{
					g_EmbeddedPython = new EmbeddedPython();
				}
				catch( std::bad_alloc & )
				{
					MadMessageBox( _( "Memory allocation failed" ), _( "Error" ),  wxOK | wxICON_ERROR );
				}
			}

			if( g_EmbeddedPython )
			{
				wxString str = scriptfile.GetFirstLine() + wxT( "\n" );

				for( ; !scriptfile.Eof(); )
				{
					str << scriptfile.GetNextLine() << wxT( "\n" );
				}

				if( str.IsNull() == false )
				{
					g_MainFrame->SetMacroRunning();
					g_EmbeddedPython->exec( std::string((const char *)(str.ToUTF8().data())) );
					g_MainFrame->SetMacroStopped();
				}
			}

			scriptfile.Close();
		}
		g_CurrentMadEdit = nullptr;
	}
}
