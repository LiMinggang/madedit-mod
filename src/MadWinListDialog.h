#ifndef MADWINLISTDIALOG_H
#define MADWINLISTDIALOG_H

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
		static const long ID_LISTCTRLMADWINLIST;
		static const long ID_BUTTONACTIVATE;
		static const long ID_BUTTONSAVE;
		static const long ID_BUTTONSAVEAS;
		static const long ID_BUTTONCLOSEWINDOWS;
		static const long ID_BUTTONSORTTABBYNAME;
		static const long ID_BUTTONSORTTABBYPATH;
		//*)

		static const long COL_TABNAME;
		static const long COL_PATH;
		MadEditFrame * m_MainFrame;
		int m_SortColumn;
	private:

		//(*Handlers(MadWinListDialog)
		void OnButtonActivateClick(wxCommandEvent& event);
		void OnButtonSaveClick(wxCommandEvent& event);
		void OnButtonCloseWindowsClick(wxCommandEvent& event);
		void OnButtonSortTabByNameClick(wxCommandEvent& event);
		void OnButtonOkClick(wxCommandEvent& event);
		void OnMadWinListDialogClose(wxCloseEvent& event);
		void OnButtonSortTabByPathClick(wxCommandEvent& event);
		void OnButtonSaveAsClick(wxCommandEvent& event);
		//*)
		void MadWinListDialogActivate( wxActivateEvent& event );
		void OnWinListSelectionChanged(wxListEvent& event);
		void InitWindowListIterms();
		void SortTabs(long column);
		void SaveFile(bool saveas = false);
		DECLARE_EVENT_TABLE()
};

extern MadWinListDialog *g_WinListDialog;
#endif
