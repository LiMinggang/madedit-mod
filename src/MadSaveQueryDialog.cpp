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
//*)

const long MadSaveQueryDialog::COL_TABNAME = 0;
const long MadSaveQueryDialog::COL_PATH = 1;

#define SAVEQUERY_MIN_PATH_COL_WIDTH 80

MadSaveQueryDialog::MadSaveQueryDialog(wxWindow* parent,wxWindowID id,const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
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
	MadFileList = new wxCheckedListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(400,300), wxLC_REPORT | wxSIMPLE_BORDER | wxVSCROLL, wxDefaultValidator, _T("ID_LISTCTRLMADFILELIST"));
	BoxSizer2->Add(MadFileList, 0, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	ButtonOK->SetDefault();
	BoxSizer3->Add(ButtonOK, 0, wxALL|wxEXPAND, 2);
	ButtonSaveNone = new wxButton(this, wxID_ANY, _("Save &None"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSAVENONE"));
	BoxSizer3->Add(ButtonSaveNone, 0, wxALL|wxEXPAND, 2);
	ButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer3->Add(ButtonCancel, 0, wxALL|wxEXPAND, 2);
	CheckBoxConfirm = new wxCheckBox(this, wxID_ANY, _("Confirm"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXCONFIRM"));
	CheckBoxConfirm->SetValue(false);
	BoxSizer3->Add(CheckBoxConfirm, 0, wxALL|wxEXPAND, 2);
	BoxSizer3->Add(-1,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ButtonSelectAll = new wxButton(this, wxID_ANY, _("Select &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSELECTALL"));
	BoxSizer3->Add(ButtonSelectAll, 0, wxALL|wxEXPAND, 2);
	ButtonDselectAll = new wxButton(this, wxID_ANY, _("&Deselect All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDSELECTALL"));
	BoxSizer3->Add(ButtonDselectAll, 0, wxALL|wxEXPAND, 2);
	BoxSizer3->Add(-1,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ButtonGoTo = new wxButton(this, wxID_ANY, _("&Go To"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONGOTO"));
	BoxSizer3->Add(ButtonGoTo, 0, wxALL|wxEXPAND, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED, &MadSaveQueryDialog::OnMadFileListItemSelectChange, this, MadFileList->GetId() );
	Bind( wxEVT_COMMAND_LIST_ITEM_DESELECTED, &MadSaveQueryDialog::OnMadFileListItemSelectChange, this, MadFileList->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadSaveQueryDialog::OnButtonOKClick, this, wxID_OK );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadSaveQueryDialog::OnButtonSaveNoneClick, this, ButtonSaveNone->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadSaveQueryDialog::OnButtonCancelClick, this, wxID_CANCEL );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadSaveQueryDialog::OnButtonSelectAllClick, this, ButtonSelectAll->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadSaveQueryDialog::OnButtonDselectAllClick, this, ButtonDselectAll->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadSaveQueryDialog::OnButtonGoToClick, this, ButtonGoTo->GetId() );
	//*)

	Bind( wxEVT_LIST_COL_CLICK, &MadSaveQueryDialog::OnWinListColumnTabClicked, this, MadFileList->GetId() );

	SetDefaultItem(ButtonOK);
	m_MainFrame = static_cast<MadEditFrame *>(parent);
	wxListItem itemCol;
	itemCol.SetText(_("Name"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadFileList->InsertColumn(COL_TABNAME, itemCol);
	itemCol.SetText(_("Path"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	MadFileList->InsertColumn(COL_PATH, itemCol);
	InitWindowListIterms();
}

MadSaveQueryDialog::~MadSaveQueryDialog()
{
	//(*Destroy(MadSaveQueryDialog)
	//*)
}

void MadSaveQueryDialog::InitWindowListIterms()
{
	wxAuiNotebook * notebookp = reinterpret_cast<wxAuiNotebook *>(m_MainFrame->m_Notebook);
	size_t count = notebookp->GetPageCount();
	long tmp;
	bool hasdata = false;

	MadFileList->Hide();
	MadFileList->DeleteAllItems();

	MadFileList->Freeze();
	
	wxListItem info;
	for( size_t id = 0; id < count; ++id )
	{
		MadEdit * madedit = dynamic_cast < MadEdit* >(notebookp->GetPage( id ));
		wxASSERT( madedit != 0 );
		if(madedit->IsModified())
		{
			wxFileName fileName( madedit->GetFileName() );
			wxString fname = notebookp->GetPageText( id );
			wxString fdir = fileName.GetPath();

			info.Clear();
			info.m_text = fname;
			info.m_mask = wxLIST_MASK_TEXT;
			info.m_itemId = MadFileList->GetItemCount();
			tmp = MadFileList->InsertItem(info);
			MadFileList->SetItemData(tmp, id);
			MadFileList->SetItem(tmp, COL_PATH, fdir);
			hasdata = true;
		}
	}
	MadFileList->Thaw();

	if(hasdata)
	{
	    MadFileList->SetColumnWidth( COL_TABNAME, wxLIST_AUTOSIZE );
		MadFileList->SetColumnWidth( COL_PATH, wxLIST_AUTOSIZE );
	}

	if(SAVEQUERY_MIN_PATH_COL_WIDTH > MadFileList->GetColumnWidth(COL_PATH))
	{
		MadFileList->SetColumnWidth( COL_PATH, SAVEQUERY_MIN_PATH_COL_WIDTH );
	}

	MadFileList->Show();
	GetSizer()->Fit( this );
}

void MadSaveQueryDialog::SortTabs(long column)
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
		item = MadFileList->GetNextItem(item);
		if ( item == -1 )
			break;

		name = MadFileList->GetItemText(item, column);
		if(column == COL_PATH)
		{
			tname = MadFileList->GetItemText(item);
			name += tname;
		}
		
		long pageId = static_cast<long>(MadFileList->GetItemData(item));
		MadEdit * madedit = dynamic_cast < MadEdit* >(notebookp->GetPage( pageId ));
		if(name.IsEmpty())
			name.Printf(wxT("*%04d"), seq);
		else if(madEditMap.find(name) != madEditMap.end())
		{
			wxString postfix;
			postfix.Printf(wxT("*%04d"), seq);
			name += postfix;
		}

#ifdef __WXMSW__
		name.MakeUpper();
#endif
		colname.Add(name);
		madEditMap[name] = madedit;
		oldmedits.push_back(madedit);
		if(selid == pageId)
			selmedit = madedit;
		tname = MadFileList->GetItemText(item);
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

void MadSaveQueryDialog::OnWinListColumnTabClicked(wxListEvent& event)
{
    int col = event.GetColumn();
	SortTabs(col);
}

void MadSaveQueryDialog::OnButtonOKClick(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_OK);
}

void MadSaveQueryDialog::OnButtonSaveNoneClick(wxCommandEvent& WXUNUSED(event))
{
	MadFileList->CheckAll(false);
	EndModal(wxID_OK);
}

void MadSaveQueryDialog::OnButtonCancelClick(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}

void MadSaveQueryDialog::OnButtonSelectAllClick(wxCommandEvent& WXUNUSED(event))
{
	MadFileList->Freeze();
	MadFileList->CheckAll(true);
	MadFileList->Thaw();
}

void MadSaveQueryDialog::OnButtonDselectAllClick(wxCommandEvent& WXUNUSED(event))
{
	MadFileList->Freeze();
	MadFileList->CheckAll(false);
	MadFileList->Thaw();
}

void MadSaveQueryDialog::OnButtonGoToClick(wxCommandEvent& WXUNUSED(event))
{
	wxASSERT(MadFileList->GetSelectedItemCount() == 1);
	long selRowId = MadFileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxASSERT ( selRowId != -1 );
	long pageId = static_cast<long>(MadFileList->GetItemData(selRowId));
	m_MainFrame->SetPageFocus( pageId );
}

void MadSaveQueryDialog::OnMadFileListItemSelectChange(wxListEvent& WXUNUSED(event))
{
	bool onlyone = (MadFileList->GetSelectedItemCount() == 1);
	ButtonGoTo->Enable(onlyone);
}

void MadSaveQueryDialog::GetCheckedItemsData(std::set< long > & selectedItems, bool checked)
{
	long item = -1, pid = -1;
	for ( ;; ) {
		item = MadFileList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
		if ( item == -1 )
			break;
		if((!checked) || (MadFileList->IsChecked(item)))
		{
			pid = static_cast<long>(MadFileList->GetItemData(item));
			selectedItems.insert(pid);
		}
	}
}

