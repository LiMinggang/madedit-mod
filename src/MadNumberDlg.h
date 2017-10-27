///////////////////////////////////////////////////////////////////////////////
// Name:		MadNumberDlg.h
// Description:
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	__MADNUMBERDLG_H__
#define	__MADNUMBERDLG_H__

#ifdef __BORLANDC__
	#pragma	hdrstop
#endif

#include <wx/wxprec.h>
#ifndef	WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/dialog.h>

//Do not add custom	headers	between	
//Header Include Start and Header Include End.
//wxDev-C++	designer will remove them. Add custom headers after	the	block.
////Header Include Start
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
////Header Include End

#if __cplusplus <= 199711L
#ifndef nullptr
	#define nullptr (0)
#endif
#endif

////Dialog Style Start
#undef MadNumberDlg_STYLE
#define	MadNumberDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT |	wxCLOSE_BOX
////Dialog Style End

#define	NUMBERING_MAX_CHARS	512

class MadNumberDlg : public	wxDialog
{
	private:
		
	public:
		MadNumberDlg(wxWindow *parent, wxWindowID id = wxID_ANY, const	wxString &title	= _("Numbering Configuration"), const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadNumberDlg_STYLE);
		virtual	~MadNumberDlg();
		void WxOKButtonClick(wxCommandEvent& event);		
	
	//private:
		//Do not add custom	control	declarations between 
		//GUI Control Declaration Start	and	GUI	Control	Declaration	End.
		//wxDev-C++	will remove	them. Add custom code after	the	block.
		////GUI	Control	Declaration	Start
		wxStaticText *WxStaticTextPreview;
		wxStaticText *WxStaticText6;
		wxButton *WxCancelButton;
		wxButton *WxOKButton;
		wxBoxSizer *WxBoxSizer5;
		wxBoxSizer *WxBoxSizer8;
		wxTextCtrl *WxEditPostfix;
		wxCheckBox *WxCheckPostfix;
		wxBoxSizer *WxBoxSizer7;
		wxTextCtrl *WxEditPrefix;
		wxCheckBox *WxCheckPrefix;
		wxBoxSizer *WxBoxSizer6;
		wxBoxSizer *WxBoxSizer3;
		wxChoice *WxChoiceAlign;
		wxStaticText *WxStaticText4;
		wxChoice *WxChoiceFormat;
		wxStaticText *WxStaticText3;
		wxChoice *WxChoiceNumberStepType;
		wxStaticText *WxStaticText2;
		wxBoxSizer *WxBoxSizer4;
		wxCheckBox *WxPadChar;
		wxTextCtrl *WxEditNumberOfChars;
		wxStaticText *WxStaticText5;
		wxTextCtrl *WxEditNumberingStep;
		wxStaticText *WxStaticText8;
		wxTextCtrl *WxEditInitialNumber;
		wxStaticText *WxStaticText1;
		wxBoxSizer *WxBoxSizer11;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
		////GUI	Control	Declaration	End
		void WxEditItialnumberUpdated(wxCommandEvent& event);
		void WxEditPrefixUpdated(wxCommandEvent& event);
		void WxEditPostfixUpdated(wxCommandEvent& event);
		void WxCheckPrefixClick(wxCommandEvent&	event);
		void WxCheckPostfixClick(wxCommandEvent& event);
		
	private:
		//Note:	if you receive any error with these	enum IDs, then you need	to
		//change your old form code	that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum	names.
		//Try copy and pasting the below block in your old form	header files.
		//static const long ID_WXSTATICTEXTPREVIEW;
		//static const long ID_WXSTATICTEXT6;
		//static const long ID_WXEDITPOSTFIX;
		//static const long ID_WXCHECKPOSTFIX;
		//static const long ID_WXEDITPREFIX;
		//static const long ID_WXCHECKPREFIX;
		//static const long ID_WXALIGN;
		//static const long ID_WXSTATICTEXT4;
		//static const long ID_WXFORMAT;
		//static const long ID_WXSTATICTEXT3;
		//static const long ID_WXCHOICENUMBERSTEPTYPE;
		//static const long ID_WXSTATICTEXT2;
		//static const long ID_PADWITH;
		//static const long ID_WXEDITNUMBEROFCHARS;
		//static const long ID_WXSTATICTEXT5;
		//static const long ID_EDITNUMBERINGSTEP;
		//static const long ID_WXSTATICTEXT8;
		//static const long ID_WXEDITINITIALNUMBER;
		//static const long ID_WXSTATICTEXT1;

	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

extern MadNumberDlg	* g_MadNumberDlg;
#endif
