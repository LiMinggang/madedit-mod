///////////////////////////////////////////////////////////////////////////////
// Name:		MadMacroDlg.cpp
// Description:
// Author:		minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#include <wx/fileconf.h>
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "EmbeddedPython.hpp"
#include "MadMacroDlg.h"
#include "MadNumberDlg.h"
#include "MadSortDialog.h"
//(*InternalHeaders(MadMacroDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MadMacroDlg)
//*)
MadMacroDlg::wxCmdEvtHandlerMap_t MadMacroDlg::m_menu_evt_map[] =
{
	// edit
	{ menuUndo, &MadMacroDlg::OnEditUndo },
	{ menuRedo, &MadMacroDlg::OnEditRedo },
	{ menuCut, &MadMacroDlg::OnEditCut },
	{ menuCopy, &MadMacroDlg::OnEditCopy },
	{ menuPaste, &MadMacroDlg::OnEditPaste },
	{ menuDelete, &MadMacroDlg::OnEditDelete },
	{ menuCutLine, &MadMacroDlg::OnEditCutLine },
	{ menuDeleteLine, &MadMacroDlg::OnEditDeleteLine },
	{ menuSelectAll, &MadMacroDlg::OnEditSelectAll },
	{ menuStartEndSelction, &MadMacroDlg::OnEditStartEndSelction },
	{ menuInsertTabChar, &MadMacroDlg::OnEditInsertTabChar },
	{ menuInsertDateTime, &MadMacroDlg::OnEditInsertDateTime },
	{ menuSortAscending, &MadMacroDlg::OnEditSortAscending },
	{ menuSortDescending, &MadMacroDlg::OnEditSortDescending },
	{ menuSortAscendingCase, &MadMacroDlg::OnEditSortAscendingCase },
	{ menuSortDescendingCase, &MadMacroDlg::OnEditSortDescendingCase },
	{ menuSortByOptions, &MadMacroDlg::OnEditSortByOptions },
	{ menuSortOptions, &MadMacroDlg::OnEditSortOptions },
	{ menuCopyAsHexString, &MadMacroDlg::OnEditCopyAsHexString },
	{ menuCopyAsHexStringWithSpace, &MadMacroDlg::OnEditCopyAsHexStringWithSpace },
	{ menuCopyRevertHex, &MadMacroDlg::OnEditCopyRevertHex },
	{ menuIncreaseIndent, &MadMacroDlg::OnEditIncIndent },
	{ menuDecreaseIndent, &MadMacroDlg::OnEditDecIndent },
	{ menuComment, &MadMacroDlg::OnEditComment },
	{ menuUncomment, &MadMacroDlg::OnEditUncomment },
	{ menuWordWrapToNewLine, &MadMacroDlg::OnEditWordWrapToNewLine },
	{ menuNewLineToWordWrap, &MadMacroDlg::OnEditNewLineToWordWrap },
	{ menuToUpperCase, &MadMacroDlg::OnEditToUpperCase },
	{ menuToLowerCase, &MadMacroDlg::OnEditToLowerCase },
	{ menuInvertCase, &MadMacroDlg::OnEditInvertCase },
	{ menuCapitalize, &MadMacroDlg::OnEditCapitalize },
	{ menuToHalfWidth, &MadMacroDlg::OnEditToHalfWidth },
	{ menuToHalfWidthByOptions, &MadMacroDlg::OnEditToHalfWidthByOptions },
	{ menuToFullWidth, &MadMacroDlg::OnEditToFullWidth },
	{ menuToFullWidthByOptions, &MadMacroDlg::OnEditToFullWidthByOptions },
	{ menuTabToSpace, &MadMacroDlg::OnEditTabToSpace },
	{ menuSpaceToTab, &MadMacroDlg::OnEditSpaceToTab },
	{ menuTrimTrailingSpaces, &MadMacroDlg::OnEditTrimTrailingSpaces },
	{ menuTrimLeadingSpaces, &MadMacroDlg::OnEditTrimLeadingSpaces },
	{ menuDeleteEmptyLines, &MadMacroDlg::OnEditDeleteEmptyLines },
	{ menuDeleteEmptyLinesWithSpaces, &MadMacroDlg::OnEditDeleteEmptyLinesWithSpaces },
	{ menuJoinLines, &MadMacroDlg::OnEditJoinLines },
	{ menuInsertNumbers, &MadMacroDlg::OnEditInsertNumbers },
	{ menuColumnAlignLeft, &MadMacroDlg::OnEditColumnAlignLeft },
	{ menuColumnAlignRight, &MadMacroDlg::OnEditColumnAlignRight },
};

MadMacroDlg::wxCmdEvtHandlerRangeMap_t MadMacroDlg::m_menu_evt_range_map[] =
{
	{ menuSpellOption1, menuSpellOption99, &MadMacroDlg::OnEditSpellCheck },
	{ menuMadScrip1, menuMadScrip200, &MadMacroDlg::OnToolsMadScriptList },
};

extern EmbeddedPython *g_EmbeddedPython;
extern MadEditFrame *g_MainFrame;
extern wxArrayString g_SpellSuggestions;
extern wxString g_MadEditAppDir;
extern wxString g_MadEditHomeDir;
extern wxMenu *g_Menu_MadMacro_Scripts;
extern MadEdit *g_ActiveMadEdit;
extern MadEdit *g_CurrentMadEdit;
extern void OnEditMouseRightUp( MadEdit * madedit );
extern wxMenu *g_Menu_EditPop;
extern MadNumberDlg * g_MadNumberDlg;
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
	m_Pymacro = new MadEdit(this, wxID_ANY, wxPoint(0, 0), pymacro);
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

	for(size_t i = 0; i < sizeof(m_menu_evt_map)/sizeof(m_menu_evt_map[0]); ++i)
	{
		Bind( wxEVT_MENU, m_menu_evt_map[i].method, this, m_menu_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_menu_evt_range_map)/sizeof(m_menu_evt_range_map[0]); ++i)
	{
		Bind( wxEVT_MENU, m_menu_evt_range_map[i].method, this, m_menu_evt_range_map[i].evtStartTag, m_menu_evt_range_map[i].evtEndTag );
	}

	m_Pymacro->SetOnMouseRightUp( &OnEditMouseRightUp );
	m_Pymacro->SetRightClickMenu( g_Menu_EditPop );
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
					MadMessageBox(_("Memory allocation failed"), _("Error"),  wxOK|wxICON_ERROR);
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
	m_debug = !m_debug;
	WxMemoOutput->Show(m_debug);
	//GetSizer()->Fit(this);
	//GetSizer()->SetSizeHints(this);
	Layout();
	if(m_debug)
		WxButtonResult->SetLabel(_("Results <<"));
	else
		WxButtonResult->SetLabel(_("Results >>"));
}

void MadMacroDlg::MadMacroDlgClose(wxCloseEvent& WXUNUSED(event))
{
	m_PyacroContext.Empty();
	m_Pymacro->GetText(m_PyacroContext);
	if(g_CurrentMadEdit == m_Pymacro) g_CurrentMadEdit = nullptr;
	Destroy();
	g_MadMacroDlg = nullptr;
}

void MadMacroDlg::OnButtonResetClick(wxCommandEvent& WXUNUSED(event))
{
	wxString endline(wxT("\r"));
	if (m_Pymacro->GetInsertNewLineType() == nltDOS) endline += wxT("\n");
	else if (m_Pymacro->GetInsertNewLineType() == nltUNIX) endline = wxT("\n");
	m_PyacroContext = (wxString(wxT("#Create MadEdit Object for the active edit")) + endline + wxT("medit = MadEdit()") + endline + endline);
}

void MadMacroDlg::OnEditUndo( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && ( !m_Pymacro->IsReadOnly() ) )
	{
		m_Pymacro->Undo();
	}
	
	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditRedo( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && ( !m_Pymacro->IsReadOnly() ) )
	{
		m_Pymacro->Redo();
	}
	
	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCut( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CutToClipboard();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCopy( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CopyToClipboard();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditPaste( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->PasteFromClipboard();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditDelete( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->Delete();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCutLine( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CutLine();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditDeleteLine( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->DeleteLine();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSelectAll( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->SelectAll();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditStartEndSelction( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->StartEndSelction();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditInsertTabChar( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->InsertTabChar();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditInsertDateTime( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->InsertDateTime();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSortAscending( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		int begin, end;
		m_Pymacro->GetSelectionLineId( begin, end );
		m_Pymacro->SortLines( sfAscending, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSortDescending( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		int begin, end;
		m_Pymacro->GetSelectionLineId( begin, end );
		m_Pymacro->SortLines( sfDescending, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSortAscendingCase( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		int begin, end;
		m_Pymacro->GetSelectionLineId( begin, end );
		m_Pymacro->SortLines( sfAscending | sfCaseSensitive, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSortDescendingCase( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		int begin, end;
		m_Pymacro->GetSelectionLineId( begin, end );
		m_Pymacro->SortLines( sfDescending | sfCaseSensitive, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSortByOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		wxConfigBase *m_Config = wxConfigBase::Get(false);
		wxString oldpath = m_Config->GetPath();
		m_Config->SetPath( wxT( "/MadEdit" ) );
		int order;
		bool cs, num, rem;
		m_Config->Read( wxT( "SortOrder" ), &order, sfAscending );
		m_Config->Read( wxT( "SortCaseSensitive" ), &cs, false );
		m_Config->Read( wxT( "SortNumeric" ), &num, false );
		m_Config->Read( wxT( "SortRemoveDup" ), &rem, false );
		m_Config->SetPath( oldpath );
		MadSortFlags flags = order |
							 ( cs ? sfCaseSensitive : 0 ) |
							 ( num ? sfNumericSort : 0 ) |
							 ( rem ? sfRemoveDuplicate : 0 ) ;
		int begin, end;
		m_Pymacro->GetSelectionLineId( begin, end );
		m_Pymacro->SortLines( flags, begin, end );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSortOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro == nullptr || m_Pymacro->GetEditMode() == emHexMode )
	{
		g_CurrentMadEdit = nullptr;
		return;
	}
	wxConfigBase *m_Config = wxConfigBase::Get(false);
	MadSortDialog dialog( this );
	wxString oldpath = m_Config->GetPath();
	m_Config->SetPath( wxT( "/MadEdit" ) );
	int order;
	bool cs, num, rem;
	m_Config->Read( wxT( "SortOrder" ), &order, sfAscending );
	dialog.WxRadioBoxOrder->SetSelection( order );
	m_Config->Read( wxT( "SortCaseSensitive" ), &cs, false );
	dialog.WxCheckBoxCase->SetValue( cs );
	m_Config->Read( wxT( "SortNumeric" ), &num, false );
	dialog.WxCheckBoxNumeric->SetValue( num );
	m_Config->Read( wxT( "SortRemoveDup" ), &rem, false );
	dialog.WxCheckBoxRemoveDup->SetValue( rem );
	// Hide Modaless Dialog
	//HideModalessDialogs();

	if( dialog.ShowModal() == wxID_OK )
	{
		order = dialog.WxRadioBoxOrder->GetSelection();
		cs = dialog.WxCheckBoxCase->GetValue();
		num = dialog.WxCheckBoxNumeric->GetValue();
		rem = dialog.WxCheckBoxRemoveDup->GetValue();
		m_Config->Write( wxT( "SortOrder" ), order );
		m_Config->Write( wxT( "SortCaseSensitive" ), cs );
		m_Config->Write( wxT( "SortNumeric" ), num );
		m_Config->Write( wxT( "SortRemoveDup" ), rem );
		int flags = order |
					( cs ? sfCaseSensitive : 0 ) |
					( num ? sfNumericSort : 0 ) |
					( rem ? sfRemoveDuplicate : 0 ) ;
		int begin, end;
		m_Pymacro->GetSelectionLineId( begin, end );
		m_Pymacro->SortLines( flags, begin, end );
	}

	m_Config->SetPath( oldpath );

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCopyAsHexString( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CopyAsHexString( false );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCopyAsHexStringWithSpace( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CopyAsHexString( true );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCopyRevertHex( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		static wxString strDelimiters = wxT( " .,?!@#\t$%^&*()-=_+[]{}\\|;:\"'`<>/~" );
		wxString str = wxGetTextFromUser( _( "Delimiters:" ), _( "Revert Hex String" ), strDelimiters );

		if( !str.IsEmpty() )
		{
			strDelimiters = str;
		}

		m_Pymacro->CopyRevertHex( str );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditIncIndent( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->IncreaseDecreaseIndent( true );
	}

	g_CurrentMadEdit = nullptr;
}
void MadMacroDlg::OnEditDecIndent( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->IncreaseDecreaseIndent( false );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditComment( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CommentUncomment( true );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditUncomment( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->CommentUncomment( false );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditWordWrapToNewLine( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ConvertWordWrapToNewLine();
	}

	g_CurrentMadEdit = nullptr;
}
void MadMacroDlg::OnEditNewLineToWordWrap( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ConvertNewLineToWordWrap();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditToUpperCase( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ToUpperCase();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditToLowerCase( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ToLowerCase();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditInvertCase( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->InvertCase();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditCapitalize( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->Capitalize();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditToHalfWidth( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ToHalfWidth();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditToHalfWidthByOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro == nullptr )
	{
		g_CurrentMadEdit = nullptr;
		return;
	}

	static wxArrayInt selections;
	static bool inited = false;

	if( !inited )
	{
		selections.Add( 0 );
		selections.Add( 1 );
		selections.Add( 2 );
		selections.Add( 3 );
		inited = true;
	}

	wxString choices[4] = { _( "ASCII characters" ), _( "Japanese characters" ),
							_( "Korean characters" ), _( "other characters" )
						  };
#if (wxMAJOR_VERSION == 2)
	size_t sels = wxGetSelectedChoices( selections,
										_( "Choose the characters you want to convert:" ), _( "To Halfwidth by Options..." ),
										4, choices, this );
#else
	int sels = wxGetSelectedChoices( selections,
									 _( "Choose the characters you want to convert:" ), _( "To Halfwidth by Options..." ),
									 4, choices, this );
#endif

	if( sels > 0 )
	{
		bool ascii = false, japanese = false, korean = false, other = false;

		for( size_t i = 0; i < (size_t)sels; ++i )
		{
			switch( selections[i] )
			{
			case 0: ascii = true; break;

			case 1: japanese = true; break;

			case 2: korean = true; break;

			case 3: other = true; break;
			}
		}

		m_Pymacro->ToHalfWidth( ascii, japanese, korean, other );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditToFullWidth( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ToFullWidth();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditToFullWidthByOptions( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro == nullptr )
	{
		g_CurrentMadEdit = nullptr;
		return;
	}

	static wxArrayInt selections;
	static bool inited = false;

	if( !inited )
	{
		selections.Add( 0 );
		selections.Add( 1 );
		selections.Add( 2 );
		selections.Add( 3 );
		inited = true;
	}

	wxString choices[4] = { _( "ASCII characters" ), _( "Japanese characters" ),
							_( "Korean characters" ), _( "other characters" )
						  };
#if (wxMAJOR_VERSION == 2)
	size_t sels = wxGetSelectedChoices( selections,
										_( "Choose the characters you want to convert:" ), _( "To Fullwidth by Options..." ),
										4, choices, this );
#else
	int sels = wxGetSelectedChoices( selections,
									 _( "Choose the characters you want to convert:" ), _( "To Fullwidth by Options..." ),
									 4, choices, this );
#endif

	if( sels > 0 )
	{
		bool ascii = false, japanese = false, korean = false, other = false;

		for( size_t i = 0; i < (size_t)sels; ++i )
		{
			switch( selections[i] )
			{
			case 0: ascii = true; break;

			case 1: japanese = true; break;

			case 2: korean = true; break;

			case 3: other = true; break;
			}
		}

		m_Pymacro->ToFullWidth( ascii, japanese, korean, other );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditTabToSpace( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ConvertTabToSpace();
	}

	g_CurrentMadEdit = nullptr;
}
void MadMacroDlg::OnEditSpaceToTab( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro )
	{
		m_Pymacro->ConvertSpaceToTab();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditTrimTrailingSpaces( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->TrimTrailingSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditTrimLeadingSpaces( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->TrimLeadingSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditDeleteEmptyLines( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->DeleteEmptyLines();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditDeleteEmptyLinesWithSpaces( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->DeleteEmptyLinesWithSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditJoinLines( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->JoinLines();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditInsertNumbers( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() == emColumnMode )
	{
		// Hide Modaless Dialog
		//HideModalessDialogs();

		if( g_MadNumberDlg == nullptr ) { g_MadNumberDlg = new MadNumberDlg( this ); }

		if( g_MadNumberDlg->ShowModal() == wxID_OK )
		{
			MadNumberingStepType numStepType = mnstLinear;
			MadNumberFormat numFormat = nfDEC;
			MadNumberAlign numAlign = naLeft;
			int sel = g_MadNumberDlg->WxChoiceNumberStepType->GetSelection();

			switch( sel )
			{
			case 1: numStepType = mnstExponential; break;

			default: break;
			}

			sel = g_MadNumberDlg->WxChoiceFormat->GetSelection();

			switch( sel )
			{
			case 1:
				{
					numFormat = nfHEX;
				}
				break;

			case 2:
				{
					numFormat = nfBIN;
				}
				break;

			case 3:
				{
					numFormat = nfOCT;
				}
				break;

			default:
				break;
			}

			sel = g_MadNumberDlg->WxChoiceAlign->GetSelection();

			switch( sel )
			{
			case 1:
				{
					numAlign = naRight;
				}
				break;

			default:
				break;
			}

			long initialNum = 0, numStep = 0, totalChar = 0;
			g_MadNumberDlg->WxEditNumberOfChars->GetValue().ToLong( &totalChar );
			g_MadNumberDlg->WxEditNumberingStep->GetValue().ToLong( &numStep );
			g_MadNumberDlg->WxEditInitialNumber->GetValue().ToLong( &initialNum );
			wxString prefix, postfix;

			if( g_MadNumberDlg->WxCheckPrefix->GetValue() )
			{ prefix = g_MadNumberDlg->WxEditPrefix->GetValue(); }

			if( g_MadNumberDlg->WxCheckPostfix->GetValue() )
			{ postfix = g_MadNumberDlg->WxEditPostfix->GetValue(); }

			m_Pymacro->InsertIncrementalNumber( initialNum, numStep, totalChar, numStepType, numFormat, numAlign, g_MadNumberDlg->WxPadChar->GetValue(), prefix, postfix );
			m_Pymacro->Refresh( false );
		}
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditColumnAlignLeft( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->ColumnAlignLeft();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditColumnAlignRight( wxCommandEvent& WXUNUSED(event) )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->ColumnAlignRight();
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnEditSpellCheck( wxCommandEvent& event )
{
	if( m_Pymacro && m_Pymacro->GetEditMode() != emHexMode )
	{
		m_Pymacro->ReplaceWordFromCaretPos( g_SpellSuggestions[event.GetId() - menuSpellOption1] );
	}

	g_CurrentMadEdit = nullptr;
}

void MadMacroDlg::OnToolsMadScriptList( wxCommandEvent& event )
{
	if( m_Pymacro != nullptr )
	{ 
		wxString scriptdir = g_MadEditAppDir + wxT( "scripts" ) + wxFILE_SEP_PATH;
		int menuId = event.GetId();
		wxString filename = g_Menu_MadMacro_Scripts->GetLabelText( menuId ) + wxT( ".mpy" );
		wxString scripfile = scriptdir + filename;
		if(!wxFileExists(scripfile)) scripfile = g_MadEditHomeDir + wxT( "scripts" ) + wxFILE_SEP_PATH + filename;
		if(!wxFileExists(scripfile)) 
		{
			scripfile = 
#if defined (DATA_DIR)
				wxT( DATA_DIR"/madedit-mod/scripts/" ) +
#else
				wxT( "/usr/share/madedit-mod/scripts/" ) +
#endif
			filename;
		}

		wxTextFile scriptfile( scripfile );
		scriptfile.Open( wxConvFile );

		if( scriptfile.IsOpened() )
		{
			if( !g_EmbeddedPython )
			{
				try
				{
					g_EmbeddedPython = new EmbeddedPython();
				}
				catch( std::bad_alloc & )
				{
					MadMessageBox( _( "Memory allocation failed" ), _( "Error" ),  wxOK | wxICON_ERROR );
				}
			}

			if( g_EmbeddedPython )
			{
				wxString str = scriptfile.GetFirstLine() + wxT( "\n" );

				for( ; !scriptfile.Eof(); )
				{
					str << scriptfile.GetNextLine() << wxT( "\n" );
				}

				if( str.IsNull() == false )
				{
					g_MainFrame->SetMacroRunning();
					g_EmbeddedPython->exec( std::string((const char *)(str.ToUTF8().data())) );
					g_MainFrame->SetMacroStopped();
				}
			}

			scriptfile.Close();
		}
	}

	g_CurrentMadEdit = nullptr;
}

