///////////////////////////////////////////////////////////////////////////////
// Name:		MadEditApp.cpp
// Description:
// Author:		minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef __MADPURGEHISTORYDIALOG_H__
#define __MADPURGEHISTORYDIALOG_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
	#include <wx/dialog.h>

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
////Header Include End

////Dialog Style Start
#undef MadPurgeHistoryDialog_STYLE
#define MadPurgeHistoryDialog_STYLE wxCAPTION | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxCLOSE_BOX
////Dialog Style End

class MadPurgeHistoryDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxCheckBox *WxCheckBoxAllAbove;
		wxCheckBox *WxCheckBoxCaretPos;
		wxCheckBox *wxCheckBoxRecentSearchedExcludeFilters;
		wxCheckBox *wxRecentSearchedFileFilters;
		wxCheckBox *wxCheckBoxRecentSearchedDirectories;
		wxCheckBox *wxCheckBoxRecentReplacedTexts;
		wxCheckBox *wxCheckBoxRecentSearchedTexts;
		wxCheckBox *wxCheckBoxRecentEncodings;
		wxCheckBox *wxCheckBoxRecentFonts;
		wxCheckBox *wxCheckBoxRecentFiles;
		MadPurgeHistoryDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Purge History"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadPurgeHistoryDialog_STYLE);
		virtual ~MadPurgeHistoryDialog();
		void WxButtonOKClick(wxCommandEvent& event);
		void wxButtonCancelClick(wxCommandEvent& event);
		void MadPurgeHistoryDialogKeyDown(wxKeyEvent& event);
        void OnAllAboveClick(wxCommandEvent& event);
		wxButton *wxButtonCancel;
		wxButton *WxButtonOK;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
		wxStaticBoxSizer *WxStaticBoxSizer1;
		////GUI Control Declaration End

	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_DUMMY_START = 1000,
			ID_WXCHECKBOXALLABOVE,
            ID_WXCHECKBOXCARETPOS,
			ID_WXCHECKBOXRECENTSEARCHEDEXCLUDEFILTERS,
			ID_WXRECENTSEARCHEDFILEFILTERS,
			ID_WXCHECKBOXRECENTSEARCHEDDIRECTORIES,
			ID_WXCHECKBOXRECENTREPLACEDTEXTS,
			ID_WXCHECKBOXRECENTSEARCHEDTEXTS,
			ID_WXCHECKBOXRECENTENCODINGS,
			ID_WXCHECKBOXRECENTFONTS,
			ID_WXCHECKBOXRECENTFILES,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
