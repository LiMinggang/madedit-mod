///////////////////////////////////////////////////////////////////////////////
// Name:        MadConvEncDialog.cpp
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadConvEncDialog.h"
#include "MadEdit/MadEncoding.h"
#include <wx/config.h>

//(*InternalHeaders(MadConvEncDialog)
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

MadConvEncDialog *g_ConvEncDialog=NULL;
//(*IdInit(MadConvEncDialog)
const long MadConvEncDialog::ID_STATICTEXT1 = wxNewId();
const long MadConvEncDialog::ID_WXCOMBOBOXENCODING = wxNewId();
const long MadConvEncDialog::ID_WXRADIOBOXOPTION = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadConvEncDialog,wxDialog)
	//(*EventTable(MadConvEncDialog)
	//*)
END_EVENT_TABLE()

MadConvEncDialog::MadConvEncDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MadConvEncDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Convert Encoding"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("New Encoding:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 1, wxALL|wxEXPAND, 5);
	WxComboBoxEncoding = new wxComboBox(this, ID_WXCOMBOBOXENCODING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXENCODING"));
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
	WxRadioBoxOption = new wxRadioBox(this, ID_WXRADIOBOXOPTION, _("Addtional Option"), wxDefaultPosition, wxDefaultSize, 6, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_WXRADIOBOXOPTION"));
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
	//*)

    size_t cnt=MadEncoding::GetEncodingsCount();
    for(size_t i=0;i<cnt;++i)
    {
        WxComboBoxEncoding->Append(MadEncoding::GetEncodingName(i));//enc+des);
    }
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

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
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

    g_ConvEncDialog=NULL;
	Destroy();
}
