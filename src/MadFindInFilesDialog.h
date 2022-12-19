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
# pragma warning(push)
# pragma warning(disable : 4996)
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
# pragma warning(pop)
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
		wxCheckBox* WxCheckBoxPanChinese;
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

		void OnEditUndo(wxCommandEvent& WXUNUSED(event));
		void OnEditRedo(wxCommandEvent& WXUNUSED(event));
		void OnEditCut(wxCommandEvent& WXUNUSED(event));
		void OnEditCopy(wxCommandEvent& WXUNUSED(event));
		void OnEditPaste(wxCommandEvent& WXUNUSED(event));
		void OnEditDelete(wxCommandEvent& WXUNUSED(event));
		void OnEditCutLine(wxCommandEvent& WXUNUSED(event));
		void OnEditDeleteLine(wxCommandEvent& WXUNUSED(event));
		void OnEditSelectAll(wxCommandEvent& WXUNUSED(event));
		void OnEditStartEndSelction(wxCommandEvent& WXUNUSED(event));
		void OnEditInsertTabChar(wxCommandEvent& WXUNUSED(event));
		void OnEditInsertDateTime(wxCommandEvent& WXUNUSED(event));
		
		void OnEditSortAscending(wxCommandEvent& WXUNUSED(event));
		void OnEditSortDescending(wxCommandEvent& WXUNUSED(event));
		void OnEditSortAscendingCase(wxCommandEvent& WXUNUSED(event));
		void OnEditSortDescendingCase(wxCommandEvent& WXUNUSED(event));
		void OnEditSortByOptions(wxCommandEvent& WXUNUSED(event));
		void OnEditSortOptions(wxCommandEvent& WXUNUSED(event));
		
		void OnEditCopyAsHexString(wxCommandEvent& WXUNUSED(event));
		void OnEditCopyAsHexStringWithSpace(wxCommandEvent& WXUNUSED(event));
		void OnEditCopyRevertHex(wxCommandEvent& WXUNUSED(event));
		void OnEditIncIndent(wxCommandEvent& WXUNUSED(event));
		void OnEditDecIndent(wxCommandEvent& WXUNUSED(event));
		void OnEditComment(wxCommandEvent& WXUNUSED(event));
		void OnEditUncomment(wxCommandEvent& WXUNUSED(event));
		void OnEditWordWrapToNewLine(wxCommandEvent& WXUNUSED(event));
		void OnEditNewLineToWordWrap(wxCommandEvent& WXUNUSED(event));
		void OnEditToUpperCase(wxCommandEvent& WXUNUSED(event));
		void OnEditToLowerCase(wxCommandEvent& WXUNUSED(event));
		void OnEditInvertCase(wxCommandEvent& WXUNUSED(event));
		void OnEditCapitalize(wxCommandEvent& WXUNUSED(event));
		void OnEditToHalfWidth(wxCommandEvent& WXUNUSED(event));
		void OnEditToHalfWidthByOptions(wxCommandEvent& WXUNUSED(event));
		void OnEditToFullWidth(wxCommandEvent& WXUNUSED(event));
		void OnEditToFullWidthByOptions(wxCommandEvent& WXUNUSED(event));
		void OnEditTabToSpace(wxCommandEvent& WXUNUSED(event));
		void OnEditSpaceToTab(wxCommandEvent& WXUNUSED(event));
		void OnEditTrimTrailingSpaces(wxCommandEvent& WXUNUSED(event));
		void OnEditTrimLeadingSpaces(wxCommandEvent& WXUNUSED(event));
		void OnEditDeleteEmptyLines(wxCommandEvent& WXUNUSED(event));
		void OnEditDeleteEmptyLinesWithSpaces(wxCommandEvent& WXUNUSED(event));
		void OnEditJoinLines(wxCommandEvent& WXUNUSED(event));
		void OnEditInsertNumbers(wxCommandEvent& WXUNUSED(event));
		void OnEditColumnAlignLeft(wxCommandEvent& WXUNUSED(event));
		void OnEditColumnAlignRight(wxCommandEvent& WXUNUSED(event));
		void OnEditSpellCheck(wxCommandEvent& event);
		void OnToolsMadScriptList(wxCommandEvent& event);
		typedef struct 
		{
			const long evtTag;
			void (MadFindInFilesDialog::*method)(wxCommandEvent &);
		} wxCmdEvtHandlerMap_t;
		static wxCmdEvtHandlerMap_t m_menu_evt_map[];
		typedef struct 
		{
			const long evtStartTag;
			const long evtEndTag;
			void (MadFindInFilesDialog::*method)(wxCommandEvent &);
		} wxCmdEvtHandlerRangeMap_t;
		static wxCmdEvtHandlerRangeMap_t m_menu_evt_range_map[];		
};

extern MadFindInFilesDialog *g_FindInFilesDialog;

#endif
