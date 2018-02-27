///////////////////////////////////////////////////////////////////////////////
// Name:        MadAboutDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadAboutDialog.h"

//(*InternalHeaders(MadAboutDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "../images/Mad.xpm"
#include "../images/Mad2.xpm"

//(*IdInit(MadAboutDialog)
//*)

wxString g_MadEdit_URL(wxT("https://sourceforge.net/projects/madedit/ or https://sourceforge.net/projects/madedit-mod/"));
wxString g_MadEditMod_URL(wxT("https://sourceforge.net/p/madedit-mod/wiki/MadEdi-Mod/"));
wxString g_MadEditModLicense (
_("Copyright (C) 2012-2018  Minggang Li <minggang.li@gmail.com>\n\n\
    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by\
 the Free Software Foundation, either version 3 of the License, or\
 (at your option) any later version.\n\n\
    This program is distributed in the hope that it will be useful,\
 but WITHOUT ANY WARRANTY; without even the implied warranty of\
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\n\
    You should have received a copy of the GNU General Public License\
 along with this program.  If not, see <http://www.gnu.org/licenses/>.\n")
);

const int g_num_contributers = 5;

const wxChar   * g_MadEditModCredits[g_num_contributers][2] = {
	wxT("Minggang Li"), _("Current maintainer"),
	wxT("Alston Chen"), _("Author of MadEdit"),
	wxT("nikoss"), _("Greek Translation"),
	wxT("zhtw2013"), _("Traditional Chinese Translation"),
	wxT("Others"), _("Translation/patch contributers"),
};

MadAboutDialog::MadAboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
{
	//(*Initialize(MadAboutDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, id, _("About MadEdit-Mod"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxStaticBitmap1 = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("wxID_ANY"));
	BoxSizer3->Add(WxStaticBitmap1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticBitmap2 = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("wxID_ANY"));
	BoxSizer3->Add(WxStaticBitmap2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	WxAuiNotebookAbout = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(440,220), wxAUI_NB_TOP|wxNO_BORDER|wxTAB_TRAVERSAL);
	WxPanelAbout = new wxPanel(WxAuiNotebookAbout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	WxMemoAbout = new wxTextCtrl(WxPanelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(435,200), wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxDOUBLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("wxID_ANY"));
	WxPanelCredits = new wxPanel(WxAuiNotebookAbout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	WxListCredits = new wxListCtrl(WxPanelCredits, wxID_ANY, wxPoint(0,0), wxSize(400,300), wxLC_REPORT|wxNO_BORDER, wxDefaultValidator, _T("wxID_ANY"));
	WxPanelLicense = new wxPanel(WxAuiNotebookAbout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, WxPanelLicense, _("GNU General Public License"));
	WxMemoLicense = new wxTextCtrl(WxPanelLicense, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(435,200), wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxVSCROLL, wxDefaultValidator, _T("wxID_ANY"));
	StaticBoxSizer1->Add(WxMemoLicense, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxPanelLicense->SetSizer(StaticBoxSizer1);
	StaticBoxSizer1->Fit(WxPanelLicense);
	StaticBoxSizer1->SetSizeHints(WxPanelLicense);
	WxAuiNotebookAbout->AddPage(WxPanelAbout, _("About"), true);
	WxAuiNotebookAbout->AddPage(WxPanelCredits, _("Credits"));
	WxAuiNotebookAbout->AddPage(WxPanelLicense, _("License"));
	BoxSizer2->Add(WxAuiNotebookAbout, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	WxButtonOK->SetDefault();
	BoxSizer4->Add(WxButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer4->Add(WxButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Bind(wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadAboutDialog::MadAboutDialogClose, this, wxID_ANY);
	//*)
	WxStaticBitmap1->SetBitmap(wxBitmap(Mad_xpm));
	WxStaticBitmap1->Enable(true);
	WxStaticBitmap2->SetBitmap(wxBitmap(Mad2_xpm));
	WxStaticBitmap2->Enable(true);

	WxAuiNotebookAbout->SetSelection(0);
	SetDefaultItem(WxButtonCancel);

	wxListItem itemCol;
	itemCol.SetText(_("Name"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	WxListCredits->InsertColumn(0, itemCol);
	itemCol.SetText(_("Role"));
	itemCol.SetAlign(wxLIST_FORMAT_LEFT);
	WxListCredits->InsertColumn(1, itemCol);

	WxListCredits->Freeze();
	wxListItem info;
	long tmp;
	for( size_t idx = 0; idx < g_num_contributers; ++idx )
	{
		wxString name(g_MadEditModCredits[ idx ][0]);
		wxString role(wxGetTranslation(g_MadEditModCredits[ idx ][1]));
		
		info.Clear();
		info.m_text = name;
		info.m_mask = wxLIST_MASK_TEXT;
		info.m_itemId = WxListCredits->GetItemCount();
		tmp = WxListCredits->InsertItem(info);
		WxListCredits->SetItem(tmp, 1, role);
	}
	WxListCredits->Thaw();

	if(g_num_contributers)
	{
	    WxListCredits->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		WxListCredits->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	}
}

MadAboutDialog::~MadAboutDialog()
{
	//(*Destroy(MadAboutDialog)
	//*)
}

void MadAboutDialog::MadAboutDialogClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}
