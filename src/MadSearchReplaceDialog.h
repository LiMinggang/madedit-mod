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
		wxCheckBox *WxCheckBoxSearchInSelection;
		wxCheckBox *WxCheckBoxSearchThrEndOfFile;
		wxCheckBox *WxCheckBoxFindHex;
		wxCheckBox *WxCheckBoxRegex;
		wxCheckBox *WxCheckBoxWholeWord;
		wxCheckBox *WxCheckBoxCaseSensitive;
		wxCheckBox *WxCheckBoxMoveFocus;
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
	enum {
////GUI	Enum Control ID	Start
			ID_WXBUTTONCLOSE = 57,
			ID_WXBUTTONREPLACEALLINALL = 56,
			ID_WXBUTTONREPLACEALL =	55,
			ID_WXBUTTONREPLACE = 54,
			ID_WXBUTTONREPLACEEXPAND = 41,
			ID_WXBUTTONCOUNT = 40,
			ID_WXBUTTONFINDALLINALL	= 36,
			ID_WXBUTTONFINDALL = 24,
			ID_WXBUTTONFINDPREV	= 11,
			ID_WXBUTTONFINDNEXT	= 10,
			ID_WXSLIDERTRANSDEGREE = 48,
			ID_WXRADIOALWAYS = 47,
			ID_WXRADIOLOSINGFOCUS =	46,
			ID_WXCHECKBOXPURGEBOOKMARK = 35,
			ID_WXCHECKBOXBOOKMARKONLY =	34,
			ID_WXCHECKBOXBOOKMARKLINE =	32,
			ID_WXCHECKBOXSEARCHINSELECTION = 26,
			ID_WXCHECKBOXSEARCHTHRENDOFFILE	= 25,
			ID_WXCHECKBOXFINDHEX = 9,
			ID_WXCHECKBOXREGEX = 8,
			ID_WXCHECKBOXWHOLEWORD = 7,
			ID_WXCHECKBOXCASESENSITIVE = 6,
			ID_WXCHECKBOXMOVEFOCUS = 16,
////GUI	Enum Control ID	End
			ID_MADEDIT1	= 1400,
			ID_MADEDIT2	= 1401,
			ID_WXBITMAPBUTTONRECENTFINDTEXT=1402,
			ID_WXBITMAPBUTTONRECENTREPLACETEXT=1403,

			ID_RECENTFINDTEXT1=1501,	// must	be the same	with MadSearchDialog
			ID_RECENTFINDTEXT20=1520,
			ID_RECENTREPLACETEXT1=ID_RECENTFINDTEXT20+1,
			ID_RECENTREPLACETEXT20=ID_RECENTREPLACETEXT1+19,
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End	of Enum

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
};

extern MadSearchReplaceDialog *g_SearchReplaceDialog;

#endif

