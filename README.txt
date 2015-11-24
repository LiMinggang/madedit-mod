                        =================================
                                  MadEdit Mod
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
      1. wxWidgets-3.0.2 or higher with Unicode and IPC enabled
      2. Boost-1.56.0 or higher(build Boost-Python)

      (install gtk2-devel gtext-devel automake rpm-build)
      copy wxwin.m4 to usr/share/acloXXX/ or install wxGTK-devel

B. MS Windows (__WXMSW__):
   a. MinGW32/GNU C++ 3.x (wxDevCpp):
      Required Libraries to compile:
      1. wxWidgets-3.0.2 or higher with Unicode and IPC enabled
      2. Boost-1.56.0 or higher(build Boost-Python)
   b. Visual C++ 7.1 (VS.Net 2005) or higher:
      Required Libraries to compile:
      1. wxWidgets-3.0.2 or higher with Unicode and IPC enabled
      2. Boost-1.56.0 or higher(build Boost-Python)
      3. Set WXWIN and BOOST to the root directory of your local ones

P.S.: Please put the debug info(.pdb) under the App directory in case there was a 
      crash happened. Paste the call stack and open a ticket
P.S.: Don't forget to Copy wxwin.m4 to /usr/share/aclocal folder if you use your
      own build of wxWidgets
P.S.: Install gettext-devel if you got AM_GNU_GETTEXT error under Linux
P.S.: madedit.pot/po has been uploaded to the top directory. Any translation for
      MadEdit-Mod is welcome.
P.S.: Please open a ticket for bug of MadEdit-Mod at 
      https://github.com/LiMinggang/madedit-mod/issues/

MadEdit_Win2K is a build by VC++ 2005 express for Windows 2000.

Example build under Linux
Boost Python(1.56)
./bjam --with-python include="${HOME}/madedit-mod/minipython/Include" python-debugging=off threading=multi variant=release link=static runtime-link=static stage
wxWidgets(3.0.2)
./configure --enable-unicode --disable-shared --with-libpng=builtin --with-zlib=builtin --with-expat=builtin  --with-gtk3=yes --with-libiconv=no
MadEdit-Mod
./configure --with-boost=${HOME}/boost_1_56_0 --with-wxdir=${HOME}/wxWidgets-3.0.2

Syntax files, Locale files, Dictionaries and Settings:
---------------------------------------
  Syntax files: in the path $(MadEditConfigPath)/syntax/
  Locale files: in the path $(MadEditConfigPath)/locale/
  Dictionaries: in the path $(MadEditConfigPath)/Dictionaries/

  $(MadEditConfigPath) are the one of below paths:
    1. MadEdit execution file directory : Windows and Linux
    2. $(HOME)/.madedit/ : Linux
    3. /usr/share/madedit/ : Linux

  If MadEdit does not find any files in those paths,
  MadEdit can also execute standalone by using default settings.

  When quit MadEdit, MadEdit will save the settings to:
    1. $(HOME)/.madedit/madedit.cfg: Linux
    2. $(MadEdit execution file directory)/MadEdit.cfg: Windows
  And FontWidth.dat will also be saved to the same path.
  FontWidth.dat is Cache of Font-Width-Data, it can speed-up MadEdit a lot.

  * Download dictionary at http://extensions.openoffice.org
    oxt is actually a zip file. So, download it and open it with 7-zip. Then extract
    the .dic and .aff to ./Dictionaries. Re-start MadEdit-Mod to load the dictionary.
  * Use MadEdit-Mod with other applications like CPPCheck, Visual Studio, try
    'madedit %f*lineNum'
  * Silent mode which can be used in scripting, no GUI will be shown
    [-h], help, Displays help on the command line parameters
    [-s], silent, Disables the GUI
    [-x], eXit, Exit MadEdit(mark as done working with previous "Silent" instance! Can also close the UI)
    [-f], force, Edit and save file ignoring the ReadOnly flag
    [-m], madpython, Specify MadPython file to be run on the file
    [-r], recursive, Recursively run on files of subdirectories
    [-w], wildcard, Enable wildcard support in file name\n(line number would be disabled because it used '*')
    [files], File(s) to be opened
    Eg: madedit -s -m madpytho.mpy ./1.txt /tmp/2.txt
    Make sure you use Single Instance with -s, -x!!

Known issues:
The Macro recording function will save all your commands to one document which
means it will not record the "switching" between documents.
Search in selection in column mode is not exact the selection highlighted.

Feature/issues to be released
-----------------------------

ChangeLog:
----------
Note: Not support wxWidgets 2.X anymore since 0.3.X

Todo:
FreeBSD packaging (with FreeBSD 10.01)

Mod v0.3.9
1.Improve on change syntax on save as
2.New: Check available translation at start time and refresh on opening Option dialog(New translation will automatically listed at runtime)
3.New: Add API SetColumnSelection
4.New: Use default script directory if not specified
5.New: Greek translation(Thanks nikoss(https://github.com/nikoss))
6.New: Add Credits and License to About Dialog
7.New: Toggle hide/show replace UI items by checkbox for FindReplaceInFilesDialog
8.New: Add Credits and License information on About dialog
9.New: Add German and Polish translations(based on wxMedit)
Fix: Improve mark brace issue(#15 in 0.3.8)
Fix: Purge recent caret position history
Fix: Typo on Option dialog(Dictionary settings) #82
Fix: Not enough space for translated string(Greek) #83
Fix: Can't convert Chinese(simplified) to Japanese #88
Fix: Update MadPython Syntax
Fix: Update Translations


Mod v0.3.8
1.New: Upgrade boost to 1.59
2.New: Silent mode which can be used in scripting(Only file names would be passed to
     running instance in single instance mode)
     Run MadEdit-Mod without show GUI and use a MadPython to handle the files to
     be opened. So, you can use the Astyle/XML formatter/HTML tools the same as 
     the original ones, eg, call it in a script to handle a lot of files.
     Furthermore, you can use MadPython to do more!!!
     [-h], help, Displays help on the command line parameters
     [-f], force, Edit and save file ignoring the ReadOnly flag(For the files in command line)
     [-s], silent, Disables the GUI(would close the file automatically after done if another instance is running)
     [-x], eXit, Exit MadEdit(mark as done working with previous "Silent" instance! Can also close the UI)
     [-m], madpython, Specify MadPython file to be run on the file
     [-r], recursive, Recursively run on files of subdirectories
     [-w], wildcard, Enable wildcard support in file name(line number would be disabled because it used '*')
     [files], File(s) to be opened
     Eg: madedit -s -m madpytho.mpy ./1.txt /tmp/2.txt
3.New: Automatically advance number for new file in NoNameXX if already opened a file with the same name
4.New: Change insert mode to insertion while changing from hex to text mode
5.New: Use indent size of current line instead of the last line that has non-space/tab char
6.New: Add Spanish dictionary in release since a lot of downloads from Spanish
7.New: Improve UI for Mad-Macro list
8.New: Auto indent support for Python
9.New: Purge Caret position history
10.New: Add brief help documents(Basic introduction, Key mapping and help for Regex)
11.New: Switch between Right-to-left and Left-to-right language
12.New: Toggle back from Replace Dialog to Find Dialog
13.New: Add VS 2015 support
14.New: RTL support
15.Fix: Match brace pair has bug if it was in comment or string(From MadEdit)
16.Fix: Select contents between a brace pair has bug
17.Fix: Size of the dictionary selection and path is too small under Ubuntu
18.Fix: Remove some of the code for compatible with wxWidgets 2.8.x
19.Fix: Missing menu items for Copy file path/name/directory after refactor
20.Fix: Fix Mouse event(LDown LDClick) handle issue(Since the beginning of MadEdit)
21.Fix: Don't need "(R)" on windows title to mark read only file
22.Fix: Assertion failure of search/replace results display(string conversion)
23.Fix: Spellchecker would insert instead of replace the word if the caret is at the end of a word(Issue #61)
24.Fix: Horizontal scroll paint issue
25.Fix: Right click menu string
26.Fix: OnPaint issue in RTL
27.Fix: The last char was not displayed while scrolling to the right most
28.Fix: Line number would be reversed in printing while enable RTL
29.Fix: Fix Astyle config could not be saved
30.Fix: Add Readonly check in MadPython before actually run
31.Fix: Assertion failure at MadLines.cpp:2829
32.Other minor improvements

Mod v0.3.7
1.New: Column Align Right(Selected text would be aligned according to right)
2.New: Prefix, postfix and preview of numbering(enhanced numbering)
3.New: Search All in All Opened documents
4.New: Replace All in All Opened documents
5.New: Indicate "Empty expression" on status bar if user did not fill the search box and hit Search
6.New: TrimLeadingSpaces----remove leading space and tab chars of lines
7.New: Selection support in TrimTrailingSpaces, TrimLeadingSpaces, DeleteEmptyLines, DeleteEmptyLinesWithSpaces and JoinLines
8.New: Selected text copying to search text while searching with regex
9.New: Tool button for Numbering(Insert incremental numbers)
10.New: Update preview while editing initial number in Numbering dialog
11.New: Config item for Quick Search bar in option dialog
12.New: Refactor of popup memu/toolbar code
13.New: ReadOnly flag on title and GUI update for ReadOnly mode(disable edit icon/menu in ReadOnly mode)
14.New: New toolbar for refresh/close preview window
15.New: Transparency supported in Search/Replace dialog
16.New: Merge Search/Replace dialog
17.New: Toggle macro result by button
18.Fix: Assertion failure MadEditFrame.cpp:3403
19.Fix: Rollback changes in Search/Replace(since 0.3.5)
20.Fix: Assertion failure at MadEditFrame.cpp:5692 under x86_x64 linux
21.Fix: Fix dictionary download link of OpenOffice on About Dialog
22.Fix: Link warning of VS project
23.Fix: Show search results by default
24.Fix: Set default page of Option dialog to "General"
25.Fix: Move encoding update and config update from MainFrame to search/replace dialog
26.Fix: Reverse order toolbars under linux
27.Fix: Remove extra overflow icon from Toolbars
28.Fix: Crash in ColumnAlign while reverse selection
29.Fix: Column align will has issue if you select beyond the EOF
30.Fix: File name was not marked as "modified*" in ColumnAlign
31.Fix: Minor fix in menu help text
32.Fix: Improve simplified Chinese translation
33.Fix: Wrong dnd cursor if no selection
34.Fix: Improvement on GUI update for menu and toolbar
35.Fix: UI update issue of toggle toolbars
36.Fix: Remove no parent from the style of Replace dialog
37.Fix: Remove duplicated code lines
38.Fix: Bookmark would be toggled if there were multiple instance found in one line
39.Fix: Remove macro debug menu item

Mod v0.3.6
1.New: Create scripts directory if not exists while saving recorded script
2.New: Shortcut for Insert numbers(Ctrl-Shift-N)
3.New: Print function parameter information while dumping(Windows only)
4.Fix: Memory leak in MadEditFrame::OnToolsAstyleFormat
5.Fix: Use global astyle::ASFormatter formatter for performance in MadEditFrame::OnToolsAstyleFormat enhancement
6.Fix: Out-of-bounds issue in toolbar init
7.Fix: Improve in MadLines::MadLines()
8.Fix: Randomly crash under Fedora 22 in initializing
9.Fix: Build issue of FreeBSD
10.Fix: Assertion in FindAll on 64bit Windows
11.Fix: Move caret to the beginning of the line instead of end while left click to select
12.Fix: Update VS projects
13.Fix: Triple click to select whole row not working after the fix above
14.Fix: Diable a warning for libhunspell in VS2013
15.Fix: Fix Assertion failure in MadEdit::UpdateCaret

Mod v0.3.5
1.New: MadMacro local script list on Toolbar
2.New: Open/edit saved MadMacro script
3.New: Build time shown on About Dialog
4.New: Copy the first word instead of the whole selection for search/replace/spell check
5.Update translation
6.Fix: Enable line breaking to enable disable 2 belows
7.Fix: Pad empty lines function in Astyle configs
8.Fix: XML formatting config GUI
9.Fix: Ctrl-Shift-K for Astyle formatter
10.Fix: Assertion if the string was not found in Translation
11.Fix: Macro Recording, missed escape slash char '\' while searching '"'
12.Fix: Remove duplicated recoring string in ProcessCommand
13.Fix: Part of the line would not be see if "show line number" is checked in WordWrap mode
14.Fix: wxGetSelectedChoices compatable issue(crash if you close ToHalfWidth/ToFullWidth dialog directly)
15.Fix: Hang/crash on opening saved script if ${MADEDIT_HOME}/scripts not existing
16.Fix: Crash on opening saved script if the script was not saved under ${MADEDIT_HOME}/scripts
17.Fix: Script file would not listed if you saved it under directory other than ${MADEDIT_HOME}/scripts

Mod v0.3.4.1
1. Fix: Saved config of Astyle don't take effect
2. New: Group text convert/format into menu group
3. New: Added XML formatter and config
4. Updated translation

Mod v0.3.4
1.Fix: Record string issue in FindNext, FindPrev, Replace, ReplaceAll while Regex
    was enabled
2.New: Automatically raise window after closing Search/Replace/FindInFiles dialog
3.New: GTK+ version selection supported(--with-gtk=[VERSION]), 2 and 3 are supported
4.New: Astyle is integrated for formatting C/C++/C#/Java code(Ctrl-Shift-K)
5.Updated Simplified Chinese


Mod v0.3.3.1
1. Fix: ReplaceTextAll being recorded as FindTextAll
2. Fix: Replace button did the same as ReplaceAll
3. Fix: Change local scripts dir to HomeDir instead of AppDir for Linux
4. Fix: Use FileSelector instead of Openfile dialog to save script
5. Fix: Text inversion not working under GTK+3
6. New: Dynamically add saved script on menu list if saving in script home
7. New: GTK+3 support (GTK+2 as default)
8. Fix: Right click menu issue under GTK+3
9. Fix: Text inversion issue under GTK+3
10. Fix: Syntax/Locale directory update for Linux

Mod v0.3.3
1. Update MiniPython to Python 2.7.10
2. Change display of Space and Tab char
3. Fix hang issue of Search/ReplaceTextAll(Use RegEx with ^, $ or (?:))
4. Fix issue of Search/Replace dialog while maxLine is set to less than or equal to 100
5. Refactoring VS2013 project files, X64 was added
6. Refactoring VS2005 project files
7. Improve version number string
8. Update rpm packaging
9. Update language files for linux
10. Add crash dump support
11. Markdown/Html preview(Lightweight Markdown/Html Editor). DON'T use it to edit
  huge Markdown files like me, eg. 20MB. The performance is not good.
12. Convert Markdown to Html
13. Convert Html to plain text, so, Markdown => Html => text
14. Use static link of Python and Hunspell

Mod v0.3.2
1. Change RegKey to "MadEdit-Mod" and add RegKey value "Edit with MadEdit-Mod"
2. Fix caret display issue after hiding Quick Search bar
3. Fix hang issue that in the patch for the https://github.com/hltj/wxMEdit/issues/63.
   Re-implemented using custom event.
4. Fix duplicate Macro recorded, such as cut, cutline, deleteline, etc
5. Fix a random hang in Search/Replace (PaintTextLines), patch release
6. RPM packaging supported(tested with Fedora 20/21)
7. Debian packaging supported(tested with Ubuntu 15.04)
8. Refactoring VS2010 project files, X64 was added
9. Win64 was released
10. Update MiniPython to Python 2.7.10(in X64 version)

Mod v0.3.1
Note: You have to change the madedit.mo to MadEdit-Mod.mo since then
1. Use wxAuiToolBar as ToolBar, group tools into Standard, Editor, Search/Replace,
   Text view, and Macro
2. Context menu for Main Frame/Tool bars(Show/Hide one or all tool bars)
3. Save/Load customized tool bars
4. Add Quick search ToolBar, toggle show/hide with F8(Escape to hide it), drag
   to float one and dock anywhere.follow your search direction if you hit Enter
   after inputing some text. Searching during inputing.
5. Toggle-able sign of the 80th column
6. Improve UI and performance, improve usability of Purge History dialog
7. Improve performance of Purge History
8. Change language tag from "MadEdit" to "MadEdit-Mod"
9. Add "Print" in ToolBar
9. Fix bug of initialized bookmark width
10. Fix crash if you purged history right after MadEdit started and select to purge
    any related to search/replace
11. Fix issue of not purging history till you search/replace after you check the
    item in global option
12. Fix issue of extra bookmark sign if the line was wrapped
13. Update Simplified Chinese

Mod v0.3.0
1. Improve UI and all strings of message box/dialog can be translated since then
2. Change application name string to 'MadEdit-Mod'
3. Update cursor as NO_ENTRY sign while being out of window(Drag and Drop)
4. Add debug output for Visual Studio
5. Disable DND and caret motion if caret is out of window(Drag and Drop)
6. Left click on line number area to select one line/all(with Control pressed)
7. Add area for bookmark instead of overwriting line number
8. Left click on bookmark area to toggle bookmark
9. Display/hide bookmark area
10. Add config item in print options for printing bookmark
11. Use color defined in syntax file for bookmark
12. Add a line to separate line number/bookmark area to the actual content
13. Add a line to as a sign of the 80th column like PSPad
14. Set Encoding of Search/Replace dialog the same as main edit
15. Fix the issue that caret was still moving while being out of window(Drag and Drop)
16. Fix the issue that should not do DND when user dropped the selection at the end/begin of the selection
17. Fix some wrong status of menus when there is no active edit
18. Fix wrong result title of FindAll in MadPython
19. Fix the issue that FindReplaceInFiles would reset the Enable Replace while activating again 
20. Update Simplified Chinese

Mod v0.2.9
1. Auto fill column data while pasting in column mode(the lines would be automatically duplicated if the
   lines in clipboard was less than the selected. Eg, you copied ONE line but you chosen 3 lines in column
   mode and pasted, the line would be automatically duplicated 3 times if the function was enabled in Options)
2. Improve the UI of SelectToCopy in Option dialog
3. Remove the Maximize from Option, FileAssociation, WordCount and Highlight dialogs
4. Improve the UI of dialogs by disabling the resize function of all dialogs except MadMacro
5. Fixed the issue of Mouse Select to Copy not working under wx 3.0(would affect 0.2.7 and 0.2.8)
6. Fixed the issue of column copy/paste crash introduced by wx 3.0(would affect 0.2.7 and 0.2.8)
7. Fix an issue of MadRecentList that would cause annoying error dialog of set working directory error
   (would affect 0.2.7 and 0.2.8)
8. Update Simplified Chinese

Mod v0.2.8
1. Fix the issue that Toolbar was not flat under wx3.0(Now we have separator on Toolbar under Windows now)
2. Revert Hex String to hex(Eg, from '4D6F64', '4D 6F 64', '%4D%6F%64', or '4D,6F,64' to 'Mod', mainly for
   Asian languages. Eg, used to correct some corrupted text----lost one byte)
3. Change default size of MadMacro dialog to 640*480
4. Improve usability of dialogs. Hide modaless dialog before show another modal/ess one
5. Improve usability of FindInFiles
6. Double click to select brace pairs if the caret is at the left/right brace
7. Up/Down to the line above/under the selection while text being selected
8. Add shortcut for Join Lines(Ctrl-J)
9. Add shortcut for Spell Check(Ctrl-K)
10. Add shortcut for Show All Symbols(Ctrl-Alt-A)
11. Move Macro menu group from Tools to top level
12. Fix translation issue with wx3.0(Some text wx3.0 of was not translated, eg. print dialog) 
13. Update Translation for wx3.0, MadPython

Mod v0.2.7
Will move to wx3.0 since then
1. Delete empty lines supported
2. Delete empty lines(with spaces) supported(More line functions?)
3. Join selected lines(replace EOL with space)
4. Fix a bug that would hang Option Dialog(wx3.0.2), MS Windows only
5. Improve usability of Search/Replace/Search In Files
6. Fix issue of SearchAll that would not should results if none found
7. Fix issue of statusbar working under wx3.0
8. Fix issue of check/uncheck FindHex of search
9. Fix issue of check/uncheck FindHex of Replace
10. Rollback changes for INS and ReadOnly showing on statusbar, which is buggy
11. Update Simplified Chinese

Mod v0.2.6.1
1. Bookmark while searching(toggle bookmark/bookmark all/don't show results in results window)
2. Update Simplified Chinese

Mod v0.2.6
1. Enhanced bookmark edit functions(copy/cut/delete/replace) under Edit menu
2. Move Toggle/Goto/clear bookmarks to Search because they are not edit functions
3. Start/End select to mark a start and end of selection by menu
4. Capitalize words in selection
5. Add Capitalize to MadPython
6. Update Simplified Chinese

What's new from v0.2.5
Enhanced bookmark edit functions(copy/cut/delete/replace) under Edit menu
Move Toggle/Goto/clear bookmarks to Search because they are not edit functions
Start/End select to mark a start and end of selection by menu
Capitalize words in selection
Add Capitalize to MadPython
Multiple dictionary supported, Add/Remove word from Personal dictionary
Clear/Set ReadOnly flag of the file
ColumnAlign to align the 'short line'
Add GNU ASM syntax file
Fix build issue with WX 3.0 under Fedora Core 17
Add shortcut to Show/Hide All Chars(Space, Tab and EOL)
Bug fixes

Mod v0.2.5.5
1. Add shortcut to Show All Chars(Space, Tab and EOL)
2. Fixed the mouse issue http://sourceforge.net/p/madedit/bugs/10/, which might cause crash if you click
   on the edit area then click on menu or toolbar(It should be a bug of wxWidgets)
3. Update Simplified Chinese

Mod v0.2.5.4
1. ColumnAlign to align the 'short line'(EOL is ahead of the caret position)
2. Add URL of dictionary download in About Dialog
3. Fix build issue with WX 3.0 under Fedora Core 17(Just follow the original way----to be investigated)
4. Move Clear/Set ReadOnly flag to File menu
5. Add GNU ASM syntax file
6. Update Simplified Chinese

Mod v0.2.5.3
1. Clear/Set ReadOnly flag of the file instead of change outside and close/open again
2. Update Simplified Chinese

Mod v0.2.5.2
1. Remove word from Personal dictionary
2. Fix Linux build(for SpellChecker changes)
3. Fix X11 detection issue of configure script
4. Update Simplified Chinese

Mod v0.2.5.1
1. Fix bugs of Spell Checker config save/load
2. Multiple dictionary supported(different files with different dictionaries at the same time)
3. Personal dictionary(AddWord)/temporary ignore by right click(Enable it @Tools->Options->
   spellChecker)
4. Disable Spell Checker/Bookmark in Hex Mode
5. Fix case sensitive issue of Personal dictionary
6. Fix typo and translation issue

Mod v0.2.5
1. Spell checker released
2. Spell checker config(Dictionary path, switch Dictionary(Tools->Options))
3. Different dictionary for different document(would not save in config)
4. Detect possible dictionaries under some directories
5. Fix typo in README.txt by using Spell Checker

Mod v0.2.5 Alpha
1. Add hunspell as spell checker(Hard code en_US as dictionary for preview, under Dictionaries/)
2. Enable/Disable spell checker(View->Spell Checker)
3. Encoding group/filter
4. Toolbar of bookmark, add ClearAllBookmarks
5. Fixed Linux build(Fedora Core 17)
6. Fixed some minor coding issues reported by g++
7. Add ".hxx" as header file of C++
8. Rollback a fix from wxMedit, which is buggy. https://github.com/hltj/wxMEdit/issues/63

Mod v0.2.4
1. Column Align released
2. Change HKEY_CLASSES_ROOT to HKEY_CURRENT_USER\\Software\\Classes\\ for Windows UAC
3. Improve usability of FindAll
4. FindAll in Selection
5. Fix issue that cannot open the files "a b" and "a\xC0b" simultaneously
6. Fix issue of wxFileHistory by porting wxRecentList from wxMEdit
7. Fix crash issue while building with VS2010
8. Fix issue that \xC0 matches spaces under Windows with Chinese Locale
9. Fix Upper/Lower issue of wxString under Windows(Chinese Ed)
10. Fix issue introduced by FindAll that it would be found if the matched text was at the beginning

Mod v0.2.3
1. Change Info window label for Search or Search in Files
2. Fix issue of mixed tabs and spaces of Column Align and released
3. Add ColumnAlign InsertIncrementalNumber to MadPython and update madpython syntax file

Mod v0.2.2.1
1. Fix Modal dialog bug of wxWidgets 2.8.12(Patch from MadEdit 0.2.9.1) (Win32 only):
    Use MadEdit open two new files, NoName1 and NoName2, the focus is on NoName2 now.
    Select [Convert File Encoding...] in Tools menu, and press OK to close the dialog.
    Then switch to NoName1, and reopen the [Convert File Encoding...] dialog.
    After pressing OK the focus will jump to NoName2, cannot convert the encoding of NoName1.

2. Fix wrong tap order of AUINoteBook http://trac.wxwidgets.org/attachment/ticket/10848/
3. Fix bug in Mouse Select to clipboard
4. Fix issue that Replace dialog never has chance to replace wrap around
5. Update simplified Chinese translation

Mod v0.2.2
1. Add "Insert incremental numbers" to edit popup menu
2. Column Align(Alpha, Known issue: crash with tab mixed with spaces.)
3. Add attribute check in file change detect. The file would be marked as modified
if it was deleted, changed attributes.
4. Improve the code of InsertColumnString
5. Colorful ReadOnly and InsertMode indicator string
6. Fixed issue that Double click would not jump to content line if the file was opened but deleted(FindAll)

Mod v0.2.1
1. Support Insert in HexMode(Toggleable)
2. Paste as insert or replace according to InsertMode
3. Replace in all mode by paste
4. Fixed issue: Purge history while existing not working
5. Fixed issue: Tab to indent while selecting more than 2 lines not working

Mod v0.2.0
New features from 0.1.x
1. Single Instance feature don't compatible with 0.1.x
2. MadPython based on Python 2.7.8 as automation macro(Edit/Record/Run/Save/Load)
3. Insert incremental numbers
3. Purge history during exiting
4. Right click on file tab supported
5. Close to right/left
6. Fixed bugs found including hang issue caused by empty (), [] and {} in regluar expression

Mod v0.2.0 beta 5
New to beta 4
1. Changed error message of Options Dialog
2. Add Incremental numbering feature in Edit->Advanced for column numbering at current caret pos
3. Use InsertString for InsertStr for performance
4. Export InsertIncrementalNumber to MadPython
5. Improve MadMacro dialog
6. Update Simplified Chinese
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
3. Change behave while string has illegal zero. ie. change 0x00(NUL) to 0x2400 Unicode NUL
4. Fixed Caret issue of search/replace(introduced by MadEdit): Caret would blink on wrong place, which 
would be shown in right place if you iconize/restore the window
5. Added MadPython syntax file and MadPython API added in the file

Mod v0.2.0 beta 2
Known issue: MadPython not work well in column mode, eg. insert char in column mode
1. Fixed wxAUIBook bug with patch(http://trac.wxwidgets.org/attachment/ticket/10848/)
2. Add Close All To The Left/Right feature
3. Add default sting in MadMacroDlg, say, medit = MadEdit()
4. Fixed hang issue caused by empty (), [] and {} in regular expression
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
7. Fixed issue of loading script with Unicode characters, eg, Unicode in comments
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
2. Record/Stop/Playback/SaveMacro is done(Known issue: SetCaretPosition would get wrong selection beginning
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
3. Improve behavior of Search and SearchInFiles
4. Show search results on status bar
5. Recoding and Playback(Todo)
6. Save script/connected with shortcut(?)(Todo)
7. Right click on file tab supported(Alpha 2)

Mod v0.1.4 2014.6.29
1. Add clear history function(search/replace history, recent file/font history) from WMedit
2. Update Simplified/Traditional Chinese translation
3. Partially update/add translation of Italian/Japanese/Spanish/Russian? based on WMedit(Anyone can help on this? contact minggang.li@gmail.com)
4. Use wxWidgets 2.8.12 because 3.0.1 still has random hang issue

Mod v0.1.3 2014.6.23
Nothing changed. Rebuild with wxWidgets 3.0.1 which is a bug fix of 3.0. So far so good.

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
6.modified the action when associating file types
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
MadEdit
Alston Chen <madedit@gmail.com>
MadEdit-Mod
Minggang Li <minggang.li@gmail.com>

Links:
-----
MadEdit: http://sourceforge.net/projects/madedit/
MadEdit-Mod: http://sourceforge.net/projects/madedit-mod/
wxWidgets: http://www.wxwidgets.org
wxDevCpp: http://wxdsgn.sourceforge.net
Boost: http://www.boost.org
Open Clip Art Library: http://www.openclipart.org

