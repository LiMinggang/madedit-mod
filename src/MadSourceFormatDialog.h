#ifndef MADSOURCEFORMATDIALOG_H
#define MADSOURCEFORMATDIALOG_H

//(*Headers(MadSourceFormatDialog)
#include <wx/aui/aui.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#define MADEDIT_USE_MADEDIT

#ifdef MADEDIT_USE_STC
#include <wx/stc/stc.h>
#endif

#ifdef MADEDIT_USE_MADEDIT
class MadEdit;
#endif
#ifdef MADEDIT_ENABLE_STC
enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};
#endif

class MadSourceFormatDialog: public wxDialog
{
	public:

		MadSourceFormatDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadSourceFormatDialog();

		//(*Declarations(MadSourceFormatDialog)
		wxCheckBox* CheckAddBraces;
		wxCheckBox* CheckAddOneLineBraces;
		wxCheckBox* CheckAttachClasses;
		wxCheckBox* CheckAttachClosingWhile;
		wxCheckBox* CheckAttachExternC;
		wxCheckBox* CheckAttachInlines;
		wxCheckBox* CheckAttachNamespaces;
		wxCheckBox* CheckBreakAfterLogical;
		wxCheckBox* CheckBreakBlocks;
		wxCheckBox* CheckBreakBlocksAll;
		wxCheckBox* CheckBreakClosing;
		wxCheckBox* CheckBreakElseIfs;
		wxCheckBox* CheckBreakLines;
		wxCheckBox* CheckBreakOneLineHeaders;
		wxCheckBox* CheckCloseTemplates;
		wxCheckBox* CheckConvertTabs;
		wxCheckBox* CheckDelEmptyLine;
		wxCheckBox* CheckFillEmptyLines;
		wxCheckBox* CheckForceUseTabs;
		wxCheckBox* CheckIndentAfterParens;
		wxCheckBox* CheckIndentCase;
		wxCheckBox* CheckIndentClasses;
		wxCheckBox* CheckIndentCol1Comments;
		wxCheckBox* CheckIndentLabels;
		wxCheckBox* CheckIndentModifiers;
		wxCheckBox* CheckIndentNamespaces;
		wxCheckBox* CheckIndentPreprocBlock;
		wxCheckBox* CheckIndentPreprocCond;
		wxCheckBox* CheckIndentPreprocDefine;
		wxCheckBox* CheckIndentSwitches;
		wxCheckBox* CheckKeepBlocks;
		wxCheckBox* CheckKeepComplex;
		wxCheckBox* CheckPadComma;
		wxCheckBox* CheckPadHeader;
		wxCheckBox* CheckPadOperators;
		wxCheckBox* CheckPadParamType;
		wxCheckBox* CheckPadParensIn;
		wxCheckBox* CheckPadParensOut;
		wxCheckBox* CheckPadReturnType;
		wxCheckBox* CheckRemoveBraces;
		wxCheckBox* CheckRemoveCommentPrefix;
		wxCheckBox* CheckUnpadParamType;
		wxCheckBox* CheckUnpadParens;
		wxCheckBox* CheckUnpadReturnType;
		wxCheckBox* CheckUseTab;
		wxChoice* ChoicePointerAlign;
		wxChoice* ChoiceReferenceAlign;
		wxRadioBox* RadioBoxBracketStyle;
		wxSpinCtrl* SpinIndentation;
		wxSpinCtrl* SpinMinConditionalEvent;
		wxTextCtrl* EditIndentContinuation;
		wxTextCtrl* EditMaxContinuationIndent;
		wxTextCtrl* EditSFMaxLineLength;
		wxTextCtrl* EditXMLversion;
		wxTextCtrl* EditXmlIndentSize;
#ifdef MADEDIT_USE_MADEDIT
		MadEdit *TextSample;
#elif MADEDIT_USE_STC
		wxStyledTextCtrl *TextSample;
#else
		wxTextCtrl *TextSample;
#endif
		//*)

	protected:

		//(*Identifiers(MadSourceFormatDialog)
		//static const long ID_RADIOBOXBRACKETSTYLE;
		//static const long ID_TEXTSAMPLE;
		//static const long ID_PANEL1;
		//static const long ID_CHECKATTACHCLASSES;
		//static const long ID_CHECKATTACHEXTERNC;
		//static const long ID_CHECKATTACHNAMESPACES;
		//static const long ID_CHECKATTACHINLINES;
		//static const long ID_CHECKATTACHCLOSINGWHILE;
		//static const long ID_PANEL2;
		//static const long ID_SPINCTRLINDENTSIZE;
		//static const long ID_CHECKUSETABOVERSPACES;
		//static const long ID_CHECKFORCETABS;
		//static const long ID_CHECKINDENTCASE;
		//static const long ID_CHECKINDENETCLASSES;
		//static const long ID_CHECKINDENTLABELS;
		//static const long ID_CHECKINDENTMODIFIERS;
		//static const long ID_CHECKINDENTNS;
		//static const long ID_CHECKINDENTSWITCHS;
		//static const long ID_CHECKINDENTPREP;
		//static const long ID_CHECKINDENTMULTIPREP;
		//static const long ID_CHECKINDENTPREPCOND;
		//static const long ID_CHECKINDENETCPPCOMMENTS;
		//static const long ID_CHECKINDENTAFTERPARENS;
		//static const long ID_SPINCTRLMININDENT;
		//static const long ID_EDITMAXCONTINUATIONINDENT;
		//static const long ID_EDITINDENTCONTINUATION;
		//static const long ID_PANEL3;
		//static const long ID_CHECKBREAKCLOSING;
		//static const long ID_CHECKBREAKELSEIFS;
		//static const long ID_CHECKADDBRACKETS;
		//static const long ID_CHECKADDONELINEBRACKETS;
		//static const long ID_CHECKBOXREMOVEBRACKETS;
		//static const long ID_CHECKKEEPBLOCKS;
		//static const long ID_CHECKKEEPCOMPLEX;
		//static const long ID_CHECKCONVERTTABS;
		//static const long ID_CHECKCLOSETEMPLATES;
		//static const long ID_CHECKREMOVECOMMENTPREFIX;
		//static const long ID_CHECKBREAKONELINEHEADERS;
		//static const long ID_CHECKBREAKLINES;
		//static const long ID_EDITSFMAXLINELENGTH;
		//static const long ID_CHECKBREAKAFTERLOGICAL;
		//static const long ID_PANEL4;
		//static const long ID_CHECKBREAKBLOCKS;
		//static const long ID_CHECKBREAKBLOCKSALL;
		//static const long ID_CHECKPADOPERATORS;
		//static const long ID_CHECKPADPARENSOUT;
		//static const long ID_CHECKPADPARENSIN;
		//static const long ID_CHECKPADHEADER;
		//static const long ID_CHECKUNPADPARENS;
		//static const long ID_CHECKDELEMPTYLINE;
		//static const long ID_CHECKFILLEMPTYLINES;
		//static const long ID_CHECKPADCOMMA;
		//static const long ID_CHECKPADRETURNTYPE;
		//static const long ID_CHECKUNPADRETURNTYPE;
		//static const long ID_CHECKPADPARAMTYPE;
		//static const long ID_CHECKUNPADPARAMTYPE;
		//static const long ID_CHOICEPOINTERALIGN;
		//static const long ID_STATICTEXT7;
		//static const long ID_CHOICEREFERENCEALIGN;
		//static const long ID_PANEL5;
		//static const long ID_EDITXMLVERSION;
		//static const long ID_EDITXMLINDENTSIZE;
		//static const long ID_PANEL6;
		//static const long ID_AUINOTEBOOK1;
		//*)

	private:

		//(*Handlers(MadSourceFormatDialog)
		void OnRadioBoxBracketStyleSelect(wxCommandEvent& event);
		void OnCheckBreakLinesClick(wxCommandEvent& event);
		void OnCheckBreakBlocksClick(wxCommandEvent& event);
		void OnRadioBoxBracketStyleSelect1(wxCommandEvent& event);
		//*)
	public:
		void LoadSettings(void);
};

#endif
