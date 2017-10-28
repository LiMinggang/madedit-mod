///////////////////////////////////////////////////////////////////////////////
// Name:		MadWordCountDialog.h
// Description:	WordCount Dialog
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	_MADWORDCOUNTDIALOG_H_
#define	_MADWORDCOUNTDIALOG_H_

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMWordCountDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class MadWordCountDialog: public wxDialog
{
	public:

		MadWordCountDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadWordCountDialog();

		//(*Declarations(MadWordCountDialog)
		wxStaticText* WxStaticText2;
		wxStaticText* WxStaticText1;
		wxButton* Button1;
		wxStaticText* WxStaticText4;
		wxStaticText* WxStaticTextWordCount;
		wxStaticText* WxStaticTextLineCount;
		wxStaticText* WxStaticText3;
		wxStaticText* WxStaticTextCharCount;
		wxStaticText* WxStaticTextFullwidthCount;
		wxStaticText* StaticText7;
		wxStaticLine* StaticLine1;
		wxTextCtrl* WxMemo1;
		wxStaticText* WxStaticTextSpaceCount;
		wxStaticText* WxStaticTextHalfwidthCount;
		wxStaticText* WxStaticText5;
		wxStaticText* WxStaticText7;
		//*)

	protected:

		//(*Identifiers(MadWordCountDialog)
		//*)

	private:

		//(*Handlers(MadWordCountDialog)
		void MadWordCountDialogClose(wxCloseEvent& event);
		//*)
};

#endif
