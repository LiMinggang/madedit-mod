///////////////////////////////////////////////////////////////////////////////
// Name:        MadSearchDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADSEARCHDIALOG_H_
#define _MADSEARCHDIALOG_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
////Header Include End

#include <wx/dialog.h>
#include <wx/docview.h>
#include <wx/fileconf.h>

////Dialog Style Start
#undef MadSearchDialog_STYLE
#define MadSearchDialog_STYLE wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
////Dialog Style End

class MadEdit;
class MadRecentList;

class MadSearchDialog : public wxDialog
{
private:
    DECLARE_EVENT_TABLE()
public:
    MadSearchDialog( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Search"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = MadSearchDialog_STYLE);
    virtual ~MadSearchDialog();
public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
		wxButton *WxButtonClose;
		wxButton *WxButtonReplace;
		wxButton *WxButtonCount;
		wxButton *WxButtonFindAllInAll;
		wxMenu *WxPopupMenuRecentFindText;
		wxButton *WxButtonFindAll;
		wxButton *WxButtonFindPrev;
		wxButton *WxButtonFindNext;
		wxBoxSizer *WxBoxSizer3;
		wxCheckBox *WxCheckBoxPurgeBookmark;
		wxCheckBox *WxCheckBoxBookmarkOnly;
		wxCheckBox *WxCheckBoxBookmarkLine;
		wxStaticBoxSizer *WxStaticBoxSizer1;
		wxCheckBox *WxCheckBoxSearchInSelection;
		wxCheckBox *WxCheckBoxSearchThrEndOfFile;
		wxCheckBox *WxCheckBoxFindHex;
		wxCheckBox *WxCheckBoxRegex;
		wxCheckBox *WxCheckBoxWholeWord;
		wxCheckBox *WxCheckBoxCaseSensitive;
		wxCheckBox *WxCheckBoxMoveFocus;
		wxBoxSizer *WxBoxSizer5;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
  ////GUI Control Declaration End
        wxInt64 m_SearchFrom;
        wxInt64 m_SearchTo;
public:
    //Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
    //#defines may replace a numeric value for the enums names.
    //Try copy pasting the below block in your old Form header Files.
	enum {
////GUI Enum Control ID Start
			ID_WXBUTTONCLOSE = 42,
			ID_WXBUTTONREPLACE = 41,
			ID_WXBUTTONCOUNT = 40,
			ID_WXBUTTONFINDALLINALL = 36,
			ID_WXBUTTONFINDALL = 24,
			ID_WXBUTTONFINDPREV = 11,
			ID_WXBUTTONFINDNEXT = 10,
			ID_WXCHECKBOXPURGEBOOKMARK = 35,
			ID_WXCHECKBOXBOOKMARKONLY = 34,
			ID_WXCHECKBOXBOOKMARKLINE = 32,
			ID_WXCHECKBOXSEARCHINSELECTION = 26,
			ID_WXCHECKBOXSEARCHTHRENDOFFILE = 25,
			ID_WXCHECKBOXFINDHEX = 9,
			ID_WXCHECKBOXREGEX = 8,
			ID_WXCHECKBOXWHOLEWORD = 7,
			ID_WXCHECKBOXCASESENSITIVE = 6,
			ID_WXCHECKBOXMOVEFOCUS = 16,
////GUI Enum Control ID End
            ID_MADEDIT=1500,
            ID_RECENTFINDTEXT1=1501,
            ID_RECENTFINDTEXT20=1520,
            ID_WXBITMAPBUTTONRECENTFINDTEXT=1521,
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    void MadSearchDialogClose(wxCloseEvent& event);
    void CreateGUIControls(void);

    MadEdit *m_FindText;
    wxBitmapButton *WxBitmapButtonRecentFindText;
    MadRecentList *m_RecentFindText;
    
	void WxButtonCloseClick(wxCommandEvent& event);
	void MadSearchDialogKeyDown(wxKeyEvent& event);
	void WxButtonFindNextClick(wxCommandEvent& event);
	
	void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
	void OnRecentFindText(wxCommandEvent& event);
	void WxButtonFindPrevClick(wxCommandEvent& event);
	void WxButtonFindAllClick(wxCommandEvent& event);
	void WxButtonFindAllInAllClick(wxCommandEvent& event);

    void ReadWriteSettings(bool bRead);
    void UpdateCheckBoxByCBHex(bool check);
    void UpdateSearchInSelection(bool check);

    void WxCheckBoxFindHexClick(wxCommandEvent& event);
    void MadSearchDialogActivate(wxActivateEvent& event);
    void WxButtonReplaceClick(wxCommandEvent& event);
    void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
    void WxButtonCountClick(wxCommandEvent& event);
    void PurgeRecentFindTexts();
    void SearchAll(MadEdit * madedit, bool needRec=true);
};

extern MadSearchDialog *g_SearchDialog;

#endif
 
 
 
 
