///-----------------------------------------------------------------
///
/// @file      MadNumberDlg.h
/// @author    user
/// Created:   2014/9/17 14:31:05
/// @section   DESCRIPTION
///            MadNumberDlg class declaration
///
///------------------------------------------------------------------

#ifndef __MADNUMBERDLG_H__
#define __MADNUMBERDLG_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
////Header Include End

////Dialog Style Start
#undef MadNumberDlg_STYLE
#define MadNumberDlg_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

#define NUMBERING_MAX_CHARS 512

class MadNumberDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		MadNumberDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = _("Numbering Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadNumberDlg_STYLE);
		virtual ~MadNumberDlg();
		void WxOKButtonClick(wxCommandEvent& event);		
	
	//private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton2;
		wxButton *WxOKButton;
		wxBoxSizer *WxBoxSizer5;
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
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXALIGN = 1119,
			ID_WXSTATICTEXT4 = 1118,
			ID_WXFORMAT = 1117,
			ID_WXSTATICTEXT3 = 1116,
			ID_WXCHOICENUMBERSTEPTYPE = 1115,
			ID_WXSTATICTEXT2 = 1113,
			ID_PADWITH = 1126,
			ID_WXEDITNUMBEROFCHARS = 1125,
			ID_WXSTATICTEXT5 = 1124,
			ID_EDITNUMBERINGSTEP = 1123,
			ID_WXSTATICTEXT8 = 1122,
			ID_WXEDITINITIALNUMBER = 1121,
			ID_WXSTATICTEXT1 = 1120,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

extern MadNumberDlg * g_MadNumberDlg;
#endif
