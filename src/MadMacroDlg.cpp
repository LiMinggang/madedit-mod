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
const long MadMacroDlg::ID_MADEDIT = wxNewId();
const long MadMacroDlg::ID_WXBUTTONTOGGLERESULT = wxNewId();
const long MadMacroDlg::ID_TEXTCTRLRESULT = wxNewId();
//*)

extern MadEdit *g_ActiveMadEdit;
extern int MadMessageBox(const wxString& message,
								 const wxString& caption = wxMessageBoxCaptionStr,
								 long style	= wxOK | wxCENTRE,
								 wxWindow *parent = nullptr,
								 int x = wxDefaultCoord, int y = wxDefaultCoord);
MadMacroDlg *g_MadMacroDlg = nullptr;
MadMacroDlg::MadMacroDlg(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	wxSize pymacro(640, 240);
	//(*Initialize(MadMacroDlg)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("MadEdit Macro"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    m_debug = false;
	m_Pymacro=new MadEdit(this, ID_MADEDIT, wxPoint(0, 0), pymacro);
	m_Pymacro->SetFixedWidthMode(false);
	m_Pymacro->SetRecordCaretMovements(false);
	m_Pymacro->SetInsertSpacesInsteadOfTab(true);
	m_Pymacro->SetWantTab(true);
	m_Pymacro->SetSyntax(wxT("MadPython"));
	m_Pymacro->SetDisplayBookmark(false);
	wxString endline(wxT("\r"));
	if (m_Pymacro->GetInsertNewLineType() == nltDOS) endline += wxT("\n");
	else if (m_Pymacro->GetInsertNewLineType() == nltUNIX) endline = wxT("\n");
	m_Pymacro->SetText((wxString(wxT("#Create MadEdit Object for the active edit")) + endline + wxT("medit = MadEdit()") + endline + endline));
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
	WxButtonResult = new wxButton(this, ID_WXBUTTONTOGGLERESULT, _("Results >>"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONTOGGLERESULT"));
	BoxSizer3->Add(WxButtonResult, 0, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxMemoOutput = new wxTextCtrl(this, ID_TEXTCTRLRESULT, wxEmptyString, wxDefaultPosition, wxSize(640,240), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxDOUBLE_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_TEXTCTRLRESULT"));
	BoxSizer4->Add(WxMemoOutput, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer4, 1, wxALL|wxEXPAND, 5);
	WxMemoOutput->Show(m_debug);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnWxButtonRunClick, this, wxID_OK );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnWxButtonCloseClick, this, wxID_CANCEL );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadMacroDlg::OnWxButtonResultClick, this, ID_WXBUTTONTOGGLERESULT );
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

void MadMacroDlg::OnWxButtonRunClick(wxCommandEvent& event)
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

void MadMacroDlg::OnWxButtonCloseClick(wxCommandEvent& event)
{
	if(WxMemoOutput) WxMemoOutput->Clear();
	EndModal(wxID_CANCEL);
}

void MadMacroDlg::OnWxButtonResultClick(wxCommandEvent& event)
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

void MadMacroDlg::MadMacroDlgClose(wxCloseEvent& event)
{
	Destroy();
	g_MadMacroDlg =	nullptr;
}
