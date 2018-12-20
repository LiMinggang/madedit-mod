///////////////////////////////////////////////////////////////////////////////
// Name:		MadPurgeHistoryDialog.h
// Description:
// Author:		minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef __MADPURGEHISTORYDIALOG_H__
#define __MADPURGEHISTORYDIALOG_H__

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadPurgeHistoriesDialog)
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class MadPurgeHistoryDialog : public wxDialog
{
	public:

		MadPurgeHistoryDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadPurgeHistoryDialog();

		//(*Declarations(MadPurgeHistoryDialog)
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		wxCheckBox* wxCheckBoxAllAbove;
		wxCheckBox* wxCheckBoxCaretPos;
		wxCheckBox* wxCheckBoxRecentEncodings;
		wxCheckBox* wxCheckBoxRecentFiles;
		wxCheckBox* wxCheckBoxRecentFonts;
		wxCheckBox* wxCheckBoxRecentReplacedTexts;
		wxCheckBox* wxCheckBoxRecentSearchedDirectories;
		wxCheckBox* wxCheckBoxRecentSearchedExcludeFilters;
		wxCheckBox* wxCheckBoxRecentSearchedTexts;
		wxCheckBox* wxCheckBoxResetToolBarsInfoWin;
		wxCheckBox* wxCheckBoxResetTransparency;
		wxCheckBox* wxRecentSearchedFileFilters;
		//*)

	protected:

		//(*Identifiers(MadPurgeHistoryDialog)
		//*)

	private:

		//(*Handlers(MadPurgeHistoryDialog)
		void MadPurgeHistoryDialogClose(wxCloseEvent& event);
		void WxButtonPurgeClick(wxCommandEvent& event);
		void WxButtonOKClick(wxCommandEvent& event);
		void WxButtonCancelClick(wxCommandEvent& event);
		void wxCheckBoxAllAboveClick(wxCommandEvent& event);
		void MadPurgeHistoryDialogKeyDown(wxKeyEvent& event);
		//*)
};

#endif
