///////////////////////////////////////////////////////////////////////////////
// Name:        MadConvEncDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADCONVENCDIALOG_H_
#define _MADCONVENCDIALOG_H_


#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable : 4996)
#endif
// disable 4996 {
//(*Headers(WXMConvEncDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning(pop)
#endif


class MadConvEncDialog : public wxDialog
{
	public:

		MadConvEncDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadConvEncDialog();

		//(*Declarations(MadConvEncDialog)
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		wxComboBox* WxComboBoxEncoding;
		wxRadioBox* WxRadioBoxOption;
		wxStaticText* StaticText1;
		//*)

		wxString GetEncoding() const;
	protected:

		//(*Identifiers(MadConvEncDialog)
		//*)

	private:

		//(*Handlers(MadConvEncDialog)
		void MadConvEncDialogClose(wxCloseEvent& event);
		//*)
};

extern MadConvEncDialog *g_ConvEncDialog;

#endif
