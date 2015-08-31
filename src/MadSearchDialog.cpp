///////////////////////////////////////////////////////////////////////////////
// Name:        MadSearchDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEditFrame.h"
#include "MadSearchDialog.h"
#include "MadReplaceDialog.h"
#include "MadRecentList.h"

#include "MadEdit/MadEdit.h"
#include <wx/progdlg.h>
#include "wx/gbsizer.h"
#include <list>
#include <sstream>
//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

extern int MadMessageBox(const wxString& message,
                             const wxString& caption = wxMessageBoxCaptionStr,
                             long style = wxOK | wxCENTRE,
                             wxWindow *parent = NULL,
                             int x = wxDefaultCoord, int y = wxDefaultCoord);

#include "../images/down.xpm"

extern wxStatusBar *g_StatusBar;   // add: gogo, 19.09.2009
extern MadRecentList * g_RecentFindText;
MadSearchDialog *g_SearchDialog=NULL;
extern void RecordAsMadMacro(MadEdit *, const wxString&);
extern MadEdit *g_ActiveMadEdit;
wxProgressDialog *g_SearchProgressDialog=NULL;

bool OnSearchProgressUpdate(int value, const wxString &newmsg=wxEmptyString, bool *skip=NULL)
{
    if(g_SearchProgressDialog == NULL)
        return true;
    return g_SearchProgressDialog->Update(value, newmsg, skip);
}

//----------------------------------------------------------------------------
// MadSearchDialog
//----------------------------------------------------------------------------
     //Add Custom Events only in the appropriate Block.
    // Code added in  other places will be removed by wx-dvcpp
    ////Event Table Start
BEGIN_EVENT_TABLE(MadSearchDialog,wxDialog)
	////Manual Code Start
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT, MadSearchDialog::WxBitmapButtonRecentFindTextClick)
	EVT_MENU_RANGE(ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20, MadSearchDialog::OnRecentFindText)
	////Manual Code End
	
	EVT_CLOSE(MadSearchDialog::MadSearchDialogClose)
	EVT_KEY_DOWN(MadSearchDialog::MadSearchDialogKeyDown)
	EVT_ACTIVATE(MadSearchDialog::MadSearchDialogActivate)
	EVT_BUTTON(ID_WXBUTTONCLOSE,MadSearchDialog::WxButtonCloseClick)
	EVT_BUTTON(ID_WXBUTTONREPLACE,MadSearchDialog::WxButtonReplaceClick)
	EVT_BUTTON(ID_WXBUTTONCOUNT,MadSearchDialog::WxButtonCountClick)
	EVT_BUTTON(ID_WXBUTTONFINDALL,MadSearchDialog::WxButtonFindAllClick)
	EVT_BUTTON(ID_WXBUTTONFINDPREV,MadSearchDialog::WxButtonFindPrevClick)
	EVT_BUTTON(ID_WXBUTTONFINDNEXT,MadSearchDialog::WxButtonFindNextClick)
	EVT_CHECKBOX(ID_WXCHECKBOXSEARCHINSELECTION,MadSearchDialog::WxCheckBoxSearchInSelectionClick)
	EVT_CHECKBOX(ID_WXCHECKBOXFINDHEX,MadSearchDialog::WxCheckBoxFindHexClick)
END_EVENT_TABLE()
    ////Event Table End



MadSearchDialog::MadSearchDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style), m_SearchFrom(0), m_SearchTo(0)
{
    CreateGUIControls();
}

MadSearchDialog::~MadSearchDialog()
{
}

//static int gs_MinX=0;

static void ResizeItem(wxBoxSizer* sizer, wxWindow *item, int ax, int ay)
{
    int x, y;
    wxString str=item->GetLabel();
    item->GetTextExtent(str, &x, &y);
    item->SetSize(x+=ax, y+=ay);
    sizer->SetItemMinSize(item, x, y);

    //wxPoint pos=item->GetPosition();
    //if(pos.x + x > gs_MinX) gs_MinX = pos.x + x;
}

void MadSearchDialog::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	WxBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 0);

	WxBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer2->Add(WxBoxSizer4, 0, wxALIGN_CENTER | wxALL, 0);

	WxBoxSizer5 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer5, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 0);

	WxCheckBoxMoveFocus = new wxCheckBox(this, ID_WXCHECKBOXMOVEFOCUS, _("&Move Focus to Editor Window"), wxPoint(164, 2), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxMoveFocus"));
	WxBoxSizer5->Add(WxCheckBoxMoveFocus, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxPoint(164, 28), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxCaseSensitive"));
	WxBoxSizer5->Add(WxCheckBoxCaseSensitive, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxPoint(164, 54), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxWholeWord"));
	WxBoxSizer5->Add(WxCheckBoxWholeWord, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxPoint(164, 80), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxRegex"));
	WxBoxSizer5->Add(WxCheckBoxRegex, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxPoint(164, 106), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxFindHex"));
	WxBoxSizer5->Add(WxCheckBoxFindHex, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxSearchThrEndOfFile = new wxCheckBox(this, ID_WXCHECKBOXSEARCHTHRENDOFFILE, _("Search Through &End of File"), wxPoint(164, 132), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxSearchThrEndOfFile"));
	WxBoxSizer5->Add(WxCheckBoxSearchThrEndOfFile, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxSearchInSelection = new wxCheckBox(this, ID_WXCHECKBOXSEARCHINSELECTION, _("Search In &Selection"), wxPoint(12, 158), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxSearchInSelection"));
	WxBoxSizer5->Add(WxCheckBoxSearchInSelection, 0, wxALIGN_LEFT | wxALL, 2);

	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox(this, wxID_ANY, _("Bookmark"));
	WxStaticBoxSizer1 = new wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL);
	WxBoxSizer5->Add(WxStaticBoxSizer1, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALIGN_CENTER | wxALL, 5);

	WxCheckBoxBookmarkLine = new wxCheckBox(this, ID_WXCHECKBOXBOOKMARKLINE, _("Bookmark line"), wxPoint(7, 17), wxSize(300, 22), 0, wxDefaultValidator, wxT("WxCheckBoxBookmarkLine"));
	WxStaticBoxSizer1->Add(WxCheckBoxBookmarkLine, 0, wxALIGN_LEFT | wxALL, 2);

	WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_TOP | wxALL, 0);

	WxButtonFindNext = new wxButton(this, ID_WXBUTTONFINDNEXT, _("Find &Next"), wxPoint(2, 2), wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonFindNext"));
	WxBoxSizer3->Add(WxButtonFindNext, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonFindPrev = new wxButton(this, ID_WXBUTTONFINDPREV, _("Find &Previous"), wxPoint(2, 34), wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonFindPrev"));
	WxBoxSizer3->Add(WxButtonFindPrev, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonFindAll = new wxButton(this, ID_WXBUTTONFINDALL, _("Find &All"), wxPoint(2, 66), wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonFindAll"));
	WxBoxSizer3->Add(WxButtonFindAll, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonCount = new wxButton(this, ID_WXBUTTONCOUNT, _("C&ount"), wxPoint(2, 98), wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonCount"));
	WxBoxSizer3->Add(WxButtonCount, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace >>"), wxPoint(2, 130), wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonReplace"));
	WxBoxSizer3->Add(WxButtonReplace, 0, wxALIGN_CENTER | wxALL, 2);

	WxButtonClose = new wxButton(this, ID_WXBUTTONCLOSE, _("Close"), wxPoint(2, 162), wxSize(100, 28), 0, wxDefaultValidator, wxT("WxButtonClose"));
	WxBoxSizer3->Add(WxButtonClose, 0, wxALIGN_CENTER | wxALL, 2);

	WxPopupMenuRecentFindText = new wxMenu(wxT(""));
	

	WxCheckBoxBookmarkOnly = new wxCheckBox(this, ID_WXCHECKBOXBOOKMARKONLY, _("Bookmark only(Find All)"), wxPoint(7, 43), wxSize(300, 17), 0, wxDefaultValidator, wxT("WxCheckBoxBookmarkOnly"));
	WxStaticBoxSizer1->Add(WxCheckBoxBookmarkOnly, 0, wxALIGN_CENTER | wxALL, 2);

	WxCheckBoxPurgeBookmark = new wxCheckBox(this, ID_WXCHECKBOXPURGEBOOKMARK, _("Purge mark for each search session"), wxPoint(311, 19), wxSize(300, 17), 0, wxDefaultValidator, wxT("WxCheckBoxPurgeBookmark"));
	WxStaticBoxSizer1->Add(WxCheckBoxPurgeBookmark, 0, wxALIGN_CENTER | wxALL, 2);

	SetTitle(_("Search"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	
    ////GUI Items Creation End

    // removed: gogo, 19.09.2009
    //this->SetPosition(wxPoint(300,100));

    int bw, bh;
    WxButtonFindNext->GetSize(&bw, &bh);

    m_FindText=new MadEdit(this, ID_MADEDIT, wxPoint(0, 0), wxSize(400, bh));
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

    WxBoxSizer4->Add(m_FindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);
    WxBoxSizer4->SetItemMinSize(m_FindText, 400, bh);

    wxBitmap WxBitmapButtonRecentFindText_BITMAP (down_xpm);
    WxBitmapButtonRecentFindText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTFINDTEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, wxT("WxBitmapButtonRecentFindText"));
    WxBoxSizer4->Add(WxBitmapButtonRecentFindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

    // resize checkbox
    ResizeItem(WxBoxSizer5, WxCheckBoxMoveFocus, 25, 4);
    ResizeItem(WxBoxSizer5, WxCheckBoxCaseSensitive, 25, 4);
    ResizeItem(WxBoxSizer5, WxCheckBoxWholeWord, 25, 4);
    ResizeItem(WxBoxSizer5, WxCheckBoxRegex, 25, 4);
    ResizeItem(WxBoxSizer5, WxCheckBoxFindHex, 25, 4);
    ResizeItem(WxBoxSizer5, WxCheckBoxSearchThrEndOfFile, 25, 4);
    ResizeItem(WxBoxSizer5, WxCheckBoxSearchInSelection, 25, 4);

    GetSizer()->Fit(this);

    // connect to KeyDown event handler
    m_FindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxBitmapButtonRecentFindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxMoveFocus->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxCaseSensitive->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxWholeWord->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxRegex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxFindHex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxSearchThrEndOfFile->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxSearchInSelection->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxBookmarkLine->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxCheckBoxBookmarkOnly->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
	WxCheckBoxPurgeBookmark->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
	//WxEditFrom->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    //WxEditTo->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxButtonFindNext->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxButtonFindPrev->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxButtonFindAll->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxButtonCount->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));
    WxButtonClose->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadSearchDialog::MadSearchDialogKeyDown));


    m_RecentFindText=g_RecentFindText;//new MadRecentList(20, ID_RECENTFINDTEXT1, true);
    m_RecentFindText->UseMenu(WxPopupMenuRecentFindText);
    m_RecentFindText->AddFilesToMenu();

    wxConfigBase *m_Config=wxConfigBase::Get(false);
    wxString oldpath=m_Config->GetPath();

    // add: gogo, 19.09.2009
    long x = 480, y = 100;
    m_Config->Read( wxT("/MadEdit/SearchWinLeft"), &x );
    m_Config->Read( wxT("/MadEdit/SearchWinTop"), &y );
    SetPosition( wxPoint(x,y) );
    bool bb;
    // I prefer to read it here instead in ReadWriteSettings(), because here
    // it will be read once per session instead of every 'Ctrl + F'
    m_Config->Read(wxT("/MadEdit/SearchThrEndOfFile"), &bb, false);
    WxCheckBoxSearchThrEndOfFile->SetValue( bb );
    //--------

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
    SetDefaultItem(WxButtonFindNext);
}

void MadSearchDialog::MadSearchDialogClose(wxCloseEvent& event)
{
    // --> Don't use Close with a Dialog,
    // use Destroy instead.

    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }

    g_SearchDialog=NULL;
    Destroy();
}


/*
 * WxButtonCloseClick
 */
void MadSearchDialog::WxButtonCloseClick(wxCommandEvent& event)
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
void MadSearchDialog::WxButtonFindNextClick(wxCommandEvent& event)
{
    // add: gogo, 19.09.2009
    g_StatusBar->SetStatusText( wxEmptyString, 0 );
    bool ThrEndOfFile = false;

    wxString text,expr;
    m_FindText->GetText(text, true);

    if(text.Len()>0)
    {
        m_RecentFindText->AddFileToHistory(text);

        MadSearchResult sr;
        wxFileOffset selend = g_ActiveMadEdit->GetSelectionEndPos();

        // moved here: gogo, 19.09.2009
        wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();

        //wxInt64 from = 0, to = 0;
        wxFileOffset rangeFrom = -1, rangeTo = -1;
        if(WxCheckBoxSearchInSelection->IsChecked())
        {
            rangeTo = m_SearchTo;
            // removed: gogo, 19.09.2009
            //wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
            if(caretpos <= m_SearchFrom || caretpos >= m_SearchTo)
                rangeFrom = m_SearchFrom;
        }

        g_ActiveMadEdit->SetBookmarkInSearch(WxCheckBoxBookmarkLine->IsChecked());

        expr = text;
        expr.Replace(wxT("\\"), wxT("\\\\"));
        expr.Replace(wxT("\""), wxT("\\\""));
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
                sr=g_ActiveMadEdit->FindTextNext(text,
                    WxCheckBoxRegex->GetValue(),
                    WxCheckBoxCaseSensitive->GetValue(),
                    WxCheckBoxWholeWord->GetValue(),
                    rangeFrom, rangeTo);
                if(sr != SR_EXPR_ERROR)
                {
					wxString fnstr(wxString::Format(wxT("FindTextNext(\"%s\", %s, %s, %s, %s, %s)"), expr.c_str(),
									WxCheckBoxRegex->GetValue()?wxT("True"):wxT("False"),
									WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
									WxCheckBoxWholeWord->GetValue()?wxT("True"):wxT("False"), (wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
					RecordAsMadMacro(g_ActiveMadEdit, fnstr);
                }
            }

            if(sr != SR_NO)
            {
                if(sr == SR_YES && g_ActiveMadEdit->GetCaretPosition() == selend)
                {
                    selend = -1;
                    continue;
                }

                // add: gogo, 19.09.2009
                if ( ThrEndOfFile )
                    g_StatusBar->SetStatusText( _("Passed the end of the file"), 0 );

                break;
            }

            // add: gogo, 19.09.2009
            if ( WxCheckBoxSearchThrEndOfFile->IsChecked() && ! WxCheckBoxSearchInSelection->IsChecked() )
            {
                if ( rangeTo == caretpos )
                {
                    g_StatusBar->SetStatusText( _("Cannot find the matched string"), 0 );
                    break;
                }
                rangeTo = caretpos;
                rangeFrom = 0;
                ThrEndOfFile = true;
                continue;
            }
            //--------------

            wxString msg(_("Cannot find the matched string."));
            msg += wxT("\n\n");
            msg += WxCheckBoxSearchInSelection->IsChecked()?
                _("Do you want to find from begin of selection?"):
                _("Do you want to find from begin of file?");

            if(wxCANCEL == MadMessageBox(msg, _("Find Next"), wxOK|wxCANCEL
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION > 9)
                |wxCANCEL_DEFAULT
#endif
                |wxICON_QUESTION ))
            {
                break;
            }
            rangeFrom = WxCheckBoxSearchInSelection->IsChecked()? m_SearchFrom : 0;
            rangeTo = WxCheckBoxSearchInSelection->IsChecked()? m_SearchTo: g_ActiveMadEdit->GetFileSize();
            if(WxCheckBoxSearchInSelection->IsChecked())
            {
                g_ActiveMadEdit->SetSelection(m_SearchFrom, m_SearchTo);
            }
            
            if(WxCheckBoxPurgeBookmark->IsChecked())
                g_ActiveMadEdit->ClearAllBookmarks();
        }
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
void MadSearchDialog::WxButtonFindPrevClick(wxCommandEvent& event)
{
    extern MadEdit *g_ActiveMadEdit;

    if(g_ActiveMadEdit==NULL)
        return;

    // add: gogo, 19.09.2009
    g_StatusBar->SetStatusText( wxEmptyString, 0 );
    bool ThrEndOfFile = false;

    wxString text, expr;
    m_FindText->GetText(text, true);

    if(text.Len()>0)
    {
        m_RecentFindText->AddFileToHistory(text);

        MadSearchResult sr;
        wxFileOffset selbeg = g_ActiveMadEdit->GetSelectionBeginPos();

        // moved here: gogo, 19.09.2009
        wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();

        wxFileOffset rangeFrom = -1, rangeTo = -1;
        if(WxCheckBoxSearchInSelection->IsChecked())
        {

            rangeFrom = m_SearchFrom;
            // removed: gogo, 19.09.2009
            //wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
            if(caretpos <= m_SearchFrom || caretpos >= m_SearchTo)
                rangeFrom = m_SearchFrom;
        }

        expr = text;
        expr.Replace(wxT("\\"), wxT("\\\\"));
        expr.Replace(wxT("\""), wxT("\\\""));

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
                sr=g_ActiveMadEdit->FindTextPrevious(text,
                    WxCheckBoxRegex->GetValue(),
                    WxCheckBoxCaseSensitive->GetValue(),
                    WxCheckBoxWholeWord->GetValue(),
                    rangeTo, rangeFrom);
                
					wxString fnstr(wxString::Format(wxT("FindTextPrevious(\"%s\", %s, %s, %s, %s, %s)"), expr.c_str(),
                            WxCheckBoxRegex->GetValue()?wxT("True"):wxT("False"),
                            WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
                            WxCheckBoxWholeWord->GetValue()?wxT("True"):wxT("False"), (wxLongLong(rangeFrom).ToString()).c_str(), (wxLongLong(rangeTo).ToString()).c_str()));
					RecordAsMadMacro(g_ActiveMadEdit, fnstr);
            }

            if(sr!=SR_NO)
            {
                if(sr == SR_YES && g_ActiveMadEdit->GetCaretPosition() == selbeg)
                {
                    selbeg = -1;
                    continue;
                }

                // add: gogo, 19.09.2009
                if ( ThrEndOfFile )
                    g_StatusBar->SetStatusText( _("Passed the end of the file"), 0 );

                break;
            }

            // add: gogo, 19.09.2009
            if ( WxCheckBoxSearchThrEndOfFile->IsChecked() && ! WxCheckBoxSearchInSelection->IsChecked() )
            {
                if ( rangeFrom == caretpos )
                {
                    g_StatusBar->SetStatusText( _("Cannot find the matched string"), 0 );
                    break;
                }
                rangeTo = g_ActiveMadEdit->GetFileSize();
                rangeFrom = caretpos;
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
            rangeTo = WxCheckBoxSearchInSelection->IsChecked()? m_SearchTo: g_ActiveMadEdit->GetFileSize();
            rangeFrom = WxCheckBoxSearchInSelection->IsChecked()? m_SearchFrom: 0;

            if(WxCheckBoxPurgeBookmark->IsChecked())
                g_ActiveMadEdit->ClearAllBookmarks();
        }
    }

    if(WxCheckBoxMoveFocus->GetValue())
    {
        ((wxFrame*)wxTheApp->GetTopWindow())->Raise();
        g_ActiveMadEdit->SetFocus();
    }
}


/*
 * MadSearchDialogKeyDown
 */
void MadSearchDialog::MadSearchDialogKeyDown(wxKeyEvent& event)
{
    int key=event.GetKeyCode();

    //g_SearchDialog->SetTitle(wxString()<<key);

    switch(key)
    {
    case WXK_ESCAPE:
        g_SearchDialog->Show(false);
        return;
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        if((wxButton*)this!=g_SearchDialog->WxButtonFindNext &&
           (wxButton*)this!=g_SearchDialog->WxButtonFindPrev &&
           (wxButton*)this!=g_SearchDialog->WxButtonFindAll &&
           (wxButton*)this!=g_SearchDialog->WxButtonClose)
        {
            wxCommandEvent e;
            g_SearchDialog->WxButtonFindNextClick(e);
            return; // no skip
        }
        break;
    case WXK_DOWN:
        if((MadEdit*)this==g_SearchDialog->m_FindText)
        {
            int x,y,w,h;
            g_SearchDialog->m_FindText->GetPosition(&x, &y);
            g_SearchDialog->m_FindText->GetSize(&w, &h);
            g_SearchDialog->PopupMenu(g_SearchDialog->WxPopupMenuRecentFindText, x, y+h);
            return;
        }
        break;
    }

    extern wxAcceleratorEntry g_AccelFindNext, g_AccelFindPrev;
    int flags=wxACCEL_NORMAL;
    if(event.m_altDown) flags|=wxACCEL_ALT;
    if(event.m_shiftDown) flags|=wxACCEL_SHIFT;
    if(event.m_controlDown) flags|=wxACCEL_CTRL;

    if(g_AccelFindNext.GetKeyCode()==key && g_AccelFindNext.GetFlags()==flags)
    {
        wxCommandEvent e;
        g_SearchDialog->WxButtonFindNextClick(e);
        return; // no skip
    }

    if(g_AccelFindPrev.GetKeyCode()==key && g_AccelFindPrev.GetFlags()==flags)
    {
        wxCommandEvent e;
        g_SearchDialog->WxButtonFindPrevClick(e);
        return; // no skip
    }

    event.Skip();
}

/*
 * WxBitmapButtonRecentFindTextClick
 */
void MadSearchDialog::WxBitmapButtonRecentFindTextClick(wxCommandEvent& event)
{
    PopupMenu(WxPopupMenuRecentFindText);
}

void MadSearchDialog::OnRecentFindText(wxCommandEvent& event)
{
    int idx=event.GetId()-ID_RECENTFINDTEXT1;
    wxString text=m_RecentFindText->GetHistoryFile(idx);
    if(!text.IsEmpty())
    {
        m_FindText->SetText(text);
        m_FindText->SetFocus();
    }
}

void MadSearchDialog::ReadWriteSettings(bool bRead)
{
    extern MadEdit *g_ActiveMadEdit;
    wxConfigBase *m_Config=wxConfigBase::Get(false);
    wxString oldpath=m_Config->GetPath();

    if(bRead)
    {
        bool bb;
        m_Config->Read(wxT("/MadEdit/SearchMoveFocus"), &bb, false);
        WxCheckBoxMoveFocus->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchCaseSensitive"), &bb, false);
        WxCheckBoxCaseSensitive->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchWholeWord"), &bb, false);
        WxCheckBoxWholeWord->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchRegex"), &bb, false);
        WxCheckBoxRegex->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchHex"), &bb, false);
        WxCheckBoxFindHex->SetValue(bb);
        UpdateCheckBoxByCBHex(bb);

        m_Config->Read(wxT("/MadEdit/SearchInSelection"), &bb, false);
        WxCheckBoxSearchInSelection->SetValue(bb);
        UpdateSearchInSelection(bb);
        
        m_Config->Read(wxT("/MadEdit/SearchBookmarkLines"), &bb, false);
        WxCheckBoxBookmarkLine->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchBookmarkOnly"), &bb, false);
        WxCheckBoxBookmarkOnly->SetValue(bb);
        
        m_Config->Read(wxT("/MadEdit/SearchPurgeBookmark"), &bb, false);
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
        //m_Config->Write(wxT("/MadEdit/SearchTo"), (wxLongLong(m_SearchTo)).ToString());
        m_Config->Write(wxT("/MadEdit/SearchBookmarkLines"), WxCheckBoxBookmarkLine->GetValue());
        m_Config->Write(wxT("/MadEdit/SearchBookmarkOnly"), WxCheckBoxBookmarkOnly->GetValue());
        m_Config->Write(wxT("/MadEdit/SearchPurgeBookmark"), WxCheckBoxPurgeBookmark->GetValue());
    }

    m_Config->SetPath(oldpath);
}

static inline ucs4_t ToHex(int d)// 0 <= d <= 15
{
    if(d < 10)
        return '0' + d;
    return 'A' + d - 10;
}

void MadSearchDialog::UpdateCheckBoxByCBHex(bool check)
{
    if(check)
    {
        WxCheckBoxCaseSensitive->Disable();
        WxCheckBoxWholeWord->Disable();
        WxCheckBoxRegex->Disable();
    }
    else
    {
        WxCheckBoxCaseSensitive->Enable();
        WxCheckBoxWholeWord->Enable();
        WxCheckBoxRegex->Enable();
    }
}

/*
 * WxCheckBoxFindHexClick
 */
void MadSearchDialog::WxCheckBoxFindHexClick(wxCommandEvent& event)
{
    extern MadEdit *g_ActiveMadEdit;
    bool checked = event.IsChecked();
    if(checked)
    {
        wxString text;
        m_FindText->GetText(text, true);
        if(text.IsEmpty())
        {
            if(g_ActiveMadEdit!=NULL)
            {
                wxString ws;
                g_ActiveMadEdit->GetSelHexString(ws, true);
                m_FindText->SetText(ws);
            }
        }
        else
        {
            wxString ws;
            m_FindText->SelectAll();
            m_FindText->GetSelHexString(ws, true);
            m_FindText->SetText(ws);
        }
    }
    else
    {
        wxString text;
        m_FindText->GetText(text, true);
        if(text.IsEmpty())
        {
            if(g_ActiveMadEdit!=NULL)
            {
                wxString ws;
                g_ActiveMadEdit->GetSelText(ws);
                m_FindText->SetText(ws);
            }
        }
    }
    UpdateCheckBoxByCBHex(event.IsChecked());
}

/*
 * MadSearchDialogActivate
 */
void MadSearchDialog::MadSearchDialogActivate(wxActivateEvent& event)
{
    ReadWriteSettings(event.GetActive());
}


/*
 * WxButtonReplaceClick
 */
void MadSearchDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
    wxString fname;
    int fsize;
    m_FindText->GetFont(fname, fsize);
    g_ReplaceDialog->m_FindText->SetFont(fname, 14);
    g_ReplaceDialog->m_ReplaceText->SetFont(fname, 14);

    wxString text;
    m_FindText->GetText(text, true);
    g_ReplaceDialog->m_FindText->SetText(text);

    this->Show(false);
    g_ReplaceDialog->Show();
    g_ReplaceDialog->SetFocus();
    g_ReplaceDialog->Raise();

    g_ReplaceDialog->UpdateCheckBoxByCBHex(g_ReplaceDialog->WxCheckBoxFindHex->GetValue());

    g_ReplaceDialog->m_FindText->SelectAll();
    g_ReplaceDialog->m_FindText->SetFocus();

}

void MadSearchDialog::UpdateSearchInSelection(bool check)
{
    //WxEditFrom->Enable(check);
    //WxEditTo->Enable(check);

    extern MadEdit *g_ActiveMadEdit;
    if(check && g_ActiveMadEdit!=NULL)
    {
        m_SearchFrom = wxLongLong(g_ActiveMadEdit->GetSelectionBeginPos()).GetValue();
        m_SearchTo = wxLongLong(g_ActiveMadEdit->GetSelectionEndPos()).GetValue();
    }
    else
    {
        m_SearchFrom = -1;
        m_SearchTo = -1;
    }

    // add: gogo, 19.09.2009
    if(check)
        WxCheckBoxSearchThrEndOfFile->Disable();
    else
        WxCheckBoxSearchThrEndOfFile->Enable();
}


void MadSearchDialog::WxCheckBoxSearchInSelectionClick(wxCommandEvent& event)
{
    UpdateSearchInSelection(event.IsChecked());
}

void MadSearchDialog::WxButtonCountClick(wxCommandEvent& event)
{
    extern MadEdit *g_ActiveMadEdit;

    if(g_ActiveMadEdit==NULL)
        return;

    int count = 0;
    wxString text;
    m_FindText->GetText(text, true);

    if(text.Len()>0)
    {
        m_RecentFindText->AddFileToHistory(text);

        wxInt64 from = 0, to = 0;
        wxFileOffset rangeFrom = -1, rangeTo = -1;
        if(WxCheckBoxSearchInSelection->IsChecked())
        {
            rangeTo = m_SearchTo;
            rangeFrom = m_SearchFrom;
        }

        if(WxCheckBoxFindHex->GetValue())
        {
            count=g_ActiveMadEdit->FindHexAll(text, false, NULL, NULL, rangeFrom, rangeTo);
        }
        else
        {
            count=g_ActiveMadEdit->FindTextAll(text,
                WxCheckBoxRegex->GetValue(),
                WxCheckBoxCaseSensitive->GetValue(),
                WxCheckBoxWholeWord->GetValue(),
                false,
                NULL, NULL,
                rangeFrom, rangeTo);
        }
    }

    if(count >= 0)
    {
        wxString msg;
        msg.Printf(_("'%s' was found %d times."), text.c_str(), count);
        MadMessageBox(msg, wxT("MadEdit-Mod"), wxOK);
    }
}

void DisplayFindAllResult(vector<wxFileOffset> &begpos, vector<wxFileOffset> &endpos, MadEdit *madedit, bool expandresults = true, OnProgressUpdatePtr updater = NULL)
{
    int ResultCount=0;
    wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;

    if(!begpos.empty()) // found data
    {
        int pid = ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageIndex(madedit);
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
            size_t count=begpos.size(), idx=0;
            int line=-1, oldline;
            wxString linetext, loc;
            results->Freeze();
            
            wxString status = _("Preparing %d of %d results...");
            status += wxT("                                \n");
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

                fmt = loc +linetext;
                g_MainFrame->AddItemToFindInFilesResults(fmt, idx, filename, pid, begpos[idx], endpos[idx]);
                ++ResultCount;
                if(updater != NULL && (count >= 2000))
                {
                    if(updater(idx, wxString::Format(status, idx, count), NULL)== false) break;
                }
            }
            while(++idx < count);
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
        g_StatusBar->SetStatusText( _("Cannot find the matched string"), 0 );
    }
    else
    {
		wxString smsg(wxLongLong(begpos.size()).ToString() + _(" results"));
        g_StatusBar->SetStatusText(smsg, 0 );
    }
}

void MadSearchDialog::WxButtonFindAllClick(wxCommandEvent& event)
{
    extern MadEdit *g_ActiveMadEdit;
    //wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;

    if(g_ActiveMadEdit==NULL)
        return;

    //g_MainFrame->ResetFindInFilesResults();
    MadEdit *madedit=g_ActiveMadEdit;

    vector<wxFileOffset> begpos, endpos;

    // get all matched data in madedit
    wxString expr, fmt;
    int ok;
    m_FindText->GetText(expr);
    if(expr.Len()>0)
    {
        begpos.reserve(128*1024);
        endpos.reserve(128*1024);
        m_RecentFindText->AddFileToHistory(expr);
        g_ActiveMadEdit->SetBookmarkInSearch(WxCheckBoxBookmarkLine->IsChecked());

        if(WxCheckBoxPurgeBookmark->IsChecked())
            g_ActiveMadEdit->ClearAllBookmarks();
        wxFileOffset selend = g_ActiveMadEdit->GetSelectionEndPos();

        // moved here: gogo, 19.09.2009
        wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
        //wxInt64 from = 0, to = 0;
        wxFileOffset rangeFrom = -1, rangeTo = -1;
        if(WxCheckBoxSearchInSelection->IsChecked())
        {
            rangeTo = m_SearchTo;
            // removed: gogo, 19.09.2009
            //wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
            if(caretpos <= m_SearchFrom || caretpos >= m_SearchTo)
                rangeFrom = m_SearchFrom;
        }

        if(WxCheckBoxFindHex->GetValue())
        {
            ok = madedit->FindHexAll(expr, false, &begpos, &endpos, rangeFrom, rangeTo);
            RecordAsMadMacro(madedit, wxString::Format(wxT("FindHexAll(\"%s\")"), expr.c_str()));
        }
        else
        {
            ok = madedit->FindTextAll(expr,
                WxCheckBoxRegex->GetValue(),
                WxCheckBoxCaseSensitive->GetValue(),
                WxCheckBoxWholeWord->GetValue(),
                false,
                &begpos, &endpos, rangeFrom, rangeTo);

            expr.Replace(wxT("\\"), wxT("\\\\"));
            expr.Replace(wxT("\""), wxT("\\\""));
			wxString fnstr(wxString::Format(wxT("FindTextAll(\"%s\", %s, %s, %s)"), expr.c_str(),
                            WxCheckBoxRegex->GetValue()?wxT("True"):wxT("False"),
                            WxCheckBoxCaseSensitive->GetValue()?wxT("True"):wxT("False"),
                            WxCheckBoxWholeWord->GetValue()?wxT("True"):wxT("False")));
			RecordAsMadMacro(g_ActiveMadEdit, fnstr);
        }

        if(ok<0) return;

        Show(false);
        if(ok > 2000)
        {
            wxString msg = _("Found %d matched texts...");
            msg += wxT("                                \n");
            wxProgressDialog dialog(_("Preparing Results"),
                                        wxString::Format(msg, 0),
                                        ok,    // range
                                        this,   // parent
                                        wxPD_CAN_ABORT |
                                        wxPD_AUTO_HIDE |
                                        wxPD_APP_MODAL);
            g_SearchProgressDialog = &dialog;

            if(!WxCheckBoxBookmarkOnly->IsChecked())
            {
                static wxString text(_("Search Results"));
                int pid = g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
                g_MainFrame->m_InfoNotebook->SetPageText(pid, text);
                DisplayFindAllResult(begpos, endpos, madedit, true, &OnSearchProgressUpdate);
            }

            dialog.Update(ok);
            g_SearchProgressDialog = NULL;
        }
        else
        {
            if(!WxCheckBoxBookmarkOnly->IsChecked())
            {
                static wxString text(_("Search Results"));
                int pid = g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
                g_MainFrame->m_InfoNotebook->SetPageText(pid, text);
                DisplayFindAllResult(begpos, endpos, madedit, true);
            }
        }

        if(WxCheckBoxBookmarkOnly->IsChecked())
        {
            if(!ok)
            {
                g_StatusBar->SetStatusText( _("Cannot find the matched string"), 0 );
            }
            else
            {
                wxString smsg;
                smsg.Printf(_("%d results"), ok);
                g_StatusBar->SetStatusText(smsg, 0 );
            }
        }
        //Show(true);
    }
}

void MadSearchDialog::PurgeRecentFindTexts()
{
    int n = (int) m_RecentFindText->GetCount();
    for(int i=n-1; i>=0; --i)
        m_RecentFindText->RemoveFileFromHistory((size_t)i);
}

