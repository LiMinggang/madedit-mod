#include "MadSourceFormatDialog.h"
#include "MadEdit/MadEdit.h"
#include "astyle/astyle.h"
#include "astylepredefinedstyles.h"

//(*InternalHeaders(MadSourceFormatDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MadSourceFormatDialog)
const long MadSourceFormatDialog::ID_RADIOBOXBRACKETSTYLE = wxNewId();
const long MadSourceFormatDialog::ID_TEXTSAMPLE = wxNewId();
const long MadSourceFormatDialog::ID_PANEL1 = wxNewId();
const long MadSourceFormatDialog::ID_CHECKATTACHCLASSES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKATTACHEXTERNC = wxNewId();
const long MadSourceFormatDialog::ID_CHECKATTACHNAMESPACES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKATTACHINLINES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKATTACHCLOSINGWHILE = wxNewId();
const long MadSourceFormatDialog::ID_PANEL2 = wxNewId();
const long MadSourceFormatDialog::ID_SPINCTRLINDENTSIZE = wxNewId();
const long MadSourceFormatDialog::ID_CHECKUSETABOVERSPACES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKFORCETABS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTCASE = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENETCLASSES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTLABELS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTMODIFIERS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTNS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTSWITCHS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTPREP = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTMULTIPREP = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTPREPCOND = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENETCPPCOMMENTS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKINDENTAFTERPARENS = wxNewId();
const long MadSourceFormatDialog::ID_SPINCTRLMININDENT = wxNewId();
const long MadSourceFormatDialog::ID_EDITMAXCONTINUATIONINDENT = wxNewId();
const long MadSourceFormatDialog::ID_EDITINDENTCONTINUATION = wxNewId();
const long MadSourceFormatDialog::ID_PANEL3 = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKCLOSING = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKELSEIFS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKADDBRACKETS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKADDONELINEBRACKETS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBOXREMOVEBRACKETS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKKEEPBLOCKS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKKEEPCOMPLEX = wxNewId();
const long MadSourceFormatDialog::ID_CHECKCONVERTTABS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKCLOSETEMPLATES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKREMOVECOMMENTPREFIX = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKONELINEHEADERS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKLINES = wxNewId();
const long MadSourceFormatDialog::ID_EDITSFMAXLINELENGTH = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKAFTERLOGICAL = wxNewId();
const long MadSourceFormatDialog::ID_PANEL4 = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKBLOCKS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKBREAKBLOCKSALL = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADOPERATORS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADPARENSOUT = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADPARENSIN = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADHEADER = wxNewId();
const long MadSourceFormatDialog::ID_CHECKUNPADPARENS = wxNewId();
const long MadSourceFormatDialog::ID_CHECKDELEMPTYLINE = wxNewId();
const long MadSourceFormatDialog::ID_CHECKFILLEMPTYLINES = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADCOMMA = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADRETURNTYPE = wxNewId();
const long MadSourceFormatDialog::ID_CHECKUNPADRETURNTYPE = wxNewId();
const long MadSourceFormatDialog::ID_CHECKPADPARAMTYPE = wxNewId();
const long MadSourceFormatDialog::ID_CHECKUNPADPARAMTYPE = wxNewId();
const long MadSourceFormatDialog::ID_CHOICEPOINTERALIGN = wxNewId();
const long MadSourceFormatDialog::ID_CHOICEREFERENCEALIGN = wxNewId();
const long MadSourceFormatDialog::ID_PANEL5 = wxNewId();
const long MadSourceFormatDialog::ID_EDITXMLVERSION = wxNewId();
const long MadSourceFormatDialog::ID_EDITXMLINDENTSIZE = wxNewId();
const long MadSourceFormatDialog::ID_PANEL6 = wxNewId();
const long MadSourceFormatDialog::ID_AUINOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MadSourceFormatDialog,wxDialog)
	//(*EventTable(MadSourceFormatDialog)
	//*)
END_EVENT_TABLE()

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
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1; }\n    else\n      return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
};

MadSourceFormatDialog::MadSourceFormatDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
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
	AuiNotebook1 = new wxAuiNotebook(this, ID_AUINOTEBOOK1, wxDefaultPosition, wxSize(800,500), wxAUI_NB_TOP);
	Panel1 = new wxPanel(AuiNotebook1, ID_PANEL1, wxDefaultPosition, wxSize(792,464), 0, _T("ID_PANEL1"));
	GridSizer1 = new wxGridSizer(0, 2, 0, 0);
	wxString __wxRadioBoxChoices_1[15] =
	{
		_T("Allman(ANSI)"),
		_T("Java"),
		_T("Kr"),
		_T("Stroustrup"),
		_T("Whitesmith"),
		_T("VTK"),
		_T("Banner"),
		_T("Gnu"),
		_T("Linux"),
		_T("Horstmann"),
		_T("1TBS"),
		_T("Google"),
		_T("Pico"),
		_T("Lisp"),
		_T("Custom")
	};
	RadioBoxBracketStyle = new wxRadioBox(Panel1, ID_RADIOBOXBRACKETSTYLE, _("Bracket Style"), wxDefaultPosition, wxDefaultSize, 15, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOXBRACKETSTYLE"));
	GridSizer1->Add(RadioBoxBracketStyle, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel1, _("Sample"));
	StaticText1 = new wxStaticText(Panel1, wxID_ANY, _("Sample of the bracket style option chosen to use:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	StaticBoxSizer1->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
#ifdef MADEDIT_USE_MADEDIT
	TextSample = new MadEdit(Panel1, ID_TEXTSAMPLE, wxPoint(19, 47), wxSize(300, 300));
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
	TextSample = new wxStyledTextCtrl( Panel1, ID_TEXTSAMPLE, wxPoint( 19, 47 ), wxSize( 219, 300 ) );
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
	TextSample = new wxTextCtrl(Panel1, ID_TEXTSAMPLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_TEXTSAMPLE"));
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
	Panel2 = new wxPanel(AuiNotebook1, ID_PANEL2, wxDefaultPosition, wxSize(792,464), 0, _T("ID_PANEL2"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	CheckAttachClasses = new wxCheckBox(Panel2, ID_CHECKATTACHCLASSES, _("Attach Classes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKATTACHCLASSES"));
	CheckAttachClasses->SetValue(false);
	BoxSizer3->Add(CheckAttachClasses, 0, wxALL|wxEXPAND, 2);
	CheckAttachExternC = new wxCheckBox(Panel2, ID_CHECKATTACHEXTERNC, _("Attach Extern C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKATTACHEXTERNC"));
	CheckAttachExternC->SetValue(false);
	BoxSizer3->Add(CheckAttachExternC, 0, wxALL|wxEXPAND, 2);
	CheckAttachNamespaces = new wxCheckBox(Panel2, ID_CHECKATTACHNAMESPACES, _("Attach Namespaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKATTACHNAMESPACES"));
	CheckAttachNamespaces->SetValue(false);
	BoxSizer3->Add(CheckAttachNamespaces, 0, wxALL|wxEXPAND, 2);
	CheckAttachInlines = new wxCheckBox(Panel2, ID_CHECKATTACHINLINES, _("Attach Inlines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKATTACHINLINES"));
	CheckAttachInlines->SetValue(false);
	BoxSizer3->Add(CheckAttachInlines, 0, wxALL|wxEXPAND, 2);
	CheckAttachClosingWhile = new wxCheckBox(Panel2, ID_CHECKATTACHCLOSINGWHILE, _("Attach Closing While"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKATTACHCLOSINGWHILE"));
	CheckAttachClosingWhile->SetValue(false);
	BoxSizer3->Add(CheckAttachClosingWhile, 0, wxALL|wxEXPAND, 2);
	Panel2->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel2);
	BoxSizer3->SetSizeHints(Panel2);
	Panel3 = new wxPanel(AuiNotebook1, ID_PANEL3, wxDefaultPosition, wxSize(792,464), 0, _T("ID_PANEL3"));
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	GridSizer4 = new wxGridSizer(0, 2, 0, 0);
	StaticText2 = new wxStaticText(Panel3, wxID_ANY, _("Indentation size(in spaces):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer4->Add(StaticText2, 0, wxALL|wxEXPAND, 2);
	SpinIndentation = new wxSpinCtrl(Panel3, ID_SPINCTRLINDENTSIZE, _T("4"), wxDefaultPosition, wxDefaultSize, 0, 2, 8, 4, _T("ID_SPINCTRLINDENTSIZE"));
	SpinIndentation->SetValue(_T("4"));
	GridSizer4->Add(SpinIndentation, 0, wxALL, 2);
	BoxSizer4->Add(GridSizer4, 0, wxALL, 2);
	CheckUseTab = new wxCheckBox(Panel3, ID_CHECKUSETABOVERSPACES, _("Use TABs instead of spaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKUSETABOVERSPACES"));
	CheckUseTab->SetValue(false);
	BoxSizer4->Add(CheckUseTab, 0, wxALL|wxEXPAND, 2);
	CheckForceUseTabs = new wxCheckBox(Panel3, ID_CHECKFORCETABS, _("Force using TABs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKFORCETABS"));
	CheckForceUseTabs->SetValue(false);
	BoxSizer4->Add(CheckForceUseTabs, 0, wxALL|wxEXPAND, 2);
	CheckIndentCase = new wxCheckBox(Panel3, ID_CHECKINDENTCASE, _("Indent case: statement in switches (commands under case:)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTCASE"));
	CheckIndentCase->SetValue(false);
	BoxSizer4->Add(CheckIndentCase, 0, wxALL|wxALIGN_LEFT, 2);
	CheckIndentClasses = new wxCheckBox(Panel3, ID_CHECKINDENETCLASSES, _("Indent classes (keywords public:, protected: and private:)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENETCLASSES"));
	CheckIndentClasses->SetValue(false);
	BoxSizer4->Add(CheckIndentClasses, 0, wxALL|wxEXPAND, 2);
	CheckIndentLabels = new wxCheckBox(Panel3, ID_CHECKINDENTLABELS, _("Indent labels"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTLABELS"));
	CheckIndentLabels->SetValue(false);
	BoxSizer4->Add(CheckIndentLabels, 0, wxALL|wxEXPAND, 2);
	CheckIndentModifiers = new wxCheckBox(Panel3, ID_CHECKINDENTMODIFIERS, _("Indent modifiers"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTMODIFIERS"));
	CheckIndentModifiers->SetValue(false);
	BoxSizer4->Add(CheckIndentModifiers, 0, wxALL|wxEXPAND, 2);
	CheckIndentNamespaces = new wxCheckBox(Panel3, ID_CHECKINDENTNS, _("Indent namespaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTNS"));
	CheckIndentNamespaces->SetValue(false);
	BoxSizer4->Add(CheckIndentNamespaces, 0, wxALL|wxEXPAND, 2);
	CheckIndentSwitches = new wxCheckBox(Panel3, ID_CHECKINDENTSWITCHS, _("Indent switches"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTSWITCHS"));
	CheckIndentSwitches->SetValue(false);
	BoxSizer4->Add(CheckIndentSwitches, 0, wxALL|wxEXPAND, 2);
	CheckIndentPreprocBlock = new wxCheckBox(Panel3, ID_CHECKINDENTPREP, _("Indent preprocessor blocks at bracket level zero, and within a namespace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTPREP"));
	CheckIndentPreprocBlock->SetValue(false);
	BoxSizer4->Add(CheckIndentPreprocBlock, 0, wxALL|wxEXPAND, 2);
	CheckIndentPreprocDefine = new wxCheckBox(Panel3, ID_CHECKINDENTMULTIPREP, _("Indent multi-line preprocessor definitions ending with a backslash"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTMULTIPREP"));
	CheckIndentPreprocDefine->SetValue(false);
	BoxSizer4->Add(CheckIndentPreprocDefine, 0, wxALL|wxEXPAND, 2);
	CheckIndentPreprocCond = new wxCheckBox(Panel3, ID_CHECKINDENTPREPCOND, _("Indent preprocessor conditionals"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTPREPCOND"));
	CheckIndentPreprocCond->SetValue(false);
	BoxSizer4->Add(CheckIndentPreprocCond, 0, wxALL|wxEXPAND, 2);
	CheckIndentCol1Comments = new wxCheckBox(Panel3, ID_CHECKINDENETCPPCOMMENTS, _("Indent C++ comments beginning in column one"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENETCPPCOMMENTS"));
	CheckIndentCol1Comments->SetValue(false);
	BoxSizer4->Add(CheckIndentCol1Comments, 0, wxALL|wxEXPAND, 2);
	CheckIndentAfterParens = new wxCheckBox(Panel3, ID_CHECKINDENTAFTERPARENS, _("Indent after parens"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKINDENTAFTERPARENS"));
	CheckIndentAfterParens->SetValue(false);
	BoxSizer4->Add(CheckIndentAfterParens, 0, wxALL|wxEXPAND, 2);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	StaticText3 = new wxStaticText(Panel3, wxID_ANY, _("Minimal indent added when a header is built of multiple lines:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText3, 0, wxALL|wxEXPAND, 2);
	SpinMinConditionalEvent = new wxSpinCtrl(Panel3, ID_SPINCTRLMININDENT, _T("2"), wxDefaultPosition, wxDefaultSize, 0, 0, 3, 2, _T("ID_SPINCTRLMININDENT"));
	SpinMinConditionalEvent->SetValue(_T("2"));
	FlexGridSizer3->Add(SpinMinConditionalEvent, 0, wxALL|wxEXPAND, 2);
	StaticText4 = new wxStaticText(Panel3, wxID_ANY, _("Maximum of # spaces to indent a continuation line (max. 120):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText4, 0, wxALL|wxEXPAND, 2);
	EditMaxContinuationIndent = new wxTextCtrl(Panel3, ID_EDITMAXCONTINUATIONINDENT, _T("40"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITMAXCONTINUATIONINDENT"));
	FlexGridSizer3->Add(EditMaxContinuationIndent, 0, wxALL|wxEXPAND, 2);
	StaticText10 = new wxStaticText(Panel3, wxID_ANY, _("Indent a continuation line (default 1)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText10, 0, wxALL|wxEXPAND, 2);
	EditIndentContinuation = new wxTextCtrl(Panel3, ID_EDITINDENTCONTINUATION, _T("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITINDENTCONTINUATION"));
	FlexGridSizer3->Add(EditIndentContinuation, 0, wxALL|wxEXPAND, 2);
	BoxSizer4->Add(FlexGridSizer3, 0, wxALL, 0);
	Panel3->SetSizer(BoxSizer4);
	BoxSizer4->Fit(Panel3);
	BoxSizer4->SetSizeHints(Panel3);
	Panel4 = new wxPanel(AuiNotebook1, ID_PANEL4, wxDefaultPosition, wxSize(792,464), 0, _T("ID_PANEL4"));
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	CheckBreakClosing = new wxCheckBox(Panel4, ID_CHECKBREAKCLOSING, _("Break closing headers from their immediately preceding closing braces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKCLOSING"));
	CheckBreakClosing->SetValue(false);
	BoxSizer5->Add(CheckBreakClosing, 0, wxALL|wxEXPAND, 2);
	CheckBreakElseIfs = new wxCheckBox(Panel4, ID_CHECKBREAKELSEIFS, _("Break \'else if()\' header combinations into seperate lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKELSEIFS"));
	CheckBreakElseIfs->SetValue(false);
	BoxSizer5->Add(CheckBreakElseIfs, 0, wxALL|wxEXPAND, 2);
	CheckAddBraces = new wxCheckBox(Panel4, ID_CHECKADDBRACKETS, _("Add braces to unbraced one line conditional statements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKADDBRACKETS"));
	CheckAddBraces->SetValue(false);
	BoxSizer5->Add(CheckAddBraces, 0, wxALL|wxEXPAND, 2);
	CheckAddOneLineBraces = new wxCheckBox(Panel4, ID_CHECKADDONELINEBRACKETS, _("Add one line braces to unbraced one line conditional statements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKADDONELINEBRACKETS"));
	CheckAddOneLineBraces->SetValue(false);
	BoxSizer5->Add(CheckAddOneLineBraces, 0, wxALL|wxEXPAND, 2);
	CheckRemoveBraces = new wxCheckBox(Panel4, ID_CHECKBOXREMOVEBRACKETS, _("Remove braces from conditional statements (single statement on a single line)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXREMOVEBRACKETS"));
	CheckRemoveBraces->SetValue(false);
	BoxSizer5->Add(CheckRemoveBraces, 0, wxALL|wxEXPAND, 2);
	CheckKeepBlocks = new wxCheckBox(Panel4, ID_CHECKKEEPBLOCKS, _("Don\'t break one-line blocks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKKEEPBLOCKS"));
	CheckKeepBlocks->SetValue(false);
	BoxSizer5->Add(CheckKeepBlocks, 0, wxALL|wxEXPAND, 2);
	CheckKeepComplex = new wxCheckBox(Panel4, ID_CHECKKEEPCOMPLEX, _("Don\'t break complex statements and multiple statements residing in a single line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKKEEPCOMPLEX"));
	CheckKeepComplex->SetValue(false);
	BoxSizer5->Add(CheckKeepComplex, 0, wxALL|wxEXPAND, 2);
	CheckConvertTabs = new wxCheckBox(Panel4, ID_CHECKCONVERTTABS, _("Convert TABs to spaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKCONVERTTABS"));
	CheckConvertTabs->SetValue(false);
	BoxSizer5->Add(CheckConvertTabs, 0, wxALL|wxEXPAND, 2);
	CheckCloseTemplates = new wxCheckBox(Panel4, ID_CHECKCLOSETEMPLATES, _("Closes whitespace in the angle brackets of template definitions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKCLOSETEMPLATES"));
	CheckCloseTemplates->SetValue(false);
	BoxSizer5->Add(CheckCloseTemplates, 0, wxALL|wxEXPAND, 2);
	CheckRemoveCommentPrefix = new wxCheckBox(Panel4, ID_CHECKREMOVECOMMENTPREFIX, _("Remove the preceding \'*\' in a multi-line comment that begins a line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKREMOVECOMMENTPREFIX"));
	CheckRemoveCommentPrefix->SetValue(false);
	BoxSizer5->Add(CheckRemoveCommentPrefix, 0, wxALL|wxEXPAND, 2);
	CheckBreakOneLineHeaders = new wxCheckBox(Panel4, ID_CHECKBREAKONELINEHEADERS, _("Break one line headers"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKONELINEHEADERS"));
	CheckBreakOneLineHeaders->SetValue(false);
	BoxSizer5->Add(CheckBreakOneLineHeaders, 0, wxALL|wxEXPAND, 2);
	CheckBreakLines = new wxCheckBox(Panel4, ID_CHECKBREAKLINES, _("Enable line breaking"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKLINES"));
	CheckBreakLines->SetValue(false);
	BoxSizer5->Add(CheckBreakLines, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6->Add(10,0,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText5 = new wxStaticText(Panel4, wxID_ANY, _("Break lines after amount of chars (range: 50-200)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer6->Add(StaticText5, 0, wxALL|wxEXPAND, 2);
	EditSFMaxLineLength = new wxTextCtrl(Panel4, ID_EDITSFMAXLINELENGTH, _T("200"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITSFMAXLINELENGTH"));
	BoxSizer6->Add(EditSFMaxLineLength, 0, wxALL, 2);
	BoxSizer5->Add(BoxSizer6, 0, wxALL|wxEXPAND, 0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer7->Add(10,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBreakAfterLogical = new wxCheckBox(Panel4, ID_CHECKBREAKAFTERLOGICAL, _("Break logical conditionals to be placed last on the previous line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKAFTERLOGICAL"));
	CheckBreakAfterLogical->SetValue(false);
	BoxSizer7->Add(CheckBreakAfterLogical, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5->Add(BoxSizer7, 0, wxALL|wxEXPAND, 0);
	Panel4->SetSizer(BoxSizer5);
	BoxSizer5->Fit(Panel4);
	BoxSizer5->SetSizeHints(Panel4);
	Panel5 = new wxPanel(AuiNotebook1, ID_PANEL5, wxDefaultPosition, wxSize(792,464), 0, _T("ID_PANEL5"));
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	CheckBreakBlocks = new wxCheckBox(Panel5, ID_CHECKBREAKBLOCKS, _("Pad empty lines around header blocks (e.g. \'if\', \'while\'...)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKBLOCKS"));
	CheckBreakBlocks->SetValue(false);
	BoxSizer9->Add(CheckBreakBlocks, 0, wxALL|wxEXPAND, 2);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(10,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBreakBlocksAll = new wxCheckBox(Panel5, ID_CHECKBREAKBLOCKSALL, _("Treat closing header blocks (e.g. \'else\', \'catch\') as stand-alone blocks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBREAKBLOCKSALL"));
	CheckBreakBlocksAll->SetValue(false);
	BoxSizer2->Add(CheckBreakBlocksAll, 0, wxALL|wxEXPAND, 2);
	BoxSizer9->Add(BoxSizer2, 0, wxALL|wxEXPAND, 2);
	CheckPadOperators = new wxCheckBox(Panel5, ID_CHECKPADOPERATORS, _("Insert space padding around operators"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADOPERATORS"));
	CheckPadOperators->SetValue(false);
	BoxSizer9->Add(CheckPadOperators, 0, wxALL|wxEXPAND, 2);
	CheckPadParensOut = new wxCheckBox(Panel5, ID_CHECKPADPARENSOUT, _("Insert space padding around parenthesies on the outside"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADPARENSOUT"));
	CheckPadParensOut->SetValue(false);
	BoxSizer9->Add(CheckPadParensOut, 0, wxALL|wxEXPAND, 2);
	CheckPadParensIn = new wxCheckBox(Panel5, ID_CHECKPADPARENSIN, _("Insert space padding around parenthesies on the inside"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADPARENSIN"));
	CheckPadParensIn->SetValue(false);
	BoxSizer9->Add(CheckPadParensIn, 0, wxALL|wxEXPAND, 2);
	CheckPadHeader = new wxCheckBox(Panel5, ID_CHECKPADHEADER, _("Insert space padding between a header and the following parenthesis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADHEADER"));
	CheckPadHeader->SetValue(false);
	BoxSizer9->Add(CheckPadHeader, 0, wxALL|wxEXPAND, 2);
	CheckUnpadParens = new wxCheckBox(Panel5, ID_CHECKUNPADPARENS, _("Remove extra space padding around parenthesis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKUNPADPARENS"));
	CheckUnpadParens->SetValue(false);
	BoxSizer9->Add(CheckUnpadParens, 0, wxALL|wxEXPAND, 2);
	CheckDelEmptyLine = new wxCheckBox(Panel5, ID_CHECKDELEMPTYLINE, _("Delete empty lines within a function or method"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKDELEMPTYLINE"));
	CheckDelEmptyLine->SetValue(false);
	BoxSizer9->Add(CheckDelEmptyLine, 0, wxALL|wxEXPAND, 2);
	CheckFillEmptyLines = new wxCheckBox(Panel5, ID_CHECKFILLEMPTYLINES, _("Fill empty lines with the whitespace of their previous lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKFILLEMPTYLINES"));
	CheckFillEmptyLines->SetValue(false);
	BoxSizer9->Add(CheckFillEmptyLines, 0, wxALL|wxEXPAND, 2);
	CheckPadComma = new wxCheckBox(Panel5, ID_CHECKPADCOMMA, _("Pad comma"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADCOMMA"));
	CheckPadComma->SetValue(false);
	BoxSizer9->Add(CheckPadComma, 0, wxALL|wxEXPAND, 2);
	CheckPadReturnType = new wxCheckBox(Panel5, ID_CHECKPADRETURNTYPE, _("Pad return type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADRETURNTYPE"));
	CheckPadReturnType->SetValue(false);
	BoxSizer9->Add(CheckPadReturnType, 0, wxALL|wxEXPAND, 2);
	CheckUnpadReturnType = new wxCheckBox(Panel5, ID_CHECKUNPADRETURNTYPE, _("Unpad return type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKUNPADRETURNTYPE"));
	CheckUnpadReturnType->SetValue(false);
	BoxSizer9->Add(CheckUnpadReturnType, 0, wxALL|wxEXPAND, 2);
	CheckPadParamType = new wxCheckBox(Panel5, ID_CHECKPADPARAMTYPE, _("Pad param type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKPADPARAMTYPE"));
	CheckPadParamType->SetValue(false);
	BoxSizer9->Add(CheckPadParamType, 0, wxALL|wxEXPAND, 2);
	CheckUnpadParamType = new wxCheckBox(Panel5, ID_CHECKUNPADPARAMTYPE, _("Unpad param type(Object-C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKUNPADPARAMTYPE"));
	CheckUnpadParamType->SetValue(false);
	BoxSizer9->Add(CheckUnpadParamType, 0, wxALL|wxEXPAND, 2);
	GridSizer3 = new wxGridSizer(0, 4, 0, 0);
	StaticText6 = new wxStaticText(Panel5, wxID_ANY, _("Pointer alignment"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer3->Add(StaticText6, 2, wxALL|wxEXPAND, 0);
	ChoicePointerAlign = new wxChoice(Panel5, ID_CHOICEPOINTERALIGN, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEPOINTERALIGN"));
	ChoicePointerAlign->SetSelection( ChoicePointerAlign->Append(_("None")) );
	ChoicePointerAlign->Append(_("Type"));
	ChoicePointerAlign->Append(_("Middle"));
	ChoicePointerAlign->Append(_("Name"));
	GridSizer3->Add(ChoicePointerAlign, 0, wxALL|wxEXPAND, 2);
	StaticText7 = new wxStaticText(Panel5, wxID_ANY, _("Reference alignment"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer3->Add(StaticText7, 0, wxALL|wxEXPAND, 2);
	ChoiceReferenceAlign = new wxChoice(Panel5, ID_CHOICEREFERENCEALIGN, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEREFERENCEALIGN"));
	ChoiceReferenceAlign->SetSelection( ChoiceReferenceAlign->Append(_("None")) );
	ChoiceReferenceAlign->Append(_("Type"));
	ChoiceReferenceAlign->Append(_("Middle"));
	ChoiceReferenceAlign->Append(_("Name"));
	GridSizer3->Add(ChoiceReferenceAlign, 0, wxALL|wxEXPAND, 2);
	BoxSizer9->Add(GridSizer3, 0, wxALL, 2);
	Panel5->SetSizer(BoxSizer9);
	BoxSizer9->Fit(Panel5);
	BoxSizer9->SetSizeHints(Panel5);
	Panel6 = new wxPanel(AuiNotebook1, ID_PANEL6, wxDefaultPosition, wxSize(792,464), 0, _T("ID_PANEL6"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	GridSizer2 = new wxGridSizer(0, 2, 0, 0);
	StaticText8 = new wxStaticText(Panel6, wxID_ANY, _("XML version:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer2->Add(StaticText8, 0, wxALL|wxEXPAND, 2);
	EditXMLversion = new wxTextCtrl(Panel6, ID_EDITXMLVERSION, _T("1.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITXMLVERSION"));
	GridSizer2->Add(EditXMLversion, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText9 = new wxStaticText(Panel6, wxID_ANY, _("Indentation size(in spaces):"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	GridSizer2->Add(StaticText9, 0, wxALL|wxEXPAND, 2);
	EditXmlIndentSize = new wxTextCtrl(Panel6, ID_EDITXMLINDENTSIZE, _T("4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITXMLINDENTSIZE"));
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

	Connect(ID_RADIOBOXBRACKETSTYLE,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&MadSourceFormatDialog::OnRadioBoxBracketStyleSelect);
	Connect(ID_CHECKBREAKLINES,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadSourceFormatDialog::OnCheckBreakLinesClick);
	Connect(ID_CHECKBREAKBLOCKS,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadSourceFormatDialog::OnCheckBreakBlocksClick);
	//*)
}

MadSourceFormatDialog::~MadSourceFormatDialog()
{
	//(*Destroy(MadSourceFormatDialog)
	//*)
}

void MadSourceFormatDialog::OnRadioBoxBracketStyleSelect( wxCommandEvent& event )
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
	case aspsBanner: // Banner
	case aspsGnu: // GNU
	case aspsLinux: // Linux
	case aspsHorstmann: // Horstmann
	case asps1TBS: // 1TBS
	case aspsGoogle: // Google
	case aspsMozilla: // Mozilla
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

void MadSourceFormatDialog::OnCheckBreakLinesClick( wxCommandEvent& event )
{
	bool bb = CheckBreakLines->GetValue();
	EditSFMaxLineLength->Enable( bb );
	CheckBreakAfterLogical->Enable( bb );
}

void MadSourceFormatDialog::OnCheckBreakBlocksClick( wxCommandEvent& event )
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

