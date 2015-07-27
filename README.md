madedit-mod
===========

MadEdit Mod(based on the madedit project @ sourceforge)

Based on madedit@sf. Patched a lot from the contributors because the author is not active any more. The patches includes bug fixing from contributors of Madedit and some bloody edge enhancement. Madedit is one of the best free editor I ever know. See detailed introduction at it homepage. I create this page just for those user who need these features but are not able to build from source code by themselves. Use GPL because the author does.

Anyway, just for fun!

NOTE: The release marked as Selfuse is a bloody edge and buggy one with a lot of enhancement without carefully tested(Eg. Drag and Drop(cross-platform), bookmark, etc). Try it at your own risk. From 2.9.1, the implementation of single instance has to be changed and not compatible with 2.8.x, which means madedit.exe could not behave correctly if another madedit.exe(complied with wxwidgets 2.8.x) were running.

Current Feature list(comparing to the original Madedit@sourceforge)
===================================================================
* Original features from MadEdit(Hex Edit, Column Edit, many encodings support, etc) with lots of bug fixes
* Drag&Drop Edit(Cross platform)
* Bookmark(From Gogo)
* Bookmark enhancement:Edit functions(Copy/Cut/Delete/Replace bookmarked lines), Bookmark in search
* Print bookmark sign with content
* Highlight word(shift+F8)
* Triple click to select whole line
* Line function enhancement(Cut/Delete/Join/Delete empty)
* Click on line number area to select one line or all(click with Control pressed)
* Search all in current file
* Open files and go to #line by command line
* Embedded Python as Macro language for automation(Run/Rec/Playback, local script list)
* Insert incremental numbers
* Delete all by using ReplaceAll with NULL
* Replace in all mode(Normal/Column/Hex) by paste according to insert mode
* Auto fill column data according to rows selected while pasting in column mode, don't need to prepare lines any more
* Column Align
* Right click on file tab(Close all to the right/left/but this file)
* Fully functional Hex edit
* Spell Check/Suggestions, ignore list and personal dictionary
* Clear history while exiting
* Revert Hex String to hex(Eg, from '4D6F64', '4D 6F 64', '%4D%6F%64', or '4D,6F,64' to 'Mod')
* Double click at a brace to select all between the brace pair(Delimiter selection)
* Up/Down to the line above/under the selection if selecting
* Click on bookmark area to toggle bookmark
* Context menu for Main Frame/Tool bars(Show/Hide one or all tool bars)
* Quick Search, F8 to show, Esc to hide search bar, Enter to search next/previous according to your last operation. Immediately matching while inputting
* Purge history manually or automatically while exiting
* Mardown/Html Preview(Lightweight Markdown/Html editor)
* Convert Mardown to Html
* Convert Html to plain text
* Automatic Formatter for C, C++, C++/CLI, Objective-C, C#, and Java Source Code(Astyle)
* XML Formatter(DTD is not supported)
* Crash dump function
* 64 Bit build (Win64 and x86_x64), GTK+3 support

History
=======

Mod v0.3.4.2
Fix: Enable line breaking to enable disable 2 belows
Fix: Pad empty lines function in Astyle configs
Fix: XML formatting config GUI
Fix: Ctrl-Shift-K for Astyle formatter
Fix: Assertion if the string was not found in Translation
Fix: Macro Recording, missed escape slash char '\' while searching '"'

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

Todo:
=====
* Partial load for large text files which was mentioned by the original author but never getting done.
* FreeBSD packaging (with FreeBSD 10.01)

Known issues:
=============
1. The cursors of Drag&Drop Edit do not be changed on Linux
