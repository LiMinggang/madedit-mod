///-----------------------------------------------------------------
///
/// @file      MadPurgeHistoryDialog.cpp
/// @author    GLi
/// Created:   2014/6/27 13:25:39
/// @section   DESCRIPTION
///            MadPurgeHistoryDialog class implementation
///
///------------------------------------------------------------------

#include "MadEditFrame.h"
#include "MadSearchReplaceDialog.h"
#include "MadFindInFilesDialog.h"
#include "MadPurgeHistoryDialog.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// MadPurgeHistoryDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadPurgeHistoryDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadPurgeHistoryDialog::OnClose)
	EVT_KEY_DOWN(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown)
	EVT_CHECKBOX(ID_WXCHECKBOXALLABOVE,MadPurgeHistoryDialog::OnAllAboveClick)
	EVT_BUTTON(wxID_CANCEL,MadPurgeHistoryDialog::wxButtonCancelClick)
	EVT_BUTTON(wxID_OK,MadPurgeHistoryDialog::WxButtonOKClick)
END_EVENT_TABLE()
////Event Table End

MadPurgeHistoryDialog::MadPurgeHistoryDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

MadPurgeHistoryDialog::~MadPurgeHistoryDialog()
{
} 

void MadPurgeHistoryDialog::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox(this, wxID_ANY, wxT(""));
	WxStaticBoxSizer1 = new wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL);
	this->SetSizer(WxStaticBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	WxStaticBoxSizer1->Add(WxBoxSizer1, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxCheckBoxRecentFiles = new wxCheckBox(this, ID_WXCHECKBOXRECENTFILES, _("Recent Files"), wxPoint(5, 5), wxSize(212, 16), 0, wxDefaultValidator, wxT("wxCheckBoxRecentFiles"));
	WxBoxSizer1->Add(wxCheckBoxRecentFiles, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxCheckBoxRecentFonts = new wxCheckBox(this, ID_WXCHECKBOXRECENTFONTS, _("Recent Fonts"), wxPoint(5, 31), wxSize(212, 18), 0, wxDefaultValidator, wxT("wxCheckBoxRecentFonts"));
	WxBoxSizer1->Add(wxCheckBoxRecentFonts, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxCheckBoxRecentEncodings = new wxCheckBox(this, ID_WXCHECKBOXRECENTENCODINGS, _("Recent Encodings"), wxPoint(5, 59), wxSize(212, 16), 0, wxDefaultValidator, wxT("wxCheckBoxRecentEncodings"));
	WxBoxSizer1->Add(wxCheckBoxRecentEncodings, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxCheckBoxRecentSearchedTexts = new wxCheckBox(this, ID_WXCHECKBOXRECENTSEARCHEDTEXTS, _("Recent Searched Texts"), wxPoint(5, 85), wxSize(212, 18), 0, wxDefaultValidator, wxT("wxCheckBoxRecentSearchedTexts"));
	WxBoxSizer1->Add(wxCheckBoxRecentSearchedTexts, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxCheckBoxRecentReplacedTexts = new wxCheckBox(this, ID_WXCHECKBOXRECENTREPLACEDTEXTS, _("Recent Replaced Texts"), wxPoint(5, 113), wxSize(212, 17), 0, wxDefaultValidator, wxT("wxCheckBoxRecentReplacedTexts"));
	WxBoxSizer1->Add(wxCheckBoxRecentReplacedTexts, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxCheckBoxRecentSearchedDirectories = new wxCheckBox(this, ID_WXCHECKBOXRECENTSEARCHEDDIRECTORIES, _("Recent Searched Directories"), wxPoint(5, 140), wxSize(212, 17), 0, wxDefaultValidator, wxT("wxCheckBoxRecentSearchedDirectories"));
	WxBoxSizer1->Add(wxCheckBoxRecentSearchedDirectories, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	wxRecentSearchedFileFilters = new wxCheckBox(this, ID_WXRECENTSEARCHEDFILEFILTERS, _("Recent Searched File Filters"), wxPoint(5, 167), wxSize(212, 18), 0, wxDefaultValidator, wxT("wxRecentSearchedFileFilters"));
	WxBoxSizer1->Add(wxRecentSearchedFileFilters, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 5);

	wxCheckBoxRecentSearchedExcludeFilters = new wxCheckBox(this, ID_WXCHECKBOXRECENTSEARCHEDEXCLUDEFILTERS, _("Recent Searched Exclude Filters"), wxPoint(5, 195), wxSize(212, 17), 0, wxDefaultValidator, wxT("wxCheckBoxRecentSearchedExcludeFilters"));
	WxBoxSizer1->Add(wxCheckBoxRecentSearchedExcludeFilters, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 5);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxStaticBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxPoint(5, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButtonOK"));
	WxBoxSizer2->Add(WxButtonOK, 0, wxALIGN_CENTER | wxALL, 5);

	wxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(90, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("wxButtonCancel"));
	WxBoxSizer2->Add(wxButtonCancel, 0, wxALIGN_CENTER | wxALL, 5);

	WxCheckBoxAllAbove = new wxCheckBox(this, ID_WXCHECKBOXALLABOVE, _("All Above"), wxPoint(5, 222), wxSize(212, 17), 0, wxDefaultValidator, wxT("WxCheckBoxAllAbove"));
	WxBoxSizer1->Add(WxCheckBoxAllAbove, 0, wxALIGN_CENTER | wxALL, 5);

	SetTitle(_("Purge Histories"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End

	// connect to KeyDown event handler
	wxCheckBoxRecentFiles->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxCheckBoxRecentFonts->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxCheckBoxRecentEncodings->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxCheckBoxRecentSearchedTexts->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxCheckBoxRecentReplacedTexts->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxCheckBoxRecentSearchedDirectories->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxRecentSearchedFileFilters->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxCheckBoxRecentSearchedExcludeFilters->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	WxButtonOK->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	wxButtonCancel->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadPurgeHistoryDialog::MadPurgeHistoryDialogKeyDown));
	WxButtonOK->SetDefault();
}

void MadPurgeHistoryDialog::OnClose(wxCloseEvent& /*event*/)
{
    Destroy();
}

/*
 * WxButtonOKClick
 */
void MadPurgeHistoryDialog::WxButtonOKClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

/*
 * wxButtonCancelClick
 */
void MadPurgeHistoryDialog::wxButtonCancelClick(wxCommandEvent& event)
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
            this->wxButtonCancelClick(e);
            return;
        }
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        if ((wxButton*)this != this->wxButtonCancel)
        {
            wxCommandEvent e;
            this->WxButtonOKClick(e);
            return; // no skip
        }
        else
        {
            wxCommandEvent e;
            this->wxButtonCancelClick(e);
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
        this->wxButtonCancelClick(e);
        return; // no skip
    }

    event.Skip();
}

void MadPurgeHistoryDialog::OnAllAboveClick(wxCommandEvent& event)
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
    }
}

