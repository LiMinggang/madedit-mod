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

		void OnEditUndo( wxCommandEvent& WXUNUSED(event) );
		void OnEditRedo( wxCommandEvent& WXUNUSED(event) );
		void OnEditCut( wxCommandEvent& WXUNUSED(event) );
		void OnEditCopy( wxCommandEvent& WXUNUSED(event) );
		void OnEditPaste( wxCommandEvent& WXUNUSED(event) );
		void OnEditDelete( wxCommandEvent& WXUNUSED(event) );
		void OnEditCutLine( wxCommandEvent& WXUNUSED(event) );
		void OnEditDeleteLine( wxCommandEvent& WXUNUSED(event) );
		void OnEditSelectAll( wxCommandEvent& WXUNUSED(event) );
		void OnEditStartEndSelction( wxCommandEvent& WXUNUSED(event) );
		void OnEditInsertTabChar( wxCommandEvent& WXUNUSED(event) );
		void OnEditInsertDateTime( wxCommandEvent& WXUNUSED(event) );

		void OnEditSortAscending( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortDescending( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortAscendingCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortDescendingCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortByOptions( wxCommandEvent& WXUNUSED(event) );
		void OnEditSortOptions( wxCommandEvent& WXUNUSED(event) );

		void OnEditCopyAsHexString( wxCommandEvent& WXUNUSED(event) );
		void OnEditCopyAsHexStringWithSpace( wxCommandEvent& WXUNUSED(event) );
		void OnEditCopyRevertHex( wxCommandEvent& WXUNUSED(event) );
		void OnEditIncIndent( wxCommandEvent& WXUNUSED(event) );
		void OnEditDecIndent( wxCommandEvent& WXUNUSED(event) );
		void OnEditComment( wxCommandEvent& WXUNUSED(event) );
		void OnEditUncomment( wxCommandEvent& WXUNUSED(event) );
		void OnEditWordWrapToNewLine( wxCommandEvent& WXUNUSED(event) );
		void OnEditNewLineToWordWrap( wxCommandEvent& WXUNUSED(event) );
		void OnEditToUpperCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditToLowerCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditInvertCase( wxCommandEvent& WXUNUSED(event) );
		void OnEditCapitalize( wxCommandEvent& WXUNUSED(event) );
		void OnEditToHalfWidth( wxCommandEvent& WXUNUSED(event) );
		void OnEditToHalfWidthByOptions( wxCommandEvent& WXUNUSED(event) );
		void OnEditToFullWidth( wxCommandEvent& WXUNUSED(event) );
		void OnEditToFullWidthByOptions( wxCommandEvent& WXUNUSED(event) );
		void OnEditTabToSpace( wxCommandEvent& WXUNUSED(event) );
		void OnEditSpaceToTab( wxCommandEvent& WXUNUSED(event) );
		void OnEditTrimTrailingSpaces( wxCommandEvent& WXUNUSED(event) );
		void OnEditTrimLeadingSpaces( wxCommandEvent& WXUNUSED(event) );
		void OnEditDeleteEmptyLines( wxCommandEvent& WXUNUSED(event) );
		void OnEditDeleteEmptyLinesWithSpaces( wxCommandEvent& WXUNUSED(event) );
		void OnEditJoinLines( wxCommandEvent& WXUNUSED(event) );
		void OnEditInsertNumbers( wxCommandEvent& WXUNUSED(event) );
		void OnEditColumnAlignLeft( wxCommandEvent& WXUNUSED(event) );
		void OnEditColumnAlignRight( wxCommandEvent& WXUNUSED(event) );
		void OnEditSpellCheck( wxCommandEvent& WXUNUSED(event) );
		void OnToolsMadScriptList( wxCommandEvent& WXUNUSED(event) );

		typedef struct 
		{
			const long evtTag;
			void (MadMacroDlg::*method)( wxCommandEvent &);
		} wxCmdEvtHandlerMap_t;
		static wxCmdEvtHandlerMap_t m_menu_evt_map[];
		
		typedef struct 
		{
			const long evtStartTag;
			const long evtEndTag;
			void (MadMacroDlg::*method)( wxCommandEvent &);
		} wxCmdEvtHandlerRangeMap_t;
		static wxCmdEvtHandlerRangeMap_t m_menu_evt_range_map[];
};
extern MadMacroDlg *g_MadMacroDlg;
#endif //__MADMACRODLG_H__
