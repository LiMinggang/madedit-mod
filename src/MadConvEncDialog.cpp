///////////////////////////////////////////////////////////////////////////////
// Name:        MadConvEncDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////
#include "MadUtils.h"

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable : 4996)
#endif
// disable 4996 {
//(*InternalHeaders(MadConvEncDialog)
#include <wx/config.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#include "MadConvEncDialog.h"
#include "MadEdit/MadEncoding.h"

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

MadConvEncDialog *g_ConvEncDialog=nullptr;
//(*IdInit(MadConvEncDialog)
//*)

MadConvEncDialog::MadConvEncDialog(wxWindow* parent,wxWindowID id,const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
{
	//(*Initialize(MadConvEncDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;

	Create(parent, id, _("Convert Encoding"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, wxID_ANY, _("New Encoding:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer2->Add(StaticText1, 1, wxALL|wxEXPAND, 5);
	WxComboBoxEncoding = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer2->Add(WxComboBoxEncoding, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxString __wxRadioBoxChoices_1[6] =
	{
		_("None"),
		_("Simplified Chinese => Traditional Chinese"),
		_("Traditional Chinese => Simplified Chinese"),
		_("Japanese Kanji => Traditional Chinese"),
		_("Japanese Kanji => Simplified Chinese"),
		_("Chinese => Japanese Kanji")
	};
	WxRadioBoxOption = new wxRadioBox(this, wxID_ANY, _("Addtional Option"), wxDefaultPosition, wxDefaultSize, 6, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("wxID_ANY"));
	WxRadioBoxOption->InvalidateBestSize();
	BoxSizer3->Add(WxRadioBoxOption, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer4->Add(WxButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	WxButtonCancel->SetDefault();
	BoxSizer4->Add(WxButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	//Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadConvEncDialog::MadConvEncDialogClose);
	//*)

	Bind(wxEVT_CLOSE_WINDOW, &MadConvEncDialog::MadConvEncDialogClose, this);

	size_t cnt=MadEncoding::GetEncodingsCount();
	for (size_t i=0;i<cnt;++i)
	{
		WxComboBoxEncoding->Append(MadEncoding::GetEncodingName(i));//enc+des);
	}

	wxString convenc=MadEncoding::GetEncodingName(0);
	wxConfigBase *cfg=wxConfigBase::Get(false);
	wxString oldpath=cfg->GetPath();
	cfg->Read(wxT("/MadEdit/ConvertEncoding"), &convenc);
	cfg->SetPath(oldpath);
	WxComboBoxEncoding->SetValue(convenc);

	WxButtonCancel->SetFocus();
}

MadConvEncDialog::~MadConvEncDialog()
{
	//(*Destroy(MadConvEncDialog)
	//*)
}


void MadConvEncDialog::MadConvEncDialogClose(wxCloseEvent& event)
{
	if (event.CanVeto() && (nullptr != dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow())))
	{
		event.Veto();
		Show(false);
		return;
	}

	g_ConvEncDialog=nullptr;
	Destroy();
}
