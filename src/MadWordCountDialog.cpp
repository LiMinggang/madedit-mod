///////////////////////////////////////////////////////////////////////////////
// Name:		MadWordCountDialog.cpp
// Description:	WordCount Dialog
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadWordCountDialog.h"
#include "MadEdit/MadEdit.h"


#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(MadWordCountDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern void	RecordAsMadMacro(MadEdit *,	const wxString&);

//(*IdInit(MadWordCountDialog)
const long MadWordCountDialog::ID_WXSTATICTEXT7 = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXTLINECOUNT = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXT1 = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXTWORDCOUNT = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXT2 = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXTCHARCOUNT = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXT3 = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXTSPACECOUNT = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXT4 = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXTHALFWIDTHCOUNT = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXT5 = wxNewId();
const long MadWordCountDialog::ID_WXSTATICTEXTFULLWIDTHCOUNT = wxNewId();
const long MadWordCountDialog::ID_STATICLINE1 = wxNewId();
const long MadWordCountDialog::ID_STATICTEXT7 = wxNewId();
const long MadWordCountDialog::ID_WXMEMO1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadWordCountDialog,wxDialog)
	//(*EventTable(MadWordCountDialog)
	//*)
END_EVENT_TABLE()

MadWordCountDialog::MadWordCountDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MadWordCountDialog)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, _("Word Count"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 60);
	WxStaticText7 = new wxStaticText(this, ID_WXSTATICTEXT7, _("Lines"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT7"));
	FlexGridSizer1->Add(WxStaticText7, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextLineCount = new wxStaticText(this, ID_WXSTATICTEXTLINECOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTLINECOUNT"));
	FlexGridSizer1->Add(WxStaticTextLineCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT1"));
	FlexGridSizer1->Add(WxStaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextWordCount = new wxStaticText(this, ID_WXSTATICTEXTWORDCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTWORDCOUNT"));
	FlexGridSizer1->Add(WxStaticTextWordCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _("Characters (no space)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT2"));
	FlexGridSizer1->Add(WxStaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextCharCount = new wxStaticText(this, ID_WXSTATICTEXTCHARCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCHARCOUNT"));
	FlexGridSizer1->Add(WxStaticTextCharCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("Characters (with spaces)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT3"));
	FlexGridSizer1->Add(WxStaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextSpaceCount = new wxStaticText(this, ID_WXSTATICTEXTSPACECOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTSPACECOUNT"));
	FlexGridSizer1->Add(WxStaticTextSpaceCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, _("Halfwidth Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT4"));
	FlexGridSizer1->Add(WxStaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextHalfwidthCount = new wxStaticText(this, ID_WXSTATICTEXTHALFWIDTHCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTHALFWIDTHCOUNT"));
	FlexGridSizer1->Add(WxStaticTextHalfwidthCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, _("Fullwidth Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT5"));
	FlexGridSizer1->Add(WxStaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextFullwidthCount = new wxStaticText(this, ID_WXSTATICTEXTFULLWIDTHCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFULLWIDTHCOUNT"));
	FlexGridSizer1->Add(WxStaticTextFullwidthCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer1->Add(StaticLine1, 0, wxALL|wxEXPAND, 2);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Detail Information of Characters:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer1->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, wxEmptyString, wxDefaultPosition, wxSize(440,160), wxTE_MULTILINE|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_WXMEMO1"));
	BoxSizer1->Add(WxMemo1, 1, wxALL|wxEXPAND, 4);
	Button1 = new wxButton(this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	Button1->SetDefault();
	BoxSizer1->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadWordCountDialog::MadWordCountDialogClose);
	//*)

	extern MadEdit *g_ActiveMadEdit;
	int	words, chars, spaces, lines, halfwidths, fullwidths;
	wxArrayString detail;
	g_ActiveMadEdit->WordCount(g_ActiveMadEdit->IsSelected(), words, chars,	spaces,	halfwidths,	fullwidths,	lines, &detail);
	RecordAsMadMacro(g_ActiveMadEdit, wxString::Format(wxT("WordCount(%s)"), g_ActiveMadEdit->IsSelected()?wxT("True"):wxT("False")));

	if(g_ActiveMadEdit->IsSelected())
	{
		this->SetTitle(_("Word Count(Selected Text)"));
	}
	
	WxStaticTextWordCount->SetLabel(wxString()<<words);
	WxStaticTextCharCount->SetLabel(wxString()<<chars);
	WxStaticTextSpaceCount->SetLabel(wxString()<<(chars+spaces));
	WxStaticTextHalfwidthCount->SetLabel(wxString()<<halfwidths);
	WxStaticTextFullwidthCount->SetLabel(wxString()<<fullwidths);
	WxStaticTextLineCount->SetLabel(wxString()<<lines);
	wxString str;
	for(size_t i=0;i<detail.Count();++i)
	{
		str<<detail[i]<<wxT("\n");
	}
	WxMemo1->SetValue(str);
	Button1->SetFocus();
}

MadWordCountDialog::~MadWordCountDialog()
{
	//(*Destroy(MadWordCountDialog)
	//*)
}


void MadWordCountDialog::MadWordCountDialogClose(wxCloseEvent& event)
{
	Destroy();
}
