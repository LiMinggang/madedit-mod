//(*InternalHeaders(MadWinListDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/arrstr.h>
#include <algorithm>  

#include <wx/filename.h>
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadWinListDialog.h"

//(*IdInit(MadWinListDialog)
//*)

const long MadWinListDialog::COL_TABNAME = 0;
const long MadWinListDialog::COL_PATH = 1;
#define WINLIST_MIN_PATH_COL_WIDTH 80

MadWinListDialog *g_WinListDialog = nullptr;

MadWinListDialog::MadWinListDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MadWinListDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Windows"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	MadWindowsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(400,300), wxLC_REPORT|wxSIMPLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_LISTCTRLMADWINLIST"));
	BoxSizer2->Add(MadWindowsList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ButtonActivate = new wxButton(this, wxID_ANY, _("Acti&vate"), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("ID_BUTTONACTIVATE"));
	BoxSizer3->Add(ButtonActivate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSave = new wxButton(this, wxID_ANY, _("&Save"), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("ID_BUTTONSAVE"));
	BoxSizer3->Add(ButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSaveAs = new wxButton(this, wxID_ANY, _("Save &As..."), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("ID_BUTTONSAVEAS"));
	BoxSizer3->Add(ButtonSaveAs, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCloseWindows = new wxButton(this, wxID_ANY, _("&Close Window(s)"), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("ID_BUTTONCLOSEWINDOWS"));
	BoxSizer3->Add(ButtonCloseWindows, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSortTabByName = new wxButton(this, wxID_ANY, _("Sort Tab(&Name)"), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("ID_BUTTONSORTTABBYNAME"));
	BoxSizer3->Add(ButtonSortTabByName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSortTabByPath = new wxButton(this, wxID_ANY, _("Sort Tab(&Path)"), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("ID_BUTTONSORTTABBYPATH"));
	ButtonSortTabByPath->SetMinSize(wxSize(110,-1));
	BoxSizer3->Add(ButtonSortTabByPath, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonOk = new wxButton(this, wxID_OK, _("&Open"), wxDefaultPosition, wxSize(110,-1), 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer3->Add(ButtonOk, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonActivateClick, this, ButtonActivate->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonSaveClick, this, ButtonSave->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonSaveAsClick, this, ButtonSaveAs->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonCloseWindowsClick, this, ButtonCloseWindows->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonSortTabByNameClick, this, ButtonSortTabByName->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonSortTabByPathClick, this, ButtonSortTabByPath->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadWinListDialog::OnButtonOkClick, this, wxID_OK );
	Bind( wxEVT_CLOSE_WINDOW, &MadWinListDialog::OnMadWinListDialogClose, this, wxID_ANY );
	Bind( wxEVT_KEY_DOWN, &MadWinListDialog::OnKeyDown, this, wxID_ANY );
	//*)

	MadWindowsList->Bind( wxEVT_KEY_DOWN, &MadWinListDialog::OnKeyDown, this );
	Bind( wxEVT_LIST_ITEM_SELECTED, &MadWinListDialog::OnWinListSelectionChanged, this, MadWindowsList->GetId() );
	Bind( wxEVT_LIST_ITEM_DESELECTED, &MadWinListDialog::OnWinListSelectionChanged, this, MadWindowsList->GetId() );

	ResetButtonStatus();

	SetDefaultItem(ButtonOk);

	m_MainFrame = static_cast<MadEditFrame *>(parent);
	wxListItem itemCol;
	itemCol.SetText(_("Name"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadWindowsList->InsertColumn(0, itemCol);
	itemCol.SetText(_("Path"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadWindowsList->InsertColumn(1, itemCol);
	Bind( wxEVT_ACTIVATE, &MadWinListDialog::MadWinListDialogActivate, this );
}

MadWinListDialog::~MadWinListDialog()
{
	//(*Destroy(MadWinListDialog)
	//*)
}

void MadWinListDialog::InitWindowListIterms()
{
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	size_t count = (long) notebookp->GetPageCount();
	long tmp;

	MadWindowsList->Hide();
	MadWindowsList->DeleteAllItems();

	MadWindowsList->Freeze();
	wxListItem info;
	for( size_t id = 0; id < count; ++id )
	{
		MadEdit * madedit = ( MadEdit* )notebookp->GetPage( id );
		wxFileName fileName( madedit->GetFileName() );
		wxString fname = notebookp->GetPageText( id );
		wxString fdir = fileName.GetPath();
		
		info.Clear();
		info.m_text = fname;
		info.m_mask = wxLIST_MASK_TEXT;
		info.m_itemId = MadWindowsList->GetItemCount();
		tmp = MadWindowsList->InsertItem(info);
		MadWindowsList->SetItemData(tmp, info.m_itemId);
		MadWindowsList->SetItem(tmp, 1, fdir);
	}
	MadWindowsList->Thaw();

	if(count)
	{
	    MadWindowsList->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		MadWindowsList->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	}

	if(WINLIST_MIN_PATH_COL_WIDTH > MadWindowsList->GetColumnWidth(1))
	{
		MadWindowsList->SetColumnWidth( 1, WINLIST_MIN_PATH_COL_WIDTH );
	}

	MadWindowsList->Show();
	GetSizer()->Fit( this );
}

void MadWinListDialog::MadWinListDialogActivate( wxActivateEvent& event )
{
	InitWindowListIterms();
	ResetButtonStatus();
}

void MadWinListDialog::OnButtonActivateClick(wxCommandEvent& event)
{
	wxASSERT(MadWindowsList->GetSelectedItemCount() == 1);
	long selRowId = MadWindowsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxASSERT ( selRowId != -1 );
	long pageId = static_cast<long>(MadWindowsList->GetItemData(selRowId));
	m_MainFrame->SetPageFocus( pageId );
}

void MadWinListDialog::SaveFile(bool saveas/* = false*/)
{
	long item = -1;
	std::vector<long> items;
	for ( ;; )
	{
		item = MadWindowsList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1 )
			break;

		// this item is selected
		items.push_back(item);
	}
	
	for(size_t i = 0; i < items.size(); ++i)
	{
		long pageId = static_cast<long>(MadWindowsList->GetItemData(items[i]));
		m_MainFrame->SaveFile(pageId, saveas);
	}
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
	std::vector<long> items, pages; 
	long item = -1;
	for ( ;; )
	{
		item = MadWindowsList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1 )
			break;

		items.push_back(item);
		// this item is selected
		long pageId = static_cast<long>(MadWindowsList->GetItemData(item));
		pages.push_back(pageId);
	}

	std::sort(items.begin(), items.end(), std::greater<long>());
	std::sort(pages.begin(), pages.end(), std::greater<long>());

	for(size_t i = 0; i < items.size(); ++i)
	{
		MadWindowsList->DeleteItem(items[i]);
		m_MainFrame->CloseFile(pages[i]);
	}
	InitWindowListIterms();
}

void MadWinListDialog::SortTabs(long column)
{
	wxArrayString colname;
	std::map<wxString, MadEdit *> madEditMap;
	std::map<wxString, MadEdit *>::iterator it;
	std::map<wxString, wxString> nameMap;
	wxString name;

	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	std::vector<MadEdit *> oldmedits, medits;

	long item = -1, seq = 0;
	int selid = notebookp->GetSelection();
	MadEdit * selmedit = 0;
	wxString tname;
	for (; ;)
	{
		++seq;
		item = MadWindowsList->GetNextItem(item);
		if ( item == -1 )
			break;

		name = MadWindowsList->GetItemText(item, column);
		if(column == COL_PATH)
		{
			tname = MadWindowsList->GetItemText(item);
			name += tname;
		}
		
		long pageId = static_cast<long>(MadWindowsList->GetItemData(item));
		MadEdit * madedit = ( MadEdit* )notebookp->GetPage( pageId );
		if(name.IsEmpty())
			name.Printf(wxT("*%04d"), seq);
		else if(madEditMap.find(name) != madEditMap.end())
		{
			wxString postfix;
			postfix.Printf(wxT("*%04d"), seq);
			name += postfix;
		}
		colname.Add(name);
		madEditMap[name] = madedit;
		oldmedits.push_back(madedit);
		if(selid == pageId)
			selmedit = madedit;
		tname = MadWindowsList->GetItemText(item);
		nameMap[name] = tname;
	}

	size_t count = colname.GetCount();
	wxASSERT(notebookp->GetPageCount() == count);
	medits.reserve(count);
	colname.Sort();
	for(size_t i = 0; i < count; ++i)
	{
		it = madEditMap.find(colname[i]);
		wxASSERT(it != madEditMap.end());
		medits.push_back(it->second);
		if(selmedit == it->second) selid = i;
	}

	if(oldmedits != medits)
	{
		m_MainFrame->WxMenuBar1->Freeze();
		while(notebookp->GetPageCount())
		{
			notebookp->RemovePage( 0 );
		}

		for( size_t id = 0; id < count; ++id )
		{
			tname = nameMap[colname[id]];
			notebookp->AddPage( medits[id], tname, false);
			
			wxString tmpname = (medits[id])->GetFileName();
			if(tmpname.IsEmpty()) tmpname = tname;
			notebookp->SetPageToolTip (id, tmpname);
		}
		m_MainFrame->WxMenuBar1->Thaw();
		m_MainFrame->SetPageFocus( selid );
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
    Show( false );
}

void MadWinListDialog::OnMadWinListDialogClose(wxCloseEvent& event)
{
	g_WinListDialog = nullptr;
    Destroy();
}

void MadWinListDialog::OnWinListSelectionChanged(wxListEvent& event)
{
	bool onlyone = (MadWindowsList->GetSelectedItemCount() == 1);
	bool selected = (MadWindowsList->GetSelectedItemCount() > 0);
	if(ButtonActivate->IsEnabled() != onlyone)
		ButtonActivate->Enable(onlyone);
	
	if(ButtonCloseWindows->IsEnabled() != selected)
		ButtonCloseWindows->Enable(selected);
	if(ButtonSaveAs->IsEnabled() != selected)
		ButtonSaveAs->Enable(selected);
	if(selected)
	{
		wxString fname = MadWindowsList->GetItemText(event.GetIndex());
		selected = (fname[fname.Len() - 1] == wxT( '*' ));
	}

	if(ButtonSave->IsEnabled() != selected)
		ButtonSave->Enable(selected);
}

void MadWinListDialog::ResetButtonStatus()
{
	ButtonActivate->Enable(false);
	ButtonCloseWindows->Enable(false);
	ButtonSaveAs->Enable(false);
	ButtonSave->Enable(false);
}

void MadWinListDialog::OnKeyDown(wxKeyEvent& event)
{
	// insert your code here
	int key=event.GetKeyCode();

	switch(key)
	{
	case WXK_ESCAPE:
		{
			wxCommandEvent e;
			OnButtonOkClick(e);
			return;
		}
	default:
		break;
	}

	int flags=wxACCEL_NORMAL;
	if(event.m_controlDown) flags|=wxACCEL_CTRL;

	if('A' == key && wxACCEL_CTRL == flags)
	{
		if(MadWindowsList->GetSelectedItemCount() > 0)
		{
			long item = -1;
			MadWindowsList->Freeze();
			for ( ;; )
			{
				item = MadWindowsList->GetNextItem(item);
				if ( item == -1 )
					break;
				MadWindowsList->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			}
			MadWindowsList->Thaw();
		}
		return; // no skip
	}
	
	event.Skip();
}
