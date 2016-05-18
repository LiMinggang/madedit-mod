#ifndef MADWINLISTDIALOG_H
#define MADWINLISTDIALOG_H

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
		wxButton* ButtonActivate;
		wxButton* ButtonSortTab;
		wxButton* ButtonSave;
		wxListCtrl* MadWindowsList;
		wxButton* ButtonOk;
		//*)

	protected:

		//(*Identifiers(MadWinListDialog)
		static const long ID_LISTCTRLMADWINLIST;
		static const long ID_BUTTONACTIVATE;
		static const long ID_BUTTONSAVE;
		static const long ID_BUTTONCLOSEWINDOWS;
		static const long ID_BUTTONSORTTAB;
		//*)

		MadEditFrame * m_MainFrame;
	private:

		//(*Handlers(MadWinListDialog)
		void OnButtonActivateClick(wxCommandEvent& event);
		void OnButtonSaveClick(wxCommandEvent& event);
		void OnButtonCloseWindowsClick(wxCommandEvent& event);
		void OnButtonSortTabClick(wxCommandEvent& event);
		void OnButtonOkClick(wxCommandEvent& event);
		void OnMadWinListDialogClose(wxCloseEvent& event);
		//*)
		void MadWinListDialogActivate( wxActivateEvent& event );
		void OnWinListSelected(wxListEvent& event);
		void OnWinListDeselected(wxListEvent& event);
		DECLARE_EVENT_TABLE()
};

extern MadWinListDialog *g_WinListDialog;
#endif
