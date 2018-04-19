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
