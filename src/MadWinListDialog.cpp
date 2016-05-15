#include "MadWinListDialog.h"

//(*InternalHeaders(MadWinListDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MadWinListDialog)
const long MadWinListDialog::ID_LISTCTRLMADWINLIST = wxNewId();
const long MadWinListDialog::ID_BUTTONACTIVATE = wxNewId();
const long MadWinListDialog::ID_BUTTONSAVE = wxNewId();
const long MadWinListDialog::ID_BUTTONCLOSEWINDOWS = wxNewId();
const long MadWinListDialog::ID_BUTTONSORTTAB = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadWinListDialog,wxDialog)
	//(*EventTable(MadWinListDialog)
	//*)
END_EVENT_TABLE()

MadWinListDialog::MadWinListDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MadWinListDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	MadWindowsList = new wxListCtrl(this, ID_LISTCTRLMADWINLIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSIMPLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_LISTCTRLMADWINLIST"));
	BoxSizer2->Add(MadWindowsList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ButtonActivate = new wxButton(this, ID_BUTTONACTIVATE, _("Activate"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONACTIVATE"));
	BoxSizer3->Add(ButtonActivate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSave = new wxButton(this, ID_BUTTONSAVE, _("Save"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONSAVE"));
	BoxSizer3->Add(ButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCloseWindows = new wxButton(this, ID_BUTTONCLOSEWINDOWS, _("Close Window(s)"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONCLOSEWINDOWS"));
	BoxSizer3->Add(ButtonCloseWindows, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSortTab = new wxButton(this, ID_BUTTONSORTTAB, _("Sort Tab"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONSORTTAB"));
	BoxSizer3->Add(ButtonSortTab, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonOk = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer3->Add(ButtonOk, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONACTIVATE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonActivateClick);
	Connect(ID_BUTTONSAVE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonSaveClick);
	Connect(ID_BUTTONCLOSEWINDOWS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonCloseWindowsClick);
	Connect(ID_BUTTONSORTTAB,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonSortTabClick);
	Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonOkClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadWinListDialog::OnMadWinListDialogClose);
	//*)
}

MadWinListDialog::~MadWinListDialog()
{
	//(*Destroy(MadWinListDialog)
	//*)
}


void MadWinListDialog::OnButtonActivateClick(wxCommandEvent& event)
{
}

void MadWinListDialog::OnButtonSaveClick(wxCommandEvent& event)
{
}

void MadWinListDialog::OnButtonCloseWindowsClick(wxCommandEvent& event)
{
}

void MadWinListDialog::OnButtonSortTabClick(wxCommandEvent& event)
{
}

void MadWinListDialog::OnButtonOkClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void MadWinListDialog::OnMadWinListDialogClose(wxCloseEvent& event)
{
    Destroy();
}
