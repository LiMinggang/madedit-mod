///////////////////////////////////////////////////////////////////////////////
// Name:        MadFindInFilesDialog.h
// Description: find or replace in files dialog
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADFINDINFILESDIALOG_H_
#define _MADFINDINFILESDIALOG_H_

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadFindInFilesDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
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
		wxButton* WxButtonReplace;
		wxRadioButton* WxRadioButtonOpenedFiles;
		wxComboBox* WxComboBoxExclude;
		wxComboBox* WxComboBoxFilter;
		wxStaticText* StaticText2;
		wxButton* WxButtonFind;
		wxStaticText* StaticText6;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxEnableReplace;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* WxButtonClose;
		wxRadioButton* WxRadioButtonDir;
		wxButton* WxButtonActiveDir;
		wxButton* WxButtonDir;
		wxComboBox* WxComboBoxDir;
		wxComboBox* WxComboBoxEncoding;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxCheckBox* WxCheckBoxListFirstOnly;
		wxCheckBox* WxCheckBoxFindHex;
		wxStaticLine* StaticLine1;
		wxCheckBox* WxCheckBoxWholeWord;
		wxCheckBox* WxCheckBoxSubDir;
		wxCheckBox* WxCheckBoxRegex;
		wxStaticText* StaticText4;
		wxCheckBox* WxCheckBoxDotMatchNewLine;
		//*)

	protected:

		//(*Identifiers(MadFindInFilesDialog)
		static const long ID_WXCHECKBOXCASESENSITIVE;
		static const long ID_WXCHECKBOXWHOLEWORD;
		static const long ID_WXCHECKBOXREGEX;
		static const long ID_CHECKBOXDOTMATCHNEWLINE;
		static const long ID_WXCHECKBOXFINDHEX;
		static const long ID_WXCHECKBOXLISTFIRSTONLY;
		static const long ID_WXBUTTONFIND;
		static const long ID_WXBUTTONREPLACE;
		static const long ID_WXCHECKBOXENABLEREPLACE;
		static const long ID_STATICLINE1;
		static const long ID_WXRADIOBUTTONOPENEDFILES;
		static const long ID_WXRADIOBUTTONDIR;
		static const long ID_COMBOBOX1;
		static const long ID_WXBUTTONDIR;
		static const long ID_WXBUTTONACTIVEDIR;
		static const long ID_STATICTEXT1;
		static const long ID_WXCOMBOBOXFILTER;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_WXCOMBOBOXEXCLUDE;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_WXCOMBOBOXENCODING;
		static const long ID_WXCHECKBOXSUBDIR;
		//*)

		static const long ID_MADEDIT1;
		static const long ID_MADEDIT2;
		static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;
		static const long ID_WXBITMAPBUTTONRECENTREPLACETEXT;
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
		//*)

		void MadFindInFilesDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		void PurgeRecentFindDirs();
		void PurgeRecentFindFilters();
		void PurgeRecentFindExcludes();

		DECLARE_EVENT_TABLE()
};

extern MadFindInFilesDialog *g_FindInFilesDialog;

#endif
