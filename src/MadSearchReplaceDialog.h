///////////////////////////////////////////////////////////////////////////////
// Name:		MadSearchReplaceDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	_MADSEARCHREPLACEDIALOG_H_
#define	_MADSEARCHREPLACEDIALOG_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
		#pragma	hdrstop
#endif
#ifndef	WX_PRECOMP
		#include <wx/wx.h>
#endif

//Do not add custom	headers.
//wx-dvcpp designer	will remove	them
////Header Include Start
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
////Header Include End

#include <wx/dialog.h>
#include <wx/docview.h>
#include <wx/fileconf.h>

////Dialog Style Start
#undef MadSearchReplaceDialog_STYLE
#define	MadSearchReplaceDialog_STYLE wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
////Dialog Style End

class MadEdit;
class MadRecentList;

class MadSearchReplaceDialog : public wxDialog
{
private:
	DECLARE_EVENT_TABLE()
public:
	MadSearchReplaceDialog(	wxWindow *parent, wxWindowID id	= 1, const wxString	&title = wxT("SearchReplace"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = MadSearchReplaceDialog_STYLE);
	virtual	~MadSearchReplaceDialog();
public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them.	Try	adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
		wxMenu *WxPopupMenuRecentReplaceText;
		wxMenu *WxPopupMenuRecentFindText;
		wxButton *WxButtonClose;
		wxButton *WxButtonReplaceAllInAll;
		wxButton *WxButtonReplaceAll;
		wxButton *WxButtonReplace;
		wxButton *WxButtonReplaceExpand;
		wxButton *WxButtonCount;
		wxButton *WxButtonFindAllInAll;
		wxButton *WxButtonFindAll;
		wxButton *WxButtonFindPrev;
		wxButton *WxButtonFindNext;
		wxBoxSizer *WxBoxSizer3;
		wxSlider *WxSliderTransDegree;
		wxRadioButton *WxRadioAlways;
		wxRadioButton *WxRadioLosingFocus;
		wxStaticBoxSizer *WxStaticBoxSizer2;
		wxCheckBox *WxCheckBoxPurgeBookmark;
		wxCheckBox *WxCheckBoxBookmarkOnly;
		wxCheckBox *WxCheckBoxBookmarkLine;
		wxStaticBoxSizer *WxStaticBoxSizer1;
		wxBoxSizer *WxBoxSizer6;
		wxCheckBox *WxCheckBoxDotMatchNewLine;
		wxCheckBox *WxCheckBoxSearchInSelection;
		wxCheckBox *WxCheckBoxSearchThrEndOfFile;
		wxCheckBox *WxCheckBoxFindHex;
		wxCheckBox *WxCheckBoxRegex;
		wxCheckBox *WxCheckBoxWholeWord;
		wxCheckBox *WxCheckBoxCaseSensitive;
		wxCheckBox *WxCheckBoxMoveFocus;
		wxBoxSizer *WxBoxSizer8;
		wxBoxSizer *WxBoxSizer5;
		wxBoxSizer *WxBoxSizer7;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
  ////GUI Control Declaration End
		wxInt64	m_SearchFrom;
		wxInt64	m_SearchTo;
		bool m_EnableTransparency;
		
		MadEdit	*m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		MadRecentList *m_RecentFindText, *m_RecentReplaceText;
		bool m_ReplaceDlgUi;
public:
	//Note:	if you receive any error with these	enums, then	you	need to
	//change your old form code	that are based on the #define control ids.
	//#defines may replace a numeric value for the enums names.
	//Try copy pasting the below block in your old Form	header Files.
	static const long ID_WXCHECKBOXBOXDOTMATCHNEWLINE;
	static const long ID_WXBUTTONCLOSE;
	static const long ID_WXBUTTONREPLACEALLINALL;
	static const long ID_WXBUTTONREPLACEALL;
	static const long ID_WXBUTTONREPLACE;
	static const long ID_WXBUTTONREPLACEEXPAND;
	static const long ID_WXBUTTONCOUNT;
	static const long ID_WXBUTTONFINDALLINALL;
	static const long ID_WXBUTTONFINDALL;
	static const long ID_WXBUTTONFINDPREV;
	static const long ID_WXBUTTONFINDNEXT;
	static const long ID_WXSLIDERTRANSDEGREE;
	static const long ID_WXRADIOALWAYS;
	static const long ID_WXRADIOLOSINGFOCUS;
	static const long ID_WXCHECKBOXPURGEBOOKMARK;
	static const long ID_WXCHECKBOXBOOKMARKONLY;
	static const long ID_WXCHECKBOXBOOKMARKLINE;
	static const long ID_WXCHECKBOXSEARCHINSELECTION;
	static const long ID_WXCHECKBOXSEARCHTHRENDOFFILE;
	static const long ID_WXCHECKBOXFINDHEX;
	static const long ID_WXCHECKBOXREGEX;
	static const long ID_WXCHECKBOXWHOLEWORD;
	static const long ID_WXCHECKBOXCASESENSITIVE;
	static const long ID_WXCHECKBOXMOVEFOCUS;
	static const long ID_MADEDIT1;
	static const long ID_MADEDIT2;
	static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;
	static const long ID_WXBITMAPBUTTONRECENTREPLACETEXT;

	static const long ID_RECENTFINDTEXT1;	// must be the same with MadSearchDialog
	static const long ID_RECENTFINDTEXT20;
	static const long ID_RECENTREPLACETEXT1;
	static const long ID_RECENTREPLACETEXT20;

public:
	void MadSearchReplaceDialogClose(wxCloseEvent& event);
	void CreateGUIControls(void);
	void WxButtonCloseClick(wxCommandEvent&	event);
	void MadSearchReplaceDialogKeyDown(wxKeyEvent& event);
	void WxButtonFindNextClick(wxCommandEvent& event);
	void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
	void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
	void OnRecentFindText(wxCommandEvent& event);
	void OnRecentReplaceText(wxCommandEvent& event);
	void WxButtonFindPrevClick(wxCommandEvent& event);
	void WxButtonFindAllClick(wxCommandEvent& event);
	void WxButtonFindAllInAllClick(wxCommandEvent& event);
	void ReadWriteSettings(bool	bRead);
	void UpdateCheckBoxByCBHex(bool	check);
	void UpdateSearchInSelection(bool check);

	void WxCheckBoxFindHexClick(wxCommandEvent&	event);
	void MadSearchReplaceDialogActivate(wxActivateEvent& event);
	void WxButtonReplaceClick(wxCommandEvent& event);
	void MadReplaceDialogActivate(wxActivateEvent& event);
	void WxButtonReplaceAllClick(wxCommandEvent& event);
	void WxButtonReplaceAllInAllClick(wxCommandEvent& event);
	void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
	void WxButtonCountClick(wxCommandEvent&	event);
	void PurgeRecentFindTexts();
	void PurgeRecentReplaceTexts();
	void ReplaceAll(MadEdit	* madedit, bool	needRec=true);
	void SearchAll(MadEdit * madedit, bool needRec=true);
	void WxRadioLosingFocusClick(wxCommandEvent& event);
	void WxRadioAlwaysClick(wxCommandEvent&	event);
	void WxSliderTransDegreeScroll(wxScrollEvent& event);
	void WxButtonReplaceExpandClick(wxCommandEvent&	event);
	void ShowReplaceUI();
	void SetTransparency();
	void ShowFindUI();
	void WxCheckBoxRegexClick(wxCommandEvent& event);
};

extern MadSearchReplaceDialog *g_SearchReplaceDialog;

#endif

