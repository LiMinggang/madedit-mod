#include "MadSaveQueryDialog.h"

//(*InternalHeaders(MadSaveQueryDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/arrstr.h>
#include <algorithm>  

#include <wx/filename.h>
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"

//(*IdInit(MadSaveQueryDialog)
const long MadSaveQueryDialog::ID_CHECKLISTBOXMADFILELIST = wxNewId();
const long MadSaveQueryDialog::ID_BUTTONSAVENONE = wxNewId();
const long MadSaveQueryDialog::ID_BUTTONSELECTALL = wxNewId();
const long MadSaveQueryDialog::ID_BUTTONDSELECTALL = wxNewId();
const long MadSaveQueryDialog::ID_BUTTONGOTO = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadSaveQueryDialog,wxDialog)
	//(*EventTable(MadSaveQueryDialog)
	//*)
END_EVENT_TABLE()

MadSaveQueryDialog *g_ModifiedFileListDialog = NULL;

MadSaveQueryDialog::MadSaveQueryDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MadSaveQueryDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Save Modified Files"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxCAPTION|wxCLOSE_BOX|wxSIMPLE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	MadFileList = new wxCheckedListCtrl(this, ID_CHECKLISTBOXMADFILELIST, wxDefaultPosition, wxSize(400,300), wxCLC_CHECK_WHEN_SELECTING, wxDefaultValidator, _T("ID_CHECKLISTBOXMADFILELIST"));
	BoxSizer2->Add(MadFileList, 0, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer3->Add(ButtonOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSaveNone = new wxButton(this, ID_BUTTONSAVENONE, _("Save &None"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSAVENONE"));
	BoxSizer3->Add(ButtonSaveNone, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer3->Add(ButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSelectAll = new wxButton(this, ID_BUTTONSELECTALL, _("Select &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSELECTALL"));
	BoxSizer3->Add(ButtonSelectAll, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonDselectAll = new wxButton(this, ID_BUTTONDSELECTALL, _("Dselect &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDSELECTALL"));
	BoxSizer3->Add(ButtonDselectAll, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonGoTo = new wxButton(this, ID_BUTTONGOTO, _("&Go To"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONGOTO"));
	BoxSizer3->Add(ButtonGoTo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSaveQueryDialog::OnButtonOKClick);
	Connect(ID_BUTTONSAVENONE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSaveQueryDialog::OnButtonSaveNoneClick);
	Connect(wxID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSaveQueryDialog::OnButtonCancelClick);
	Connect(ID_BUTTONSELECTALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSaveQueryDialog::OnButtonSelectAllClick);
	Connect(ID_BUTTONDSELECTALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSaveQueryDialog::OnButtonDselectAllClick);
	Connect(ID_BUTTONGOTO,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadSaveQueryDialog::OnButtonGoToClick);
	//*)
}

MadSaveQueryDialog::~MadSaveQueryDialog()
{
	//(*Destroy(MadSaveQueryDialog)
	//*)
}

void MadSaveQueryDialog::InitWindowListIterms()
{
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	long count = (long) notebookp->GetPageCount();
	long tmp;

	MadFileList->Hide();
	MadFileList->DeleteAllItems();

	MadFileList->Freeze();
	for( long id = 0; id < count; ++id )
	{
		MadEdit * madedit = ( MadEdit* )notebookp->GetPage( id );
		if(madedit->IsModified())
		{
			wxFileName fileName( madedit->GetFileName() );
			wxString fname = notebookp->GetPageText( id );
			wxString fdir = fileName.GetPath();
			tmp = MadFileList->InsertItem(id, fname);
			MadFileList->SetItemData(tmp, id);
			MadFileList->SetItem(tmp, 1, fdir);
		}
	}
	MadFileList->Thaw();

	if(count)
	{
	    MadFileList->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		MadFileList->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	}
	MadFileList->Show();
	GetSizer()->Fit( this );
}

void MadSaveQueryDialog::MadSaveQueryDialogActivate( wxActivateEvent& event )
{
	InitWindowListIterms();
	//ResetButtonStatus();
}

void MadSaveQueryDialog::OnButtonOKClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void MadSaveQueryDialog::OnButtonSaveNoneClick(wxCommandEvent& event)
{
}

void MadSaveQueryDialog::OnButtonCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void MadSaveQueryDialog::OnButtonSelectAllClick(wxCommandEvent& event)
{
}

void MadSaveQueryDialog::OnButtonDselectAllClick(wxCommandEvent& event)
{
}

void MadSaveQueryDialog::OnButtonGoToClick(wxCommandEvent& event)
{
}
