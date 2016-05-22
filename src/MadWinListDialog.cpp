//(*InternalHeaders(MadWinListDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/arrstr.h>

#include <wx/filename.h>
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadWinListDialog.h"

//(*IdInit(MadWinListDialog)
const long MadWinListDialog::ID_LISTCTRLMADWINLIST = wxNewId();
const long MadWinListDialog::ID_BUTTONACTIVATE = wxNewId();
const long MadWinListDialog::ID_BUTTONSAVE = wxNewId();
const long MadWinListDialog::ID_BUTTONSAVEAS = wxNewId();
const long MadWinListDialog::ID_BUTTONCLOSEWINDOWS = wxNewId();
const long MadWinListDialog::ID_BUTTONSORTTABBYNAME = wxNewId();
const long MadWinListDialog::ID_BUTTONSORTTABBYPATH = wxNewId();
//*)

const long MadWinListDialog::COL_TABNAME = 0;
const long MadWinListDialog::COL_PATH = 1;

BEGIN_EVENT_TABLE(MadWinListDialog,wxDialog)
	EVT_ACTIVATE( MadWinListDialog::MadWinListDialogActivate )
	//(*EventTable(MadWinListDialog)
	//*)
END_EVENT_TABLE()

MadWinListDialog *g_WinListDialog = NULL;

MadWinListDialog::MadWinListDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MadWinListDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Windows"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	MadWindowsList = new wxListCtrl(this, ID_LISTCTRLMADWINLIST, wxDefaultPosition, wxSize(400,300), wxLC_REPORT|wxSIMPLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_LISTCTRLMADWINLIST"));
	BoxSizer2->Add(MadWindowsList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ButtonActivate = new wxButton(this, ID_BUTTONACTIVATE, _("Activate"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONACTIVATE"));
	BoxSizer3->Add(ButtonActivate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSave = new wxButton(this, ID_BUTTONSAVE, _("Save"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONSAVE"));
	BoxSizer3->Add(ButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSaveAs = new wxButton(this, ID_BUTTONSAVEAS, _("Save As"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONSAVEAS"));
	BoxSizer3->Add(ButtonSaveAs, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCloseWindows = new wxButton(this, ID_BUTTONCLOSEWINDOWS, _("Close Window(s)"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONCLOSEWINDOWS"));
	BoxSizer3->Add(ButtonCloseWindows, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSortTabByName = new wxButton(this, ID_BUTTONSORTTABBYNAME, _("Sort Tab(Name)"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("ID_BUTTONSORTTABBYNAME"));
	BoxSizer3->Add(ButtonSortTabByName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSortTabByPath = new wxButton(this, ID_BUTTONSORTTABBYPATH, _("Sort Tab(Path)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSORTTABBYPATH"));
	ButtonSortTabByPath->SetMinSize(wxSize(110,27));
	BoxSizer3->Add(ButtonSortTabByPath, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonOk = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxSize(110,27), 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer3->Add(ButtonOk, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONACTIVATE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonActivateClick);
	Connect(ID_BUTTONSAVE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonSaveClick);
	Connect(ID_BUTTONSAVEAS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonSaveAsClick);
	Connect(ID_BUTTONCLOSEWINDOWS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonCloseWindowsClick);
	Connect(ID_BUTTONSORTTABBYNAME,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonSortTabByNameClick);
	Connect(ID_BUTTONSORTTABBYPATH,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonSortTabByPathClick);
	Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadWinListDialog::OnButtonOkClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadWinListDialog::OnMadWinListDialogClose);
	//*)

	Connect(ID_LISTCTRLMADWINLIST,wxEVT_LIST_ITEM_SELECTED,(wxObjectEventFunction)&MadWinListDialog::OnWinListSelectionChanged);
	Connect(ID_LISTCTRLMADWINLIST,wxEVT_LIST_ITEM_DESELECTED,(wxObjectEventFunction)&MadWinListDialog::OnWinListSelectionChanged);

	ButtonActivate->Enable(false);
	ButtonCloseWindows->Enable(false);
	ButtonSave->Enable(false);
	ButtonSaveAs->Enable(false);

	SetDefaultItem(ButtonOk);

	m_MainFrame = static_cast<MadEditFrame *>(parent);
	wxListItem itemCol;
	itemCol.SetText(_("Name"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadWindowsList->InsertColumn(0, itemCol);
	itemCol.SetText(_("Path"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadWindowsList->InsertColumn(1, itemCol);
}

MadWinListDialog::~MadWinListDialog()
{
	//(*Destroy(MadWinListDialog)
	//*)
}

void MadWinListDialog::InitWindowListIterms()
{
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	long count = (long) notebookp->GetPageCount();
	long tmp;

	MadWindowsList->Hide();
	MadWindowsList->DeleteAllItems();

	for( long id = 0; id < count; ++id )
	{
		MadEdit * madedit = ( MadEdit* )notebookp->GetPage( id );
		wxFileName fileName( madedit->GetFileName() );
		wxString fname = notebookp->GetPageText( id );
		wxString fdir = fileName.GetPath();
		tmp = MadWindowsList->InsertItem(id, fname);
		MadWindowsList->SetItemData(tmp, id);
		MadWindowsList->SetItem(tmp, 1, fdir);
	}

	if(count)
	{
	    MadWindowsList->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		MadWindowsList->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	}
	MadWindowsList->Show();
	GetSizer()->Fit( this );
}

void MadWinListDialog::MadWinListDialogActivate( wxActivateEvent& event )
{
	InitWindowListIterms();
}

void MadWinListDialog::OnButtonActivateClick(wxCommandEvent& event)
{
	wxASSERT(MadWindowsList->GetSelectedItemCount() == 1);
	long selRowId = MadWindowsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxASSERT ( selRowId != -1 );
	long pageId = static_cast<long>(MadWindowsList->GetItemData(selRowId));
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	if(pageId != notebookp->GetSelection())
		notebookp->SetSelection(pageId);
}

void MadWinListDialog::SaveFile(bool saveas/* = false*/)
{
	long item = -1;
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	Show(false);
	for ( ;; )
	{
		item = MadWindowsList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1 )
			break;

		// this item is selected
		long pageId = static_cast<long>(MadWindowsList->GetItemData(item));
		m_MainFrame->SaveFile(pageId, saveas);
	}
	Show(true);
}
void MadWinListDialog::OnButtonSaveClick(wxCommandEvent& event)
{
	wxASSERT(MadWindowsList->GetSelectedItemCount() > 0);
	SaveFile(false);
}

void MadWinListDialog::OnButtonSaveAsClick(wxCommandEvent& event)
{
	wxASSERT(MadWindowsList->GetSelectedItemCount() > 0);
	SaveFile(true);
}

void MadWinListDialog::OnButtonCloseWindowsClick(wxCommandEvent& event)
{
	wxASSERT(MadWindowsList->GetSelectedItemCount() > 0);
	std::vector<long> items;
	long item = -1;
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	for ( ;; )
	{
		item = MadWindowsList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1 )
			break;

		items.push_back(item);
		// this item is selected
		long pageId = static_cast<long>(MadWindowsList->GetItemData(item));
		m_MainFrame->CloseFile(pageId);
	}

	for(int i = 0; i < items.size(); ++i)
	{
		MadWindowsList->DeleteItem(items[i]);
	}
}

void MadWinListDialog::SortTabs(long column)
{
	wxArrayString colname, tabname;
	wxArrayString * tabref = &colname;
	std::map<wxString, MadEdit *> madEditMap;
	bool need_tab_name = (column != COL_TABNAME);
	wxString name;

	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	std::vector<MadEdit *> oldmedits, medits;

	if(need_tab_name)
	{
		tabref = &tabname;
	}

	long item = -1, seq = 0;
	for (; ;)
	{
		++seq;
		item = MadWindowsList->GetNextItem(item);
		if ( item == -1 )
			break;

		name = MadWindowsList->GetItemText(item, column);
		
		long pageId = static_cast<long>(MadWindowsList->GetItemData(item));
		MadEdit * madedit = ( MadEdit* )notebookp->GetPage( pageId );
		if(name.IsEmpty())
			name.Printf(wxT("*%4d"), seq);
		colname.Add(name);
		madEditMap[name] = madedit;
		oldmedits.push_back(madedit);
		if(need_tab_name)
		{
			name = MadWindowsList->GetItemText(item);
			tabname.Add(name);
		}
	}

	size_t count = colname.GetCount();
	wxASSERT(notebookp->GetPageCount() == count);
	medits.reserve(count);
	colname.Sort();
	std::map<wxString, MadEdit *>::iterator it;
	for(size_t i = 0; i < count; ++i)
	{
		it = madEditMap.find(colname[i]);
		wxASSERT(it != madEditMap.end());
		medits.push_back(it->second);
	}

	if(oldmedits != medits)
	{
		while(notebookp->GetPageCount())
		{
			notebookp->RemovePage( 0 );
		}

		for( long id = 0; id < count; ++id )
		{
			notebookp->AddPage( medits[id], (*tabref)[id], false);
		}
		notebookp->SetSelection(0);
		InitWindowListIterms();
	}
}

void MadWinListDialog::OnButtonSortTabByNameClick(wxCommandEvent& event)
{
	SortTabs(COL_TABNAME);
}

void MadWinListDialog::OnButtonSortTabByPathClick(wxCommandEvent& event)
{
	SortTabs(COL_PATH);
}

void MadWinListDialog::OnButtonOkClick(wxCommandEvent& event)
{
    Show( false );Show( false );Show( false );
}

void MadWinListDialog::OnMadWinListDialogClose(wxCloseEvent& event)
{
	g_WinListDialog = NULL;
    Destroy();
}

void MadWinListDialog::OnWinListSelectionChanged(wxListEvent& event)
{
	bool onlyone = (MadWindowsList->GetSelectedItemCount() == 1);
	bool selected = (MadWindowsList->GetSelectedItemCount() > 0);
	ButtonActivate->Enable(onlyone);
	ButtonCloseWindows->Enable(selected);
	ButtonSave->Enable(selected);
	ButtonSaveAs->Enable(selected);
}

