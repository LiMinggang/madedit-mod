#ifndef MADFILEHISTORYDIALOG_H
#define MADFILEHISTORYDIALOG_H

//(*Headers(MadFileHistoryDialog)
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#include "checkedlistctrl.h"

#include <set>

class MadEditFrame;
class MadRecentList;
class MadFileHistoryDialog: public wxDialog
{
	public:

		MadFileHistoryDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadFileHistoryDialog();

		//(*Declarations(MadFileHistoryDialog)
		wxButton* ButtonSelectAll;
		wxButton* ButtonDselectAll;
		wxCheckedListCtrl* MadFileList;
		wxButton* ButtonOK;
		wxButton* ButtonCancel;
		//*)
        void GetCheckedItemsData(wxArrayString & selectedItems, bool checked = true);
        void ResetWindowListIterms();

	protected:

		//(*Identifiers(MadFileHistoryDialog)
		static const long ID_CHECKLISTMADHISFILELIST;
		static const long ID_BUTTONSELECTALL;
		static const long ID_BUTTONDSELECTALL;
		//*)

		MadEditFrame * m_MainFrame;
		MadRecentList *m_RecentFiles;

	private:

		//(*Handlers(MadFileHistoryDialog)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnButtonSelectAllClick(wxCommandEvent& event);
		void OnButtonDselectAllClick(wxCommandEvent& event);
		void MadFileHistoryDialogClose(wxCloseEvent& event);
		//*)
		void MadFileHistoryDialogActivate( wxActivateEvent& event );

		DECLARE_EVENT_TABLE()
};

extern MadFileHistoryDialog *g_RecentOpenedFileListDialog;

#endif
