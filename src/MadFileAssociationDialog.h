///////////////////////////////////////////////////////////////////////////////
// Name:        MadFileAssociationDialog.h
// Description: File Type Association Dialog for MSWindows
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADFILEASSOCIATIONDIALOG_H_
#define _MADFILEASSOCIATIONDIALOG_H_

#include <wx/platform.h>
#ifdef __WXMSW__

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable : 4996)
#endif
// disable 4996 {
//(*Headers(MadFileAssociationDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning(pop)
#endif

class MadFileAssociationDialog: public wxDialog
{
	public:

		MadFileAssociationDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadFileAssociationDialog();

		//(*Declarations(MadFileAssociationDialog)
		wxButton* WxButtonAdd;
		wxButton* WxButtonAddCustom;
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		wxButton* WxButtonRemove;
		wxListBox* WxListBoxAssociated;
		wxListBox* WxListBoxPredefined;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* WxEditCustomType;
		//*)

	protected:

		//(*Identifiers(MadFileAssociationDialog)
		//*)

	private:

		//(*Handlers(MadFileAssociationDialog)
		void MadFileAssociationDialogClose(wxCloseEvent& event);
		void WxButtonAddClick(wxCommandEvent& event);
		void WxButtonRemoveClick(wxCommandEvent& event);
		void WxButtonAddCustomClick(wxCommandEvent& event);
		void WxButtonOKClick(wxCommandEvent& event);
		void WxListBoxAssociatedDoubleClicked(wxCommandEvent& event);
		void WxListBoxPredefinedDoubleClicked(wxCommandEvent& event);
		//*)
};

#endif //__WXMSW__

#endif
