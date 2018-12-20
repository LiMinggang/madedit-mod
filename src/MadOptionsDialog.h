///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.h
// Description:
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADOPTIONSDIALOG_H_
#define _MADOPTIONSDIALOG_H_
#include "MadUtils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(MadOptionsDialog)
#include <wx/aui/aui.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <list>
#include <map>
using std::list;

#include "MadCommand.h"

class TreeItemData: public wxTreeItemData
{
public:
	CommandData *cmddata;
	wxArrayString keys;
};

class KeyTextCtrl;
class wxAuiNotebook;
#ifdef MADEDIT_ENABLE_MADEDIT
class MadEdit;
#endif

class MadOptionsDialog: public wxDialog
{
	public:

		MadOptionsDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~MadOptionsDialog();

		//(*Declarations(MadOptionsDialog)
		KeyTextCtrl* EditKey;
		wxAuiNotebook* AuiNotebook1;
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button3;
		wxButton* Button4;
		wxButton* Button5;
		wxButton* Button6;
		wxButton* ButtonAddKey;
		wxButton* ButtonCancel;
		wxButton* ButtonDateTime;
		wxButton* ButtonDeleteKey;
		wxButton* ButtonDictionaryDir;
		wxButton* ButtonOK;
		wxButton* ButtonShowInMenu;
		wxCheckBox* CheckBoxAddKeywords;
		wxCheckBox* CheckBoxAutoCompletePair;
		wxCheckBox* CheckBoxAutoFillColumnPaste;
		wxCheckBox* CheckBoxAutoIndent;
		wxCheckBox* CheckBoxCtrlWithMouseToSelect;
		wxCheckBox* CheckBoxDateTimeInEnglish;
		wxCheckBox* CheckBoxDoNotSaveSettings;
		wxCheckBox* CheckBoxEnableAutoBackup;
		wxCheckBox* CheckBoxEnableAutoSave;
		wxCheckBox* CheckBoxFixWidthMode;
		wxCheckBox* CheckBoxInsertPairForSelection;
		wxCheckBox* CheckBoxLDClickHighlight;
		wxCheckBox* CheckBoxMiddleMouseToPaste;
		wxCheckBox* CheckBoxMouseSelectToCopy;
		wxCheckBox* CheckBoxNewDocEncUTF8WithBOM;
		wxCheckBox* CheckBoxPersonalDict;
		wxCheckBox* CheckBoxPrintBookmark;
		wxCheckBox* CheckBoxPrintEndOfLine;
		wxCheckBox* CheckBoxPrintLineNumber;
		wxCheckBox* CheckBoxPrintPageFooter;
		wxCheckBox* CheckBoxPrintPageHeader;
		wxCheckBox* CheckBoxPrintSpaceChar;
		wxCheckBox* CheckBoxPrintSyntax;
		wxCheckBox* CheckBoxPrintTabChar;
		wxCheckBox* CheckBoxPurgeHistory;
		wxCheckBox* CheckBoxRecordCaretMovements;
		wxCheckBox* CheckBoxReloadFiles;
		wxCheckBox* CheckBoxResetAllKeys;
		wxCheckBox* CheckBoxRestoreCaretPos;
#ifdef __WXMSW__
		wxCheckBox* CheckBoxRightClickMenu;
#endif
		wxCheckBox* CheckBoxSingleInstance;
		wxCheckBox* CheckBoxSkipAutoEncoding;
		wxCheckBox* CheckBoxTabOrSpaces;
		wxCheckBox* CheckBoxTypewriterMode;
		wxChoice* ChoiceDictionary;
		wxComboBox* ComboBoxDefaultFont;
		wxComboBox* ComboBoxEncoding;
		wxComboBox* ComboBoxLanguage;
		wxComboBox* ComboBoxNewDocEncOther;
		wxComboBox* ComboBoxNewDocFont;
		wxComboBox* ComboBoxNewDocSyntax;
		wxListBox* ListBoxKeys;
		wxMenu PopupMenuDateTimeMark;
		wxMenu PopupMenuPrintMark;
		wxPanel* Panel4;
		wxRadioBox* RadioBoxPrintOffset;
		wxRadioButton* RadioButtonNewDocEncOther;
		wxRadioButton* RadioButtonNewDocEncSystemDefault;
		wxRadioButton* RadioButtonNewDocEncUTF8;
		wxRadioButton* RadioButtonNewDocLineEndingCR;
		wxRadioButton* RadioButtonNewDocLineEndingCRLF;
		wxRadioButton* RadioButtonNewDocLineEndingDefault;
		wxRadioButton* RadioButtonNewDocLineEndingLF;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* EditAutoSaveTimeout;
		wxTextCtrl* EditCommandHint;
		wxTextCtrl* EditDateTime;
		wxTextCtrl* EditDefaultFontSize;
		wxTextCtrl* EditDictionaryDir;
		wxTextCtrl* EditFooterCenter;
		wxTextCtrl* EditFooterLeft;
		wxTextCtrl* EditFooterRight;
		wxTextCtrl* EditHeaderCenter;
		wxTextCtrl* EditHeaderLeft;
		wxTextCtrl* EditHeaderRight;
		wxTextCtrl* EditIndentColumns;
		wxTextCtrl* EditKeyHint;
		wxTextCtrl* EditMaxColumns;
		wxTextCtrl* EditMaxDisplaySize;
		wxTextCtrl* EditMaxLineLength;
		wxTextCtrl* EditMaxSizeToLoad;
		wxTextCtrl* EditMaxTextFileSize;
		wxTextCtrl* EditNewDocFontSize;
		wxTextCtrl* EditTabColumns;
		wxTreeCtrl* TreeCtrl1;
		//*)

	protected:
		friend class KeyTextCtrl;
		//(*Identifiers(MadOptionsDialog)
		//*)
	private:

		//(*Handlers(MadOptionsDialog)
		void MadOptionsDialogClose(wxCloseEvent& event);
		void ButtonAddKeyClick(wxCommandEvent& event);
		void ButtonCancelClick(wxCommandEvent& event);
		void ButtonDateTimeClick(wxCommandEvent& event);
		void ButtonDeleteKeyClick(wxCommandEvent& event);
		void ButtonOKClick(wxCommandEvent& event);
		void ButtonShowInMenuClick(wxCommandEvent& event);
		void ListBoxKeysSelected(wxCommandEvent& event);
		void TreeCtrl1SelChanged(wxTreeEvent& event);
		void CheckBoxMouseSelectToCopyClick(wxCommandEvent& event);
		void ButtonDictionaryDirClick(wxCommandEvent& event);
		void CheckBoxAutoCompletePairClick(wxCommandEvent& event);
		void EnableAutoSaveClick(wxCommandEvent& event);
		void ChoiceDictionarySelect(wxCommandEvent& event);
		void EditDictionaryDirTextEnter(wxCommandEvent& event);
		void RadioButtonNewDocEncSelect(wxCommandEvent& event);
		void RadioButtonNewDocLineEndingSelect(wxCommandEvent& event);
		//*)

		void MadOptionsDialogActivate(wxActivateEvent& event);


		void PrintMarkClick(wxCommandEvent& event);
		void PrintMarkButtonClick(wxCommandEvent& event);
		void DateTimeMarkClick(wxCommandEvent& event);
private:
	typedef struct 
	{
		const long evtTag;
		void (MadOptionsDialog::*method)( wxCommandEvent &);
	} wxCmdEvtHandlerMap_t;

public:
	void LoadOptions(void);

	list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
	list<TreeItemData*> ChangedTreeItemDataList;
	
	TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
	bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
	void UpdateKeyHint();
	void InitDictionaryChoice(const wxString &path = wxEmptyString);
	int m_NewDocLineEnding;
	wxString m_NewDocEncoding;

private:
	int ButtonID;
	int EditHeaderLeftId;
	int EditHeaderCenterId;
	int EditHeaderRightId;
	int EditFooterLeftId;
	int EditFooterCenterId;
	int EditFooterRightId;
};


extern MadOptionsDialog *g_OptionsDialog;

#endif
