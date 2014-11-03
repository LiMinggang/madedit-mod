///-----------------------------------------------------------------
///
/// @file      MadNumberDlg.cpp
/// @author    user
/// Created:   2014/9/17 14:31:05
/// @section   DESCRIPTION
///            MadNumberDlg class implementation
///
///------------------------------------------------------------------

#include "MadNumberDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// MadNumberDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadNumberDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadNumberDlg::OnClose)
	EVT_BUTTON(wxID_OK,MadNumberDlg::WxOKButtonClick)
END_EVENT_TABLE()
////Event Table End

MadNumberDlg * g_MadNumberDlg = NULL;

MadNumberDlg::MadNumberDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

MadNumberDlg::~MadNumberDlg()
{
} 

void MadNumberDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND | wxALL, 5);

	WxBoxSizer11 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer11, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Intial Number"), wxPoint(32, 5), wxDefaultSize, 0, wxT("WxStaticText1"));
	WxBoxSizer11->Add(WxStaticText1, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditInitialNumber = new wxTextCtrl(this, ID_WXEDITINITIALNUMBER, wxT("0"), wxPoint(10, 34), wxSize(122, 18), 0, wxTextValidator(wxFILTER_NUMERIC, NULL), wxT("WxEditInitialNumber"));
	WxBoxSizer11->Add(WxEditInitialNumber, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText8 = new wxStaticText(this, ID_WXSTATICTEXT8, _("Numbering Step"), wxPoint(25, 62), wxDefaultSize, 0, wxT("WxStaticText8"));
	WxBoxSizer11->Add(WxStaticText8, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditNumberingStep = new wxTextCtrl(this, ID_EDITNUMBERINGSTEP, wxT("1"), wxPoint(10, 91), wxSize(121, 19), 0, wxTextValidator(wxFILTER_NUMERIC, NULL), wxT("WxEditNumberingStep"));
	WxBoxSizer11->Add(WxEditNumberingStep, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, _("Number of Chars"), wxPoint(23, 120), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxBoxSizer11->Add(WxStaticText5, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditNumberOfChars = new wxTextCtrl(this, ID_WXEDITNUMBEROFCHARS, wxT("0"), wxPoint(9, 149), wxSize(124, 17), 0, wxTextValidator(wxFILTER_NUMERIC, NULL), wxT("WxEditNumberOfChars"));
	WxBoxSizer11->Add(WxEditNumberOfChars, 0, wxALIGN_LEFT | wxALL, 5);

	WxPadChar = new wxCheckBox(this, ID_PADWITH, _("Pad With Zero"), wxPoint(5, 176), wxSize(132, 15), 0, wxDefaultValidator, wxT("WxPadChar"));
	WxPadChar->SetValue(true);
	WxBoxSizer11->Add(WxPadChar, 0, wxALIGN_LEFT | wxALL, 5);

	WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer4, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _("Numbering Step Type"), wxPoint(9, 5), wxDefaultSize, 0, wxT("WxStaticText2"));
	WxBoxSizer4->Add(WxStaticText2, 0, wxALIGN_LEFT | wxALL, 5);

	wxArrayString arrayStringFor_WxChoiceNumberStepType;
	arrayStringFor_WxChoiceNumberStepType.Add(_("Linear"));
	arrayStringFor_WxChoiceNumberStepType.Add(_("Exponential"));
	WxChoiceNumberStepType = new wxChoice(this, ID_WXCHOICENUMBERSTEPTYPE, wxPoint(5, 34), wxSize(128, 23), arrayStringFor_WxChoiceNumberStepType, 0, wxDefaultValidator, wxT("WxChoiceNumberStepType"));
	WxChoiceNumberStepType->SetSelection(0);
	WxBoxSizer4->Add(WxChoiceNumberStepType, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("Format"), wxPoint(48, 67), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxBoxSizer4->Add(WxStaticText3, 0, wxALIGN_LEFT | wxALL, 5);

	wxArrayString arrayStringFor_WxChoiceFormat;
	arrayStringFor_WxChoiceFormat.Add(_("DEC"));
	arrayStringFor_WxChoiceFormat.Add(_("HEX"));
	arrayStringFor_WxChoiceFormat.Add(_("BIN"));
	arrayStringFor_WxChoiceFormat.Add(_("OCT"));
	WxChoiceFormat = new wxChoice(this, ID_WXFORMAT, wxPoint(5, 96), wxSize(128, 23), arrayStringFor_WxChoiceFormat, 0, wxDefaultValidator, wxT("WxChoiceFormat"));
	WxChoiceFormat->SetSelection(0);
	WxBoxSizer4->Add(WxChoiceFormat, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, _("Align"), wxPoint(51, 129), wxDefaultSize, 0, wxT("WxStaticText4"));
	WxBoxSizer4->Add(WxStaticText4, 0, wxALIGN_LEFT | wxALL, 5);

	wxArrayString arrayStringFor_WxChoiceAlign;
	arrayStringFor_WxChoiceAlign.Add(_("Align Left"));
	arrayStringFor_WxChoiceAlign.Add(_("Align Right"));
	WxChoiceAlign = new wxChoice(this, ID_WXALIGN, wxPoint(5, 158), wxSize(128, 23), arrayStringFor_WxChoiceAlign, 0, wxDefaultValidator, wxT("WxChoiceAlign"));
	WxChoiceAlign->SetSelection(1);
	WxBoxSizer4->Add(WxChoiceAlign, 0, wxALIGN_LEFT | wxALL, 5);

	WxBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer5, 0, wxALIGN_CENTER | wxALL, 5);

	WxOKButton = new wxButton(this, wxID_OK, _("OK"), wxPoint(5, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxOKButton"));
	WxBoxSizer5->Add(WxOKButton, 0, wxALIGN_CENTER | wxALL, 5);

	WxButton2 = new wxButton(this, wxID_CANCEL, _("Cancel"), wxPoint(90, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton2"));
	WxBoxSizer5->Add(WxButton2, 0, wxALIGN_CENTER | wxALL, 5);

	SetTitle(wxT("Numbering Configuration"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
}

void MadNumberDlg::OnClose(wxCloseEvent& /*event*/)
{
    Destroy();
    g_MadNumberDlg = NULL;
}

/*
 * WxOKButtonClick
 */
void MadNumberDlg::WxOKButtonClick(wxCommandEvent& event)
{
	// insert your code here
	
    long lo;
    bool error=false;
    wxString errtext(_("Invalid value of \"%s:%s\""));
	if(!WxEditNumberOfChars->GetValue().ToLong(&lo) || lo<0 || lo>NUMBERING_MAX_CHARS) //HardCode
    {
        wxString Err = WxEditNumberOfChars->GetValue() + _(": Valid range is 0~512, 0 means NO PADDING");
        wxLogError(errtext, WxStaticText5->GetLabel().c_str(), Err.c_str());
        error=true;
    }

	if(!WxEditInitialNumber->GetValue().ToLong(&lo))
    {
        wxLogError(errtext, WxStaticText1->GetLabel().c_str(), WxEditNumberOfChars->GetValue().c_str());
        error=true;
    }

	if(!WxEditNumberingStep->GetValue().ToLong(&lo))
    {
        wxLogError(errtext, WxStaticText8->GetLabel().c_str(), WxEditNumberingStep->GetValue().c_str());
        error=true;
    }
    
	if(!WxEditNumberOfChars->GetValue().ToLong(&lo))
    {
        wxLogError(errtext, WxStaticText5->GetLabel().c_str(), WxEditNumberOfChars->GetValue().c_str());
        error=true;
    }

    if(!error) EndModal(wxID_OK);
}

