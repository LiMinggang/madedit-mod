///////////////////////////////////////////////////////////////////////////////
// Name:		MadMacroDlg.h
// Description:
// Author:		minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef	__MADMACRODLG_H__
#define	__MADMACRODLG_H__
#include "MadUtils.h"

//(*Headers(MadMacroDlg)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
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
		void SetEncoding(const wxString &encname);

		//(*Declarations(MadMacroDlg)
		wxButton* WxButtonResult;
		wxTextCtrl* WxMemoOutput;
		MadEdit*  m_Pymacro;
		//*)

		static wxString m_PyacroContext;
	protected:

		//(*Identifiers(MadMacroDlg)
		//*)
		bool m_debug;

	private:

		//(*Handlers(MadMacroDlg)
		void OnWxButtonRunClick(wxCommandEvent& event);
		void OnWxButtonCloseClick(wxCommandEvent& event);
		void OnWxButtonResultClick(wxCommandEvent& event);
		void MadMacroDlgClose(wxCloseEvent& event);
		void OnButtonResetClick(wxCommandEvent& event);
		//*)
};
extern MadMacroDlg *g_MadMacroDlg;
#endif //__MADMACRODLG_H__
