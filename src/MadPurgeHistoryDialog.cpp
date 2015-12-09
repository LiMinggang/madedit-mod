///////////////////////////////////////////////////////////////////////////////
// Name:		MadPurgeHistoryDialog.cpp
// Description:	MadPurgeHistoryDialog class implementation
// Author:		minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadPurgeHistoryDialog.h"
#include "MadEditFrame.h"
#include "MadSearchReplaceDialog.h"
#include "MadFindInFilesDialog.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(MadPurgeHistoriesDialog)
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

//(*IdInit(MadPurgeHistoryDialog)
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTFILES = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTFONTS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTENCODINGS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTSEARCHEDTEXTS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTREPLACEDTEXTS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTSEARCHEDDIRECTORIES = wxNewId();
const long MadPurgeHistoryDialog::ID_WXRECENTSEARCHEDFILEFILTERS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXRECENTSEARCHEDEXCLUDEFILTERS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXCARETPOS = wxNewId();
const long MadPurgeHistoryDialog::ID_WXCHECKBOXALLABOVE = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadPurgeHistoryDialog,wxDialog)
	//(*EventTable(MadPurgeHistoryDialog)
	//*)
END_EVENT_TABLE()

MadPurgeHistoryDialog::MadPurgeHistoryDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MadPurgeHistoryDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Purge Histories"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, wxEmptyString);
	wxCheckBoxRecentFiles = new wxCheckBox(this, ID_WXCHECKBOXRECENTFILES, _("Recent &Files"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTFILES"));
	wxCheckBoxRecentFiles->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentFiles, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxRecentFonts = new wxCheckBox(this, ID_WXCHECKBOXRECENTFONTS, _("Recent Fon&ts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTFONTS"));
	wxCheckBoxRecentFonts->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentFonts, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxRecentEncodings = new wxCheckBox(this, ID_WXCHECKBOXRECENTENCODINGS, _("Recent &Encodings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTENCODINGS"));
	wxCheckBoxRecentEncodings->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentEncodings, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxRecentSearchedTexts = new wxCheckBox(this, ID_WXCHECKBOXRECENTSEARCHEDTEXTS, _("Recent &Searched Texts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTSEARCHEDTEXTS"));
	wxCheckBoxRecentSearchedTexts->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentSearchedTexts, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxRecentReplacedTexts = new wxCheckBox(this, ID_WXCHECKBOXRECENTREPLACEDTEXTS, _("Recent &Replaced Texts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTREPLACEDTEXTS"));
	wxCheckBoxRecentReplacedTexts->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentReplacedTexts, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxRecentSearchedDirectories = new wxCheckBox(this, ID_WXCHECKBOXRECENTSEARCHEDDIRECTORIES, _("Recent Searched &Directories"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTSEARCHEDDIRECTORIES"));
	wxCheckBoxRecentSearchedDirectories->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentSearchedDirectories, 0, wxALL|wxALIGN_LEFT, 5);
	wxRecentSearchedFileFilters = new wxCheckBox(this, ID_WXRECENTSEARCHEDFILEFILTERS, _("Recent Searched File Fi&lters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXRECENTSEARCHEDFILEFILTERS"));
	wxRecentSearchedFileFilters->SetValue(false);
	StaticBoxSizer1->Add(wxRecentSearchedFileFilters, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxRecentSearchedExcludeFilters = new wxCheckBox(this, ID_WXCHECKBOXRECENTSEARCHEDEXCLUDEFILTERS, _("Recent Searched E&xclude Filters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTSEARCHEDEXCLUDEFILTERS"));
	wxCheckBoxRecentSearchedExcludeFilters->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentSearchedExcludeFilters, 0, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxCaretPos = new wxCheckBox(this, ID_WXCHECKBOXCARETPOS, _("Recent Caret Position"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCARETPOS"));
	wxCheckBoxCaretPos->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxCaretPos, 1, wxALL|wxALIGN_LEFT, 5);
	wxCheckBoxAllAbove = new wxCheckBox(this, ID_WXCHECKBOXALLABOVE, _("All Above"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXALLABOVE"));
	wxCheckBoxAllAbove->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxAllAbove, 1, wxALL|wxALIGN_LEFT, 5);
	BoxSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer2->Add(WxButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	WxButtonCancel->SetDefault();
	BoxSizer2->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_WXCHECKBOXALLABOVE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadPurgeHistoryDialog::wxCheckBoxAllAboveClick);
	Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadPurgeHistoryDialog::WxButtonOKClick);
	Connect(wxID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadPurgeHistoryDialog::WxButtonCancelClick);
	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown);
	//*)
}

MadPurgeHistoryDialog::~MadPurgeHistoryDialog()
{
	//(*Destroy(MadPurgeHistoryDialog)
	//*)
}


void MadPurgeHistoryDialog::MadPurgeHistoryDialogClose(wxCloseEvent& event)
{
	Destroy();
}

void MadPurgeHistoryDialog::WxButtonOKClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

/*
 * wxButtonCancelClick
 */
void MadPurgeHistoryDialog::WxButtonCancelClick(wxCommandEvent& event)
{
    // insert your code here
    EndModal(wxID_CANCEL);
}

/*
 * MadPurgeHistoryDialogKeyDown
 */
void MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown(wxKeyEvent& event)
{
    // insert your code here
    int key=event.GetKeyCode();

    switch(key)
    {
    case WXK_ESCAPE:
        {
            wxCommandEvent e;
            this->WxButtonCancelClick(e);
            return;
        }
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        if ((wxButton*)this != this->WxButtonCancel)
        {
            wxCommandEvent e;
            this->WxButtonOKClick(e);
            return; // no skip
        }
        else
        {
            wxCommandEvent e;
            this->WxButtonCancelClick(e);
            return; // no skip
        }
        break;
    default:
        break;
    }

    int flags=wxACCEL_NORMAL;
    if(event.m_altDown) flags|=wxACCEL_ALT;
    if(event.m_shiftDown) flags|=wxACCEL_SHIFT;
    if(event.m_controlDown) flags|=wxACCEL_CTRL;

    if('o' == key && wxACCEL_ALT == flags)
    {
        wxCommandEvent e;
        this->WxButtonOKClick(e);
        return; // no skip
    }

    if('c' == key && wxACCEL_ALT == flags)
    {
        wxCommandEvent e;
        this->WxButtonCancelClick(e);
        return; // no skip
    }

    event.Skip();
}

void MadPurgeHistoryDialog::wxCheckBoxAllAboveClick(wxCommandEvent& event)
{
    if(event.IsChecked()) 
    {
        wxCheckBoxRecentSearchedExcludeFilters->SetValue(true);
        wxRecentSearchedFileFilters->SetValue(true);
        wxCheckBoxRecentSearchedDirectories->SetValue(true);
        wxCheckBoxRecentReplacedTexts->SetValue(true);
        wxCheckBoxRecentSearchedTexts->SetValue(true);
        wxCheckBoxRecentEncodings->SetValue(true);
        wxCheckBoxRecentFonts->SetValue(true);
        wxCheckBoxRecentFiles->SetValue(true); 
		wxCheckBoxCaretPos->SetValue(true); 
    }
    else
    {
        wxCheckBoxRecentSearchedExcludeFilters->SetValue(false);
        wxRecentSearchedFileFilters->SetValue(false);
        wxCheckBoxRecentSearchedDirectories->SetValue(false);
        wxCheckBoxRecentReplacedTexts->SetValue(false);
        wxCheckBoxRecentSearchedTexts->SetValue(false);
        wxCheckBoxRecentEncodings->SetValue(false);
        wxCheckBoxRecentFonts->SetValue(false);
        wxCheckBoxRecentFiles->SetValue(false);
		wxCheckBoxCaretPos->SetValue(false); 
    }
}
