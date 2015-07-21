///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////
#include <wx/textctrl.h>
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadMacroDlg.h"
#include "EmbeddedPython.hpp"

MadMacroDlg * g_MadMacroDlg = NULL;
    ////Event Table Start
BEGIN_EVENT_TABLE(MadMacroDlg,wxDialog)
	
	EVT_CLOSE(MadMacroDlg::MadMacroDlgClose)
	EVT_BUTTON(ID_WXBUTTONRUN,MadMacroDlg::OnRunClick)
	EVT_BUTTON(ID_WXBUTTONCLOSE,MadMacroDlg::OnCloseClick)
END_EVENT_TABLE()
    ////Event Table End

///////////////////////////////////////////////////////////////////////////
    extern int MadMessageBox(const wxString& message,
                                 const wxString& caption = wxMessageBoxCaptionStr,
                                 long style = wxOK | wxCENTRE,
                                 wxWindow *parent = NULL,
                                 int x = wxDefaultCoord, int y = wxDefaultCoord);

MadMacroDlg::MadMacroDlg(wxWindow* parent, bool debug, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    //this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox(this, wxID_ANY, wxT(""));
	WxStaticBoxSizer1 = new wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL);
	this->SetSizer(WxStaticBoxSizer1);
	this->SetAutoLayout(true);

    bSizer1 = new wxBoxSizer( wxVERTICAL );
	WxStaticBoxSizer1->Add(bSizer1, 1, wxALIGN_CENTER | wxALIGN_TOP | wxEXPAND | wxALL, 5);

    m_debug = debug;
    wxSize pymacro(640, 480);
    if (m_debug)
    {
        pymacro = wxSize(640, 240);
        m_output = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(640, 240), wxTE_READONLY|wxTE_MULTILINE|wxVSCROLL|wxHSCROLL|wxSIMPLE_BORDER );
    }
    else
    {
        m_output = 0;
    }

    m_pymacro=new MadEdit(this, ID_MADEDIT, wxDefaultPosition, pymacro);
    m_pymacro->SetFixedWidthMode(false);
    m_pymacro->SetRecordCaretMovements(false);
    m_pymacro->SetInsertSpacesInsteadOfTab(true);
    m_pymacro->SetWantTab(true);
    m_pymacro->SetSyntax(wxT("MadPython"));
    m_pymacro->SetDisplayBookmark(false);
    wxString endline(wxT("\r"));
    if (m_pymacro->GetInsertNewLineType() == nltDOS) endline += wxT("\n");
    else if (m_pymacro->GetInsertNewLineType() == nltUNIX) endline = wxT("\n");
    m_pymacro->SetText((wxString(wxT("#Create MadEdit Object for active edit")) + endline + wxT("medit = MadEdit()") + endline + endline));
    bSizer1->Add( m_pymacro, 1, wxEXPAND | wxALL, 5 );
    if (m_output)
        bSizer1->Add( m_output, 1, wxEXPAND | wxALL, 5 );

    bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	WxStaticBoxSizer1->Add(bSizer2, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
    m_run = new wxButton( this, ID_WXBUTTONRUN, _("Run"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer2->Add( m_run, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3 );
    
    m_close = new wxButton( this, ID_WXBUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer2->Add( m_close, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3 );
    m_run->SetDefault();
    
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
    
    this->Centre( wxBOTH );
}

MadMacroDlg::~MadMacroDlg()
{
}

void MadMacroDlg::OnRunClick( wxCommandEvent& event )
{
    wxString pystr;
    m_pymacro->GetText(pystr);
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
                MadMessageBox(_("Memory allocation failed"), _("Error"),  wxOK|wxICON_ERROR);
                g_EmbeddedPython = 0;
            }
        }
        if(g_EmbeddedPython)
        {
            if (m_debug)
            {
                wxStreamToTextRedirector redirector((wxTextCtrl *)m_output);
                g_MainFrame->SetMacroRunning();
                g_EmbeddedPython->exec(std::string(pystr.mb_str()));
                g_MainFrame->SetMacroStopped();
            }
            else
            {
                g_MainFrame->SetMacroRunning();
                g_EmbeddedPython->exec(std::string(pystr.mb_str()));
                g_MainFrame->SetMacroStopped();                
            }
        }
    }
}
void MadMacroDlg::OnCloseClick( wxCommandEvent& event ) 
{
	if(m_output) m_output->Clear();
    EndModal(ID_WXBUTTONCLOSE);
}

void MadMacroDlg::SetPyScript(wxString & pyscript)
{
    m_pymacro->SetText(pyscript);
}

void MadMacroDlg::MadMacroDlgClose(wxCloseEvent& event)
{
    // --> Don't use Close with a Dialog,
    // use Destroy instead.
    Destroy();
    g_MadMacroDlg = NULL;
}

