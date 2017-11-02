#include "MadFileHistoryDialog.h"

//(*InternalHeaders(MadFileHistoryDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/arrstr.h>
#include <algorithm>  

#include <wx/filename.h>
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadRecentList.h"

//(*IdInit(MadFileHistoryDialog)
//*)

#define FILEHISTORY_MIN_PATH_COL_WIDTH 80

MadFileHistoryDialog *g_RecentOpenedFileListDialog = nullptr;

MadFileHistoryDialog::MadFileHistoryDialog(wxWindow* parent,wxWindowID id, const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size))
{
	//(*Initialize(MadFileHistoryDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Recent Opened Files"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxCAPTION|wxCLOSE_BOX|wxSIMPLE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	MadFileList = new wxCheckedListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(480, 300), wxLC_REPORT | wxSIMPLE_BORDER | wxVSCROLL, wxDefaultValidator, _T("ID_CHECKLISTMADHISFILELIST"));
	BoxSizer2->Add(MadFileList, 0, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ButtonOK = new wxButton(this, wxID_OK, _("&Open"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	ButtonOK->SetDefault();
	BoxSizer3->Add(ButtonOK, 0, wxALL|wxEXPAND, 2);
	ButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer3->Add(ButtonCancel, 0, wxALL|wxEXPAND, 2);
	ButtonSelectAll = new wxButton(this, wxID_ANY, _("Select &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSELECTALL"));
	BoxSizer3->Add(ButtonSelectAll, 0, wxALL|wxEXPAND, 2);
	ButtonDselectAll = new wxButton(this, wxID_ANY, _("&Deselect All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDSELECTALL"));
	BoxSizer3->Add(ButtonDselectAll, 0, wxALL|wxEXPAND, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadFileHistoryDialog::OnButtonOKClick, this, wxID_OK);
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadFileHistoryDialog::OnButtonCancelClick, this, wxID_CANCEL);
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadFileHistoryDialog::OnButtonSelectAllClick, this, ButtonSelectAll->GetId());
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadFileHistoryDialog::OnButtonDselectAllClick, this, ButtonDselectAll->GetId());
	Bind( wxEVT_CLOSE_WINDOW, &MadFileHistoryDialog::MadFileHistoryDialogClose, this, wxID_ANY);
	//*)

	SetDefaultItem(ButtonOK);
	m_MainFrame = static_cast<MadEditFrame *>(parent);
	wxASSERT(m_MainFrame && m_MainFrame->m_RecentFiles);	
	m_RecentFiles = m_MainFrame->m_RecentFiles;
	wxListItem itemCol;
	itemCol.SetText(_("Name"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadFileList->InsertColumn(0, itemCol);
	itemCol.SetText(_("Path"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadFileList->InsertColumn(1, itemCol);
	ResetWindowListIterms();
}

MadFileHistoryDialog::~MadFileHistoryDialog()
{
	//(*Destroy(MadFileHistoryDialog)
	//*)
}

void MadFileHistoryDialog::ResetWindowListIterms()
{
	size_t count = m_RecentFiles->GetCount();
	long tmp;
	wxString fname;
	wxString fdir;

	MadFileList->Hide();
	MadFileList->DeleteAllItems();
	if( count )
	{
		MadFileList->Freeze();
		wxListItem info;
		for( size_t id = 0; id < count; ++id )
		{
			wxFileName fileName( m_RecentFiles->GetHistoryFile( id ) );
			if(fileName.Exists())
			{
				fname = fileName.GetFullName();
				fdir = fileName.GetPath();
				info.Clear();
				info.m_text = fname;
				info.m_mask = wxLIST_MASK_TEXT;
				info.m_itemId = MadFileList->GetItemCount();
				tmp = MadFileList->InsertItem(info);
				//MadFileList->SetItemData(tmp, id);
				MadFileList->SetItem(tmp, 1, fdir);
			}
		}
		MadFileList->Thaw();
	    MadFileList->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		MadFileList->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	}

	if(FILEHISTORY_MIN_PATH_COL_WIDTH > MadFileList->GetColumnWidth(1))
	{
		MadFileList->SetColumnWidth( 1, FILEHISTORY_MIN_PATH_COL_WIDTH );
	}

	MadFileList->Show();
	GetSizer()->Fit( this );
}

void MadFileHistoryDialog::OnButtonOKClick(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_OK);
}

void MadFileHistoryDialog::OnButtonCancelClick(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}

void MadFileHistoryDialog::OnButtonSelectAllClick(wxCommandEvent& WXUNUSED(event))
{
	MadFileList->Freeze();
	MadFileList->CheckAll(true);
	MadFileList->Thaw();
}

void MadFileHistoryDialog::OnButtonDselectAllClick(wxCommandEvent& WXUNUSED(event))
{
	MadFileList->Freeze();
	MadFileList->CheckAll(false);
	MadFileList->Thaw();
}

void MadFileHistoryDialog::GetCheckedItemsData(wxArrayString & selectedItems, bool checked)
{
	long item = -1;
	wxString fname;
	wxString fdir;
	for ( ;; ) {
		item = MadFileList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
		if ( item == -1 )
			break;
		if((!checked) || (MadFileList->IsChecked(item)))
		{
			fname = MadFileList->GetItemText(item, 0);
			fdir = MadFileList->GetItemText(item, 1);
			selectedItems.Add(fdir + wxFILE_SEP_PATH + fname);
		}
	}
}

void MadFileHistoryDialog::MadFileHistoryDialogClose(wxCloseEvent& WXUNUSED(event))
{
	g_RecentOpenedFileListDialog = nullptr;
	Destroy();
}
