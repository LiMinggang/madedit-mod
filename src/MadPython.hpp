#ifndef __MADPYTHON__
#define __MADPYTHON__
#include <stdexcept>
//#include <Python.h>

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <boost/function.hpp>

#include <iostream>
#include <string>
#include <memory>

#include "MadEdit/MadEncoding.h"
#include "MadEdit/MadEdit.h"
#include "MadEdit/MadEditCommand.h"

#ifndef PYMADEDIT_DLL
#include "MadEditFrame.h"
extern wxStatusBar *g_StatusBar;
extern MadEdit *g_ActiveMadEdit;
#else
MadEdit *g_ActiveMadEdit = NULL;
wxFrame g_DummyWin;
#endif
extern void DisplayFindAllResult(vector<wxFileOffset> &begpos, vector<wxFileOffset> &endpos, MadEdit *madedit, bool expandresults = true, OnProgressUpdatePtr updater = NULL);

// Ugly bigger switch than bigger map
bool FromCmdToString(wxString &cmdStr, int madCmd)
{
    switch(madCmd)
    {
        case ecCharFirst:
            cmdStr<<(wxT("MadEditCommand.CharFirst"));
            break;
        case ecCharLast:
            cmdStr<<(wxT("MadEditCommand.CharLast"));
            break;
        case ecCaretCommandFirst:
            cmdStr<<(wxT("MadEditCommand.CaretCommandFirst"));
            break;
        case ecLeft:
            cmdStr<<(wxT("MadEditCommand.Left"));
            break;
        case ecRight:
            cmdStr<<(wxT("MadEditCommand.Right"));
            break;
        case ecUp:
            cmdStr<<(wxT("MadEditCommand.Up"));
            break;
        case ecDown:
            cmdStr<<(wxT("MadEditCommand.Down"));
            break;
        case ecBeginLine:
            cmdStr<<(wxT("MadEditCommand.BeginLine"));
            break;
        case ecEndLine:
            cmdStr<<(wxT("MadEditCommand.EndLine"));
            break;
        case ecBeginDoc:
            cmdStr<<(wxT("MadEditCommand.BeginDoc"));
            break;
        case ecEndDoc:
            cmdStr<<(wxT("MadEditCommand.EndDoc"));
            break;
        case ecPrevPage:
            cmdStr<<(wxT("MadEditCommand.PrevPage"));
            break;
        case ecNextPage:
            cmdStr<<(wxT("MadEditCommand.NextPage"));
            break;
        case ecPrevWord:
            cmdStr<<(wxT("MadEditCommand.PrevWord"));
            break;
        case ecNextWord:
            cmdStr<<(wxT("MadEditCommand.NextWord"));
            break;
        case ecLeftBrace:
            cmdStr<<(wxT("MadEditCommand.LeftBrace"));
            break;
        case ecRightBrace:
            cmdStr<<(wxT("MadEditCommand.RightBrace"));
            break;
        case ecCaretCommandLast:
            cmdStr<<(wxT("MadEditCommand.CaretCommandLast"));
            break;
        case ecSelCommandFirst:
            cmdStr<<(wxT("MadEditCommand.SelCommandFirst"));
            break;
        case ecSelLeft:
            cmdStr<<(wxT("MadEditCommand.SelLeft"));
            break;
        case ecSelRight:
            cmdStr<<(wxT("MadEditCommand.SelRight"));
            break;
        case ecSelUp:
            cmdStr<<(wxT("MadEditCommand.SelUp"));
            break;
        case ecSelDown:
            cmdStr<<(wxT("MadEditCommand.SelDown"));
            break;
        case ecSelBeginLine:
            cmdStr<<(wxT("MadEditCommand.SelBeginLine"));
            break;
        case ecSelEndLine:
            cmdStr<<(wxT("MadEditCommand.SelEndLine"));
            break;
        case ecSelBeginDoc:
            cmdStr<<(wxT("MadEditCommand.SelBeginDoc"));
            break;
        case ecSelEndDoc:
            cmdStr<<(wxT("MadEditCommand.SelEndDoc"));
            break;
        case ecSelPrevPage:
            cmdStr<<(wxT("MadEditCommand.SelPrevPage"));
            break;
        case ecSelNextPage:
            cmdStr<<(wxT("MadEditCommand.SelNextPage"));
            break;
        case ecSelPrevWord:
            cmdStr<<(wxT("MadEditCommand.SelPrevWord"));
            break;
        case ecSelNextWord:
            cmdStr<<(wxT("MadEditCommand.SelNextWord"));
            break;
        case ecSelLeftBrace:
            cmdStr<<(wxT("MadEditCommand.SelLeftBrace"));
            break;
        case ecSelRightBrace:
            cmdStr<<(wxT("MadEditCommand.SelRightBrace"));
            break;
        case ecSelCommandLast:
            cmdStr<<(wxT("MadEditCommand.SelCommandLast"));
            break;
        case ecHighlightWords:
            cmdStr<<(wxT("MadEditCommand.HighlightWords"));
            break;
        case ecSelectAll:
            cmdStr<<(wxT("MadEditCommand.SelectAll"));
            break;
        case ecScrollLineUp:
            cmdStr<<(wxT("MadEditCommand.ScrollLineUp"));
            break;
        case ecScrollLineDown:
            cmdStr<<(wxT("MadEditCommand.ScrollLineDown"));
            break;
        case ecScrollPageUp:
            cmdStr<<(wxT("MadEditCommand.ScrollPageUp"));
            break;
        case ecScrollPageDown:
            cmdStr<<(wxT("MadEditCommand.ScrollPageDown"));
            break;
        case ecScrollLeft:
            cmdStr<<(wxT("MadEditCommand.ScrollLeft"));
            break;
        case ecScrollRight:
            cmdStr<<(wxT("MadEditCommand.ScrollRight"));
            break;
        case ecReturn:
            cmdStr<<(wxT("MadEditCommand.Return"));
            break;
        case ecReturnNoIndent:
            cmdStr<<(wxT("MadEditCommand.ReturnNoIndent"));
            break;
        case ecTab:
            cmdStr<<(wxT("MadEditCommand.Tab"));
            break;
        case ecInsertTabChar:
            cmdStr<<(wxT("MadEditCommand.InsertTabChar"));
            break;
        case ecDelete:
            cmdStr<<(wxT("MadEditCommand.Delete"));
            break;
        case ecBackSpace:
            cmdStr<<(wxT("MadEditCommand.BackSpace"));
            break;
        case ecDelPrevWord:
            cmdStr<<(wxT("MadEditCommand.DelPrevWord"));
            break;
        case ecDelNextWord:
            cmdStr<<(wxT("MadEditCommand.DelNextWord"));
            break;
#if 0
        //case ecCutLine:
        //    cmdStr<<(wxT("MadEditCommand.CutLine"));
        //    break;
        //case ecDeleteLine:
        //    cmdStr<<(wxT("MadEditCommand.DeleteLine"));
        //    break;
        //case ecUndo:
        //    cmdStr<<(wxT("MadEditCommand.Undo"));
        //    break;
        //case ecRedo:
        //    cmdStr<<(wxT("MadEditCommand.Redo"));
        //    break;
        //case ecCut:
        //    cmdStr<<(wxT("MadEditCommand.Cut"));
        //    break;
        //case ecCopy:
            cmdStr<<(wxT("MadEditCommand.Copy"));
            break;
        case ecPaste:
            cmdStr<<(wxT("MadEditCommand.Paste"));
            break;
        case ecToggleInsertMode:
            cmdStr<<(wxT("MadEditCommand.ToggleInsertMode"));
            break;
        case ecTextMode:
            cmdStr<<(wxT("MadEditCommand.TextMode"));
            break;
        case ecColumnMode:
            cmdStr<<(wxT("MadEditCommand.ColumnMode"));
            break;
        case ecHexMode:
            cmdStr<<(wxT("MadEditCommand.HexMode"));
            break;
        case ecNoWrap:
            cmdStr<<(wxT("MadEditCommand.NoWrap"));
            break;
        case ecWrapByWindow:
            cmdStr<<(wxT("MadEditCommand.WrapByWindow"));
            break;
        case ecWrapByColumn:
            cmdStr<<(wxT("MadEditCommand.WrapByColumn"));
            break;
        case ecToggleWindow:
            cmdStr<<(wxT("MadEditCommand.ToggleWindow"));
            break;
        case ecIncreaseIndent:
            cmdStr<<(wxT("MadEditCommand.IncreaseIndent"));
            break;
        case ecDecreaseIndent:
            cmdStr<<(wxT("MadEditCommand.DecreaseIndent"));
            break;
        case ecComment:
            cmdStr<<(wxT("MadEditCommand.Comment"));
            break;
        case ecUncomment:
            cmdStr<<(wxT("MadEditCommand.Uncomment"));
            break;
        case ecToUpperCase:
            cmdStr<<(wxT("MadEditCommand.ToUpperCase"));
            break;
        case ecToLowerCase:
            cmdStr<<(wxT("MadEditCommand.ToLowerCase"));
            break;
        case ecInvertCase:
            cmdStr<<(wxT("MadEditCommand.InvertCase"));
            break;
        case ecToHalfWidth:
            cmdStr<<(wxT("MadEditCommand.ToHalfWidth"));
            break;
        case ecToFullWidth:
            cmdStr<<(wxT("MadEditCommand.ToFullWidth"));
            break;
        case ecInsertDateTime:
            cmdStr<<(wxT("MadEditCommand.InsertDateTime"));
            break;
        case ecMouseNotify:
            cmdStr<<(wxT("MadEditCommand.MouseNotify"));
            break;
#endif
        default:
            cmdStr<<(wxT("MadEditCommand.None)"));
            return false;
            break;
    }
    return true;
}

namespace mad_py = ::boost::python;
namespace mad_python
{
    class PyMadEdit
    {
    public:
        PyMadEdit()
        {
#ifndef PYMADEDIT_DLL        
            if(g_MainFrame) //Should be always true!!!
            {
                if(!g_ActiveMadEdit)
                {
                    // Simulate MadEditFrame::OnFileNew
                    g_MainFrame->OpenFile(wxEmptyString, false);
                    return;
                }
            }
            else
                throw std::runtime_error("No active main frame!");
#else
            if(!g_ActiveMadEdit)
            {
                g_ActiveMadEdit = new MadEdit( &g_DummyWin, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
                return;
            }
#endif
        }
        void ProcessCommand(int command)
        {
            g_ActiveMadEdit->ProcessCommand(command);
        }

        void InsertWChar(int key)
        {
            g_ActiveMadEdit->ProcessCommand(key);
        }
        void InsertStr(const std::string &str)
        {
            wxString wxStr(str.c_str(), wxConvLocal);
            
            ucs4string out;
            vector<ucs4_t> ucs;

            g_ActiveMadEdit->TranslateText(wxStr.c_str(), wxStr.Len(), &ucs, true);
            
            for(size_t i=0, size=ucs.size(); i<size; ++i)
            {
                out += ucs[i] ;
            }
            g_ActiveMadEdit->InsertString(out.c_str(), out.length(), false, true, false);
        }

        void InsertIncrementalNumber(int initial, int step, int total, int stepType,
                    int fmt, int align, bool zeroPad, const std::string & pref, const std::string & post)
        {
            wxString wxPrefix(pref.c_str(), wxConvLocal), wxPostfix(post.c_str(), wxConvLocal);
            g_ActiveMadEdit->InsertIncrementalNumber(initial, step, total, 
                    (MadNumberingStepType)stepType, (MadNumberFormat)fmt, (MadNumberAlign)align, zeroPad, wxPrefix, wxPostfix);
        }
        void ScrollLineUp()
        {
            g_ActiveMadEdit->ProcessCommand(ecScrollLineUp);
        }
        
        void ScrollLineDown()
        {
            g_ActiveMadEdit->ProcessCommand(ecScrollLineDown);
        }
        
        void ScrollPageUp()
        {
            g_ActiveMadEdit->ProcessCommand(ecScrollPageUp);
        }
        
        void ScrollPageDown()
        {
            g_ActiveMadEdit->ProcessCommand(ecScrollPageDown);
        }
        
        void ScrollLeft()
        {
            g_ActiveMadEdit->ProcessCommand(ecScrollLeft);
        }
        
        void ScrollRight()
        {
            g_ActiveMadEdit->ProcessCommand(ecScrollRight);
        }

        void GoToLine(int line)
        {
            g_ActiveMadEdit->GoToLine(line);
        }

        void GoToLineColumn(int line, int column)
        {
            g_ActiveMadEdit->GoToLine(line);
            for(int col = 0; col < (column-1); ++col) // no validate for input
                g_ActiveMadEdit->ProcessCommand(ecRight);
        }
        
        void SetSyntax(const std::string &title)
        {
            if(title.empty())
                return;
            wxString wxTitle(title.c_str(), wxConvLocal);
            g_ActiveMadEdit->SetSyntax(wxTitle);
        }
        const std::string GetSyntaxTitle()
        {
            wxString title = g_ActiveMadEdit->GetSyntaxTitle();
            return std::string(title.mb_str());
        }
        void LoadDefaultSyntaxScheme()
        {
            g_ActiveMadEdit->LoadDefaultSyntaxScheme();
        }
        void SetEncoding(const std::string &encname)
        {
            if(encname.empty())
                return;
            wxString wxEncname(encname.c_str(), wxConvLocal);
            g_ActiveMadEdit->SetEncoding(wxEncname);
        }
        const std::string GetEncodingName()
        {
            wxString desc = g_ActiveMadEdit->GetEncodingName();
            return std::string(desc.mb_str());
        }
        const std::string GetEncodingDescription()
        {
            wxString desc = g_ActiveMadEdit->GetEncodingDescription();
            return std::string(desc.mb_str());
        }
        int GetEncodingType()
        {
            return (int)g_ActiveMadEdit->GetEncodingType();
        }
        bool GetRecordCaretMovements()
        {
            return g_ActiveMadEdit->GetRecordCaretMovements();
        }
        void SetRecordCaretMovements(bool value)
        {
            g_ActiveMadEdit->SetRecordCaretMovements(value);
        }

        void SetTextFont(const std::string &name, int size, bool forceReset)
        {
            if(name.empty())
                return;
            wxString wxName(name.c_str(), wxConvLocal);
            g_ActiveMadEdit->SetTextFont(wxName, size, forceReset);
        }
        void SetHexFont(const std::string &name, int size, bool forceReset)
        {
            if(name.empty())
                return;
            wxString wxName(name.c_str(), wxConvLocal);
            g_ActiveMadEdit->SetHexFont(wxName, size, forceReset);
        }

        mad_py::tuple GetTextFont()
        {
            wxString name;
            int size;
            g_ActiveMadEdit->GetTextFont(name, size);
            return mad_py::make_tuple(std::string(name.mb_str()), size);
        }
        mad_py::tuple GetHexFont()
        {
            wxString name;
            int size;
            g_ActiveMadEdit->GetHexFont(name, size);
            return mad_py::make_tuple(std::string(name.mb_str()), size);
        }

        /*wxFont GetFont()
        {
            return g_ActiveMadEdit->GetFont();
        }*/

        mad_py::tuple GetFontNameSize()
        {
            wxString name;
            int size = 0;
            g_ActiveMadEdit->GetFont(name, size);
            return mad_py::make_tuple(std::string(name.mb_str()), size);
        }
        void SetFontA(const std::string &name, int size)
        {
            if(name.empty())
                return;
            wxString wxName(name.c_str(), wxConvLocal);
            g_ActiveMadEdit->SetFont(wxName, size);
        }

        /*bool SetFontB(const wxFont& font)
        {
            return g_ActiveMadEdit->SetFont(font);
        }*/

        void SetFixedWidthMode(bool mode)
        {
            g_ActiveMadEdit->SetFixedWidthMode(mode);
        }
        bool GetFixedWidthMode()
        {
            return g_ActiveMadEdit->GetFixedWidthMode();
        }

        void SetLineSpacing(int percent)
        {
            g_ActiveMadEdit->SetLineSpacing(percent);
        }
        long GetLineSpacing()
        {
            return g_ActiveMadEdit->GetLineSpacing();
        }

        void SetEditMode(int mode)
        {
            g_ActiveMadEdit->SetEditMode((MadEditMode)mode);
        }

        int GetEditMode()
        {
            return (int)g_ActiveMadEdit->GetEditMode();
        }

        void SetSingleLineMode(bool mode)
        {
            g_ActiveMadEdit->SetSingleLineMode(mode);
        }

        void SetTabColumns(long value)
        {
            g_ActiveMadEdit->SetTabColumns(value);
        }
        long GetTabColumns()
        {
            return g_ActiveMadEdit->GetTabColumns();
        }

        void SetIndentColumns(long value)
        {
            g_ActiveMadEdit->SetIndentColumns(value);
        }
        long GetIndentColumns()
        {
            return g_ActiveMadEdit->GetIndentColumns();
        }

        void SetInsertSpacesInsteadOfTab(bool value)
        {
            g_ActiveMadEdit->SetInsertSpacesInsteadOfTab(value);
        }
        bool GetInsertSpacesInsteadOfTab()
        {
            return g_ActiveMadEdit->GetInsertSpacesInsteadOfTab();
        }
        void SetWantTab(bool value)
        {
            g_ActiveMadEdit->SetWantTab(value);
        }
        bool GetWantTab()
        {
            return g_ActiveMadEdit->GetWantTab();
        }

        void SetWordWrapMode(int mode)
        {
            g_ActiveMadEdit->SetWordWrapMode((MadWordWrapMode)mode);
        }
        int GetWordWrapMode()
        {
            return (int)g_ActiveMadEdit->GetWordWrapMode();
        }

        void SetShowEndOfLine(bool value)
        {
            g_ActiveMadEdit->SetShowEndOfLine(value);
        }
        void SetShowTabChar(bool value)
        {
            g_ActiveMadEdit->SetShowTabChar(value);
        }
        void SetShowSpaceChar(bool value)
        {
            g_ActiveMadEdit->SetShowSpaceChar(value);
        }
        void SetMarkActiveLine(bool value)
        {
            g_ActiveMadEdit->SetMarkActiveLine(value);
        }
        void SetDisplayLineNumber(bool value)
        {
            g_ActiveMadEdit->SetDisplayLineNumber(value);
        }
        bool GetDisplayLineNumber()
        {
            return g_ActiveMadEdit->GetDisplayLineNumber();
        }
        void SetDisplayBookmark(bool value)
        {
            g_ActiveMadEdit->SetDisplayBookmark(value);
        }
        bool GetDisplayBookmark()
        {
            return g_ActiveMadEdit->GetDisplayBookmark();
        }
        bool GetShowEndOfLine()
        {
            return g_ActiveMadEdit->GetShowEndOfLine();
        }
        bool GetShowTabChar()
        {
            return g_ActiveMadEdit->GetShowTabChar();
        }
        bool GetShowSpaceChar()
        {
            return g_ActiveMadEdit->GetShowSpaceChar();
        }
        bool GetMarkActiveLine()
        {
            return g_ActiveMadEdit->GetMarkActiveLine();
        }

        void SetMarkBracePair(bool value)
        {
            g_ActiveMadEdit->SetMarkBracePair(value);
        }
        bool GetMarkBracePair()
        {
            return g_ActiveMadEdit->GetMarkBracePair();
        }

        long GetMaxColumns()
        {
            return g_ActiveMadEdit->GetMaxColumns();
        }
        void SetMaxColumns(long cols)
        {
            g_ActiveMadEdit->SetMaxColumns(cols);
        }

        bool GetAutoIndent()
        {
            return g_ActiveMadEdit->GetAutoIndent();
        }
        void SetAutoIndent(bool value)
        {
            g_ActiveMadEdit->SetAutoIndent(value);
        }

        void SetAutoCompletePair(bool value)
        {
            g_ActiveMadEdit->SetAutoCompletePair(value);
        }
        bool GetAutoCompletePair()
        {
            return g_ActiveMadEdit->GetAutoCompletePair();
        }

        void SetInsertMode(bool mode)  // true: insert, false: overwrite
        {
            g_ActiveMadEdit->SetInsertMode(mode);
        }
        bool GetInsertMode()
        {
            return g_ActiveMadEdit->GetInsertMode();
        }

        void SetCaretType(int type)
        {
            g_ActiveMadEdit->SetCaretType((MadCaretType)type);
        }

        bool GetMouseSelectToCopy()
        {
            return g_ActiveMadEdit->GetMouseSelectToCopy();
        }
        void SetMouseSelectToCopy(bool value)
        {
            g_ActiveMadEdit->SetMouseSelectToCopy(value);
        }
        bool GetMouseSelectToCopyWithCtrlKey()
        {
            return g_ActiveMadEdit->GetMouseSelectToCopyWithCtrlKey();
        }
        void SetMouseSelectToCopyWithCtrlKey(bool value)
        {
            g_ActiveMadEdit->SetMouseSelectToCopyWithCtrlKey(value);
        }

        bool GetMiddleMouseToPaste()
        {
            return g_ActiveMadEdit->GetMiddleMouseToPaste();
        }
        void SetMiddleMouseToPaste(bool value)
        {
            g_ActiveMadEdit->SetMiddleMouseToPaste(value);
        }

        int GetMaxWordWrapWidth()
        {
            return g_ActiveMadEdit->GetMaxWordWrapWidth();
        }
        int GetUCharWidth(int uc)
        {
            return g_ActiveMadEdit->GetUCharWidth((ucs4_t)uc);
        }
        int GetHexUCharWidth(int uc)
        {
            return g_ActiveMadEdit->GetHexUCharWidth((ucs4_t)uc);
        }
        int GetUCharType(int uc)
        {
            return g_ActiveMadEdit->GetUCharType((ucs4_t)uc);
        }

        // all are zero-based
        mad_py::tuple GetCaretPositionA()
        {
            int line = 0, subrow = 0;
            wxFileOffset column;
            g_ActiveMadEdit->GetCaretPosition(line, subrow, column);
            return mad_py::make_tuple(line, subrow, (int)column);
        }
        int GetCaretPositionB()
        {
            return (int)g_ActiveMadEdit->GetCaretPosition();
        }
        const std::string GetFileName()
        {
            wxString name = g_ActiveMadEdit->GetFileName();
            return std::string(name.mb_str());
        }
        int GetFileSize()
        {
            return (int)g_ActiveMadEdit->GetFileSize();
        }

        bool IsSelected()
        {
            return g_ActiveMadEdit->IsSelected();
        }
        int GetSelectionSize()
        {
            return (int)g_ActiveMadEdit->GetSelectionSize();
        }
        int GetSelectionBeginPos()
        {
            return (int)g_ActiveMadEdit->GetSelectionBeginPos();
        }
        int GetSelectionEndPos()
        {
            return (int)g_ActiveMadEdit->GetSelectionEndPos();
        }

        // return -1 for no selection
        mad_py::tuple GetSelectionLineId()
        {
            int beginline = -1, endline = -1;
            g_ActiveMadEdit->GetSelectionLineId(beginline, endline);
            return mad_py::make_tuple(beginline, endline);
        }
        void SetSelection(int beginpos, wxFileOffset endpos, bool bCaretAtBeginPos = false)
        {
            g_ActiveMadEdit->SetSelection((wxFileOffset)beginpos, (wxFileOffset)endpos, bCaretAtBeginPos);
        }

        int GetLineCount()
        {
            return g_ActiveMadEdit->GetLineCount();
        }

        void ConvertNewLineType(int type)
        {
            g_ActiveMadEdit->ConvertNewLineType((MadNewLineType)type);
        }
        void SetInsertNewLineType(int type)
        {
            g_ActiveMadEdit->SetInsertNewLineType((MadNewLineType)type);
        }

        int GetNewLineType()
        {
            return (int)g_ActiveMadEdit->GetNewLineType();
        }

        int GetInsertNewLineType()
        {
            return (int)g_ActiveMadEdit->GetInsertNewLineType();
        }

        bool IsModified()
        {
            return g_ActiveMadEdit->IsModified();
        }
        long GetModificationTime()
        {
            return (long)g_ActiveMadEdit->GetModificationTime();
        }

        void SetReadOnly(bool value)
        {
            g_ActiveMadEdit->SetReadOnly(value);
        }
        bool IsReadOnly()
        {
            return g_ActiveMadEdit->IsReadOnly();
        }
        bool IsTextFile()
        {
            return g_ActiveMadEdit->IsTextFile();
        }

        const std::string GetSelText()
        {
            wxString ws;
            g_ActiveMadEdit->GetSelText(ws);
            return std::string(ws.mb_str());
        }
        const std::string GetText(bool ignoreBOM = true)
        {
            wxString ws;
            g_ActiveMadEdit->GetText(ws, ignoreBOM);
            
            return std::string(ws.mb_str());
        }
        void SetText(const std::string &ws)
        {
            if(ws.empty())
                return;
            wxString wxWs(ws.c_str(), wxConvLocal);
            g_ActiveMadEdit->SetText(wxWs);
        }

        // line: zero based
        // return true for full line, false for partial line
        mad_py::tuple GetLine(int line, size_t maxlen = 0, bool ignoreBOM = true)
        {
            wxString wxWs;
            bool ret = g_ActiveMadEdit->GetLine(wxWs, line, maxlen, ignoreBOM);
            return mad_py::make_tuple(std::string(wxWs.mb_str()), ret);
        }
        int GetLineByPos(int pos)
        {
            return g_ActiveMadEdit->GetLineByPos(pos);
        }

        const std::string GetSelHexString(bool withSpace)
        {
            wxString wxWs;
            g_ActiveMadEdit->GetSelHexString(wxWs, withSpace);
            return std::string(wxWs.mb_str());
        }

        const std::string GetWordFromCaretPos()
        {
            wxString wxWs;
            g_ActiveMadEdit->GetWordFromCaretPos(wxWs);
            return std::string(wxWs.mb_str());
        }

        void Delete()
        {
            g_ActiveMadEdit->Delete();
        }
        void CutLine()
        {
            g_ActiveMadEdit->CutLine();
        }
        void DeleteLine()
        {
            g_ActiveMadEdit->DeleteLine();
        }
        void InsertTabChar()
        {
            g_ActiveMadEdit->InsertTabChar();
        }
        void InsertDateTime()
        {
            g_ActiveMadEdit->InsertDateTime();
        }

        void HighlightWords()
        {
            g_ActiveMadEdit->HighlightWords();
        }

        void SelectAll()
        {
            g_ActiveMadEdit->SelectAll();
        }
        void CutToClipboard()
        {
            g_ActiveMadEdit->CutToClipboard();
        }
        void CopyToClipboardA()
        {
            g_ActiveMadEdit->CopyToClipboard();
        }
        void PasteFromClipboard()
        {
            g_ActiveMadEdit->PasteFromClipboard();
        }

        void DndBegDrag()
        {
            g_ActiveMadEdit->DndBegDrag();
        }
        void DndDrop()
        {
            g_ActiveMadEdit->DndDrop();
        }
        bool CanPaste()
        {
            return g_ActiveMadEdit->CanPaste();
        }
        void CopyToClipboardB(const std::string &txt)
        {
            wxString text(txt.c_str(), wxConvLocal);
            g_ActiveMadEdit->CopyToClipboard(text);
        }

        bool CanUndo()
        {
            return g_ActiveMadEdit->CanUndo();
        }
        bool CanRedo()
        {
            return g_ActiveMadEdit->CanRedo();
        }
        void Undo()
        {
            g_ActiveMadEdit->Undo();
        }
        void Redo()
        {
            g_ActiveMadEdit->Redo();
        }

        void SetCaretPosition(int pos, int selbeg=-1, int selend=-1)
        {
            g_ActiveMadEdit->SetCaretPosition((wxFileOffset)pos, (wxFileOffset)selbeg, (wxFileOffset)selend);
        }

        bool HasBracePair()
        {
            return g_ActiveMadEdit->HasBracePair();
        }
        void GoToLeftBrace()
        {
            g_ActiveMadEdit->GoToLeftBrace();
        }
        void GoToRightBrace()
        {
            g_ActiveMadEdit->GoToRightBrace();
        }

        // search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
        int FindTextNext(const std::string &text,
                                bool bRegex, bool bCaseSensitive, bool bWholeWord,
                                int rangeFrom = -1, int rangeTo = -1)
        {
            if(text.empty())
                return SR_EXPR_ERROR;
            wxString wxText(text.c_str(), wxConvLocal);
            wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
            
            return g_ActiveMadEdit->FindTextNext(wxText, bRegex, bCaseSensitive, bWholeWord, from, to);
            
        }
        // search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
        int FindTextPrevious(const std::string &text,
                                    bool bRegex, bool bCaseSensitive, bool bWholeWord,
                                    int rangeFrom = -1, int rangeTo = -1)
        {
            if(text.empty())
                return SR_EXPR_ERROR;
            wxString wxText(text.c_str(), wxConvLocal);
            wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;
            
            return g_ActiveMadEdit->FindTextPrevious(wxText, bRegex, bCaseSensitive, bWholeWord, from, to);                
        }

        // search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
        int FindHexNext(const std::string &hexstr, int rangeFrom = -1, int rangeTo = -1)
        {
            if(hexstr.empty())
                return SR_EXPR_ERROR;
            wxString wxHexExpr(hexstr.c_str(), wxConvLocal);
            wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;

            return g_ActiveMadEdit->FindHexNext(wxHexExpr, from, to);
        }
        // search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
        int FindHexPrevious(const std::string &hexstr, int rangeFrom = -1, int rangeTo = -1)
        {
            if(hexstr.empty())
                return SR_EXPR_ERROR;
            wxString wxHexExpr(hexstr.c_str(), wxConvLocal);
            wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;

            return g_ActiveMadEdit->FindHexPrevious(wxHexExpr, from, to);
        }

        // replace the selected text that must match expr
        int ReplaceText(const std::string &expr, const std::string &fmt,
                                     bool bRegex, bool bCaseSensitive, bool bWholeWord,
                                     int rangeFrom = -1, int rangeTo = -1)
        {
            if(expr.empty())
                return RR_EXPR_ERROR;
            wxString wxExpr(expr.c_str(), wxConvLocal), wxFmt(fmt.c_str(), wxConvLocal);
            wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;

            return g_ActiveMadEdit->ReplaceText(wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, from, to);
        }
        int ReplaceHex(const std::string &expr, const std::string &fmt,
                                     int rangeFrom = -1, int rangeTo = -1)
        {
            if(expr.empty())
                return RR_EXPR_ERROR;
            wxString wxExpr(expr.c_str(), wxConvLocal), wxFmt(fmt.c_str(), wxConvLocal);
            wxFileOffset from = (wxFileOffset)rangeFrom, to = (wxFileOffset)rangeTo;

            return g_ActiveMadEdit->ReplaceHex(wxExpr, wxFmt, from, to);
        }

        // return the replaced count or SR_EXPR_ERROR
        int ReplaceTextAll(const std::string &expr, const std::string &fmt,
                            bool bRegex, bool bCaseSensitive, bool bWholeWord,
                            int rangeFrom = -1, int rangeTo = -1)
        {
            if(expr.empty())
                return 0;
            wxString wxExpr(expr.c_str(), wxConvLocal), wxFmt(fmt.c_str(), wxConvLocal);
            return g_ActiveMadEdit->ReplaceTextAll(wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, NULL, NULL, (wxFileOffset)rangeFrom, (wxFileOffset)rangeTo);
        }
        int ReplaceHexAll(const std::string &expr, const std::string &fmt,
                            int rangeFrom = -1, int rangeTo = -1)
        {
            if(expr.empty())
                return 0;
            wxString wxExpr(expr.c_str(), wxConvLocal), wxFmt(fmt.c_str(), wxConvLocal);
            return g_ActiveMadEdit->ReplaceHexAll(wxExpr, wxFmt, NULL, NULL, (wxFileOffset)rangeFrom, (wxFileOffset)rangeTo);
        }

#ifndef PYMADEDIT_DLL
        // list the matched data to pbegpos & pendpos
        // return the found count or SR_EXPR_ERROR
        int FindTextAll(const std::string &expr, bool bRegex, bool bCaseSensitive, bool bWholeWord, bool showresults = true)
        {
            int ResultCount=0;
            if(expr.empty())
                return -2;
            wxString wxExpr(expr.c_str(), wxConvLocal), fmt;
            vector<wxFileOffset> begpos, endpos;
            MadEdit *madedit=g_ActiveMadEdit;
            
            wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;

            int ok = madedit->FindTextAll(wxExpr, bRegex, bCaseSensitive, bWholeWord, false, &begpos, &endpos);

            if(ok>=0 && showresults)
            {
                static wxString text(_("Search Results"));
                int pid = g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
                g_MainFrame->m_InfoNotebook->SetPageText(pid, text);
                DisplayFindAllResult(begpos, endpos, madedit);
            }
            
            return ok;
        }
        int FindHexAll(const std::string &expr, bool showresults = true)
        {
            int ResultCount=0;
            if(expr.empty())
                return -2;
            wxString wxExpr(expr.c_str(), wxConvLocal), fmt;
            vector<wxFileOffset> begpos, endpos;
            MadEdit *madedit=g_ActiveMadEdit;
            
            wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
         
            int ok = madedit->FindHexAll(wxExpr, false, &begpos, &endpos);

            if(ok>=0 && showresults)
            {
                static wxString text(_("Search Results"));
                int pid = g_MainFrame->m_InfoNotebook->GetPageIndex(g_MainFrame->m_FindInFilesResults);
                g_MainFrame->m_InfoNotebook->SetPageText(pid, text);
                DisplayFindAllResult(begpos, endpos, madedit);
            }

            return ok;
        }
#endif        
        bool LoadFromFile(const std::string &filename, const std::string &encoding=std::string(""))
        {
            if(filename.empty())
                return false;
            wxString wxEncoding(encoding.c_str(), wxConvLocal);
            wxString wxFilename(filename.c_str(), wxConvLocal);
            return g_ActiveMadEdit->LoadFromFile(wxFilename, wxEncoding);
        }
        bool SaveToFile(const std::string &filename)
        {
            if(filename.empty())
                return false;
            wxString wxFilename(filename.c_str(), wxConvLocal);
            return g_ActiveMadEdit->SaveToFile(wxFilename);
        }
        bool Reload()
        {
            return g_ActiveMadEdit->Reload();
        }
        // if the file is modified by another app, reload it.
        bool ReloadByModificationTime()
        {
            return g_ActiveMadEdit->ReloadByModificationTime(false);
        }
        // restore pos in Reload(), ConvertEncoding()
        void RestorePosition(int pos, int toprow)
        {
            wxFileOffset wxPos = pos;
            g_ActiveMadEdit->RestorePosition(wxPos, toprow);
        }

        // write back to the original FileName
        // if FileName is empty, ask the user to get filename
        // return wxID_YES(Saved), wxID_NO(Not Saved), or wxID_CANCEL
        int Save(bool ask, const std::string &title, bool saveas)
        {
            if(title.empty())
                return wxID_NO;
            wxString wxTitle(title.c_str(), wxConvLocal);
            return g_ActiveMadEdit->Save(ask, wxTitle, saveas);
        }

        // add: gogo, 21.09.2009
        void SetBookmark()
        {
            g_ActiveMadEdit->SetBookmark();
        }
        void GotoNextBookmark()
        {
            g_ActiveMadEdit->GotoNextBookmark();
        }
        void GotoPreviousBookmark()
        {
            g_ActiveMadEdit->GotoPreviousBookmark();
        }
        //----------
        void ConvertEncoding(const std::string &newenc, int flag)
        {
            if(newenc.empty())
                return;
            MadConvertEncodingFlag mflag = (MadConvertEncodingFlag)flag;
            wxString wxNewenc(newenc.c_str(), wxConvLocal);
            g_ActiveMadEdit->ConvertEncoding(wxNewenc, mflag);
        }
        void ConvertChineseA(int flag)
        {
            g_ActiveMadEdit->ConvertChinese((MadConvertEncodingFlag)flag);
        }

        bool HasBOM()
        {
            return g_ActiveMadEdit->HasBOM();
        }
        void ToggleBOM()
        {
            g_ActiveMadEdit->ToggleBOM();
        }

        void IncreaseDecreaseIndent(bool incIndent)
        {
            g_ActiveMadEdit->IncreaseDecreaseIndent(incIndent);
        }
        bool HasLineComment()
        {
            return g_ActiveMadEdit->HasLineComment();
        }
        void CommentUncomment(bool comment)
        {
            g_ActiveMadEdit->CommentUncomment(comment);
        }

        void ToUpperCase()
        {
            g_ActiveMadEdit->ToUpperCase();
        }
        void ToLowerCase()
        {
            g_ActiveMadEdit->ToLowerCase();
        }
        void InvertCase()
        {
            g_ActiveMadEdit->InvertCase();
        }
        void Capitalize()
        {
            g_ActiveMadEdit->Capitalize();
        }
        void ToHalfWidth(bool ascii=true, bool japanese=true, bool korean=true, bool other=true)
        {
            g_ActiveMadEdit->ToHalfWidth(ascii, japanese, korean, other);
        }
        void ToFullWidth(bool ascii=true, bool japanese=true, bool korean=true, bool other=true)
        {
            g_ActiveMadEdit->ToFullWidth(ascii, japanese, korean, other);
        }

        void TrimTrailingSpaces()
        {
            g_ActiveMadEdit->TrimTrailingSpaces();
        }

        void DeleteEmptyLines()
        {
            g_ActiveMadEdit->DeleteEmptyLines();
        }

        
        void DeleteEmptyLinesWithSpaces()
        {
            g_ActiveMadEdit->DeleteEmptyLinesWithSpaces();
        }

        void JoinLines()
        {
            g_ActiveMadEdit->JoinLines();
        }

        // startline<0 : sort all lines otherwise sort [beginline, endline]
        void SortLines(MadSortFlags flags, int beginline, int endline)
        {
            g_ActiveMadEdit->SortLines(flags, beginline, endline);
        }

        // convert WordWraps to NewLine-chars in the SelText or whole file
        void ConvertWordWrapToNewLine()
        {
            g_ActiveMadEdit->ConvertWordWrapToNewLine();
        }
        // convert NewLine-chars to WordWraps in the SelText
        void ConvertNewLineToWordWrap()
        {
            g_ActiveMadEdit->ConvertNewLineToWordWrap();
        }

        void ConvertSpaceToTab()
        {
            g_ActiveMadEdit->ConvertSpaceToTab();
        }
        void ConvertTabToSpace()
        {
            g_ActiveMadEdit->ConvertTabToSpace();
        }

        void CopyAsHexString(bool withSpace)
        {
            g_ActiveMadEdit->CopyAsHexString(withSpace);
        }

        void CopyRevertHex(const std::string &delimiters)
        {
            wxString wxDelimiters(delimiters.c_str(), wxConvLocal);
            g_ActiveMadEdit->CopyRevertHex(wxDelimiters);
        }

        mad_py::tuple WordCount(bool selection)
        {
            int words = 0, chars = 0, spaces = 0, lines = 0, halfwidths = 0, fullwidths = 0;
            wxArrayString detail;
            g_ActiveMadEdit->WordCount(selection, words, chars, spaces, halfwidths, fullwidths, lines, &detail);
            wxString str;
            for(size_t i=0;i<detail.Count();++i)
            {
                str<<detail[i]<<wxT("\n");
            }

            return mad_py::make_tuple(words, chars, spaces, lines, halfwidths, fullwidths, std::string(str.mb_str()));
        }

        void ColumnAlign()
        {
            g_ActiveMadEdit->ColumnAlign();
        }

    };
    //PyMadEdit * InitMadPython() { return new PyMadEdit();}
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindTextNext_member_overloads, FindTextNext, 4, 6)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindTextPrevious_member_overloads, FindTextPrevious, 4, 6)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindHexNext_member_overloads, FindHexNext, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindHexPrevious_member_overloads, FindHexPrevious, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindTextAll_member_overloads, FindTextAll, 4, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FindHexAll_member_overloads, FindHexAll, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceText_member_overloads, ReplaceText, 5, 7)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceHex_member_overloads, ReplaceHex, 2, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceTextAll_member_overloads, ReplaceTextAll, 5, 7)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ReplaceHexAll_member_overloads, ReplaceHexAll, 2, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(LoadFromFile_member_overloads, LoadFromFile, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ToHalfWidth_member_overloads, ToHalfWidth, 0, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ToFullWidth_member_overloads, ToFullWidth, 0, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetCaretPosition_member_overloads, SetCaretPosition, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GetLine_member_overloads, GetLine, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GetText_member_overloads, GetText, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetSelection_member_overloads, SetSelection, 2, 3) 


BOOST_PYTHON_MODULE(madpython)
{
    using namespace mad_python;
    using namespace mad_py;

    class_<PyMadEdit>("MadEdit", "This class is a collection of wrapper functions of MadEdit.", init<>())
        .def("ProcessCommand", &PyMadEdit::ProcessCommand, "")
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
        .def("SetRecordCaretMovements", &PyMadEdit::SetRecordCaretMovements, return_value_policy<return_by_value>(), "")
        .def("SetTextFont", &PyMadEdit::SetTextFont, "")
        .def("SetHexFont", &PyMadEdit::SetHexFont, "")
        .def("GetTextFont", &PyMadEdit::GetTextFont, "")
        .def("GetHexFont", &PyMadEdit::GetHexFont, "")
        .def("GetFontNameSize", &PyMadEdit::GetFontNameSize, return_value_policy<return_by_value>(), "Doc")
        .def("SetFixedWidthMode", &PyMadEdit::SetFixedWidthMode, "")
        .def("GetFixedWidthMode", &PyMadEdit::GetFixedWidthMode, "")
        .def("SetLineSpacing", &PyMadEdit::SetLineSpacing, "")
        .def("GetLineSpacing", &PyMadEdit::GetLineSpacing, "")
        .def("SetEditMode", &PyMadEdit::SetEditMode, "")
        .def("GetEditMode", &PyMadEdit::GetEditMode, return_value_policy<return_by_value>(), "")
        .def("SetSingleLineMode", &PyMadEdit::SetSingleLineMode, "")
        .def("SetTabColumns", &PyMadEdit::SetTabColumns, "")
        .def("GetTabColumns", &PyMadEdit::GetTabColumns, "")
        .def("SetIndentColumns", &PyMadEdit::SetIndentColumns, "")
        .def("GetIndentColumns", &PyMadEdit::GetIndentColumns, "")
        .def("SetInsertSpacesInsteadOfTab", &PyMadEdit::SetInsertSpacesInsteadOfTab, "")
        .def("GetInsertSpacesInsteadOfTab", &PyMadEdit::GetInsertSpacesInsteadOfTab, "")
        .def("SetWantTab", &PyMadEdit::SetWantTab, "")
        .def("GetWantTab", &PyMadEdit::GetWantTab, "")
        .def("SetWordWrapMode", &PyMadEdit::SetWordWrapMode, "")
        .def("GetWordWrapMode", &PyMadEdit::GetWordWrapMode, "")
        .def("SetDisplayLineNumber", &PyMadEdit::SetDisplayLineNumber, "")
        .def("SetShowEndOfLine", &PyMadEdit::SetShowEndOfLine, "")
        .def("SetShowTabChar", &PyMadEdit::SetShowTabChar, "")
        .def("SetShowSpaceChar", &PyMadEdit::SetShowSpaceChar, "")
        .def("SetMarkActiveLine", &PyMadEdit::SetMarkActiveLine, "")
        .def("GetDisplayLineNumber", &PyMadEdit::GetDisplayLineNumber, "")
        .def("GetShowEndOfLine", &PyMadEdit::GetShowEndOfLine, "")
        .def("GetShowTabChar", &PyMadEdit::GetShowTabChar, "")
        .def("GetShowSpaceChar", &PyMadEdit::GetShowSpaceChar, "")
        .def("GetMarkActiveLine", &PyMadEdit::GetMarkActiveLine, "")
        .def("SetMarkBracePair", &PyMadEdit::SetMarkBracePair, "")
        .def("GetMarkBracePair", &PyMadEdit::GetMarkBracePair, "")
        .def("GetMaxColumns", &PyMadEdit::GetMaxColumns, "")
        .def("SetMaxColumns", &PyMadEdit::SetMaxColumns, "")
        .def("GetAutoIndent", &PyMadEdit::GetAutoIndent, "")
        .def("SetAutoIndent", &PyMadEdit::SetAutoIndent, "")
        .def("SetAutoCompletePair", &PyMadEdit::SetAutoCompletePair, "")
        .def("GetAutoCompletePair", &PyMadEdit::GetAutoCompletePair, "")
        .def("SetInsertMode", &PyMadEdit::SetInsertMode, "")
        .def("GetInsertMode", &PyMadEdit::GetInsertMode, "")

        .def("SetCaretType", &PyMadEdit::SetCaretType, "")
        .def("GetMouseSelectToCopy", &PyMadEdit::GetMouseSelectToCopy, "")
        .def("SetMouseSelectToCopy", &PyMadEdit::SetMouseSelectToCopy, "")
        .def("GetMouseSelectToCopyWithCtrlKey", &PyMadEdit::GetMouseSelectToCopyWithCtrlKey, "")
        .def("SetMouseSelectToCopyWithCtrlKey", &PyMadEdit::SetMouseSelectToCopyWithCtrlKey, "")
        .def("GetMiddleMouseToPaste", &PyMadEdit::GetMiddleMouseToPaste, "")
        .def("SetMiddleMouseToPaste", &PyMadEdit::SetMiddleMouseToPaste, "")
        .def("GetMaxWordWrapWidth", &PyMadEdit::GetMaxWordWrapWidth, "")
        .def("GetUCharWidth", &PyMadEdit::GetUCharWidth, "")
        .def("GetHexUCharWidth", &PyMadEdit::GetHexUCharWidth, "")
        .def("GetUCharType", &PyMadEdit::GetUCharType, "")

        .def("GetCaretPositionA", &PyMadEdit::GetCaretPositionA, return_value_policy<return_by_value>(), "")
        .def("GetCaretPositionB", &PyMadEdit::GetCaretPositionB, return_value_policy<return_by_value>(), "")
        .def("GetFileName", &PyMadEdit::GetFileName, return_value_policy<return_by_value>(), "")
        .def("GetFileSize", &PyMadEdit::GetFileSize, return_value_policy<return_by_value>(), "")
        .def("IsSelected", &PyMadEdit::IsSelected, "")
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
        .def("IsReadOnly", &PyMadEdit::IsReadOnly, "")
        .def("IsTextFile", &PyMadEdit::IsTextFile, "")
        .def("GetSelText", &PyMadEdit::GetSelText, return_value_policy<return_by_value>(), "")
        .def("SetText", &PyMadEdit::SetText, "")
        .def("GetLineByPos", &PyMadEdit::GetLineByPos, "")
        .def("GetSelHexString", &PyMadEdit::GetSelHexString, "")
        .def("GetWordFromCaretPos", &PyMadEdit::GetWordFromCaretPos, return_value_policy<return_by_value>(), "")

        .def("Delete", &PyMadEdit::Delete, "")
        .def("CutLine", &PyMadEdit::CutLine, "")
        .def("DeleteLine", &PyMadEdit::DeleteLine, "")
        .def("InsertTabChar", &PyMadEdit::InsertTabChar, "")
        .def("InsertDateTime", &PyMadEdit::InsertDateTime, "")
        .def("HighlightWords", &PyMadEdit::HighlightWords, "")
        .def("SelectAll", &PyMadEdit::SelectAll, "")

        .def("CutToClipboard", &PyMadEdit::CutToClipboard, "")

        .def("PasteFromClipboard", &PyMadEdit::PasteFromClipboard, "")
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
        .def("SaveToFile", &PyMadEdit::SaveToFile, "")
        .def("Reload", &PyMadEdit::Reload, "")
        .def("ReloadByModificationTime", &PyMadEdit::ReloadByModificationTime, "")
        .def("RestorePosition", &PyMadEdit::RestorePosition, "")
        .def("Save", &PyMadEdit::Save, "")
        .def("SetBookmark", &PyMadEdit::SetBookmark, "")
        .def("GotoNextBookmark", &PyMadEdit::GotoNextBookmark, "")
        .def("GotoPreviousBookmark", &PyMadEdit::GotoPreviousBookmark, "")
        .def("ConvertEncoding", &PyMadEdit::ConvertEncoding, "")
        .def("ConvertChineseA", &PyMadEdit::ConvertChineseA, "")
        .def("HasBOM", &PyMadEdit::HasBOM, "")
        .def("ToggleBOM", &PyMadEdit::ToggleBOM, "")
        .def("IncreaseDecreaseIndent", &PyMadEdit::IncreaseDecreaseIndent, "")
        .def("HasLineComment", &PyMadEdit::HasLineComment, "")
        .def("CommentUncomment", &PyMadEdit::CommentUncomment, "")
        .def("ToUpperCase", &PyMadEdit::ToUpperCase, "")
        .def("ToLowerCase", &PyMadEdit::ToLowerCase, "")

        .def("InvertCase", &PyMadEdit::InvertCase, "")
        .def("Capitalize", &PyMadEdit::Capitalize, "")
        .def("TrimTrailingSpaces", &PyMadEdit::TrimTrailingSpaces, "")
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
        .def("SetFontA", &PyMadEdit::SetFontA, "Doc")
        .def("CopyToClipboardA", &PyMadEdit::CopyToClipboardA, "")
        .def("CopyToClipboardB", &PyMadEdit::CopyToClipboardB, "")
        .def("ScrollLineUp", &PyMadEdit::ScrollLineUp, "")
        .def("ScrollLineDown", &PyMadEdit::ScrollLineDown, "")
        .def("ScrollPageUp", &PyMadEdit::ScrollPageUp, "")
        .def("ScrollPageDown", &PyMadEdit::ScrollPageDown, "")
        .def("ScrollLeft", &PyMadEdit::ScrollLeft, "")
        .def("ScrollRight", &PyMadEdit::ScrollRight, "")
        .def("ColumnAlign", &PyMadEdit::ColumnAlign, "")

        .def("FindTextNext", &PyMadEdit::FindTextNext, FindTextNext_member_overloads( args("text", "bRegex", "bCaseSensitive", "bWholeWord", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])

        .def("FindTextPrevious", &PyMadEdit::FindTextPrevious, FindTextPrevious_member_overloads( args("text", "bRegex", "bCaseSensitive", "bWholeWord", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("FindHexNext", &PyMadEdit::FindHexNext, FindHexNext_member_overloads( args("hexstr", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("FindHexPrevious", &PyMadEdit::FindHexPrevious, FindHexPrevious_member_overloads( args("hexstr", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("ReplaceText", &PyMadEdit::ReplaceText, ReplaceText_member_overloads( args("expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("ReplaceHex", &PyMadEdit::ReplaceHex, ReplaceHex_member_overloads( args("expr", "fmt"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("ReplaceTextAll", &PyMadEdit::ReplaceTextAll, ReplaceTextAll_member_overloads( args("expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("ReplaceHexAll", &PyMadEdit::ReplaceHexAll, ReplaceHexAll_member_overloads( args("expr", "fmt", "rangeFrom", "rangeTo"), "Doc string" )[return_value_policy<return_by_value>()])
#ifndef PYMADEDIT_DLL
        .def("FindTextAll", &PyMadEdit::FindTextAll, FindTextAll_member_overloads( args("expr", "bRegex", "bCaseSensitive", "bWholeWord", "showresults"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("FindHexAll", &PyMadEdit::FindHexAll, FindHexAll_member_overloads( args("expr", "showresults"), "Doc string" )[return_value_policy<return_by_value>()])
#endif
        .def("LoadFromFile", &PyMadEdit::LoadFromFile, LoadFromFile_member_overloads( args("filename", "encoding"), "Doc string" )[return_value_policy<return_by_value>()])

        .def("ToHalfWidth", &PyMadEdit::ToHalfWidth, ToHalfWidth_member_overloads( args("ascii", "japanese", "korean", "other"), "Doc string" ))
        .def("ToFullWidth", &PyMadEdit::ToFullWidth, ToFullWidth_member_overloads( args("ascii", "japanese", "korean", "other"), "Doc string" ))
        .def("SetCaretPosition", &PyMadEdit::SetCaretPosition, SetCaretPosition_member_overloads( args("pos", "selbeg", "selend"), "Doc string" ))
        .def("GetLine", &PyMadEdit::GetLine, GetLine_member_overloads( args("line", "maxlen", "ignoreBOM"), "Doc string" )[return_value_policy<return_by_value>()])
        .def("SetSelection", &PyMadEdit::SetSelection, SetSelection_member_overloads( args("beginpos", "endpos", "bCaretAtBeginPos"), "Doc string" ))
        .def("GetText", &PyMadEdit::GetText, GetText_member_overloads( args("ignoreBOM"), "Doc string" )[return_value_policy<return_by_value>()])
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
    enum_<MadWordWrapMode>("MadWordWrapMode")
        .value("NoWrap", wwmNoWrap)
        .value("WrapByWindow", wwmWrapByWindow)
        .value("WrapByColumn", wwmWrapByColumn)
        ;

    enum_<MadEditMode>("MadEditMode")
        .value("TextMode", emTextMode)
        .value("ColumnMode", emColumnMode)
        .value("HexMode", emHexMode)
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
        .value("SC2TC", cefSC2TC)// Simplified Chinese  ==> Traditional Chinese
        .value("TC2SC", cefTC2SC)// Traditional Chinese ==> Simplified Chinese
        .value("JK2TC", cefJK2TC)// Japanese Kanji      ==> Traditional Chinese
        .value("JK2SC", cefJK2SC)// Japanese Kanji      ==> Simplified Chinese
        .value("C2JK", cefC2JK)  // Trad&Simp Chinese   ==> Japanese Kanji
        ;
    enum_<MadNumberingStepType>("MadNumberingStepType")
        .value("Linear", nstLinear)
        .value("Exponential", nstExponential)
        ;

    enum_<MadNumberFormat>("MadNumberFormat")
        .value("DEC", nfDEC)
        .value("HEX", nfHEX)
        .value("BIN", nfBIN)
        .value("OCT", nfOCT)
        ;

    enum_<MadNumberAlign>("MadNumberAlign")
        .value("Left", naLeft)
        .value("Right",naRight)
        ;
}
#endif //__MADPYTHON__
