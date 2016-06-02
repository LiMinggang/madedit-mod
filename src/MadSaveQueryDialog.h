#ifndef MADSAVEQUERYDIALOG_H
#define MADSAVEQUERYDIALOG_H

//(*Headers(MadSaveQueryDialog)
#include <wx/sizer.h>
#include "checkedlistctrl.h"
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class MadEditFrame;

class MadSaveQueryDialog: public wxDialog
{
	public:

		MadSaveQueryDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadSaveQueryDialog();

		//(*Declarations(MadSaveQueryDialog)
		wxButton* ButtonSelectAll;
		wxButton* ButtonDselectAll;
		wxCheckedListCtrl* MadFileList;
		wxButton* ButtonOK;
		wxButton* ButtonGoTo;
		wxButton* ButtonCancel;
		wxButton* ButtonSaveNone;
		//*)

	protected:

		//(*Identifiers(MadSaveQueryDialog)
		static const long ID_CHECKLISTBOXMADFILELIST;
		static const long ID_BUTTONSAVENONE;
		static const long ID_BUTTONSELECTALL;
		static const long ID_BUTTONDSELECTALL;
		static const long ID_BUTTONGOTO;
		//*)
		MadEditFrame * m_MainFrame;

	private:

		//(*Handlers(MadSaveQueryDialog)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonSaveNoneClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnButtonSelectAllClick(wxCommandEvent& event);
		void OnButtonDselectAllClick(wxCommandEvent& event);
		void OnButtonGoToClick(wxCommandEvent& event);
		//*)

        void InitWindowListIterms();
        void MadSaveQueryDialogActivate( wxActivateEvent& event );
		DECLARE_EVENT_TABLE()
};

extern MadSaveQueryDialog *g_ModifiedFileListDialog;

#endif
