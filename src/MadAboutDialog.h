///////////////////////////////////////////////////////////////////////////////
// Name:        MadAboutDialog.h
// Description:
// Author:      madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADABOUTDIALOG_H_
#define _MADABOUTDIALOG_H_

//(*Headers(MadAboutDialog)
#include <wx/aui/aui.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
//*)

class MadAboutDialog: public wxDialog
{
	public:

		MadAboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadAboutDialog();

		//(*Declarations(MadAboutDialog)
		wxAuiNotebook* WxAuiNotebookAbout;
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		wxListCtrl* WxListCredits;
		wxPanel* WxPanelAbout;
		wxPanel* WxPanelCredits;
		wxPanel* WxPanelLicense;
		wxStaticBitmap* WxStaticBitmap1;
		wxStaticBitmap* WxStaticBitmap2;
		wxTextCtrl* WxMemoAbout;
		wxTextCtrl* WxMemoLicense;
		//*)

	protected:

		//(*Identifiers(MadAboutDialog)
		static const long ID_STATICBITMAP1;
		static const long ID_STATICBITMAP2;
		static const long ID_WXMEMOABOUT;
		static const long ID_WXPANELABOUT;
		static const long ID_LISTCREDITS;
		static const long ID_WXPANELCREDITS;
		static const long ID_TEXTCTRL1;
		static const long ID_WXMEMOLICENSE;
		static const long ID_WXAUINOTEBOOKABOUT;
		//*)

	private:

		//(*Handlers(MadAboutDialog)
		void MadAboutDialogClose(wxCloseEvent& event);
		//*)

};

#endif
