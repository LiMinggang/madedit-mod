///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditFrame.h
// Description: Main frame of MadEdit
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDITFRAME_H_
#define _MADEDITFRAME_H_
#include "MadUtils.h"

#if CPLUSEPLUSE98
	#include <boost/shared_ptr.hpp>
	using boost::shared_ptr;
#else
	#include <memory>
	using std::shared_ptr;
#endif

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

////Frame Style Start
#undef MadEditFrame_STYLE
#define MadEditFrame_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Frame Style End

extern wxUniChar g_MadConfigSeparator;
extern wxUniChar g_MadBmSeparator;
extern wxUniChar g_MadEscParameter;
extern wxUniChar g_Delimiter;

#define MAX_MADSCRIPT_LOAD  200
#define MAX_RECENTFILS      20
#define MAX_RECENTSEARCHS   20
#define MAX_RECENTFONTS     9
#define MAX_RECENTENCODINGS 9

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
	tbFONTENCODING,
	tbSEARCHREPLACE,
	tbTEXTVIEW,
	tbEDITMODE,
	tbMACRO,
	tbQSEARCH,
	tbMAX,
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
	menuSaveAll = 1000,
    menuSaveACopy,
	menuReload,
	menuRecentFiles,
	menuRecentFilesList,
	menuRecentFilesToolbar,
	menuCopyFilePath,
	menuCopyFileName,
	menuCopyFileNameNoExt,
	menuCopyFileDir,
	menuOpenFileDir,
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
	menuBookmarkCopyUnmarked,
	menuBookmarkCutUnmarked,
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
	menuShowFindInFilesResults,
    menuCollapseAllResults,
    menuGoToLine,
	menuGoToPosition,
    menuGoBack,
    menuGoForward,
	menuLeftBrace,
	menuRightBrace,
	menuToggleBookmark,       // add: gogo, 21.09.2009
	menuGotoNextBookmark,     // add: gogo, 21.09.2009
	menuGotoPreviousBookmark, // add: gogo, 21.09.2009
	menuClearAllBookmarks,

	// view
	menuAlwaysOnTop,
	menuFullScreen,
    menuPostIt,
	menuEncoding,
	menuAllEncodings,
	menuEncodingGroup1,
	menuEncodingGroup32 = menuEncodingGroup1 + 31,

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
	menuToolBarsLockPos,
	menuToolBar1,
	menuToolBar99 = menuToolBar1 + 98,
	menuMenuBarToggle,

	// tools
	menuOptions,
	menuHighlighting,
#ifdef __WXMSW__
	menuFileAssociation,
#endif
	menuSourceFormatter,
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
	menuMadScrip200 = menuMadScrip1 + (MAX_MADSCRIPT_LOAD -1), //200 scripts, enough?
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
	menuWindowList,
	menuWindow1,
	menuWindow100 = menuWindow1 + 99,

	// results
	menuCopyCurResult,
	menuCopyAllResults,
	menuResetResult,
	menuDeleteCurResult,
    menuCollapseCurResult,

	menuTypewriterMode,
	menuShowQuickSearchBar,
	menuQuickFindNext,
	menuQuickFindPrevious = menuQuickFindNext+1,
    menuQuickFindWholeWord = menuQuickFindNext+2,
    menuQuickFindRegex = menuQuickFindNext+3,
    menuQuickFindCase = menuQuickFindNext+4,
    menuQuickFindDotMatchNewLine = menuQuickFindNext+5,

    menuVScrollHere,
    menuVScrollTop,
    menuVScrollBottom,
    menuVScrollPageUp,
    menuVScrollPageDown,
    menuVScrollUp,
    menuVScrollDown,
    menuHScrollHere,
    menuHScrollLeftEdge,
    menuHScrollRightEdge,
    menuHScrollPageLeft,
    menuHScrollPageRight,
    menuHScrollLeft,
    menuHScrollRight,
    menuIncFontSize,
    menuDecFontSize,
	menuMAXMENUITEMID,
};

class MadEditFrame : public wxFrame
{
public:
	MadEditFrame( wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxT( "MadEdit-Mod" ),
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
	wxAuiToolBar *m_QuickSearchBar;
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
		ID_WXSTATUSBAR1 = menuMAXMENUITEMID,
		ID_WXTOOLBAR1, //NextValue should be (ID_WXTOOLBAR1+tbTOOLBAR_MAX)
		////GUI Enum Control ID End

		ID_NOTEBOOK = ( ID_WXTOOLBAR1 + tbMAX ), // for wxAuiNotebook m_Notebook
		ID_OUTPUTNOTEBOOK,
		ID_FINDINFILESRESULTS,
		//ID_WXTOOLBARQUICKSEARCH,
		ID_ENCODINGS,
		ID_SYNTAXS,
		ID_FONTS,
		ID_FONTSIZES,
		ID_LINESPACES,
		ID_QUICKSEARCH,
		ID_QUICKSEARCHWHOLEWORD,
		ID_QUICKSEARCHREGEX,
		ID_QUICKSEARCHCASESENSITIVE,
		ID_QUICKSEARCHDOTMATCHNEWLINE,

		ID_DUMMY_VALUE_ //Dont Delete this DummyValue
	}; //End of Enum

public:
	wxMadAuiNotebook *m_Notebook;
	long           m_FullScreenStyle;
	int            m_NoteBookTabHeight;
	int            m_NewFileCount;
	wxConfigBase  *m_Config;
	wxImageList   *m_ImageList;
	MadRecentList *m_RecentFiles;
	MadRecentList *m_RecentEncodings;
	MadRecentList *m_RecentFonts;

	wxAuiManager  m_AuiManager; // wxAUI
	wxAuiNotebook *m_InfoNotebook; //
	//wxTreeCtrl *m_FindInFilesResults;
	MadTreeCtrl   *m_FindInFilesResults;
	wxHtmlWindow  *m_HtmlPreview;
	int            m_PreviewType;
	wxComboBox    *m_QuickSearch;
	wxComboBox    *m_Encodings;
	wxComboBox    *m_Syntaxs;
	wxComboBox    *m_Fonts;
	wxComboBox    *m_FontSizes;
	wxComboBox    *m_LineSpaces;
	bool           m_ReloadFiles;
	bool           m_PurgeHistory;
	bool           m_ResetToolBars;
	bool           m_SearchDirectionNext;
    wxFileOffset   m_QSearchBegPos, m_QSearchEndPos;
private:
	void OnUpdateUI_CheckFrameStyle( wxUpdateUIEvent& event );
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
	void OnUpdateUI_MenuFile_Readonly( wxUpdateUIEvent& event );

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

	void OnUpdateUI_MenuSearchGoBack( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuSearchGoForward( wxUpdateUIEvent& event );

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
	void OnUpdateUI_MenuViewToolbarsToggleAll( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewToolbarsLockPosition( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewMenuBarToggle( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewToolbarList( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuViewTypewriterMode( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuToolsByteOrderMark( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsNewLineChar( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsInsertNewLineChar( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuCheckWritable( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuCheckIsThisModified( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuCheckIsAnyoneModified( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsConvertEncoding( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuWindow_CheckCount( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuWindow_Window( wxUpdateUIEvent& event );

	void OnUpdateUI_MenuToolsStartRecMacro( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsStopRecMacro( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsPlayRecMacro( wxUpdateUIEvent& event );
	void OnUpdateUI_MenuToolsSaveRecMacro( wxUpdateUIEvent& event );
	
	void OnUpdateUI_MenuSearch_QuickBar( wxUpdateUIEvent& event );

	void OnFileNew( wxCommandEvent& WXUNUSED(event) );
	void OnFileOpen( wxCommandEvent& WXUNUSED(event) );
	void OnFileSave( wxCommandEvent& WXUNUSED(event) );
	void OnFileSaveAs( wxCommandEvent& WXUNUSED(event) );
	void OnFileSaveAll( wxCommandEvent& WXUNUSED(event) );
	void OnFileSaveACopy( wxCommandEvent& WXUNUSED(event) );
	void OnFileReload( wxCommandEvent& WXUNUSED(event) );
	void OnRecentFilesList( wxCommandEvent& WXUNUSED(event) );
	void OnRecentFilesPop( wxCommandEvent& WXUNUSED(event) );
	void OnFileClose( wxCommandEvent& WXUNUSED(event) );
	void OnFileCloseAll( wxCommandEvent& WXUNUSED(event) );
	void OnFileCloseAllButThis( wxCommandEvent& WXUNUSED(event) );
	void OnFileCloseAllToTheLeft( wxCommandEvent& WXUNUSED(event) );
	void OnFileCloseAllToTheRight( wxCommandEvent& WXUNUSED(event) );
	void OnFilePageSetup( wxCommandEvent& WXUNUSED(event) );
	void OnFilePrintPreview( wxCommandEvent& WXUNUSED(event) );
	void OnFilePrint( wxCommandEvent& WXUNUSED(event) );
	void OnFileRecentFile( wxCommandEvent& event );
	void OnFileExit( wxCommandEvent& WXUNUSED(event) );
	void OnCopyFilePath( wxCommandEvent& WXUNUSED(event) );
	void OnCopyFileName( wxCommandEvent& event );
	void OnCopyFileDir( wxCommandEvent& WXUNUSED(event) );
	void OnOpenFileDir( wxCommandEvent& WXUNUSED(event) );
	void OnFileToggleReadOnly( wxCommandEvent& WXUNUSED(event) );

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

	// add: gogo, 21.09.2009
	void OnSearchToggleBookmark( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGotoNextBookmark( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGotoPreviousBookmark( wxCommandEvent& WXUNUSED(event) );
	void OnSearchClearAllBookmarks( wxCommandEvent& WXUNUSED(event) );
	//----------

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
	void OnEditBookmarkCopy( wxCommandEvent& WXUNUSED(event) );
	void OnEditBookmarkCut( wxCommandEvent& WXUNUSED(event) );
	void OnEditBookmarkDel( wxCommandEvent& WXUNUSED(event) );
	void OnEditBookmarkCopyUnmarked( wxCommandEvent& WXUNUSED(event) );
	void OnEditBookmarkCutUnmarked( wxCommandEvent& WXUNUSED(event) );
	void OnEditBookmarkDelUnmarked( wxCommandEvent& WXUNUSED(event) );
	void OnEditBookmarkReplace( wxCommandEvent& WXUNUSED(event) );

	void OnSearchFind( wxCommandEvent& WXUNUSED(event) );
	void OnSearchFindNext( wxCommandEvent& WXUNUSED(event) );
	void OnSearchFindPrevious( wxCommandEvent& WXUNUSED(event) );
	void OnSearchReplace( wxCommandEvent& WXUNUSED(event) );
	void OnSearchFindInFiles( wxCommandEvent& WXUNUSED(event) );
	void OnSearchShowFindInFilesResults( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGoToLine( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGoToPosition( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGoBack( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGoForward( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGoToLeftBrace( wxCommandEvent& WXUNUSED(event) );
	void OnSearchGoToRightBrace( wxCommandEvent& WXUNUSED(event) );
	void OnSearchQuickFind( wxCommandEvent& event );
	void OnSearchQuickFindNext( wxCommandEvent& event );
	void OnSearchQuickFindPrevious( wxCommandEvent& event );
	void OnShowQuickSearchBar( wxCommandEvent& WXUNUSED(event) );

    void OnViewAlwaysOnTop( wxCommandEvent& WXUNUSED(event) );
    void OnViewFullScreen( wxCommandEvent& WXUNUSED(event) );
	void OnViewPostIt( wxCommandEvent& WXUNUSED(event) );
	void OnViewEncoding( wxCommandEvent& WXUNUSED(event) );
	void OnViewRecentEncoding( wxCommandEvent& WXUNUSED(event) );
	void OnViewSyntax( wxCommandEvent& WXUNUSED(event) );
	void OnViewFontName( wxCommandEvent& WXUNUSED(event) );
	void OnViewRecentFont( wxCommandEvent& WXUNUSED(event) );
	void OnViewFontSize( wxCommandEvent& WXUNUSED(event) );
	void OnViewSetFont( wxCommandEvent& WXUNUSED(event) );
	void OnViewFixedWidthMode( wxCommandEvent& WXUNUSED(event) );
	void OnViewLineSpacing( wxCommandEvent& WXUNUSED(event) );
	void OnViewTabColumn( wxCommandEvent& WXUNUSED(event) );
	void OnViewPreview( wxCommandEvent& WXUNUSED(event) );
	void OnRefreshPreview( wxCommandEvent& WXUNUSED(event) );
	void OnClosePreview( wxCommandEvent& WXUNUSED(event) );
	void OnViewNoWrap( wxCommandEvent& WXUNUSED(event) );
	void OnViewWrapByWindow( wxCommandEvent& WXUNUSED(event) );
	void OnViewWrapByColumn( wxCommandEvent& WXUNUSED(event) );
	void OnViewDisplayLineNumber( wxCommandEvent& WXUNUSED(event) );
	void OnViewDisplayBookmark( wxCommandEvent& WXUNUSED(event) );
	void OnViewDisplay80ColHint( wxCommandEvent& WXUNUSED(event) );
	void OnViewShowEndOfLine( wxCommandEvent& WXUNUSED(event) );
	void OnViewShowTabChar( wxCommandEvent& WXUNUSED(event) );
	void OnViewShowSpaceChar( wxCommandEvent& WXUNUSED(event) );
	void OnViewShowAllChars( wxCommandEvent& WXUNUSED(event) );
	void OnViewRightToLeft( wxCommandEvent& WXUNUSED(event) );
	void OnViewMarkActiveLine( wxCommandEvent& WXUNUSED(event) );
	void OnViewMarkBracePair( wxCommandEvent& WXUNUSED(event) );
	void OnViewTextMode( wxCommandEvent& WXUNUSED(event) );
	void OnViewColumnMode( wxCommandEvent& WXUNUSED(event) );
	void OnViewHexMode( wxCommandEvent& WXUNUSED(event) );
	void OnViewSpellChecker( wxCommandEvent& WXUNUSED(event) );
	void OnSpellCheckIgnore( wxCommandEvent& WXUNUSED(event) );
	void OnSpellAdd2Dict( wxCommandEvent& WXUNUSED(event) );
	void OnSpellCheckRemoveFromDict( wxCommandEvent& WXUNUSED(event) );
	void OnViewToolBarsToggleAll( wxCommandEvent& event );
	void OnViewToolBarsLockPosistion( wxCommandEvent& event );
	void OnViewMenuBarToggle( wxCommandEvent& event );
	void OnViewToolbars( wxCommandEvent& WXUNUSED(event) );
	void OnViewTypewriterMode( wxCommandEvent& WXUNUSED(event) );

	void OnFontEncoding( wxCommandEvent& WXUNUSED(event) );
	void OnIncDecFontSize( wxCommandEvent& WXUNUSED(event) );
	void UpdateFontEncoding(  );

	void OnToolsOptions( wxCommandEvent& WXUNUSED(event) );
	void OnToolsHighlighting( wxCommandEvent& WXUNUSED(event) );
#ifdef __WXMSW__
	void OnToolsFileAssociation( wxCommandEvent& WXUNUSED(event) );
#endif
	void OnToolsSourceFormatter( wxCommandEvent& WXUNUSED(event) );
	void OnToolsPurgeHistories( wxCommandEvent& WXUNUSED(event) );
	void OnToolsRunTempMacro( wxCommandEvent& WXUNUSED(event) );
	void OnToolsRunMacroFile( wxCommandEvent& WXUNUSED(event) );
	void OnToolsStartRecMacro( wxCommandEvent& WXUNUSED(event) );
	void OnToolsStopRecMacro( wxCommandEvent& WXUNUSED(event) );
	void OnToolsPlayRecMacro( wxCommandEvent& WXUNUSED(event) );
	void OnToolsSaveRecMacro( wxCommandEvent& WXUNUSED(event) );
	void OnToolsMadScriptList( wxCommandEvent& WXUNUSED(event) );
	void OnToolsEditMacroFile( wxCommandEvent& WXUNUSED(event) );

	void OnToolsToggleBOM( wxCommandEvent& WXUNUSED(event) );
	void OnToolsConvertToDOS( wxCommandEvent& WXUNUSED(event) );
	void OnToolsConvertToMAC( wxCommandEvent& WXUNUSED(event) );
	void OnToolsConvertToUNIX( wxCommandEvent& WXUNUSED(event) );
	void OnToolsInsertDOS( wxCommandEvent& WXUNUSED(event) );
	void OnToolsInsertMAC( wxCommandEvent& WXUNUSED(event) );
	void OnToolsInsertUNIX( wxCommandEvent& WXUNUSED(event) );
	void OnToolsConvertEncoding( wxCommandEvent& WXUNUSED(event) );
	void OnToolsSimp2TradChinese( wxCommandEvent& WXUNUSED(event) );
	void OnToolsTrad2SimpChinese( wxCommandEvent& WXUNUSED(event) );
	void OnToolsKanji2TradChinese( wxCommandEvent& WXUNUSED(event) );
	void OnToolsKanji2SimpChinese( wxCommandEvent& WXUNUSED(event) );
	void OnToolsChinese2Kanji( wxCommandEvent& WXUNUSED(event) );
	void OnToolsSimp2TradClipboard( wxCommandEvent& WXUNUSED(event) );
	void OnToolsTrad2SimpClipboard( wxCommandEvent& WXUNUSED(event) );
	void OnToolsKanji2TradClipboard( wxCommandEvent& WXUNUSED(event) );
	void OnToolsKanji2SimpClipboard( wxCommandEvent& WXUNUSED(event) );
	void OnToolsChinese2KanjiClipboard( wxCommandEvent& WXUNUSED(event) );
	void OnToolsWordCount( wxCommandEvent& WXUNUSED(event) );

	void OnWindowPreviousWindow( wxCommandEvent& WXUNUSED(event) );
	void OnWindowNextWindow( wxCommandEvent& WXUNUSED(event) );
	void OnWindowWindowList( wxCommandEvent& WXUNUSED(event) );
	void OnWindowWindowActivate( wxCommandEvent& WXUNUSED(event) );

	void OnHelpAbout( wxCommandEvent& WXUNUSED(event) );

    void CollapseAllResults( wxCommandEvent& WXUNUSED(event) );
	void OnCopyCurrResult( wxCommandEvent& WXUNUSED(event) );
	void OnCopyAllResults( wxCommandEvent& WXUNUSED(event) );
	void OnResetResult( wxCommandEvent& WXUNUSED(event) );
	void OnDeleteCurrResult( wxCommandEvent& WXUNUSED(event) );
	void OnCollapseCurrResult( wxCommandEvent& WXUNUSED(event) );
    void OnRightClickToolBar( wxAuiToolBarEvent& event );
    void OnQuickSearchSetFocus( wxFocusEvent& event );
	void OnContextMenu( wxContextMenuEvent& event );
	void OnScrollBarMenu( wxCommandEvent& WXUNUSED(event) );
public:
	void OnWindowToggleWindow( wxCommandEvent& WXUNUSED(event) );
	void OnToolsMarkdown2Html( wxCommandEvent& WXUNUSED(event) );
	void OnToolsHtml2PlainText( wxCommandEvent& WXUNUSED(event) );
	void OnToolsAstyleFormat( wxCommandEvent& WXUNUSED(event) );
	void OnToolsXMLFormat( wxCommandEvent& WXUNUSED(event) );
private:
	bool m_PageClosing; // prevent from reentry of CloseFile(), OnNotebookPageClosing()
public:
    
	void InitMenu_HScrollPop();
	void InitMenu_VScrollPop();
	size_t OpenedFileCount();
    void ActivateFile(int num);
	bool OpenFile( const wxString &filename, bool mustExist, bool changeSelection = true ); // if filename is empty, open a new file
	void RunScriptWithFile( const wxString &filename, const wxString &script, bool mustExist, bool closeafterdone, bool ignorereadonly, bool activeFile );
	void CloseFile( size_t pageId );
	void CloseAllFiles( bool force );
	void MadEditFrameKeyDown( wxKeyEvent& event );
	void SetPageFocus( size_t pageId );
	int GetPageFocus( );
	MadEdit *GetEditByFileName( const wxString &filename, size_t &id );
	void ResetAcceleratorTable();
	void HideQuickFindBar();
    void SaveFile(size_t pageId, bool saveas = false, bool hideDlg = true);

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

	bool QueryCloseFile( size_t idx );
	bool QueryCloseAllFiles();

	void LoadMenuKeys( wxConfigBase *config );
	wxString GetMenuKey( const wxString &menu, const wxString &defaultkey );

	//void OnInfoNotebookSize( wxSizeEvent &evt );
	void OnFindInFilesResultsDClick( wxMouseEvent& event );
    void HideAllToolBars();
    void ShowAllToolBars();
    void ToggleFullScreen(long style, bool maxmize);
    bool ResetNormalToolBarPos(wxWindow * toolbar, const wxChar * toolname, const wxChar * caption, int row, int xpos = 0);
    bool ResetQuickSearchBarPos();
    bool ResetInformationWinPos();    
    bool RestoreAuiPanel(wxWindow * toolbar, wxString& toobar_status, bool gripper = false);

#ifdef __WXMSW__
	WXLRESULT MSWWindowProc( WXUINT message, WXWPARAM wParam, WXLPARAM lParam );
#endif
	static const long ID_WXTIMER;
public:
	void ResetFindInFilesResults();
	wxTreeItemId & NewSearchSession( const wxString &sessionLabel );
	void AddItemToFindInFilesResults( wxTreeItemId & myroot, const wxString &distext, const wxString &acttext, size_t index, const wxString &filename, int pageid, const wxFileOffset &begpos, wxFileOffset &endpos );

	void PurgeRecentFiles();
	void PurgeRecentFonts();
	void PurgeRecentEncodings();
	void OnTimer(wxTimerEvent& event);
	void ShowSelectionTab();
private:
	enum MadMacroMode
	{ emMacroStopped = 0, emMacroRecoding, emMacroRunning };
	MadMacroMode m_MadMacroStatus;
	wxArrayString m_MadMacroScripts;
	wxFileOffset m_LastSelBeg, m_LastSelEnd;
	wxTimer m_AutoSaveTimer;
	long m_AutoSaveTimout;
    wxMenuItem * m_ToggleReadOnly;
    wxMenuItem * m_SetSelecting;
	static bool m_Closing;

	typedef struct 
	{
		const long evtTag;
		void (MadEditFrame::*method)( wxCommandEvent &);
	} wxCmdEvtHandlerMap_t;
    static wxCmdEvtHandlerMap_t m_menu_evt_map[];
	typedef struct 
	{
		const long evtStartTag;
		const long evtEndTag;
		void (MadEditFrame::*method)( wxCommandEvent &);
	} wxCmdEvtHandlerRangeMap_t;
    static wxCmdEvtHandlerRangeMap_t m_menu_evt_range_map[];
	typedef struct 
	{
		const long evtTag;
		void (MadEditFrame::*method)( wxUpdateUIEvent &);
	} wxUIUpdateEvtHandlerMap_t;
	static wxUIUpdateEvtHandlerMap_t m_menu_ui_updater_map[];
	typedef struct 
	{
		const long evtStartTag;
		const long evtEndTag;
		void (MadEditFrame::*method)( wxUpdateUIEvent &);
	} wxCmdUpdaterRangeMap_t;
    static wxCmdUpdaterRangeMap_t m_menu_ui_updater_range_map[];
	void UpdateControlsColour(const wxColour& colFt, const wxColour& colBg);
public:
	MadMacroMode GetMadMacroStatus() {return m_MadMacroStatus;}
	bool IsMacroRunning() {return ( m_MadMacroStatus == emMacroRunning );}
	bool IsMacroRecording() {return ( m_MadMacroStatus == emMacroRecoding );}
	bool IsMacroStopped() {return ( m_MadMacroStatus == emMacroStopped );}
	void SetMacroRunning() {m_MadMacroStatus = emMacroRunning;}
	void SetMacroRecording() {m_MadMacroStatus = emMacroRecoding;}
	void SetMacroStopped() {m_MadMacroStatus = emMacroStopped;}
	void AddMacroScript( const wxString & script, /*wxFileOffset caretPos = 0,*/ wxFileOffset selBeg = -1, wxFileOffset selEnd = -1 ) {
		if( ( ( selBeg != -1 ) && ( selEnd != -1 ) ) && ( selBeg != m_LastSelBeg || selEnd != m_LastSelEnd ) ) {
			m_LastSelBeg = selBeg;
			m_LastSelEnd = selEnd;
			m_MadMacroScripts.Add( wxString::Format( wxT( "SetSelection(%s, %s, True)" ), ( wxLongLong( m_LastSelBeg ).ToString() ).c_str(), ( wxLongLong( m_LastSelEnd ).ToString() ).c_str() ) );
		}

		m_MadMacroScripts.Add( script );
	}
	bool HasRecordedScript() {return ( m_MadMacroScripts.GetCount() > 0 );}
	wxArrayString& GetRecordedScripts() {return m_MadMacroScripts;}

	friend void OnFontChanged( MadEdit *madedit );
	friend void OnEncodingChanged( MadEdit *madedit );
    friend void OnSyntaxChanged( MadEdit *madedit );
    friend void OnLineSpaceChanged( MadEdit *madedit );
	friend void OnEditSelectionChanged( MadEdit *madedit );
	friend void OnEditStatusChanged( MadEdit *madedit );
	friend void OnEditActivate( MadEdit *WXUNUSED(madedit) );
	friend void OnEditToggleWindow( MadEdit * WXUNUSED(madedit) );
	friend void OnEditMouseRightUp( MadEdit * madedit );
	friend void OnVScrollMouseRightUp( MadEdit *madedit );
	friend void OnHScrollMouseRightUp( MadEdit *madedit );
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
extern void OnReceiveMessage( const wchar_t *msg, size_t WXUNUSED(size), bool activeFile = true );

#define USE_GENERIC_TREECTRL 1
#if USE_GENERIC_TREECTRL
#include "wx/generic/treectlg.h"
//#ifndef wxTreeCtrl
//#define wxTreeCtrl wxGenericTreeCtrl
class MadTreeCtrl : public wxGenericTreeCtrl
//#endif
#else
class MadTreeCtrl : public wxTreeCtrl
#endif
{
public:
	MadTreeCtrl() {}
	MadTreeCtrl( wxWindow *parent, const wxWindowID id,
	             const wxPoint& pos, const wxSize& size,
	             long style );
	virtual ~MadTreeCtrl() {};
	void OnItemMenu( wxTreeEvent& event );
	void OnMouseWheel( wxMouseEvent &evt );
protected:
	void ShowMenu( wxTreeItemId id, const wxPoint& pt );
private:
	// NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
	//     if you want your overloaded OnCompareItems() to be called.
	//     OTOH, if you don't want it you may omit the next line - this will
	//     make default (alphabetical) sorting much faster under wxMSW.
	//DECLARE_DYNAMIC_CLASS(MadTreeCtrl)
};

#endif

