///////////////////////////////////////////////////////////////////////////////
// Name:		MadSearchReplaceDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	_MADSEARCHREPLACEDIALOG_H_
#define	_MADSEARCHREPLACEDIALOG_H_

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadSearchReplaceDialog)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/fileconf.h>
#include <wx/bmpbuttn.h>

#include <map>

class MadEdit;
class MadRecentList;

class MadSearchReplaceDialog: public wxDialog
{
	public:

		MadSearchReplaceDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadSearchReplaceDialog();

		//(*Declarations(MadSearchReplaceDialog)
		wxSlider* WxSliderTransDegree;
		wxButton* WxButtonReplace;
		wxButton* WxButtonFindPrev;
		wxButton* WxButtonFindNext;
		wxButton* WxButtonFindAll;
		wxRadioButton* WxRadioAlways;
		wxButton* WxButtonReplaceAll;
		wxCheckBox* WxCheckBoxMoveFocus;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxSlider* WxSliderInputSizer;
		wxCheckBox* WxCheckBoxPurgeBookmark;
		wxCheckBox* WxCheckBoxSearchInSelection;
		wxMenu WxPopupMenuRecentFindText;
		wxCheckBox* WxCheckBoxBookmarkLine;
		wxButton* WxButtonFindAllInAll;
		wxButton* WxButtonClose;
		wxRadioButton* WxRadioLosingFocus;
		wxMenu WxPopupMenuRecentReplaceText;
		wxCheckBox* WxCheckBoxFindHex;
		wxCheckBox* WxCheckBoxWholeWord;
		wxBoxSizer* BoxSizerSearch;
		wxButton* WxButtonReplaceAllInAll;
		wxButton* WxButtonCount;
		wxCheckBox* WxCheckBoxBookmarkOnly;
		wxCheckBox* WxCheckBoxRegex;
		wxCheckBox* WxCheckBoxWrapAround;
		wxBoxSizer* BoxSizerReplace;
		wxCheckBox* WxCheckBoxDotMatchNewLine;
		wxButton* WxButtonReplaceExpand;
		//*)

	protected:

		//(*Identifiers(MadSearchReplaceDialog)
		static const long ID_WXCHECKBOXMOVEFOCUS;
		static const long ID_WXCHECKBOXCASESENSITIVE;
		static const long ID_WXCHECKBOXWHOLEWORD;
		static const long ID_WXCHECKBOXREGEX;
		static const long ID_WXCHECKBOXBOXDOTMATCHNEWLINE;
		static const long ID_WXCHECKBOXFINDHEX;
		static const long ID_WXCHECKBOXSEARCHINSELECTION;
		static const long ID_WXCHECKBOXWRAPAROUND;
		static const long ID_WXCHECKBOXBOOKMARKLINE;
		static const long ID_WXCHECKBOXBOOKMARKONLY;
		static const long ID_WXCHECKBOXPURGEBOOKMARK;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_WXSLIDERTRANSDEGREE;
		static const long ID_WXSLIDERINPUTSIZER;
		static const long ID_WXBUTTONFINDNEXT;
		static const long ID_WXBUTTONFINDPREV;
		static const long ID_WXBUTTONFINDALL;
		static const long ID_WXBUTTONFINDALLINALL;
		static const long ID_WXBUTTONREPLACE;
		static const long ID_WXBUTTONREPLACEALL;
		static const long ID_WXBUTTONREPLACEALLINALL;
		static const long ID_WXBUTTONCOUNT;
		static const long ID_WXBUTTONREPLACEEXPAND;
		//*)

		static const long ID_MADEDIT1;
		static const long ID_MADEDIT2;
		static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;
		static const long ID_WXBITMAPBUTTONRECENTREPLACETEXT;

		static const long ID_RECENTFINDTEXT1;	// must be the same with MadSearchDialog
		static const long ID_RECENTFINDTEXT20;
		static const long ID_RECENTREPLACETEXT1;
		static const long ID_RECENTREPLACETEXT20;
	public:
		wxFileOffset m_SearchFrom;
		wxFileOffset m_SearchTo;
		bool m_EnableTransparency;
		bool m_ReplaceDlgUi;

		MadEdit *m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		MadRecentList *m_RecentFindText;
		MadRecentList *m_RecentReplaceText;
		wxSize m_OriginInputSize;

		void ReadWriteSettings(bool bRead);
		void UpdateCheckBoxByCBHex(bool check);
		void UpdateSearchInSelection(bool check);


	//private:
	public:
		//(*Handlers(MadSearchReplaceDialog)
		void MadSearchReplaceDialogClose(wxCloseEvent& event);
		void MadSearchReplaceDialogKeyDown(wxKeyEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
		void WxButtonFindNextClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxButtonReplaceAllClick(wxCommandEvent& event);
		void WxButtonFindPrevClick(wxCommandEvent& event);
		void WxButtonCountClick(wxCommandEvent& event);
		void WxButtonCloseClick(wxCommandEvent& event);
		void WxButtonReplaceExpandClick(wxCommandEvent& event);
		void WxCheckBoxRegexClick(wxCommandEvent& event);
		void WxRadioLosingFocusSelect(wxCommandEvent& event);
		void WxRadioAlwaysSelect(wxCommandEvent& event);
		void WxSliderTransDegreeScroll(wxScrollEvent& event);
		void WxButtonReplaceAllInAllClick(wxCommandEvent& event);
		void WxButtonFindAllClick(wxCommandEvent& event);
		void WxButtonFindAllInAllClick(wxCommandEvent& event);
		void OnWxSliderInputSizerCmdScroll(wxScrollEvent& event);
		//*)

		void MadSearchReplaceDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		void PurgeRecentFindTexts();
		void PurgeRecentReplaceTexts();
		void ReplaceAll(MadEdit	* madedit, bool	needRec=true);
		void SearchAll(MadEdit * madedit, bool needRec=true);
		void ShowReplaceUI();
		void SetTransparency();
		void ShowFindUI();
		DECLARE_EVENT_TABLE()
};

extern MadSearchReplaceDialog *g_SearchReplaceDialog;

#endif
