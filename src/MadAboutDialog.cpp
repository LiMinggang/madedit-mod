///////////////////////////////////////////////////////////////////////////////
// Name:        MadAboutDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadAboutDialog.h"

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

#include "../images/Mad.xpm"
#include "../images/Mad2.xpm"

//----------------------------------------------------------------------------
// MadAboutDialog
//----------------------------------------------------------------------------
	 //Add Custom Events only in the appropriate Block.
	// Code added in  other places will be removed by wx-dvcpp 
	////Event Table Start
BEGIN_EVENT_TABLE(MadAboutDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadAboutDialog::MadAboutDialogClose)
END_EVENT_TABLE()
	////Event Table End

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

MadAboutDialog::MadAboutDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
	: wxDialog( parent, id, title, position, size, style)
{
	CreateGUIControls();
}

MadAboutDialog::~MadAboutDialog() {} 

void MadAboutDialog::CreateGUIControls(void)
{
	//Do not add custom Code here
	//wx-devcpp designer will remove them.
	//Add the custom code before or after the Blocks
	////GUI Items Creation Start

	//wxInitAllImageHandlers();   //Initialize graphic format handlers

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer4, 0, wxALIGN_CENTER | wxALL, 5);

	WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(5, 5), wxSize(48, 48) );
	WxBoxSizer4->Add(WxStaticBitmap1, 0, wxALIGN_CENTER | wxALL, 5);

	WxStaticBitmap2 = new wxStaticBitmap(this, ID_WXSTATICBITMAP2, wxNullBitmap, wxPoint(5, 63), wxSize(48, 48) );
	WxBoxSizer4->Add(WxStaticBitmap2, 0, wxALIGN_CENTER | wxALL, 5);

	WxBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer2->Add(WxBoxSizer5, 0, wxALIGN_CENTER | wxALL, 5);

	WxAuiNotebookAbout = new wxAuiNotebook(this, ID_WXAUINOTEBOOKABOUT, wxPoint(5, 5), wxSize(450, 220), wxNB_DEFAULT);
	WxBoxSizer5->Add(WxAuiNotebookAbout, 0, wxALIGN_CENTER | wxALL, 5);

	WxPanelAbout = new wxPanel(WxAuiNotebookAbout, ID_WXPANELABOUT, wxPoint(-1, -1), wxSize(440, 210));
	WxAuiNotebookAbout->AddPage(WxPanelAbout, _("About"));

	WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxPanelAbout->SetSizer(WxBoxSizer6);
	WxPanelAbout->SetAutoLayout(true);

	WxMemoAbout = new wxTextCtrl(WxPanelAbout, ID_WXMEMOABOUT, wxEmptyString, wxPoint(5, 5), wxSize(435, 200), wxTE_READONLY | wxTE_AUTO_URL | wxTE_MULTILINE, wxDefaultValidator, wxT("WxMemoAbout"));
	WxMemoAbout->AppendText(wxT(""));
	WxMemoAbout->SetFocus();
	WxMemoAbout->SetInsertionPointEnd();
	WxBoxSizer6->Add(WxMemoAbout, 0, wxALIGN_CENTER | wxALL, 5);

	WxPanelCredits = new wxPanel(WxAuiNotebookAbout, ID_WXPANELCREDITS, wxPoint(0, -1), wxSize(440, 210));
	WxAuiNotebookAbout->AddPage(WxPanelCredits, _("Credits"));

	WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxPanelCredits->SetSizer(WxBoxSizer7);
	WxPanelCredits->SetAutoLayout(true);

	WxMemoCredits = new wxTextCtrl(WxPanelCredits, ID_WXMEMOCREDITS, wxEmptyString, wxPoint(5, 5), wxSize(425, 200), wxTE_READONLY | wxTE_AUTO_URL | wxTE_MULTILINE, wxDefaultValidator, wxT("WxMemoCredits"));
	WxMemoCredits->AppendText(wxT(""));
	WxMemoCredits->SetFocus();
	WxMemoCredits->SetInsertionPointEnd();
	WxBoxSizer7->Add(WxMemoCredits, 0, wxALIGN_CENTER | wxALL, 5);

	WxPanelLicense = new wxPanel(WxAuiNotebookAbout, ID_WXPANELLICENSE, wxPoint(-1, 2), wxSize(440, 210));
	WxAuiNotebookAbout->AddPage(WxPanelLicense, _("License"));

	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox(WxPanelLicense, wxID_ANY, _("GNU General Public License"));
	WxStaticBoxSizer1 = new wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxHORIZONTAL);
	WxPanelLicense->SetSizer(WxStaticBoxSizer1);
	WxPanelLicense->SetAutoLayout(true);

	WxMemoLicense = new wxTextCtrl(WxPanelLicense, ID_WXMEMOLICENSE, wxEmptyString, wxPoint(10, 20), wxSize(435, 200), wxTE_READONLY | wxTE_AUTO_URL | wxTE_MULTILINE, wxDefaultValidator, wxT("WxMemoLicense"));
	WxMemoLicense->SetFocus();
	WxMemoLicense->SetInsertionPointEnd();
	WxStaticBoxSizer1->Add(WxMemoLicense, 0, wxALIGN_CENTER | wxALL, 5);

	WxBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_CENTER | wxALL, 5);

	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxPoint(5, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButtonOK"));
	WxBoxSizer3->Add(WxButtonOK, 0, wxALIGN_CENTER | wxALL, 5);

	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(90, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButtonCancel"));
	WxBoxSizer3->Add(WxButtonCancel, 0, wxALIGN_CENTER | wxALL, 5);

	SetTitle(_("About MadEdit-Mod"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
	
	WxStaticBitmap1->SetBitmap(wxBitmap(Mad_xpm));
	WxStaticBitmap1->Enable(true);
	WxStaticBitmap2->SetBitmap(wxBitmap(Mad2_xpm));
	WxStaticBitmap2->Enable(true);

	WxAuiNotebookAbout->SetSelection(0);
	SetDefaultItem(WxButtonCancel);
	WxButtonCancel->SetFocus();
}

void MadAboutDialog::MadAboutDialogClose(wxCloseEvent& event)
{
	// --> Don't use Close with a wxDialog,
	// use Destroy instead.
	Destroy();
}

