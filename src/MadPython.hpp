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
	extern MadEdit *g_CurrentMadEdit;
#else
	MadEdit *g_ActiveMadEdit = nullptr;
	MadEdit *g_CurrentMadEdit = nullptr;
	wxFrame g_DummyWin;
#endif
extern void DisplayFindAllResult( wxTreeItemId &myroot, vector<wxFileOffset> &begpos, vector<wxFileOffset> &endpos, MadEdit *madedit, bool expandresults = true, OnProgressUpdatePtr updater = nullptr );
extern int MadMessageBox( const wxString& message,
						  const wxString& caption = wxMessageBoxCaptionStr,
						  long style = wxOK | wxCENTRE,
						  wxWindow *parent = nullptr,
						  int x = wxDefaultCoord, int y = wxDefaultCoord );


// Ugly bigger switch than bigger map
bool FromCmdToString( wxString &cmdStr, long madCmd ) {
	switch( madCmd ) {
	case ecCharFirst:
		cmdStr << ( wxT( "MadEditCommand.CharFirst" ) );
		break;

	case ecCharLast:
		cmdStr << ( wxT( "MadEditCommand.CharLast" ) );
		break;

	case ecCaretCommandFirst:
		cmdStr << ( wxT( "MadEditCommand.CaretCommandFirst" ) );
		break;

	case ecLeft:
		cmdStr << ( wxT( "MadEditCommand.Left" ) );
		break;

	case ecRight:
		cmdStr << ( wxT( "MadEditCommand.Right" ) );
		break;

	case ecUp:
		cmdStr << ( wxT( "MadEditCommand.Up" ) );
		break;

	case ecDown:
		cmdStr << ( wxT( "MadEditCommand.Down" ) );
		break;

	case ecBeginLine:
		cmdStr << ( wxT( "MadEditCommand.BeginLine" ) );
		break;

	case ecEndLine:
		cmdStr << ( wxT( "MadEditCommand.EndLine" ) );
		break;

	case ecBeginDoc:
		cmdStr << ( wxT( "MadEditCommand.BeginDoc" ) );
		break;

	case ecEndDoc:
		cmdStr << ( wxT( "MadEditCommand.EndDoc" ) );
		break;

	case ecPrevPage:
		cmdStr << ( wxT( "MadEditCommand.PrevPage" ) );
		break;

	case ecNextPage:
		cmdStr << ( wxT( "MadEditCommand.NextPage" ) );
		break;

	case ecPrevWord:
		cmdStr << ( wxT( "MadEditCommand.PrevWord" ) );
		break;

	case ecNextWord:
		cmdStr << ( wxT( "MadEditCommand.NextWord" ) );
		break;

	case ecLeftBrace:
		cmdStr << ( wxT( "MadEditCommand.LeftBrace" ) );
		break;

	case ecRightBrace:
		cmdStr << ( wxT( "MadEditCommand.RightBrace" ) );
		break;

	case ecCaretCommandLast:
		cmdStr << ( wxT( "MadEditCommand.CaretCommandLast" ) );
		break;

	case ecSelCommandFirst:
		cmdStr << ( wxT( "MadEditCommand.SelCommandFirst" ) );
		break;

	case ecSelLeft:
		cmdStr << ( wxT( "MadEditCommand.SelLeft" ) );
		break;

	case ecSelRight:
		cmdStr << ( wxT( "MadEditCommand.SelRight" ) );
		break;

	case ecSelUp:
		cmdStr << ( wxT( "MadEditCommand.SelUp" ) );
		break;

	case ecSelDown:
		cmdStr << ( wxT( "MadEditCommand.SelDown" ) );
		break;

	case ecSelBeginLine:
		cmdStr << ( wxT( "MadEditCommand.SelBeginLine" ) );
		break;

	case ecSelEndLine:
		cmdStr << ( wxT( "MadEditCommand.SelEndLine" ) );
		break;

	case ecSelBeginDoc:
		cmdStr << ( wxT( "MadEditCommand.SelBeginDoc" ) );
		break;

	case ecSelEndDoc:
		cmdStr << ( wxT( "MadEditCommand.SelEndDoc" ) );
		break;

	case ecSelPrevPage:
		cmdStr << ( wxT( "MadEditCommand.SelPrevPage" ) );
		break;

	case ecSelNextPage:
		cmdStr << ( wxT( "MadEditCommand.SelNextPage" ) );
		break;

	case ecSelPrevWord:
		cmdStr << ( wxT( "MadEditCommand.SelPrevWord" ) );
		break;

	case ecSelNextWord:
		cmdStr << ( wxT( "MadEditCommand.SelNextWord" ) );
		break;

	case ecSelLeftBrace:
		cmdStr << ( wxT( "MadEditCommand.SelLeftBrace" ) );
		break;

	case ecSelRightBrace:
		cmdStr << ( wxT( "MadEditCommand.SelRightBrace" ) );
		break;

	case ecSelCommandLast:
		cmdStr << ( wxT( "MadEditCommand.SelCommandLast" ) );
		break;

	case ecHighlightWords:
		cmdStr << ( wxT( "MadEditCommand.HighlightWords" ) );
		break;

	case ecSelectAll:
		cmdStr << ( wxT( "MadEditCommand.SelectAll" ) );
		break;

	case ecScrollLineUp:
		cmdStr << ( wxT( "MadEditCommand.ScrollLineUp" ) );
		break;

	case ecScrollLineDown:
		cmdStr << ( wxT( "MadEditCommand.ScrollLineDown" ) );
		break;

	case ecScrollPageUp:
		cmdStr << ( wxT( "MadEditCommand.ScrollPageUp" ) );
		break;

	case ecScrollPageDown:
		cmdStr << ( wxT( "MadEditCommand.ScrollPageDown" ) );
		break;

	case ecScrollLeft:
		cmdStr << ( wxT( "MadEditCommand.ScrollLeft" ) );
		break;

	case ecScrollRight:
		cmdStr << ( wxT( "MadEditCommand.ScrollRight" ) );
		break;

	case ecReturn:
		cmdStr << ( wxT( "MadEditCommand.Return" ) );
		break;

	case ecReturnNoIndent:
		cmdStr << ( wxT( "MadEditCommand.ReturnNoIndent" ) );
		break;

	case ecTab:
		cmdStr << ( wxT( "MadEditCommand.Tab" ) );
		break;

	case ecInsertTabChar:
		cmdStr << ( wxT( "MadEditCommand.InsertTabChar" ) );
		break;

	case ecDelete:
		cmdStr << ( wxT( "MadEditCommand.Delete" ) );
		break;

	case ecBackSpace:
		cmdStr << ( wxT( "MadEditCommand.BackSpace" ) );
		break;

	case ecDelPrevWord:
		cmdStr << ( wxT( "MadEditCommand.DelPrevWord" ) );
		break;

	case ecDelNextWord:
		cmdStr << ( wxT( "MadEditCommand.DelNextWord" ) );
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
		cmdStr << ( wxT( "MadEditCommand.Copy" ) );
		break;

	case ecPaste:
		cmdStr << ( wxT( "MadEditCommand.Paste" ) );
		break;

	case ecToggleInsertMode:
		cmdStr << ( wxT( "MadEditCommand.ToggleInsertMode" ) );
		break;

	case ecTextMode:
		cmdStr << ( wxT( "MadEditCommand.TextMode" ) );
		break;

	case ecColumnMode:
		cmdStr << ( wxT( "MadEditCommand.ColumnMode" ) );
		break;

	case ecHexMode:
		cmdStr << ( wxT( "MadEditCommand.HexMode" ) );
		break;

	case ecNoWrap:
		cmdStr << ( wxT( "MadEditCommand.NoWrap" ) );
		break;

	case ecWrapByWindow:
		cmdStr << ( wxT( "MadEditCommand.WrapByWindow" ) );
		break;

	case ecWrapByColumn:
		cmdStr << ( wxT( "MadEditCommand.WrapByColumn" ) );
		break;

	case ecToggleWindow:
		cmdStr << ( wxT( "MadEditCommand.ToggleWindow" ) );
		break;

	case ecIncreaseIndent:
		cmdStr << ( wxT( "MadEditCommand.IncreaseIndent" ) );
		break;

	case ecDecreaseIndent:
		cmdStr << ( wxT( "MadEditCommand.DecreaseIndent" ) );
		break;

	case ecComment:
		cmdStr << ( wxT( "MadEditCommand.Comment" ) );
		break;

	case ecUncomment:
		cmdStr << ( wxT( "MadEditCommand.Uncomment" ) );
		break;

	case ecToUpperCase:
		cmdStr << ( wxT( "MadEditCommand.ToUpperCase" ) );
		break;

	case ecToLowerCase:
		cmdStr << ( wxT( "MadEditCommand.ToLowerCase" ) );
		break;

	case ecInvertCase:
		cmdStr << ( wxT( "MadEditCommand.InvertCase" ) );
		break;

	case ecToHalfWidth:
		cmdStr << ( wxT( "MadEditCommand.ToHalfWidth" ) );
		break;

	case ecToFullWidth:
		cmdStr << ( wxT( "MadEditCommand.ToFullWidth" ) );
		break;

	case ecInsertDateTime:
		cmdStr << ( wxT( "MadEditCommand.InsertDateTime" ) );
		break;

	case ecMouseNotify:
		cmdStr << ( wxT( "MadEditCommand.MouseNotify" ) );
		break;
#endif

	default:
		cmdStr << ( wxT( "MadEditCommand.None)" ) );
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
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( !madedit ) {
				// Simulate MadEditFrame::OnFileNew
				std::cout << "WARNING: No active MadEdit!!!" << std::endl;
			}
			else if (madedit->IsReadOnly() ) {
				std::cout << "WARNING: Current file is ReadOnly!!!" << std::endl;
			}
			if(!g_MainFrame)
			{ 
				std::cout << "WARNING: No main frame!!!" << std::endl;
			}

#else

			if( !madedit ) {
				madedit = new MadEdit( &g_DummyWin, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
				return;
			}

#endif
		}

		void ProcessCommand( long command ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				if( madedit->IsReadOnly() ) {
					if( ( command >= ecCharFirst && command <= ecCharLast )
							|| ( command >= ecReturn && command <= ecCut )
							|| ( command == ecPaste )
							|| ( command >= ecIncreaseIndent && command <= ecInsertDateTime ) )
					{ return; }
				}

				madedit->ProcessCommand( command );
			}
		}

		void InsertWChar( long key ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ProcessCommand( key ); }
		}

		void InsertStr( const std::string &str ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxString wxStr( str.c_str(), wxConvUTF8 );
				ucs4string out;
				vector<ucs4_t> ucs;
				madedit->TranslateText( wxStr.c_str(), wxStr.Len(), &ucs, true );

				for( size_t i = 0, size = ucs.size(); i < size; ++i ) {
					out += ucs[i] ;
				}

				madedit->InsertString( out.c_str(), out.length(), false, true, false );
			}
		}

		void InsertNewline( ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if(( madedit ) && ( !madedit->IsReadOnly() )) {
				madedit->ProcessCommand( ecReturn );
			}
		}

		void InsertIncrementalNumber( long initial, long step, long total, long stepType,
									  long fmt, long align, bool zeroPad, const std::string & pref, const std::string & post ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxString wxPrefix( pref.c_str(), wxConvUTF8 ), wxPostfix( post.c_str(), wxConvUTF8 );
				madedit->InsertIncrementalNumber( initial, step, total,
						( MadNumberingStepType )stepType, ( MadNumberFormat )fmt, ( MadNumberAlign )align, zeroPad, wxPrefix, wxPostfix );
			}
		}

		void ScrollLineUp() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ProcessCommand( ecScrollLineUp ); }
		}

		void ScrollLineDown() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ProcessCommand( ecScrollLineDown ); }
		}

		void ScrollPageUp() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ProcessCommand( ecScrollPageUp ); }
		}

		void ScrollPageDown() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ProcessCommand( ecScrollPageDown ); }
		}

		void ScrollLeft() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ProcessCommand( ecScrollLeft ); }
		}

		void ScrollRight() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ProcessCommand( ecScrollRight ); }
		}

		void GoToLine( long line ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GoToLine( line ); }
		}

		void GoToLineColumn( long line, long column ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				if(column < 0) column = 0;
				if(line < 0) line = 0;
				madedit->GoToLine( line );

				for( long col = 0; col < ( column - 1 ); ++col ) // no validate for input
				{ madedit->ProcessCommand( ecRight ); }
			}
		}

		void SetSyntax( const std::string &title ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( ! title.empty() ) ) {
				wxString wxTitle( title.c_str(), wxConvUTF8 );
				madedit->SetSyntax( wxTitle );
			}
		}

		const std::string GetSyntaxTitle() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxString title = madedit->GetSyntaxTitle();
				return std::string( title.mb_str() );
			}
			else { return std::string( "" );}
		}

		void LoadDefaultSyntaxScheme() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->LoadDefaultSyntaxScheme(); }
		}

		void SetEncoding( const std::string &encname ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				if( encname.empty() )
				{ return; }

				wxString wxEncname( encname.c_str(), wxConvUTF8 );
				madedit->SetEncoding( wxEncname );
			}
		}
		const std::string GetEncodingName() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) ) {
				wxString desc = madedit->GetEncodingName();
				return std::string( desc.mb_str() );
			}
			else {
				return std::string( "" );
			}
		}

		const std::string GetEncodingDescription() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxString desc = madedit->GetEncodingDescription();
				return std::string( desc.mb_str() );
			}
			else {
				return std::string( "" );
			}
		}

		long GetEncodingType() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return ( long )madedit->GetEncodingType(); }
			else { return 0; }
		}

		bool GetRecordCaretMovements() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( !( madedit ) ) { return false; }

			return madedit->GetRecordCaretMovements();
		}

		void SetRecordCaretMovements( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( !( madedit ) ) { return; }

			madedit->SetRecordCaretMovements( value );
		}

		void SetTextFont( const std::string &name, long size, bool forceReset ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( ! name.empty()) && (size > 0) && ( madedit ) ) {
				wxString wxName( name.c_str(), wxConvUTF8 );
				madedit->SetTextFont( wxName, size, forceReset );
			}
		}

		void SetHexFont( const std::string &name, long size, bool forceReset ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( ! name.empty()) && (size > 0) && ( madedit ) ) {
				wxString wxName( name.c_str(), wxConvUTF8 );
				madedit->SetHexFont( wxName, size, forceReset );
			}
		}

		mad_py::tuple GetTextFont() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				int size = 0;
				wxString name;
				madedit->GetTextFont( name, size );

				if(name.size())
					return mad_py::make_tuple( std::string( name.mb_str() ), size );
			}

			return mad_py::make_tuple( std::string( "" ), 0 );
		}

		mad_py::tuple GetHexFont() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				int size = 0;
				wxString name;
				madedit->GetHexFont( name, size );

				if(name.size())
					return mad_py::make_tuple( std::string( name.mb_str() ), size );
			}

			return mad_py::make_tuple( std::string( "" ), 0 );
		}

		/*wxFont GetFont()
		{
		    return (madedit)->GetFont();
		}*/

		mad_py::tuple GetFontNameSize() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				int size = 0;
				wxString name;
				madedit->GetFont( name, size );

				if(name.size())
					return mad_py::make_tuple( std::string( name.mb_str() ), size );
			}

			return mad_py::make_tuple( std::string( "" ), 0 );
		}

		void SetFontA( const std::string &name, long size ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !name.empty() ) ) {
				wxString wxName( name.c_str(), wxConvUTF8 );
				madedit->SetFont( wxName, size );
			}
		}

		/*bool SetFontB(const wxFont& font)
		{
		    return (madedit)->SetFont(font);
		}*/

		void SetFixedWidthMode( bool mode ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetFixedWidthMode( mode ); }
		}

		bool GetFixedWidthMode() {
			bool mode = false;
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;

			if( madedit )
			{ mode = madedit->GetFixedWidthMode(); }

			return mode;
		}

		void SetLineSpacing( long percent ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetLineSpacing( percent ); }
		}

		long GetLineSpacing() {
			long spacing = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ spacing = madedit->GetLineSpacing(); }

			return spacing;
		}

		void SetEditMode( long mode ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit && ( mode >= emTextMode ) && ( mode <= emHexMode ) )
			{ madedit->SetEditMode( ( MadEditMode )mode ); }
		}

		long GetEditMode() {
			long mode = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ mode = ( long )madedit->GetEditMode(); }

			return mode;
		}

		void SetSingleLineMode( bool mode ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetSingleLineMode( mode ); }
		}

		void SetTabColumns( long value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetTabColumns( value ); }
		}

		long GetTabColumns() {
			long cols = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ cols = madedit->GetTabColumns(); }

			return cols;
		}

		void SetIndentColumns( long value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetIndentColumns( value ); }
		}

		long GetIndentColumns() {
			long cols = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ cols = madedit->GetIndentColumns(); }

			return cols;
		}

		void SetInsertSpacesInsteadOfTab( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetInsertSpacesInsteadOfTab( value ); }
		}

		bool GetInsertSpacesInsteadOfTab() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetInsertSpacesInsteadOfTab(); }

			return res;
		}

		void SetWantTab( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetWantTab( value ); }
		}
		bool GetWantTab() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetWantTab(); }

			return res;
		}

		void SetWordWrapMode( long mode ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit && ( mode >= wwmNoWrap ) && ( mode <= wwmWrapByColumn ) )
			{ madedit->SetWordWrapMode( ( MadWordWrapMode )mode ); }
		}

		long GetWordWrapMode() {
			long mode  = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ mode = ( long )madedit->GetWordWrapMode(); }

			return mode;
		}

		void SetShowEndOfLine( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetShowEndOfLine( value ); }
		}

		void SetShowTabChar( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetShowTabChar( value ); }
		}

		void SetShowSpaceChar( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetShowSpaceChar( value ); }
		}

		void SetMarkActiveLine( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetMarkActiveLine( value ); }
		}

		void SetDisplayLineNumber( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetDisplayLineNumber( value ); }
		}

		bool GetDisplayLineNumber() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetDisplayLineNumber(); }

			return res;
		}
		void SetDisplayBookmark( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetDisplayBookmark( value ); }
		}

		bool GetDisplayBookmark() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetDisplayBookmark(); }

			return res;
		}

		bool GetShowEndOfLine() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetShowEndOfLine(); }

			return res;
		}
		bool GetShowTabChar() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->GetShowTabChar(); }

			return res;
		}

		bool GetShowSpaceChar() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetShowSpaceChar(); }

			return res;
		}

		bool GetMarkActiveLine() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetMarkActiveLine(); }

			return res;
		}

		void SetMarkBracePair( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetMarkBracePair( value ); }
		}

		bool GetMarkBracePair() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetMarkBracePair(); }

			return res;
		}

		long GetMaxColumns() {
			long cols = 0;
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
				cols = madedit->GetMaxColumns();
			return cols;
		}

		void SetMaxColumns( long cols ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetMaxColumns( cols ); }
		}

		bool GetAutoIndent() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetAutoIndent(); }

			return res;
		}

		void SetAutoIndent( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
				madedit->SetAutoIndent( value );
		}

		void SetAutoCompletePair( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
				madedit->SetAutoCompletePair( value );
		}

		bool GetAutoCompletePair() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetAutoCompletePair(); }

			return res;
		}

		void SetInsertPairForSelection( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
				madedit->SetInsertPairForSelection( value );
		}

		bool GetInsertPairForSelection() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetInsertPairForSelection(); }

			return res;
		}

		void SetInsertMode( bool mode ) { // true: insert, false: overwrite
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetInsertMode( mode ); }
		}

		bool GetInsertMode() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetInsertMode(); }

			return res;
		}

		void SetCaretType( long type ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit  && ( type >= ctVerticalLine ) && ( type <= ctBlock ))
			{ madedit->SetCaretType( ( MadCaretType )type ); }
		}

		bool GetMouseSelectToCopy() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetMouseSelectToCopy(); }

			return res;
		}

		void SetMouseSelectToCopy( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetMouseSelectToCopy( value ); }
		}

		bool GetMouseSelectToCopyWithCtrlKey() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if (madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetMouseSelectToCopyWithCtrlKey(); }

			return res;
		}

		void SetMouseSelectToCopyWithCtrlKey( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetMouseSelectToCopyWithCtrlKey( value ); }
		}

		bool GetMiddleMouseToPaste() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetMiddleMouseToPaste(); }

			return res;
		}

		void SetMiddleMouseToPaste( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetMiddleMouseToPaste( value ); }
		}

		long GetMaxWordWrapWidth() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetMaxWordWrapWidth(); }

			return res;
		}

		long GetUCharWidth( long uc ) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetUCharWidth( ( ucs4_t )uc ); }

			return res;
		}

		long GetHexUCharWidth( long uc ) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetHexUCharWidth( ( ucs4_t )uc ); }

			return res;
		}

		long GetUCharType( long uc ) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetUCharType( ( ucs4_t )uc ); }

			return res;
		}

		// all are zero-based
		mad_py::tuple GetCaretPositionA() {
			int line = 0, subrow = 0;
			wxFileOffset column = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GetCaretPosition( line, subrow, column ); }

			return mad_py::make_tuple( line, subrow, ( long )column );
		}

		long GetCaretPositionB() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetCaretPosition(); }

			return res;
		}

		const std::string GetFileName() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				wxString name;
				name = madedit->GetFileName();
				if(name.size())
					return std::string( name.mb_str() );
			}

			return std::string( "" );
		}

		long GetFileSize() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetFileSize(); }

			return res;
		}

		bool IsSelected() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->IsSelected(); }

			return res;
		}

		bool IsZeroSelected() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->IsSelected(); }

			return res;
		}

		long GetLineBeginPos( int line )	{
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetLineBeginPos( line ); }

			return res;
		}

		long GetSelectionSize() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetSelectionSize(); }

			return res;
		}

		long GetIndentCountByPos( long endpos ) {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetIndentCountByPos( (wxFileOffset) endpos ); }

			return res;
			
		}

		long GetSelectionBeginPos() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetSelectionBeginPos(); }

			return res;
		}

		long GetSelectionEndPos() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetSelectionEndPos(); }

			return res;
		}

		// return -1 for no selection
		mad_py::tuple GetSelectionLineId() {
			int beginline = -1, endline = -1;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GetSelectionLineId( beginline, endline ); }

			return mad_py::make_tuple( beginline, endline );
		}

		void SetSelection( long beginpos, long endpos, bool bCaretAtBeginPos = false ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				long fsize = ( long )g_ActiveMadEdit->GetFileSize();
				if( endpos == fsize )
					endpos = -1;
				madedit->SetSelection( ( wxFileOffset )beginpos, ( wxFileOffset )endpos, bCaretAtBeginPos );
			}
		}

		void SelectWholeLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SelectWholeLine(); }
		}

		long GetLineCount() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->GetLineCount(); }

			return res;
		}

		void ConvertNewLineType( long type ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ConvertNewLineType( ( MadNewLineType )type ); }
		}
		void SetInsertNewLineType( long type ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetInsertNewLineType( ( MadNewLineType )type ); }
		}

		long GetNewLineType() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetNewLineType(); }

			return res;
		}

		long GetInsertNewLineType() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetInsertNewLineType(); }

			return res;
		}

		bool IsModified() {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = madedit->IsModified(); }

			return res;
		}

		long GetModificationTime() {
			long res = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ res = ( long )madedit->GetModificationTime(); }

			return res;
		}

		void SetReadOnly( bool value ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetReadOnly( value ); }
		}

		bool IsReadOnly() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->IsReadOnly(); }
			else
			{ return true; }
		}

		bool IsTextFile() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->IsTextFile(); }
			else
			{ return false; }
		}

		const std::string GetSelText() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				wxString ws;
				madedit->GetSelText( ws );
				if(ws.size())
					return std::string( ws.mb_str() );
			}
			return std::string("");
		}

		const std::string GetRangeText( long begpos, long endpos ){
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				wxString ws;
				wxFileOffset bpos = begpos, epos = endpos;
				madedit->GetRangeText( ws, bpos, epos );
				if(ws.size())
					return std::string( ws.mb_str() );
			}
			return std::string("");
		}

		const std::string GetText( bool ignoreBOM = true ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				wxString ws;
				madedit->GetText( ws, ignoreBOM );
				if(ws.size())
					return std::string( ws.mb_str() );
			}
			return std::string("");
		}

		void SetText( const std::string &ws ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( ! ws.empty() ) && ( madedit ) ) {
				wxString wxWs( ws.c_str(), wxConvUTF8 );
				madedit->SetText( wxWs );
			}
		}

		// line: zero based
		// return true for full line, false for partial line
		mad_py::tuple GetLine( long line, size_t maxlen = 0, bool ignoreBOM = true ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				wxString wxWs;
				bool ret = madedit->GetLine( wxWs, line, maxlen, ignoreBOM );
				if(wxWs.size())
					return mad_py::make_tuple( std::string( wxWs.mb_str() ), ret );
			}

			return mad_py::make_tuple( std::string( "" ), false );
		}

		long GetLineByPos( long pos ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->GetLineByPos( pos ); }
			else
			{ return -1; }
		}

		const std::string GetSelHexString( bool withSpace ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxString wxWs;
				madedit->GetSelHexString( wxWs, withSpace );
				return std::string( wxWs.mb_str() );
			}
			else
			{ return std::string( "" ); }
		}

		const std::string GetWordFromCaretPos() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxString wxWs;
				madedit->GetWordFromCaretPos( wxWs );
				return std::string( wxWs.mb_str() );
			}
			else
			{ return std::string( "" ); }
		}

		void Delete() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->Delete(); }
		}

		void CutLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->CutLine(); }
		}

		void DeleteLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->DeleteLine(); }
		}

		void InsertTabChar() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->InsertTabChar(); }
		}

		void InsertDateTime() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->InsertDateTime(); }
		}

		void HighlightWords() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->HighlightWords(); }
		}

		void SelectAll() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SelectAll(); }
		}

		void CutToClipboard() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{
				if( !madedit->IsReadOnly() )
				{ madedit->CutToClipboard(); }
				else
				{ madedit->CopyToClipboard(); }
			}
		}

		void CopyToClipboardA() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->CopyToClipboard(); }
		}

		void PasteFromClipboard() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->PasteFromClipboard(); }
		}

		void DndBegDrag() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->DndBegDrag(); }
		}

		void DndDrop() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->DndDrop(); }
		}

		bool CanPaste() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->CanPaste(); }
			else
			{ return false; }
		}

		void CopyToClipboardB( const std::string &txt ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxString text( txt.c_str(), wxConvUTF8 );
				madedit->CopyToClipboard( text );
			}
		}

		bool CanUndo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->CanUndo(); }
			return false;
		}

		bool CanRedo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->CanRedo(); }
			return false;
		}

		void Undo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->Undo(); }
		}

		void Redo() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->Redo(); }
		}

		void Goto( long pos ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetCaretPosition( ( wxFileOffset )pos ); }
		}

		void SetCaretPosition( long pos, long selbeg = -1, long selend = -1 ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetCaretPosition( ( wxFileOffset )pos, ( wxFileOffset )selbeg, ( wxFileOffset )selend ); }
		}

		void GoBack() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GoBack(); }
		}

		void GoForward() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GoForward(); }
		}
		
		bool HasBracePair() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ return madedit->HasBracePair(); }
			else
			{ return false; }
		}

		void GoToLeftBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GoToLeftBrace(); }
		}

		void GoToRightBrace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GoToRightBrace(); }
		}

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		long FindTextNext( const std::string &text,
						  bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false,
						  long rangeFrom = -1, long rangeTo = -1 ) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !text.empty() ) ) {
				wxString wxText( text.c_str(), wxConvUTF8 );
				wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
				
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				ok = madedit->FindTextNext( wxText, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, from, to );
			}

			return ok;
		}

		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		long FindTextPrevious( const std::string &text,
							  bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false,
							  long rangeFrom = -1, long rangeTo = -1 ) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !text.empty() ) ) {
				wxString wxText( text.c_str(), wxConvUTF8 );
				wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
				
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				ok = madedit->FindTextPrevious( wxText, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, from, to );
			}

			return ok;
		}

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		long FindHexNext( const std::string &hexstr, long rangeFrom = -1, long rangeTo = -1 ) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !hexstr.empty() ) ) {
				wxString wxHexExpr( hexstr.c_str(), wxConvUTF8 );
				wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
				ok = madedit->FindHexNext( wxHexExpr, from, to );
			}

			return ok;
		}

		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		long FindHexPrevious( const std::string &hexstr, long rangeFrom = -1, long rangeTo = -1 ) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !hexstr.empty() ) ) {
				wxString wxHexExpr( hexstr.c_str(), wxConvUTF8 );
				wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
				ok = madedit->FindHexPrevious( wxHexExpr, from, to );
			}

			return ok;
		}

		// replace the selected text that must match expr
		long ReplaceText( const std::string &expr, const std::string &fmt,
						 bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false,
						 long rangeFrom = -1, long rangeTo = -1 ) {
			if( expr.empty() )
			{ return RR_EXPR_ERROR; }

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( !( madedit ) || madedit->IsReadOnly() )
			{ return RR_NREP_NNEXT; }

			wxString wxExpr( expr.c_str(), wxConvUTF8 ), wxFmt( fmt.c_str(), wxConvUTF8 );
			wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
			
			if(bRegex) bWholeWord = false;
			else bDotMatchNewline = false;
			return madedit->ReplaceText( wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, from, to );
		}

		// replace the selected text that must match expr
		long ReplaceTextNoDoubleCheck( const std::string &expr, const std::string &fmt,
						 bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false,
						 long rangeFrom = -1, long rangeTo = -1 ) {
			if( expr.empty() )
			{ return RR_EXPR_ERROR; }

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( !( madedit ) || madedit->IsReadOnly() )
			{ return RR_NREP_NNEXT; }

			wxString wxExpr( expr.c_str(), wxConvUTF8 ), wxFmt( fmt.c_str(), wxConvUTF8 );
			wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
			
			if(bRegex) bWholeWord = false;
			else bDotMatchNewline = false;
			return madedit->ReplaceTextNoDoubleCheck( wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, from, to );
		}

		long ReplaceHex( const std::string &expr, const std::string &fmt,
						long rangeFrom = -1, long rangeTo = -1 ) {
			if( expr.empty() )
			{ return RR_EXPR_ERROR; }

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( !( madedit ) || madedit->IsReadOnly() )
			{ return RR_NREP_NNEXT; }

			wxString wxExpr( expr.c_str(), wxConvUTF8 ), wxFmt( fmt.c_str(), wxConvUTF8 );
			wxFileOffset from = ( wxFileOffset )rangeFrom, to = ( wxFileOffset )rangeTo;
			return madedit->ReplaceHex( wxExpr, wxFmt, from, to );
		}

		// return the replaced count or SR_EXPR_ERROR
		long ReplaceTextAll( const std::string &expr, const std::string &fmt,
							bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, 
							long rangeFrom = -1, long rangeTo = -1 ) {
			long ok = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !expr.empty() ) && ( !madedit->IsReadOnly() ) ) {
				wxString wxExpr( expr.c_str(), wxConvUTF8 ), wxFmt( fmt.c_str(), wxConvUTF8 );
				
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				ok = madedit->ReplaceTextAll( wxExpr, wxFmt, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline,
					nullptr, nullptr, ( wxFileOffset )rangeFrom, ( wxFileOffset )rangeTo );
			}

			return ok;
		}

		long ReplaceHexAll( const std::string &expr, const std::string &fmt,
						   long rangeFrom = -1, long rangeTo = -1 ) {
			long ok = 0;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !expr.empty() ) && ( !madedit->IsReadOnly() ) ) {
				wxString wxExpr( expr.c_str(), wxConvUTF8 ), wxFmt( fmt.c_str(), wxConvUTF8 );
				ok = madedit->ReplaceHexAll( wxExpr, wxFmt, nullptr, nullptr, ( wxFileOffset )rangeFrom, ( wxFileOffset )rangeTo );
			}

			return ok;
		}

#ifndef PYMADEDIT_DLL
		// list the matched data to pbegpos & pendpos
		// return the found count or SR_EXPR_ERROR
		long FindTextAll( const std::string &expr, bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bDotMatchNewline = false, bool showresults = true ) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( !expr.empty() ) && ( madedit ) ) {
				wxString wxExpr( expr.c_str(), wxConvUTF8 );
				vector<wxFileOffset> begpos, endpos;
				//wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
				
				if(bRegex) bWholeWord = false;
				else bDotMatchNewline = false;
				ok = madedit->FindTextAll( wxExpr, bRegex, bCaseSensitive, bWholeWord, bDotMatchNewline, false, &begpos, &endpos );

				if( ok >= 0 && showresults ) {
					static wxString text( _( "Search Results" ) );
					int pid = g_MainFrame->m_InfoNotebook->GetPageIndex( g_MainFrame->m_FindInFilesResults );
					wxASSERT(pid != wxNOT_FOUND);
					g_MainFrame->m_InfoNotebook->SetPageText( (size_t)pid, text );
					wxString strtobesearch = _("Search \"") + expr + wxT("\" ") + wxString::Format( _("(%s hits in 1 file)"), ( wxLongLong( ok ).ToString().c_str() ) );
					wxTreeItemId myroot = g_MainFrame->NewSearchSession(strtobesearch);
					DisplayFindAllResult( myroot, begpos, endpos, madedit );
				}
			}

			return ok;
		}

		long FindHexAll( const std::string &expr, bool showresults = true ) {
			long ok = SR_EXPR_ERROR;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( !expr.empty() ) && ( madedit ) ) {
				wxString wxExpr( expr.c_str(), wxConvUTF8 ), fmt;
				vector<wxFileOffset> begpos, endpos;
				//wxTreeCtrl * results = g_MainFrame->m_FindInFilesResults;
				ok = madedit->FindHexAll( wxExpr, false, &begpos, &endpos );

				if( ok >= 0 && showresults ) {
					static wxString text( _( "Search Results" ) );
					int pid = g_MainFrame->m_InfoNotebook->GetPageIndex( g_MainFrame->m_FindInFilesResults );
					wxASSERT(pid != wxNOT_FOUND);
					g_MainFrame->m_InfoNotebook->SetPageText((size_t)pid, text );
					
					wxString strtobesearch = _("Search \"") + expr + wxT("\" ") + wxString::Format( _("(%s hits in 1 file)"), ( wxLongLong( ok ).ToString().c_str() ) );
					wxTreeItemId myroot = g_MainFrame->NewSearchSession(strtobesearch);
					DisplayFindAllResult( myroot, begpos, endpos, madedit );
				}
			}

			return ok;
		}
#endif
		bool LoadFromFile( const std::string &filename, const std::string &encoding = std::string( "" ) ) {
			bool res = false;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( ! filename.empty() ) ) {
				wxString wxEncoding( encoding.c_str(), wxConvUTF8 );
				wxString wxFilename( filename.c_str(), wxConvUTF8 );
				res = madedit->LoadFromFile( wxFilename, wxEncoding );
			}

			return res;
		}

		bool SaveToFile( const std::string &filename ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( !filename.empty() ) && ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxString wxFilename( filename.c_str(), wxConvUTF8 );
				return madedit->SaveToFile( wxFilename );
			}

			return false;
		}

		bool Reload() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) 
				return madedit->Reload();
			else
				return false;
		}

		// if the file is modified by another app, reload it.
		bool ReloadByModificationTime() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) 
				return madedit->ReloadByModificationTime( false );
			else
				return false;
		}

		// restore pos in Reload(), ConvertEncoding()
		void RestorePosition( long pos, long toprow ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxFileOffset wxPos = pos;
				madedit->RestorePosition( wxPos, toprow );
			}
		}

		// write back to the original FileName
		// if FileName is empty, ask the user to get filename
		// return wxID_YES(Saved), wxID_NO(Not Saved), or wxID_CANCEL
		long Save( bool ask, const std::string &title, bool saveas ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !title.empty() ) && ( !madedit->IsReadOnly() ) ) {
				wxString wxTitle( title.c_str(), wxConvUTF8 );
				return madedit->Save( ask, wxTitle, saveas );
			}
			else
			{ return wxID_NO; }
		}

		// add: gogo, 21.09.2009
		void ToggleBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->ToggleBookmark(); }
		}

		void GotoNextBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GotoNextBookmark(); }
		}

		void GotoPreviousBookmark() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->GotoPreviousBookmark(); }
		}

		//----------
		void ConvertEncoding( const std::string &newenc, long flag ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				if( newenc.empty() )
				{ return; }

				MadConvertEncodingFlag mflag = ( MadConvertEncodingFlag )flag;
				wxString wxNewenc( newenc.c_str(), wxConvUTF8 );
				madedit->ConvertEncoding( wxNewenc, mflag );
			}
		}

		void ConvertChineseA( long flag ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ConvertChinese( ( MadConvertEncodingFlag )flag ); }
		}

		bool HasBOM() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) 
				return madedit->HasBOM();
			else
				return false;
		}

		void ToggleBOM() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ToggleBOM(); }
		}

		void IncreaseDecreaseIndent( bool incIndent ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if (madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->IncreaseDecreaseIndent( incIndent ); }
		}

		bool HasLineComment() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) 
				return madedit->HasLineComment();
			else
				return false;
		}

		void CommentUncomment( bool comment ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->CommentUncomment( comment ); }
		}

		void ToUpperCase() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ToUpperCase(); }
		}

		void ToLowerCase() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ToLowerCase(); }
		}

		void InvertCase() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->InvertCase(); }
		}

		void Capitalize() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->Capitalize(); }
		}

		void ToHalfWidth( bool ascii = true, bool japanese = true, bool korean = true, bool other = true ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ToHalfWidth( ascii, japanese, korean, other ); }
		}

		void ToFullWidth( bool ascii = true, bool japanese = true, bool korean = true, bool other = true ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ToFullWidth( ascii, japanese, korean, other ); }
		}

		void TrimTrailingSpaces() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->TrimTrailingSpaces(); }
		}

		void TrimLeadingSpaces() {
			MadEdit *madedit = g_CurrentMadEdit;
			if (madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->TrimLeadingSpaces(); }
		}

		void DeleteEmptyLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->DeleteEmptyLines(); }
		}


		void DeleteEmptyLinesWithSpaces() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->DeleteEmptyLinesWithSpaces(); }
		}

		void JoinLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->JoinLines(); }
		}

		// startline<0 : sort all lines otherwise sort [beginline, endline]
		void SortLines( MadSortFlags flags, long beginline, long endline ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->SortLines( flags, beginline, endline ); }
		}

		// convert WordWraps to NewLine-chars in the SelText or whole file
		void ConvertWordWrapToNewLine() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ConvertWordWrapToNewLine(); }
		}
		// convert NewLine-chars to WordWraps in the SelText
		void ConvertNewLineToWordWrap() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ConvertNewLineToWordWrap(); }
		}

		void ConvertSpaceToTab() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ConvertSpaceToTab(); }
		}
		void ConvertTabToSpace() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ConvertTabToSpace(); }
		}

		void CopyAsHexString( bool withSpace ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->CopyAsHexString( withSpace ); }
		}

		void CopyRevertHex( const std::string &delimiters ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxString wxDelimiters( delimiters.c_str(), wxConvUTF8 );
				madedit->CopyRevertHex( wxDelimiters );
			}
		}

		mad_py::tuple WordCount( bool selection ) {
			int words = 0, chars = 0, spaces = 0, lines = 0, halfwidths = 0, fullwidths = 0;
			wxString str;

			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit ) {
				wxArrayString detail;
				madedit->WordCount( selection, words, chars, spaces, halfwidths, fullwidths, lines, &detail );

				for( size_t i = 0; i < detail.Count(); ++i ) {
					str << detail[i] << wxT( "\n" );
				}
			}

			return mad_py::make_tuple( words, chars, spaces, lines, halfwidths, fullwidths, std::string( str.mb_str() ) );
		}

		void SetColumnSelection( long startlineid, long startxpos, long hlines, long wlines ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( madedit )
			{ madedit->SetColumnSelection( startlineid, startxpos, hlines, wlines ); }
		}

		void ColumnAlignLeft() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ColumnAlignLeft(); }
		}

		void ColumnAlignRight() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ColumnAlignRight(); }
		}
		/**=======================================================**/

		void Astyle() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( g_MainFrame && ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxCommandEvent event;
				g_MainFrame->OnToolsAstyleFormat( event );
			}
		}

		void XMLFormat() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( g_MainFrame && ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxCommandEvent event;
				g_MainFrame->OnToolsXMLFormat( event );
			}
		}

		void Markdown2Html() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( g_MainFrame && ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxCommandEvent event;
				g_MainFrame->OnToolsMarkdown2Html( event );
			}
		}

		void Html2PlainText() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( g_MainFrame && ( madedit ) && ( !madedit->IsReadOnly() ) ) {
				wxCommandEvent event;
				g_MainFrame->OnToolsHtml2PlainText( event );
			}
		}

		void ToggleBookmarkInSearch( bool bookmark ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ToggleBookmarkInSearch(bookmark); }
		}

		void CutDelBookmarkedLines( bool copyLines = false )  {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->CutDelBookmarkedLines(copyLines); }
		}

		void DeleteUnmarkedLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->DeleteUnmarkedLines(); }
		}

		void CopyUnmarkedLines()  {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->CopyUnmarkedLines(); }
		}

		void CutUnmarkedLines()  {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->CutUnmarkedLines(); }
		}

		void CopyCutDeleteUnmarkedLines( bool copyLines = false, bool deleteLines = false ) {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->CopyCutDeleteUnmarkedLines( copyLines, deleteLines ); }
		}

		void ReplaceBookmarkedLines() {
			MadEdit *madedit = g_CurrentMadEdit;
			if(madedit == nullptr)
				madedit = g_ActiveMadEdit;
			if( ( madedit ) && ( !madedit->IsReadOnly() ) )
			{ madedit->ReplaceBookmarkedLines(); }
		}
	};

	int MsgBox(const std::string& message, const std::string& caption = "Message", long style = wxOK | wxCENTRE)
	{
		wxString wxMessage( message.c_str(), wxConvUTF8 ), wxCaption( caption.c_str(), wxConvUTF8 );
		return MadMessageBox( wxMessage, wxCaption, style );
	}

	const std::string InputBox(const std::string& message, const std::string& caption = "Input Text")
	{
		wxString wxMessage( message.c_str(), wxConvUTF8 ), wxCaption( caption.c_str(), wxConvUTF8 );
		wxString input = wxGetTextFromUser( wxMessage, wxCaption );
		return std::string( input.mb_str() );
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FindTextNext_member_overloads, FindTextNext, 4, 7 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FindTextPrevious_member_overloads, FindTextPrevious, 4, 7 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FindHexNext_member_overloads, FindHexNext, 1, 3 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FindHexPrevious_member_overloads, FindHexPrevious, 1, 3 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FindTextAll_member_overloads, FindTextAll, 4, 6 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( FindHexAll_member_overloads, FindHexAll, 1, 2 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ReplaceText_member_overloads, ReplaceText, 5, 8 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ReplaceHex_member_overloads, ReplaceHex, 2, 4 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ReplaceTextAll_member_overloads, ReplaceTextAll, 5, 8 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ReplaceHexAll_member_overloads, ReplaceHexAll, 2, 4 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( LoadFromFile_member_overloads, LoadFromFile, 1, 2 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ToHalfWidth_member_overloads, ToHalfWidth, 0, 4 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ToFullWidth_member_overloads, ToFullWidth, 0, 4 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( SetCaretPosition_member_overloads, SetCaretPosition, 1, 3 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( GetLine_member_overloads, GetLine, 1, 3 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( GetText_member_overloads, GetText, 0, 1 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( SetSelection_member_overloads, SetSelection, 2, 3 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( CutDelBookmarkedLines_member_overloads, CutDelBookmarkedLines, 0, 1 )
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( CopyCutDeleteUnmarkedLines_member_overloads, CopyCutDeleteUnmarkedLines, 0, 2 )

BOOST_PYTHON_FUNCTION_OVERLOADS( InputBox_overloads, mad_python::InputBox, 1, 2 )
BOOST_PYTHON_FUNCTION_OVERLOADS( MsgBox_overloads, mad_python::MsgBox, 1, 3 )

BOOST_PYTHON_MODULE( madpython ) {
	using namespace mad_python;
	using namespace mad_py;
	class_<PyMadEdit>( "MadEdit", "This class is a collection of wrapper functions of MadEdit.", init<>() )
	.def( "ProcessCommand", &PyMadEdit::ProcessCommand, "" )
	.def( "InsertWChar", &PyMadEdit::InsertWChar, "" )
	.def( "InsertStr", &PyMadEdit::InsertStr, "" )
	.def( "InsertIncrementalNumber", &PyMadEdit::InsertIncrementalNumber, "" )
	.def( "GoToLine", &PyMadEdit::GoToLine, "Go To Line of current file" )
	.def( "GoToLineColumn", &PyMadEdit::GoToLineColumn, "Go To Line, Column of current file" )
	.def( "SetSyntax", &PyMadEdit::SetSyntax, "Set syntax for current file" )
	.def( "GetSyntaxTitle", &PyMadEdit::GetSyntaxTitle, return_value_policy<return_by_value>(), "" )
	.def( "LoadDefaultSyntaxScheme", &PyMadEdit::LoadDefaultSyntaxScheme, "" )
	.def( "SetEncoding", &PyMadEdit::SetEncoding, "" )
	.def( "GetEncodingName", &PyMadEdit::GetEncodingName, return_value_policy<return_by_value>(), "" )
	.def( "GetEncodingDescription", &PyMadEdit::GetEncodingDescription, return_value_policy<return_by_value>(), "" )
	.def( "GetEncodingType", &PyMadEdit::GetEncodingType, return_value_policy<return_by_value>(), "" )
	.def( "GetRecordCaretMovements", &PyMadEdit::GetRecordCaretMovements, return_value_policy<return_by_value>(), "" )
	.def( "SetRecordCaretMovements", &PyMadEdit::SetRecordCaretMovements, return_value_policy<return_by_value>(), "" )
	.def( "SetTextFont", &PyMadEdit::SetTextFont, "" )
	.def( "SetHexFont", &PyMadEdit::SetHexFont, "" )
	.def( "GetTextFont", &PyMadEdit::GetTextFont, "" )
	.def( "GetHexFont", &PyMadEdit::GetHexFont, "" )
	.def( "GetFontNameSize", &PyMadEdit::GetFontNameSize, return_value_policy<return_by_value>(), "Doc" )
	.def( "SetFixedWidthMode", &PyMadEdit::SetFixedWidthMode, "" )
	.def( "GetFixedWidthMode", &PyMadEdit::GetFixedWidthMode, "" )
	.def( "SetLineSpacing", &PyMadEdit::SetLineSpacing, "" )
	.def( "GetLineSpacing", &PyMadEdit::GetLineSpacing, "" )
	.def( "SetEditMode", &PyMadEdit::SetEditMode, "" )
	.def( "GetEditMode", &PyMadEdit::GetEditMode, return_value_policy<return_by_value>(), "" )
	.def( "SetSingleLineMode", &PyMadEdit::SetSingleLineMode, "" )
	.def( "SetTabColumns", &PyMadEdit::SetTabColumns, "" )
	.def( "GetTabColumns", &PyMadEdit::GetTabColumns, "" )
	.def( "SetIndentColumns", &PyMadEdit::SetIndentColumns, "" )
	.def( "GetIndentColumns", &PyMadEdit::GetIndentColumns, "" )
	.def( "SetInsertSpacesInsteadOfTab", &PyMadEdit::SetInsertSpacesInsteadOfTab, "" )
	.def( "GetInsertSpacesInsteadOfTab", &PyMadEdit::GetInsertSpacesInsteadOfTab, "" )
	.def( "SetWantTab", &PyMadEdit::SetWantTab, "" )
	.def( "GetWantTab", &PyMadEdit::GetWantTab, "" )
	.def( "SetWordWrapMode", &PyMadEdit::SetWordWrapMode, "" )
	.def( "GetWordWrapMode", &PyMadEdit::GetWordWrapMode, "" )
	.def( "SetDisplayLineNumber", &PyMadEdit::SetDisplayLineNumber, "" )
	.def( "SetShowEndOfLine", &PyMadEdit::SetShowEndOfLine, "" )
	.def( "SetShowTabChar", &PyMadEdit::SetShowTabChar, "" )
	.def( "SetShowSpaceChar", &PyMadEdit::SetShowSpaceChar, "" )
	.def( "SetMarkActiveLine", &PyMadEdit::SetMarkActiveLine, "" )
	.def( "GetDisplayLineNumber", &PyMadEdit::GetDisplayLineNumber, "" )
	.def( "GetShowEndOfLine", &PyMadEdit::GetShowEndOfLine, "" )
	.def( "GetShowTabChar", &PyMadEdit::GetShowTabChar, "" )
	.def( "GetShowSpaceChar", &PyMadEdit::GetShowSpaceChar, "" )
	.def( "GetMarkActiveLine", &PyMadEdit::GetMarkActiveLine, "" )
	.def( "SetMarkBracePair", &PyMadEdit::SetMarkBracePair, "" )
	.def( "GetMarkBracePair", &PyMadEdit::GetMarkBracePair, "" )
	.def( "GetMaxColumns", &PyMadEdit::GetMaxColumns, "" )
	.def( "SetMaxColumns", &PyMadEdit::SetMaxColumns, "" )
	.def( "GetAutoIndent", &PyMadEdit::GetAutoIndent, "" )
	.def( "SetAutoIndent", &PyMadEdit::SetAutoIndent, "" )
	.def( "SetAutoCompletePair", &PyMadEdit::SetAutoCompletePair, "" )
	.def( "GetAutoCompletePair", &PyMadEdit::GetAutoCompletePair, "" )
	.def( "SetInsertPairForSelection", &PyMadEdit::SetInsertPairForSelection, "" )
	.def( "GetInsertPairForSelection", &PyMadEdit::GetInsertPairForSelection, "" )
	.def( "SetInsertMode", &PyMadEdit::SetInsertMode, "" )
	.def( "GetInsertMode", &PyMadEdit::GetInsertMode, "" )
	.def( "SetCaretType", &PyMadEdit::SetCaretType, "" )
	.def( "GetMouseSelectToCopy", &PyMadEdit::GetMouseSelectToCopy, "" )
	.def( "SetMouseSelectToCopy", &PyMadEdit::SetMouseSelectToCopy, "" )
	.def( "GetMouseSelectToCopyWithCtrlKey", &PyMadEdit::GetMouseSelectToCopyWithCtrlKey, "" )
	.def( "SetMouseSelectToCopyWithCtrlKey", &PyMadEdit::SetMouseSelectToCopyWithCtrlKey, "" )
	.def( "GetMiddleMouseToPaste", &PyMadEdit::GetMiddleMouseToPaste, "" )
	.def( "SetMiddleMouseToPaste", &PyMadEdit::SetMiddleMouseToPaste, "" )
	.def( "GetMaxWordWrapWidth", &PyMadEdit::GetMaxWordWrapWidth, "" )
	.def( "GetUCharWidth", &PyMadEdit::GetUCharWidth, "" )
	.def( "GetHexUCharWidth", &PyMadEdit::GetHexUCharWidth, "" )
	.def( "GetUCharType", &PyMadEdit::GetUCharType, "" )
	.def( "GetCaretPositionA", &PyMadEdit::GetCaretPositionA, return_value_policy<return_by_value>(), "" )
	.def( "GetCaretPositionB", &PyMadEdit::GetCaretPositionB, return_value_policy<return_by_value>(), "" )
	.def( "GetFileName", &PyMadEdit::GetFileName, return_value_policy<return_by_value>(), "" )
	.def( "GetFileSize", &PyMadEdit::GetFileSize, return_value_policy<return_by_value>(), "" )
	.def( "IsSelected", &PyMadEdit::IsSelected, "" )
	.def( "IsZeroSelected", &PyMadEdit::IsZeroSelected, "" )
	.def( "GetLineBeginPos", &PyMadEdit::GetLineBeginPos, return_value_policy<return_by_value>(), "" )
	.def( "GetSelectionSize", &PyMadEdit::GetSelectionSize, return_value_policy<return_by_value>(), "" )
	.def( "GetSelectionBeginPos", &PyMadEdit::GetSelectionBeginPos, return_value_policy<return_by_value>(), "" )
	.def( "GetSelectionEndPos", &PyMadEdit::GetSelectionEndPos, return_value_policy<return_by_value>(), "" )
	.def( "GetSelectionLineId", &PyMadEdit::GetSelectionLineId, return_value_policy<return_by_value>(), "" )
	.def( "GetLineCount", &PyMadEdit::GetLineCount, "" )
	.def( "ConvertNewLineType", &PyMadEdit::ConvertNewLineType, "" )
	.def( "SetInsertNewLineType", &PyMadEdit::SetInsertNewLineType, "" )
	.def( "GetNewLineType", &PyMadEdit::GetNewLineType, "" )
	.def( "GetInsertNewLineType", &PyMadEdit::GetInsertNewLineType, "" )
	.def( "IsModified", &PyMadEdit::IsModified, "" )
	.def( "GetModificationTime", &PyMadEdit::GetModificationTime, "" )
	.def( "SetReadOnly", &PyMadEdit::SetReadOnly, "" )
	.def( "IsReadOnly", &PyMadEdit::IsReadOnly, "" )
	.def( "IsTextFile", &PyMadEdit::IsTextFile, "" )
	.def( "GetSelText", &PyMadEdit::GetSelText, return_value_policy<return_by_value>(), "" )
	.def( "GetRangeText", &PyMadEdit::GetRangeText, return_value_policy<return_by_value>(), "" )
	.def( "SetText", &PyMadEdit::SetText, "" )
	.def( "Goto", &PyMadEdit::Goto, "" )
	.def( "GetLineByPos", &PyMadEdit::GetLineByPos, "" )
	.def( "GetSelHexString", &PyMadEdit::GetSelHexString, "" )
	.def( "GetWordFromCaretPos", &PyMadEdit::GetWordFromCaretPos, return_value_policy<return_by_value>(), "" )
	.def( "InsertNewline", &PyMadEdit::InsertNewline, "" )
	.def( "Delete", &PyMadEdit::Delete, "" )
	.def( "CutLine", &PyMadEdit::CutLine, "" )
	.def( "DeleteLine", &PyMadEdit::DeleteLine, "" )
	.def( "InsertTabChar", &PyMadEdit::InsertTabChar, "" )
	.def( "InsertDateTime", &PyMadEdit::InsertDateTime, "" )
	.def( "HighlightWords", &PyMadEdit::HighlightWords, "" )
	.def( "SelectAll", &PyMadEdit::SelectAll, "" )
	.def( "CutToClipboard", &PyMadEdit::CutToClipboard, "" )
	.def( "PasteFromClipboard", &PyMadEdit::PasteFromClipboard, "" )
	.def( "DndBegDrag", &PyMadEdit::DndBegDrag, "" )
	.def( "DndDrop", &PyMadEdit::DndDrop, "" )
	.def( "CanPaste", &PyMadEdit::CanPaste, "" )
	.def( "CanUndo", &PyMadEdit::CanUndo, return_value_policy<return_by_value>(), "" )
	.def( "CanRedo", &PyMadEdit::CanRedo, return_value_policy<return_by_value>(), "" )
	.def( "Undo", &PyMadEdit::Undo, "" )
	.def( "Redo", &PyMadEdit::Redo, "" )
	.def( "HasBracePair", &PyMadEdit::HasBracePair, return_value_policy<return_by_value>(), "" )
	.def( "GoToLeftBrace", &PyMadEdit::GoToLeftBrace, "" )
	.def( "GoToRightBrace", &PyMadEdit::GoToRightBrace, "" )
	.def( "GoBack", &PyMadEdit::GoBack, "" )
	.def( "GoForward", &PyMadEdit::GoForward, "" )
	.def( "SaveToFile", &PyMadEdit::SaveToFile, "" )
	.def( "Reload", &PyMadEdit::Reload, "" )
	.def( "ReloadByModificationTime", &PyMadEdit::ReloadByModificationTime, "" )
	.def( "RestorePosition", &PyMadEdit::RestorePosition, "" )
	.def( "Save", &PyMadEdit::Save, "" )
	.def( "ToggleBookmark", &PyMadEdit::ToggleBookmark, "" )
	.def( "GotoNextBookmark", &PyMadEdit::GotoNextBookmark, "" )
	.def( "GotoPreviousBookmark", &PyMadEdit::GotoPreviousBookmark, "" )
	.def( "ConvertEncoding", &PyMadEdit::ConvertEncoding, "" )
	.def( "ConvertChineseA", &PyMadEdit::ConvertChineseA, "" )
	.def( "HasBOM", &PyMadEdit::HasBOM, "" )
	.def( "ToggleBOM", &PyMadEdit::ToggleBOM, "" )
	.def( "IncreaseDecreaseIndent", &PyMadEdit::IncreaseDecreaseIndent, "" )
	.def( "HasLineComment", &PyMadEdit::HasLineComment, "" )
	.def( "CommentUncomment", &PyMadEdit::CommentUncomment, "" )
	.def( "ToUpperCase", &PyMadEdit::ToUpperCase, "" )
	.def( "ToLowerCase", &PyMadEdit::ToLowerCase, "" )
	.def( "InvertCase", &PyMadEdit::InvertCase, "" )
	.def( "Capitalize", &PyMadEdit::Capitalize, "" )
	.def( "TrimTrailingSpaces", &PyMadEdit::TrimTrailingSpaces, "" )
	.def( "TrimLeadingSpaces", &PyMadEdit::TrimLeadingSpaces, "" )
	.def( "DeleteEmptyLinesWithSpaces", &PyMadEdit::DeleteEmptyLinesWithSpaces, "" )
	.def( "DeleteEmptyLines", &PyMadEdit::DeleteEmptyLines, "" )
	.def( "JoinLines", &PyMadEdit::JoinLines, "" )
	.def( "SortLines", &PyMadEdit::SortLines, "" )
	.def( "ConvertWordWrapToNewLine", &PyMadEdit::ConvertWordWrapToNewLine, "" )
	.def( "ConvertNewLineToWordWrap", &PyMadEdit::ConvertNewLineToWordWrap, "" )
	.def( "ConvertSpaceToTab", &PyMadEdit::ConvertSpaceToTab, "" )
	.def( "ConvertTabToSpace", &PyMadEdit::ConvertTabToSpace, "" )
	.def( "CopyAsHexString", &PyMadEdit::CopyAsHexString, "" )
	.def( "CopyRevertHex", &PyMadEdit::CopyRevertHex, "" )
	.def( "WordCount", &PyMadEdit::WordCount, return_value_policy<return_by_value>(), "" )
	.def( "SetColumnSelection", &PyMadEdit::SetColumnSelection, "" )
	.def( "SetFontA", &PyMadEdit::SetFontA, "Doc" )
	.def( "CopyToClipboardA", &PyMadEdit::CopyToClipboardA, "" )
	.def( "CopyToClipboardB", &PyMadEdit::CopyToClipboardB, "" )
	.def( "ScrollLineUp", &PyMadEdit::ScrollLineUp, "" )
	.def( "ScrollLineDown", &PyMadEdit::ScrollLineDown, "" )
	.def( "ScrollPageUp", &PyMadEdit::ScrollPageUp, "" )
	.def( "ScrollPageDown", &PyMadEdit::ScrollPageDown, "" )
	.def( "ScrollLeft", &PyMadEdit::ScrollLeft, "" )
	.def( "ScrollRight", &PyMadEdit::ScrollRight, "" )
	.def( "ColumnAlignLeft", &PyMadEdit::ColumnAlignLeft, "" )
	.def( "ColumnAlignRight", &PyMadEdit::ColumnAlignRight, "" )
	.def( "Astyle", &PyMadEdit::Astyle, "" )
	.def( "XMLFormat", &PyMadEdit::XMLFormat, "" )
	.def( "Markdown2Html", &PyMadEdit::Markdown2Html, "" )
	.def( "Html2PlainText", &PyMadEdit::Html2PlainText, "" )
	.def( "SelectWholeLine", &PyMadEdit::SelectWholeLine, "" )
	.def( "ToggleBookmarkInSearch", &PyMadEdit::ToggleBookmarkInSearch, "Doc string" )
	.def( "GetIndentCountByPos", &PyMadEdit::GetIndentCountByPos, "" )
	.def( "CutDelBookmarkedLines", &PyMadEdit::CutDelBookmarkedLines, "" )
	.def( "DeleteUnmarkedLines", &PyMadEdit::DeleteUnmarkedLines, "" )
	.def( "CopyUnmarkedLines", &PyMadEdit::CopyUnmarkedLines, "" )
	.def( "CutUnmarkedLines", &PyMadEdit::CutUnmarkedLines, "" )
	.def( "ReplaceBookmarkedLines", &PyMadEdit::ReplaceBookmarkedLines, "" )
	.def( "FindTextNext", &PyMadEdit::FindTextNext, FindTextNext_member_overloads( args( "text", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "FindTextPrevious", &PyMadEdit::FindTextPrevious, FindTextPrevious_member_overloads( args( "text", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "FindHexNext", &PyMadEdit::FindHexNext, FindHexNext_member_overloads( args( "hexstr", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "FindHexPrevious", &PyMadEdit::FindHexPrevious, FindHexPrevious_member_overloads( args( "hexstr", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "ReplaceText", &PyMadEdit::ReplaceText, ReplaceText_member_overloads( args( "expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "ReplaceTextNoDoubleCheck", &PyMadEdit::ReplaceTextNoDoubleCheck, ReplaceText_member_overloads( args( "expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "ReplaceHex", &PyMadEdit::ReplaceHex, ReplaceHex_member_overloads( args( "expr", "fmt" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "ReplaceTextAll", &PyMadEdit::ReplaceTextAll, ReplaceTextAll_member_overloads( args( "expr", "fmt", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "ReplaceHexAll", &PyMadEdit::ReplaceHexAll, ReplaceHexAll_member_overloads( args( "expr", "fmt", "rangeFrom", "rangeTo" ), "Doc string" )[return_value_policy<return_by_value>()] )
#ifndef PYMADEDIT_DLL
	.def( "FindTextAll", &PyMadEdit::FindTextAll, FindTextAll_member_overloads( args( "expr", "bRegex", "bCaseSensitive", "bWholeWord", "bDotMatchNewline", "showresults" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "FindHexAll", &PyMadEdit::FindHexAll, FindHexAll_member_overloads( args( "expr", "showresults" ), "Doc string" )[return_value_policy<return_by_value>()] )
#endif
	.def( "LoadFromFile", &PyMadEdit::LoadFromFile, LoadFromFile_member_overloads( args( "filename", "encoding" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "ToHalfWidth", &PyMadEdit::ToHalfWidth, ToHalfWidth_member_overloads( args( "ascii", "japanese", "korean", "other" ), "Doc string" ) )
	.def( "ToFullWidth", &PyMadEdit::ToFullWidth, ToFullWidth_member_overloads( args( "ascii", "japanese", "korean", "other" ), "Doc string" ) )
	.def( "SetCaretPosition", &PyMadEdit::SetCaretPosition, SetCaretPosition_member_overloads( args( "pos", "selbeg", "selend" ), "Doc string" ) )
	.def( "GetLine", &PyMadEdit::GetLine, GetLine_member_overloads( args( "line", "maxlen", "ignoreBOM" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "SetSelection", &PyMadEdit::SetSelection, SetSelection_member_overloads( args( "beginpos", "endpos", "bCaretAtBeginPos" ), "Doc string" ) )
	.def( "GetText", &PyMadEdit::GetText, GetText_member_overloads( args( "ignoreBOM" ), "Doc string" )[return_value_policy<return_by_value>()] )
	.def( "CopyCutDeleteUnmarkedLines", &PyMadEdit::CopyCutDeleteUnmarkedLines, CopyCutDeleteUnmarkedLines_member_overloads( args( "copyLines", "deleteLines" ), "Doc string" ) )
	;
	enum_<MadEditCmd>( "MadEditCommand" )
	.value( "None", ecNone )
	.value( "CharFirst", ecCharFirst )
	.value( "CharLast", ecCharLast )
	.value( "CaretCommandFirst", ecCaretCommandFirst )
	.value( "Left", ecLeft )
	.value( "Right", ecRight )
	.value( "Up", ecUp )
	.value( "Down", ecDown )
	.value( "BeginLine", ecBeginLine )
	.value( "EndLine", ecEndLine )
	.value( "BeginDoc", ecBeginDoc )
	.value( "EndDoc", ecEndDoc )
	.value( "PrevPage", ecPrevPage )
	.value( "NextPage", ecNextPage )
	.value( "PrevWord", ecPrevWord )
	.value( "NextWord", ecNextWord )
	.value( "LeftBrace", ecLeftBrace )
	.value( "RightBrace", ecRightBrace )
	.value( "CaretCommandLast", ecCaretCommandLast )
	.value( "SelCommandFirst", ecSelCommandFirst )
	.value( "SelLeft", ecSelLeft )
	.value( "SelRight", ecSelRight )
	.value( "SelUp", ecSelUp )
	.value( "SelDown", ecSelDown )
	.value( "SelBeginLine", ecSelBeginLine )
	.value( "SelEndLine", ecSelEndLine )
	.value( "SelBeginDoc", ecSelBeginDoc )
	.value( "SelEndDoc", ecSelEndDoc )
	.value( "SelPrevPage", ecSelPrevPage )
	.value( "SelNextPage", ecSelNextPage )
	.value( "SelPrevWord", ecSelPrevWord )
	.value( "SelNextWord", ecSelNextWord )
	.value( "SelLeftBrace", ecSelLeftBrace )
	.value( "SelRightBrace", ecSelRightBrace )
	.value( "SelCommandLast", ecSelCommandLast )
	.value( "HighlightWords", ecHighlightWords )
	.value( "SelectAll", ecSelectAll )
	.value( "ScrollLineUp", ecScrollLineUp )
	.value( "ScrollLineDown", ecScrollLineDown )
	.value( "ScrollPageUp", ecScrollPageUp )
	.value( "ScrollPageDown", ecScrollPageDown )
	.value( "ScrollLeft", ecScrollLeft )
	.value( "ScrollRight", ecScrollRight )
	.value( "Return", ecReturn )
	.value( "ReturnNoIndent", ecReturnNoIndent )
	.value( "Tab", ecTab )
	.value( "InsertTabChar", ecInsertTabChar )
	.value( "Delete", ecDelete )
	.value( "BackSpace", ecBackSpace )
	.value( "DelPrevWord", ecDelPrevWord )
	.value( "DelNextWord", ecDelNextWord )
	.value( "CutLine", ecCutLine )
	.value( "DeleteLine", ecDeleteLine )
	.value( "Undo", ecUndo )
	.value( "Redo", ecRedo )
	.value( "Cut", ecCut )
	.value( "Copy", ecCopy )
	.value( "Paste", ecPaste )
	.value( "ToggleInsertMode", ecToggleInsertMode )
	.value( "TextMode", ecTextMode )
	.value( "ColumnMode", ecColumnMode )
	.value( "HexMode", ecHexMode )
	.value( "NoWrap", ecNoWrap )
	.value( "WrapByWindow", ecWrapByWindow )
	.value( "WrapByColumn", ecWrapByColumn )
	.value( "ToggleWindow", ecToggleWindow )
	.value( "IncreaseIndent", ecIncreaseIndent )
	.value( "DecreaseIndent", ecDecreaseIndent )
	.value( "Comment", ecComment )
	.value( "Uncomment", ecUncomment )
	.value( "ToUpperCase", ecToUpperCase )
	.value( "ToLowerCase", ecToLowerCase )
	.value( "InvertCase", ecInvertCase )
	.value( "ToHalfWidth", ecToHalfWidth )
	.value( "ToFullWidth", ecToFullWidth )
	.value( "InsertDateTime", ecInsertDateTime )
	.value( "MouseNotify", ecMouseNotify )
	;
	enum_<MadWordWrapMode>( "MadWordWrapMode" )
	.value( "NoWrap", wwmNoWrap )
	.value( "WrapByWindow", wwmWrapByWindow )
	.value( "WrapByColumn", wwmWrapByColumn )
	;
	enum_<MadEditMode>( "MadEditMode" )
	.value( "TextMode", emTextMode )
	.value( "ColumnMode", emColumnMode )
	.value( "HexMode", emHexMode )
	;
	enum_<MadCaretType>( "MadCaretType" )
	.value( "VerticalLine", ctVerticalLine )
	.value( "HorizontalLine", ctHorizontalLine )
	.value( "Block", ctBlock )
	;
	enum_<MadNewLineType>( "MadNewLineType" )
	.value( "Default", nltDefault )
	.value( "DOS", nltDOS ) /*0D0A*/
	.value( "UNIX", nltUNIX ) /*0A*/
	.value( "MAC", nltMAC ) /*0D*/
	;
	enum_<MadConvertEncodingFlag>( "MadConvertEncodingFlag" )
	.value( "None", cefNone )
	.value( "SC2TC", cefSC2TC ) // Simplified Chinese  ==> Traditional Chinese
	.value( "TC2SC", cefTC2SC ) // Traditional Chinese ==> Simplified Chinese
	.value( "JK2TC", cefJK2TC ) // Japanese Kanji      ==> Traditional Chinese
	.value( "JK2SC", cefJK2SC ) // Japanese Kanji      ==> Simplified Chinese
	.value( "C2JK", cefC2JK ) // Trad&Simp Chinese   ==> Japanese Kanji
	;
	enum_<MadNumberingStepType>( "MadNumberingStepType" )
	.value( "Linear", mnstLinear )
	.value( "Exponential", mnstExponential )
	;
	enum_<MadNumberFormat>( "MadNumberFormat" )
	.value( "DEC", nfDEC )
	.value( "HEX", nfHEX )
	.value( "BIN", nfBIN )
	.value( "OCT", nfOCT )
	;
	enum_<MadNumberAlign>( "MadNumberAlign" )
	.value( "Left", naLeft )
	.value( "Right", naRight )
	;
	enum_<MadMsgBoxStyle>( "MadDlgStyle" )
	.value( "YES",                      MAD_YES )
	.value( "OK",                       MAD_OK )
	.value( "NO",                       MAD_NO )
	.value( "CANCEL",                   MAD_CANCEL )
	.value( "APPLY",                    MAD_APPLY )
	.value( "CLOSE",                    MAD_CLOSE )
	.value( "NO_DEFAULT",               MAD_NO_DEFAULT )/* only valid with wxYES_NO */ 
	.value( "CANCEL_DEFAULT",           MAD_CANCEL_DEFAULT )/* only valid with wxCANCEL */ 
	.value( "ICON_WARNING_EXCLAMATION", MAD_ICON_WARNING_EXCLAMATION )
	.value( "ICON_ERROR_HAND_STOP",     MAD_ICON_ERROR_HAND_STOP )
	.value( "ICON_QUESTION",            MAD_ICON_QUESTION )
	.value( "ICON_INFORMATION",         MAD_ICON_INFORMATION )
	.value( "ICON_NONE",                MAD_ICON_NONE )
	.value( "CENTRE",                   MAD_CENTRE )
	;
	enum_<MadMsgBoxRet>( "MadMsgBoxRet" )
	.value( "OK", MADRET_OK )
	.value( "CANCEL", MADRET_CANCEL )
	.value( "APPLY", MADRET_APPLY )
	.value( "YES", MADRET_YES )
	.value( "NO", MADRET_NO )
	;
	def( "MsgBox", &MsgBox, MsgBox_overloads( args( "message", "caption", "style" ), "Doc string" ) );
	def( "InputBox", &InputBox, InputBox_overloads( args( "message", "caption" ), "Doc string" ) );
}
#endif //__MADPYTHON__
