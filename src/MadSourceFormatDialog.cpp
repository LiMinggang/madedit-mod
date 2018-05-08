#include "MadSourceFormatDialog.h"
#include "MadEdit/MadEdit.h"
#include "astyle/astyle.h"
#include "astylepredefinedstyles.h"
#include <wx/fileconf.h>

//(*InternalHeaders(MadSourceFormatDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MadSourceFormatDialog)
//*)

static wxString bracket_style[aspsCustom + 1] =
{
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isBar)\n    {\n        bar();\n        return 1;\n    }\n    else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n    } else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isBar) {\n        bar();\n        return 1;\n    } else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n     if (isBar) {\n          bar();\n          return 1;\n     } else\n          return 0;\n}" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n    {\n    if (isBar)\n        {\n        bar();\n        return 1;\n        }\n    else\n        return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n    {\n        if (isBar)\n            {\n            bar();\n            return 1;\n            }\n        else\n            return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n        }\n    else\n        return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n  if (isBar)\n    {\n      bar();\n      return 1;\n    }\n  else\n    return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n        if (isBar) {\n                bar();\n                return 1;\n        } else\n                return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1;\n   }\n   else\n      return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isFoo) {\n        bar();\n        return 1;\n    } else {\n        return 0;\n    }\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n    }\n    else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo( bool isBar )\n{\n    if( isBar ) {\n        bar();\n        return 1;\n    }\n    else\n    { return 0; }\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo( bool isBar )\n{\n    if( isBar ) {\n        bar();\n        return 1;\n    }\n    else\n    { return 0; }\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1; }\n    else\n      return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
};

MadSourceFormatDialog::MadSourceFormatDialog(wxWindow* parent,wxWindowID WXUNUSED(id),const wxPoint& WXUNUSED(pos),const wxSize& WXUNUSED(size))
{
	//(*Initialize(MadSourceFormatDialog)
	wxAuiNotebook* AuiNotebook1;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer3;
	wxGridSizer* GridSizer1;
	wxGridSizer* GridSizer2;
	wxGridSizer* GridSizer3;
	wxGridSizer* GridSizer4;
	wxPanel* Panel1;
	wxPanel* Panel2;
	wxPanel* Panel3;
	wxPanel* Panel4;
	wxPanel* Panel5;
	wxPanel* Panel6;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticText* StaticText10;
	wxStaticText* StaticText1;
	wxStaticText* StaticText2;
	wxStaticText* StaticText3;
	wxStaticText* StaticText4;
	wxStaticText* StaticText5;
	wxStaticText* StaticText6;
	wxStaticText* StaticText7;
	wxStaticText* StaticText8;
	wxStaticText* StaticText9;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _("Source Formatter"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(800,500));
	SetMaxSize(wxSize(-1,-1));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	AuiNotebook1 = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(800,500), wxAUI_NB_TOP);
	Panel1 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(792,490), 0, _T("wxID_ANY"));
	GridSizer1 = new wxGridSizer(0, 2, 0, 0);
	wxString __wxRadioBoxChoices_1[17] =
	{
		_T("Allman(ANSI)"),
		_T("Java"),
		_T("Kr"),
		_T("Stroustrup"),
		_T("Whitesmith"),
		_T("VTK"),
		_T("Ratliff"),
		_T("Gnu"),
		_T("Linux"),
		_T("Horstmann"),
		_T("1TBS"),
		_T("Google"),
		_T("Mozilla"),
		_T("Webkit"),
		_T("Pico"),
		_T("Lisp"),
		_T("Custom")
	};
	RadioBoxBracketStyle = new wxRadioBox(Panel1, wxID_ANY, _("Bracket Style"), wxDefaultPosition, wxDefaultSize, 17, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("wxID_ANY"));
	GridSizer1->Add(RadioBoxBracketStyle, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel1, _("Sample"));
	StaticText1 = new wxStaticText(Panel1, wxID_ANY, _("Sample of the bracket style option chosen to use:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	StaticBoxSizer1->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
#ifdef MADEDIT_USE_MADEDIT
	TextSample = new MadEdit(Panel1, wxID_ANY, wxPoint(19, 47), wxSize(300, 300));
	TextSample->SetStorePropertiesToGlobalConfig(false);
	TextSample->SetEncoding(wxT("UTF-32LE"));
	TextSample->SetFixedWidthMode(true);
	TextSample->SetRecordCaretMovements(false);
	TextSample->SetInsertSpacesInsteadOfTab(false);
	TextSample->SetWantTab(false);
	TextSample->SetMaxLineLength(DEFAULT_MAX_LINELEN);
	TextSample->SetReadOnly(true);
	TextSample->SetDisplayBookmark(false);
	TextSample->SetShowSpaceChar(false);
	TextSample->SetShowTabChar(false);
	TextSample->SetShowEndOfLine(false);
	TextSample->SetSpellCheck(false);
	TextSample->SetSyntax(wxT("C/C++"));
	TextSample->SetText(bracket_style[aspsAllman]);
#elif MADEDIT_USE_STC
	TextSample = new wxStyledTextCtrl( Panel1, wxID_ANY, wxPoint( 19, 47 ), wxSize( 219, 300 ) );
	TextSample->StyleClearAll();
	TextSample->SetLexer( wxSTC_LEX_CPP );
	TextSample->SetMarginWidth( MARGIN_LINE_NUMBERS, 50 );
	TextSample->StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( 75, 75, 75 ) );
	TextSample->StyleSetBackground( wxSTC_STYLE_LINENUMBER, wxColour( 220, 220, 220 ) );
	TextSample->SetMarginType( MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER );
	// ---- Enable code folding
	TextSample->SetMarginType( MARGIN_FOLD, wxSTC_MARGIN_SYMBOL );
	TextSample->SetMarginWidth( MARGIN_FOLD, 15 );
	TextSample->SetMarginMask( MARGIN_FOLD, wxSTC_MASK_FOLDERS );
	TextSample->StyleSetBackground( MARGIN_FOLD, wxColor( 200, 200, 200 ) );
	TextSample->SetMarginSensitive( MARGIN_FOLD, true );
	// Properties found from http://www.scintilla.org/SciTEDoc.html
	TextSample->SetProperty( wxT( "fold" ),         wxT( "1" ) );
	TextSample->SetProperty( wxT( "fold.comment" ), wxT( "1" ) );
	TextSample->SetProperty( wxT( "fold.compact" ), wxT( "1" ) );
	wxColor grey( 100, 100, 100 );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, grey );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERSUB, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERSUB, grey );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_ARROW );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, _T( "WHITE" ) );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, _T( "WHITE" ) );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	TextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY );
	TextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	TextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	// ---- End of code folding part
	TextSample->SetWrapMode( wxSTC_WRAP_WORD ); // other choice is wxSCI_WRAP_NONE
	TextSample->SetText( bracket_style[aspsAllman] );
	TextSample->StyleSetForeground( wxSTC_C_STRING,            wxColour( 150, 0, 0 ) );
	TextSample->StyleSetForeground( wxSTC_C_PREPROCESSOR,      wxColour( 165, 105, 0 ) );
	TextSample->StyleSetForeground( wxSTC_C_IDENTIFIER,        wxColour( 40, 0, 60 ) );
	TextSample->StyleSetForeground( wxSTC_C_NUMBER,            wxColour( 0, 150, 0 ) );
	TextSample->StyleSetForeground( wxSTC_C_CHARACTER,         wxColour( 150, 0, 0 ) );
	TextSample->StyleSetForeground( wxSTC_C_WORD,              wxColour( 0, 0, 150 ) );
	TextSample->StyleSetForeground( wxSTC_C_WORD2,             wxColour( 0, 150, 0 ) );
	TextSample->StyleSetForeground( wxSTC_C_COMMENT,           wxColour( 150, 150, 150 ) );
	TextSample->StyleSetForeground( wxSTC_C_COMMENTLINE,       wxColour( 150, 150, 150 ) );
	TextSample->StyleSetForeground( wxSTC_C_COMMENTDOC,        wxColour( 150, 150, 150 ) );
	TextSample->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORD, wxColour( 0, 0, 200 ) );
	TextSample->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour( 0, 0, 200 ) );
	TextSample->StyleSetBold( wxSTC_C_WORD, true );
	TextSample->StyleSetBold( wxSTC_C_WORD2, true );
	TextSample->StyleSetBold( wxSTC_C_COMMENTDOCKEYWORD, true );
	// a sample list of keywords, I haven't included them all to keep it short...
	TextSample->SetKeyWords( 0, wxT( "if else return for while break continue" ) );
	TextSample->SetKeyWords( 1, wxT( "const int float void char double bool" ) );
	TextSample->SetReadOnly( true );
	TextSample->Bind( wxEVT_STC_MARGINCLICK, &MadOptionsDialog::OnMarginClick, this );
#else
	TextSample = new wxTextCtrl(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL|wxHSCROLL, wxDefaultValidator, _T("wxID_ANY"));
	//TextSample->SetMaxLength(0);
	TextSample->AppendText( bracket_style[aspsAllman] );
#endif
	TextSample->SetFocus();
#ifndef MADEDIT_USE_MADEDIT
	TextSample->SetInsertionPointEnd();
#endif
	StaticBoxSizer1->Add(TextSample, 0, wxALL|wxEXPAND, 5);
	GridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
	Panel1->SetSizer(GridSizer1);
	GridSizer1->Fit(Panel1);
	GridSizer1->SetSizeHints(Panel1);
	Panel2 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(792,490), 0, _T("wxID_ANY"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	CheckAttachClasses = new wxCheckBox(Panel2, wxID_ANY, _("Attach Classes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAttachClasses->SetValue(false);
	BoxSizer3->Add(CheckAttachClasses, 0, wxALL|wxEXPAND, 2);
	CheckAttachExternC = new wxCheckBox(Panel2, wxID_ANY, _("Attach Extern C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAttachExternC->SetValue(false);
	BoxSizer3->Add(CheckAttachExternC, 0, wxALL|wxEXPAND, 2);
	CheckAttachNamespaces = new wxCheckBox(Panel2, wxID_ANY, _("Attach Namespaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAttachNamespaces->SetValue(false);
	BoxSizer3->Add(CheckAttachNamespaces, 0, wxALL|wxEXPAND, 2);
	CheckAttachInlines = new wxCheckBox(Panel2, wxID_ANY, _("Attach Inlines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAttachInlines->SetValue(false);
	BoxSizer3->Add(CheckAttachInlines, 0, wxALL|wxEXPAND, 2);
	CheckAttachClosingWhile = new wxCheckBox(Panel2, wxID_ANY, _("Attach Closing While"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAttachClosingWhile->SetValue(false);
	BoxSizer3->Add(CheckAttachClosingWhile, 0, wxALL|wxEXPAND, 2);
	Panel2->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel2);
	BoxSizer3->SetSizeHints(Panel2);
	Panel3 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(792,490), 0, _T("wxID_ANY"));
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	GridSizer4 = new wxGridSizer(0, 2, 0, 0);
	StaticText2 = new wxStaticText(Panel3, wxID_ANY, _("Indentation size(in spaces):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer4->Add(StaticText2, 0, wxALL|wxEXPAND, 2);
	SpinIndentation = new wxSpinCtrl(Panel3, wxID_ANY, _T("4"), wxDefaultPosition, wxDefaultSize, 0, 2, 8, 4, _T("wxID_ANY"));
	SpinIndentation->SetValue(_T("4"));
	GridSizer4->Add(SpinIndentation, 0, wxALL, 2);
	BoxSizer4->Add(GridSizer4, 0, wxALL, 2);
	CheckUseTab = new wxCheckBox(Panel3, wxID_ANY, _("Use TABs instead of spaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckUseTab->SetValue(false);
	BoxSizer4->Add(CheckUseTab, 0, wxALL|wxEXPAND, 2);
	CheckForceUseTabs = new wxCheckBox(Panel3, wxID_ANY, _("Force using TABs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckForceUseTabs->SetValue(false);
	BoxSizer4->Add(CheckForceUseTabs, 0, wxALL|wxEXPAND, 2);
	CheckIndentCase = new wxCheckBox(Panel3, wxID_ANY, _("Indent case: statement in switches (commands under case:)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentCase->SetValue(false);
	BoxSizer4->Add(CheckIndentCase, 0, wxALL|wxALIGN_LEFT, 2);
	CheckIndentClasses = new wxCheckBox(Panel3, wxID_ANY, _("Indent classes (keywords public:, protected: and private:)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentClasses->SetValue(false);
	BoxSizer4->Add(CheckIndentClasses, 0, wxALL|wxEXPAND, 2);
	CheckIndentLabels = new wxCheckBox(Panel3, wxID_ANY, _("Indent labels"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentLabels->SetValue(false);
	BoxSizer4->Add(CheckIndentLabels, 0, wxALL|wxEXPAND, 2);
	CheckIndentModifiers = new wxCheckBox(Panel3, wxID_ANY, _("Indent modifiers"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentModifiers->SetValue(false);
	BoxSizer4->Add(CheckIndentModifiers, 0, wxALL|wxEXPAND, 2);
	CheckIndentNamespaces = new wxCheckBox(Panel3, wxID_ANY, _("Indent namespaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentNamespaces->SetValue(false);
	BoxSizer4->Add(CheckIndentNamespaces, 0, wxALL|wxEXPAND, 2);
	CheckIndentSwitches = new wxCheckBox(Panel3, wxID_ANY, _("Indent switches"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentSwitches->SetValue(false);
	BoxSizer4->Add(CheckIndentSwitches, 0, wxALL|wxEXPAND, 2);
	CheckIndentPreprocBlock = new wxCheckBox(Panel3, wxID_ANY, _("Indent preprocessor blocks at bracket level zero, and within a namespace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentPreprocBlock->SetValue(false);
	BoxSizer4->Add(CheckIndentPreprocBlock, 0, wxALL|wxEXPAND, 2);
	CheckIndentPreprocDefine = new wxCheckBox(Panel3, wxID_ANY, _("Indent multi-line preprocessor definitions ending with a backslash"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentPreprocDefine->SetValue(false);
	BoxSizer4->Add(CheckIndentPreprocDefine, 0, wxALL|wxEXPAND, 2);
	CheckIndentPreprocCond = new wxCheckBox(Panel3, wxID_ANY, _("Indent preprocessor conditionals"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentPreprocCond->SetValue(false);
	BoxSizer4->Add(CheckIndentPreprocCond, 0, wxALL|wxEXPAND, 2);
	CheckIndentCol1Comments = new wxCheckBox(Panel3, wxID_ANY, _("Indent C++ comments beginning in column one"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentCol1Comments->SetValue(false);
	BoxSizer4->Add(CheckIndentCol1Comments, 0, wxALL|wxEXPAND, 2);
	CheckIndentAfterParens = new wxCheckBox(Panel3, wxID_ANY, _("Indent after parens"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckIndentAfterParens->SetValue(false);
	BoxSizer4->Add(CheckIndentAfterParens, 0, wxALL|wxEXPAND, 2);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	StaticText3 = new wxStaticText(Panel3, wxID_ANY, _("Minimal indent added when a header is built of multiple lines:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText3, 0, wxALL|wxEXPAND, 2);
	SpinMinConditionalEvent = new wxSpinCtrl(Panel3, wxID_ANY, _T("2"), wxDefaultPosition, wxDefaultSize, 0, 0, 3, 2, _T("wxID_ANY"));
	SpinMinConditionalEvent->SetValue(_T("2"));
	FlexGridSizer3->Add(SpinMinConditionalEvent, 0, wxALL|wxEXPAND, 2);
	StaticText4 = new wxStaticText(Panel3, wxID_ANY, _("Maximum of # spaces to indent a continuation line (max. 120):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText4, 0, wxALL|wxEXPAND, 2);
	EditMaxContinuationIndent = new wxTextCtrl(Panel3, wxID_ANY, _T("40"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer3->Add(EditMaxContinuationIndent, 0, wxALL|wxEXPAND, 2);
	StaticText10 = new wxStaticText(Panel3, wxID_ANY, _("Indent a continuation line (default 1)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText10, 0, wxALL|wxEXPAND, 2);
	EditIndentContinuation = new wxTextCtrl(Panel3, wxID_ANY, _T("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	FlexGridSizer3->Add(EditIndentContinuation, 0, wxALL|wxEXPAND, 2);
	BoxSizer4->Add(FlexGridSizer3, 0, wxALL, 0);
	Panel3->SetSizer(BoxSizer4);
	BoxSizer4->Fit(Panel3);
	BoxSizer4->SetSizeHints(Panel3);
	Panel4 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(792,490), 0, _T("ID_PANEL4"));
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	CheckBreakClosing = new wxCheckBox(Panel4, wxID_ANY, _("Break closing headers from their immediately preceding closing braces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakClosing->SetValue(false);
	BoxSizer5->Add(CheckBreakClosing, 0, wxALL|wxEXPAND, 2);
	CheckBreakElseIfs = new wxCheckBox(Panel4, wxID_ANY, _("Break \'else if()\' header combinations into seperate lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakElseIfs->SetValue(false);
	BoxSizer5->Add(CheckBreakElseIfs, 0, wxALL|wxEXPAND, 2);
	CheckAddBraces = new wxCheckBox(Panel4, wxID_ANY, _("Add braces to unbraced one line conditional statements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAddBraces->SetValue(false);
	BoxSizer5->Add(CheckAddBraces, 0, wxALL|wxEXPAND, 2);
	CheckAddOneLineBraces = new wxCheckBox(Panel4, wxID_ANY, _("Add one line braces to unbraced one line conditional statements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckAddOneLineBraces->SetValue(false);
	BoxSizer5->Add(CheckAddOneLineBraces, 0, wxALL|wxEXPAND, 2);
	CheckRemoveBraces = new wxCheckBox(Panel4, wxID_ANY, _("Remove braces from conditional statements (single statement on a single line)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckRemoveBraces->SetValue(false);
	BoxSizer5->Add(CheckRemoveBraces, 0, wxALL|wxEXPAND, 2);
	CheckKeepBlocks = new wxCheckBox(Panel4, wxID_ANY, _("Don\'t break one-line blocks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckKeepBlocks->SetValue(false);
	BoxSizer5->Add(CheckKeepBlocks, 0, wxALL|wxEXPAND, 2);
	CheckKeepComplex = new wxCheckBox(Panel4, wxID_ANY, _("Don\'t break complex statements and multiple statements residing in a single line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckKeepComplex->SetValue(false);
	BoxSizer5->Add(CheckKeepComplex, 0, wxALL|wxEXPAND, 2);
	CheckConvertTabs = new wxCheckBox(Panel4, wxID_ANY, _("Convert TABs to spaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckConvertTabs->SetValue(false);
	BoxSizer5->Add(CheckConvertTabs, 0, wxALL|wxEXPAND, 2);
	CheckCloseTemplates = new wxCheckBox(Panel4, wxID_ANY, _("Closes whitespace in the angle brackets of template definitions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckCloseTemplates->SetValue(false);
	BoxSizer5->Add(CheckCloseTemplates, 0, wxALL|wxEXPAND, 2);
	CheckRemoveCommentPrefix = new wxCheckBox(Panel4, wxID_ANY, _("Remove the preceding \'*\' in a multi-line comment that begins a line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckRemoveCommentPrefix->SetValue(false);
	BoxSizer5->Add(CheckRemoveCommentPrefix, 0, wxALL|wxEXPAND, 2);
	CheckBreakOneLineHeaders = new wxCheckBox(Panel4, wxID_ANY, _("Break one line headers"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakOneLineHeaders->SetValue(false);
	BoxSizer5->Add(CheckBreakOneLineHeaders, 0, wxALL|wxEXPAND, 2);
	CheckBreakLines = new wxCheckBox(Panel4, wxID_ANY, _("Enable line breaking"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakLines->SetValue(false);
	BoxSizer5->Add(CheckBreakLines, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText5 = new wxStaticText(Panel4, wxID_ANY, _("Break lines after amount of chars (range: 50-200)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer6->Add(StaticText5, 0, wxALL|wxEXPAND, 2);
	EditSFMaxLineLength = new wxTextCtrl(Panel4, wxID_ANY, _T("200"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	BoxSizer6->Add(EditSFMaxLineLength, 0, wxALL, 2);
	BoxSizer5->Add(BoxSizer6, 0, wxALL|wxEXPAND, 0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer7->Add(10,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBreakAfterLogical = new wxCheckBox(Panel4, wxID_ANY, _("Break logical conditionals to be placed last on the previous line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakAfterLogical->SetValue(false);
	BoxSizer7->Add(CheckBreakAfterLogical, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5->Add(BoxSizer7, 0, wxALL|wxEXPAND, 0);
	Panel4->SetSizer(BoxSizer5);
	BoxSizer5->Fit(Panel4);
	BoxSizer5->SetSizeHints(Panel4);
	Panel5 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(792,490), 0, _T("wxID_ANY"));
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	CheckBreakBlocks = new wxCheckBox(Panel5, wxID_ANY, _("Pad empty lines around header blocks (e.g. \'if\', \'while\'...)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakBlocks->SetValue(false);
	BoxSizer9->Add(CheckBreakBlocks, 0, wxALL|wxEXPAND, 2);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(10,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBreakBlocksAll = new wxCheckBox(Panel5, wxID_ANY, _("Treat closing header blocks (e.g. \'else\', \'catch\') as stand-alone blocks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckBreakBlocksAll->SetValue(false);
	BoxSizer2->Add(CheckBreakBlocksAll, 0, wxALL|wxEXPAND, 2);
	BoxSizer9->Add(BoxSizer2, 0, wxALL|wxEXPAND, 2);
	CheckPadOperators = new wxCheckBox(Panel5, wxID_ANY, _("Insert space padding around operators"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadOperators->SetValue(false);
	BoxSizer9->Add(CheckPadOperators, 0, wxALL|wxEXPAND, 2);
	CheckPadParensOut = new wxCheckBox(Panel5, wxID_ANY, _("Insert space padding around parenthesies on the outside"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadParensOut->SetValue(false);
	BoxSizer9->Add(CheckPadParensOut, 0, wxALL|wxEXPAND, 2);
	CheckPadParensIn = new wxCheckBox(Panel5, wxID_ANY, _("Insert space padding around parenthesies on the inside"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadParensIn->SetValue(false);
	BoxSizer9->Add(CheckPadParensIn, 0, wxALL|wxEXPAND, 2);
	CheckPadHeader = new wxCheckBox(Panel5, wxID_ANY, _("Insert space padding between a header and the following parenthesis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadHeader->SetValue(false);
	BoxSizer9->Add(CheckPadHeader, 0, wxALL|wxEXPAND, 2);
	CheckUnpadParens = new wxCheckBox(Panel5, wxID_ANY, _("Remove extra space padding around parenthesis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckUnpadParens->SetValue(false);
	BoxSizer9->Add(CheckUnpadParens, 0, wxALL|wxEXPAND, 2);
	CheckDelEmptyLine = new wxCheckBox(Panel5, wxID_ANY, _("Delete empty lines within a function or method"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckDelEmptyLine->SetValue(false);
	BoxSizer9->Add(CheckDelEmptyLine, 0, wxALL|wxEXPAND, 2);
	CheckFillEmptyLines = new wxCheckBox(Panel5, wxID_ANY, _("Fill empty lines with the whitespace of their previous lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckFillEmptyLines->SetValue(false);
	BoxSizer9->Add(CheckFillEmptyLines, 0, wxALL|wxEXPAND, 2);
	CheckPadComma = new wxCheckBox(Panel5, wxID_ANY, _("Pad comma"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadComma->SetValue(false);
	BoxSizer9->Add(CheckPadComma, 0, wxALL|wxEXPAND, 2);
	CheckPadReturnType = new wxCheckBox(Panel5, wxID_ANY, _("Pad return type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadReturnType->SetValue(false);
	BoxSizer9->Add(CheckPadReturnType, 0, wxALL|wxEXPAND, 2);
	CheckUnpadReturnType = new wxCheckBox(Panel5, wxID_ANY, _("Unpad return type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckUnpadReturnType->SetValue(false);
	BoxSizer9->Add(CheckUnpadReturnType, 0, wxALL|wxEXPAND, 2);
	CheckPadParamType = new wxCheckBox(Panel5, wxID_ANY, _("Pad param type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckPadParamType->SetValue(false);
	BoxSizer9->Add(CheckPadParamType, 0, wxALL|wxEXPAND, 2);
	CheckUnpadParamType = new wxCheckBox(Panel5, wxID_ANY, _("Unpad param type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	CheckUnpadParamType->SetValue(false);
	BoxSizer9->Add(CheckUnpadParamType, 0, wxALL|wxEXPAND, 2);
	GridSizer3 = new wxGridSizer(0, 4, 0, 0);
	StaticText6 = new wxStaticText(Panel5, wxID_ANY, _("Pointer alignment"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer3->Add(StaticText6, 2, wxALL|wxEXPAND, 0);
	ChoicePointerAlign = new wxChoice(Panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("wxID_ANY"));
	ChoicePointerAlign->SetSelection( ChoicePointerAlign->Append(_("None")) );
	ChoicePointerAlign->Append(_("Type"));
	ChoicePointerAlign->Append(_("Middle"));
	ChoicePointerAlign->Append(_("Name"));
	GridSizer3->Add(ChoicePointerAlign, 0, wxALL|wxEXPAND, 2);
	StaticText7 = new wxStaticText(Panel5, wxID_ANY, _("Reference alignment"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer3->Add(StaticText7, 0, wxALL|wxEXPAND, 2);
	ChoiceReferenceAlign = new wxChoice(Panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("wxID_ANY"));
	ChoiceReferenceAlign->SetSelection( ChoiceReferenceAlign->Append(_("None")) );
	ChoiceReferenceAlign->Append(_("Type"));
	ChoiceReferenceAlign->Append(_("Middle"));
	ChoiceReferenceAlign->Append(_("Name"));
	GridSizer3->Add(ChoiceReferenceAlign, 0, wxALL|wxEXPAND, 2);
	BoxSizer9->Add(GridSizer3, 0, wxALL, 2);
	Panel5->SetSizer(BoxSizer9);
	BoxSizer9->Fit(Panel5);
	BoxSizer9->SetSizeHints(Panel5);
	Panel6 = new wxPanel(AuiNotebook1, wxID_ANY, wxDefaultPosition, wxSize(792,490), 0, _T("wxID_ANY"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	GridSizer2 = new wxGridSizer(0, 2, 0, 0);
	StaticText8 = new wxStaticText(Panel6, wxID_ANY, _("XML version:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer2->Add(StaticText8, 0, wxALL|wxEXPAND, 2);
	EditXMLversion = new wxTextCtrl(Panel6, wxID_ANY, _T("1.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	GridSizer2->Add(EditXMLversion, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText9 = new wxStaticText(Panel6, wxID_ANY, _("Indentation size(in spaces):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer2->Add(StaticText9, 0, wxALL|wxEXPAND, 2);
	EditXmlIndentSize = new wxTextCtrl(Panel6, wxID_ANY, _T("4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ANY"));
	GridSizer2->Add(EditXmlIndentSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(GridSizer2, 0, wxALL|wxALIGN_LEFT, 2);
	Panel6->SetSizer(BoxSizer8);
	BoxSizer8->Fit(Panel6);
	BoxSizer8->SetSizeHints(Panel6);
	AuiNotebook1->AddPage(Panel1, _("Style"));
	AuiNotebook1->AddPage(Panel2, _("Brackets"));
	AuiNotebook1->AddPage(Panel3, _("Indentation"));
	AuiNotebook1->AddPage(Panel4, _("Formatting"));
	AuiNotebook1->AddPage(Panel5, _("Padding"));
	AuiNotebook1->AddPage(Panel6, _T("XML"));
	BoxSizer1->Add(AuiNotebook1, 1, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Layout();

	Bind(wxEVT_COMMAND_RADIOBOX_SELECTED,&MadSourceFormatDialog::OnRadioBoxBracketStyleSelect, this, RadioBoxBracketStyle->GetId());
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&MadSourceFormatDialog::OnCheckBreakLinesClick, this, CheckBreakLines->GetId());
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&MadSourceFormatDialog::OnCheckBreakBlocksClick, this, CheckBreakBlocks->GetId());
	//*)
}

MadSourceFormatDialog::~MadSourceFormatDialog()
{
	//(*Destroy(MadSourceFormatDialog)
	//*)
}

void MadSourceFormatDialog::OnRadioBoxBracketStyleSelect( wxCommandEvent& WXUNUSED(event) )
{
	long style = RadioBoxBracketStyle->GetSelection();
#ifdef MADEDIT_ENABLE_STC
	TextSample->SetReadOnly( false );
#endif

	switch( style )
	{
	case aspsAllman: // Allman (ANSI)
	case aspsJava: // Java
	case aspsKr: // K&R
	case aspsStroustrup: // Stroustrup
	case aspsWhitesmith: // Whitesmith
	case aspsVTK: // VTK
	case aspsRaliff: // Ratliff
	case aspsGnu: // GNU
	case aspsLinux: // Linux
	case aspsHorstmann: // Horstmann
	case asps1TBS: // 1TBS
	case aspsGoogle: // Google
	case aspsMozilla: // Mozilla
	case aspsWebkit: // Webkit
	case aspsPico: // Pico
	case aspsLisp: // Lisp
#ifndef MADEDIT_USE_MADEDIT
		TextSample->SetValue( bracket_style[style] );
#else
		TextSample->SetText(bracket_style[style]);
#endif
		break;

	default: // Custom
#ifndef MADEDIT_USE_MADEDIT
		TextSample->SetValue(bracket_style[aspsCustom]);
#else
		TextSample->SetText(bracket_style[aspsCustom]);
#endif
		break;
	}

#ifdef MADEDIT_USE_STC
	TextSample->SetReadOnly( true );
#endif
}

void MadSourceFormatDialog::OnCheckBreakLinesClick( wxCommandEvent& WXUNUSED(event) )
{
	bool bb = CheckBreakLines->GetValue();
	EditSFMaxLineLength->Enable( bb );
	CheckBreakAfterLogical->Enable( bb );
}

void MadSourceFormatDialog::OnCheckBreakBlocksClick( wxCommandEvent& WXUNUSED(event) )
{
	bool bb =  CheckBreakBlocks->GetValue();
	CheckBreakBlocksAll->Enable( bb );
}

void MadSourceFormatDialog::LoadSettings(void)
{
	wxConfigBase *cfg = wxConfigBase::Get( false );
	wxString oldpath = cfg->GetPath();
	cfg->SetPath( wxT( "/MadEdit" ) );
//	long ll;
	bool bb;
	wxString ss;
	cfg->SetPath( wxT( "/astyle" ) );
	RadioBoxBracketStyle->SetSelection( cfg->ReadLong( wxT( "style" ), aspsAllman ) );
	CheckAttachClasses->SetValue( cfg->ReadBool( wxT( "attach_classes" ), false ) );
	CheckAttachExternC->SetValue( cfg->ReadBool( wxT( "attach_extern_c" ), true ) );
	CheckAttachNamespaces->SetValue( cfg->ReadBool( wxT( "attach_namespaces" ), true ) );
	CheckAttachInlines->SetValue( cfg->ReadBool( wxT( "attach_inlines" ), true ) );
	CheckAttachClosingWhile->SetValue( cfg->ReadBool( wxT( "attach_closing_while" ), false ) );
	CheckForceUseTabs->SetValue( cfg->ReadBool( wxT( "force_tabs" ), false ) );
	SpinIndentation->SetValue( cfg->ReadLong( wxT( "indentation" ), 4 ) );
	CheckUseTab->SetValue( cfg->ReadBool( wxT( "use_tabs" ), false ) );
	CheckIndentCase->SetValue( cfg->ReadBool( wxT( "indent_case" ), true ) );
	CheckIndentClasses->SetValue( cfg->ReadBool( wxT( "indent_classes" ), false ) );
	CheckIndentLabels->SetValue( cfg->ReadBool( wxT( "indent_labels" ), false ) );
	CheckIndentModifiers->SetValue( cfg->ReadBool( wxT( "indent_modifiers" ), false ) );
	CheckIndentNamespaces->SetValue( cfg->ReadBool( wxT( "indent_namespaces" ), true ) );
	CheckIndentSwitches->SetValue( cfg->ReadBool( wxT( "indent_switches" ), false ) );
	CheckIndentPreprocBlock->SetValue( cfg->ReadBool( wxT( "indent_preproc_block" ), true ) );
	CheckIndentPreprocDefine->SetValue( cfg->ReadBool( wxT( "indent_preproc_define" ), false ) );
	CheckIndentPreprocCond->SetValue( cfg->ReadBool( wxT( "indent_preproc_cond" ), false ) );
	CheckIndentCol1Comments->SetValue( cfg->ReadBool( wxT( "indent_col1_comments" ), true ) );
	CheckIndentAfterParens->SetValue( cfg->ReadBool( wxT( "indent_after_parens" ), false ) );
	SpinMinConditionalEvent->SetValue( cfg->ReadLong( wxT( "min_conditional_indent" ), 2 ) );
	EditMaxContinuationIndent->SetValue( wxString() << cfg->ReadLong( wxT( "max_continuation_indent" ), 40 ) );
	EditIndentContinuation->SetValue( wxString() << cfg->ReadLong( wxT( "indent_continuation" ), 1 ) );
	CheckBreakClosing->SetValue( cfg->ReadBool( wxT( "break_closing" ), true ) );
	CheckBreakElseIfs->SetValue( cfg->ReadBool( wxT( "break_elseifs" ), true ) );
	CheckAddBraces->SetValue( cfg->ReadBool( wxT( "add_brackets" ), false ) );
	CheckAddOneLineBraces->SetValue( cfg->ReadBool( wxT( "add_one_line_brackets" ), true ) );
	CheckRemoveBraces->SetValue( cfg->ReadBool( wxT( "remove_brackets" ), false ) );
	CheckKeepBlocks->SetValue( cfg->ReadBool( wxT( "keep_blocks" ), true ) );
	CheckConvertTabs->SetValue( cfg->ReadBool( wxT( "convert_tabs" ), true ) );
	CheckCloseTemplates->SetValue( cfg->ReadBool( wxT( "close_templates" ), false ) );
	CheckRemoveCommentPrefix->SetValue( cfg->ReadBool( wxT( "remove_comment_prefix" ), false ) );
	bb = cfg->ReadBool( wxT( "break_lines" ), false );
	CheckBreakLines->SetValue( bb );

	if( bb )
	{
		EditSFMaxLineLength->Enable( true );
		CheckBreakAfterLogical->Enable( true );
		EditSFMaxLineLength->SetValue( cfg->Read( wxT( "max_line_length" ), wxString( wxT( "200" ) ) ) );
		CheckBreakAfterLogical->SetValue( cfg->ReadBool( wxT( "break_after_mode" ), false ) );
	}
	else
	{
		EditSFMaxLineLength->Enable( false );
		CheckBreakAfterLogical->Enable( false );
	}

	CheckBreakBlocks->SetValue( cfg->ReadBool( wxT( "break_blocks" ), true ) );

	if( CheckBreakBlocks->GetValue() )
	{
		CheckBreakBlocksAll->Enable( true );
		CheckBreakBlocksAll->SetValue( cfg->ReadBool( wxT( "break_blocks_all" ), false ) );
	}
	else
	{
		CheckBreakBlocksAll->Enable( false );
	}

	CheckPadOperators->SetValue( cfg->ReadBool( wxT( "pad_operators" ), true ) );
	CheckPadParensOut->SetValue( cfg->ReadBool( wxT( "pad_parentheses_out" ), false ) );
	CheckPadParensIn->SetValue( cfg->ReadBool( wxT( "pad_parentheses_in" ), true ) );
	CheckPadHeader->SetValue( cfg->ReadBool( wxT( "pad_header" ), false ) );
	CheckUnpadParens ->SetValue( cfg->ReadBool( wxT( "unpad_parentheses" ), true ) );
	CheckDelEmptyLine->SetValue( cfg->ReadBool( wxT( "delete_empty_lines" ), true ) );
	CheckFillEmptyLines->SetValue( cfg->ReadBool( wxT( "fill_empty_lines" ), false ) );
	wxString pointerAlign = cfg->Read( wxT( "pointer_align" ), wxEmptyString );

	if( pointerAlign == wxT( "Type" ) )
	{ ChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_TYPE ); }
	else
		if( pointerAlign == wxT( "Middle" ) )
		{ ChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_MIDDLE ); }
		else
			if( pointerAlign == wxT( "Name" ) )
			{ ChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_NAME ); }
			else
			{ ChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_NONE ); }

	wxString referenceAlign = cfg->Read( wxT( "reference_align" ), wxEmptyString );

	if( referenceAlign == wxT( "Type" ) )
	{ ChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_TYPE ); }
	else
		if( referenceAlign == wxT( "Middle" ) )
		{ ChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_MIDDLE ); }
		else
			if( referenceAlign == wxT( "Name" ) )
			{ ChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_NAME ); }
			else
			{ ChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_NONE ); }

	cfg->SetPath( wxT( "/xml" ) );
	EditXmlIndentSize->SetValue( wxString() << cfg->ReadLong( wxT( "indentation" ), 4 ) );
	EditXMLversion->SetValue( cfg->Read( wxT( "version" ), wxString( wxT( "1.0" ) ) ) );
	cfg->SetPath( oldpath );
}

