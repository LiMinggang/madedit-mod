                        =================================
                               MadEdit v0.2.9 mod
                        =================================
About:
-----
MadEdit is a cross-platform Text/Hex Editor written in C++ & wxWidgets.
MadEdit supports many useful functions, e.g. SyntaxHighlightings, WordWraps,
Encodings, Column/Hex Modes, and a Plugin system(not work yet).
In HexMode, MadEdit can open large files which size is up to 32GB (INT_MAX*16).


Supported Platforms:
-------------------
A. Linux, FreeBSD, and Unix-like OS (__WXGTK__):
   a. GNU C++ 3.x/4.x:
      Required Libraries to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. Boost.Xpressive latest version

B. MS Windows (__WXMSW__):
   a. MinGW32/GNU C++ 3.x (wxDevCpp):
      Required Libraries to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled(wxWidgets-2.8.12 is recommanded)
      2. Boost-1.33.0 or higher
      3. Boost.Xpressive latest version
      4. libunicows: it's optional under WinNT/XP, but required under Win98
   b. Visual C++ 7.1 (VS.Net 2003):
      Required Libraries to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher(Source code is enough for MadEdit)
      3. Boost.Xpressive latest version
      4. libunicows: it's optional under WinNT/XP, but required under Win98
      5. Set WXWIN and BOOST to the root directory of your local ones

P.S.: If you use CVS version of Boost, Boost.Xpressive has been included in the CVS HEAD,
      or you must get 'xpressive.zip' at:
      http://www.boost-consulting.com/vault/index.php?directory=Strings%20-%20Text%20Processing
P.S.2: Since MadEdit v0.2.2, Xpressive is attached in MadEdit sources package.

P.S.3 wxAUIBook patch http://trac.wxwidgets.org/attachment/ticket/10848/


Syntax files, Locale files and Settings:
---------------------------------------
  Syntax files: in the path $(MadEditConfigPath)/syntax/
  Locale files: in the path $(MadEditConfigPath)/locale/

  $(MadEditConfigPath) are the one of below paths:
    1. MadEdit execution file directory : Windows and Linux
    2. $(HOME)/.madedit/ : Linux
    3. /usr/share/madedit/ : Linux

  If MadEdit does not find any files in those paths,
  MadEdit can also execute standalone by using default settings.

  If you are under Win98, you must get unicows.dll from MS's website:
  http://www.microsoft.com/downloads/details.aspx?FamilyId=73BA7BD7-ED06-4F0D-80A4-2A7EEAEE17E2&displaylang=en

  When quit MadEdit, MadEdit will save the settings to:
    1. $(HOME)/.madedit/madedit.cfg: Linux
    2. $(MadEdit execution file directory)/MadEdit.cfg: Windows
  And FontWidth.dat will also be saved to the same path.
  FontWidth.dat is Cache of Font-Width-Data, it can speed-up MadEdit a lot.

Mod v0.2.2
1. Add "Insert increamental numbers" to edit popup menu
2. Column Align(Alpha, Known issue: crash with tab misked with spaces.)
3. Add attribute check in file change detect. The file would be marked as modified
if it was deleted, changed attributes.
4. Improve the code of InsertColumnString
5. Colorful ReadOnly and InsertMode

Mod v0.2.1
1. Support Insert in HexMode(Toggleable)
2. Paste as insert or replace according to InsertMode
3. Replace in all mode by paste
4. Fixed issue: Purge history while existing not working
5. Fixed issue: Tab to indent while selecting more than 2 lines not working

Mod v0.2.0
New features from 0.1.x
1. Single Instance feature don't compatable with 0.1.x
2. MadPython based on Python 2.7.8 as automation macro(Edit/Record/Run/Save/Load)
3. Insert incremantal numbers
3. Purge history during exiting
4. Right click on file tab supported
5. Close to right/left
6. Fixed bugs found including hang issue caused by empty (), [] and {} in regluar expression

Mod v0.2.0
New to beta 4
1. Changed error message of Options Dialog
2. Add Incremental numbering feature in Edit->Advanced for column numbering at current caret pos
3. Use InsertString for InsertStr for performance
4. Export InsertIncrementalNumber to MadPython
5. Improve MadMacro dialog
6. Update Simplifed Chinese
Waiting for bug report of MadPython and Incremental numbering:-P

Mod v0.2.0 beta 4
New to beta 3
1. Replace wxRichTextCtrl with wxTextCtrl
2. Add Macro debug mode, default off
3. Close to clear output instead of clear on Run
4. Add some default button for several dialogs
5. URL changed to sourceforge.org

Mod v0.2.0 beta 3
This would be 0.2.0 if no critical issue found
1. Fixed Known issue(wxStyledTextCtrl bug in beta 2) by replacing wxStyledTextCtrl with MadEdit
2. Fixed MadPython insert char issue in column mode
3. Change behave while string has illegal zero. ie. change 0x00(NUL) to 0x2400 uncode NUL
4. Fixed Caret issue of search/replace(introduced by MadEdit): Caret would blink on wrong place, which 
would be shown in right place if you iconize/restore the window
5. Added MadPython syntax file and MadPython API added in the file

Mod v0.2.0 beta 2
Known issue: MadPython not work well in column mode, eg. insert char in column mode
1. Fixed wxAUIBook bug with patch(http://trac.wxwidgets.org/attachment/ticket/10848/)
2. Add Close All To The Left/Right feature
3. Add default sting in MadMacroDlg, say, medit = MadEdit()
4. Fixed hang issue caused by empty (), [] and {} in regluar expression
5. More missed commands was recorded, eg, SetEditMode

Mod v0.2.0 beta 2014.9.12
Warning: This is a Preview for 0.1.5, MadPython was not fully tested. Save your work before try MadPython
New to 0.1.5 beta
1. Replace the ugly code for single instance support with the IPC of wxWidgets
2. Change the version number to 0.2.0 because of #1, which would not work with 0.1.x as Single Instance.
3. Fixed critical issue issue introduced by supporting record macro in ProcessCommand(!)
4. Fixed issue of extra script because of Search/Replace dialog using MadEdit as TextEdit
5. Fixed issue of updating StartRecord on Toolbar
6. Fixed issue of duplicated Find/Replace generated
7. Fixed issue of loading script with unicode characters, eg, unicode in comments
8. Update translation of Simplified Chinese

Mod v0.1.5 beta 2014.9.11
Warning: This is a Preview for 0.1.5, MadPython was not fully tested. Save your work before try MadPython
New to alpha 4
1. Add new image for Run TempMacro
2. Change the MadMacro dialog to resizable
3. Use scripts/ as the local script library directory. The scripts(*.mpy) under the directory would be
listed on the menu.
4. Use "####" as prefix of help string for the script in the first line of the script which would be shown at the
status bar
6. Remove and add new APIs in MadPython

Mod v0.1.5 alpha 4 2014.9.10
Warning: This is a Preview for 0.1.5, MadPython was not fully tested. Save your work before try MadPython
New to alpha 3
Fixed known issue: SetCaretPosition would get wrong selection beginning pos and duplicated SetCaretPosition

Mod v0.1.5 alpha 3 2014.9.10
Warning: This is a Preview for 0.1.5, MadPython was not fully tested. Save your work before try MadPython
New to alpha 2
1. Fixed Assertion on wxEVT_MOUSE_CAPTURE_LOST
2. Record/Stop/Playpack/SaveMacro is done(Known issue: SetCaretPosition would get wrong selection beginning
pos and duplicated SetCaretPosition). Not well tested.
3. Added more apis
4. Updated MadPython.txt
5. Add syntax support for MadMacro(.mpy)
6. Updated translation for Simplified Chinese.
7. SaveRecord Icon is to be updated.

Mod v0.1.5 alpha 2014.9.5
Warning: This is a Preview for 0.1.5, MadPython was not fully tested. Save your work before try MadPython

1. Replace the ugly code with wxSingleInstanceChecker for singleton check(Todo)
2. Embed minimized Python as Macro language(Only MS Windows supported currently)[Tools->Macros]
3. Improve behaviour of Search and SearchInFiles
4. Show search results on status bar
5. Recoding and Playback(Todo)
6. Save script/connected with shortcut(?)(Todo)
7. Right click on file tab supported(Alpha 2)

Mod v0.1.4 2014.6.29
1. Add clear history function(search/replace history, recent file/font history) from WMedit
2. Update Simplifed/Traditional Chinese translation
3. Partially upadate/add translation of Italian/Japanese/Spanish/Russion? based on WMedit(Anyone can help on this? contact minggang.li@gmail.com)
4. Use wxWidgets 2.8.12 because 3.0.1 still has random hang issue

Mod v0.1.3 2014.6.23
Nothing changed. Rebuid with wxWidgets 3.0.1 which is a bug fix of 3.0. So far so good.

Mod v0.1.2
1. Adding extra checking code for UTF8(From CodeBlocks)
2. New Encoding CP855(https://sourceforge.net/p/madedit/discussion/421172/thread/4e901148/)
3. Add FreeBasic Syntax(wxMedit)


2014.02.13
1. Adding extra checking code for UTF8(From CodeBlocks)
2. New Encoding CP855(https://sourceforge.net/p/madedit/discussion/421172/thread/4e901148/)
3. Release 0.1.2

2014.02.05
1. Using VS 2013 Express for compiling and fix some unsafe code.
2. Update wxWidgets to 3.0
3. Update boost to 1.55.0
4. Change name to MadEdit-Mod, starting version 0.1.1
5. Add FreeBasic Syntax


2014.01.26
1. Added support for open file and go to #line by command line arguments, use "FileName*linnum".
   '*' can not be used as filename in all OS for it's a wildcard char.
   This feature can be used for multiple files open function in Madedit, eg.
   "FileName1*linnum1|FileName2*linnum2|FileName3*linnum3|..."

2012.07.05
1.Fixed issues in Search/Replace in Selection
2.Removed the confusing From/To on Search/Replace dialog

ChangeLog:
v0.2.9 Mod
1.Fixed several critical/crash bugs in Madedit 0.2.9.
2.New features added.
    Drag&Drop Edit
    Bookmark
    Highlight words(shift+F8)
    Triple click to select whole line
    Search all in current file

----------
v0.2.9:
1.added that restore the encoding and font(name, size) of a file when reopening it.
2.added that use Ctrl+MouseWheel to decrease or increase the font size.
3.added Japanese language. Thank Tilt for the translations.
4.fixed a bug when converting between Tab and Space char in wordwrap mode.
5.modified that the dialogs will stay on top of main window, not top of all windows.
6.modified the action when associating file types.
7.some minor changes.

v0.2.8:
1.added Japanese Kanji supporting for converting Chinese characters.
2.added the function to split window into multiple Tab Controls (by using wxAuiNotebook)
  ScreenShot: http://madedit.sourceforge.net/screenshot06.png
3.added the function to convert between WordWraps and NewLine chars.
4.added File-Type Associations Dialog under win32.
5.added Search-In-Selection function when searching and replaceing.
6.added Count function in SearchDialog.
7.added ToHalfWidthByOptions, ToFullWidthByOptions to choose the characters(ASCII,Japanese,Korean)
  you want to convert.
8.added ConvertSpaceToTab and ConvertTabToSpace functions.
9.added new syntax files for D, SQL, Verilog and VHDL.
10.fixed that cannot replace text in Hex-Mode.
11.many bug-fixes and changes.

v0.2.7:
1.fixed that ISO-8859-7 charset doesn't available under Win32.
2.fixed that incorrectly handle Japanese halfwidth characters in Shift-JIS charset.
3.supported XIM Over-The-Spot feature under Linux/FreeBSD(wxGTK).
4.added options to set Date/Time format.
5.added a option to reload files previously open on startup.
6.added a option to restore previous caret position when files are opened.
7.fixed a crashing bug that incorrectly detect MAC/DOS new-line char on HTML/PHP files.
8.changed for showing CR, LF, CRLF and EOF in different marks.

v0.2.6:
1.improved the redraw speed under Linux (wxGTK).
2.added charset-detector, a port to C of "universalchardet", for detecting the encoding of files.
  Thanks Jserv (http://blog.linux.org.tw/~jserv/archives/001672.html).
3.improved the speed of FindInFiles function.
4.fixed crashing bug when opening a file begins with a 0x0b/0x0c character.
5.added new syntax files for Fortran, TeX/LaTeX, Squirrel, C#, Visual Basic, ASP(VBScript).
6.many misc changes and bug-fixes.

v0.2.5:
1.fixed crashes when copying and pasting data between TextMode and ColumnMode under Linux.
2.fixed sometimes crashes when using FindInFiles function under Linux.
3.added a dockable Information Window for listing the results of FindInFiles.
4.added Exclude Filters in FindInFiles dialog.
5.added a button ("<<") to get the path of the editing document in FindInFiles dialog.
6.added support for multiple shortcuts on all menu and editor commands.
7.added support for Mouse-Over-Get-Word of StarDict under Win32.
8.added Trim Trialing Spaces function.
9.added Sort Lines function.
10.added a new option to temporary disable or enable MouseSelectToCopy function when pressing Ctrl key.
11.added a new option to choose the Language of UI.
12.added Italian language. Thank Contardi Angelo for the translations.

v0.2.4-2:
1.fixed that lost the last byte of clipboard data (Linux version only).

v0.2.4:
1.added custom Syntax Highlighting settings.
2.added Find/Replace In Files function.
3.some minor changes.

v0.2.3:
1.improved the loading speed for the files contain U+10000~U+10FFFF widechars.
2.improved the repaint speed when the lines are very long(>1MB).
3.added ToHalfWidth, ToFullWidth functions.
4.added WordCount function.
5.added InsertDataTime function.
6.added ecReturnNoIndent(Shift-Enter) to insert NewLine char without indentation.
7.now 'ESC' key can be used in custom keybindings.
8.some minor changes.

v0.2.2:
1.fixed that menu icons will cause crash under Win98.
2.fixed that input incorrect CJK chars under Win98.
3.fixed that copy and then paste data in Column/Hex modes will cause crash under Win98.
4.added a messagebox to notify that madedit cannot find the matched string.
5.added ToUpperCase, ToLowerCase, InvertCase functions.
6.some misc changes.

v0.2.1-2:
1.fixed that it will crash when close ConvertEncodingDialog by pressing top-right 'X' button.
2.fixed the bug of AutoCompletePair function.

v0.2.1:
1.fixed copy/paste text to/from PrimarySelection when use mouse under Linux.
2.added ConvertEncoding function.
3.added ConvertChineseChar(Simplified<=>Traditional) in both of file and clipboard.
4.added AutoCompletePair function. The AutoCompletePair must be defined in syntax files.
5.improved AutoIndent function. The IndentChar and UnindentChar must be defined in syntax files.

v0.2.0:
1.fixed syntax parsing error in non-global range(ex: PHP, JavaScript).
2.fixed that reopen the same file under Win32 because compare filename with case sensitive.
3.added new option of custom KeyBindings.
4.added MouseSelectToCopy and MiddleMouseToPaste functions.
5.added new syntax files for CSS, Windows Registry File, XML.

v0.1.9.9:
1.fixed the incorrect searching result on some search patterns.
2.added SaveAll function.
3.some misc changes.

v0.1.9:
1.replaced Boost.Regex with Boost.Xpressive for searching & replacing Regular Expressions.
  now we can use wxDevCpp with the precompiled wxWidgets library(*.devpak, Unicode version)
  on www.devpaks.org to build MadEdit under win32.
2.added a new function for Multi-Line Editing in Column Mode.
3.several misc changes & bug-fixs.

v0.1.8:
1.fixed that randomly crash when quit MadEdit for Linux version.
2.fixed EscapeChar syntax checking.
3.fixed that Print/Preview incorrectly when the position of Horizontal-ScrollBar is not 0.
4.added a new option of 'Default Encoding'.
5.added automatically detect the encoding of FileName under Linux by current locale.
  When save a file, MadEdit will use UTF8 as default encoding of the FileName.
6.added new syntax files for diff/patch, JavaScript and Ruby.

v0.1.7:
1.fixed several misc bugs.
2.fixed MultiByte Encoding <==> Unicode converting error in v0.1.6 under Linux.
3.removed the linking to the unnecessary libs(e.g. libtiff, libjpeg and libpng) for Linux version.
4.added Print&PrintPreview functions. (Known bug: cannot print/preview correctly on the papers
 with landscape orientation.)
5.added Comment(Ctrl-M)/Uncomment(Ctrl-Shift-M) functions. MadEdit will add/remove LineComment
 string to the selected lines, the LineComment must be defined in the syntax files.

v0.1.6:
1.fixed that cannot search U+10000~U+10FFFF ucs4 char under win32.
2.added Increase&Decrease Indent functions.
When select multi-lines and press 'Tab' will Inc-Indent, and press 'Shift-Tab' will Dec-Indent.

v0.1.5:
1.fixed several bugs.
2.now MadEdit will open the same files once only.
3.improved Reload() function, and MadEdit will reload the files which were changed by another app.

v0.1.4:
1.fixed the bug of Mark Brace incorrect position when use WordWrap.
2.fixed several misc. bugs.
3.used a new approach to determine file types:
  (a)by file extension (b)by the context of first line (c)by file name.
4.added a setting of "Do Not Save Settings to MadEdit.cfg after MadEdit Finish",
  it's useful when you edit 'MadEdit.cfg' by MadEdit.

v0.1.3:
1.fixed several bugs.
2.added Mark Brace Pair.
3.added Go To Left/Right Brace (Ctrl-[ and Ctrl-], press Shift to select).
  the Brace-Pair must be defined in syntax_*.syn files.
4.added Cut Line (Ctrl-Shift-L).
5.added Insert Tab Char (Ctrl-~ or Ctrl-`).
6.supported Drop Files from window-manager under Linux.
7.improved the speed of repaint in HexMode.

v0.1.2:
1.fixed several bugs.
2.added Replace Text/Hex functions.
3.added some fine improvements.

v0.1.1:
  fixed some bugs.
  added RightClick Popup Menu.
  added new syntax files.
  added FindPrevious & FindHexNext/Previous functions.
  changed ShortCuts of Text/Column/HexMode to Alt-1, Alt-2, Alt-3.

v0.1:
  added Edit & Search functions.

v0.0.9:
  Preview & AlphaTest version.


License:
-------
MadEdit is released under the GNU General Public License.


Author:
------
Alston Chen <madedit@gmail.com>
Minggang Li <minggang.li@gmail.com>

Links:
-----
MadEdit: http://sourceforge.net/projects/madedit/
MadEdit-Mod: http://sourceforge.net/projects/madedit-mod/
wxWidgets: http://www.wxwidgets.org
wxDevCpp: http://wxdsgn.sourceforge.net
Boost: http://www.boost.org
Open Clip Art Library: http://www.openclipart.org

