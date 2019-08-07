#ifndef MADWINLISTDIALOG_H
#define MADWINLISTDIALOG_H
#include "MadUtils.h"

#include <set>
#include <map>
//(*Headers(MadWinListDialog)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class MadEditFrame;

class MadWinListDialog: public wxDialog
{
	public:

		MadWinListDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~MadWinListDialog();

		//(*Declarations(MadWinListDialog)
		wxButton* ButtonCloseWindows;
		wxButton* ButtonSortTabByPath;
		wxButton* ButtonActivate;
		wxButton* ButtonSortTabByName;
		wxButton* ButtonSave;
		wxButton* ButtonSaveAs;
		wxListCtrl* MadWindowsList;
		wxButton* ButtonOk;
		//*)

	protected:

		//(*Identifiers(MadWinListDialog)
		//*)

		static const long COL_TABNAME;
		static const long COL_PATH;
		MadEditFrame * m_MainFrame;
		int m_SortColumn;
	private:

		//(*Handlers(MadWinListDialog)
		void OnButtonActivateClick(wxCommandEvent& WXUNUSED(event));
		void OnButtonSaveClick(wxCommandEvent& WXUNUSED(event));
		void OnButtonCloseWindowsClick(wxCommandEvent& WXUNUSED(event));
		void OnButtonSortTabByNameClick(wxCommandEvent& WXUNUSED(event));
		void OnButtonOkClick(wxCommandEvent& WXUNUSED(event));
		void OnMadWinListDialogClose(wxCloseEvent& WXUNUSED(event));
		void OnButtonSortTabByPathClick(wxCommandEvent& WXUNUSED(event));
		void OnButtonSaveAsClick(wxCommandEvent& WXUNUSED(event));
		void OnKeyDown(wxKeyEvent& event);
		//*)
		void MadWinListDialogActivate( wxActivateEvent& WXUNUSED(event) );
		void OnWinListSelectionChanged(wxListEvent& event);
		void OnWinListColumnTabClicked(wxListEvent& event);
		void InitWindowListIterms();
		void SortTabs(long column);
		void SaveFile(bool saveas = false);
        void ResetButtonStatus();
};

extern MadWinListDialog *g_WinListDialog;
#endif
