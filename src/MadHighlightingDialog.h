///////////////////////////////////////////////////////////////////////////////
// Name:        MadHighlightingDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADHIGHLIGHTINGDIALOG_H_
#define _MADHIGHLIGHTINGDIALOG_H_


#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable : 4996)
#endif
// disable 4996 {
//(*Headers(MadHighlightingDialog)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)
#include <vector>
using std::vector;
// disable 4996 }
#ifdef _MSC_VER
# pragma warning(pop)
#endif

#include <wx/colordlg.h>

class MadSyntax;

class MadHighlightingDialog : public wxDialog
{
	public:

		MadHighlightingDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MadHighlightingDialog();

		//(*Declarations(MadHighlightingDialog)
		wxListCtrl* WxListCtrlKeyword;
		wxListBox* WxListBoxSyntax;
		wxComboBox* WxComboBoxScheme;
		wxListCtrl* WxListCtrlFC;
		wxStaticText* StaticText2;
		wxListCtrl* WxListCtrlBC;
		wxPanel* WxPanelFC;
		wxStaticText* WxStaticTextBCName;
		wxButton* WxButtonSave;
		wxPanel* WxPanelBC;
		wxStaticText* StaticText1;
		wxStaticText* WxStaticText4;
		wxButton* WxButtonClose;
		wxStaticLine* StaticLine2;
		wxStaticText* WxStaticText3;
		wxStaticText* WxStaticTextFCName;
		wxCheckBox* WxCheckBoxBold;
		wxCheckBox* WxCheckBoxUnderline;
		wxButton* WxButtonCancel;
		wxButton* WxButtonLoad;
		wxButton* WxButtonBC;
		wxStaticLine* StaticLine3;
		wxStaticLine* StaticLine1;
		wxButton* WxButtonDelete;
		wxButton* WxButtonFC;
		wxCheckBox* WxCheckBoxItalic;
		//*)

	protected:

		//(*Identifiers(MadHighlightingDialog)
		//*)
	private:
		MadSyntax *m_Syntax;
		vector<MadSyntax*> m_ModifiedSyntax;

		void SetKeywordFont(bool set, int style);
	public:
		wxString m_InitSetting; // the init title of madsyntax
		MadSyntax *GetSyntax(const wxString &title);
		void SetToModifiedSyntax(MadSyntax *syn);
		void SetAttrFC(const wxColor& color, const wxString& colorname);
		void SetAttrBC(const wxColor& color, const wxString& colorname);
		void RepaintKeyword();
		void FreeSyntax(bool restore);
		void SetPanelFC(const wxColor& color);
		void SetPanelBC(const wxColor& color);
	private:

		//(*Handlers(MadHighlightingDialog)
		void MadHighlightingDialogClose(wxCloseEvent& event);
		void WxListBoxSyntaxSelected(wxCommandEvent& event);
		void WxButtonLoadClick(wxCommandEvent& event);
		void WxButtonSaveClick(wxCommandEvent& event);
		void WxButtonDeleteClick(wxCommandEvent& event);
		void WxCheckBoxBoldClick(wxCommandEvent& event);
		void WxCheckBoxItalicClick(wxCommandEvent& event);
		void WxCheckBoxUnderlineClick(wxCommandEvent& event);
		void WxListCtrlKeywordSelected(wxListEvent& event);
		void WxListCtrlFCSelected(wxListEvent& event);
		void WxListCtrlBCSelected(wxListEvent& event);
		void WxButtonFCClick(wxCommandEvent& event);
		void WxButtonBCClick(wxCommandEvent& event);
		//*)

		void MadHighlightingDialogActivate(wxActivateEvent& event);
};

extern MadHighlightingDialog *g_HighlightingDialog;

#endif
