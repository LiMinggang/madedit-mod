///////////////////////////////////////////////////////////////////////////////
// Name:        MadFindInFilesDialog.h
// Description: find or replace in files dialog
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADFINDINFILESDIALOG_H_
#define _MADFINDINFILESDIALOG_H_
#include "MadUtils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadFindInFilesDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/fileconf.h>
#include <wx/bmpbuttn.h>

class MadEdit;
class MadRecentList;

class MadFindInFilesDialog: public wxDialog
{
	public:

		MadFindInFilesDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadFindInFilesDialog();

		//(*Declarations(MadFindInFilesDialog)
		wxButton* WxButtonActiveDir;
		wxButton* WxButtonClose;
		wxButton* WxButtonDir;
		wxButton* WxButtonFind;
		wxButton* WxButtonReplace;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxDotMatchNewLine;
		wxCheckBox* WxCheckBoxEnableReplace;
		wxCheckBox* WxCheckBoxFindHex;
		wxCheckBox* WxCheckBoxListFirstOnly;
		wxCheckBox* WxCheckBoxRegex;
		wxCheckBox* WxCheckBoxSubDir;
		wxCheckBox* WxCheckBoxWholeWord;
		wxComboBox* WxComboBoxDir;
		wxComboBox* WxComboBoxEncoding;
		wxComboBox* WxComboBoxExclude;
		wxComboBox* WxComboBoxFilter;
		wxRadioButton* WxRadioButtonDir;
		wxRadioButton* WxRadioButtonOpenedFiles;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		//*)

	protected:

		//(*Identifiers(MadFindInFilesDialog)
		//*)

		static const long ID_RECENTFINDTEXT1;    // must be the same with MadSearchDialog
		static const long ID_RECENTFINDTEXT20;
		static const long ID_RECENTREPLACETEXT1;
		static const long ID_RECENTREPLACETEXT20;

	public:
		MadEdit *m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		MadRecentList *m_RecentFindDir;
		MadRecentList *m_RecentFindFilter, *m_RecentFindExclude;
		void UpdateCheckBoxByCBHex(bool check);
		void FindReplaceInFiles(bool bReplace);//false: find; true: replace
	//private:
	public:
		//(*Handlers(MadFindInFilesDialog)
		void MadFindInFilesDialogClose(wxCloseEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxButtonFindClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxCheckBoxEnableReplaceClick(wxCommandEvent& event);
		void WxButtonDirClick(wxCommandEvent& event);
		void WxButtonActiveDirClick(wxCommandEvent& event);
		void WxCheckBoxRegexClick(wxCommandEvent& event);
		void MadFindInFilesDialogKeyDown(wxKeyEvent& event);
		//*)

		void MadFindInFilesDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		void PurgeRecentFindDirs();
		void PurgeRecentFindFilters();
		void PurgeRecentFindExcludes();
};

extern MadFindInFilesDialog *g_FindInFilesDialog;

#endif
