///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include "MadOptionsDialog.h"

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable : 4996)
#endif
// disable 4996 {
//(*InternalHeaders(MadOptionsDialog)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning(pop)
#endif

#include <wx/aui/auibook.h>
#include <wx/fileconf.h>
#include <wx/config.h>

#include <wx/dir.h>
#include <wx/aui/auibook.h>

#include "MadEdit/MadEncoding.h"
#include "MadEdit/MadEditCommand.h"
#include "MadEdit/MadEdit.h"
#include "MadEdit/MadSyntax.h"
#include "MadEditFrame.h"
#include "MadUtils.h"
#include "MadEdit/MadEditPv.h"
#include "SpellCheckerManager.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

MadOptionsDialog *g_OptionsDialog = nullptr;
extern std::vector<wxString> g_LanguageString;
extern wxString g_MadEditAppDir;
extern wxArrayString g_FontNames;

TreeItemData *g_SelectedCommandItem = nullptr;
TreeItemData *g_CommandItemOfNewKey = nullptr;
int g_SelectedKeyId = -1;
#define ENABLE_BITMAP_THUARI 0

class KeyTextCtrl : public wxTextCtrl
{
	MadOptionsDialog * m_OptDlg;
public:
	KeyTextCtrl() : m_OptDlg(nullptr)
	{
	}
	KeyTextCtrl(wxWindow *parent, wxWindowID id,
			   const wxString& value = wxEmptyString,
			   const wxPoint& pos = wxDefaultPosition,
			   const wxSize& size = wxDefaultSize,
			   long style = 0,
			   const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxTextCtrlNameStr)
			   :wxTextCtrl(parent, id, value, pos, size, style, validator, name), m_OptDlg(nullptr)
	{
		Bind(wxEVT_KEY_DOWN, &KeyTextCtrl::OnKeyDown, this);
		Bind(wxEVT_SET_FOCUS, &KeyTextCtrl::OnSetFocus, this);
		Bind(wxEVT_KILL_FOCUS, &KeyTextCtrl::OnKillFocus, this);
	}

	void BindDialog(MadOptionsDialog * optDlg)
	{
		m_OptDlg = optDlg;
	}

	void OnKeyDown(wxKeyEvent& evt)
	{
		int flags=wxACCEL_NORMAL;
		int key=evt.GetKeyCode();

		if (evt.ControlDown()) flags|=wxACCEL_CTRL;
		if (evt.AltDown())     flags|=wxACCEL_ALT;
		if (evt.ShiftDown())   flags|=wxACCEL_SHIFT;

		MadEditShortCut sc=ShortCut(flags, key);
		static MadEditShortCut prevsc=0;

		if (sc==prevsc) return;
		prevsc=sc;

		wxString scstr=ShortCutToString(sc);
		SetValue(scstr);

		if (m_OptDlg)
			m_OptDlg->UpdateKeyHint();
		//evt.Skip();
	}
	
	void OnSetFocus(wxFocusEvent &evt)
	{   // for getting Ctrl-Tab
		if (m_OptDlg)
		{
			m_OptDlg->SetWindowStyleFlag(m_OptDlg->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
			m_OptDlg->AuiNotebook1->wxControl::SetWindowStyleFlag(m_OptDlg->AuiNotebook1->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
			m_OptDlg->Panel4->SetWindowStyleFlag(m_OptDlg->Panel4->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
			m_OptDlg->ButtonCancel->SetId(wxID_CANCEL);
		}
		evt.Skip();
	}
	
	void OnKillFocus(wxFocusEvent &evt)
	{   // restore wxTAB_TRAVERSAL
		if (m_OptDlg)
		{
			m_OptDlg->SetWindowStyleFlag(m_OptDlg->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
			m_OptDlg->AuiNotebook1->wxControl::SetWindowStyleFlag(m_OptDlg->AuiNotebook1->wxControl::GetWindowStyleFlag() | wxTAB_TRAVERSAL);
			m_OptDlg->Panel4->SetWindowStyleFlag(m_OptDlg->Panel4->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
			m_OptDlg->ButtonCancel->SetId(wxID_CANCEL);
		}
		evt.Skip();
	}
};

// filter '&' and '.'
wxString FilterChar(const wxChar *ws)
{
	wxString str;
	wxChar wc;

	while ((wc=*ws) != wxT('\0'))
	{
		if (wc!=wxT('&') && wc!=wxT('.'))
		{
			str << wc;
		}
		++ws;
	}

	return str;
}

//(*IdInit(MadOptionsDialog)
//*)

typedef struct {
	wxWindow * pWin;
	void (MadOptionsDialog::*method)(wxCommandEvent &);
} MadEvtMap;

#define ADD2EVENTMAP(con, pwin, method) \
{\
    MadEvtMap evtm = { pwin, method}; \
	con.push_back(evtm); \
}

MadOptionsDialog::MadOptionsDialog(wxWindow* parent,wxWindowID WXUNUSED(id))
{
	std::vector< MadEvtMap > ButtonEvtMapVec;
	std::vector< MadEvtMap > CheckboxEvtMapVec;
	std::vector< MadEvtMap > RadioButtonEvtMapVec;
	//(*Initialize(MadOptionsDialog)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer13;
	wxBoxSizer* BoxSizer14;
	wxBoxSizer* BoxSizer15;
	wxBoxSizer* BoxSizer16;
	wxBoxSizer* BoxSizer17;
	wxBoxSizer* BoxSizer18;
	wxBoxSizer* BoxSizer19;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer20;
	wxBoxSizer* BoxSizer21;
	wxBoxSizer* BoxSizer22;
	wxBoxSizer* BoxSizer23;
	wxBoxSizer* BoxSizer24;
	wxBoxSizer* BoxSizer25;
	wxBoxSizer* BoxSizer26;
	wxBoxSizer* BoxSizer27;
	wxBoxSizer* BoxSizer28;
	wxBoxSizer* BoxSizer29;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer30;
	wxBoxSizer* BoxSizer31;
	//wxBoxSizer* BoxSizer32;
	//wxBoxSizer* BoxSizer33;
	wxBoxSizer* BoxSizer34;
	wxBoxSizer* BoxSizer35;
	wxBoxSizer* BoxSizer36;
	wxBoxSizer* BoxSizer37;
	wxBoxSizer* BoxSizer38;
	wxBoxSizer* BoxSizer39;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer40;
	wxBoxSizer* BoxSizer41;
	wxBoxSizer* BoxSizer42;
	wxBoxSizer* BoxSizer43;
	wxBoxSizer* BoxSizer44;
	wxBoxSizer* BoxSizer47;
	wxBoxSizer* BoxSizer49;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer50;
	wxBoxSizer* BoxSizer51;
	wxBoxSizer* BoxSizer52;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxGridSizer* GridSizer3;
	wxPanel* Panel1;
	wxPanel* Panel2;
	wxPanel* Panel3;
	wxPanel* Panel5;
	wxPanel* Panel6;
	wxStaticBoxSizer* StaticBoxSizer10;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer5;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxStaticBoxSizer* StaticBoxSizer9;
	wxStaticText* StaticText12;
	wxStaticText* StaticText13;
	wxStaticText* StaticText14;
	wxStaticText* StaticText15;
	wxStaticText* StaticText16;
	wxStaticText* StaticText17;
	wxStaticText* StaticText18;
	wxStaticText* StaticText19;
	wxStaticText* StaticText1;
	wxStaticText* StaticText20;
	wxStaticText* StaticText21;
	wxStaticText* StaticText22;
	wxStaticText* StaticText23;
	wxStaticText* StaticText24;
	wxStaticText* StaticText25;
	wxStaticText* StaticText26;
	wxStaticText* StaticText27;
	wxStaticText* StaticText2;
	wxStaticText* StaticText3;
	wxStaticText* StaticText6;

	Create(parent, wxID_ANY, _("Options"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(900,400));

	SetMaxSize(wxSize(-1,-1));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer31 = new wxBoxSizer(wxVERTICAL);
	AuiNotebook1 = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS);
	Panel1 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer27->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	wxASSERT(g_LanguageString.empty() != true);
	ComboBoxLanguage = new wxComboBox(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, nullptr,  wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, wxT("wxID_ANY"));
#if wxMAJOR_VERSION < 3 || (wxMAJOR_VERSION == 3 && wxMINOR_VERSION < 1)
	wxArrayString items;
	items.Alloc(g_LanguageString.size());
	std::vector<wxString>::iterator itt = g_LanguageString.begin(); std::vector<wxString>::iterator itend = g_LanguageString.end();
	for (; itt != itend; ++itt)
	{
		items.Add(*itt);
	}
	ComboBoxLanguage->Append(items);
#else
	ComboBoxLanguage->Append(g_LanguageString);
#endif
	ComboBoxLanguage->SetValue(g_LanguageString[0]);
	BoxSizer27->Add(ComboBoxLanguage, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText17 = new wxStaticText(Panel1, wxID_ANY, _("Language of User Interface (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer27->Add(StaticText17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer27, 0, wxALL|wxEXPAND, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ComboBoxEncoding = new wxComboBox(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer4->Add(ComboBoxEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText1 = new wxStaticText(Panel1, wxID_ANY, _("Use this encoding to create new file or when MadEdit cannot determine the encoding of old file"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer4->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxEXPAND, 2);
	BoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer40->Add(10,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBoxSkipAutoEncoding = new wxCheckBox(Panel1, wxID_ANY, _("Enforce default encoding"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxSkipAutoEncoding->SetValue(false);
	BoxSizer40->Add(CheckBoxSkipAutoEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer3->Add(BoxSizer40, 0, wxALL|wxEXPAND, 2);
	BoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer35->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ComboBoxDefaultFont = new wxComboBox(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), g_FontNames, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer35->Add(ComboBoxDefaultFont, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText2 = new wxStaticText(Panel1, wxID_ANY, _("Default font"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer35->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer35, 0, wxALL|wxEXPAND, 2);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	BoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer39->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditDefaultFontSize = new wxTextCtrl(Panel1, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	EditDefaultFontSize->SetMaxLength(2);
	BoxSizer39->Add(EditDefaultFontSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText3 = new wxStaticText(Panel1, wxID_ANY, _("Default font size"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer39->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer39, 0, wxALL|wxEXPAND, 2);
	BoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer38->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxSizeToLoad = new wxTextCtrl(Panel1, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	EditMaxSizeToLoad->SetMaxLength(128);
	BoxSizer38->Add(EditMaxSizeToLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText4 = new wxStaticText(Panel1, wxID_ANY, _("Max file size to load whole file into memory"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer38->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer38, 0, wxALL|wxEXPAND, 2);
	BoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer17->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxTextFileSize = new wxTextCtrl(Panel1, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	EditMaxTextFileSize->SetMaxLength(128);
	BoxSizer17->Add(EditMaxTextFileSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText5 = new wxStaticText(Panel1, wxID_ANY, _("Max file size to load as a text file"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer17->Add(StaticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer17, 0, wxALL|wxEXPAND, 2);
	BoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer41->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ComboBoxViewModeInOpen = new wxComboBox(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	ComboBoxViewModeInOpen->SetSelection(ComboBoxViewModeInOpen->Append(_("Auto"))); /*{ emTextMode, emColumnMode, emHexMode }*/
	ComboBoxViewModeInOpen->Append(_("Text"));
	ComboBoxViewModeInOpen->Append(_("Column"));
	ComboBoxViewModeInOpen->Append(_("Hex"));
	BoxSizer41->Add(ComboBoxViewModeInOpen, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText27 = new wxStaticText(Panel1, wxID_ANY, _("View mode while opening file(for text file size)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer41->Add(StaticText27, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer41, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxDisplaySize = new wxTextCtrl(Panel1, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	EditMaxDisplaySize->SetMaxLength(5);
	BoxSizer6->Add(EditMaxDisplaySize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText6 = new wxStaticText(Panel1, wxID_ANY, _("Max chars displayed in search results"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer6->Add(StaticText6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer6, 0, wxALL|wxEXPAND, 2);
	CheckBoxSingleInstance = new wxCheckBox(Panel1, wxID_ANY, _("Single Instance (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxSingleInstance->SetValue(false);
	BoxSizer7->Add(CheckBoxSingleInstance, 0, wxALL|wxEXPAND, 2);
	CheckBoxReloadFiles = new wxCheckBox(Panel1, wxID_ANY, _("Reload files previously open on startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxReloadFiles->SetValue(false);
	BoxSizer7->Add(CheckBoxReloadFiles, 0, wxALL|wxEXPAND, 2);
	CheckBoxRecordCaretMovements = new wxCheckBox(Panel1, wxID_ANY, _("Record caret movements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxRecordCaretMovements->SetValue(false);
	BoxSizer7->Add(CheckBoxRecordCaretMovements, 0, wxALL|wxEXPAND, 2);
	CheckBoxRestoreCaretPos = new wxCheckBox(Panel1, wxID_ANY, _("Restore previous caret position when files are opened"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxRestoreCaretPos->SetValue(false);
	BoxSizer7->Add(CheckBoxRestoreCaretPos, 0, wxALL|wxEXPAND, 2);
	CheckBoxDoNotSaveSettings = new wxCheckBox(Panel1, wxID_ANY, _("Do not save settings to MadEdit.cfg when MadEdit closed (this session only)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxDoNotSaveSettings->SetValue(false);
	BoxSizer7->Add(CheckBoxDoNotSaveSettings, 0, wxALL|wxEXPAND, 2);
	CheckBoxPurgeHistory = new wxCheckBox(Panel1, wxID_ANY, _("Purge History while exiting"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPurgeHistory->SetValue(false);
	BoxSizer7->Add(CheckBoxPurgeHistory, 0, wxALL|wxEXPAND, 2);
	CheckBoxEnableAutoSave = new wxCheckBox(Panel1, wxID_ANY, _("Auto save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxEnableAutoSave->SetValue(false);
	ADD2EVENTMAP(CheckboxEvtMapVec, CheckBoxEnableAutoSave, &MadOptionsDialog::EnableAutoSaveClick);
	BoxSizer7->Add(CheckBoxEnableAutoSave, 0, wxALL|wxEXPAND, 2);
	BoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer37->Add(10,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditAutoSaveTimeout = new wxTextCtrl(Panel1, wxID_ANY, _T("10"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	EditAutoSaveTimeout->SetMaxLength(2);
	BoxSizer37->Add(EditAutoSaveTimeout, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText7 = new wxStaticText(Panel1, wxID_ANY, _("Timeout(M)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer37->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer37, 0, wxALL|wxEXPAND, 2);
	CheckBoxEnableAutoBackup = new wxCheckBox(Panel1, wxID_ANY, _("Auto backup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxEnableAutoBackup->SetValue(false);
	BoxSizer7->Add(CheckBoxEnableAutoBackup, 0, wxALL|wxEXPAND, 2);
#ifdef __WXMSW__
	CheckBoxRightClickMenu = new wxCheckBox(Panel1, wxID_ANY, _("Add MadEdit to the RightClickMenu of Explorer(Deselect to Remove the Entry from Windows Registry)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxRightClickMenu->SetValue(false);
	BoxSizer7->Add(CheckBoxRightClickMenu, 0, wxALL|wxEXPAND, 2);
#endif
	BoxSizer3->Add(BoxSizer7, 0, wxALL|wxEXPAND, 2);
	Panel1->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
	Panel2 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(900,400), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer9->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxLineLength = new wxTextCtrl(Panel2, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer9->Add(EditMaxLineLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText8 = new wxStaticText(Panel2, wxID_ANY, _("Max line length before Line-Wrap (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer9->Add(StaticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer9, 0, wxALL|wxEXPAND, 2);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer10->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxColumns = new wxTextCtrl(Panel2, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer10->Add(EditMaxColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText9 = new wxStaticText(Panel2, wxID_ANY, _("Columns of Wrap-By-Column"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer10->Add(StaticText9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer10, 0, wxALL|wxEXPAND, 2);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer11->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditTabColumns = new wxTextCtrl(Panel2, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer11->Add(EditTabColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText10 = new wxStaticText(Panel2, wxID_ANY, _("Columns of Tab"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer11->Add(StaticText10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer11, 0, wxALL|wxEXPAND, 2);
	BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer13->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditIndentColumns = new wxTextCtrl(Panel2, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer13->Add(EditIndentColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText11 = new wxStaticText(Panel2, wxID_ANY, _("Columns of Indent"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer13->Add(StaticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer13, 0, wxALL|wxEXPAND, 2);
	BoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer44->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditHintColumns = new wxTextCtrl(Panel2, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer44->Add(EditHintColumns, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText28 = new wxStaticText(Panel2, wxID_ANY, _("Columns Pos to Show Indicator"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer44->Add(StaticText28, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer44, 0, wxALL|wxEXPAND, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditDateTime = new wxTextCtrl(Panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer5->Add(EditDateTime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText12 = new wxStaticText(Panel2, wxID_ANY, _("Format of Date/Time"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer5->Add(StaticText12, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ButtonDateTime = new wxButton(Panel2, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonDateTime, &MadOptionsDialog::ButtonDateTimeClick);
	BoxSizer5->Add(ButtonDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBoxDateTimeInEnglish = new wxCheckBox(Panel2, wxID_ANY, _("Use English instead of current locale"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxDateTimeInEnglish->SetValue(false);
	BoxSizer5->Add(CheckBoxDateTimeInEnglish, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer5, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	CheckBoxTabOrSpaces = new wxCheckBox(Panel2, wxID_ANY, _("Insert Space char instead of Tab char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxTabOrSpaces->SetValue(false);
	BoxSizer12->Add(CheckBoxTabOrSpaces, 0, wxALL|wxEXPAND, 2);
	CheckBoxAutoIndent = new wxCheckBox(Panel2, wxID_ANY, _("Auto Indent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxAutoIndent->SetValue(false);
	BoxSizer12->Add(CheckBoxAutoIndent, 0, wxALL|wxEXPAND, 2);
	CheckBoxAutoCompletePair = new wxCheckBox(Panel2, wxID_ANY, _("Auto complete character pair"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxAutoCompletePair->SetValue(false);
	ADD2EVENTMAP(CheckboxEvtMapVec, CheckBoxAutoCompletePair, &MadOptionsDialog::CheckBoxAutoCompletePairClick);
	BoxSizer12->Add(CheckBoxAutoCompletePair, 0, wxALL|wxEXPAND, 2);
	BoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer23->Add(10,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBoxInsertPairForSelection = new wxCheckBox(Panel2, wxID_ANY, _("Insert pair instead of replacing selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxInsertPairForSelection->SetValue(false);
	BoxSizer23->Add(CheckBoxInsertPairForSelection, 1, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(BoxSizer23, 1, wxALL|wxEXPAND, 0);
	BoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	CheckBoxMouseSelectToCopy = new wxCheckBox(Panel2, wxID_ANY, _("Auto copy the mouse-selected text to clipboard"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxMouseSelectToCopy->SetValue(false);
	ADD2EVENTMAP(CheckboxEvtMapVec, CheckBoxMouseSelectToCopy, &MadOptionsDialog::CheckBoxMouseSelectToCopyClick);
	BoxSizer28->Add(CheckBoxMouseSelectToCopy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	CheckBoxCtrlWithMouseToSelect = new wxCheckBox(Panel2, wxID_ANY, _("when pressing Ctrl key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxCtrlWithMouseToSelect->SetValue(false);
	BoxSizer28->Add(CheckBoxCtrlWithMouseToSelect, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(BoxSizer28, 0, wxALL|wxALIGN_LEFT, 0);
	CheckBoxMiddleMouseToPaste = new wxCheckBox(Panel2, wxID_ANY, _("Paste text from clipboard when pressing middle mouse button"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxMiddleMouseToPaste->SetValue(false);
	BoxSizer12->Add(CheckBoxMiddleMouseToPaste, 0, wxALL|wxEXPAND, 2);
	CheckBoxAutoFillColumnPaste = new wxCheckBox(Panel2, wxID_ANY, _("Auto fill in column paste"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxAutoFillColumnPaste->SetValue(false);
	BoxSizer12->Add(CheckBoxAutoFillColumnPaste, 0, wxALL|wxEXPAND, 2);
	CheckBoxLDClickHighlight = new wxCheckBox(Panel2, wxID_ANY, _("Left double click to highlight"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxLDClickHighlight->SetValue(false);
	BoxSizer12->Add(CheckBoxLDClickHighlight, 0, wxALL|wxEXPAND, 2);
	CheckBoxTypewriterMode = new wxCheckBox(Panel2, wxID_ANY, _("Typewriter mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxTypewriterMode->SetValue(false);
	BoxSizer12->Add(CheckBoxTypewriterMode, 0, wxALL|wxEXPAND, 2);
	CheckBoxFixWidthMode = new wxCheckBox(Panel2, wxID_ANY, _("Fix width mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxFixWidthMode->SetValue(false);
	BoxSizer12->Add(CheckBoxFixWidthMode, 0, wxALL|wxEXPAND, 2);
	CheckBoxIgnoreUndoWrnMsg = new wxCheckBox(Panel2, wxID_ANY, _("Ignore undo warning message"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxIgnoreUndoWrnMsg->SetValue(false);
	BoxSizer12->Add(CheckBoxIgnoreUndoWrnMsg, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer12, 0, wxALL|wxEXPAND, 2);
	Panel2->SetSizer(BoxSizer8);
	BoxSizer8->Fit(Panel2);
	BoxSizer8->SetSizeHints(Panel2);
	Panel6 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(900,400), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer43 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer9 = new wxStaticBoxSizer(wxVERTICAL, Panel6, _("Line Ending"));
	RadioButtonNewDocLineEndingDefault = new wxRadioButton(Panel6, wxID_ANY, _("System Default"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocLineEndingDefault, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingDefault, 0, wxALL|wxEXPAND, 2);
	RadioButtonNewDocLineEndingCRLF = new wxRadioButton(Panel6, wxID_ANY, _T("Windows(CR LF)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocLineEndingCRLF, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingCRLF, 0, wxALL|wxEXPAND, 2);
	RadioButtonNewDocLineEndingLF = new wxRadioButton(Panel6, wxID_ANY, _T("Unix(LF)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocLineEndingLF, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	//ADD2CONTROLS(controls, RadioBoxLineEnding);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingLF, 0, wxALL|wxEXPAND, 2);
	RadioButtonNewDocLineEndingCR = new wxRadioButton(Panel6, wxID_ANY, _T("Macintosh"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocLineEndingCR, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingCR, 0, wxALL|wxEXPAND, 2);
	BoxSizer43->Add(StaticBoxSizer9, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer49 = new wxBoxSizer(wxVERTICAL);
	StaticText13 = new wxStaticText(Panel6, wxID_ANY, _("Syntax:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer49->Add(StaticText13, 0, wxALL|wxALIGN_LEFT, 2);
	ComboBoxNewDocSyntax = new wxComboBox(Panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer49->Add(ComboBoxNewDocSyntax, 0, wxALL|wxEXPAND, 2);
	BoxSizer43->Add(BoxSizer49, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer42->Add(BoxSizer43, 0, wxALL|wxALIGN_TOP, 5);
	BoxSizer47 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer10 = new wxStaticBoxSizer(wxVERTICAL, Panel6, _("Encoding"));
	RadioButtonNewDocEncSystemDefault = new wxRadioButton(Panel6, wxID_ANY, _("System Default"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocEncSystemDefault, &MadOptionsDialog::RadioButtonNewDocEncSelect);
	StaticBoxSizer10->Add(RadioButtonNewDocEncSystemDefault, 0, wxALL|wxEXPAND, 2);
	GridSizer3 = new wxGridSizer(0, 2, 0, 0);
	RadioButtonNewDocEncUTF8 = new wxRadioButton(Panel6, wxID_ANY, _T("UTF8"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	GridSizer3->Add(RadioButtonNewDocEncUTF8, 0, wxALL|wxEXPAND, 2);
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocEncUTF8, &MadOptionsDialog::RadioButtonNewDocEncSelect);
	CheckBoxNewDocEncUTF8WithBOM = new wxCheckBox(Panel6, wxID_ANY, _("with BOM"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxNewDocEncUTF8WithBOM->SetValue(false);
	GridSizer3->Add(CheckBoxNewDocEncUTF8WithBOM, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer10->Add(GridSizer3, 1, wxALL|wxALIGN_LEFT, 0);
	BoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
	RadioButtonNewDocEncOther = new wxRadioButton(Panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(RadioButtonEvtMapVec, RadioButtonNewDocEncOther, &MadOptionsDialog::RadioButtonNewDocEncSelect);
	BoxSizer50->Add(RadioButtonNewDocEncOther, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ComboBoxNewDocEncOther = new wxComboBox(Panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer50->Add(ComboBoxNewDocEncOther, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer10->Add(BoxSizer50, 0, wxALL|wxEXPAND, 0);
	BoxSizer47->Add(StaticBoxSizer10, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer51 = new wxBoxSizer(wxVERTICAL);
	StaticText14 = new wxStaticText(Panel6, wxID_ANY, _("Font:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer51->Add(StaticText14, 0, wxALL|wxALIGN_LEFT, 2);
	ComboBoxNewDocFont = new wxComboBox(Panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,-1), g_FontNames, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer51->Add(ComboBoxNewDocFont, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer47->Add(BoxSizer51, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer52 = new wxBoxSizer(wxVERTICAL);
	StaticText15 = new wxStaticText(Panel6, wxID_ANY, _("Font Size:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer52->Add(StaticText15, 0, wxALL|wxALIGN_LEFT, 2);
	EditNewDocFontSize = new wxTextCtrl(Panel6, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	EditNewDocFontSize->SetMaxLength(2);
	BoxSizer52->Add(EditNewDocFontSize, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer47->Add(BoxSizer52, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer42->Add(BoxSizer47, 0, wxALL|wxALIGN_TOP, 5);
	Panel6->SetSizer(BoxSizer42);
	BoxSizer42->Fit(Panel6);
	BoxSizer42->SetSizeHints(Panel6);
	Panel3 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(900,400), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer15 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Text Mode"));
	CheckBoxPrintSyntax = new wxCheckBox(Panel3, wxID_ANY, _("Print Syntax Highlighter"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintSyntax->SetValue(false);
	StaticBoxSizer1->Add(CheckBoxPrintSyntax, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintLineNumber = new wxCheckBox(Panel3, wxID_ANY, _("Print Line Number"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintLineNumber->SetValue(false);
	StaticBoxSizer1->Add(CheckBoxPrintLineNumber, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintBookmark = new wxCheckBox(Panel3, wxID_ANY, _("Print Bookmark"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintBookmark->SetValue(false);
	StaticBoxSizer1->Add(CheckBoxPrintBookmark, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintEndOfLine = new wxCheckBox(Panel3, wxID_ANY, _("Print End of Line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintEndOfLine->SetValue(false);
	StaticBoxSizer1->Add(CheckBoxPrintEndOfLine, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintTabChar = new wxCheckBox(Panel3, wxID_ANY, _("Print Tab Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintTabChar->SetValue(false);
	StaticBoxSizer1->Add(CheckBoxPrintTabChar, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintSpaceChar = new wxCheckBox(Panel3, wxID_ANY, _("Print Space Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintSpaceChar->SetValue(false);
	StaticBoxSizer1->Add(CheckBoxPrintSpaceChar, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer15->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Hex Mode"));
	wxString __wxRadioBoxChoices_1[3] =
	{
		_("None"),
		_("First Page Only"),
		_("Every Page")
	};
	RadioBoxPrintOffset = new wxRadioBox(Panel3, wxID_ANY, _("Print Offset Header"), wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("wxID_ANY"));
	StaticBoxSizer2->Add(RadioBoxPrintOffset, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer15->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer15, 0, wxALL|wxALIGN_TOP, 2);
	BoxSizer16 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Header"));
	CheckBoxPrintPageHeader = new wxCheckBox(Panel3, wxID_ANY, _("Print Page Header"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintPageHeader->SetValue(false);
	StaticBoxSizer3->Add(CheckBoxPrintPageHeader, 0, wxALL|wxALIGN_LEFT, 1);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText16 = new wxStaticText(Panel3, wxID_ANY, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText16, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditHeaderLeft = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer1->Add(EditHeaderLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button1 = new wxButton(Panel3, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, Button1, &MadOptionsDialog::PrintMarkButtonClick);
	EditHeaderLeftId   = Button1->GetId();
	FlexGridSizer1->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText18 = new wxStaticText(Panel3, wxID_ANY, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText18, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditHeaderCenter = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer1->Add(EditHeaderCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button2 = new wxButton(Panel3, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, Button2, &MadOptionsDialog::PrintMarkButtonClick);
	EditHeaderCenterId = Button2->GetId();
	FlexGridSizer1->Add(Button2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText19 = new wxStaticText(Panel3, wxID_ANY, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText19, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditHeaderRight = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer1->Add(EditHeaderRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button3 = new wxButton(Panel3, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, Button3, &MadOptionsDialog::PrintMarkButtonClick);
	EditHeaderRightId  = Button3->GetId();
	FlexGridSizer1->Add(Button3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer3->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Footer"));
	CheckBoxPrintPageFooter = new wxCheckBox(Panel3, wxID_ANY, _("Print Page Footer"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPrintPageFooter->SetValue(false);
	StaticBoxSizer4->Add(CheckBoxPrintPageFooter, 0, wxALL|wxALIGN_LEFT, 2);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText20 = new wxStaticText(Panel3, wxID_ANY, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer2->Add(StaticText20, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditFooterLeft = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer2->Add(EditFooterLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button4 = new wxButton(Panel3, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, Button4, &MadOptionsDialog::PrintMarkButtonClick);
	EditFooterLeftId   = Button4->GetId();
	FlexGridSizer2->Add(Button4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText21 = new wxStaticText(Panel3, wxID_ANY, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer2->Add(StaticText21, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditFooterCenter = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer2->Add(EditFooterCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button5 = new wxButton(Panel3, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, Button5, &MadOptionsDialog::PrintMarkButtonClick);
	EditFooterCenterId = Button5->GetId();
	FlexGridSizer2->Add(Button5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText22 = new wxStaticText(Panel3, wxID_ANY, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer2->Add(StaticText22, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditFooterRight = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer2->Add(EditFooterRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button6 = new wxButton(Panel3, wxID_ANY, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, Button6, &MadOptionsDialog::PrintMarkButtonClick);
	EditFooterRightId  = Button6->GetId();
	FlexGridSizer2->Add(Button6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer4->Add(FlexGridSizer2, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer16, 0, wxALL|wxALIGN_TOP, 2);
	Panel3->SetSizer(BoxSizer14);
	BoxSizer14->Fit(Panel3);
	BoxSizer14->SetSizeHints(Panel3);
	Panel4 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(900,400), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	TreeCtrl1 = new wxTreeCtrl(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer19->Add(TreeCtrl1, 1, wxALL|wxEXPAND, 3);
	BoxSizer18->Add(BoxSizer19, 2, wxALL|wxEXPAND, 4);
	BoxSizer20 = new wxBoxSizer(wxVERTICAL);
	BoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	StaticText23 = new wxStaticText(Panel4, wxID_ANY, _("Hint:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer24->Add(StaticText23, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	EditCommandHint = new wxTextCtrl(Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("wxID_ANY"));
	EditCommandHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer24->Add(EditCommandHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer20->Add(BoxSizer24, 0, wxALL|wxEXPAND, 2);
	BoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer22 = new wxBoxSizer(wxVERTICAL);
	StaticText24 = new wxStaticText(Panel4, wxID_ANY, _("Assigned Keys:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer22->Add(StaticText24, 0, wxALL|wxALIGN_LEFT, 5);
	ListBoxKeys = new wxListBox(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer22->Add(ListBoxKeys, 1, wxALL|wxEXPAND, 2);
	BoxSizer21->Add(BoxSizer22, 0, wxALL|wxEXPAND, 2);
	BoxSizer25 = new wxBoxSizer(wxVERTICAL);
	StaticText25 = new wxStaticText(Panel4, wxID_ANY, _("New Key:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer25->Add(StaticText25, 0, wxALL|wxALIGN_LEFT, 5);
	EditKey = new KeyTextCtrl(Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB|wxWANTS_CHARS, wxDefaultValidator, _T("wxID_ANY"));
	EditKey->BindDialog(this);
	BoxSizer25->Add(EditKey, 0, wxALL|wxEXPAND, 5);
	EditKeyHint = new wxTextCtrl(Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("wxID_ANY"));
	EditKeyHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer25->Add(EditKeyHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer29 = new wxBoxSizer(wxVERTICAL);
	ButtonAddKey = new wxButton(Panel4, wxID_ANY, _("<== Add Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonAddKey, &MadOptionsDialog::ButtonAddKeyClick);
	BoxSizer29->Add(ButtonAddKey, 0, wxALL|wxEXPAND, 5);
	ButtonDeleteKey = new wxButton(Panel4, wxID_ANY, _("==> Delete Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonDeleteKey, &MadOptionsDialog::ButtonDeleteKeyClick);
	BoxSizer29->Add(ButtonDeleteKey, 0, wxALL|wxEXPAND, 5);
	ButtonShowInMenu = new wxButton(Panel4, wxID_ANY, _("Show the Key in Menu"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonShowInMenu, &MadOptionsDialog::ButtonShowInMenuClick);
	BoxSizer29->Add(ButtonShowInMenu, 0, wxALL|wxEXPAND, 5);
	BoxSizer25->Add(BoxSizer29, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer21->Add(BoxSizer25, 5, wxALL|wxEXPAND, 1);
	BoxSizer20->Add(BoxSizer21, 1, wxALL|wxEXPAND, 2);
	BoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
	CheckBoxResetAllKeys = new wxCheckBox(Panel4, wxID_ANY, _("Reset all keys to default (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxResetAllKeys->SetValue(false);
	BoxSizer26->Add(CheckBoxResetAllKeys, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer20->Add(BoxSizer26, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer18->Add(BoxSizer20, 3, wxALL|wxEXPAND, 2);
	Panel4->SetSizer(BoxSizer18);
	BoxSizer18->Fit(Panel4);
	BoxSizer18->SetSizeHints(Panel4);
	Panel5 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(900,400), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer36 = new wxBoxSizer(wxVERTICAL);
	BoxSizer34 = new wxBoxSizer(wxVERTICAL);
	CheckBoxPersonalDict = new wxCheckBox(Panel5, wxID_ANY, _("Enable Personal Dictionary"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxPersonalDict->SetValue(false);
	BoxSizer34->Add(CheckBoxPersonalDict, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxAddKeywords = new wxCheckBox(Panel5, wxID_ANY, _("Add Keywords from Syntax File"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBoxAddKeywords->SetValue(false);
	BoxSizer34->Add(CheckBoxAddKeywords, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer36->Add(BoxSizer34, 0, wxALL|wxALIGN_LEFT, 2);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, Panel5, _("Language"));
	ComboDictionary = new wxComboBox(Panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200,-1), 0, 0, wxCB_READONLY, wxDefaultValidator, _T("wxID_ANY"));
	StaticBoxSizer5->Add(ComboDictionary, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText26 = new wxStaticText(Panel5, wxID_ANY, _("Dictionary"), wxDefaultPosition, wxSize(200,-1), 0, _T("wxID_ANY"));
	StaticBoxSizer5->Add(StaticText26, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(StaticBoxSizer5, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, Panel5, _("Path Setting"));
	EditDictionaryDir = new wxTextCtrl(Panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200,-1), 0, wxDefaultValidator, _T("wxID_ANY"));
	StaticBoxSizer6->Add(EditDictionaryDir, 0, wxALL|wxALIGN_LEFT, 2);
	ButtonDictionaryDir = new wxButton(Panel5, wxID_ANY, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonDictionaryDir, &MadOptionsDialog::ButtonDictionaryDirClick);
	StaticBoxSizer6->Add(ButtonDictionaryDir, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(StaticBoxSizer6, 0, wxALL|wxEXPAND, 2);
	Panel5->SetSizer(BoxSizer36);
	BoxSizer36->Fit(Panel5);
	BoxSizer36->SetSizeHints(Panel5);
	AuiNotebook1->AddPage(Panel1, _("General"));
	AuiNotebook1->AddPage(Panel2, _("Edit"));
	AuiNotebook1->AddPage(Panel6, _("New Document"));
	AuiNotebook1->AddPage(Panel3, _("Print"));
	AuiNotebook1->AddPage(Panel4, _("Keys"));
	AuiNotebook1->AddPage(Panel5, _("SpellChecker"));
	BoxSizer31->Add(AuiNotebook1, 1, wxALL|wxEXPAND, 0);
	//BoxSizer32 = new wxBoxSizer(wxVERTICAL);
	//BoxSizer32->Add(450,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	//BoxSizer31->Add(BoxSizer32, 0, wxALL|wxEXPAND, 0);
	BoxSizer30->Add(BoxSizer31, 1, wxALL|wxEXPAND, 0);
	//BoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
	//BoxSizer33->Add(0,260,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	//BoxSizer30->Add(BoxSizer33, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Add(BoxSizer30, 1, wxALL|wxEXPAND, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonOK, &MadOptionsDialog::ButtonOKClick);
	BoxSizer2->Add(ButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	ADD2EVENTMAP(ButtonEvtMapVec, ButtonCancel, &MadOptionsDialog::ButtonCancelClick);
	BoxSizer2->Add(ButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
#if 0
	Layout();
	Center();

	Connect(wxID_ANY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::EnableAutoSaveClick);
	Connect(wxID_ANY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonDateTimeClick);
	Connect(wxID_ANY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::CheckBoxAutoCompletePairClick);
	Connect(wxID_ANY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::CheckBoxMouseSelectToCopyClick);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocEncSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocEncSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocEncSelect);
	Connect(wxID_ANY,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&MadOptionsDialog::TreeCtrl1SelChanged);
	Connect(wxID_ANY,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::ListBoxKeysSelected);
	Connect(wxID_ANY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonAddKeyClick);
	Connect(wxID_ANY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonDeleteKeyClick);
	Connect(wxID_ANY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonShowInMenuClick);
	Connect(wxID_ANY,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::ChoiceDictionarySelect);
	Connect(wxID_ANY,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&MadOptionsDialog::EditDictionaryDirTextEnter);
	Connect(wxID_ANY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonDictionaryDirClick);
	Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonOKClick);
	Connect(wxID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonCancelClick);
#endif
	//*)
	for (size_t i = 0; i < ButtonEvtMapVec.size(); ++i)
	{
		Bind(wxEVT_BUTTON, ButtonEvtMapVec[i].method, this, ButtonEvtMapVec[i].pWin->GetId());
	}

	for (size_t i = 0; i < CheckboxEvtMapVec.size(); ++i)
	{
		Bind(wxEVT_CHECKBOX, CheckboxEvtMapVec[i].method, this, CheckboxEvtMapVec[i].pWin->GetId());
	}

	for (size_t i = 0; i < RadioButtonEvtMapVec.size(); ++i)
	{
		Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, RadioButtonEvtMapVec[i].method, this, RadioButtonEvtMapVec[i].pWin->GetId());
	}

	Bind(wxEVT_CLOSE_WINDOW, &MadOptionsDialog::MadOptionsDialogClose , this);
	Bind(wxEVT_ACTIVATE, &MadOptionsDialog::MadOptionsDialogActivate , this);

	Bind(wxEVT_TEXT_ENTER, &MadOptionsDialog::EditDictionaryDirTextEnter, this, EditDictionaryDir->GetId());
	Bind(wxEVT_COMBOBOX, &MadOptionsDialog::ComboDictionarySelected, this, ComboDictionary->GetId());
	Bind(wxEVT_LISTBOX, &MadOptionsDialog::ListBoxKeysSelected , this, ListBoxKeys->GetId());
	Bind(wxEVT_TREE_SEL_CHANGED, &MadOptionsDialog::TreeCtrl1SelChanged , this, TreeCtrl1->GetId());

	const wxChar * printMenus[] = 
	{
		_("[%f] &File Name"), 
		_("[%p] &Path Name"),
		nullptr,
		_("[%n] Page &Number"),
		_("[%s] Total Page&s"),
		nullptr,
		_("[%d] &Date"),
		_("[%t] &Time"),
	};

	for (size_t i = 0; i < (sizeof(printMenus)/sizeof(const wxChar *)); ++i)
	{
		if (printMenus[i])
		{
			wxMenuItem * menuItem = PopupMenuPrintMark.Append(wxID_ANY, printMenus[i], _T(""), wxITEM_NORMAL);
			Bind(wxEVT_MENU, &MadOptionsDialog::PrintMarkClick, this, menuItem->GetId());
		}
		else
		{
			PopupMenuPrintMark.AppendSeparator();
		}
	}

	const wxChar * datetimeMenus[] =
	{
		_T("[%Y/%m/%d %I:%M:%S %p] 2018/02/24 08:30:55 AM"),
		_T("[%d/%m/%Y] 24/02/2018"),
		_("[%a, %d %b %Y %H:%M:%S %z] RFC822 timestamp"), 
		nullptr,
		_("[%a] Abbreviated weekday name"),
		_("[%A] Full weekday name"),
		_("[%b] Abbreviated month name"),
		_("[%B] Full month name"),
		_("[%c] Date and time representation appropriate for locale"),
		_("[%d] Day of month as decimal number (01 - 31)"),
		_("[%H] Hour in 24-hour format (00 - 23)"),
		_("[%I] Hour in 12-hour format (01 - 12)"),
		_("[%j] Day of year as decimal number (001 - 366)"),
		_("[%m] Month as decimal number (01 - 12)"),
		_("[%M] Minute as decimal number (00 - 59)"),
		_("[%p] Current locale's A.M./P.M. indicator for 12-hour clock"),
		_("[%S] Second as decimal number (00 - 59)"),
		_("[%U] Week of year as decimal number,  with Sunday as first day of week (00 - 53)"),
		_("[%w] Weekday as decimal number (0 - 6; Sunday is 0)"),
		_("[%W] Week of year as decimal number, with Monday as first day of week (00 - 53)"),
		_("[%x] Date representation for current locale"),
		_("[%X] Time representation for current locale"),
		_("[%y] Year without century, as decimal number (00 - 99)"),
		_("[%Y] Year with century, as decimal number"),
		_("[%z] Time-zone name"),
		_("[%Z] Time-zone abbreviation"),
	};

	for (size_t i = 0; i < (sizeof(datetimeMenus)/sizeof(const wxChar *)); ++i)
	{
		if (datetimeMenus[i])
		{
			wxMenuItem * menuItem = PopupMenuDateTimeMark.Append(wxID_ANY, datetimeMenus[i], _T(""), wxITEM_NORMAL);
			Bind(wxEVT_MENU, &MadOptionsDialog::DateTimeMarkClick, this, menuItem->GetId());
		}
		else
		{
			PopupMenuDateTimeMark.AppendSeparator();
		}
	}

	wxString systemenc(_("System Default"));
	ComboBoxEncoding->Append(systemenc);
	size_t cnt = MadEncoding::GetEncodingsCount();
	size_t i;

	for (i = 0; i < cnt; ++i)
	{
		ComboBoxEncoding->Append(MadEncoding::GetEncodingName(i)); //enc+des);
		ComboBoxNewDocEncOther->Append(MadEncoding::GetEncodingName(i));
	}

	ComboBoxEncoding->SetValue(systemenc);

	/*for (i=0; i<g_LanguageString.GetCount(); ++i)
	{
	ComboBoxLanguage->Append(g_LanguageString[i]);
	}
	ComboBoxLanguage->SetValue(g_LanguageString[0]);*/

	cnt=MadSyntax::GetSyntaxCount();
	for (i = 0; i < cnt; ++i)
	{
		ComboBoxNewDocSyntax->Append(wxGetTranslation(MadSyntax::GetSyntaxTitle(i)));
	}

	int index = ComboBoxNewDocSyntax->FindString(_("Plain Text"));
	if (index != wxNOT_FOUND)
		ComboBoxNewDocSyntax->SetSelection(index);
	m_NewDocEncoding = _("System Default");
	m_NewDocLineEnding = nltDefault;

#ifdef __WXMSW__
	wxString fontname(wxT("Courier New"));
#elif defined(__APPLE__) && defined(__MACH__)
	wxString fontname(wxT("Monaco"));
#else
	wxString fontname(wxT("Monospace"));
#endif
	index = ComboBoxDefaultFont->FindString(fontname);
	if (index != wxNOT_FOUND) ComboBoxDefaultFont->SetSelection(index);
	index = ComboBoxNewDocFont->FindString(fontname);
	if (index != wxNOT_FOUND) ComboBoxNewDocFont->SetSelection(index);

	wxSize sz1 = BoxSizer3->CalcMin();
	wxSize sz2 = BoxSizer8->CalcMin();
	wxSize sz3 = BoxSizer14->CalcMin();
	wxSize sz4 = BoxSizer18->CalcMin();

	int ww = std::max(std::max(sz1.x, sz2.x), std::max(sz3.x, sz4.x));
	int hh = std::max(std::max(sz1.y, sz2.y), std::max(sz3.y, sz4.y));

	//BoxSizer33->Add(0, hh + AuiNotebook1->GetTabCtrlHeight() + 2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	//BoxSizer30->Add(0, hh + AuiNotebook1->GetTabCtrlHeight() + 2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	//BoxSizer32->Add(ww + 2, 0, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer30->SetMinSize(ww + 10,  hh + AuiNotebook1->GetTabCtrlHeight() + 10);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Layout();
	Center();

	// build Command Tree
	TreeCtrl1->SetImageList(g_MainFrame->m_ImageList);
	wxTreeItemId root=TreeCtrl1->AddRoot(_("Commands"));

	wxTreeItemId menuRoot=TreeCtrl1->AppendItem(root, _("Menu"));
	wxTreeItemId editorRoot=TreeCtrl1->AppendItem(root, _("Editor"));

	list<wxTreeItemId> tree_stack;
	CommandData *cd = &CommandTable[0];
	wxTreeItemId tid;

	// Menu command
	do
	{
		if (cd->menu_level==0)
		{
			tid=TreeCtrl1->AppendItem(menuRoot, FilterChar(wxGetTranslation(cd->text)));
			tree_stack.clear();
			tree_stack.push_back(tid);
			++cd;
			continue;
		}
		else if (cd->menu_level < int(tree_stack.size()))
		{
			do
			{
				tree_stack.pop_back();
			}
			while (cd->menu_level < int(tree_stack.size()));
		}

		if (cd->menu_ptr != 0)
		{
			tid=TreeCtrl1->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)));
			tree_stack.push_back(tid);
		}
		else if (cd->kind != wxITEM_SEPARATOR)
		{
			TreeItemData *data=new TreeItemData;
			data->cmddata=cd;
			TreeItemDataList.push_back(data);

			tid=TreeCtrl1->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)), cd->image_idx, cd->image_idx, data);
			TreeCtrl1->SetItemBold(tid, true);
		}

		++cd;
	}
	while (cd->menu_level>=0);

	// Editor command
	do
	{
		TreeItemData *data=new TreeItemData;
		data->cmddata=cd;
		TreeItemDataList.push_back(data);

		tid=TreeCtrl1->AppendItem(editorRoot, MadKeyBindings::CommandToText(cd->command), cd->image_idx, cd->image_idx, data);
		TreeCtrl1->SetItemBold(tid, true);
		cd++;
	}
	while (cd->command > 0);

	TreeCtrl1->Expand(menuRoot);
	TreeCtrl1->Expand(editorRoot);
	TreeCtrl1->Expand(root);

	ButtonCancel->SetId(wxID_CANCEL);
}

MadOptionsDialog::~MadOptionsDialog()
{
	//(*Destroy(MadOptionsDialog)
	//*)
}

void MadOptionsDialog::MadOptionsDialogClose(wxCloseEvent& event)
{
	if (event.CanVeto() && (dynamic_cast< wxFrame * >(wxTheApp->GetTopWindow())))
	{
		event.Veto();
		Show(false);
		return;
	}

	g_OptionsDialog=nullptr;
	Destroy();
}

void MadOptionsDialog::MadOptionsDialogActivate(wxActivateEvent& event)
{
	if (event.GetActive())
	{
		if (!FindFocus())
		{
			SetReturnCode(wxID_CANCEL);
			ButtonCancel->SetFocus();
		}

		if (ComboDictionary->GetCount())
		{
			ComboDictionary->Enable(true);
		}
		else
		{
			ComboDictionary->Enable(false);
		}
	}

	event.Skip();
}

void MadOptionsDialog::LoadOptions(void)
{
	wxConfigBase *cfg = wxConfigBase::Get(false);
	wxString oldpath = cfg->GetPath();
	cfg->SetPath(wxT("/MadEdit"));
	long ll;
	bool bb;
	wxString ss;
#ifdef __WXMSW__
	wxString fontname(wxT("Courier New"));
#elif defined(__APPLE__) && defined(__MACH__)
	wxString fontname(wxT("Monaco"));
#else
	wxString fontname(wxT("Monospace"));
#endif
	// General page
	ss = g_LanguageString[0];
	cfg->Read(wxT("/Application/Language"), &ss);
	ComboBoxLanguage->SetValue(ss);
	cfg->Read(wxT("/Application/SingleInstance"), &bb, false);
	CheckBoxSingleInstance->SetValue(bb);
	cfg->Read(wxT("RecordCaretMovements"), &bb, true);
	CheckBoxRecordCaretMovements->SetValue(bb);
	cfg->Read(wxT("MaxSizeToLoad"), &ll);
	EditMaxSizeToLoad->SetValue(wxString() << ll);
	cfg->Read(wxT("MaxTextFileSize"), &ll);
	EditMaxTextFileSize->SetValue(wxString() << ll);
	cfg->Read(wxT("ViewModeInOpen"), &ll, 0);
	if (ll < 0 || ll > 3) /*{ emTextMode, emColumnMode, emHexMode }*/
		ll = 0; //Auto
	ComboBoxViewModeInOpen->SetSelection(ll);
	cfg->Read(wxT("/UIView/MaxDisplaySize"), &ll);
	EditMaxDisplaySize->SetValue(wxString() << ll);
	ss = _("System Default");
	cfg->Read(wxT("DefaultEncoding"), &ss);
	ComboBoxEncoding->SetValue(ss);
	cfg->Read(wxT("EnforceDefaultEncoding"), &bb, false);
	CheckBoxSkipAutoEncoding->SetValue(bb);

	cfg->Read(wxT("DefaultTextFont"), &fontname);
	int index = ComboBoxDefaultFont->FindString(fontname);
	if (index != wxNOT_FOUND) ComboBoxDefaultFont->SetSelection(index);
	cfg->Read(wxT("DefaultTextFontSize"), &ll, DEFAULT_FONT_SIZE);
	if (ll < 0) ll = DEFAULT_FONT_SIZE;
	if (ll > MAX_FONT_SIZE) ll = MAX_FONT_SIZE;
	EditDefaultFontSize->SetValue(wxString() << ll);

	cfg->Read(wxT("/Application/AutoSaveTimeout"), &ll, 0);
	bb = ((ll >= 10) && (ll <= 30));
	CheckBoxEnableAutoSave->SetValue(bb);
	if (!bb) ll = 10;
	EditAutoSaveTimeout->Enable(bb);
	EditAutoSaveTimeout->SetValue(wxString() << ll);
	cfg->Read(wxT("/Application/AutoBackup"), &bb, false);
	CheckBoxEnableAutoBackup->SetValue(bb);
#ifdef __WXMSW__
	wxRegKey *pRegKey = new wxRegKey(g_MadEditRegkeyPath + wxT("*\\shell\\MadEdit-Mod\\command"));

	if (pRegKey->Exists())
	{
		wxString str;

		if (pRegKey->QueryValue(wxEmptyString, str))
		{
			wxString exepath = GetExecutablePath();
			CheckBoxRightClickMenu->SetValue(str.Upper().Find(exepath.Upper()) >= 0);
		}
	}

	delete pRegKey;
#endif

	// Edit page
	cfg->Read(wxT("MaxLineLength"), &ll);
	EditMaxLineLength->SetValue(wxString() << ll);
	cfg->Read(wxT("MaxColumns"), &ll);
	EditMaxColumns->SetValue(wxString() << ll);
	cfg->Read(wxT("TabColumns"), &ll);
	EditTabColumns->SetValue(wxString() << ll);
	cfg->Read(wxT("IndentColumns"), &ll);
	EditIndentColumns->SetValue(wxString() << ll);
	cfg->Read(wxT("HintColumns"), &ll, 80);
	EditHintColumns->SetValue(wxString() << ll);
	cfg->Read(wxT("DateTimeFormat"), &ss, wxT("%c"));
	EditDateTime->SetValue(ss);
	cfg->Read(wxT("DateTimeInEnglish"), &bb, false);
	CheckBoxDateTimeInEnglish->SetValue(bb);
	cfg->Read(wxT("InsertSpacesInsteadOfTab"), &bb, false);
	CheckBoxTabOrSpaces->SetValue(bb);
	cfg->Read(wxT("AutoIndent"), &bb, false);
	CheckBoxAutoIndent->SetValue(bb);
	cfg->Read(wxT("AutoCompletePair"), &bb, false);
	CheckBoxAutoCompletePair->SetValue(bb);
	CheckBoxInsertPairForSelection->Enable(bb);
	cfg->Read(wxT("InsertPairForSelction"), &bb, true);
	CheckBoxInsertPairForSelection->SetValue(bb);
	cfg->Read(wxT("MouseSelectToCopy"), &bb, false);
	CheckBoxMouseSelectToCopy->SetValue(bb);
	CheckBoxCtrlWithMouseToSelect->Enable(bb);
	cfg->Read(wxT("MouseSelectToCopyWithCtrlKey"), &bb, true);
	CheckBoxCtrlWithMouseToSelect->SetValue(bb);
	cfg->Read(wxT("MiddleMouseToPaste"), &bb, false);
	CheckBoxMiddleMouseToPaste->SetValue(bb);
	cfg->Read(wxT("AutoFillColumnPaste"), &bb, true);
	CheckBoxAutoFillColumnPaste->SetValue(bb);
	cfg->Read(wxT("LDoubleClickHighlight"), &bb, true);
	CheckBoxLDClickHighlight->SetValue(bb);
	cfg->Read(wxT("TypewriterMode"), &bb, false);
	CheckBoxTypewriterMode->SetValue(bb);
	cfg->Read(wxT("FixedWidthMode"), &bb, false);
	CheckBoxFixWidthMode->SetValue(bb);
	cfg->Read(wxT("IgnoreUndoWarn"), &bb, false);
	CheckBoxIgnoreUndoWrnMsg->SetValue(bb);
	extern bool g_DoNotSaveSettings;
	CheckBoxDoNotSaveSettings->SetValue(g_DoNotSaveSettings);
	cfg->Read(wxT("/Application/ReloadFiles"), &bb, true);
	CheckBoxReloadFiles->SetValue(bb);
	cfg->Read(wxT("/Application/PurgeHistory"), &bb, false);
	CheckBoxPurgeHistory->SetValue(bb);
	cfg->Read(wxT("RestoreCaretPos"), &bb, true);
	CheckBoxRestoreCaretPos->SetValue(bb);

	// New Document
	cfg->Read(wxT("NewDocumentLineEnding"), &m_NewDocLineEnding, nltDefault);
	switch (m_NewDocLineEnding)
	{
	case nltDefault:
		RadioButtonNewDocLineEndingCR->SetValue(false);
		RadioButtonNewDocLineEndingCRLF->SetValue(false);
		RadioButtonNewDocLineEndingDefault->SetValue(true);
		RadioButtonNewDocLineEndingLF->SetValue(false);
		break;
	case nltDOS:
		RadioButtonNewDocLineEndingCR->SetValue(false);
		RadioButtonNewDocLineEndingCRLF->SetValue(true);
		RadioButtonNewDocLineEndingDefault->SetValue(false);
		RadioButtonNewDocLineEndingLF->SetValue(false);
		break;
	case nltUNIX:
		RadioButtonNewDocLineEndingCR->SetValue(false);
		RadioButtonNewDocLineEndingCRLF->SetValue(false);
		RadioButtonNewDocLineEndingDefault->SetValue(false);
		RadioButtonNewDocLineEndingLF->SetValue(true);
		break;
	case nltMAC:
		RadioButtonNewDocLineEndingCR->SetValue(true);
		RadioButtonNewDocLineEndingCRLF->SetValue(false);
		RadioButtonNewDocLineEndingDefault->SetValue(false);
		RadioButtonNewDocLineEndingLF->SetValue(false);
		break;
	default:
		wxASSERT(0);
	}

	ss = wxGetTranslation(wxT("Plain Text"));
	cfg->Read(wxT("NewDocumentSyntax"), &ss);
	index = ComboBoxNewDocSyntax->FindString(ss);
	if (index == wxNOT_FOUND) index = 0;
	ComboBoxNewDocSyntax->SetSelection(index);

	m_NewDocEncoding = _("System Default");
	cfg->Read(wxT("NewDocumentEncoding"), &m_NewDocEncoding);
	wxFontEncoding enc = wxFontMapper::GetEncodingFromName(m_NewDocEncoding);
	index = ComboBoxNewDocEncOther->FindString(m_NewDocEncoding);
	if (wxFONTENCODING_MAX == enc || wxFONTENCODING_DEFAULT == enc || index == wxNOT_FOUND)
		enc = wxFONTENCODING_SYSTEM;

	if (wxFONTENCODING_UTF8 == enc)
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(true);
		ComboBoxNewDocEncOther->Enable(false);
		
		RadioButtonNewDocEncOther->SetValue(false);
		RadioButtonNewDocEncSystemDefault->SetValue(false);
		RadioButtonNewDocEncUTF8->SetValue(true);
	}
	else
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(false);
		RadioButtonNewDocEncUTF8->SetValue(false);
		if (wxFONTENCODING_SYSTEM == enc)
		{
			ComboBoxNewDocEncOther->Enable(false);
			RadioButtonNewDocEncOther->SetValue(false);
			RadioButtonNewDocEncSystemDefault->SetValue(true);
		}
		else
		{
			ComboBoxNewDocEncOther->Enable(true);
			ComboBoxNewDocEncOther->SetSelection(index);
			RadioButtonNewDocEncOther->SetValue(true);
			RadioButtonNewDocEncSystemDefault->SetValue(false);
		}
	}

	cfg->Read(wxT("NewDocumentEncodingUTF8WithBOM"), &bb, false);
	CheckBoxNewDocEncUTF8WithBOM->SetValue(bb);

#ifdef __WXMSW__
	fontname = wxString(wxT("Courier New"));
#elif defined(__APPLE__) && defined(__MACH__)
	fontname = wxString(wxT("Monaco"));
#else
	fontname = wxString(wxT("Monospace"));
#endif
	cfg->Read(wxT("NewDocumentTextFont"), &fontname);
	index = ComboBoxNewDocFont->FindString(fontname);
	if (index != wxNOT_FOUND) ComboBoxNewDocFont->SetSelection(index);

	cfg->Read(wxT("NewDocumentTextFontSize"), &ll, DEFAULT_FONT_SIZE);
	if (ll < 0) ll = DEFAULT_FONT_SIZE;
	if (ll > MAX_FONT_SIZE) ll = MAX_FONT_SIZE;
	EditNewDocFontSize->SetValue(wxString() << ll);

	// Print page
	cfg->Read(wxT("PrintSyntax"), &bb);
	CheckBoxPrintSyntax->SetValue(bb);
	cfg->Read(wxT("PrintLineNumber"), &bb);
	CheckBoxPrintLineNumber->SetValue(bb);
	cfg->Read(wxT("PrintBookmark"), &bb);
	CheckBoxPrintBookmark->SetValue(bb);
	cfg->Read(wxT("PrintEndOfLine"), &bb);
	CheckBoxPrintEndOfLine->SetValue(bb);
	cfg->Read(wxT("PrintTabChar"), &bb);
	CheckBoxPrintTabChar->SetValue(bb);
	cfg->Read(wxT("PrintSpaceChar"), &bb);
	CheckBoxPrintSpaceChar->SetValue(bb);
	cfg->Read(wxT("PrintOffsetHeader"), &ll);
	RadioBoxPrintOffset->SetSelection(ll);
	cfg->Read(wxT("PrintPageHeader"), &bb);
	CheckBoxPrintPageHeader->SetValue(bb);
	cfg->Read(wxT("PageHeaderLeft"), &ss);
	EditHeaderLeft->SetValue(ss);
	cfg->Read(wxT("PageHeaderCenter"), &ss);
	EditHeaderCenter->SetValue(ss);
	cfg->Read(wxT("PageHeaderRight"), &ss);
	EditHeaderRight->SetValue(ss);
	cfg->Read(wxT("PrintPageFooter"), &bb);
	CheckBoxPrintPageFooter->SetValue(bb);
	cfg->Read(wxT("PageFooterLeft"), &ss);
	EditFooterLeft->SetValue(ss);
	cfg->Read(wxT("PageFooterCenter"), &ss);
	EditFooterCenter->SetValue(ss);
	cfg->Read(wxT("PageFooterRight"), &ss);
	EditFooterRight->SetValue(ss);
	bb = SpellCheckerManager::Instance().GetEnablePersonalDictionary();
	CheckBoxPersonalDict->SetValue(bb);
	cfg->Read(wxT("/SpellChecker/AddKeywords"), &bb, true);
	CheckBoxAddKeywords->SetValue(bb);
	EditDictionaryDir->SetValue(SpellCheckerManager::Instance().GetDictionaryPath());
	InitDictionaryChoice();
	//cfg->Read(wxT("ThesPath"), &ss, dictDir);
	//EditThesauriDir->SetValue(ss);
	//cfg->Read(wxT("BitmPath"), &ss, dictDir);
	//EditBitMapDir->SetValue(ss);
	extern bool g_ResetAllKeys;
	CheckBoxResetAllKeys->SetValue(g_ResetAllKeys);
	ChangedTreeItemDataList.clear();
	// get all keys of each command and srore them in TreeItemDataList
	list<TreeItemData*>::iterator tidit = TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend = TreeItemDataList.end();

	do
	{
		CommandData *cd = (*tidit)->cmddata;
		(*tidit)->keys.Clear();
		MadEdit::ms_KeyBindings.GetKeys(cd->menu_id, cd->command, (*tidit)->keys);
	}
	while (++tidit != tiditend);

	g_SelectedCommandItem = nullptr;
	g_SelectedKeyId = -1;
	EditCommandHint->Clear();
	ListBoxKeys->Clear();
	UpdateKeyHint();
	wxTreeItemId selid = TreeCtrl1->GetSelection();

	if (selid.IsOk())
	{
		TreeCtrl1->Unselect();
		TreeCtrl1->SelectItem(selid, true);
	}

	cfg->SetPath(oldpath);
}

void MadOptionsDialog::ButtonOKClick(wxCommandEvent& WXUNUSED(event))
{
	long lo, ml;
	bool error = false;
	wxString errtext(_("Invalid value of \"%s(%s)\""));

	if (!EditMaxSizeToLoad->GetValue().ToLong(&lo) || lo < 0)
	{
		wxLogError(errtext + _(": Should be greater than zero"), StaticText4->GetLabel().c_str(), EditMaxSizeToLoad->GetValue().c_str());
		error = true;
	}

	if (!EditMaxTextFileSize->GetValue().ToLong(&lo) || lo < 0)
	{
		wxLogError(errtext + _(": Should be greater than zero"), StaticText5->GetLabel().c_str(), EditMaxTextFileSize->GetValue().c_str());
		error = true;
	}

#if PATCH_MAXLINELENGTH == 1
	if (!EditMaxLineLength->GetValue().ToLong(&lo) || lo < 80 || lo > 4096)
#else
	if (!EditMaxLineLength->GetValue().ToLong(&lo) || lo < 80)
#endif
	{
		wxLogError(errtext + _(": Should be 1~4096"), StaticText8->GetLabel().c_str(), EditMaxLineLength->GetValue().c_str());
		error = true;
	}

	if (!EditMaxColumns->GetValue().ToLong(&ml) || ml <= 0 || ml > 4096)
	{
		wxLogError(errtext + _(": Should be 1~4096"), StaticText9->GetLabel().c_str(), EditMaxColumns->GetValue().c_str());
		error = true;
	}

	if (!EditTabColumns->GetValue().ToLong(&lo) || lo <= 0 || lo > 256)
	{
		wxLogError(errtext + _(": Should be 1~256"), StaticText10->GetLabel().c_str(), EditTabColumns->GetValue().c_str());
		error = true;
	}

	if (!EditIndentColumns->GetValue().ToLong(&lo) || lo <= 0 || lo > 256)
	{
		wxLogError(errtext + _(": Should be 1~256"), StaticText11->GetLabel().c_str(), EditIndentColumns->GetValue().c_str());
		error = true;
	}

	if (!EditHintColumns->GetValue().ToLong(&lo) || lo <= 0 || lo > ml)
	{
		wxLogError(errtext + _(": Should be 1~") + EditMaxColumns->GetValue(), StaticText11->GetLabel().c_str(), EditHintColumns->GetValue().c_str());
		error = true;
	}

	if (!EditAutoSaveTimeout->GetValue().ToLong(&lo) || (lo < 10 || lo > 30))
	{
		wxLogError(errtext + _(": Should be 10~30"), StaticText7->GetLabel().c_str(), EditAutoSaveTimeout->GetValue().c_str());
		error = true;
	}

	if (!error) { EndModal(wxID_OK); }
}

void MadOptionsDialog::ButtonCancelClick(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}

void MadOptionsDialog::PrintMarkButtonClick(wxCommandEvent& event)
{
	ButtonID=event.GetId();
	PopupMenu(&PopupMenuPrintMark);
}

void MadOptionsDialog::PrintMarkClick(wxCommandEvent& event)
{
	wxString str = PopupMenuPrintMark.GetLabel(event.GetId());
	wxTextCtrl *edit = nullptr;
	if      (ButtonID == EditHeaderLeftId)   { edit = EditHeaderLeft; }
	else if (ButtonID == EditHeaderCenterId) { edit = EditHeaderCenter; }
	else if (ButtonID == EditHeaderRightId)  { edit = EditHeaderRight; }
	else if (ButtonID == EditFooterLeftId)   { edit = EditFooterLeft; }
	else if (ButtonID == EditFooterCenterId) { edit = EditFooterCenter; }
	else if (ButtonID == EditFooterRightId)  { edit = EditFooterRight; }
	else { ; }

	if (edit && str[0] == wxT('[') && str[3] == wxT(']'))
	{
		wxString text=edit->GetValue();
		edit->SetValue(text+ str.Mid(1, 2));
	}
	ButtonID=0;
}

void MadOptionsDialog::TreeCtrl1SelChanged(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	if (!id.IsOk()) return;

	g_SelectedCommandItem = dynamic_cast< TreeItemData* >(TreeCtrl1->GetItemData(id));
	g_SelectedKeyId=-1;

	if (g_SelectedCommandItem == nullptr)
	{
		ListBoxKeys->Clear();
		EditCommandHint->SetValue(_("Cannot assign key to this item"));
	}
	else
	{
		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		wxString hint=wxT('[');
		if (g_SelectedCommandItem->cmddata->menuid_name != 0)
		{
			hint+= wxString(g_SelectedCommandItem->cmddata->menuid_name);
		}
		if (g_SelectedCommandItem->cmddata->command > 0)
		{
			wxString text=MadKeyBindings::CommandToText(g_SelectedCommandItem->cmddata->command);
			if (!text.IsEmpty())
			{
				if (hint.Length()>1)
				{
					hint+= wxT(',');
				}
				hint+= text;
			}
		}
		hint+= wxT("] ");
		hint+= wxString(wxGetTranslation(g_SelectedCommandItem->cmddata->hint));

		EditCommandHint->SetValue(hint);
	}
}

void MadOptionsDialog::ListBoxKeysSelected(wxCommandEvent& event)
{
	g_SelectedKeyId=event.GetSelection();
}

// find key in TreeItemDataList
TreeItemData* MadOptionsDialog::FindKeyInList(const wxString &key)
{
	list<TreeItemData*>::iterator tidit=TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend=TreeItemDataList.end();

	wxString lkey = key.Lower();

	while (tidit!=tiditend)
	{
		TreeItemData *tid = (*tidit);
		if (!tid->keys.IsEmpty())
		{
			size_t count=tid->keys.GetCount();
			size_t idx=0;

			for (; idx < count; ++idx)
			{
				if (tid->keys[idx].Lower() == lkey)
				{
					return tid;
				}
			}
		}

		++tidit;
	}
	return nullptr;
}

bool MadOptionsDialog::FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist)
{
	list<TreeItemData*>::const_iterator tidit=tlist.begin();
	list<TreeItemData*>::const_iterator tiditend=tlist.end();

	while (tidit != tiditend)
	{
		if ((*tidit) == tid)
		{
			return true;
		}
		++tidit;
	}
	return false;
}

void MadOptionsDialog::UpdateKeyHint()
{
	wxString scstr=EditKey->GetValue();
	g_CommandItemOfNewKey=nullptr;

	if (scstr.IsEmpty())
	{
		g_OptionsDialog->EditKeyHint->Clear();
	}
	else
	{
		// find the key is assigned to which command
		TreeItemData *tid = g_OptionsDialog->FindKeyInList(scstr);
		if (!tid)
		{
			g_OptionsDialog->EditKeyHint->SetValue(_("This key is not assigned"));
		}
		else
		{
			g_CommandItemOfNewKey=tid;
			wxString cmd;

			if (tid->cmddata->menuid_name != 0)
			{
				cmd+= wxString(tid->cmddata->menuid_name);
			}
			if (tid->cmddata->command > 0)
			{
				wxString text=MadKeyBindings::CommandToText(tid->cmddata->command);
				if (!text.IsEmpty())
				{
					if (cmd.Length()>1)
					{
						cmd+= wxT(',');
					}
					cmd+= text;
				}
			}

			wxString str=wxString::Format(_("This key is assigned to [%s]"), cmd.c_str());
			g_OptionsDialog->EditKeyHint->SetValue(str);
		}
	}
}

void MadOptionsDialog::ButtonAddKeyClick(wxCommandEvent& WXUNUSED(event))
{
	if (g_SelectedCommandItem && g_SelectedCommandItem!=g_CommandItemOfNewKey)
	{
		wxString key=EditKey->GetValue();
		if (g_CommandItemOfNewKey) // new key is assigned to another command
		{
			/*
			wxMessageDialog dlg(this, key +wxT(": ") +EditKeyHint->GetValue() +wxT("\n\n") + wxString(_("Do you want to reassign this key?")),
				wxT("MadEdit"), wxYES_NO|wxICON_QUESTION);
			if (dlg.ShowModal()!=wxID_YES)
			{
				return;
			}
			*/

			//delete the key from g_CommandItemOfNewKey
			int idx=g_CommandItemOfNewKey->keys.Index(key.c_str(), false);
			wxASSERT(idx>=0);
			g_CommandItemOfNewKey->keys.RemoveAt(idx);
			if (FindItemInList(g_CommandItemOfNewKey, ChangedTreeItemDataList)==false)
			{
				ChangedTreeItemDataList.push_back(g_CommandItemOfNewKey);
			}
		}

		// add the key to g_SelectedCommandItem
		g_SelectedCommandItem->keys.Add(key);

		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		if (FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
		UpdateKeyHint();
	}
}

void MadOptionsDialog::ButtonDeleteKeyClick(wxCommandEvent& WXUNUSED(event))
{
	if (g_SelectedCommandItem && g_SelectedKeyId >= 0)
	{
		g_SelectedCommandItem->keys.RemoveAt(g_SelectedKeyId);

		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		if (FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
		UpdateKeyHint();
	}
}

void MadOptionsDialog::ButtonShowInMenuClick(wxCommandEvent& WXUNUSED(event))
{
	// move the selected key to first element of ListBoxKeys
	if (g_SelectedCommandItem && g_SelectedCommandItem->cmddata->menu_id>0 && g_SelectedKeyId > 0)
	{
		wxString key=g_SelectedCommandItem->keys[g_SelectedKeyId];

		g_SelectedCommandItem->keys.RemoveAt(g_SelectedKeyId);
		g_SelectedCommandItem->keys.Insert(key, 0);

		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		if (FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
	}
}

void MadOptionsDialog::ButtonDateTimeClick(wxCommandEvent& WXUNUSED(event))
{
	PopupMenu(&PopupMenuDateTimeMark);
}

void MadOptionsDialog::DateTimeMarkClick(wxCommandEvent& event)
{
	wxString str=PopupMenuDateTimeMark.GetLabel(event.GetId());
	int pos=str.Find(wxT(']'));
	if (pos != wxNOT_FOUND && str[0]==wxT('['))
	{
		str = str.Mid(1, pos-1);
	}

	wxString text=EditDateTime->GetValue();
	EditDateTime->SetValue(text+str);
}

#if ENABLE_BITMAP_THUARI
void MadOptionsDialog::ButtonBitmapDirClick(wxCommandEvent& WXUNUSED(event))
{
}
void MadOptionsDialog::ButtonThesauriDirClick(wxCommandEvent& WXUNUSED(event))
{
	wxString defdir = SpellCheckerManager::Instance().GetThesaurusPath();

	if (defdir.IsEmpty()) { defdir = wxGetCwd(); }

	wxDirDialog dlg(this, _("Choose a directory"), defdir);

	if (dlg.ShowModal() == wxID_OK)
	{
		EditThesauriDir->SetValue(dlg.GetPath());
		SpellCheckerManager::Instance().SetThesaurusPath(dlg.GetPath());
		InitDictionaryChoice();
	}
}
#endif

void MadOptionsDialog::ButtonDictionaryDirClick(wxCommandEvent& WXUNUSED(event))
{
	wxString defdir = SpellCheckerManager::Instance().GetDictionaryPath();

	if (defdir.IsEmpty()) { defdir = wxGetCwd(); }

	wxDirDialog dlg(this, _("Choose a directory"), defdir);

	if (dlg.ShowModal() == wxID_OK)
	{
		EditDictionaryDir->SetValue(dlg.GetPath());
		InitDictionaryChoice(dlg.GetPath());
	}
}

void MadOptionsDialog::InitDictionaryChoice(const wxString &path/* = wxEmptyString*/)
{
	if (!path.IsEmpty())
	{
		SpellCheckerManager::Instance().SetDictionaryPath(path);
		SpellCheckerManager::Instance().ScanForDictionaries();
	}

	std::vector<wxString> dics = SpellCheckerManager::Instance().GetPossibleDictionaries();
	int sel = SpellCheckerManager::Instance().GetSelectedDictionaryNumber();
	ComboDictionary->Clear();

	for (unsigned int i = 0 ; i < dics.size(); i++)
	{ ComboDictionary->Append(SpellCheckerManager::Instance().GetLanguageName(dics[i])); }

	if (sel != -1)
	{ ComboDictionary->SetSelection(sel); }
}

void MadOptionsDialog::EditDictionaryDirTextEnter(wxCommandEvent& WXUNUSED(event))
{
	wxString path = EditDictionaryDir->GetValue();

	if (wxDir::Exists(path))
	{
		InitDictionaryChoice(path);
	}
	else
	{
		ComboDictionary->Clear();
	}
}

void MadOptionsDialog::ComboDictionarySelected(wxCommandEvent& WXUNUSED(event))
{
	wxString dictDesc = ComboDictionary->GetString(ComboDictionary->GetSelection());
	wxString dictName = SpellCheckerManager::Instance().GetDictionaryName(dictDesc);

	if (!dictName.IsEmpty())
	{
		SpellCheckerManager::Instance().SetDictionaryName(dictName);
	}
}

void MadOptionsDialog::CheckBoxMouseSelectToCopyClick(wxCommandEvent& WXUNUSED(event))
{
	CheckBoxCtrlWithMouseToSelect->Enable(CheckBoxMouseSelectToCopy->GetValue());
}

void MadOptionsDialog::CheckBoxAutoCompletePairClick(wxCommandEvent& WXUNUSED(event))
{
	CheckBoxInsertPairForSelection->Enable(CheckBoxAutoCompletePair->GetValue());
}

void MadOptionsDialog::EnableAutoSaveClick(wxCommandEvent& WXUNUSED(event))
{
	EditAutoSaveTimeout->Enable(CheckBoxEnableAutoSave->GetValue());
}

#ifdef MADEDIT_ENABLE_STC
void MadOptionsDialog::OnMarginClick(wxStyledTextEvent &event)
{
	if (event.GetMargin() == MARGIN_FOLD)
	{
		int lineClick = TextSample->LineFromPosition(event.GetPosition());
		int levelClick = TextSample->GetFoldLevel(lineClick);
		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
		{
			TextSample->ToggleFold(lineClick);
		}
	}
}
#endif

void MadOptionsDialog::RadioButtonNewDocEncSelect(wxCommandEvent& event)
{
	if (RadioButtonNewDocEncUTF8->GetId() == event.GetId())
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(true);
		ComboBoxNewDocEncOther->Enable(false);
		m_NewDocEncoding = wxFontMapper::GetEncodingName(wxFONTENCODING_UTF8);
	}
	else
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(false);
		if (RadioButtonNewDocEncOther->GetId() == event.GetId())
		{
			ComboBoxNewDocEncOther->Enable(true);
			if (ComboBoxNewDocEncOther->GetSelection() == wxNOT_FOUND && ComboBoxNewDocEncOther->GetCount() > 0)
			{
				ComboBoxNewDocEncOther->SetSelection(0);
			}
			m_NewDocEncoding = ComboBoxNewDocEncOther->GetString(ComboBoxNewDocEncOther->GetSelection());
		}
		else
		{
			ComboBoxNewDocEncOther->Enable(false);
			m_NewDocEncoding = _("System Default");
		}
	}
}

void MadOptionsDialog::RadioButtonNewDocLineEndingSelect(wxCommandEvent& event)
{
	if (event.GetId() == RadioButtonNewDocLineEndingCRLF->GetId())
	{
		m_NewDocLineEnding = nltDOS;
	}
	else if (event.GetId() == RadioButtonNewDocLineEndingLF->GetId())
	{
		m_NewDocLineEnding = nltUNIX;
	}
	else if (event.GetId() == RadioButtonNewDocLineEndingCR->GetId())
	{
		m_NewDocLineEnding = nltMAC;
	}
	else
	{
		m_NewDocLineEnding = nltDefault;
	}
}
