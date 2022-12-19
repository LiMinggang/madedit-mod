///////////////////////////////////////////////////////////////////////////////
// Name:        MadNumberDlg.cpp
// Description:
// Author:      minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadNumberDlg.h"

//Do not add custom	headers
//wxDev-C++	designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// MadNumberDlg
//----------------------------------------------------------------------------
//Add Custom Events	only in	the	appropriate	block.

MadNumberDlg * g_MadNumberDlg = nullptr;

MadNumberDlg::MadNumberDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	WxStaticTextPreview = 0;
	CreateGUIControls();

	Bind(wxEVT_CLOSE_WINDOW, &MadNumberDlg::OnClose, this);
	Bind(wxEVT_BUTTON, &MadNumberDlg::WxOKButtonClick, this, wxID_OK);

	Bind(wxEVT_TEXT,&MadNumberDlg::WxEditItialnumberUpdated, this, WxEditInitialNumber->GetId());
	
	Bind(wxEVT_TEXT, &MadNumberDlg::WxEditPostfixUpdated, this, WxEditPostfix->GetId());
	Bind(wxEVT_CHECKBOX, &MadNumberDlg::WxCheckPostfixClick, this, WxCheckPostfix->GetId());

	Bind(wxEVT_TEXT, &MadNumberDlg::WxEditPrefixUpdated, this, WxEditPrefix->GetId());
	Bind(wxEVT_CHECKBOX, &MadNumberDlg::WxCheckPrefixClick, this, WxCheckPrefix->GetId());
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

	WxBoxSizer1	= new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND | wxALL, 5);

	WxBoxSizer11 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer11, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText1 = new wxStaticText(this, wxID_ANY, _("Intial Number"), wxPoint(32, 5), wxDefaultSize, 0, wxT("WxStaticText1"));
	WxBoxSizer11->Add(WxStaticText1, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditInitialNumber = new wxTextCtrl(this, wxID_ANY, wxT("0"), wxPoint(10, 34), wxSize(122, -1), 0, wxTextValidator(wxFILTER_NUMERIC, nullptr), wxT("WxEditInitialNumber"));
	WxBoxSizer11->Add(WxEditInitialNumber, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText8 = new wxStaticText(this, wxID_ANY, _("Numbering Step"), wxPoint(25, 62), wxDefaultSize, 0, wxT("WxStaticText8"));
	WxBoxSizer11->Add(WxStaticText8, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditNumberingStep = new wxTextCtrl(this, wxID_ANY, wxT("1"), wxPoint(10, 91), wxSize(121, -1), 0, wxTextValidator(wxFILTER_NUMERIC, nullptr), wxT("WxEditNumberingStep"));
	WxBoxSizer11->Add(WxEditNumberingStep, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText5 = new wxStaticText(this, wxID_ANY, _("Number of Chars"), wxPoint(23, 120), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxBoxSizer11->Add(WxStaticText5, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditNumberOfChars = new wxTextCtrl(this, wxID_ANY, wxT("0"), wxPoint(9, 149), wxSize(124, -1), 0, wxTextValidator(wxFILTER_NUMERIC, nullptr), wxT("WxEditNumberOfChars"));
	WxBoxSizer11->Add(WxEditNumberOfChars, 0, wxALIGN_LEFT | wxALL, 5);

	WxPadChar = new wxCheckBox(this, wxID_ANY, _("Pad With Zero"), wxPoint(5, 176), wxSize(132, -1), 0, wxDefaultValidator, wxT("WxPadChar"));
	WxPadChar->SetValue(true);
	WxBoxSizer11->Add(WxPadChar, 0, wxALIGN_LEFT | wxALL, 5);

	WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer4, 0, wxALIGN_LEFT |	wxALL, 5);

	WxStaticText2 = new wxStaticText(this, wxID_ANY, _("Numbering Step Type"), wxPoint(9, 5), wxDefaultSize, 0, wxT("WxStaticText2"));
	WxBoxSizer4->Add(WxStaticText2, 0, wxALIGN_LEFT | wxALL, 5);

	wxArrayString arrayStringFor_WxChoiceNumberStepType;
	arrayStringFor_WxChoiceNumberStepType.Add(_("Linear"));
	arrayStringFor_WxChoiceNumberStepType.Add(_("Exponential"));
	WxChoiceNumberStepType = new wxChoice(this, wxID_ANY, wxPoint(5, 34), wxSize(128, -1), arrayStringFor_WxChoiceNumberStepType, 0, wxDefaultValidator, wxT("WxChoiceNumberStepType"));
	WxChoiceNumberStepType->SetSelection(0);
	WxBoxSizer4->Add(WxChoiceNumberStepType, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText3 = new wxStaticText(this, wxID_ANY, _("Format"), wxPoint(48, 67), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxBoxSizer4->Add(WxStaticText3, 0, wxALIGN_LEFT | wxALL, 5);

	wxArrayString arrayStringFor_WxChoiceFormat;
	arrayStringFor_WxChoiceFormat.Add(_("DEC"));
	arrayStringFor_WxChoiceFormat.Add(_("HEX"));
	arrayStringFor_WxChoiceFormat.Add(_("BIN"));
	arrayStringFor_WxChoiceFormat.Add(_("OCT"));
	WxChoiceFormat = new wxChoice(this, wxID_ANY, wxPoint(5, 96), wxSize(128, -1), arrayStringFor_WxChoiceFormat, 0, wxDefaultValidator, wxT("WxChoiceFormat"));
	WxChoiceFormat->SetSelection(0);
	WxBoxSizer4->Add(WxChoiceFormat, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticText4 = new wxStaticText(this, wxID_ANY, _("Align"), wxPoint(51, 129), wxDefaultSize, 0, wxT("WxStaticText4"));
	WxBoxSizer4->Add(WxStaticText4, 0, wxALIGN_LEFT | wxALL, 5);

	wxArrayString arrayStringFor_WxChoiceAlign;
	arrayStringFor_WxChoiceAlign.Add(_("Align Left"));
	arrayStringFor_WxChoiceAlign.Add(_("Align Right"));
	WxChoiceAlign = new wxChoice(this, wxID_ANY, wxPoint(5, 158), wxSize(128, -1), arrayStringFor_WxChoiceAlign, 0, wxDefaultValidator, wxT("WxChoiceAlign"));
	WxChoiceAlign->SetSelection(1);
	WxBoxSizer4->Add(WxChoiceAlign, 0, wxALIGN_LEFT | wxALL, 5);

	WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_LEFT |	wxALL, 5);

	WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer6, 0, wxALIGN_CENTER | wxALL, 5);

	WxCheckPrefix = new wxCheckBox(this, wxID_ANY, _("Prefix"), wxPoint(5, 5), wxSize(132, -1), 0, wxDefaultValidator, wxT("WxCheckPrefix"));
	WxBoxSizer6->Add(WxCheckPrefix, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditPrefix = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(147, 5), wxSize(124, -1), 0, wxDefaultValidator, wxT("WxEditPrefix"));
	WxBoxSizer6->Add(WxEditPrefix, 0, wxALIGN_LEFT | wxALL, 5);
	WxEditPrefix->Enable(false);

	WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer7, 0, wxALIGN_CENTER | wxALL, 5);

	WxCheckPostfix = new wxCheckBox(this, wxID_ANY, _("Postfix"), wxPoint(5, 5), wxSize(132, -1), 0, wxDefaultValidator, wxT("WxCheckPostfix"));
	WxBoxSizer7->Add(WxCheckPostfix, 0, wxALIGN_LEFT | wxALL, 5);

	WxEditPostfix = new wxTextCtrl(this, wxID_ANY, wxT("."), wxPoint(147, 5), wxSize(124, -1), 0, wxDefaultValidator, wxT("WxEditPostfix"));
	WxBoxSizer7->Add(WxEditPostfix, 0, wxALIGN_LEFT | wxALL, 5);
	WxEditPostfix->Enable(false);

	WxBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer8, 0, wxALIGN_LEFT | wxALL, 5);

	WxBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer5, 0, wxALIGN_CENTER | wxALL, 5);

	WxOKButton = new wxButton(this, wxID_OK, _("OK"), wxPoint(5, 5), wxDefaultSize, 0, wxDefaultValidator, wxT("WxOKButton"));
	WxBoxSizer5->Add(WxOKButton, 0, wxALIGN_CENTER | wxALL, 5);

	WxCancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxPoint(90, 5), wxDefaultSize, 0, wxDefaultValidator, wxT("WxCancelButton"));
	WxBoxSizer5->Add(WxCancelButton, 0, wxALIGN_CENTER | wxALL, 5);

	WxStaticText6 = new wxStaticText(this, wxID_ANY, _("Preview: "), wxPoint(5, 5), wxDefaultSize, 0, wxT("WxStaticText6"));
	WxBoxSizer8->Add(WxStaticText6, 0, wxALIGN_LEFT | wxALL, 5);

	WxStaticTextPreview = new wxStaticText(this, wxID_ANY, wxT("0"), wxPoint(63, 5), wxDefaultSize, 0, wxT("WxStaticTextPreview"));
	WxBoxSizer8->Add(WxStaticTextPreview, 0, wxALIGN_LEFT | wxALL, 5);

	SetTitle(_("Numbering Configuration"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI	Items Creation End
}

void MadNumberDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
	g_MadNumberDlg = nullptr;
}

/*
 * WxOKButtonClick
 */
void MadNumberDlg::WxOKButtonClick(wxCommandEvent& WXUNUSED(event))
{
	// insert your code here
	
	long lo;
	bool error=false;
	wxString errtext(_("Invalid value of \"%s:%s\""));
	if (!WxEditNumberOfChars->GetValue().ToLong(&lo) || lo<0 || lo>NUMBERING_MAX_CHARS) //HardCode
	{
		wxString Err = WxEditNumberOfChars->GetValue() + _(": Valid range is 0~512, 0 means NO PADDING");
		wxLogError(errtext, WxStaticText5->GetLabel().c_str(), Err.c_str());
		error=true;
	}

	if (!WxEditInitialNumber->GetValue().ToLong(&lo))
	{
		wxLogError(errtext, WxStaticText1->GetLabel().c_str(), WxEditNumberOfChars->GetValue().c_str());
		error=true;
	}

	if (!WxEditNumberingStep->GetValue().ToLong(&lo))
	{
		wxLogError(errtext, WxStaticText8->GetLabel().c_str(), WxEditNumberingStep->GetValue().c_str());
		error=true;
	}
	
	if (!WxEditNumberOfChars->GetValue().ToLong(&lo))
	{
		wxLogError(errtext, WxStaticText5->GetLabel().c_str(), WxEditNumberOfChars->GetValue().c_str());
		error=true;
	}

	if (!error) EndModal(wxID_OK);
}


/*
 * WxEditPrefixUpdated
 */
void MadNumberDlg::WxEditPrefixUpdated(wxCommandEvent& WXUNUSED(event))
{
	// insert your code	here
	if (WxStaticTextPreview)
	{	
		wxString preview((WxCheckPrefix->GetValue()?WxEditPrefix->GetValue():wxT(""))+WxEditInitialNumber->GetValue());
		if (WxCheckPostfix->GetValue())
			preview += WxEditPostfix->GetValue();
		WxStaticTextPreview->SetLabel(preview);
	}
}

void MadNumberDlg::WxEditItialnumberUpdated(wxCommandEvent&	WXUNUSED(event))
{
	// insert your code	here
	if (WxStaticTextPreview)
	{
		wxString preview;
		if (WxCheckPrefix->GetValue())
		{
			if (WxCheckPrefix->GetValue())
				preview += WxEditPrefix->GetValue();
			
			preview += WxEditInitialNumber->GetValue();
		}
		else
		{
			preview = WxEditInitialNumber->GetValue();
		}
		if (WxCheckPostfix->GetValue())
			preview += WxEditPostfix->GetValue();
		WxStaticTextPreview->SetLabel(preview);
	}
}

/*
 * WxEditPostfixUpdated
 */
void MadNumberDlg::WxEditPostfixUpdated(wxCommandEvent&	WXUNUSED(event))
{
	// insert your code	here
	if (WxStaticTextPreview)
	{	
		wxString preview;
		if (WxCheckPrefix->GetValue())
			preview += WxEditPrefix->GetValue();
		
		preview += WxEditInitialNumber->GetValue();
		if (WxCheckPostfix->GetValue())
			preview += WxEditPostfix->GetValue();
		WxStaticTextPreview->SetLabel(preview);
	}
}

/*
 * WxCheckPrefixClick
 */
void MadNumberDlg::WxCheckPrefixClick(wxCommandEvent& WXUNUSED(event))
{
	// insert your code	here
	wxString preview;
	if (WxCheckPrefix->GetValue())
	{
		WxEditPrefix->Enable(true);
		preview += WxEditPrefix->GetValue()+WxEditInitialNumber->GetValue();
		if (WxCheckPostfix->GetValue())
			preview += WxEditPostfix->GetValue();
	}
	else
	{
		WxEditPrefix->Enable(false);
		preview += WxEditInitialNumber->GetValue();
		if (WxCheckPostfix->GetValue())
			preview += WxEditPostfix->GetValue();
	}
	WxStaticTextPreview->SetLabel(preview);
}

/*
 * WxCheckPostfixClick
 */
void MadNumberDlg::WxCheckPostfixClick(wxCommandEvent& WXUNUSED(event))
{
	// insert your code	here
	wxString preview;
	if (WxCheckPostfix->GetValue())
	{
		WxEditPostfix->Enable(true);
		if (WxCheckPrefix->GetValue())
			preview += WxEditPrefix->GetValue();
		
		preview += WxEditInitialNumber->GetValue() + WxEditPostfix->GetValue();
	}
	else
	{
		WxEditPostfix->Enable(false);
		if (WxCheckPrefix->GetValue())
			preview += WxEditPrefix->GetValue();
		
		preview += WxEditInitialNumber->GetValue();
	}
	WxStaticTextPreview->SetLabel(preview);
}
