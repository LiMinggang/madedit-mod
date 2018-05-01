///////////////////////////////////////////////////////////////////////////////
// Name:		MadSearchReplaceDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	_MADSEARCHREPLACEDIALOG_H_
#define	_MADSEARCHREPLACEDIALOG_H_
#include "MadUtils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadSearchReplaceDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/menu.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/slider.h>
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
		wxBoxSizer* BoxSizer4;
		wxBoxSizer* BoxSizerReplace;
		wxBoxSizer* BoxSizerSearch;
		wxButton* WxButtonClose;
		wxButton* WxButtonCount;
		wxButton* WxButtonFindAll;
		wxButton* WxButtonFindAllInAll;
		wxButton* WxButtonFindNext;
		wxButton* WxButtonFindPrev;
		wxButton* WxButtonReplace;
		wxButton* WxButtonReplaceAll;
		wxButton* WxButtonReplaceAllInAll;
		wxButton* WxButtonReplaceExpand;
		wxCheckBox* WxCheckBoxBookmarkLine;
		wxCheckBox* WxCheckBoxBookmarkOnly;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxDotMatchNewLine;
		wxCheckBox* WxCheckBoxFindHex;
		wxCheckBox* WxCheckBoxMoveFocus;
		wxCheckBox* WxCheckBoxPurgeBookmark;
		wxCheckBox* WxCheckBoxRegex;
		wxCheckBox* WxCheckBoxSearchInSelection;
		wxCheckBox* WxCheckBoxWholeWord;
		wxCheckBox* WxCheckBoxWrapAround;
		wxCheckBox* wxCheckBoxReplaceNoDoubleCheck;
		wxMenu WxPopupMenuRecentFindText;
		wxMenu WxPopupMenuRecentReplaceText;
		wxRadioButton* WxRadioAlways;
		wxRadioButton* WxRadioLosingFocus;
		wxSlider* WxSliderInputSizer;
		wxSlider* WxSliderTransDegree;
		//*)

	protected:

		//(*Identifiers(MadSearchReplaceDialog)
		//*)

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

		void OnEditUndo( wxCommandEvent& WXUNUSED(event) );
		void OnEditRedo( wxCommandEvent& WXUNUSED(event) );
		void OnEditCut( wxCommandEvent& WXUNUSED(event) );
		void OnEditCopy( wxCommandEvent& WXUNUSED(event) );
		void OnEditPaste( wxCommandEvent& WXUNUSED(event) );
		void OnEditDelete( wxCommandEvent& WXUNUSED(event) );
		void OnEditCutLine( wxCommandEvent& WXUNUSED(event) );
		void OnEditDeleteLine( wxCommandEvent& WXUNUSED(event) );
		void OnEditSelectAll( wxCommandEvent& WXUNUSED(event) );
		void OnEditStartEndSelction( wxCommandEvent& WXUNUSED(event) );
		void OnEditInsertTabChar( wxCommandEvent& WXUNUSED(event) );
		void OnEditInsertDateTime( wxCommandEvent& WXUNUSED(event) );
		
		void OnEditSortAscending( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortDescending( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortAscendingCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortDescendingCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortByOptions( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortOptions( wxCommandEvent& WXUNUSED(event) );
		
		void OnEditCopyAsHexString( wxCommandEvent& WXUNUSED(event) );
		void OnEditCopyAsHexStringWithSpace( wxCommandEvent& WXUNUSED(event) );
		void OnEditCopyRevertHex( wxCommandEvent& WXUNUSED(event) );
		void OnEditIncIndent( wxCommandEvent& WXUNUSED(event) );
		void OnEditDecIndent( wxCommandEvent& WXUNUSED(event) );
		void OnEditComment( wxCommandEvent& WXUNUSED(event) );
		void OnEditUncomment( wxCommandEvent& WXUNUSED(event) );
		void OnEditWordWrapToNewLine( wxCommandEvent& WXUNUSED(event) );
		void OnEditNewLineToWordWrap( wxCommandEvent& WXUNUSED(event) );
		void OnEditToUpperCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditToLowerCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditInvertCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditCapitalize( wxCommandEvent& WXUNUSED(event) );
		void OnEditToHalfWidth( wxCommandEvent& WXUNUSED(event) );
		void OnEditToHalfWidthByOptions( wxCommandEvent& WXUNUSED(event) );
		void OnEditToFullWidth( wxCommandEvent& WXUNUSED(event) );
		void OnEditToFullWidthByOptions( wxCommandEvent& WXUNUSED(event) );
		void OnEditTabToSpace( wxCommandEvent& WXUNUSED(event) );
		void OnEditSpaceToTab( wxCommandEvent& WXUNUSED(event) );
		void OnEditTrimTrailingSpaces( wxCommandEvent& WXUNUSED(event) );
		void OnEditTrimLeadingSpaces( wxCommandEvent& WXUNUSED(event) );
		void OnEditDeleteEmptyLines( wxCommandEvent& WXUNUSED(event) );
		void OnEditDeleteEmptyLinesWithSpaces( wxCommandEvent& WXUNUSED(event) );
		void OnEditJoinLines( wxCommandEvent& WXUNUSED(event) );
		void OnEditInsertNumbers( wxCommandEvent& WXUNUSED(event) );
		void OnEditColumnAlignLeft( wxCommandEvent& WXUNUSED(event) );
		void OnEditColumnAlignRight( wxCommandEvent& WXUNUSED(event) );
		void OnEditSpellCheck( wxCommandEvent& WXUNUSED(WXUNUSED(event)) );
		void OnToolsMadScriptList( wxCommandEvent& WXUNUSED(WXUNUSED(event)) );
		
		typedef struct 
		{
			const long evtTag;
			void (MadSearchReplaceDialog::*method)( wxCommandEvent &);
		} wxCmdEvtHandlerMap_t;
		static wxCmdEvtHandlerMap_t m_menu_evt_map[];
		typedef struct 
		{
			const long evtStartTag;
			const long evtEndTag;
			void (MadSearchReplaceDialog::*method)( wxCommandEvent &);
		} wxCmdEvtHandlerRangeMap_t;
		static wxCmdEvtHandlerRangeMap_t m_menu_evt_range_map[];

	//private:
	public:
		//(*Handlers(MadSearchReplaceDialog)
		void MadSearchReplaceDialogClose(wxCloseEvent& event);
		void MadSearchReplaceDialogKeyDown(wxKeyEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& WXUNUSED(event));
		void WxCheckBoxSearchInSelectionClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonFindNextClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonReplaceClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonReplaceAllClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonFindPrevClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonCountClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonCloseClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonReplaceExpandClick(wxCommandEvent& WXUNUSED(event));
		void WxCheckBoxRegexClick(wxCommandEvent& WXUNUSED(event));
		void WxRadioLosingFocusSelect(wxCommandEvent& WXUNUSED(event));
		void WxRadioAlwaysSelect(wxCommandEvent& WXUNUSED(event));
		void WxSliderTransDegreeScroll(wxCommandEvent& WXUNUSED(event));
		void WxButtonReplaceAllInAllClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonFindAllClick(wxCommandEvent& WXUNUSED(event));
		void WxButtonFindAllInAllClick(wxCommandEvent& WXUNUSED(event));
		void OnWxSliderInputSizerCmdScroll(wxCommandEvent& WXUNUSED(event));
		void WxCheckBoxBookmarkLineClick(wxCommandEvent& WXUNUSED(event));
		//*)

		void MadSearchReplaceDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& WXUNUSED(event));
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& WXUNUSED(event));
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		void PurgeRecentFindTexts();
		void PurgeRecentReplaceTexts();
		void ReplaceAll(MadEdit	* madedit, bool	needRec=true);
		void SearchAll(MadEdit * madedit, bool needRec=true);
		void ShowReplaceUI();
		void SetTransparency();
		void ShowFindUI();
};

extern MadSearchReplaceDialog *g_SearchReplaceDialog;

#endif
