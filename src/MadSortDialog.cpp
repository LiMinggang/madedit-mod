///////////////////////////////////////////////////////////////////////////////
// Name:		MadSortDialog.cpp
// Description:	Sort Options Dialog
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadSortDialog.h"

//(*InternalHeaders(MadSortDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MadSortDialog)
const long MadSortDialog::ID_WXRADIOBOXORDER = wxNewId();
const long MadSortDialog::ID_WXCHECKBOXCASE = wxNewId();
const long MadSortDialog::ID_WXCHECKBOXNUMERIC = wxNewId();
const long MadSortDialog::ID_WXCHECKBOXREMOVEDUP = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadSortDialog,wxDialog)
	//(*EventTable(MadSortDialog)
	//*)
END_EVENT_TABLE()

MadSortDialog::MadSortDialog(wxWindow* parent)
{
	//(*Initialize(MadSortDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, _("Sort Options"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	wxString __wxRadioBoxChoices_1[2] =
	{
		_("&Ascending"),
		_("&Descending")
	};
	WxRadioBoxOrder = new wxRadioBox(this, ID_WXRADIOBOXORDER, _("Sort Order"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_WXRADIOBOXORDER"));
	BoxSizer1->Add(WxRadioBoxOrder, 1, wxALL|wxALIGN_LEFT, 5);
	WxCheckBoxCase = new wxCheckBox(this, ID_WXCHECKBOXCASE, _("&Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCASE"));
	WxCheckBoxCase->SetValue(false);
	BoxSizer1->Add(WxCheckBoxCase, 0, wxALL|wxALIGN_LEFT, 5);
	WxCheckBoxNumeric = new wxCheckBox(this, ID_WXCHECKBOXNUMERIC, _("&Numeric Sort"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXNUMERIC"));
	WxCheckBoxNumeric->SetValue(false);
	BoxSizer1->Add(WxCheckBoxNumeric, 0, wxALL|wxALIGN_LEFT, 5);
	WxCheckBoxRemoveDup = new wxCheckBox(this, ID_WXCHECKBOXREMOVEDUP, _("&Remove Duplicates"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXREMOVEDUP"));
	WxCheckBoxRemoveDup->SetValue(false);
	BoxSizer1->Add(WxCheckBoxRemoveDup, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonSort = new wxButton(this, wxID_OK, _("&Sort"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	WxButtonSort->SetDefault();
	BoxSizer3->Add(WxButtonSort, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer3->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadSortDialog::MadSortDialogClose);
	//*)
}

MadSortDialog::~MadSortDialog()
{
	//(*Destroy(MadSortDialog)
	//*)
}


void MadSortDialog::MadSortDialogClose(wxCloseEvent& event)
{
    Destroy();
}
