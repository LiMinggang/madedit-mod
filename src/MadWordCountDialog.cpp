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

extern bool IsMacroRecording();
extern void RecordAsMadMacro(MadEdit *, const wxString&, bool=false);

//(*IdInit(MadWordCountDialog)
//*)

MadWordCountDialog::MadWordCountDialog(wxWindow* parent,wxWindowID WXUNUSED(id),const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
{
	//(*Initialize(MadWordCountDialog)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, _("Word Count"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 60);
	WxStaticText7 = new wxStaticText(this, wxID_ANY, _("Lines"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT7"));
	FlexGridSizer1->Add(WxStaticText7, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextLineCount = new wxStaticText(this, wxID_ANY, _T("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTLINECOUNT"));
	FlexGridSizer1->Add(WxStaticTextLineCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText1 = new wxStaticText(this, wxID_ANY, _("Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT1"));
	FlexGridSizer1->Add(WxStaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextWordCount = new wxStaticText(this, wxID_ANY, _T("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTWORDCOUNT"));
	FlexGridSizer1->Add(WxStaticTextWordCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText2 = new wxStaticText(this, wxID_ANY, _("Characters (no space)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT2"));
	FlexGridSizer1->Add(WxStaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextCharCount = new wxStaticText(this, wxID_ANY, _T("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCHARCOUNT"));
	FlexGridSizer1->Add(WxStaticTextCharCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText3 = new wxStaticText(this, wxID_ANY, _("Characters (with spaces)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT3"));
	FlexGridSizer1->Add(WxStaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextSpaceCount = new wxStaticText(this, wxID_ANY, _T("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTSPACECOUNT"));
	FlexGridSizer1->Add(WxStaticTextSpaceCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText4 = new wxStaticText(this, wxID_ANY, _("Halfwidth Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT4"));
	FlexGridSizer1->Add(WxStaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextHalfwidthCount = new wxStaticText(this, wxID_ANY, _T("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTHALFWIDTHCOUNT"));
	FlexGridSizer1->Add(WxStaticTextHalfwidthCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticText5 = new wxStaticText(this, wxID_ANY, _("Fullwidth Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXT5"));
	FlexGridSizer1->Add(WxStaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextFullwidthCount = new wxStaticText(this, wxID_ANY, _T("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFULLWIDTHCOUNT"));
	FlexGridSizer1->Add(WxStaticTextFullwidthCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	StaticLine1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer1->Add(StaticLine1, 0, wxALL|wxEXPAND, 2);
	StaticText7 = new wxStaticText(this, wxID_ANY, _("Detail Information of Characters:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer1->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxMemo1 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(440,160), wxTE_MULTILINE|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_WXMEMO1"));
	BoxSizer1->Add(WxMemo1, 1, wxALL|wxEXPAND, 4);
	Button1 = new wxButton(this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	Button1->SetDefault();
	BoxSizer1->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Bind(wxEVT_CLOSE_WINDOW, &MadWordCountDialog::MadWordCountDialogClose, this);
	//*)

	extern MadEdit *g_ActiveMadEdit;
	int	words, chars, spaces, lines, halfwidths, fullwidths;
	wxArrayString detail;
	g_ActiveMadEdit->WordCount(g_ActiveMadEdit->IsSelected(), words, chars, spaces, halfwidths, fullwidths, lines, &detail);
	if( IsMacroRecording() )
		RecordAsMadMacro(g_ActiveMadEdit, wxString::Format(wxT("WordCount(selection=%s)"), g_ActiveMadEdit->IsSelected()?wxT("True"):wxT("False")));

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


void MadWordCountDialog::MadWordCountDialogClose(wxCloseEvent& WXUNUSED(event))
{
	Destroy();
}
