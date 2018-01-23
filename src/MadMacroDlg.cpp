///////////////////////////////////////////////////////////////////////////////
// Name:		MadMacroDlg.cpp
// Description:
// Author:		minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "EmbeddedPython.hpp"
#include "MadMacroDlg.h"

//(*InternalHeaders(MadMacroDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MadMacroDlg)
//*)

extern MadEdit *g_ActiveMadEdit;
extern int MadMessageBox(const wxString& message,
								 const wxString& caption = wxMessageBoxCaptionStr,
								 long style	= wxOK | wxCENTRE,
								 wxWindow *parent = nullptr,
								 int x = wxDefaultCoord, int y = wxDefaultCoord);
MadMacroDlg *g_MadMacroDlg = nullptr;
wxString MadMacroDlg::m_PyacroContext;

MadMacroDlg::MadMacroDlg(wxWindow* parent,wxWindowID id,const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
{
	wxSize pymacro(640, 240);
	//(*Initialize(MadMacroDlg)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxButton* WxButtonClose;
	wxButton* WxButtonRun;
	wxButton* wxButtonReset;

	Create(parent, id, _("MadEdit Macro"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    m_debug = false;
	m_Pymacro=new MadEdit(this, wxID_ANY, wxPoint(0, 0), pymacro);
	m_Pymacro->SetFixedWidthMode(false);
	m_Pymacro->SetRecordCaretMovements(false);
	m_Pymacro->SetInsertSpacesInsteadOfTab(true);
	m_Pymacro->SetWantTab(true);
	m_Pymacro->SetSyntax(wxT("MadPython"));
	m_Pymacro->SetDisplayBookmark(false);
	if(m_PyacroContext == wxEmptyString)
	{
		wxString endline(wxT("\r"));
		if (m_Pymacro->GetInsertNewLineType() == nltDOS) endline += wxT("\n");
		else if (m_Pymacro->GetInsertNewLineType() == nltUNIX) endline = wxT("\n");
		m_PyacroContext = (wxString(wxT("#Create MadEdit Object for the active edit")) + endline + wxT("medit = MadEdit()") + endline + endline);
	}
	m_Pymacro->SetText( m_PyacroContext );
	m_Pymacro->SetCaretPosition(m_Pymacro->GetFileSize());
	//m_Pymacro = new wxTextCtrl(this, ID_MADEDIT, _("Text"), wxDefaultPosition, wxSize(640,240), 0, wxDefaultValidator, _T("ID_MADEDIT"));
	BoxSizer2->Add(m_Pymacro, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonRun = new wxButton(this, wxID_OK, _("&Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	WxButtonRun->SetDefault();
	BoxSizer3->Add(WxButtonRun, 0, wxALL|wxEXPAND, 5);
	WxButtonClose = new wxButton(this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer3->Add(WxButtonClose, 0, wxALL|wxEXPAND, 5);
	wxButtonReset = new wxButton(this, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer3->Add(wxButtonReset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonResult = new wxButton(this, wxID_ANY, _("Results >>"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer3->Add(WxButtonResult, 0, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxMemoOutput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(640,240), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxDOUBLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer4->Add(WxMemoOutput, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer4, 1, wxALL|wxEXPAND, 5);
	WxMemoOutput->Show(m_debug);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnWxButtonRunClick, this, wxID_OK );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnWxButtonCloseClick, this, wxID_CANCEL );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnButtonResetClick, this, wxButtonReset->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnWxButtonResultClick, this, WxButtonResult->GetId() );
	Bind( wxEVT_CLOSE_WINDOW, &MadMacroDlg::MadMacroDlgClose, this, wxID_ANY );
	//*)
	m_Pymacro->SetFocus();
}

MadMacroDlg::~MadMacroDlg()
{
	//(*Destroy(MadMacroDlg)
	//*)
	g_MadMacroDlg = nullptr;
}

void MadMacroDlg::SetPyScript(wxString & pyscript)
{
	m_Pymacro->SetText(pyscript);
}

void MadMacroDlg::SetEncoding(const wxString &encname)
{
	m_Pymacro->SetEncoding(encname);
}

void MadMacroDlg::OnWxButtonRunClick(wxCommandEvent& WXUNUSED(event))
{
	if(g_ActiveMadEdit)
	{
		wxString pystr;
		m_Pymacro->GetText(pystr);
		if(!pystr.IsEmpty())
		{
			if(!g_EmbeddedPython)
			{
				try
				{
					g_EmbeddedPython = new EmbeddedPython();
				}
				catch(std::bad_alloc &)
				{
					MadMessageBox(_("Memory	allocation failed"), _("Error"),  wxOK|wxICON_ERROR);
					g_EmbeddedPython = 0;
				}
			}
			if(g_EmbeddedPython)
			{
				if (m_debug)
				{
					wxStreamToTextRedirector redirector((wxTextCtrl	*)WxMemoOutput);
					g_MainFrame->SetMacroRunning();
					g_EmbeddedPython->exec(std::string(pystr.mb_str(wxConvUTF8)));
					g_MainFrame->SetMacroStopped();
				}
				else
				{
					g_MainFrame->SetMacroRunning();
					g_EmbeddedPython->exec(std::string(pystr.mb_str(wxConvUTF8)));
					g_MainFrame->SetMacroStopped();
				}
			}
		}
	}
}

void MadMacroDlg::OnWxButtonCloseClick(wxCommandEvent& WXUNUSED(event))
{
	if(WxMemoOutput) WxMemoOutput->Clear();
	EndModal(wxID_CANCEL);
}

void MadMacroDlg::OnWxButtonResultClick(wxCommandEvent& WXUNUSED(event))
{
	m_debug= !m_debug;
	WxMemoOutput->Show(m_debug);
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	if(m_debug)
		WxButtonResult->SetLabel(_("Results <<"));
	else
		WxButtonResult->SetLabel(_("Results >>"));
}

void MadMacroDlg::MadMacroDlgClose(wxCloseEvent& WXUNUSED(event))
{
	m_PyacroContext.Empty();
	m_Pymacro->GetText(m_PyacroContext);
	Destroy();
	g_MadMacroDlg = nullptr;
}

void MadMacroDlg::OnButtonResetClick(wxCommandEvent& event)
{
	wxString endline(wxT("\r"));
	if (m_Pymacro->GetInsertNewLineType() == nltDOS) endline += wxT("\n");
	else if (m_Pymacro->GetInsertNewLineType() == nltUNIX) endline = wxT("\n");
	m_PyacroContext = (wxString(wxT("#Create MadEdit Object for the active edit")) + endline + wxT("medit = MadEdit()") + endline + endline);
}
