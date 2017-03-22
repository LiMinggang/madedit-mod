///////////////////////////////////////////////////////////////////////////////
// Name:		MadMacroDlg.h
// Description:
// Author:		minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	__MADMACRODLG_H__
#define	__MADMACRODLG_H__

//(*Headers(MadMacroDlg)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
class EmbeddedPython;
class wxStreamToTextRedirector;
class MadEdit;

class MadMacroDlg: public wxDialog
{
	public:

		MadMacroDlg(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadMacroDlg();
		void SetPyScript(wxString &	pyscript);
		bool IsDebugOn() {return m_debug;}

		//(*Declarations(MadMacroDlg)
		wxTextCtrl* WxMemoOutput;
		wxButton* WxButtonRun;
		MadEdit*  m_Pymacro;
		wxButton* WxButtonClose;
		wxButton* WxButtonResult;
		//*)

	protected:

		//(*Identifiers(MadMacroDlg)
		static const long ID_MADEDIT;
		static const long ID_WXBUTTONTOGGLERESULT;
		static const long ID_TEXTCTRLRESULT;
		//*)
		bool m_debug;

	private:

		//(*Handlers(MadMacroDlg)
		void OnWxButtonRunClick(wxCommandEvent& event);
		void OnWxButtonCloseClick(wxCommandEvent& event);
		void OnWxButtonResultClick(wxCommandEvent& event);
		void MadMacroDlgClose(wxCloseEvent& event);
		//*)
};
extern MadMacroDlg *g_MadMacroDlg;
#endif //__MADMACRODLG_H__
