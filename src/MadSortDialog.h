#ifndef MADSORTDIALOG_H
#define MADSORTDIALOG_H

//(*Headers(MadSortDialog)
#include <wx/sizer.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class MadSortDialog: public wxDialog
{
	public:

		explicit MadSortDialog(wxWindow* parent);
		virtual ~MadSortDialog();

		//(*Declarations(MadSortDialog)
		wxCheckBox* WxCheckBoxNumeric;
		wxButton* WxButtonSort;
		wxCheckBox* WxCheckBoxCase;
		wxCheckBox* WxCheckBoxRemoveDup;
		wxButton* WxButtonCancel;
		wxRadioBox* WxRadioBoxOrder;
		//*)

	protected:

		//(*Identifiers(MadSortDialog)
		static const long ID_WXRADIOBOXORDER;
		static const long ID_WXCHECKBOXCASE;
		static const long ID_WXCHECKBOXNUMERIC;
		static const long ID_WXCHECKBOXREMOVEDUP;
		//*)

	private:

		//(*Handlers(MadSortDialog)
		void MadSortDialogClose(wxCloseEvent& event);
		//*)
};

#endif
