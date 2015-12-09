///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditFrame.h
// Description: Main frame of MadEdit
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDITFRAME_H_
#define _MADEDITFRAME_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
	#pragma hdrstop
#endif
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
////Header Include End

#include <wx/arrstr.h>
#include <wx/frame.h>
#include <wx/docview.h>
#include <wx/fileconf.h>
#include <wx/hashset.h>
#include <wx/imaglist.h>
#include <wx/treectrl.h>

#include <wx/aui/aui.h> // wxAUI
#if __cplusplus <= 199711L
	#include <boost/shared_ptr.hpp>
	using boost::shared_ptr;
#else
	#include <memory>
	using std::shared_ptr;
#endif

////Frame Style Start
#undef MadEditFrame_STYLE
#define MadEditFrame_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Frame Style End

class wxMadAuiNotebook;
class wxAuiNotebookEvent;
class MadEdit;
class MadTreeCtrl;
class MadRecentList;
class wxSpellCheckEngineInterface;
class wxHtmlWindow;

enum MadToolBarType
{
	tbSTANDARD = 0,
	tbEDITOR,
	tbSEARCHREPLACE,
	tbTEXTVIEW,
	tbEDITMODE,
	tbMACRO,
	tbQSEARCH,
	tbMAX,
};

class MadEditFrame : public wxFrame
{
private:
	DECLARE_EVENT_TABLE()
public:
	MadEditFrame( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT( "MadEdit-Mod" ),
	              const wxPoint& pos = wxDefaultPosition,
	              const wxSize& size = wxDefaultSize,
	              long style = MadEditFrame_STYLE );
	virtual ~MadEditFrame();
public:
	//Do not add custom Control Declarations here.
	//wx-devcpp will remove them. Try adding the custom code
	//after the block.
	////GUI Control Declaration Start
	wxMenuBar *WxMenuBar1;
	wxAuiToolBar *WxToolBar[tbMAX];
	wxAuiToolBar *m_QuickSeachBar;
	wxAuiToolBar *m_RefreshView;
	wxStatusBar *WxStatusBar1;
	////GUI Control Declaration End
public:
	//Note: if you receive any error with these enums, then you need to
	//change your old form code that are based on the #define control ids.
	//It may replace a numeric value in the enums names.
	static const long ID_RECENTFINDTEXT1;	 // must be the same with MadSearchDialog
	static const long ID_RECENTFINDTEXT20;
	enum
	{
		////GUI Enum Control ID Start
		ID_WXSTATUSBAR1 = 3003,
		ID_WXTOOLBAR1 = 3005, //NextValue should be (ID_WXTOOLBAR1+tbTOOLBAR_MAX)
		////GUI Enum Control ID End

		ID_NOTEBOOK = ( ID_WXTOOLBAR1 + tbMAX ), // for wxAuiNotebook m_Notebook
		ID_OUTPUTNOTEBOOK,
		ID_FINDINFILESRESULTS,
		//ID_WXTOOLBARQUICKSEARCH,
		ID_QUICKSEARCH,
		ID_QUICKSEARCHWHOLEWORD,
		ID_QUICKSEARCHREGEX,
		ID_QUICKSEARCHCASESENSITIVE,
		ID_QUICKSEARCHDOTMATCHNEWLINE,

		ID_DUMMY_VALUE_ //Dont Delete this DummyValue
	}; //End of Enum

public:
	wxMadAuiNotebook *m_Notebook;
	int            m_NewFileCount;
	wxConfigBase  *m_Config;
	wxImageList   *m_ImageList;
	MadRecentList *m_RecentFiles;
	MadRecentList *m_RecentEncodings;
	MadRecentList *m_RecentFonts;

	wxAuiManager   m_AuiManager; // wxAUI
	wxAuiNotebook *m_InfoNotebook; //
	//wxTreeCtrl *m_FindInFilesResults;
	MadTreeCtrl   *m_FindInFilesResults;
	wxHtmlWindow *m_HtmlPreview;
	int            m_PreviewType;
	wxComboBox    *m_QuickSearch;
	wxCheckBox    *m_CheckboxWholeWord;
	wxCheckBox    *m_CheckboxRegEx;
	wxCheckBox    *m_CheckboxDotMatchNewline;
	wxCheckBox    *m_CheckboxCaseSensitive;
	bool           m_ReloadFiles;
	bool           m_PurgeHistory;
	bool           m_SearchDirectionNext;
	bool           m_ToolbarStatus[tbMAX + 1];
	void OnUpdateUI_MenuFile_CheckCount( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuFileReload( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuFileRecentFiles( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuEditUndo( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditRedo( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEdit_CheckSelSize( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditCut( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditCopy( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditPaste( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditDelete( wxUpdateUIEvent& event );
	void OnUpdateUI_Menu_CheckSize( wxUpdateUIEvent& event );
	void OnUpdateUI_Menu_CheckTextFileSize( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditStartEndSelction( wxUpdateUIEvent& event );

	// add: gogo, 21.09.2009
	void OnUpdateUI_MenuEditToggleBookmark( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditCheckBookmark( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuEditCheckBookmarkWritable( wxUpdateUIEvent& event );

	void OnUpdateUI_Menu_CheckTextFile( wxUpdateUIEvent& event );
	void OnUpdateUI_Menu_CheckWritableTextFile( wxUpdateUIEvent& event );
	void OnUpdateUI_Menu_CheckColumnMode( wxUpdateUIEvent& event );
	void OnUpdateUI_Menu_JoinLines( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuEditCopyAsHexString( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuIndent( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuComment( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuSearchGoToBrace( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuViewEncoding( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewSyntax( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewFontName( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewFontSize( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewFixedWidthMode( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewTabColumn( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewLineSpacing( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewPreview( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewPreviewList( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewNoWrap( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewWrapByWindow( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewWrapByColumn( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewDisplayLineNumber( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewDisplayBookmark( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewDisplay80ColHint( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewShowEndOfLine( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewShowTabChar( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewShowSpaceChar( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewShowAllChars( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewRightToLeft( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewMarkActiveLine( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewMarkBracePair( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewTextMode( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewColumnMode( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewHexMode( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewSpellChecker( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuSpellIgnore( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuSpellAdd2Dict( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuSpellRemoveFromDict( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewToolbars( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewToolbarsToggleAll( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewToolbarList( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuToolsByteOrderMark( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsNewLineChar( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsInsertNewLineChar( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuCheckWritable( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsConvertEncoding( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuWindow_CheckCount( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuToolsStartRecMacro( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsStopRecMacro( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsPlayRecMacro( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsSaveRecMacro( wxUpdateUIEvent& event );

	void OnFileNew( wxCommandEvent& event );
	void OnFileOpen( wxCommandEvent& event );
	void OnFileSave( wxCommandEvent& event );
	void OnFileSaveAs( wxCommandEvent& event );
	void OnFileSaveAll( wxCommandEvent& event );
	void OnFileReload( wxCommandEvent& event );
	void OnFileClose( wxCommandEvent& event );
	void OnFileCloseAll( wxCommandEvent& event );
	void OnFileCloseAllButThis( wxCommandEvent& event );
	void OnFileCloseAllToTheLeft( wxCommandEvent& event );
	void OnFileCloseAllToTheRight( wxCommandEvent& event );
	void OnFilePageSetup( wxCommandEvent& event );
	void OnFilePrintPreview( wxCommandEvent& event );
	void OnFilePrint( wxCommandEvent& event );
	void OnFileRecentFile( wxCommandEvent& event );
	void OnFileExit( wxCommandEvent& event );
	void OnCopyFilePath( wxCommandEvent& event );
	void OnCopyFileName( wxCommandEvent& event );
	void OnCopyFileDir( wxCommandEvent& event );
	void OnFileToggleReadOnly( wxCommandEvent& event );

	void OnEditUndo( wxCommandEvent& event );
	void OnEditRedo( wxCommandEvent& event );
	void OnEditCut( wxCommandEvent& event );
	void OnEditCopy( wxCommandEvent& event );
	void OnEditPaste( wxCommandEvent& event );
	void OnEditDelete( wxCommandEvent& event );
	void OnEditCutLine( wxCommandEvent& event );
	void OnEditDeleteLine( wxCommandEvent& event );
	void OnEditSelectAll( wxCommandEvent& event );
	void OnEditStartEndSelction( wxCommandEvent& event );
	void OnEditInsertTabChar( wxCommandEvent& event );
	void OnEditInsertDateTime( wxCommandEvent& event );

	// add: gogo, 21.09.2009
	void OnSearchToggleBookmark( wxCommandEvent& event );
	void OnSearchGotoNextBookmark( wxCommandEvent& event );
	void OnSearchGotoPreviousBookmark( wxCommandEvent& event );
	void OnSearchClearAllBookmarks( wxCommandEvent& event );
	//----------

	void OnEditSortAscending( wxCommandEvent& event );
	void OnEditSortDescending( wxCommandEvent& event );
	void OnEditSortAscendingCase( wxCommandEvent& event );
	void OnEditSortDescendingCase( wxCommandEvent& event );
	void OnEditSortByOptions( wxCommandEvent& event );
	void OnEditSortOptions( wxCommandEvent& event );

	void OnEditCopyAsHexString( wxCommandEvent& event );
	void OnEditCopyAsHexStringWithSpace( wxCommandEvent& event );
	void OnEditCopyRevertHex( wxCommandEvent& event );
	void OnEditIncIndent( wxCommandEvent& event );
	void OnEditDecIndent( wxCommandEvent& event );
	void OnEditComment( wxCommandEvent& event );
	void OnEditUncomment( wxCommandEvent& event );
	void OnEditWordWrapToNewLine( wxCommandEvent& event );
	void OnEditNewLineToWordWrap( wxCommandEvent& event );
	void OnEditToUpperCase( wxCommandEvent& event );
	void OnEditToLowerCase( wxCommandEvent& event );
	void OnEditInvertCase( wxCommandEvent& event );
	void OnEditCapitalize( wxCommandEvent& event );
	void OnEditToHalfWidth( wxCommandEvent& event );
	void OnEditToHalfWidthByOptions( wxCommandEvent& event );
	void OnEditToFullWidth( wxCommandEvent& event );
	void OnEditToFullWidthByOptions( wxCommandEvent& event );
	void OnEditTabToSpace( wxCommandEvent& event );
	void OnEditSpaceToTab( wxCommandEvent& event );
	void OnEditTrimTrailingSpaces( wxCommandEvent& event );
	void OnEditTrimLeadingSpaces( wxCommandEvent& event );
	void OnEditDeleteEmptyLines( wxCommandEvent& event );
	void OnEditDeleteEmptyLinesWithSpaces( wxCommandEvent& event );
	void OnEditJoinLines( wxCommandEvent& event );
	void OnEditInsertNumbers( wxCommandEvent& event );
	void OnEditColumnAlignLeft( wxCommandEvent& event );
	void OnEditColumnAlignRight( wxCommandEvent& event );
	void OnEditSpellCheck( wxCommandEvent& event );
	void OnEditBookmarkCopy( wxCommandEvent& event );
	void OnEditBookmarkCut( wxCommandEvent& event );
	void OnEditBookmarkDel( wxCommandEvent& event );
	void OnEditBookmarkDelUnmarked( wxCommandEvent& event );
	void OnEditBookmarkReplace( wxCommandEvent& event );

	void OnSearchFind( wxCommandEvent& event );
	void OnSearchFindNext( wxCommandEvent& event );
	void OnSearchFindPrevious( wxCommandEvent& event );
	void OnSearchReplace( wxCommandEvent& event );
	void OnSearchFindInFiles( wxCommandEvent& event );
	void OnSearchShowFindInFilesResults( wxCommandEvent& event );
	void OnSearchGoToLine( wxCommandEvent& event );
	void OnSearchGoToPosition( wxCommandEvent& event );
	void OnSearchGoToLeftBrace( wxCommandEvent& event );
	void OnSearchGoToRightBrace( wxCommandEvent& event );
	void OnSearchQuickFind( wxCommandEvent& event );
	void OnSearchQuickFindNext( wxCommandEvent& event );
	void OnSearchQuickFindPrevious( wxCommandEvent& event );
	void OnShowSearchQuickFindBar( wxCommandEvent& event );

	void OnViewEncoding( wxCommandEvent& event );
	void OnViewRecentEncoding( wxCommandEvent& event );
	void OnViewSyntax( wxCommandEvent& event );
	void OnViewFontName( wxCommandEvent& event );
	void OnViewRecentFont( wxCommandEvent& event );
	void OnViewFontSize( wxCommandEvent& event );
	void OnViewSetFont( wxCommandEvent& event );
	void OnViewFixedWidthMode( wxCommandEvent& event );
	void OnViewLineSpacing( wxCommandEvent& event );
	void OnViewTabColumn( wxCommandEvent& event );
	void OnViewPreview( wxCommandEvent& event );
	void OnRefreshPreview( wxCommandEvent& event );
	void OnClosePreview( wxCommandEvent& event );
	void OnViewNoWrap( wxCommandEvent& event );
	void OnViewWrapByWindow( wxCommandEvent& event );
	void OnViewWrapByColumn( wxCommandEvent& event );
	void OnViewDisplayLineNumber( wxCommandEvent& event );
	void OnViewDisplayBookmark( wxCommandEvent& event );
	void OnViewDisplay80ColHint( wxCommandEvent& event );
	void OnViewShowEndOfLine( wxCommandEvent& event );
	void OnViewShowTabChar( wxCommandEvent& event );
	void OnViewShowSpaceChar( wxCommandEvent& event );
	void OnViewShowAllChars( wxCommandEvent& event );
	void OnViewRightToLeft( wxCommandEvent& event );
	void OnViewMarkActiveLine( wxCommandEvent& event );
	void OnViewMarkBracePair( wxCommandEvent& event );
	void OnViewTextMode( wxCommandEvent& event );
	void OnViewColumnMode( wxCommandEvent& event );
	void OnViewHexMode( wxCommandEvent& event );
	void OnViewSpellChecker( wxCommandEvent& event );
	void OnSpellCheckIgnore( wxCommandEvent& event );
	void OnSpellAdd2Dict( wxCommandEvent& event );
	void OnSpellCheckRemoveFromDict( wxCommandEvent& event );
	void OnViewToolBarsToggleAll( wxCommandEvent& event );
	void OnViewToolbars( wxCommandEvent& event );

	void OnToolsOptions( wxCommandEvent& event );
	void OnToolsHighlighting( wxCommandEvent& event );
#ifdef __WXMSW__
	void OnToolsFileAssociation( wxCommandEvent& event );
#endif
	void OnToolsPurgeHistories( wxCommandEvent& event );
	void OnToolsRunTempMacro( wxCommandEvent& event );
	void OnToolsRunMacroFile( wxCommandEvent& event );
	void OnToolsStartRecMacro( wxCommandEvent& event );
	void OnToolsStopRecMacro( wxCommandEvent& event );
	void OnToolsPlayRecMacro( wxCommandEvent& event );
	void OnToolsSaveRecMacro( wxCommandEvent& event );
	void OnToolsMadScriptList( wxCommandEvent& event );
	void OnToolsEditMacroFile( wxCommandEvent& event );

	void OnToolsToggleBOM( wxCommandEvent& event );
	void OnToolsConvertToDOS( wxCommandEvent& event );
	void OnToolsConvertToMAC( wxCommandEvent& event );
	void OnToolsConvertToUNIX( wxCommandEvent& event );
	void OnToolsInsertDOS( wxCommandEvent& event );
	void OnToolsInsertMAC( wxCommandEvent& event );
	void OnToolsInsertUNIX( wxCommandEvent& event );
	void OnToolsConvertEncoding( wxCommandEvent& event );
	void OnToolsSimp2TradChinese( wxCommandEvent& event );
	void OnToolsTrad2SimpChinese( wxCommandEvent& event );
	void OnToolsKanji2TradChinese( wxCommandEvent& event );
	void OnToolsKanji2SimpChinese( wxCommandEvent& event );
	void OnToolsChinese2Kanji( wxCommandEvent& event );
	void OnToolsSimp2TradClipboard( wxCommandEvent& event );
	void OnToolsTrad2SimpClipboard( wxCommandEvent& event );
	void OnToolsKanji2TradClipboard( wxCommandEvent& event );
	void OnToolsKanji2SimpClipboard( wxCommandEvent& event );
	void OnToolsChinese2KanjiClipboard( wxCommandEvent& event );
	void OnToolsWordCount( wxCommandEvent& event );
	void OnToolsMarkdown2Html( wxCommandEvent& event );
	void OnToolsHtml2PlainText( wxCommandEvent& event );
	void OnToolsAstyleFormat( wxCommandEvent& event );
	void OnToolsXMLFormat( wxCommandEvent& event );

	void OnWindowToggleWindow( wxCommandEvent& event );
	void OnWindowPreviousWindow( wxCommandEvent& event );
	void OnWindowNextWindow( wxCommandEvent& event );

	void OnHelpAbout( wxCommandEvent& event );

	void OnCopyCurrResult( wxCommandEvent& event );
	void OnCopyAllResults( wxCommandEvent& event );
	void OnResetCurrResult( wxCommandEvent& event );
	void OnRightClickToolBar( wxAuiToolBarEvent& event );
	void OnContextMenu( wxContextMenuEvent& event );
private:
	bool m_PageClosing; // prevent from reentry of CloseFile(), OnNotebookPageClosing()
public:
	int OpenedFileCount();
	void OpenFile( const wxString &filename, bool mustExist ); // if filename is empty, open a new file
	void RunScriptWithFile( const wxString &filename, const wxString &script, bool mustExist, bool closeafterdone, bool ignorereadonly );
	void CloseFile( int pageId );
	void CloseAllFiles( bool force );
	void MadEditFrameKeyDown( wxKeyEvent& event );
	void SetPageFocus( int pageId );
	MadEdit *GetEditByFileName( const wxString &filename, int &id );
	void ResetAcceleratorTable();
	void HideQuickFindBar();

protected:
	void MadEditFrameClose( wxCloseEvent& event );
	void CreateGUIControls( void );

	void OnNotebookPageChanging( wxAuiNotebookEvent& event );
	void OnNotebookPageChanged( wxAuiNotebookEvent& event );
	void OnNotebookPageClosing( wxAuiNotebookEvent& event );
	void OnNotebookPageClosed( bool bZeroPage = false ); //wxAuiNotebookEvent& event); //wxAUI doesn't support this event
	void OnNotebookPageRightUp( wxAuiNotebookEvent& event );

	void OnSize( wxSizeEvent &evt );
	//void OnChar(wxKeyEvent& evt);
	void OnActivate( wxActivateEvent &evt );

	bool QueryCloseFile( int idx );
	bool QueryCloseAllFiles();

	void LoadMenuKeys( wxConfigBase *config );
	wxString GetMenuKey( const wxString &menu, const wxString &defaultkey );

	void OnInfoNotebookSize( wxSizeEvent &evt );
	void OnFindInFilesResultsDClick( wxMouseEvent& event );

#ifdef __WXMSW__
	WXLRESULT MSWWindowProc( WXUINT message, WXWPARAM wParam, WXLPARAM lParam );
#endif

public:
	void ResetFindInFilesResults();
	void AddItemToFindInFilesResults( const wxString &text, size_t index, const wxString &filename, int pageid, const wxFileOffset &begpos, wxFileOffset &endpos );

	void PurgeRecentFiles();
	void PurgeRecentFonts();
	void PurgeRecentEncodings();
private:
	enum MadMacroMode
	{ emMacroStopped = 0, emMacroRecoding, emMacroRunning };
	MadMacroMode m_MadMacroStatus;
	wxArrayString m_MadMacroScripts;
	int m_LastSelBeg, m_LastSelEnd;
public:
	MadMacroMode GetMadMacroStatus() {return m_MadMacroStatus;}
	bool IsMacroRunning() {return ( m_MadMacroStatus == emMacroRunning );}
	bool IsMacroRecording() {return ( m_MadMacroStatus == emMacroRecoding );}
	bool IsMacroStopped() {return ( m_MadMacroStatus == emMacroStopped );}
	void SetMacroRunning() {m_MadMacroStatus = emMacroRunning;}
	void SetMacroRecording() {m_MadMacroStatus = emMacroRecoding;}
	void SetMacroStopped() {m_MadMacroStatus = emMacroStopped;}
	void AddMacroScript( const wxString & script, int caretPos = 0, int selBeg = -1, int selEnd = -1 ) {
		if( ( ( selBeg != -1 ) && ( selEnd != -1 ) ) && ( selBeg != m_LastSelBeg || selEnd != m_LastSelEnd ) ) {
			m_LastSelBeg = selBeg;
			m_LastSelEnd = selEnd;
			m_MadMacroScripts.Add( wxString::Format( wxT( "SetSelection(%d, %d, True)" ), m_LastSelBeg, m_LastSelEnd ) );
		}

		m_MadMacroScripts.Add( script );
	}
	bool HasRecordedScript() {return ( m_MadMacroScripts.GetCount() > 2 );}
	wxArrayString& GetRecordedScripts() {return m_MadMacroScripts;}
};

enum   // menu id
{
	menuNew = wxID_NEW,
	menuOpen = wxID_OPEN,
	menuSave = wxID_SAVE,
	menuSaveAs = wxID_SAVEAS,
	menuClose = wxID_CLOSE,
	menuCloseAll = wxID_CLOSE_ALL,
	menuPageSetup = wxID_PRINT_SETUP,
	menuPrintPreview = wxID_PREVIEW,
	menuPrint = wxID_PRINT,
	menuExit = wxID_EXIT,
	menuUndo = wxID_UNDO,
	menuRedo = wxID_REDO,
	menuCut = wxID_CUT,
	menuCopy = wxID_COPY,
	menuPaste = wxID_PASTE,
	menuDelete = wxID_DELETE,
	menuSelectAll = wxID_SELECTALL,
	menuFind = wxID_FIND,
	menuReplace = wxID_REPLACE,
	menuAbout = wxID_ABOUT,

	// file
	menuSaveAll = 1100,
	menuReload,
	menuRecentFiles,
	menuCopyFilePath,
	menuCopyFileName,
	menuCopyFileDir,
	menuCloseAllButThis,
	menuCloseAllToTheLeft,
	menuCloseAllToTheRight,

	// edit
	menuCutLine,
	menuDeleteLine,
	menuInsertTabChar,
	menuInsertDateTime,
	menuBookmark,
	menuBookmarkCopy,
	menuBookmarkCut,
	menuBookmarkDel,
	menuBookmarkDelUnmarked,
	menuBookmarkReplace,
	menuSort,
	menuSortAscending,
	menuSortDescending,
	menuSortAscendingCase,
	menuSortDescendingCase,
	menuSortByOptions,
	menuSortOptions,
	menuAdvanced,
	menuToggleReadOnly,
	menuStartEndSelction,

	menuCopyAsHexString,
	menuCopyAsHexStringWithSpace,
	menuCopyRevertHex,
	menuIncreaseIndent,
	menuDecreaseIndent,
	menuComment,
	menuUncomment,
	menuWordWrapToNewLine,
	menuNewLineToWordWrap,
	menuToUpperCase,
	menuToLowerCase,
	menuInvertCase,
	menuCapitalize,
	menuToHalfWidth,
	menuToHalfWidthByOptions,
	menuToFullWidth,
	menuToFullWidthByOptions,
	menuTabToSpace,
	menuSpaceToTab,
	menuTrimTrailingSpaces,
	menuTrimLeadingSpaces,
	menuDeleteEmptyLines,
	menuDeleteEmptyLinesWithSpaces,
	menuJoinLines,
	menuInsertNumbers,
	menuColumnAlignLeft,
	menuColumnAlignRight,

	// search
	menuFindNext,
	menuFindPrevious,
	menuFindInFiles,
	menuShowQuickFindBar,
	menuQuickFindNext,
	menuQuickFindPrevious,
	menuShowFindInFilesResults,
	menuGoToLine,
	menuGoToPosition,
	menuLeftBrace,
	menuRightBrace,
	menuToggleBookmark,       // add: gogo, 21.09.2009
	menuGotoNextBookmark,     // add: gogo, 21.09.2009
	menuGotoPreviousBookmark, // add: gogo, 21.09.2009
	menuClearAllBookmarks,

	// view
	menuEncoding,
	menuAllEncodings,
	menuEncodingGroup1,
	menuEncodingGroup99 = menuEncodingGroup1 + 98,

	menuEncoding1,
	menuEncoding99 = menuEncoding1 + 98,

	menuRecentEncoding1,
	menuRecentEncoding9 = menuRecentEncoding1 + 8,

	menuSyntax,
	menuSyntax1,
	menuSyntax199 = menuSyntax1 + 198,

	menuFontName,
	menuFontName1,
	menuFontName999 = menuFontName1 + 998,

	menuFont0, //[@]
	menuFont1, //[ABCDE]
	menuFont2, //[FGHIJ]
	menuFont3, //[KLMNO]
	menuFont4, //[PQRST]
	menuFont5, //[UVWXYZ]
	menuFont6, //[Other]

	menuRecentFonts,
	menuRecentFont1,
	menuRecentFont9 = menuRecentFont1 + 8,

	menuFontSize,
	menuFontSize1,
	menuFontSize99 = menuFontSize1 + 98,

	menuSetFont,
	menuFixedWidthMode,

	menuTabColumn,
	menuTabColumn1,
	menuTabColumn16 = menuTabColumn1 + 15,

	menuLineSpacing,
	menuLineSpacing100,
	menuLineSpacing250 = menuLineSpacing100 + 30,

	menuNoWrap,
	menuWrapByWindow,
	menuWrapByColumn,
	menuDisplayLineNumber,
	menuDisplayBookmark,
	menuDisplay80ColHint,
	menuShowEndOfLine,
	menuShowTabChar,
	menuShowSpaceChar,
	menuViewRightToLeft,
	menuPreview,
	menuPreview1,
	menuPreview16 = menuPreview1 + 15,
	menuRefreshPreview,
	menuClosePreview,
	menuShowAllChars,
	menuMarkActiveLine,
	menuMarkBracePair,
	menuTextMode,
	menuColumnMode,
	menuHexMode,
	menuSpellChecker,
	menuSpellIgnore,
	menuSpellAdd2Dict,
	menuSpellRemoveFromDict,
	menuSpellOption1,
	menuSpellOption99 = menuSpellOption1 + 98,
	// ToolBar
	menuToolBars,
	menuToolBarsToggleAll,
	menuToolBar1,
	menuToolBar99 = menuToolBar1 + 98,

	// tools
	menuOptions,
	menuHighlighting,
#ifdef __WXMSW__
	menuFileAssociation,
#endif
	menuPurgeHistories,
	menuMadMacro,
	menuRunTempMacro,
	menuRunMacroFile,
	menuStartRecMacro,
	menuStopRecMacro,
	menuPlayRecMacro,
	menuSaveRecMacro,
	menuMadScriptList,
	menuEditMacroFile,
	menuMadScrip1,
	menuMadScrip1000 = menuMadScrip1 + 999, //1000 scripts, enough?
	menuPlugins,
	menuByteOrderMark,
	menuToggleBOM,
	menuNewLineChar,
	menuConvertToDOS,
	menuConvertToMAC,
	menuConvertToUNIX,
	menuInsertNewLineChar,
	menuInsertDOS,
	menuInsertMAC,
	menuInsertUNIX,
	menuConvertEncoding,
	menuConvertChineseChar,
	menuSimp2TradChinese,
	menuTrad2SimpChinese,
	menuKanji2TradChinese,
	menuKanji2SimpChinese,
	menuChinese2Kanji,
	menuSimp2TradClipboard,
	menuTrad2SimpClipboard,
	menuKanji2TradClipboard,
	menuKanji2SimpClipboard,
	menuChinese2KanjiClipboard,
	menuMarkdown2Html,
	menuHtml2PlainText,
	menuTextConvFormatter,
	menuAstyleFormat,
	menuXMLFormat,
	menuWordCount,

	// window
	menuToggleWindow,
	menuPreviousWindow,
	menuNextWindow,

	// results
	menuCopyCurResult,
	menuCopyAllResults,
	menuResetCurResult,
};

enum MadPreviewType
{
	ptPREVIEW_NONE,
	ptPREVIEW_HTML = menuPreview1,
	ptPREVIEW_MARKDOWN,
	ptPREVIEW_MAXTYPE,
	//MAX::menuPreview16
};
extern MadEditFrame *g_MainFrame;
extern void OnReceiveMessage( const wchar_t *msg, size_t size );

class MadTreeCtrl : public wxTreeCtrl
{
public:
	MadTreeCtrl() {}
	MadTreeCtrl( wxWindow *parent, const wxWindowID id,
	             const wxPoint& pos, const wxSize& size,
	             long style );
	virtual ~MadTreeCtrl() {};
	void OnItemMenu( wxTreeEvent& event );
protected:
	void ShowMenu( wxTreeItemId id, const wxPoint& pt );
private:
	// NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
	//     if you want your overloaded OnCompareItems() to be called.
	//     OTOH, if you don't want it you may omit the next line - this will
	//     make default (alphabetical) sorting much faster under wxMSW.
	//DECLARE_DYNAMIC_CLASS(MadTreeCtrl)
	DECLARE_EVENT_TABLE()
};

#endif

