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
//*)

MadPurgeHistoryDialog::MadPurgeHistoryDialog(wxWindow* parent,wxWindowID WXUNUSED(id),const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
{
	//(*Initialize(MadPurgeHistoryDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Purge Histories"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, wxEmptyString);
	wxCheckBoxRecentFiles = new wxCheckBox(this, wxID_ANY, _("Recent &Files"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTFILES"));
	wxCheckBoxRecentFiles->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentFiles, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxRecentFonts = new wxCheckBox(this, wxID_ANY, _("Recent Fon&ts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTFONTS"));
	wxCheckBoxRecentFonts->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentFonts, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxRecentEncodings = new wxCheckBox(this, wxID_ANY, _("Recent &Encodings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTENCODINGS"));
	wxCheckBoxRecentEncodings->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentEncodings, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxRecentSearchedTexts = new wxCheckBox(this, wxID_ANY, _("Recent &Searched Texts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTSEARCHEDTEXTS"));
	wxCheckBoxRecentSearchedTexts->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentSearchedTexts, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxRecentReplacedTexts = new wxCheckBox(this, wxID_ANY, _("Recent &Replaced Texts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTREPLACEDTEXTS"));
	wxCheckBoxRecentReplacedTexts->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentReplacedTexts, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxRecentSearchedDirectories = new wxCheckBox(this, wxID_ANY, _("Recent Searched &Directories"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTSEARCHEDDIRECTORIES"));
	wxCheckBoxRecentSearchedDirectories->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentSearchedDirectories, 0, wxALL|wxEXPAND, 5);
	wxRecentSearchedFileFilters = new wxCheckBox(this, wxID_ANY, _("Recent Searched File Fi&lters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXRECENTSEARCHEDFILEFILTERS"));
	wxRecentSearchedFileFilters->SetValue(false);
	StaticBoxSizer1->Add(wxRecentSearchedFileFilters, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxRecentSearchedExcludeFilters = new wxCheckBox(this, wxID_ANY, _("Recent Searched E&xclude Filters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECENTSEARCHEDEXCLUDEFILTERS"));
	wxCheckBoxRecentSearchedExcludeFilters->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxRecentSearchedExcludeFilters, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxCaretPos = new wxCheckBox(this, wxID_ANY, _("Recent Caret Position"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCARETPOS"));
	wxCheckBoxCaretPos->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxCaretPos, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxResetToolBarsInfoWin = new wxCheckBox(this, wxID_ANY, _("Reset Tool Bars and Information Window Pos"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRESETTOOLBARSINFOWIN"));
	wxCheckBoxResetToolBarsInfoWin->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxResetToolBarsInfoWin, 0, wxALL|wxEXPAND, 5);
	wxCheckBoxAllAbove = new wxCheckBox(this, wxID_ANY, _("All Above"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXALLABOVE"));
	wxCheckBoxAllAbove->SetValue(false);
	StaticBoxSizer1->Add(wxCheckBoxAllAbove, 0, wxALL|wxEXPAND, 5);
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

	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &MadPurgeHistoryDialog::wxCheckBoxAllAboveClick, this, wxCheckBoxAllAbove->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadPurgeHistoryDialog::WxButtonOKClick, this, wxID_OK );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadPurgeHistoryDialog::WxButtonCancelClick, this, wxID_CANCEL );
	Bind( wxEVT_CLOSE_WINDOW, &MadPurgeHistoryDialog::MadPurgeHistoryDialogClose, this, wxID_ANY );
	Bind(wxEVT_KEY_DOWN, &MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown, this, wxID_ANY );
	//*)
}

MadPurgeHistoryDialog::~MadPurgeHistoryDialog()
{
	//(*Destroy(MadPurgeHistoryDialog)
	//*)
}


void MadPurgeHistoryDialog::MadPurgeHistoryDialogClose(wxCloseEvent& WXUNUSED(event))
{
	Destroy();
}

void MadPurgeHistoryDialog::WxButtonOKClick(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_OK);
}

/*
 * wxButtonCancelClick
 */
void MadPurgeHistoryDialog::WxButtonCancelClick(wxCommandEvent& WXUNUSED(event))
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
            WxButtonCancelClick(e);
            return;
        }
    default:
        break;
    }

    int flags=wxACCEL_NORMAL;
    if(event.m_altDown) flags|=wxACCEL_ALT;
    if(event.m_shiftDown) flags|=wxACCEL_SHIFT;
    if(event.m_controlDown) flags|=wxACCEL_CTRL;

    if('O' == key && wxACCEL_ALT == flags)
    {
        wxCommandEvent e;
        WxButtonOKClick(e);
        return; // no skip
    }

    if('C' == key && wxACCEL_ALT == flags)
    {
        wxCommandEvent e;
        WxButtonCancelClick(e);
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
		wxCheckBoxResetToolBarsInfoWin->SetValue(true);
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
		wxCheckBoxResetToolBarsInfoWin->SetValue(false);
    }
}
