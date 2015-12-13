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
const long MadAboutDialog::ID_STATICBITMAP1 = wxNewId();
const long MadAboutDialog::ID_STATICBITMAP2 = wxNewId();
const long MadAboutDialog::ID_WXMEMOABOUT = wxNewId();
const long MadAboutDialog::ID_WXPANELABOUT = wxNewId();
const long MadAboutDialog::ID_WXMEMOCREDITS = wxNewId();
const long MadAboutDialog::ID_WXPANELCREDITS = wxNewId();
const long MadAboutDialog::ID_TEXTCTRL1 = wxNewId();
const long MadAboutDialog::ID_WXMEMOLICENSE = wxNewId();
const long MadAboutDialog::ID_WXAUINOTEBOOKABOUT = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadAboutDialog,wxDialog)
	//(*EventTable(MadAboutDialog)
	//*)
END_EVENT_TABLE()

wxString g_MadEdit_URL(wxT("http://sourceforge.net/projects/madedit/ or http://sourceforge.net/projects/madedit-mod/"));
wxString g_MadEditMod_URL(wxT("https://sourceforge.net/p/madedit-mod/wiki/MadEdi-Mod/"));
wxString g_MadEditModLicense (
_("Copyright (C) 2012-2015  Minggang Li <minggang.li@gmail.com>\n\n\
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.\n\n\
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n\
You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n")
);
wxString g_MadEditModCredits(
_("Minggang Li\t\tCurrent maintainer of MadEdit-Mod\n\
===========================================\n\
Alston Chen\t\tAuthor of MadEdit\n\
JiaYanwei\t\tPatch contributer for MadEdit\n\
nikoss\t\t\tGreek Translation\n\
Others\t\t\tTranslation/patch contributers")
);

MadAboutDialog::MadAboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MadAboutDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("About MadEdit-Mod"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxStaticBitmap1 = new wxStaticBitmap(this, ID_STATICBITMAP1, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("ID_STATICBITMAP1"));
	BoxSizer3->Add(WxStaticBitmap1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticBitmap2 = new wxStaticBitmap(this, ID_STATICBITMAP2, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("ID_STATICBITMAP2"));
	BoxSizer3->Add(WxStaticBitmap2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	WxAuiNotebookAbout = new wxAuiNotebook(this, ID_WXAUINOTEBOOKABOUT, wxDefaultPosition, wxSize(440,220), wxAUI_NB_TOP|wxNO_BORDER|wxTAB_TRAVERSAL);
	WxPanelAbout = new wxPanel(WxAuiNotebookAbout, ID_WXPANELABOUT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_WXPANELABOUT"));
	WxMemoAbout = new wxTextCtrl(WxPanelAbout, ID_WXMEMOABOUT, wxEmptyString, wxDefaultPosition, wxSize(435,200), wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxDOUBLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_WXMEMOABOUT"));
	WxPanelCredits = new wxPanel(WxAuiNotebookAbout, ID_WXPANELCREDITS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_WXPANELCREDITS"));
	WxMemoCredits = new wxTextCtrl(WxPanelCredits, ID_WXMEMOCREDITS, wxEmptyString, wxDefaultPosition, wxSize(435,200), wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxDOUBLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_WXMEMOCREDITS"));
	WxPanelLicense = new wxPanel(WxAuiNotebookAbout, ID_WXMEMOLICENSE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_WXMEMOLICENSE"));
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, WxPanelLicense, _("GNU General Public License"));
	WxMemoLicense = new wxTextCtrl(WxPanelLicense, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(435,200), wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxVSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL1"));
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

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadAboutDialog::MadAboutDialogClose);
	//*)
	WxStaticBitmap1->SetBitmap(wxBitmap(Mad_xpm));
	WxStaticBitmap1->Enable(true);
	WxStaticBitmap2->SetBitmap(wxBitmap(Mad2_xpm));
	WxStaticBitmap2->Enable(true);

	WxAuiNotebookAbout->SetSelection(0);
	SetDefaultItem(WxButtonCancel);
}

MadAboutDialog::~MadAboutDialog()
{
	//(*Destroy(MadAboutDialog)
	//*)
}

void MadAboutDialog::MadAboutDialogClose(wxCloseEvent& event)
{
    Destroy();
}
