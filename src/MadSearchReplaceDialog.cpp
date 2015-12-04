///////////////////////////////////////////////////////////////////////////////
// Name:		MadSearchReplaceDialog.cpp
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>
#include <wx/longlong.h>
#include <wx/progdlg.h>
#include <wx/gbsizer.h>
#include "MadEditFrame.h"
#include "MadSearchReplaceDialog.h"
#include "MadRecentList.h"

#include "MadEdit/MadEdit.h"
//Do not add custom	headers.
//wx-dvcpp designer	will remove	them
////Header Include Start
////Header Include End

extern int MadMessageBox(const wxString& message,
							 const wxString& caption = wxMessageBoxCaptionStr,
							 long style	= wxOK | wxCENTRE,
							 wxWindow *parent =	NULL,
							 int x = wxDefaultCoord, int y = wxDefaultCoord);

#include "../images/down.xpm"

extern wxStatusBar *g_StatusBar;   // add: gogo, 19.09.2009
extern MadRecentList * g_RecentFindText;
MadSearchReplaceDialog *g_SearchReplaceDialog=NULL;
extern void	RecordAsMadMacro(MadEdit *,	const wxString&);
extern MadEdit *g_ActiveMadEdit;
wxProgressDialog *g_SearchProgressDialog=NULL;

bool OnSearchProgressUpdate(int	value, const wxString &newmsg=wxEmptyString, bool *skip=NULL)
{
	if(g_SearchProgressDialog == NULL)
		return true;
	return g_SearchProgressDialog->Update(value, newmsg, skip);
}

//----------------------------------------------------------------------------
// MadSearchReplaceDialog
//----------------------------------------------------------------------------
	 //Add Custom Events only in the appropriate Block.
	// Code	added in  other	places will	be removed by wx-dvcpp
	////Event Table	Start
BEGIN_EVENT_TABLE(MadSearchReplaceDialog,wxDialog)
	////Manual Code	Start
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT,	MadSearchReplaceDialog::WxBitmapButtonRecentFindTextClick)
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTREPLACETEXT, MadSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick)
	EVT_MENU_RANGE(ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20,	MadSearchReplaceDialog::OnRecentFindText)
	EVT_MENU_RANGE(ID_RECENTREPLACETEXT1, ID_RECENTREPLACETEXT20, MadSearchReplaceDialog::OnRecentReplaceText)
	////Manual Code	End
	
	EVT_CLOSE(MadSearchReplaceDialog::MadSearchReplaceDialogClose)
	EVT_KEY_DOWN(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown)
	EVT_ACTIVATE(MadSearchReplaceDialog::MadSearchReplaceDialogActivate)
	EVT_BUTTON(ID_WXBUTTONCLOSE,MadSearchReplaceDialog::WxButtonCloseClick)
	EVT_BUTTON(ID_WXBUTTONREPLACEALLINALL,MadSearchReplaceDialog::WxButtonReplaceAllInAllClick)
	EVT_BUTTON(ID_WXBUTTONREPLACEALL,MadSearchReplaceDialog::WxButtonReplaceAllClick)
	EVT_BUTTON(ID_WXBUTTONREPLACE,MadSearchReplaceDialog::WxButtonReplaceClick)
	EVT_BUTTON(ID_WXBUTTONREPLACEEXPAND,MadSearchReplaceDialog::WxButtonReplaceExpandClick)
	EVT_BUTTON(ID_WXBUTTONCOUNT,MadSearchReplaceDialog::WxButtonCountClick)
	EVT_BUTTON(ID_WXBUTTONFINDALLINALL,MadSearchReplaceDialog::WxButtonFindAllInAllClick)
	EVT_BUTTON(ID_WXBUTTONFINDALL,MadSearchReplaceDialog::WxButtonFindAllClick)
	EVT_BUTTON(ID_WXBUTTONFINDPREV,MadSearchReplaceDialog::WxButtonFindPrevClick)
	EVT_BUTTON(ID_WXBUTTONFINDNEXT,MadSearchReplaceDialog::WxButtonFindNextClick)
	EVT_BUTTON(ID_WXBUTTONREPLACEALLINALL,MadSearchReplaceDialog::WxButtonReplaceAllInAllClick)
	EVT_BUTTON(ID_WXBUTTONREPLACEALL,MadSearchReplaceDialog::WxButtonReplaceAllClick)
	EVT_BUTTON(ID_WXBUTTONREPLACE,MadSearchReplaceDialog::WxButtonReplaceClick)
	EVT_COMMAND_SCROLL(ID_WXSLIDERTRANSDEGREE,MadSearchReplaceDialog::WxSliderTransDegreeScroll)
	EVT_RADIOBUTTON(ID_WXRADIOALWAYS,MadSearchReplaceDialog::WxRadioAlwaysClick)
	EVT_RADIOBUTTON(ID_WXRADIOLOSINGFOCUS,MadSearchReplaceDialog::WxRadioLosingFocusClick)
	EVT_CHECKBOX(ID_WXCHECKBOXSEARCHINSELECTION,MadSearchReplaceDialog::WxCheckBoxSearchInSelectionClick)
	EVT_CHECKBOX(ID_WXCHECKBOXFINDHEX,MadSearchReplaceDialog::WxCheckBoxFindHexClick)
	EVT_CHECKBOX(ID_WXCHECKBOXREGEX,MadSearchReplaceDialog::WxCheckBoxRegexClick)
END_EVENT_TABLE()
	////Event Table	End

MadSearchReplaceDialog::MadSearchReplaceDialog(	wxWindow *parent, wxWindowID id, const wxString	&title,	const wxPoint	&position, const wxSize& size, long	style )
	: wxDialog(	parent,	id,	title, position, size, style), m_SearchFrom(-1), m_SearchTo(-1), m_ReplaceDlgUi(false),
	  WxStaticBoxSizer2(0),	WxRadioLosingFocus(0), WxRadioAlways(0), WxSliderTransDegree(0)
{
	m_EnableTransparency = CanSetTransparent();	   
	CreateGUIControls();
}

MadSearchReplaceDialog::~MadSearchReplaceDialog()
{
}

//static int gs_MinX=0;

static void	ResizeItem(wxBoxSizer* sizer, wxWindow *item, int ax, int ay)
{
	int	x, y;
	wxString str=item->GetLabel();
	item->GetTextExtent(str, &x, &y);
	item->SetSize(x+=ax, y+=ay);
	sizer->SetItemMinSize(item,	x, y);

	//wxPoint pos=item->GetPosition();
	//if(pos.x + x > gs_MinX) gs_MinX =	pos.x +	x;
}

void MadSearchReplaceDialog::CreateGUIControls(void)
{
	//Do not add custom	Code here
	//wx-devcpp	designer will remove them.
	//Add the custom code before or	after the Blocks
	////GUI	Items Creation Start

	WxBoxSizer1	= new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2	= new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER	| wxEXPAND | wxALL,	0);

	WxBoxSizer4	= new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer2->Add(WxBoxSizer4, 0, wxALIGN_CENTER	| wxEXPAND | wxALL,	0);

	WxBoxSizer7	= new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer2->Add(WxBoxSizer7, 0, wxALIGN_CENTER	| wxEXPAND | wxALL,	0);

	WxBoxSizer5	= new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer5, 0, wxALIGN_LEFT |	wxEXPAND | wxALIGN_TOP | wxALL,	0);

	WxCheckBoxMoveFocus = new wxCheckBox(this, ID_WXCHECKBOXMOVEFOCUS, _("&Move Focus to Editor Window"), wxPoint(96, 2), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxMoveFocus"));
	WxBoxSizer5->Add(WxCheckBoxMoveFocus, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2);

	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxPoint(96, 28), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxCaseSensitive"));
	WxBoxSizer5->Add(WxCheckBoxCaseSensitive, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2);

	WxCheckBoxWholeWord	= new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxPoint(96, 54), wxSize(300,	22), 0,	wxDefaultValidator,	wxT("WxCheckBoxWholeWord"));
	WxBoxSizer5->Add(WxCheckBoxWholeWord, 0, wxALIGN_LEFT |	wxEXPAND | wxALL, 2);

	WxCheckBoxRegex	= new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxPoint(96, 80), wxSize(300,	22), 0,	wxDefaultValidator,	wxT("WxCheckBoxRegex"));
	WxBoxSizer5->Add(WxCheckBoxRegex, 0, wxALIGN_LEFT |	wxEXPAND | wxALL, 2);

	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxPoint(96, 106), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxFindHex"));
	WxBoxSizer5->Add(WxCheckBoxFindHex, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2);

	WxCheckBoxSearchThrEndOfFile = new wxCheckBox(this, ID_WXCHECKBOXSEARCHTHRENDOFFILE, _("Search Through &End of File"), wxPoint(96, 132), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxSearchThrEndOfFile"));
	WxBoxSizer5->Add(WxCheckBoxSearchThrEndOfFile, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2);

	WxCheckBoxSearchInSelection = new wxCheckBox(this, ID_WXCHECKBOXSEARCHINSELECTION, _("Search In &Selection"), wxPoint(96, 158), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxSearchInSelection"));
	WxBoxSizer5->Add(WxCheckBoxSearchInSelection, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2);

	WxCheckBoxDotMatchNewLine = new wxCheckBox(this, ID_WXCHECKBOXBOXDOTMATCHNEWLINE, _("&. Matches Newline"), wxPoint(96, 184), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxDotMatchNewLine"));
	WxBoxSizer5->Add(WxCheckBoxDotMatchNewLine, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 2);

	WxBoxSizer6	= new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer5->Add(WxBoxSizer6, 0, wxALIGN_CENTER	| wxEXPAND | wxALL,	2);

	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj	= new wxStaticBox(this,	wxID_ANY, _("Bookmark in Searching"));
	WxStaticBoxSizer1 =	new	wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL);
	WxBoxSizer6->Add(WxStaticBoxSizer1,	0, wxALIGN_LEFT	| wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER |	wxALL, 2);

	WxCheckBoxBookmarkLine = new wxCheckBox(this, ID_WXCHECKBOXBOOKMARKLINE, _("Bookmark line"), wxPoint(7,	17), wxSize(270,	22), 0,	wxDefaultValidator,	wxT("WxCheckBoxBookmarkLine"));
	WxStaticBoxSizer1->Add(WxCheckBoxBookmarkLine, 0, wxALIGN_LEFT | wxEXPAND |	wxALL, 2);

	WxCheckBoxBookmarkOnly = new wxCheckBox(this, ID_WXCHECKBOXBOOKMARKONLY, _("Bookmark only(Find All in Current)"), wxPoint(7,	43), wxSize(270, 22), 0, wxDefaultValidator, wxT("WxCheckBoxBookmarkOnly"));
	WxStaticBoxSizer1->Add(WxCheckBoxBookmarkOnly, 0, wxALIGN_CENTER | wxEXPAND	| wxALL, 2);

	WxCheckBoxPurgeBookmark = new wxCheckBox(this, ID_WXCHECKBOXPURGEBOOKMARK, _("Purge mark for each search session"), wxPoint(7, 69), wxSize(270, 22), 0, wxDefaultValidator, wxT("WxCheckBoxPurgeBookmark"));
	WxStaticBoxSizer1->Add(WxCheckBoxPurgeBookmark, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 2);

	if(m_EnableTransparency)
	{
		wxStaticBox* WxStaticBoxSizer2_StaticBoxObj	= new wxStaticBox(this,	wxID_ANY, _("Transparency"));
		WxStaticBoxSizer2 =	new	wxStaticBoxSizer(WxStaticBoxSizer2_StaticBoxObj, wxVERTICAL);
		WxBoxSizer6->Add(WxStaticBoxSizer2,	0, wxALIGN_CENTER |	wxEXPAND | wxALL, 2);

		WxRadioLosingFocus = new wxRadioButton(this, ID_WXRADIOLOSINGFOCUS,	_("On Losing Focus"), wxPoint(17, 17), wxSize(113,	22), 0,	wxDefaultValidator,	wxT("WxRadioLosingFocus"));
		WxStaticBoxSizer2->Add(WxRadioLosingFocus, 0, wxALIGN_CENTER | wxEXPAND	| wxALL, 2);

		WxRadioAlways =	new	wxRadioButton(this,	ID_WXRADIOALWAYS, _("Always"), wxPoint(17, 43),	wxSize(113,	22), 0,	 wxDefaultValidator, wxT("WxRadioAlways"));
		WxStaticBoxSizer2->Add(WxRadioAlways, 0, wxALIGN_CENTER	| wxEXPAND | wxALL,	2);

		WxSliderTransDegree	= new wxSlider(this, ID_WXSLIDERTRANSDEGREE, 25, 25, 255, wxPoint(7, 69), wxSize(113, 22), wxSL_HORIZONTAL | wxSL_SELRANGE , wxDefaultValidator, wxT("WxSliderTransDegree"));
		WxSliderTransDegree->SetRange(25,255);
		WxSliderTransDegree->SetValue(25);
		WxStaticBoxSizer2->Add(WxSliderTransDegree,	0, wxALIGN_CENTER |	wxALL, 2);
	}
	WxBoxSizer3	= new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_TOP | wxEXPAND	| wxALL, 0);

	WxButtonFindNext = new wxButton(this, ID_WXBUTTONFINDNEXT, _("Find &Next"),	wxPoint(2, 2), wxSize(100, 28),	0, wxDefaultValidator,	wxT("WxButtonFindNext"));
	WxBoxSizer3->Add(WxButtonFindNext, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonFindPrev = new wxButton(this, ID_WXBUTTONFINDPREV, _("Find &Previous"),	wxPoint(2, 34),	wxSize(100,	28), 0,	wxDefaultValidator,	wxT("WxButtonFindPrev"));
	WxBoxSizer3->Add(WxButtonFindPrev, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonFindAll	= new wxButton(this, ID_WXBUTTONFINDALL, _("Find &All in\nCurrent"), wxPoint(2,	66), wxSize(100, 42), 0, wxDefaultValidator, wxT("WxButtonFindAll"));
	WxBoxSizer3->Add(WxButtonFindAll, 0, wxALIGN_CENTER	| wxALL, 2);

	WxButtonFindAllInAll = new wxButton(this, ID_WXBUTTONFINDALLINALL, _("Find All in All\nOpened"), wxPoint(2, 112), wxSize(100, 42), 0, wxDefaultValidator, wxT("WxButtonFindAllInAll"));
	WxBoxSizer3->Add(WxButtonFindAllInAll, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonReplace	= new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"),	wxPoint(2, 158), wxSize(100, 28), 0, wxDefaultValidator,	wxT("WxButtonReplace"));
	WxBoxSizer3->Add(WxButtonReplace, 0, wxALIGN_CENTER	| wxALL, 2);

	WxButtonReplaceAll = new wxButton(this,	ID_WXBUTTONREPLACEALL, _("Replace A&ll in\nCurrent"), wxPoint(2, 190), wxSize(100,	42), 0,	wxDefaultValidator,	wxT("WxButtonReplaceAll"));
	WxBoxSizer3->Add(WxButtonReplaceAll, 0,	wxALIGN_CENTER | wxALL,	2);

	WxButtonReplaceAllInAll = new wxButton(this, ID_WXBUTTONREPLACEALLINALL, _("Repl All in All\nOpened"), wxPoint(4, 236), wxSize(100, 42), 0, wxDefaultValidator, wxT("WxButtonReplaceAllInAll"));
	WxBoxSizer3->Add(WxButtonReplaceAllInAll, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonCount =	new	wxButton(this, ID_WXBUTTONCOUNT, _("C&ount"), wxPoint(2, 282), wxSize(100, 28),	0, wxDefaultValidator,	wxT("WxButtonCount"));
	WxBoxSizer3->Add(WxButtonCount,	0, wxALIGN_CENTER |	wxALL, 2);

	WxButtonReplaceExpand =	new	wxButton(this, ID_WXBUTTONREPLACEEXPAND, _("R&eplace >>"), wxPoint(2, 314),	wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonReplaceExpand"));
	WxBoxSizer3->Add(WxButtonReplaceExpand,	0, wxALIGN_CENTER |	wxALL, 2);

	WxButtonClose =	new	wxButton(this, ID_WXBUTTONCLOSE, _("&Close"), wxPoint(2, 346), wxSize(100, 28),	0, wxDefaultValidator,	wxT("WxButtonClose"));
	WxBoxSizer3->Add(WxButtonClose,	0, wxALIGN_CENTER |	wxALL, 2);

	WxPopupMenuRecentFindText =	new	wxMenu(wxT(""));
	WxPopupMenuRecentReplaceText = new wxMenu(wxT(""));
	
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	
	////GUI	Items Creation End

	// removed:	gogo, 19.09.2009
	//this->SetPosition(wxPoint(300,100));

	int	bw,	bh;
	WxButtonFindNext->GetSize(&bw, &bh);

	m_FindText=new MadEdit(this, ID_MADEDIT1, wxPoint(0, 0), wxSize(400, bh));
	m_FindText->SetSingleLineMode(true);

	if(g_ActiveMadEdit)
		m_FindText->SetEncoding(g_ActiveMadEdit->GetEncodingName());
	else
		m_FindText->SetEncoding(wxT("UTF-32LE"));
	m_FindText->SetFixedWidthMode(false);
	m_FindText->SetRecordCaretMovements(false);
	m_FindText->SetInsertSpacesInsteadOfTab(false);
	m_FindText->SetWantTab(false);
	m_FindText->LoadDefaultSyntaxScheme();
	m_FindText->SetMaxLineLength(DEFAULT_MAX_LINELEN);

	WxBoxSizer4->Add(m_FindText,0,wxALIGN_CENTER_HORIZONTAL	| wxALL,2);
	WxBoxSizer4->SetItemMinSize(m_FindText,	400, bh);

	wxBitmap WxBitmapButtonRecentFindText_BITMAP (down_xpm);
	WxBitmapButtonRecentFindText = new wxBitmapButton(this,	ID_WXBITMAPBUTTONRECENTFINDTEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, wxT("WxBitmapButtonRecentFindText"));
	WxBoxSizer4->Add(WxBitmapButtonRecentFindText,0,wxALIGN_CENTER_HORIZONTAL |	wxALL,2);

	// replace
	WxButtonReplace->GetSize(&bw, &bh);

	m_ReplaceText=new MadEdit(this,	ID_MADEDIT2, wxPoint(0,	0),	wxSize(400,	bh));
	m_ReplaceText->SetSingleLineMode(true);
	
	if(g_ActiveMadEdit)
		m_ReplaceText->SetEncoding(g_ActiveMadEdit->GetEncodingName());
	else
		m_ReplaceText->SetEncoding(wxT("UTF-32LE"));
	m_ReplaceText->SetFixedWidthMode(false);
	m_ReplaceText->SetRecordCaretMovements(false);
	m_ReplaceText->SetInsertSpacesInsteadOfTab(false);
	m_ReplaceText->SetWantTab(false);
	m_ReplaceText->LoadDefaultSyntaxScheme();
	m_ReplaceText->SetMaxLineLength(DEFAULT_MAX_LINELEN);

	WxBoxSizer7->Add(m_ReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);
	WxBoxSizer7->SetItemMinSize(m_ReplaceText, 400,	bh);

	WxBitmapButtonRecentReplaceText	= new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTREPLACETEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, wxT("WxBitmapButtonRecentReplaceText"));
	WxBoxSizer7->Add(WxBitmapButtonRecentReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);
	// resize checkbox
	ResizeItem(WxBoxSizer5,	WxCheckBoxMoveFocus, 25, 4);
	ResizeItem(WxBoxSizer5,	WxCheckBoxCaseSensitive, 25, 4);
	ResizeItem(WxBoxSizer5,	WxCheckBoxWholeWord, 25, 4);
	ResizeItem(WxBoxSizer5,	WxCheckBoxRegex, 25, 4);
	ResizeItem(WxBoxSizer5,	WxCheckBoxFindHex, 25, 4);
	ResizeItem(WxBoxSizer5,	WxCheckBoxSearchThrEndOfFile, 25, 4);
	ResizeItem(WxBoxSizer5,	WxCheckBoxSearchInSelection, 25, 4);

	GetSizer()->Fit(this);

	// connect to KeyDown event	handler
	m_FindText->Connect(wxEVT_KEY_DOWN,	wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxBitmapButtonRecentFindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxBitmapButtonRecentReplaceText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxMoveFocus->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxCaseSensitive->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxWholeWord->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxRegex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxFindHex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxSearchThrEndOfFile->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxSearchInSelection->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxBookmarkLine->Connect(wxEVT_KEY_DOWN,	wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxBookmarkOnly->Connect(wxEVT_KEY_DOWN,	wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxCheckBoxPurgeBookmark->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	//WxEditFrom->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	//WxEditTo->Connect(wxEVT_KEY_DOWN,	wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonFindNext->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonFindPrev->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonFindAll->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonReplace->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonReplaceAll->Connect(wxEVT_KEY_DOWN,	wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonCount->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));
	WxButtonClose->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown));

	m_RecentFindText=g_RecentFindText;//new	MadRecentList(20, ID_RECENTFINDTEXT1, true);
	m_RecentFindText->UseMenu(WxPopupMenuRecentFindText);
	m_RecentFindText->AddFilesToMenu();
	m_RecentReplaceText=new	MadRecentList(20, ID_RECENTREPLACETEXT1, true);
	m_RecentReplaceText->UseMenu(WxPopupMenuRecentReplaceText);


	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();

	// add:	gogo, 19.09.2009
	long x = 480, y	= 100;
	m_Config->Read(	wxT("/MadEdit/SearchWinLeft"), &x );
	m_Config->Read(	wxT("/MadEdit/SearchWinTop"), &y );
	m_Config->SetPath(wxT("/RecentReplaceText"));
	m_RecentReplaceText->Load(*m_Config);
	m_Config->SetPath(oldpath);
	SetPosition( wxPoint(x,y) );
	bool bb;
	// I prefer	to read	it here	instead	in ReadWriteSettings(),	because	here
	// it will be read once	per	session	instead	of every 'Ctrl + F'
	m_Config->Read(wxT("/MadEdit/SearchThrEndOfFile"), &bb,	false);
	WxCheckBoxSearchThrEndOfFile->SetValue(	bb );
	//--------
	if(m_EnableTransparency)
	{
		wxASSERT(WxRadioAlways != 0);
		wxASSERT(WxRadioLosingFocus	!= 0);
		wxASSERT(WxSliderTransDegree !=	0);
		long trans = 25;
		if(g_SearchReplaceDialog)
		{
			bb = g_SearchReplaceDialog->WxRadioAlways->GetValue();
			trans =	g_SearchReplaceDialog->WxSliderTransDegree->GetValue();
		}
		else
		{
			m_Config->Read(wxT("/MadEdit/AlwaysTransparent"), &bb, false);
			m_Config->Read(	wxT("/MadEdit/Transparency"), &trans );
		}
		WxRadioAlways->SetValue(bb);
		WxRadioLosingFocus->SetValue(!bb);
		WxSliderTransDegree->SetValue(trans);
		if(bb)
		{
			SetTransparent(trans);
		}
	}

	//m_Config->SetPath(wxT("/RecentFindText"));
	//m_RecentFindText->Load(*m_Config);
	//m_Config->SetPath(oldpath);

	if(m_RecentFindText->GetCount()>0)
	{
		wxString text=m_RecentFindText->GetHistoryFile(0);
		if(!text.IsEmpty())
		{
			m_FindText->SetText(text);
		}
	}
	
	if(m_RecentReplaceText->GetCount()>0)
	{
		wxString text=m_RecentReplaceText->GetHistoryFile(0);
		if(!text.IsEmpty())
		{
			m_ReplaceText->SetText(text);
		}
	}

	SetDefaultItem(WxButtonFindNext);
}

void MadSearchReplaceDialog::MadSearchReplaceDialogClose(wxCloseEvent& event)
{
	// --> Don't use Close with	a Dialog,
	// use Destroy instead.

	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_SearchReplaceDialog=NULL;
	Destroy();
}


/*
 * WxButtonCloseClick
 */
void MadSearchReplaceDialog::WxButtonCloseClick(wxCommandEvent&	event)
{
	Show(false);

	((wxFrame*)wxTheApp->GetTopWindow())->Raise();

	if(g_ActiveMadEdit!=NULL)
	{
		g_ActiveMadEdit->Refresh(false);
		g_ActiveMadEdit->SetFocus();
	}
}

/*
 * WxButtonFindNextClick
 */
void MadSearchReplaceDialog::WxButtonFindNextClick(wxCommandEvent& event)
{
	// add:	gogo, 19.09.2009
	g_StatusBar->SetStatusText(	wxEmptyString, 0 );
	bool ThrEndOfFile =	false;

	wxString text,expr;
	m_FindText->GetText(text, true);

	if(text.Len()>0)
	{
		m_RecentFindText->AddFileToHistory(text);

		MadSearchResult	sr;
		wxFileOffset selend	= g_ActiveMadEdit->GetSelectionEndPos();

		// moved here: gogo, 19.09.2009
		wxFileOffset caretpos =	g_ActiveMadEdit->GetCaretPosition();

		//wxInt64 from = 0,	to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			rangeTo	= m_SearchTo;
			// removed:	gogo, 19.09.2009
			//wxFileOffset caretpos	= g_ActiveMadEdit->GetCaretPosition();
			if(caretpos	<= m_SearchFrom	|| caretpos	>= m_SearchTo)
				rangeFrom =	m_SearchFrom;
		}

		g_ActiveMadEdit->SetBookmarkInSearch(WxCheckBoxBookmarkLine->IsChecked());

		expr = text;
		expr.Replace(wxT("\\"),	wxT("\\\\"));
		expr.Replace(wxT("\""),	wxT("\\\""));
		for(;;)
		{
			if(WxCheckBoxFindHex->GetValue())
			{
				sr=g_ActiveMadEdit->FindHexNext(text, rangeFrom, rangeTo);
				if(sr != SR_EXPR_ERROR)
				{
					RecordAsMadMacro(g_ActiveMadEdit, wxString::Format(wxT("FindHexNext(\"%s\", %s, %s)"), expr.c_str(),
								(wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
				}
			}
			else
			{
				bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				sr=g_ActiveMadEdit->FindTextNext(text,
					bRegex,
					WxCheckBoxCaseSensitive->GetValue(),
					bWholeWord,
					bDotMatchNewline,
					rangeFrom, rangeTo);
				if(sr != SR_EXPR_ERROR)
				{
					wxString fnstr(wxString::Format(wxT("FindTextNext(\"%s\", %s, %s, %s, %s, %s, %s)"), expr.c_str(),
									bRegex?wxT("True"):wxT("False"),
									WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
									bWholeWord?wxT("True"):wxT("False"),
									bDotMatchNewline?wxT("True"):wxT("False"),
									(wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
					RecordAsMadMacro(g_ActiveMadEdit, fnstr);
				}
			}

			if(sr != SR_NO)
			{
				if(sr == SR_YES	&& g_ActiveMadEdit->GetCaretPosition() == selend)
				{
					selend = -1;
					continue;
				}

				// add:	gogo, 19.09.2009
				if ( ThrEndOfFile )
					g_StatusBar->SetStatusText(	_("Passed the end of the file"), 0 );

				break;
			}

			// add:	gogo, 19.09.2009
			if ( WxCheckBoxSearchThrEndOfFile->IsChecked() && !	WxCheckBoxSearchInSelection->IsChecked() )
			{
				if ( rangeTo ==	caretpos )
				{
					g_StatusBar->SetStatusText(	_("Cannot find the matched string"), 0 );
					break;
				}
				rangeTo	= caretpos;
				rangeFrom =	0;
				ThrEndOfFile = true;
				continue;
			}
			//--------------

			wxString msg(_("Cannot find the matched string."));
			msg += wxT("\n\n");
			msg += WxCheckBoxSearchInSelection->IsChecked()?
				_("Do you want to find from begin of selection?"):
				_("Do you want to find from begin of file?");

			if(wxCANCEL	== MadMessageBox(msg, _("Find Next"), wxOK|wxCANCEL
#if	(wxMAJOR_VERSION ==	2 && wxMINOR_VERSION > 9)
				|wxCANCEL_DEFAULT
#endif
				|wxICON_QUESTION ))
			{
				break;
			}
			rangeFrom =	WxCheckBoxSearchInSelection->IsChecked()? m_SearchFrom : 0;
			rangeTo	= WxCheckBoxSearchInSelection->IsChecked()?	m_SearchTo:	g_ActiveMadEdit->GetFileSize();
			if(WxCheckBoxSearchInSelection->IsChecked())
			{
				g_ActiveMadEdit->SetSelection(m_SearchFrom,	m_SearchTo);
			}
			
			if(WxCheckBoxPurgeBookmark->IsChecked())
				g_ActiveMadEdit->ClearAllBookmarks();
		}
	}
	else
	{
		g_StatusBar->SetStatusText(	_("Empty expression"), 0 );
	}

	if(WxCheckBoxMoveFocus->GetValue())
	{
		((wxFrame*)wxTheApp->GetTopWindow())->Raise();
		g_ActiveMadEdit->SetFocus();
	}
}

/*
 * WxButtonFindPrevClick
 */
void MadSearchReplaceDialog::WxButtonFindPrevClick(wxCommandEvent& event)
{
	extern MadEdit *g_ActiveMadEdit;

	if(g_ActiveMadEdit==NULL)
		return;

	// add:	gogo, 19.09.2009
	g_StatusBar->SetStatusText(	wxEmptyString, 0 );
	bool ThrEndOfFile =	false;

	wxString text, expr;
	m_FindText->GetText(text, true);

	if(text.Len()>0)
	{
		m_RecentFindText->AddFileToHistory(text);

		MadSearchResult	sr;
		wxFileOffset selbeg	= g_ActiveMadEdit->GetSelectionBeginPos();

		// moved here: gogo, 19.09.2009
		wxFileOffset caretpos =	g_ActiveMadEdit->GetCaretPosition();

		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{

			rangeFrom =	m_SearchFrom;
			// removed:	gogo, 19.09.2009
			//wxFileOffset caretpos	= g_ActiveMadEdit->GetCaretPosition();
			if(caretpos	<= m_SearchFrom	|| caretpos	>= m_SearchTo)
				rangeFrom =	m_SearchFrom;
		}

		expr = text;
		expr.Replace(wxT("\\"),	wxT("\\\\"));
		expr.Replace(wxT("\""),	wxT("\\\""));

		g_ActiveMadEdit->SetBookmarkInSearch(WxCheckBoxBookmarkLine->IsChecked());

		for(;;)
		{
			if(WxCheckBoxFindHex->GetValue())
			{
				sr=g_ActiveMadEdit->FindHexPrevious(text, rangeTo, rangeFrom);
				RecordAsMadMacro(g_ActiveMadEdit, wxString::Format(wxT("FindHexPrevious(\"%s\", %s, %s)"), expr.c_str(), (wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
			}
			else
			{
			
				bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				sr=g_ActiveMadEdit->FindTextPrevious(text,
					bRegex,
					WxCheckBoxCaseSensitive->GetValue(),
					bWholeWord,
					bDotMatchNewline,
					rangeTo, rangeFrom);
				
					wxString fnstr(wxString::Format(wxT("FindTextPrevious(\"%s\", %s, %s, %s, %s, %s, %s)"), expr.c_str(),
							bRegex?wxT("True"):wxT("False"),
							WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
							bWholeWord?wxT("True"):wxT("False"), 
							bDotMatchNewline?wxT("True"):wxT("False"),
							(wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
					RecordAsMadMacro(g_ActiveMadEdit, fnstr);
			}

			if(sr!=SR_NO)
			{
				if(sr == SR_YES	&& g_ActiveMadEdit->GetCaretPosition() == selbeg)
				{
					selbeg = -1;
					continue;
				}

				// add:	gogo, 19.09.2009
				if ( ThrEndOfFile )
					g_StatusBar->SetStatusText(	_("Passed the end of the file"), 0 );

				break;
			}

			// add:	gogo, 19.09.2009
			if ( WxCheckBoxSearchThrEndOfFile->IsChecked() && !	WxCheckBoxSearchInSelection->IsChecked() )
			{
				if ( rangeFrom == caretpos )
				{
					g_StatusBar->SetStatusText(	_("Cannot find the matched string"), 0 );
					break;
				}
				rangeTo	= g_ActiveMadEdit->GetFileSize();
				rangeFrom =	caretpos;
				ThrEndOfFile = true;
				continue;
			}
			//--------------

			wxString msg(_("Cannot find the matched string."));
			msg += wxT("\n\n");
			msg += WxCheckBoxSearchInSelection->IsChecked()?
				_("Do you want to find from end of selection?"):
				_("Do you want to find from end of file?");

			if(wxCANCEL==MadMessageBox(msg, _("Find Previous"), wxOK|wxCANCEL|wxICON_QUESTION ))
			{
				break;
			}
			rangeTo	= WxCheckBoxSearchInSelection->IsChecked()?	m_SearchTo:	g_ActiveMadEdit->GetFileSize();
			rangeFrom =	WxCheckBoxSearchInSelection->IsChecked()? m_SearchFrom:	0;

			if(WxCheckBoxPurgeBookmark->IsChecked())
				g_ActiveMadEdit->ClearAllBookmarks();
		}
	}
	else
	{
		g_StatusBar->SetStatusText(	_("Empty expression"), 0 );
	}

	if(WxCheckBoxMoveFocus->GetValue())
	{
		((wxFrame*)wxTheApp->GetTopWindow())->Raise();
		g_ActiveMadEdit->SetFocus();
	}
}


/*
 * MadSearchReplaceDialogKeyDown
 */
void MadSearchReplaceDialog::MadSearchReplaceDialogKeyDown(wxKeyEvent& event)
{
	int	key=event.GetKeyCode();

	//g_SearchReplaceDialog->SetTitle(wxString()<<key);

	switch(key)
	{
	case WXK_ESCAPE:
		g_SearchReplaceDialog->Show(false);
		return;
	case WXK_RETURN:
	case WXK_NUMPAD_ENTER:
		if(this->GetClassInfo()->GetClassName()!=wxString(wxT("wxButton")))
		{
			wxCommandEvent e;
			wxButton* default_btn =	static_cast<wxButton*>(g_SearchReplaceDialog->GetDefaultItem());
			if(default_btn == g_SearchReplaceDialog->WxButtonReplace)
				return g_SearchReplaceDialog->WxButtonReplaceClick(e); // no skip

			return g_SearchReplaceDialog->WxButtonFindNextClick(e);	// no skip
		}

		break;
	case WXK_DOWN:
		if((MadEdit*)this==g_SearchReplaceDialog->m_FindText)
		{
			int	x,y,w,h;
			g_SearchReplaceDialog->m_FindText->GetPosition(&x, &y);
			g_SearchReplaceDialog->m_FindText->GetSize(&w, &h);
			g_SearchReplaceDialog->PopupMenu(g_SearchReplaceDialog->WxPopupMenuRecentFindText, x, y+h);
			return;
		}
		else if((MadEdit*)this==g_SearchReplaceDialog->m_ReplaceText)
		{
			int	x,y,w,h;
			g_SearchReplaceDialog->m_ReplaceText->GetPosition(&x, &y);
			g_SearchReplaceDialog->m_ReplaceText->GetSize(&w, &h);
			g_SearchReplaceDialog->PopupMenu(g_SearchReplaceDialog->WxPopupMenuRecentReplaceText, x, y+h);
			return;
		}
		break;
	}

	extern wxAcceleratorEntry g_AccelFindNext, g_AccelFindPrev;
	int	flags=wxACCEL_NORMAL;
	if(event.m_altDown)	flags|=wxACCEL_ALT;
	if(event.m_shiftDown) flags|=wxACCEL_SHIFT;
	if(event.m_controlDown)	flags|=wxACCEL_CTRL;

	if(g_AccelFindNext.GetKeyCode()==key &&	g_AccelFindNext.GetFlags()==flags)
	{
		wxCommandEvent e;
		g_SearchReplaceDialog->WxButtonFindNextClick(e);
		return;	// no skip
	}

	if(g_AccelFindPrev.GetKeyCode()==key &&	g_AccelFindPrev.GetFlags()==flags)
	{
		wxCommandEvent e;
		g_SearchReplaceDialog->WxButtonFindPrevClick(e);
		return;	// no skip
	}

	event.Skip();
}

/*
 * WxBitmapButtonRecentFindTextClick
 */
void MadSearchReplaceDialog::WxBitmapButtonRecentFindTextClick(wxCommandEvent& event)
{
	PopupMenu(WxPopupMenuRecentFindText);
}

void MadSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event)
{
	PopupMenu(WxPopupMenuRecentReplaceText);
}

void MadSearchReplaceDialog::OnRecentFindText(wxCommandEvent& event)
{
	int	idx=event.GetId()-ID_RECENTFINDTEXT1;
	wxString text=m_RecentFindText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_FindText->SetText(text);
		m_FindText->SetFocus();
	}
}

void MadSearchReplaceDialog::OnRecentReplaceText(wxCommandEvent& event)
{
	int	idx=event.GetId()-ID_RECENTREPLACETEXT1;
	wxString text=m_RecentReplaceText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_ReplaceText->SetText(text);
		m_ReplaceText->SetFocus();
	}
}

void MadSearchReplaceDialog::ReadWriteSettings(bool	bRead)
{
	extern MadEdit *g_ActiveMadEdit;
	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();

	if(bRead)
	{
		bool bb;
		m_Config->Read(wxT("/MadEdit/SearchMoveFocus"),	&bb, false);
		WxCheckBoxMoveFocus->SetValue(bb);

		m_Config->Read(wxT("/MadEdit/SearchCaseSensitive"),	&bb, false);
		WxCheckBoxCaseSensitive->SetValue(bb);

		m_Config->Read(wxT("/MadEdit/SearchWholeWord"),	&bb, false);
		WxCheckBoxWholeWord->SetValue(bb);

		m_Config->Read(wxT("/MadEdit/SearchRegex"),	&bb, false);
		WxCheckBoxRegex->SetValue(bb);

		m_Config->Read(wxT("/MadEdit/SearchHex"), &bb, false);
		WxCheckBoxFindHex->SetValue(bb);
		UpdateCheckBoxByCBHex(bb);

		m_Config->Read(wxT("/MadEdit/SearchInSelection"), &bb, false);
		WxCheckBoxSearchInSelection->SetValue(bb);
		UpdateSearchInSelection(bb);
		
		m_Config->Read(wxT("/MadEdit/SearchBookmarkLines"),	&bb, false);
		WxCheckBoxBookmarkLine->SetValue(bb);

		m_Config->Read(wxT("/MadEdit/SearchBookmarkOnly"), &bb,	false);
		WxCheckBoxBookmarkOnly->SetValue(bb);
		
		m_Config->Read(wxT("/MadEdit/SearchPurgeBookmark"),	&bb, false);
		WxCheckBoxPurgeBookmark->SetValue(bb);
	}
	else
	{
		m_Config->Write(wxT("/MadEdit/SearchMoveFocus"), WxCheckBoxMoveFocus->GetValue());
		m_Config->Write(wxT("/MadEdit/SearchCaseSensitive"), WxCheckBoxCaseSensitive->GetValue());
		m_Config->Write(wxT("/MadEdit/SearchWholeWord"), WxCheckBoxWholeWord->GetValue());
		m_Config->Write(wxT("/MadEdit/SearchRegex"), WxCheckBoxRegex->GetValue());
		m_Config->Write(wxT("/MadEdit/SearchHex"), WxCheckBoxFindHex->GetValue());

		m_Config->Write(wxT("/MadEdit/SearchInSelection"), WxCheckBoxSearchInSelection->GetValue());
		//m_Config->Write(wxT("/MadEdit/SearchFrom"), (wxLongLong(m_SearchFrom)).ToString());
		//m_Config->Write(wxT("/MadEdit/SearchTo"),	(wxLongLong(m_SearchTo)).ToString());
		m_Config->Write(wxT("/MadEdit/SearchBookmarkLines"), WxCheckBoxBookmarkLine->GetValue());
		m_Config->Write(wxT("/MadEdit/SearchBookmarkOnly"),	WxCheckBoxBookmarkOnly->GetValue());
		m_Config->Write(wxT("/MadEdit/SearchPurgeBookmark"), WxCheckBoxPurgeBookmark->GetValue());
	}

	m_Config->SetPath(oldpath);
}

static inline ucs4_t ToHex(int d)//	0 <= d <= 15
{
	if(d < 10)
		return '0' + d;
	return 'A' + d - 10;
}

void MadSearchReplaceDialog::UpdateCheckBoxByCBHex(bool check)
{
	if(check)
	{
		WxCheckBoxCaseSensitive->Disable();
		WxCheckBoxWholeWord->Disable();
		WxCheckBoxRegex->Disable();
		WxCheckBoxDotMatchNewLine->Disable();
	}
	else
	{
		WxCheckBoxCaseSensitive->Enable();
		WxCheckBoxRegex->Enable();
		WxCheckBoxDotMatchNewLine->Enable();
		if(WxCheckBoxRegex->GetValue())
		{
			WxCheckBoxDotMatchNewLine->Enable();
			WxCheckBoxWholeWord->Disable();
		}
		else
		{
			WxCheckBoxWholeWord->Enable();
			WxCheckBoxDotMatchNewLine->Disable();
		}
	}
}

/*
 * WxCheckBoxFindHexClick
 */
void MadSearchReplaceDialog::WxCheckBoxFindHexClick(wxCommandEvent&	event)
{
	extern MadEdit *g_ActiveMadEdit;
	bool checked = event.IsChecked();
	wxString text, ws;
	if(checked)
	{
		m_FindText->GetText(text, true);
		if(text.IsEmpty())
		{
			if(g_ActiveMadEdit!=NULL)
			{
				g_ActiveMadEdit->GetSelHexString(ws, true);
				m_FindText->SetText(ws);
			}
		}
		else
		{
			m_FindText->SelectAll();
			m_FindText->GetSelHexString(ws,	true);
			m_FindText->SetText(ws);
		}
	}
	else
	{
		m_FindText->GetText(text, true);
		if(text.IsEmpty())
		{
			if(g_ActiveMadEdit!=NULL)
			{
				g_ActiveMadEdit->GetSelText(ws);
				m_FindText->SetText(ws);
			}
		}
	}
	UpdateCheckBoxByCBHex(event.IsChecked());
}

/*
 * WxButtonReplaceClick
 */
void MadSearchReplaceDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
	if(g_ActiveMadEdit==NULL)
		return;

	wxString text, expr;
	m_FindText->GetText(text, true);

	if(text.Len()>0)
	{
		wxString reptext;
		m_ReplaceText->GetText(reptext,	true);

		g_SearchReplaceDialog->m_RecentFindText->AddFileToHistory(text);

		if(reptext.Len()>0)
		{
			m_RecentReplaceText->AddFileToHistory(reptext);
		}

		wxFileOffset rangeFrom = -1, rangeTo = -1;
		wxFileOffset caretpos =	g_ActiveMadEdit->GetCaretPosition();
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			rangeTo	= m_SearchTo;
			if(caretpos	<= m_SearchFrom	|| caretpos	> m_SearchTo)
				rangeFrom =	m_SearchFrom;
		}
		else
		{
			rangeFrom =	caretpos;
		}

		expr = text;
		expr.Replace(wxT("\\"),	wxT("\\\\"));
		expr.Replace(wxT("\""),	wxT("\\\""));

		for(;;)
		{
			MadReplaceResult ret=RR_EXPR_ERROR;
			if(WxCheckBoxFindHex->GetValue())
			{
				ret=g_ActiveMadEdit->ReplaceHex(text, reptext, rangeFrom, rangeTo);
				
				if(ret == RR_REP_NEXT || ret ==	RR_NREP_NEXT)
				{
					RecordAsMadMacro(g_ActiveMadEdit, wxString::Format(wxT("ReplaceHex(\"%s\", \"%s\", %s, %s)"), expr.c_str(), reptext.c_str(), (wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
				}
			}
			else
			{
				bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				ret=g_ActiveMadEdit->ReplaceText(text, reptext,
					bRegex,
					WxCheckBoxCaseSensitive->GetValue(),
					bWholeWord,
					bDotMatchNewline,
					rangeFrom, rangeTo);
				if(ret == RR_REP_NEXT || ret ==	RR_NREP_NEXT)
				{
					wxString fnstr(wxString::Format(wxT("ReplaceText(\"%s\", \"%s\", %s, %s, %s, %s, %s, %s)"),	expr.c_str(), reptext.c_str(),
										bRegex?wxT("True"):wxT("False"),
										WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
										bWholeWord?wxT("True"):wxT("False"),
										bDotMatchNewline?wxT("True"):wxT("False"),
										(wxLongLong(rangeFrom).ToString()).c_str(),	(wxLongLong(rangeTo).ToString()).c_str()));
					RecordAsMadMacro(g_ActiveMadEdit, fnstr);
				}
			}

			switch(ret)
			{
			case RR_REP_NNEXT:
			case RR_NREP_NNEXT:
				// handle latter
				break;
			case RR_REP_NEXT:
			case RR_NREP_NEXT:
				if(WxCheckBoxMoveFocus->GetValue())
				{
					((wxFrame*)wxTheApp->GetTopWindow())->Raise();
					g_ActiveMadEdit->SetFocus();
					
					if(WxCheckBoxFindHex->GetValue())
					{
						RecordAsMadMacro(g_ActiveMadEdit, wxString::Format(wxT("ReplaceHex(\"%s\", \"%s\", %s, %s)"), expr.c_str(), reptext.c_str(),	(wxLongLong(rangeFrom).ToString()).c_str(),	(wxLongLong(rangeTo).ToString()).c_str()));
					}
					else
					{
						bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
						if(bRegex) bWholeWord = false;
						else bDotMatchNewline = false;
						wxString fnstr(wxString::Format(wxT("ReplaceText(\"%s\", \"%s\", %s, %s, %s, %s, %s, %s)"),	expr.c_str(), reptext.c_str(),
										bRegex?wxT("True"):wxT("False"),
										WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
										bWholeWord?wxT("True"):wxT("False"),
										bDotMatchNewline?wxT("True"):wxT("False"),
										(wxLongLong(rangeFrom).ToString()).c_str(),	(wxLongLong(rangeTo).ToString()).c_str()));
						RecordAsMadMacro(g_ActiveMadEdit, fnstr);
					}
				}
				break;
			default:
				break;
			}
			if(ret == RR_REP_NNEXT || ret == RR_NREP_NNEXT)
			{
				wxString msg(_("Cannot find the matched string.\nReplace is finished."));
				msg += wxT("\n\n");
				msg += WxCheckBoxSearchInSelection->IsChecked()?
					_("Do you want to find from begin of selection?"):
					_("Do you want to find from begin of file?");

				if(wxCANCEL	== MadMessageBox(msg, _("Find Next"), wxOK|wxCANCEL
#if	(wxMAJOR_VERSION ==	2 && wxMINOR_VERSION > 9)
					|wxCANCEL_DEFAULT
#endif
					|wxICON_QUESTION ))
				{
					m_FindText->SetFocus();
					break;
				}

				if(WxCheckBoxSearchInSelection->IsChecked())
				{
					rangeFrom =	m_SearchFrom;
					rangeTo	= m_SearchTo;
					g_ActiveMadEdit->SetSelection(m_SearchFrom,	m_SearchTo);
				}
				else
				{
					rangeFrom =	0;
					rangeTo	= -1;
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
void MadSearchReplaceDialog::WxButtonReplaceAllClick(wxCommandEvent& event)
{
	if(g_ActiveMadEdit!=NULL)
		ReplaceAll(g_ActiveMadEdit);
}

void MadSearchReplaceDialog::WxButtonReplaceAllInAllClick(wxCommandEvent& event)
{
	int	count =	int( ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageCount() );

	for( int id	= 0; id	< count; ++id )
	{
		MadEdit	*madedit = ( MadEdit* )((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPage( id );
		ReplaceAll(madedit,	madedit==g_ActiveMadEdit);
	}
}

/*
 * MadSearchReplaceDialogActivate
 */
void MadSearchReplaceDialog::MadSearchReplaceDialogActivate(wxActivateEvent& event)
{
	ReadWriteSettings(event.GetActive());
	if(event.GetActive())
	{
		if(g_ActiveMadEdit)
		{
			wxString fname;
			int	fsize;
			g_ActiveMadEdit->GetFont( fname, fsize );
			m_FindText->SetEncoding( g_ActiveMadEdit->GetEncodingName()	);
			m_FindText->SetFont( fname,	14 );
			m_FindText->SetSpellCheck(g_ActiveMadEdit->GetSpellCheckStatus());
			m_ReplaceText->SetEncoding(	g_ActiveMadEdit->GetEncodingName() );
			m_ReplaceText->SetFont(	fname, 14 );
			m_ReplaceText->SetSpellCheck(g_ActiveMadEdit->GetSpellCheckStatus());
		}
		UpdateCheckBoxByCBHex( WxCheckBoxFindHex->GetValue() );

		if(m_EnableTransparency	&& WxRadioLosingFocus->GetValue())
		{
			SetTransparent(wxIMAGE_ALPHA_OPAQUE);
		}
	}
}


/*
 * WxButtonReplaceClick
 */
void MadSearchReplaceDialog::WxButtonReplaceExpandClick(wxCommandEvent&	event)
{
	if(!m_ReplaceDlgUi)
	{
		wxString fname;
		int	fsize;
		m_FindText->GetFont(fname, fsize);
		m_FindText->SetFont(fname, 14);
		m_ReplaceText->SetFont(fname, 14);

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
	//WxButtonReplaceExpand->Show(false);
	m_ReplaceDlgUi = true;
	WxButtonFindAllInAll->Show(false);
	WxButtonFindAll->Show(false);
	WxButtonReplaceExpand->SetLabel(_("Search <<"));

	m_ReplaceText->Show(true);
	WxBitmapButtonRecentReplaceText->Show(true);
	WxButtonReplace->Show(true);
	WxButtonReplaceAll->Show(true);
	WxButtonReplaceAllInAll->Show(true);

	WxButtonReplace->SetDefault();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	SetTitle(_("Replace"));

	Show(true);
	SetFocus();
	Raise();
}

void MadSearchReplaceDialog::ShowFindUI()
{
	m_ReplaceDlgUi = false;
	m_ReplaceText->Show(false);
	WxBitmapButtonRecentReplaceText->Show(false);
	WxButtonReplace->Show(false);
	WxButtonReplaceAll->Show(false);
	WxButtonReplaceAllInAll->Show(false);

	WxButtonFindAllInAll->Show(true);
	WxButtonFindAll->Show(true);
	//WxButtonReplaceExpand->Show(true);
	WxButtonReplaceExpand->SetLabel(_("R&eplace >>"));

	WxButtonFindNext->SetDefault();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	SetTitle(_("Search"));

	Show(true);
	SetFocus();
	Raise();
}

void MadSearchReplaceDialog::UpdateSearchInSelection(bool check)
{
	//WxEditFrom->Enable(check);
	//WxEditTo->Enable(check);

	extern MadEdit *g_ActiveMadEdit;
	if(check &&	g_ActiveMadEdit!=NULL)
	{
		m_SearchFrom = wxLongLong(g_ActiveMadEdit->GetSelectionBeginPos()).GetValue();
		m_SearchTo = wxLongLong(g_ActiveMadEdit->GetSelectionEndPos()).GetValue();
	}
	else
	{
		m_SearchFrom = -1;
		m_SearchTo = -1;
	}

	// add:	gogo, 19.09.2009
	if(check)
		WxCheckBoxSearchThrEndOfFile->Disable();
	else
		WxCheckBoxSearchThrEndOfFile->Enable();
}


void MadSearchReplaceDialog::WxCheckBoxSearchInSelectionClick(wxCommandEvent& event)
{
	UpdateSearchInSelection(event.IsChecked());
}

void MadSearchReplaceDialog::WxButtonCountClick(wxCommandEvent&	event)
{
	extern MadEdit *g_ActiveMadEdit;

	if(g_ActiveMadEdit==NULL)
		return;

	int	count =	0;
	wxString text;
	m_FindText->GetText(text, true);

	if(text.Len()>0)
	{
		m_RecentFindText->AddFileToHistory(text);

		wxInt64	from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			rangeTo	= m_SearchTo;
			rangeFrom =	m_SearchFrom;
		}

		if(WxCheckBoxFindHex->GetValue())
		{
			count=g_ActiveMadEdit->FindHexAll(text,	false, NULL, NULL, rangeFrom, rangeTo);
		}
		else
		{
			bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
			if(bRegex) bWholeWord = false;
			else bDotMatchNewline = false;
			count=g_ActiveMadEdit->FindTextAll(text,
				bRegex,
				WxCheckBoxCaseSensitive->GetValue(),
				bWholeWord,
				bDotMatchNewline,
				false,
				NULL, NULL,
				rangeFrom, rangeTo);
		}
	}

	if(count >=	0)
	{
		wxString msg;
		msg.Printf(_("'%s' was found %d times."), text.c_str(), count);
		MadMessageBox(msg, wxT("MadEdit-Mod"), wxOK);
	}
}

void DisplayFindAllResult(vector<wxFileOffset> &begpos,	vector<wxFileOffset> &endpos, MadEdit *madedit,	bool expandresults =	true, OnProgressUpdatePtr updater =	NULL)
{
	int	ResultCount=0;
	wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;

	if(!begpos.empty())	// found data
	{
		int	pid	= ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageIndex(madedit);
		wxString fmt, filename=madedit->GetFileName();

		if(filename.IsEmpty())
		{
			if(pid>=0)
			{
				filename=((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageText(pid);
				if(filename[filename.Len()-1]==wxT('*'))
					filename.Truncate(filename.Len()-1);
			}
		}
		if(!filename.IsEmpty())
		{
			size_t count=begpos.size(),	idx=0;
			int	line=-1, oldline;
			wxString linetext, loc;
			results->Freeze();
			
			wxString status = _("Preparing %s of %s results...");
			status += wxT(" 							   \n");
			do
			{
				if(madedit->IsTextFile())
				{
					oldline=line;
					line=madedit->GetLineByPos(begpos[idx]);
					if(line!=oldline)
					{
						linetext.Empty();
						madedit->GetLine(linetext, line, 1024);
					}
					loc.Printf(_("Line(%d): "), line+1);
				}
				else
				{
					loc.Printf(_("Offset(%s): "), wxLongLong(begpos[idx]).ToString().c_str());
					linetext = _("Binary file matches");
				}

				fmt	= loc +linetext;
				g_MainFrame->AddItemToFindInFilesResults(fmt, idx, filename, pid, begpos[idx], endpos[idx]);
				++ResultCount;
				if(updater != NULL && (count >=	2000))
				{
					if(updater(idx,	wxString::Format(status, (wxLongLong(idx).ToString().c_str()), (wxLongLong(count).ToString().c_str())),	NULL)==	false) break;
				}
			}
			while(++idx	< count);
			results->Thaw();
			if(results->GetCount())
			{
				if(expandresults) results->ExpandAll();
				g_MainFrame->m_AuiManager.GetPane(g_MainFrame->m_InfoNotebook).Show();
				g_MainFrame->m_AuiManager.Update();
			}
		}
	}

	if(!ResultCount)
	{
		g_StatusBar->SetStatusText(	_("Cannot find the matched string"), 0 );
	}
	else
	{
		wxString smsg(wxLongLong(begpos.size()).ToString() + _(" results"));
		g_StatusBar->SetStatusText(smsg, 0 );
	}
}

void MadSearchReplaceDialog::WxButtonFindAllClick(wxCommandEvent& event)
{
	extern MadEdit *g_ActiveMadEdit;

	if(g_ActiveMadEdit)
		SearchAll(g_ActiveMadEdit);
}

void MadSearchReplaceDialog::WxButtonFindAllInAllClick(wxCommandEvent& event)
{
	extern MadEdit *g_ActiveMadEdit;

	int	count =	int( ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageCount() );

	for( int id	= 0; id	< count; ++id )
	{
		MadEdit	*madedit = ( MadEdit* )((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPage( id );
		SearchAll(madedit, madedit==g_ActiveMadEdit);
	}
}

void MadSearchReplaceDialog::PurgeRecentFindTexts()
{
	int	n =	(int) m_RecentFindText->GetCount();
	for(int	i=n-1; i>=0; --i)
		m_RecentFindText->RemoveFileFromHistory((size_t)i);
}

void MadSearchReplaceDialog::SearchAll(MadEdit * madedit, bool needRec/*=true*/)
{
	vector<wxFileOffset> begpos, endpos;

	// get all matched data	in madedit
	wxString expr, fmt;
	int	ok;
	m_FindText->GetText(expr);
	if(expr.Len()>0)
	{
		begpos.reserve(128*1024);
		endpos.reserve(128*1024);
		m_RecentFindText->AddFileToHistory(expr);
		madedit->SetBookmarkInSearch(WxCheckBoxBookmarkLine->IsChecked());

		if(WxCheckBoxPurgeBookmark->IsChecked())
			madedit->ClearAllBookmarks();
		wxFileOffset selend	= madedit->GetSelectionEndPos();

		// moved here: gogo, 19.09.2009
		wxFileOffset caretpos =	madedit->GetCaretPosition();
		//wxInt64 from = 0,	to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			rangeTo	= m_SearchTo;
			// removed:	gogo, 19.09.2009
			//wxFileOffset caretpos	= g_ActiveMadEdit->GetCaretPosition();
			if(caretpos	<= m_SearchFrom	|| caretpos	>= m_SearchTo)
				rangeFrom =	m_SearchFrom;
		}

		if(WxCheckBoxFindHex->GetValue())
		{
			ok = madedit->FindHexAll(expr, false, &begpos, &endpos,	rangeFrom, rangeTo);
			if(needRec)
				RecordAsMadMacro(madedit, wxString::Format(wxT("FindHexAll(\"%s\")"), expr.c_str()));
		}
		else
		{
			bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
			if(bRegex) bWholeWord = false;
			else bDotMatchNewline = false;
			ok = madedit->FindTextAll(expr,
				bRegex,
				WxCheckBoxCaseSensitive->GetValue(),
				bWholeWord,
				bDotMatchNewline,
				false,
				&begpos, &endpos, rangeFrom, rangeTo);

			expr.Replace(wxT("\\"),	wxT("\\\\"));
			expr.Replace(wxT("\""),	wxT("\\\""));
			wxString fnstr(wxString::Format(wxT("FindTextAll(\"%s\", %s, %s, %s, %s, False)"),	expr.c_str(),
							bRegex?wxT("True"):wxT("False"),
							WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
							bWholeWord?wxT("True"):wxT("False"),
							bDotMatchNewline?wxT("True"):wxT("False")));
			if(needRec)
				RecordAsMadMacro(madedit, fnstr);
		}

		if(ok<0) return;

		Show(false);
		if(ok >	2000)
		{
			wxString msg = _("Found %d matched texts...");
			msg += wxT("								\n");
			wxProgressDialog dialog(_("Preparing Results"),
										wxString::Format(msg, (wxLongLong(ok).ToString().c_str())),
										ok,	   // range
										this,	// parent
										wxPD_CAN_ABORT |
										wxPD_AUTO_HIDE |
										wxPD_APP_MODAL);
			g_SearchProgressDialog = &dialog;

			if(!WxCheckBoxBookmarkOnly->IsChecked())
			{
				static wxString	text(_("Search Results"));
				int	pid	= g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
				g_MainFrame->m_InfoNotebook->SetPageText(pid, text);
				DisplayFindAllResult(begpos, endpos, madedit, true,	&OnSearchProgressUpdate);
			}

			dialog.Update(ok);
			g_SearchProgressDialog = NULL;
		}
		else
		{
			if(!WxCheckBoxBookmarkOnly->IsChecked())
			{
				static wxString	text(_("Search Results"));
				int	pid	= g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
				g_MainFrame->m_InfoNotebook->SetPageText(pid, text);
				DisplayFindAllResult(begpos, endpos, madedit, true);
			}
		}

		if(!ok)
		{
			g_StatusBar->SetStatusText(	_("Cannot find the matched string"), 0 );
		}
		else
		{
			wxString smsg;
			smsg.Printf(_("%s results"), (wxLongLong(ok).ToString().c_str()));
			g_StatusBar->SetStatusText(smsg, 0 );
		}
	}
	else
	{
		g_StatusBar->SetStatusText(	_("Empty expression"), 0 );
	}
}

void MadSearchReplaceDialog::PurgeRecentReplaceTexts()
{
	int	n =	(int) m_RecentReplaceText->GetCount();
	for(int	i =	n-1; i>=0; --i)
		m_RecentReplaceText->RemoveFileFromHistory((size_t)i);
}

void MadSearchReplaceDialog::ReplaceAll(MadEdit	* madedit, bool	needRec/*=true*/)
{
	wxString text;
	m_FindText->GetText(text, true);

	if(text.Len()>0)
	{
		wxString reptext;
		m_ReplaceText->GetText(reptext,	true);

		g_SearchReplaceDialog->m_RecentFindText->AddFileToHistory(text);

		if(reptext.Len()>0)
		{
			m_RecentReplaceText->AddFileToHistory(reptext);
		}

		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			rangeTo	= m_SearchTo;
			rangeFrom =	m_SearchFrom;
		}

		int	count=0;
		if(WxCheckBoxFindHex->GetValue())
		{
			count=madedit->ReplaceHexAll(text, reptext,	NULL, NULL,	rangeFrom, rangeTo);
			if(needRec)
				RecordAsMadMacro(madedit, wxString::Format(wxT("ReplaceHexAll(\"%s\", \"%s\", %s, %s)"), text.c_str(),	reptext.c_str(), (wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
		}
		else
		{
			bool bRegex = WxCheckBoxRegex->GetValue(), bWholeWord = WxCheckBoxWholeWord->GetValue(), bDotMatchNewline = WxCheckBoxDotMatchNewLine->GetValue();
			if(bRegex) bWholeWord = false;
			else bDotMatchNewline = false;
			count=madedit->ReplaceTextAll(text,	reptext,
				bRegex,
				WxCheckBoxCaseSensitive->GetValue(),
				bWholeWord,
				bDotMatchNewline,
				NULL, NULL,	rangeFrom, rangeTo);
			if(needRec)
			{
				text.Replace(wxT("\\"),	wxT("\\\\"));
				text.Replace(wxT("\""),	wxT("\\\""));
				wxString fnstr(wxString::Format(wxT("ReplaceTextAll(\"%s\", \"%s\", %s, %s, %s, %s, %s)"), text.c_str(), reptext.c_str(),
								bRegex?wxT("True"):wxT("False"),
								WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
								bWholeWord?wxT("True"):wxT("False"),
								bDotMatchNewline?wxT("True"):wxT("False"),
								(wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
				RecordAsMadMacro(madedit, fnstr);
			}
		}

		if(count>=0)
		{
			wxString msg;
			if(count==0)
				msg=wxString(_("Cannot find any matched string."));
			else
				msg=wxString::Format(_("%s string(s) were replaced."), (wxLongLong(count).ToString().c_str()));

			wxMessageDialog	dlg(this, msg, wxT("MadEdit-Mod"));
			dlg.SetYesNoLabels(wxMessageDialog::ButtonLabel(_("&Yes")),	wxMessageDialog::ButtonLabel(_("&No")));
			dlg.ShowModal();
		}

		m_FindText->SetFocus();
	}
}

/*
 * WxRadioLosingFocusClick
 */
void MadSearchReplaceDialog::WxRadioLosingFocusClick(wxCommandEvent& event)
{
	// insert your code	here
	if(m_EnableTransparency)
	{
		SetTransparent(wxIMAGE_ALPHA_OPAQUE);
	}
}

/*
 * WxRadioAlwaysClick
 */
void MadSearchReplaceDialog::WxRadioAlwaysClick(wxCommandEvent&	event)
{
	// insert your code	here
	if(m_EnableTransparency)
	{
		SetTransparent((wxByte)WxSliderTransDegree->GetValue());
	}
}

/*
 * MadSearchReplaceDialogKillFocus
 */
void MadSearchReplaceDialog::SetTransparency()
{
	// insert your code	here
	if(m_EnableTransparency	&& WxRadioLosingFocus->GetValue())
	{
		SetTransparent((wxByte)WxSliderTransDegree->GetValue());
	}
}

/*
 * WxSliderTransDegreeScroll
 */
void MadSearchReplaceDialog::WxSliderTransDegreeScroll(wxScrollEvent& event)
{
	// insert your code	here
	wxByte trans = wxIMAGE_ALPHA_OPAQUE;
	if(m_EnableTransparency	&& WxRadioAlways->GetValue())
	{
		trans =	(wxByte)WxSliderTransDegree->GetValue();
	}
	SetTransparent(trans);
}

/*
 * WxCheckBoxRegexClick
 */
void MadSearchReplaceDialog::WxCheckBoxRegexClick(wxCommandEvent& event)
{
	// insert your code here
	if(event.IsChecked())
	{
		WxCheckBoxWholeWord->Disable();
		WxCheckBoxFindHex->Disable();
		WxCheckBoxDotMatchNewLine->Enable();
	}
	else
	{
		WxCheckBoxWholeWord->Enable();
		WxCheckBoxFindHex->Enable();
		WxCheckBoxDotMatchNewLine->Disable();
	}
}
