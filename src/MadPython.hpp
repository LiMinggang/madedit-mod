///////////////////////////////////////////////////////////////////////////////
// Name:        MadPython.hpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef __MADPYTHON__
#define __MADPYTHON__
#include "MadUtils.h"
#include <stdexcept>
#include <iostream>
#include <string>

//#include <Python.h>

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <boost/function.hpp>

#include <iostream>
#include <string>
#include <memory>

#include "MadEdit/MadEncoding.h"
#include "MadEdit/TradSimp.h"
#include "MadEdit/MadEdit.h"
#include "MadEdit/MadEditCommand.h"

#ifndef PYMADEDIT_DLL
	#include "MadEditFrame.h"
	extern wxStatusBar *g_StatusBar;
	extern MadEdit *g_ActiveMadEdit;
	extern MadEdit *g_CurrentMadEdit;
	extern wxTextCtrl *g_Output;
#else
	MadEdit *g_ActiveMadEdit = nullptr;
	MadEdit *g_CurrentMadEdit = nullptr;
	wxFrame g_DummyWin;
#endif
extern void DisplayFindAllResult(wxTreeItemId &myroot, vector<wxFileOffset> &begpos, vector<wxFileOffset> &endpos, MadEdit *madedit, bool expandresults = true, OnProgressUpdatePtr updater = nullptr);
extern int MadMessageBox(const wxString& message,
						  const wxString& caption = wxMessageBoxCaptionStr,
						  long style = wxOK | wxCENTRE,
						  wxWindow *parent = nullptr,
						  int x = wxDefaultCoord, int y = wxDefaultCoord);

void PrintString(const std::string& str)
{
	if (g_Output)
	{
		wxString wxStr(str.c_str(), wxConvUTF8);
		g_Output->AppendText(wxStr);
	}
	else
	{ std::cout << str; }
}

// Ugly bigger switch than bigger map
bool FromCmdToString(wxString &cmdStr, long madCmd) {
	switch (madCmd) {
		case ecCharFirst:
			cmdStr << (wxT("CharFirst()"));
			break;

		case ecCharLast:
			cmdStr << (wxT("CharLast()"));
			break;

		case ecCaretCommandFirst:
			cmdStr << (wxT("CaretCommandFirst()"));
			break;

		case ecCaretCommandLast:
			cmdStr << (wxT("CaretCommandLast()"));
			break;

		case ecLeft:
			cmdStr << (wxT("Left()"));
			break;

		case ecRight:
			cmdStr << (wxT("Right()"));
			break;

		case ecUp:
			cmdStr << (wxT("Up()"));
			break;

		case ecDown:
			cmdStr << (wxT("Down()"));
			break;

		case ecBeginLine:
			cmdStr << (wxT("BeginLine()"));
			break;

		case ecEndLine:
			cmdStr << (wxT("EndLine()"));
			break;

		case ecBeginDoc:
			cmdStr << (wxT("BeginDoc()"));
			break;

		case ecEndDoc:
			cmdStr << (wxT("EndDoc()"));
			break;

		case ecPrevPage:
			cmdStr << (wxT("PrevPage()"));
			break;

		case ecNextPage:
			cmdStr << (wxT("NextPage()"));
			break;

		case ecPrevWord:
			cmdStr << (wxT("PrevWord()"));
			break;

		case ecNextWord:
			cmdStr << (wxT("NextWord()"));
			break;

		case ecLeftBrace:
			cmdStr << (wxT("LeftBrace()"));
			break;

		case ecRightBrace:
			cmdStr << (wxT("RightBrace()"));
			break;

		case ecSelCommandFirst:
			cmdStr << (wxT("SelCommandFirst()"));
			break;

		case ecSelLeft:
			cmdStr << (wxT("SelLeft()"));
			break;

		case ecSelRight:
			cmdStr << (wxT("SelRight()"));
			break;

		case ecSelUp:
			cmdStr << (wxT("SelUp()"));
			break;

		case ecSelDown:
			cmdStr << (wxT("SelDown()"));
			break;

		case ecSelBeginLine:
			cmdStr << (wxT("SelBeginLine()"));
			break;

		case ecSelEndLine:
			cmdStr << (wxT("SelEndLine()"));
			break;

		case ecSelBeginDoc:
			cmdStr << (wxT("SelBeginDoc()"));
			break;

		case ecSelEndDoc:
			cmdStr << (wxT("SelEndDoc()"));
			break;

		case ecSelPrevPage:
			cmdStr << (wxT("SelPrevPage()"));
			break;

		case ecSelNextPage:
			cmdStr << (wxT("SelNextPage()"));
			break;

		case ecSelPrevWord:
			cmdStr << (wxT("SelPrevWord()"));
			break;

		case ecSelNextWord:
			cmdStr << (wxT("SelNextWord()"));
			break;

		case ecSelLeftBrace:
			cmdStr << (wxT("SelLeftBrace()"));
			break;

		case ecSelRightBrace:
			cmdStr << (wxT("SelRightBrace()"));
			break;

		case ecSelCommandLast:
			cmdStr << (wxT("SelCommandLast()"));
			break;

		case ecHighlightWords:
			cmdStr << (wxT("HighlightWords()"));

			break;
			
		case ecSelectAll:
			cmdStr << (wxT("SelectAll()"));
			break;
		
		case ecScrollLineUp:
			cmdStr << (wxT("ScrollLineUp()"));
			break;
		
		case ecScrollLineDown:
			cmdStr << (wxT("ScrollLineDown()"));
			break;
		
		case ecScrollPageUp:
			cmdStr << (wxT("ScrollPageUp()"));
			break;
		
		case ecScrollPageDown:
			cmdStr << (wxT("ScrollPageDown()"));
			break;
		
		case ecScrollLeft:
			cmdStr << (wxT("ScrollLeft()"));
			break;
		
		case ecScrollRight:
			cmdStr << (wxT("ScrollRight()"));
			break;
		
		case ecReturn:
			cmdStr << (wxT("Return()"));
			break;
		
		case ecReturnNoIndent:
			cmdStr << (wxT("ReturnNoIndent()"));
			break;
		
		case ecTab:
			cmdStr << (wxT("Tab()"));
			break;
		
		case ecInsertTabChar:
			cmdStr << (wxT("InsertTabChar()"));
			break;
		
		case ecDelete:
			cmdStr << (wxT("Delete()"));
			break;
		
		case ecBackSpace:
			cmdStr << (wxT("BackSpace()"));
			break;
		
		case ecDelPrevWord:
			cmdStr << (wxT("DelPrevWord()"));
			break;
		
		case ecDelNextWord:
			cmdStr << (wxT("DelNextWord()"));
			break;

		case ecCutLine:
			cmdStr<< (wxT("CutLine()"));
			break;

		case ecDeleteLine:
			cmdStr<< (wxT("DeleteLine()"));
			break;

		case ecUndo:
			cmdStr<< (wxT("Undo()"));
			break;

		case ecRedo:
			cmdStr<< (wxT("Redo()"));
			break;

		case ecCut:
			cmdStr<< (wxT("Cut()"));
			break;

		case ecCopy:
			cmdStr << (wxT("Copy()"));
			break;

		case ecPaste:
			cmdStr << (wxT("Paste()"));
			break;
		
		case ecToggleInsertMode:
			cmdStr << (wxT("ToggleInsertMode()"));
			break;
		
		case ecTextMode:
			cmdStr << (wxT("TextMode()"));
			break;
		
		case ecColumnMode:
			cmdStr << (wxT("ColumnMode()"));
			break;
		
		case ecHexMode:
			cmdStr << (wxT("HexMode()"));
			break;
		
		case ecNoWrap:
			cmdStr << (wxT("NoWrap()"));
			break;
		
		case ecWrapByWindow:
			cmdStr << (wxT("WrapByWindow()"));
			break;
		
		case ecWrapByColumn:
			cmdStr << (wxT("WrapByColumn()"));
			break;
		
		//case ecToggleWindow:
		//	cmdStr << (wxT("ProcessCommand(MadEditCommand.ToggleWindow)"));
		//	break;
		
		case ecIncreaseIndent:
			cmdStr << (wxT("IncreaseIndent()"));
			break;
		
		case ecDecreaseIndent:
			cmdStr << (wxT("DecreaseIndent()"));
			break;
		
		case ecComment:
			cmdStr << (wxT("Comment()"));
			break;
		
		case ecUncomment:
			cmdStr << (wxT("Uncomment()"));
			break;
		
		case ecToUpperCase:
			cmdStr << (wxT("ToUpperCase()"));
			break;
		
		case ecToLowerCase:
			cmdStr << (wxT("ToLowerCase()"));
			break;
		
		case ecInvertCase:
			cmdStr << (wxT("InvertCase()"));
			break;
		
		case ecToHalfWidth:
			cmdStr << (wxT("ToHalfWidth()"));
			break;
		
		case ecToFullWidth:
			cmdStr << (wxT("ToFullWidth()"));
			break;
		
		case ecInsertDateTime:
			cmdStr << (wxT("InsertDateTime()"));
			break;

		//case ecMouseNotify:
		//	cmdStr << (wxT("ProcessCommand(MadEditCommand.MouseNotify)"));
		//	break;

		default:
			return false;
			break;
	}

	return true;
}

enum MadMsgBoxStyle {
	MAD_YES 					 = wxYES,
	MAD_OK						 = wxOK,
	MAD_NO						 = wxNO,
	MAD_CANCEL					 = wxCANCEL,
	MAD_APPLY					 = wxAPPLY,
	MAD_CLOSE					 = wxCLOSE,
	MAD_NO_DEFAULT				 = wxNO_DEFAULT,
	MAD_CANCEL_DEFAULT			 = wxCANCEL_DEFAULT,
	MAD_ICON_WARNING_EXCLAMATION = wxICON_WARNING,
	MAD_ICON_ERROR_HAND_STOP	 = wxICON_ERROR,
	MAD_ICON_QUESTION			 = wxICON_QUESTION,
	MAD_ICON_INFORMATION		 = wxICON_INFORMATION,
	MAD_ICON_NONE				 = wxICON_NONE,
	MAD_CENTRE                   = wxCENTRE,
};

enum MadMsgBoxRet {
	MADRET_OK  = wxID_OK,
	MADRET_CANCEL = wxID_CANCEL,
	MADRET_APPLY = wxID_APPLY,
	MADRET_YES  = wxID_YES,
	MADRET_NO = wxID_NO,
};

namespace mad_py = ::boost::python;
namespace mad_python {
	class PyMadEdit {
	public:
		PyMadEdit(){
#ifndef PYMADEDIT_DLL
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!madedit) {
				// Simulate MadEditFrame::OnFileNew
				std::cout << "WARNING: No active MadEdit!!!" << std::endl;
			}
			else if (madedit->IsReadOnly()) {
				std::cout << "WARNING: Current file is ReadOnly!!!" << std::endl;
			}
			if (!g_MainFrame)
			{ 
				std::cout << "WARNING: No main frame!!!" << std::endl;
			}

#else

			if (!madedit) {
				madedit = new MadEdit(&g_DummyWin, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
				return;
			}

#endif
		}

		void ProcessCommand(long command) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				if (madedit->IsReadOnly()) {
					if ((command >= ecCharFirst && command <= ecCharLast)
							|| (command >= ecReturn && command <= ecCut)
							|| (command == ecPaste)
							|| (command >= ecIncreaseIndent && command <= ecInsertDateTime))
					{ return; }
				}

				madedit->ProcessCommand(command);
			}
		}

		void CharFirst() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecCharFirst); }
		}

		void CharLast() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecCharLast); }
		}

		void CaretCommandFirst() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecCaretCommandFirst); }
		}

		void Left() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecLeft); }
		}

		void Right() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecRight); }
		}

		void Up() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecUp); }
		}

		void Down() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecDown); }
		}

		void BeginLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecBeginLine); }
		}

		void EndLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecEndLine); }
		}

		void BeginDoc() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecBeginDoc); }
		}

		void EndDoc() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecEndDoc); }
		}

		void PrevPage() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecPrevPage); }
		}

		void NextPage() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecNextPage); }
		}

		void PrevWord() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecPrevWord); }
		}

		void NextWord() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecNextWord); }
		}

		void LeftBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecLeftBrace); }
		}

		void RightBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecRightBrace); }
		}

		void CaretCommandLast() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecCaretCommandLast); }
		}

		void SelCommandFirst() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelCommandFirst); }
		}

		void SelLeft() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelLeft); }
		}

		void SelRight() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelRight); }
		}

		void SelUp() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelUp); }
		}

		void SelDown() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelDown); }
		}

		void SelBeginLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelBeginLine); }
		}

		void SelEndLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelEndLine); }
		}

		void SelBeginDoc() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelBeginDoc); }
		}

		void SelEndDoc() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelEndDoc); }
		}

		void SelPrevPage() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelPrevPage); }
		}

		void SelNextPage() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelNextPage); }
		}

		void SelPrevWord() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelPrevWord); }
		}

		void SelNextWord() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelNextWord); }
		}

		void SelLeftBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelLeftBrace); }
		}

		void SelRightBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelRightBrace); }
		}

		void SelCommandLast() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecSelCommandLast); }
		}

		void Return() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecReturn); }
		}

		void ReturnNoIndent() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecReturnNoIndent); }
		}

		void Tab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecTab); }
		}

		void InsertTabChar() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecInsertTabChar); }
		}

		void Delete() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecDelete); }
		}

		void BackSpace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecBackSpace); }
		}

		void DelPrevWord() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecDelPrevWord); }
		}

		void DelNextWord() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecDelNextWord); }
		}

		void ToggleInsertMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecToggleInsertMode); }
		}

		void InsertWChar(long key) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ProcessCommand(key); }
		}

		void InsertStr(const std::string &str) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly())) {
				wxString wxStr(str.c_str(), wxConvUTF8);
				ucs4string out;
				vector<ucs4_t> ucs;
				madedit->TranslateText(wxStr.c_str(), wxStr.Len(), &ucs, true);

				for (size_t i = 0, size = ucs.size(); i < size; ++i) {
					out += ucs[i] ;
				}

				madedit->InsertString(out.c_str(), out.length(), false, true, false);
			}
		}

		void InsertNewline() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly())) {
				madedit->ProcessCommand(ecReturn);
			}
		}

		void InsertIncrementalNumber(long initial, long step, long total, long stepType,
									  long fmt, long align, bool zeroPad, const std::string & pref, const std::string & post) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly())) {
				wxString wxPrefix(pref.c_str(), wxConvUTF8), wxPostfix(post.c_str(), wxConvUTF8);
				madedit->InsertIncrementalNumber(initial, step, total,
						(MadNumberingStepType)stepType, (MadNumberFormat)fmt, (MadNumberAlign)align, zeroPad, wxPrefix, wxPostfix);
			}
		}

		void ScrollLineUp() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecScrollLineUp); }
		}

		void ScrollLineDown() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecScrollLineDown); }
		}

		void ScrollPageUp() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecScrollPageUp); }
		}

		void ScrollPageDown() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecScrollPageDown); }
		}

		void ScrollLeft() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecScrollLeft); }
		}

		void ScrollRight() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ProcessCommand(ecScrollRight); }
		}

		void GoToLine(long line) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GoToLine(line); }
		}

		void GoToLineColumn(long line, long column) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				if (column < 0) column = 0;
				if (line < 0) line = 0;
				madedit->GoToLine(line);

				for (long col = 0; col < (column - 1); ++col) // no validate for input
				{ madedit->ProcessCommand(ecRight); }
			}
		}

		void SetSyntax(const std::string &title) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (! title.empty())) {
				wxString wxTitle(title.c_str(), wxConvUTF8);
				madedit->SetSyntax(wxTitle);
			}
		}

		const std::string GetSyntaxTitle() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxString title = madedit->GetSyntaxTitle();
				return std::string(title.mb_str(wxConvUTF8));
			}
			else { return std::string("");}
		}

		void LoadDefaultSyntaxScheme() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->LoadDefaultSyntaxScheme(); }
		}

		void SetEncoding(const std::string &encname) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly())) {
				if (encname.empty())
				{ return; }

				wxString wxEncname(encname.c_str(), wxConvUTF8);
				madedit->SetEncoding(wxEncname);
			}
		}
		const std::string GetEncodingName() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit)) {
				wxString desc = madedit->GetEncodingName();
				return std::string(desc.mb_str(wxConvUTF8));
			}
			else {
				return std::string("");
			}
		}

		const std::string GetEncodingDescription() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxString desc = madedit->GetEncodingDescription();
				return std::string(desc.mb_str(wxConvUTF8));
			}
			else {
				return std::string("");
			}
		}

		long GetEncodingType() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return (long)madedit->GetEncodingType(); }
			else { return 0; }
		}

		bool GetRecordCaretMovements() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!(madedit)) { return false; }

			return madedit->GetRecordCaretMovements();
		}

		void RecordCaretMovements() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!(madedit)) { return; }

			madedit->SetRecordCaretMovements(true);
		}

		void NotRecordCaretMovements() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!(madedit)) { return; }

			madedit->SetRecordCaretMovements(false);
		}

		void SetTextFont(const std::string &name, long size, bool forceReset) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((! name.empty()) && (size > 0) && (madedit)) {
				wxString wxName(name.c_str(), wxConvUTF8);
				madedit->SetTextFont(wxName, size, forceReset);
			}
		}

		void SetHexFont(const std::string &name, long size, bool forceReset) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((! name.empty()) && (size > 0) && (madedit)) {
				wxString wxName(name.c_str(), wxConvUTF8);
				madedit->SetHexFont(wxName, size, forceReset);
			}
		}

		mad_py::tuple GetTextFont() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				int size = 0;
				wxString name;
				madedit->GetTextFont(name, size);

				if (name.size())
					return mad_py::make_tuple(std::string(name.mb_str(wxConvUTF8)), size);
			}

			return mad_py::make_tuple(std::string(""), 0);
		}

		mad_py::tuple GetHexFont() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				int size = 0;
				wxString name;
				madedit->GetHexFont(name, size);

				if (name.size())
					return mad_py::make_tuple(std::string(name.mb_str(wxConvUTF8)), size);
			}

			return mad_py::make_tuple(std::string(""), 0);
		}

		/*wxFont GetFont()
		{
		    return (madedit)->GetFont();
		}*/

		mad_py::tuple GetFontNameSize() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				int size = 0;
				wxString name;
				madedit->GetFont(name, size);

				if (name.size())
					return mad_py::make_tuple(std::string(name.mb_str(wxConvUTF8)), size);
			}

			return mad_py::make_tuple(std::string(""), 0);
		}

		void SetFontA(const std::string &name, long size) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!name.empty())) {
				wxString wxName(name.c_str(), wxConvUTF8);
				madedit->SetFont(wxName, size);
			}
		}

		/*bool SetFontB(const wxFont& font)
		{
		    return (madedit)->SetFont(font);
		}*/

		void FixedWidthMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetFixedWidthMode(true); }
		}

		void NoFixedWidthMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetFixedWidthMode(false); }
		}

		bool GetFixedWidthMode() {
			bool mode = false;
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;

			if (madedit)
			{ mode = madedit->GetFixedWidthMode(); }

			return mode;
		}

		void SetLineSpacing(long percent) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetLineSpacing(percent); }
		}

		long GetLineSpacing() {
			long spacing = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ spacing = madedit->GetLineSpacing(); }

			return spacing;
		}

		void TextMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			madedit->SetEditMode(emTextMode);
		}
		void ColumnMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			madedit->SetEditMode(emColumnMode);
		}
		void HexMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			madedit->SetEditMode(emHexMode);
		}

		long GetEditMode() {
			long mode = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ mode = (long)madedit->GetEditMode(); }

			return mode;
		}

		void SingleLineMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetSingleLineMode(true); }
		}

		void MultiLineMode() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetSingleLineMode(false); }
		}

		void SetTabColumns(long value) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetTabColumns(value); }
		}

		long GetTabColumns() {
			long cols = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ cols = madedit->GetTabColumns(); }

			return cols;
		}

		void SetIndentColumns(long value) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetIndentColumns(value); }
		}

		long GetIndentColumns() {
			long cols = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ cols = madedit->GetIndentColumns(); }

			return cols;
		}

		void InsertSpacesInsteadOfTab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetInsertSpacesInsteadOfTab(true); }
		}

		void InsertTab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetInsertSpacesInsteadOfTab(false); }
		}

		bool GetInsertSpacesInsteadOfTab() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetInsertSpacesInsteadOfTab(); }

			return res;
		}

		void WantTab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetWantTab(true); }
		}

		void NotWantTab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetWantTab(false); }
		}

		bool GetWantTab() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetWantTab(); }

			return res;
		}

		void NoWrap() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			madedit->SetWordWrapMode(wwmNoWrap);
		}

		void WrapByWindow() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			madedit->SetWordWrapMode(wwmWrapByWindow);
		}

		void WrapByColumn() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			madedit->SetWordWrapMode(wwmWrapByColumn);
		}

		long GetWordWrapMode() {
			long mode  = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ mode = (long)madedit->GetWordWrapMode(); }

			return mode;
		}

		void ShowEndOfLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetShowEndOfLine(true); }
		}

		void ShowTabChar() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetShowTabChar(true); }
		}

		void ShowSpaceChar() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetShowSpaceChar(true); }
		}

		void MarkActiveLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMarkActiveLine(true); }
		}

		void DisplayLineNumber() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetDisplayLineNumber(true); }
		}

		void DisplayColHint() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetDisplayColHint(true); }
		}

		void DisplayBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetDisplayBookmark(true); }
		}

		void HideEndOfLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetShowEndOfLine(false); }
		}

		void HideTabChar() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetShowTabChar(false); }
		}

		void HideSpaceChar() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetShowSpaceChar(false); }
		}

		void HideMarkActiveLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMarkActiveLine(false); }
		}

		void HideLineNumber() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetDisplayLineNumber(false); }
		}

		void HideBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetDisplayBookmark(false); }
		}

		void HideColHint() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetDisplayColHint(false); }
		}

		bool GetDisplayLineNumber() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetDisplayLineNumber(); }

			return res;
		}

		bool GetDisplayBookmark() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetDisplayBookmark(); }

			return res;
		}

		bool GetShowEndOfLine() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetShowEndOfLine(); }

			return res;
		}
		bool GetShowTabChar() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->GetShowTabChar(); }

			return res;
		}

		bool GetShowSpaceChar() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetShowSpaceChar(); }

			return res;
		}

		bool GetMarkActiveLine() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetMarkActiveLine(); }

			return res;
		}

		void MarkBracePair() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMarkBracePair(true); }
		}

		void NoMarkBracePair() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMarkBracePair(false); }
		}

		bool GetMarkBracePair() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetMarkBracePair(); }

			return res;
		}

		long GetMaxColumns() {
			long cols = 0;
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				cols = madedit->GetMaxColumns();
			return cols;
		}

		void SetMaxColumns(long cols) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMaxColumns(cols); }
		}

		bool IsAutoIndent() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetAutoIndent(); }

			return res;
		}

		void AutoIndent() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				madedit->SetAutoIndent(true);
		}

		void NoAutoIndent() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				madedit->SetAutoIndent(false);
		}

		void AutoCompletePair() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				madedit->SetAutoCompletePair(true);
		}

		void NoAutoCompletePair() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				madedit->SetAutoCompletePair(false);
		}

		bool GetAutoCompletePair() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetAutoCompletePair(); }

			return res;
		}

		void InsertPairForSelection() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				madedit->SetInsertPairForSelection(true);
		}

		void NoInsertPairForSelection() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
				madedit->SetInsertPairForSelection(false);
		}

		bool GetInsertPairForSelection() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetInsertPairForSelection(); }

			return res;
		}

		void InsertMode() { // true: insert, false: overwrite
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetInsertMode(true); }
		}

		void OverwriteMode() { // true: insert, false: overwrite
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetInsertMode(false); }
		}

		bool GetInsertMode() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetInsertMode(); }

			return res;
		}

		void SetCaretType(long caType) {
			MadEdit *madedit = g_CurrentMadEdit;
			MadCaretType catype = (MadCaretType)caType;
			if (ctVerticalLine > catype && catype > ctBlock)
				throw std::out_of_range("Valid range VerticalLine, HorizontalLine, Block");

			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetCaretType(catype); }
		}

		bool GetMouseSelectToCopy() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetMouseSelectToCopy(); }

			return res;
		}

		void MouseSelectToCopy() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMouseSelectToCopy(true); }
		}

		void NoMouseSelectToCopy() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMouseSelectToCopy(false); }
		}

		bool GetMouseSelectToCopyWithCtrlKey() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetMouseSelectToCopyWithCtrlKey(); }

			return res;
		}

		void MouseSelectToCopyWithCtrlKey() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMouseSelectToCopyWithCtrlKey(true); }
		}

		void NoMouseSelectToCopyWithCtrlKey() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMouseSelectToCopyWithCtrlKey(false); }
		}

		bool GetMiddleMouseToPaste() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetMiddleMouseToPaste(); }

			return res;
		}

		void MiddleMouseToPaste() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMiddleMouseToPaste(true); }
		}

		void NoMiddleMouseToPaste() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetMiddleMouseToPaste(true); }
		}

		long GetMaxWordWrapWidth() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetMaxWordWrapWidth(); }

			return res;
		}

		long GetUCharWidth(long uc) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetUCharWidth((ucs4_t)uc); }

			return res;
		}

		long GetHexUCharWidth(long uc) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetHexUCharWidth((ucs4_t)uc); }

			return res;
		}

		long GetUCharType(long uc) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetUCharType((ucs4_t)uc); }

			return res;
		}

		// all are zero-based
		mad_py::tuple GetCaretPositionA() {
			int line = 0, subrow = 0;
			wxFileOffset column = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GetCaretPosition(line, subrow, column); }

			return mad_py::make_tuple(line, subrow, (long)column);
		}

		long GetCaretPositionB() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetCaretPosition(); }

			return res;
		}

		const std::string GetFileName() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				wxString name;
				name = madedit->GetFileName();
				if (name.size())
					return std::string(name.mb_str(wxConvUTF8));
			}

			return std::string("");
		}

		long GetFileSize() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetFileSize(); }

			return res;
		}

		bool IsSelected() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->IsSelected(); }

			return res;
		}

		bool IsZeroSelected() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->IsSelected(); }

			return res;
		}

		long GetLineBeginPos(int line)	{
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetLineBeginPos(line); }

			return res;
		}

		long GetSelectionSize() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetSelectionSize(); }

			return res;
		}

		long GetIndentCountByPos(long endpos) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetIndentCountByPos((wxFileOffset) endpos); }

			return res;
			
		}

		long GetSelectionBeginPos() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetSelectionBeginPos(); }

			return res;
		}

		long GetSelectionEndPos() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetSelectionEndPos(); }

			return res;
		}

		// return -1 for no selection
		mad_py::tuple GetSelectionLineId() {
			int beginline = -1, endline = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GetSelectionLineId(beginline, endline); }

			return mad_py::make_tuple(beginline, endline);
		}

		void SetSelection(long beginpos, long endpos, bool bCaretAtBeginPos = false) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetSelection((wxFileOffset)beginpos, (wxFileOffset)endpos, bCaretAtBeginPos); }
		}

		void SelectWholeLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SelectWholeLine(); }
		}

		long GetLineCount() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->GetLineCount(); }

			return res;
		}

		void ConvertNewLineType(long nlType) {
			MadEdit *madedit = g_CurrentMadEdit;
			MadNewLineType nltype = (MadNewLineType)nlType;
			if (nltDefault > nltype && nltype > nltMAC)
				throw std::out_of_range("Valid range Default, DOS, UNIX, MAC)");
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ConvertNewLineType(nltype); }
		}

		void SetInsertNewLineType(long nlType) {
			MadEdit *madedit = g_CurrentMadEdit;
			MadNewLineType nltype = (MadNewLineType)nlType;
			if (nltDefault > nltype && nltype > nltMAC)
				throw std::out_of_range("Valid range Default, DOS, UNIX, MAC");
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetInsertNewLineType(nltype); }
		}

		long GetNewLineType() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetNewLineType(); }

			return res;
		}

		long GetInsertNewLineType() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetInsertNewLineType(); }

			return res;
		}

		bool IsModified() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = madedit->IsModified(); }

			return res;
		}

		long GetModificationTime() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ res = (long)madedit->GetModificationTime(); }

			return res;
		}

		void SetReadOnly() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetReadOnly(true); }
		}

		void ClearReadOnly() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetReadOnly(false); }
		}

		bool IsReadOnly() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->IsReadOnly(); }
			else
			{ return true; }
		}

		bool IsTextFile() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->IsTextFile(); }
			else
			{ return false; }
		}

		const std::string GetSelText() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				wxString ws;
				madedit->GetSelText(ws);
				if (ws.size())
					return std::string(ws.mb_str(wxConvUTF8));
			}
			return std::string("");
		}

		const std::string GetRangeText(long begpos, long endpos){
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				wxString ws;
				wxFileOffset bpos = begpos, epos = endpos;
				madedit->GetRangeText(ws, bpos, epos);
				if (ws.size())
					return std::string(ws.mb_str(wxConvUTF8));
			}
			return std::string("");
		}

		const std::string GetText(bool ignoreBOM = true) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				wxString ws;
				madedit->GetText(ws, ignoreBOM);
				if (ws.size())
					return std::string(ws.mb_str(wxConvUTF8));
			}
			return std::string("");
		}

		void SetText(const std::string &ws) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((! ws.empty()) && (madedit)) {
				wxString wxWs(ws.c_str(), wxConvUTF8);
				madedit->SetText(wxWs);
			}
		}

		// line: zero based
		// return true for full line, false for partial line
		mad_py::tuple GetLine(long line, size_t maxlen = 0, bool ignoreBOM = true) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				wxString wxWs;
				bool ret = madedit->GetLine(wxWs, line, maxlen, ignoreBOM);
				if (wxWs.size())
					return mad_py::make_tuple(std::string(wxWs.mb_str(wxConvUTF8)), ret);
			}

			return mad_py::make_tuple(std::string(""), false);
		}

		long GetLineByPos(long pos) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->GetLineByPos(pos); }
			else
			{ return -1; }
		}

		const std::string GetSelHexString(bool withSpace) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxString wxWs;
				madedit->GetSelHexString(wxWs, withSpace);
				return std::string(wxWs.mb_str(wxConvUTF8));
			}
			else
			{ return std::string(""); }
		}

		const std::string GetWordFromCaretPos() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxString wxWs;
				madedit->GetWordFromCaretPos(wxWs);
				return std::string(wxWs.mb_str(wxConvUTF8));
			}
			else
			{ return std::string(""); }
		}

		void CutLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CutLine(); }
		}

		void DeleteLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->DeleteLine(); }
		}

		void InsertDateTime() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->InsertDateTime(); }
		}

		void HighlightWords() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->HighlightWords(); }
		}

		void SelectAll() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SelectAll(); }
		}

		void Cut() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{
				if (!madedit->IsReadOnly())
				{ madedit->CutToClipboard(); }
				else
				{ madedit->CopyToClipboard(); }
			}
		}

		void Copy() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->CopyToClipboard(); }
		}

		void Paste() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->PasteFromClipboard(); }
		}

		void DndBegDrag() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->DndBegDrag(); }
		}

		void DndDrop() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->DndDrop(); }
		}

		bool CanPaste() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->CanPaste(); }
			else
			{ return false; }
		}

		void CopyText(const std::string &txt) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxString text(txt.c_str(), wxConvUTF8);
				madedit->CopyToClipboard(text);
			}
		}

		bool CanUndo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->CanUndo(); }
			return false;
		}

		bool CanRedo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->CanRedo(); }
			return false;
		}

		void Undo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->Undo(); }
		}

		void Redo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->Redo(); }
		}

		void Goto(long pos) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetCaretPosition((wxFileOffset)pos); }
		}

		void SetCaretPosition(long pos, long selbeg = -1, long selend = -1) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetCaretPosition((wxFileOffset)pos, (wxFileOffset)selbeg, (wxFileOffset)selend); }
		}

		void GoBack() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GoBack(); }
		}

		void GoForward() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GoForward(); }
		}
		
		bool HasBracePair() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ return madedit->HasBracePair(); }
			else
			{ return false; }
		}

		void GoToLeftBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GoToLeftBrace(); }
		}

		void GoToRightBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GoToRightBrace(); }
		}

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		long FindTextNext(const std::string &text,
						  bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool bPanChinese = false,
						  long rangeFrom = -1, long rangeTo = -1) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!text.empty())) {
				wxString wxText(text.c_str(), wxConvUTF8);
				wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
				
				if (bRegex) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}
				ok = madedit->FindTextNext(wxText, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, bPanChinese, from, to);
			}

			return ok;
		}

		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		long FindTextPrevious(const std::string &text,
							  bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool bPanChinese = false,
							  long rangeFrom = -1, long rangeTo = -1) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!text.empty())) {
				wxString wxText(text.c_str(), wxConvUTF8);
				wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
				
				if (bRegex) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}
				ok = madedit->FindTextPrevious(wxText, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, bPanChinese, from, to);
			}

			return ok;
		}

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		long FindHexNext(const std::string &hexstr, long rangeFrom = -1, long rangeTo = -1) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!hexstr.empty())) {
				wxString wxHexExpr(hexstr.c_str(), wxConvUTF8);
				wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
				ok = madedit->FindHexNext(wxHexExpr, from, to);
			}

			return ok;
		}

		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		long FindHexPrevious(const std::string &hexstr, long rangeFrom = -1, long rangeTo = -1) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!hexstr.empty())) {
				wxString wxHexExpr(hexstr.c_str(), wxConvUTF8);
				wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
				ok = madedit->FindHexPrevious(wxHexExpr, from, to);
			}

			return ok;
		}

		// replace the selected text that must match expr
		long ReplaceText(const std::string &expr, const std::string &fmt,
						 bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool bPanChinese = false,
						 long rangeFrom = -1, long rangeTo = -1) {
			if (expr.empty())
			{ return RR_EXPR_ERROR; }

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!(madedit) || madedit->IsReadOnly())
			{ return RR_NREP_NNEXT; }

			wxString wxExpr(expr.c_str(), wxConvUTF8), wxFmt(fmt.c_str(), wxConvUTF8);
			wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
			
			if (bRegex) bWholeWord = false;
			else
			{
				bDotMatchNewline = false;
				bPanChinese = false;
			}
			return madedit->ReplaceText(wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, bPanChinese, from, to);
		}

		// replace the selected text that must match expr
		long ReplaceTextNoDoubleCheck(const std::string &expr, const std::string &fmt,
						 bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool bPanChinese = false,
						 long rangeFrom = -1, long rangeTo = -1) {
			if (expr.empty())
			{ return RR_EXPR_ERROR; }

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!(madedit) || madedit->IsReadOnly())
			{ return RR_NREP_NNEXT; }

			wxString wxExpr(expr.c_str(), wxConvUTF8), wxFmt(fmt.c_str(), wxConvUTF8);
			wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
			
			if (bRegex) bWholeWord = false;
			else
			{
				bDotMatchNewline = false;
				bPanChinese = false;
			}
			return madedit->ReplaceTextNoDoubleCheck(wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, bPanChinese, from, to);
		}

		long ReplaceHex(const std::string &expr, const std::string &fmt,
						long rangeFrom = -1, long rangeTo = -1) {
			if (expr.empty())
			{ return RR_EXPR_ERROR; }

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (!(madedit) || madedit->IsReadOnly())
			{ return RR_NREP_NNEXT; }

			wxString wxExpr(expr.c_str(), wxConvUTF8), wxFmt(fmt.c_str(), wxConvUTF8);
			wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
			return madedit->ReplaceHex(wxExpr, wxFmt, from, to);
		}

		// return the replaced count or SR_EXPR_ERROR
		long ReplaceTextAll(const std::string &expr, const std::string &fmt,
							bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool bPanChinese = false, 
							long rangeFrom = -1, long rangeTo = -1) {
			long ok = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!expr.empty()) && (!madedit->IsReadOnly())) {
				wxString wxExpr(expr.c_str(), wxConvUTF8), wxFmt(fmt.c_str(), wxConvUTF8);
				
				if (bRegex) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}
				ok = madedit->ReplaceTextAll(wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, bPanChinese,
					nullptr, nullptr, (wxFileOffset)rangeFrom, (wxFileOffset)rangeTo);
			}

			return ok;
		}

		long ReplaceHexAll(const std::string &expr, const std::string &fmt,
						   long rangeFrom = -1, long rangeTo = -1) {
			long ok = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!expr.empty()) && (!madedit->IsReadOnly())) {
				wxString wxExpr(expr.c_str(), wxConvUTF8), wxFmt(fmt.c_str(), wxConvUTF8);
				ok = madedit->ReplaceHexAll(wxExpr, wxFmt, nullptr, nullptr, (wxFileOffset)rangeFrom, (wxFileOffset)rangeTo);
			}

			return ok;
		}

#ifndef PYMADEDIT_DLL
		// list the matched data to pbegpos & pendpos
		// return the found count or SR_EXPR_ERROR
		long FindTextAll(const std::string &expr, bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool bPanChinese = false, bool keepresults = true) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((!expr.empty()) && (madedit)) {
				wxString wxExpr(expr.c_str(), wxConvUTF8);
				vector<wxFileOffset> begpos, endpos;
				//wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
#if USE_GENERIC_TREECTRL
				wxGenericTreeCtrl* results = g_MainFrame->m_FindInFilesResults;
#else
				wxTreeCtrl* results = g_MainFrame->m_FindInFilesResults;
#endif
				// Root
				if (!keepresults){
					results->DeleteAllItems();
					results->AddRoot(wxT("Root"));
				}
				wxTreeItemId rtid = results->GetRootItem();
				wxASSERT(rtid.IsOk());
				wxTreeItemIdValue rtck;
				// First Level----search results summary
				wxTreeItemId id = results->GetFirstChild(rtid, rtck);
				if (id.IsOk())
				{
					// Second Level File results
					wxTreeItemIdValue cookie;
					wxTreeItemId lstid = results->GetFirstChild(id, cookie);
					if (lstid.IsOk() && results->IsExpanded(lstid))
						results->Collapse(lstid);
				}

				if (bRegex) bWholeWord = false;
				else
				{
					bDotMatchNewline = false;
					bPanChinese = false;
				}
				ok = madedit->FindTextAll(wxExpr, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, bPanChinese, false, &begpos, &endpos);

				if (ok >= 0 ) {
					static wxString text(_("Search Results"));
					int pid = g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
					wxASSERT(pid != wxNOT_FOUND);
					g_MainFrame->m_InfoNotebook->SetPageText((size_t)pid, text);
					wxString strtobesearch = _("Search \"") + wxExpr + wxT("\" ") + wxString::Format(_("(%s hits in 1 file)"), (wxLongLong(ok).ToString().c_str()));
					wxTreeItemId myroot = g_MainFrame->NewSearchSession(strtobesearch);
					DisplayFindAllResult(myroot, begpos, endpos, madedit);
				}

				{
					wxTreeItemIdValue cookie;
					id = results->GetFirstChild(rtid, cookie);
					if (id.IsOk())
					{
						if (!results->IsExpanded(id))
							results->Expand(id);
						// Second Level File results
						wxTreeItemIdValue ck;
						wxTreeItemId lstid = results->GetFirstChild(id, ck);
						if (lstid.IsOk() && !results->IsExpanded(lstid))
							results->Expand(lstid);
					}
				}
			}

			return ok;
		}

		long FindHexAll(const std::string &expr, bool showresults = true) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((!expr.empty()) && (madedit)) {
				wxString wxExpr(expr.c_str(), wxConvUTF8), fmt;
				vector<wxFileOffset> begpos, endpos;
				//wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
				ok = madedit->FindHexAll(wxExpr, false, &begpos, &endpos);

				if (ok >= 0 && showresults) {
					static wxString text(_("Search Results"));
					int pid = g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
					wxASSERT(pid != wxNOT_FOUND);
					g_MainFrame->m_InfoNotebook->SetPageText((size_t)pid, text);
					
					wxString strtobesearch = _("Search \"") + wxExpr + wxT("\" ") + wxString::Format(_("(%s hits in 1 file)"), (wxLongLong(ok).ToString().c_str()));
					wxTreeItemId myroot = g_MainFrame->NewSearchSession(strtobesearch);
					DisplayFindAllResult(myroot, begpos, endpos, madedit);
				}
			}

			return ok;
		}
#endif
		bool LoadFromFile(const std::string &filename, const std::string &encoding = std::string(""), bool bForceText = false) {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (! filename.empty())) {
				wxString wxEncoding(encoding.c_str(), wxConvUTF8);
				wxString wxFilename(filename.c_str(), wxConvUTF8);
				res = madedit->LoadFromFile(wxFilename, wxEncoding, bForceText);
			}

			return res;
		}

		bool SaveToFile(const std::string &filename) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((!filename.empty()) && (madedit) && (!madedit->IsReadOnly())) {
				wxString wxFilename(filename.c_str(), wxConvUTF8);
				return madedit->SaveToFile(wxFilename);
			}

			return false;
		}

		bool Reload() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) 
				return madedit->Reload();
			else
				return false;
		}

		// if the file is modified by another app, reload it.
		bool ReloadByModificationTime() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) 
				return madedit->ReloadByModificationTime(false);
			else
				return false;
		}

		// restore pos in Reload(), ConvertEncoding()
		void RestorePosition(long pos, long toprow) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxFileOffset wxPos = pos;
				madedit->RestorePosition(wxPos, toprow);
			}
		}

		// write back to the original FileName
		// if FileName is empty, ask the user to get filename
		// return wxID_YES(Saved), wxID_NO(Not Saved), or wxID_CANCEL
		long Save(bool ask, const std::string &title, bool saveas) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!title.empty()) && (!madedit->IsReadOnly())) {
				wxString wxTitle(title.c_str(), wxConvUTF8);
				return madedit->Save(ask, wxTitle, saveas);
			}
			else
			{ return wxID_NO; }
		}

		// add: gogo, 21.09.2009
		void ToggleBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->ToggleBookmark(); }
		}

		void GotoNextBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GotoNextBookmark(); }
		}

		void GotoPreviousBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->GotoPreviousBookmark(); }
		}

		//----------
		void ConvertEncoding(const std::string &newenc, long flag) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly())) {
				if (newenc.empty())
				{ return; }

				MadConvertEncodingFlag mflag = (MadConvertEncodingFlag)flag;
				wxString wxNewenc(newenc.c_str(), wxConvUTF8);
				madedit->ConvertEncoding(wxNewenc, mflag);
			}
		}

		void ConvertChineseA(long flag) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ConvertChinese((MadConvertEncodingFlag)flag); }
		}

		bool HasBOM() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) 
				return madedit->HasBOM();
			else
				return false;
		}

		void ToggleBOM() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ToggleBOM(); }
		}

		void IncreaseIndent() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->IncreaseDecreaseIndent(true); }
		}

		void DecreaseIndent() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->IncreaseDecreaseIndent(false); }
		}

		bool HasLineComment() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) 
				return madedit->HasLineComment();
			else
				return false;
		}

		void Comment() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CommentUncomment(true); }
		}

		void Uncomment() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CommentUncomment(false); }
		}

		void ToUpperCase() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ToUpperCase(); }
		}

		void ToLowerCase() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ToLowerCase(); }
		}

		void InvertCase() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->InvertCase(); }
		}

		void Capitalize() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->Capitalize(); }
		}

		void ToHalfWidth(bool ascii = true, bool japanese = true, bool korean = true, bool other = true) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ToHalfWidth(ascii, japanese, korean, other); }
		}

		void ToFullWidth(bool ascii = true, bool japanese = true, bool korean = true, bool other = true) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ToFullWidth(ascii, japanese, korean, other); }
		}

		void TrimTrailingSpaces() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->TrimTrailingSpaces(); }
		}

		void TrimLeadingSpaces() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->TrimLeadingSpaces(); }
		}

		void DeleteEmptyLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->DeleteEmptyLines(); }
		}


		void DeleteEmptyLinesWithSpaces() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->DeleteEmptyLinesWithSpaces(); }
		}

		void JoinLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->JoinLines(); }
		}

		// startline<0 : sort all lines otherwise sort [beginline, endline]
		void SortLines(MadSortFlags flags, long beginline, long endline) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->SortLines(flags, beginline, endline); }
		}

		// convert WordWraps to NewLine-chars in the SelText or whole file
		void ConvertWordWrapToNewLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ConvertWordWrapToNewLine(); }
		}
		// convert NewLine-chars to WordWraps in the SelText
		void ConvertNewLineToWordWrap() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ConvertNewLineToWordWrap(); }
		}

		void ConvertSpaceToTab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ConvertSpaceToTab(); }
		}
		void ConvertTabToSpace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ConvertTabToSpace(); }
		}

		void CopyAsHexString(bool withSpace) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->CopyAsHexString(withSpace); }
		}

		void CopyRevertHex(const std::string &delimiters) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxString wxDelimiters(delimiters.c_str(), wxConvUTF8);
				madedit->CopyRevertHex(wxDelimiters);
			}
		}

		mad_py::tuple WordCount(bool selection) {
			int words = 0, chars = 0, spaces = 0, lines = 0, halfwidths = 0, fullwidths = 0;
			wxString str;

			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit) {
				wxArrayString detail;
				madedit->WordCount(selection, words, chars, spaces, halfwidths, fullwidths, lines, &detail);

				for (size_t i = 0; i < detail.Count(); ++i) {
					str << detail[i] << wxT("\n");
				}
			}

			return mad_py::make_tuple(words, chars, spaces, lines, halfwidths, fullwidths, std::string(str.mb_str(wxConvUTF8)));
		}

		void SetColumnSelection(long startlineid, long startxpos, long hlines, long wlines) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (madedit)
			{ madedit->SetColumnSelection(startlineid, startxpos, hlines, wlines); }
		}

		void ColumnAlignLeft() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ColumnAlignLeft(); }
		}

		void ColumnAlignRight() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ColumnAlignRight(); }
		}
		/**=======================================================**/

		void Astyle() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (g_MainFrame && (madedit) && (!madedit->IsReadOnly()) && (!madedit->IsHexMode())) {
				wxCommandEvent event;
				g_MainFrame->OnToolsAstyleFormat(event);
			}
		}

		void XMLFormat() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (g_MainFrame && (madedit) && (!madedit->IsReadOnly()) && (!madedit->IsHexMode())) {
				wxCommandEvent event;
				g_MainFrame->OnToolsXMLFormat(event);
			}
		}

		void JSONFormat() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (g_MainFrame && (madedit) && (!madedit->IsReadOnly()) && (!madedit->IsHexMode())) {
				wxCommandEvent event;
				g_MainFrame->OnToolsJSONFormat(event);
			}
		}

		void Markdown2Html() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (g_MainFrame && (madedit) && (!madedit->IsReadOnly()) && (!madedit->IsHexMode())) {
				wxCommandEvent event;
				g_MainFrame->OnToolsMarkdown2Html(event);
			}
		}

		void Html2PlainText() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if (g_MainFrame && (madedit) && (!madedit->IsReadOnly()) && (!madedit->IsHexMode())) {
				wxCommandEvent event;
				g_MainFrame->OnToolsHtml2PlainText(event);
			}
		}

		void ToggleBookmarkInSearch(bool bookmark) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ToggleBookmarkInSearch(bookmark); }
		}

		void CutDelBookmarkedLines(bool copyLines = false) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CutDelBookmarkedLines(copyLines); }
		}

		void DeleteUnmarkedLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->DeleteUnmarkedLines(); }
		}

		void CopyUnmarkedLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CopyUnmarkedLines(); }
		}

		void CutUnmarkedLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CutUnmarkedLines(); }
		}

		void CopyCutDeleteUnmarkedLines(bool copyLines = false, bool deleteLines = false) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->CopyCutDeleteUnmarkedLines(copyLines, deleteLines); }
		}

		void ReplaceBookmarkedLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (!madedit)
				madedit = g_ActiveMadEdit;
			if ((madedit) && (!madedit->IsReadOnly()))
			{ madedit->ReplaceBookmarkedLines(); }
		}
	};

	int MsgBox(const std::string& message, const std::string& caption = "Message", long style = wxOK | wxCENTRE) {
		wxString wxMessage(message.c_str(), wxConvUTF8), wxCaption(caption.c_str(), wxConvUTF8);
		return MadMessageBox(wxMessage, wxCaption, style);
	}

	const std::string InputBox(const std::string& message, const std::string& caption = "Input Text") {
		wxString wxMessage(message.c_str(), wxConvUTF8), wxCaption(caption.c_str(), wxConvUTF8);
		wxString input = wxGetTextFromUser(wxMessage, wxCaption);
		return std::string(input.mb_str(wxConvUTF8));
	}

	void ConvertChineseInClipboard(long flag) {
		::ConvertChineseInClipboard((MadConvertChineseFlag)flag);
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindTextNext_member_overloads, FindTextNext, 4, 8)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindTextPrevious_member_overloads, FindTextPrevious, 4, 8)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindHexNext_member_overloads, FindHexNext, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindHexPrevious_member_overloads, FindHexPrevious, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindTextAll_member_overloads, FindTextAll, 4, 7)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindHexAll_member_overloads, FindHexAll, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceText_member_overloads, ReplaceText, 5, 9)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceHex_member_overloads, ReplaceHex, 2, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceTextAll_member_overloads, ReplaceTextAll, 5, 9)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceHexAll_member_overloads, ReplaceHexAll, 2, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(LoadFromFile_member_overloads, LoadFromFile, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ToHalfWidth_member_overloads, ToHalfWidth, 0, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ToFullWidth_member_overloads, ToFullWidth, 0, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetCaretPosition_member_overloads, SetCaretPosition, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GetLine_member_overloads, GetLine, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GetText_member_overloads, GetText, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetSelection_member_overloads, SetSelection, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CutDelBookmarkedLines_member_overloads, CutDelBookmarkedLines, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CopyCutDeleteUnmarkedLines_member_overloads, CopyCutDeleteUnmarkedLines, 0, 2)

BOOST_PYTHON_FUNCTION_OVERLOADS(InputBox_overloads, mad_python::InputBox, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(MsgBox_overloads, mad_python::MsgBox, 1, 3)

BOOST_PYTHON_MODULE(madpython) {
	using namespace mad_python;
	using namespace mad_py;
	class_<PyMadEdit>("MadEdit", "MadEdit Python API wrapper class", init<>())
	.def("ProcessCommand", &PyMadEdit::ProcessCommand, "")
	.def("CharFirst", &PyMadEdit::CharFirst, "")
	.def("CharLast", &PyMadEdit::CharLast, "")
	.def("CaretCommandFirst", &PyMadEdit::CaretCommandFirst, "")
	.def("Left", &PyMadEdit::Left, "")
	.def("Right", &PyMadEdit::Right, "")
	.def("Up", &PyMadEdit::Up, "")
	.def("Down", &PyMadEdit::Down, "")
	.def("BeginLine", &PyMadEdit::BeginLine, "")
	.def("EndLine", &PyMadEdit::EndLine, "")
	.def("BeginDoc", &PyMadEdit::BeginDoc, "")
	.def("EndDoc", &PyMadEdit::EndDoc, "")
	.def("PrevPage", &PyMadEdit::PrevPage, "")
	.def("NextPage", &PyMadEdit::NextPage, "")
	.def("PrevWord", &PyMadEdit::PrevWord, "")
	.def("NextWord", &PyMadEdit::NextWord, "")
	.def("LeftBrace", &PyMadEdit::LeftBrace, "")
	.def("RightBrace", &PyMadEdit::RightBrace, "")
	.def("CaretCommandLast", &PyMadEdit::CaretCommandLast, "")
	.def("SelCommandFirst", &PyMadEdit::SelCommandFirst, "")
	.def("SelLeft", &PyMadEdit::SelLeft, "")
	.def("SelRight", &PyMadEdit::SelRight, "")
	.def("SelUp", &PyMadEdit::SelUp, "")
	.def("SelDown", &PyMadEdit::SelDown, "")
	.def("SelBeginLine", &PyMadEdit::SelBeginLine, "")
	.def("SelEndLine", &PyMadEdit::SelEndLine, "")
	.def("SelBeginDoc", &PyMadEdit::SelBeginDoc, "")
	.def("SelEndDoc", &PyMadEdit::SelEndDoc, "")
	.def("SelPrevPage", &PyMadEdit::SelPrevPage, "")
	.def("SelNextPage", &PyMadEdit::SelNextPage, "")
	.def("SelPrevWord", &PyMadEdit::SelPrevWord, "")
	.def("SelNextWord", &PyMadEdit::SelNextWord, "")
	.def("SelLeftBrace", &PyMadEdit::SelLeftBrace, "")
	.def("SelRightBrace", &PyMadEdit::SelRightBrace, "")
	.def("SelCommandLast", &PyMadEdit::SelCommandLast, "")
	.def("Return", &PyMadEdit::Return, "")
	.def("ReturnNoIndent", &PyMadEdit::ReturnNoIndent, "")
	.def("Tab", &PyMadEdit::Tab, "")
	.def("InsertTabChar", &PyMadEdit::InsertTabChar, "")
	.def("Delete", &PyMadEdit::Delete, "")
	.def("BackSpace", &PyMadEdit::BackSpace, "")
	.def("DelPrevWord", &PyMadEdit::DelPrevWord, "")
	.def("DelNextWord", &PyMadEdit::DelNextWord, "")
	.def("ToggleInsertMode", &PyMadEdit::ToggleInsertMode, "")
	.def("InsertWChar", &PyMadEdit::InsertWChar, "")
	.def("InsertStr", &PyMadEdit::InsertStr, "")
	.def("InsertIncrementalNumber", &PyMadEdit::InsertIncrementalNumber, "")
	.def("GoToLine", &PyMadEdit::GoToLine, "Go To Line of current file")
	.def("GoToLineColumn", &PyMadEdit::GoToLineColumn, "Go To Line, Column of current file")
	.def("SetSyntax", &PyMadEdit::SetSyntax, "Set syntax for current file")
	.def("GetSyntaxTitle", &PyMadEdit::GetSyntaxTitle, return_value_policy<return_by_value>(), "")
	.def("LoadDefaultSyntaxScheme", &PyMadEdit::LoadDefaultSyntaxScheme, "")
	.def("SetEncoding", &PyMadEdit::SetEncoding, "")
	.def("GetEncodingName", &PyMadEdit::GetEncodingName, return_value_policy<return_by_value>(), "")
	.def("GetEncodingDescription", &PyMadEdit::GetEncodingDescription, return_value_policy<return_by_value>(), "")
	.def("GetEncodingType", &PyMadEdit::GetEncodingType, return_value_policy<return_by_value>(), "")
	.def("GetRecordCaretMovements", &PyMadEdit::GetRecordCaretMovements, return_value_policy<return_by_value>(), "")
	.def("RecordCaretMovements", &PyMadEdit::RecordCaretMovements, return_value_policy<return_by_value>(), "")
	.def("NotRecordCaretMovements", &PyMadEdit::NotRecordCaretMovements, return_value_policy<return_by_value>(), "")
	.def("SetTextFont", &PyMadEdit::SetTextFont, "")
	.def("SetHexFont", &PyMadEdit::SetHexFont, "")
	.def("GetTextFont", &PyMadEdit::GetTextFont, "")
	.def("GetHexFont", &PyMadEdit::GetHexFont, "")
	.def("GetFontNameSize", &PyMadEdit::GetFontNameSize, return_value_policy<return_by_value>(), "Doc")
	.def("FixedWidthMode", &PyMadEdit::FixedWidthMode, "")
	.def("NoFixedWidthMode", &PyMadEdit::NoFixedWidthMode, "")
	.def("GetFixedWidthMode", &PyMadEdit::GetFixedWidthMode, "")
	.def("SetLineSpacing", &PyMadEdit::SetLineSpacing, "")
	.def("GetLineSpacing", &PyMadEdit::GetLineSpacing, "")
	.def("TextMode", &PyMadEdit::TextMode, "")
	.def("ColumnMode", &PyMadEdit::ColumnMode, "")
	.def("HexMode", &PyMadEdit::HexMode, "")
	.def("GetEditMode", &PyMadEdit::GetEditMode, return_value_policy<return_by_value>(), "")
	.def("SingleLineMode", &PyMadEdit::SingleLineMode, "")
	.def("MultiLineMode", &PyMadEdit::MultiLineMode, "")
	.def("SetTabColumns", &PyMadEdit::SetTabColumns, "")
	.def("GetTabColumns", &PyMadEdit::GetTabColumns, "")
	.def("SetIndentColumns", &PyMadEdit::SetIndentColumns, "")
	.def("GetIndentColumns", &PyMadEdit::GetIndentColumns, "")
	.def("InsertSpacesInsteadOfTab", &PyMadEdit::InsertSpacesInsteadOfTab, "")
	.def("InsertTab", &PyMadEdit::InsertTab, "")
	.def("GetInsertSpacesInsteadOfTab", &PyMadEdit::GetInsertSpacesInsteadOfTab, "")
	.def("WantTab", &PyMadEdit::WantTab, "")
	.def("NotWantTab", &PyMadEdit::NotWantTab, "")
	.def("GetWantTab", &PyMadEdit::GetWantTab, "")
	.def("NoWrap", &PyMadEdit::NoWrap, "")
	.def("WrapByWindow", &PyMadEdit::WrapByWindow, "")
	.def("WrapByColumn", &PyMadEdit::WrapByColumn, "")
	.def("GetWordWrapMode", &PyMadEdit::GetWordWrapMode, "")
	.def("DisplayLineNumber", &PyMadEdit::DisplayLineNumber, "")
	.def("ShowEndOfLine", &PyMadEdit::ShowEndOfLine, "")
	.def("ShowTabChar", &PyMadEdit::ShowTabChar, "")
	.def("ShowSpaceChar", &PyMadEdit::ShowSpaceChar, "")
	.def("MarkActiveLine", &PyMadEdit::MarkActiveLine, "")
	.def("DisplayLineNumber", &PyMadEdit::DisplayLineNumber, "")
	.def("DisplayBookmark", &PyMadEdit::DisplayBookmark, "")
	.def("DisplayColHint", &PyMadEdit::DisplayColHint, "")
	.def("HideEndOfLine", &PyMadEdit::HideEndOfLine, "")
	.def("HideTabChar", &PyMadEdit::HideTabChar, "")
	.def("HideSpaceChar", &PyMadEdit::HideSpaceChar, "")
	.def("HideMarkActiveLine", &PyMadEdit::HideMarkActiveLine, "")
	.def("HideBookmark", &PyMadEdit::HideBookmark, "")
	.def("HideColHint", &PyMadEdit::HideColHint, "")
	.def("GetDisplayLineNumber", &PyMadEdit::GetDisplayLineNumber, "")
	.def("GetShowEndOfLine", &PyMadEdit::GetShowEndOfLine, "")
	.def("GetShowTabChar", &PyMadEdit::GetShowTabChar, "")
	.def("GetShowSpaceChar", &PyMadEdit::GetShowSpaceChar, "")
	.def("GetMarkActiveLine", &PyMadEdit::GetMarkActiveLine, "")
	.def("MarkBracePair", &PyMadEdit::MarkBracePair, "")
	.def("NoMarkBracePair", &PyMadEdit::NoMarkBracePair, "")
	.def("GetMarkBracePair", &PyMadEdit::GetMarkBracePair, "")
	.def("GetMaxColumns", &PyMadEdit::GetMaxColumns, "")
	.def("SetMaxColumns", &PyMadEdit::SetMaxColumns, "")
	.def("IsAutoIndent", &PyMadEdit::IsAutoIndent, "")
	.def("AutoIndent", &PyMadEdit::AutoIndent, "")
	.def("NoAutoIndent", &PyMadEdit::NoAutoIndent, "")
	.def("AutoCompletePair", &PyMadEdit::AutoCompletePair, "")
	.def("NoAutoCompletePair", &PyMadEdit::NoAutoCompletePair, "")
	.def("GetAutoCompletePair", &PyMadEdit::GetAutoCompletePair, "")
	.def("InsertPairForSelection", &PyMadEdit::InsertPairForSelection, "")
	.def("NoInsertPairForSelection", &PyMadEdit::NoInsertPairForSelection, "")
	.def("GetInsertPairForSelection", &PyMadEdit::GetInsertPairForSelection, "")
	.def("InsertMode", &PyMadEdit::InsertMode, "")
	.def("OverwriteMode", &PyMadEdit::OverwriteMode, "")
	.def("GetInsertMode", &PyMadEdit::GetInsertMode, "")
	.def("SetCaretType", &PyMadEdit::SetCaretType, "")
	.def("GetMouseSelectToCopy", &PyMadEdit::GetMouseSelectToCopy, "")
	.def("MouseSelectToCopy", &PyMadEdit::MouseSelectToCopy, "")
	.def("NoMouseSelectToCopy", &PyMadEdit::NoMouseSelectToCopy, "")
	.def("GetMouseSelectToCopyWithCtrlKey", &PyMadEdit::GetMouseSelectToCopyWithCtrlKey, "")
	.def("MouseSelectToCopyWithCtrlKey", &PyMadEdit::MouseSelectToCopyWithCtrlKey, "")
	.def("NoMouseSelectToCopyWithCtrlKey", &PyMadEdit::NoMouseSelectToCopyWithCtrlKey, "")
	.def("GetMiddleMouseToPaste", &PyMadEdit::GetMiddleMouseToPaste, "")
	.def("MiddleMouseToPaste", &PyMadEdit::MiddleMouseToPaste, "")
	.def("NoMiddleMouseToPaste", &PyMadEdit::NoMiddleMouseToPaste, "")
	.def("GetMaxWordWrapWidth", &PyMadEdit::GetMaxWordWrapWidth, "")
	.def("GetUCharWidth", &PyMadEdit::GetUCharWidth, "")
	.def("GetHexUCharWidth", &PyMadEdit::GetHexUCharWidth, "")
	.def("GetUCharType", &PyMadEdit::GetUCharType, "")
	.def("GetCaretPositionA", &PyMadEdit::GetCaretPositionA, return_value_policy<return_by_value>(), "")
	.def("GetCaretPositionB", &PyMadEdit::GetCaretPositionB, return_value_policy<return_by_value>(), "")
	.def("GetFileName", &PyMadEdit::GetFileName, return_value_policy<return_by_value>(), "")
	.def("GetFileSize", &PyMadEdit::GetFileSize, return_value_policy<return_by_value>(), "")
	.def("IsSelected", &PyMadEdit::IsSelected, "")
	.def("IsZeroSelected", &PyMadEdit::IsZeroSelected, "")
	.def("GetLineBeginPos", &PyMadEdit::GetLineBeginPos, return_value_policy<return_by_value>(), "")
	.def("GetSelectionSize", &PyMadEdit::GetSelectionSize, return_value_policy<return_by_value>(), "")
	.def("GetSelectionBeginPos", &PyMadEdit::GetSelectionBeginPos, return_value_policy<return_by_value>(), "")
	.def("GetSelectionEndPos", &PyMadEdit::GetSelectionEndPos, return_value_policy<return_by_value>(), "")
	.def("GetSelectionLineId", &PyMadEdit::GetSelectionLineId, return_value_policy<return_by_value>(), "")
	.def("GetLineCount", &PyMadEdit::GetLineCount, "")
	.def("ConvertNewLineType", &PyMadEdit::ConvertNewLineType, "")
	.def("SetInsertNewLineType", &PyMadEdit::SetInsertNewLineType, "")
	.def("GetNewLineType", &PyMadEdit::GetNewLineType, "")
	.def("GetInsertNewLineType", &PyMadEdit::GetInsertNewLineType, "")
	.def("IsModified", &PyMadEdit::IsModified, "")
	.def("GetModificationTime", &PyMadEdit::GetModificationTime, "")
	.def("SetReadOnly", &PyMadEdit::SetReadOnly, "")
	.def("ClearReadOnly", &PyMadEdit::ClearReadOnly, "")
	.def("IsReadOnly", &PyMadEdit::IsReadOnly, "")
	.def("IsTextFile", &PyMadEdit::IsTextFile, "")
	.def("GetSelText", &PyMadEdit::GetSelText, return_value_policy<return_by_value>(), "")
	.def("GetRangeText", &PyMadEdit::GetRangeText, return_value_policy<return_by_value>(), "")
	.def("SetText", &PyMadEdit::SetText, "")
	.def("Goto", &PyMadEdit::Goto, "")
	.def("GetLineByPos", &PyMadEdit::GetLineByPos, "")
	.def("GetSelHexString", &PyMadEdit::GetSelHexString, "")
	.def("GetWordFromCaretPos", &PyMadEdit::GetWordFromCaretPos, return_value_policy<return_by_value>(), "")
	.def("InsertNewline", &PyMadEdit::InsertNewline, "")
	.def("CutLine", &PyMadEdit::CutLine, "")
	.def("DeleteLine", &PyMadEdit::DeleteLine, "")
	.def("InsertDateTime", &PyMadEdit::InsertDateTime, "")
	.def("HighlightWords", &PyMadEdit::HighlightWords, "")
	.def("SelectAll", &PyMadEdit::SelectAll, "")
	.def("Copy", &PyMadEdit::Copy, "")
	.def("CopyText", &PyMadEdit::CopyText, "")
	.def("Cut", &PyMadEdit::Cut, "")
	.def("Paste", &PyMadEdit::Paste, "")
	.def("DndBegDrag", &PyMadEdit::DndBegDrag, "")
	.def("DndDrop", &PyMadEdit::DndDrop, "")
	.def("CanPaste", &PyMadEdit::CanPaste, "")
	.def("CanUndo", &PyMadEdit::CanUndo, return_value_policy<return_by_value>(), "")
	.def("CanRedo", &PyMadEdit::CanRedo, return_value_policy<return_by_value>(), "")
	.def("Undo", &PyMadEdit::Undo, "")
	.def("Redo", &PyMadEdit::Redo, "")
	.def("HasBracePair", &PyMadEdit::HasBracePair, return_value_policy<return_by_value>(), "")
	.def("GoToLeftBrace", &PyMadEdit::GoToLeftBrace, "")
	.def("GoToRightBrace", &PyMadEdit::GoToRightBrace, "")
	.def("GoBack", &PyMadEdit::GoBack, "")
	.def("GoForward", &PyMadEdit::GoForward, "")
	.def("SaveToFile", &PyMadEdit::SaveToFile, "")
	.def("Reload", &PyMadEdit::Reload, "")
	.def("ReloadByModificationTime", &PyMadEdit::ReloadByModificationTime, "")
	.def("RestorePosition", &PyMadEdit::RestorePosition, "")
	.def("Save", &PyMadEdit::Save, "")
	.def("ToggleBookmark", &PyMadEdit::ToggleBookmark, "")
	.def("GotoNextBookmark", &PyMadEdit::GotoNextBookmark, "")
	.def("GotoPreviousBookmark", &PyMadEdit::GotoPreviousBookmark, "")
	.def("ConvertEncoding", &PyMadEdit::ConvertEncoding, "")
	.def("ConvertChineseA", &PyMadEdit::ConvertChineseA, "")
	.def("HasBOM", &PyMadEdit::HasBOM, "")
	.def("ToggleBOM", &PyMadEdit::ToggleBOM, "")
	.def("IncreaseIndent", &PyMadEdit::IncreaseIndent, "")
	.def("DecreaseIndent", &PyMadEdit::DecreaseIndent, "")
	.def("HasLineComment", &PyMadEdit::HasLineComment, "")
	.def("Comment", &PyMadEdit::Comment, "")
	.def("Uncomment", &PyMadEdit::Uncomment, "")
	.def("ToUpperCase", &PyMadEdit::ToUpperCase, "")
	.def("ToLowerCase", &PyMadEdit::ToLowerCase, "")
	.def("InvertCase", &PyMadEdit::InvertCase, "")
	.def("Capitalize", &PyMadEdit::Capitalize, "")
	.def("TrimTrailingSpaces", &PyMadEdit::TrimTrailingSpaces, "")
	.def("TrimLeadingSpaces", &PyMadEdit::TrimLeadingSpaces, "")
	.def("DeleteEmptyLinesWithSpaces", &PyMadEdit::DeleteEmptyLinesWithSpaces, "")
	.def("DeleteEmptyLines", &PyMadEdit::DeleteEmptyLines, "")
	.def("JoinLines", &PyMadEdit::JoinLines, "")
	.def("SortLines", &PyMadEdit::SortLines, "")
	.def("ConvertWordWrapToNewLine", &PyMadEdit::ConvertWordWrapToNewLine, "")
	.def("ConvertNewLineToWordWrap", &PyMadEdit::ConvertNewLineToWordWrap, "")
	.def("ConvertSpaceToTab", &PyMadEdit::ConvertSpaceToTab, "")
	.def("ConvertTabToSpace", &PyMadEdit::ConvertTabToSpace, "")
	.def("CopyAsHexString", &PyMadEdit::CopyAsHexString, "")
	.def("CopyRevertHex", &PyMadEdit::CopyRevertHex, "")
	.def("WordCount", &PyMadEdit::WordCount, return_value_policy<return_by_value>(), "")
	.def("SetColumnSelection", &PyMadEdit::SetColumnSelection, "")
	.def("SetFontA", &PyMadEdit::SetFontA, "Doc")
	.def("ScrollLineUp", &PyMadEdit::ScrollLineUp, "")
	.def("ScrollLineDown", &PyMadEdit::ScrollLineDown, "")
	.def("ScrollPageUp", &PyMadEdit::ScrollPageUp, "")
	.def("ScrollPageDown", &PyMadEdit::ScrollPageDown, "")
	.def("ScrollLeft", &PyMadEdit::ScrollLeft, "")
	.def("ScrollRight", &PyMadEdit::ScrollRight, "")
	.def("ColumnAlignLeft", &PyMadEdit::ColumnAlignLeft, "")
	.def("ColumnAlignRight", &PyMadEdit::ColumnAlignRight, "")
	.def("Astyle", &PyMadEdit::Astyle, "")
	.def("XMLFormat", &PyMadEdit::XMLFormat, "")
	.def("JSONFormat", &PyMadEdit::JSONFormat, "")
	.def("Markdown2Html", &PyMadEdit::Markdown2Html, "")
	.def("Html2PlainText", &PyMadEdit::Html2PlainText, "")
	.def("SelectWholeLine", &PyMadEdit::SelectWholeLine, "")
	.def("ToggleBookmarkInSearch", &PyMadEdit::ToggleBookmarkInSearch, "Doc string")
	.def("GetIndentCountByPos", &PyMadEdit::GetIndentCountByPos, "")
	.def("CutDelBookmarkedLines", &PyMadEdit::CutDelBookmarkedLines, "")
	.def("DeleteUnmarkedLines", &PyMadEdit::DeleteUnmarkedLines, "")
	.def("CopyUnmarkedLines", &PyMadEdit::CopyUnmarkedLines, "")
	.def("CutUnmarkedLines", &PyMadEdit::CutUnmarkedLines, "")
	.def("ReplaceBookmarkedLines", &PyMadEdit::ReplaceBookmarkedLines, "")
	.def("FindTextNext", &PyMadEdit::FindTextNext, FindTextNext_member_overloads(args("text", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "bPanChinese", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("FindTextPrevious", &PyMadEdit::FindTextPrevious, FindTextPrevious_member_overloads(args("text", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "bPanChinese", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("FindHexNext", &PyMadEdit::FindHexNext, FindHexNext_member_overloads(args("hexstr", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("FindHexPrevious", &PyMadEdit::FindHexPrevious, FindHexPrevious_member_overloads(args("hexstr", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("ReplaceText", &PyMadEdit::ReplaceText, ReplaceText_member_overloads(args("expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "bPanChinese", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("ReplaceTextNoDoubleCheck", &PyMadEdit::ReplaceTextNoDoubleCheck, ReplaceText_member_overloads(args("expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "bPanChinese", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("ReplaceHex", &PyMadEdit::ReplaceHex, ReplaceHex_member_overloads(args("expr", "fmt"), "Doc string")[return_value_policy<return_by_value>()])
	.def("ReplaceTextAll", &PyMadEdit::ReplaceTextAll, ReplaceTextAll_member_overloads(args("expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "bPanChinese", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
	.def("ReplaceHexAll", &PyMadEdit::ReplaceHexAll, ReplaceHexAll_member_overloads(args("expr", "fmt", "rangeFrom", "rangeTo"), "Doc string")[return_value_policy<return_by_value>()])
#ifndef PYMADEDIT_DLL
	.def("FindTextAll", &PyMadEdit::FindTextAll, FindTextAll_member_overloads(args("expr", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "bPanChinese", "keepresults"), "Doc string")[return_value_policy<return_by_value>()])
	.def("FindHexAll", &PyMadEdit::FindHexAll, FindHexAll_member_overloads(args("expr", "showresults"), "Doc string")[return_value_policy<return_by_value>()])
#endif
	.def("LoadFromFile", &PyMadEdit::LoadFromFile, LoadFromFile_member_overloads(args("filename", "encoding", "bForceText"), "Doc string")[return_value_policy<return_by_value>()])
	.def("ToHalfWidth", &PyMadEdit::ToHalfWidth, ToHalfWidth_member_overloads(args("ascii", "japanese", "korean", "other"), "Doc string"))
	.def("ToFullWidth", &PyMadEdit::ToFullWidth, ToFullWidth_member_overloads(args("ascii", "japanese", "korean", "other"), "Doc string"))
	.def("SetCaretPosition", &PyMadEdit::SetCaretPosition, SetCaretPosition_member_overloads(args("pos", "selbeg", "selend"), "Doc string"))
	.def("GetLine", &PyMadEdit::GetLine, GetLine_member_overloads(args("line", "maxlen", "ignoreBOM"), "Doc string")[return_value_policy<return_by_value>()])
	.def("SetSelection", &PyMadEdit::SetSelection, SetSelection_member_overloads(args("beginpos", "endpos", "bCaretAtBeginPos"), "Doc string"))
	.def("GetText", &PyMadEdit::GetText, GetText_member_overloads(args("ignoreBOM"), "Doc string")[return_value_policy<return_by_value>()])
	.def("CopyCutDeleteUnmarkedLines", &PyMadEdit::CopyCutDeleteUnmarkedLines, CopyCutDeleteUnmarkedLines_member_overloads(args("copyLines", "deleteLines"), "Doc string"))
	;
	enum_<MadEditCmd>("MadEditCommand")
	.value("None", ecNone)
	.value("CharFirst", ecCharFirst)
	.value("CharLast", ecCharLast)
	.value("CaretCommandFirst", ecCaretCommandFirst)
	.value("Left", ecLeft)
	.value("Right", ecRight)
	.value("Up", ecUp)
	.value("Down", ecDown)
	.value("BeginLine", ecBeginLine)
	.value("EndLine", ecEndLine)
	.value("BeginDoc", ecBeginDoc)
	.value("EndDoc", ecEndDoc)
	.value("PrevPage", ecPrevPage)
	.value("NextPage", ecNextPage)
	.value("PrevWord", ecPrevWord)
	.value("NextWord", ecNextWord)
	.value("LeftBrace", ecLeftBrace)
	.value("RightBrace", ecRightBrace)
	.value("CaretCommandLast", ecCaretCommandLast)
	.value("SelCommandFirst", ecSelCommandFirst)
	.value("SelLeft", ecSelLeft)
	.value("SelRight", ecSelRight)
	.value("SelUp", ecSelUp)
	.value("SelDown", ecSelDown)
	.value("SelBeginLine", ecSelBeginLine)
	.value("SelEndLine", ecSelEndLine)
	.value("SelBeginDoc", ecSelBeginDoc)
	.value("SelEndDoc", ecSelEndDoc)
	.value("SelPrevPage", ecSelPrevPage)
	.value("SelNextPage", ecSelNextPage)
	.value("SelPrevWord", ecSelPrevWord)
	.value("SelNextWord", ecSelNextWord)
	.value("SelLeftBrace", ecSelLeftBrace)
	.value("SelRightBrace", ecSelRightBrace)
	.value("SelCommandLast", ecSelCommandLast)
	.value("HighlightWords", ecHighlightWords)
	.value("SelectAll", ecSelectAll)
	.value("ScrollLineUp", ecScrollLineUp)
	.value("ScrollLineDown", ecScrollLineDown)
	.value("ScrollPageUp", ecScrollPageUp)
	.value("ScrollPageDown", ecScrollPageDown)
	.value("ScrollLeft", ecScrollLeft)
	.value("ScrollRight", ecScrollRight)
	.value("Return", ecReturn)
	.value("ReturnNoIndent", ecReturnNoIndent)
	.value("Tab", ecTab)
	.value("InsertTabChar", ecInsertTabChar)
	.value("Delete", ecDelete)
	.value("BackSpace", ecBackSpace)
	.value("DelPrevWord", ecDelPrevWord)
	.value("DelNextWord", ecDelNextWord)
	.value("CutLine", ecCutLine)
	.value("DeleteLine", ecDeleteLine)
	.value("Undo", ecUndo)
	.value("Redo", ecRedo)
	.value("Cut", ecCut)
	.value("Copy", ecCopy)
	.value("Paste", ecPaste)
	.value("ToggleInsertMode", ecToggleInsertMode)
	.value("TextMode", ecTextMode)
	.value("ColumnMode", ecColumnMode)
	.value("HexMode", ecHexMode)
	.value("NoWrap", ecNoWrap)
	.value("WrapByWindow", ecWrapByWindow)
	.value("WrapByColumn", ecWrapByColumn)
	.value("ToggleWindow", ecToggleWindow)
	.value("IncreaseIndent", ecIncreaseIndent)
	.value("DecreaseIndent", ecDecreaseIndent)
	.value("Comment", ecComment)
	.value("Uncomment", ecUncomment)
	.value("ToUpperCase", ecToUpperCase)
	.value("ToLowerCase", ecToLowerCase)
	.value("InvertCase", ecInvertCase)
	.value("ToHalfWidth", ecToHalfWidth)
	.value("ToFullWidth", ecToFullWidth)
	.value("InsertDateTime", ecInsertDateTime)
	.value("MouseNotify", ecMouseNotify)
	;
	enum_<MadCaretType>("MadCaretType")
	.value("VerticalLine", ctVerticalLine)
	.value("HorizontalLine", ctHorizontalLine)
	.value("Block", ctBlock)
	;
	enum_<MadNewLineType>("MadNewLineType")
	.value("Default", nltDefault)
	.value("DOS", nltDOS) /*0D0A*/
	.value("UNIX", nltUNIX) /*0A*/
	.value("MAC", nltMAC) /*0D*/
	;
	enum_<MadConvertEncodingFlag>("MadConvertEncodingFlag")
	.value("None", cefNone)
	.value("SC2TC", cefSC2TC) // Simplified Chinese  ==> Traditional Chinese
	.value("TC2SC", cefTC2SC) // Traditional Chinese ==> Simplified Chinese
	.value("JK2TC", cefJK2TC) // Japanese Kanji      ==> Traditional Chinese
	.value("JK2SC", cefJK2SC) // Japanese Kanji      ==> Simplified Chinese
	.value("C2JK", cefC2JK) // Trad&Simp Chinese   ==> Japanese Kanji
	;
	enum_<MadConvertChineseFlag>("MadConvertChineseFlag")
	.value("Simp2Trad", ccfSimp2Trad) // Simplified Chinese  ==> Traditional Chinese
	.value("Trad2Simp", ccfTrad2Simp) // Traditional Chinese ==> Simplified Chinese
	.value("Kanji2Trad", ccfKanji2Trad) // Japanese Kanji      ==> Traditional Chinese
	.value("Kanji2Simp", ccfKanji2Simp) // Japanese Kanji      ==> Simplified Chinese
	.value("Chinese2Kanji", ccfChinese2Kanji) // Trad&Simp Chinese   ==> Japanese Kanji
	;
	enum_<MadNumberingStepType>("MadNumberingStepType")
	.value("Linear", mnstLinear)
	.value("Exponential", mnstExponential)
	;
	enum_<MadNumberFormat>("MadNumberFormat")
	.value("DEC", nfDEC)
	.value("HEX", nfHEX)
	.value("BIN", nfBIN)
	.value("OCT", nfOCT)
	;
	enum_<MadNumberAlign>("MadNumberAlign")
	.value("Left", naLeft)
	.value("Right", naRight)
	;
	enum_<MadSortLinesFlags>("MadSortLinesFlags")
	.value("Ascending", sfAscending)
	.value("Descending", sfDescending)
	.value("CaseSensitive", sfCaseSensitive)
	.value("RemoveDuplicate", sfRemoveDuplicate)
	.value("NumericSort", sfNumericSort)
	;
	enum_<MadMsgBoxStyle>("MadDlgStyle")
	.value("YES",                      MAD_YES)
	.value("OK",                       MAD_OK)
	.value("NO",                       MAD_NO)
	.value("CANCEL",                   MAD_CANCEL)
	.value("APPLY",                    MAD_APPLY)
	.value("CLOSE",                    MAD_CLOSE)
	.value("NO_DEFAULT",               MAD_NO_DEFAULT)/* only valid with wxYES_NO */ 
	.value("CANCEL_DEFAULT",           MAD_CANCEL_DEFAULT)/* only valid with wxCANCEL */ 
	.value("ICON_WARNING_EXCLAMATION", MAD_ICON_WARNING_EXCLAMATION)
	.value("ICON_ERROR_HAND_STOP",     MAD_ICON_ERROR_HAND_STOP)
	.value("ICON_QUESTION",            MAD_ICON_QUESTION)
	.value("ICON_INFORMATION",         MAD_ICON_INFORMATION)
	.value("ICON_NONE",                MAD_ICON_NONE)
	.value("CENTRE",                   MAD_CENTRE)
	;
	enum_<MadMsgBoxRet>("MadMsgBoxRet")
	.value("OK", MADRET_OK)
	.value("CANCEL", MADRET_CANCEL)
	.value("APPLY", MADRET_APPLY)
	.value("YES", MADRET_YES)
	.value("NO", MADRET_NO)
	;
	def("MsgBox", &MsgBox, MsgBox_overloads(args("message", "caption", "style"), "Message Dialog Box"));
	def("InputBox", &InputBox, InputBox_overloads(args("message", "caption"), "Input Dialog Box"));
	def("ConvertChineseInClipboard", &mad_python::ConvertChineseInClipboard, "");
}
#endif //__MADPYTHON__
