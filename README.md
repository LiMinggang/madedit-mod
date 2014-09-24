madedit-mod
===========

MadEdit Mod(based on the madedit project @ sourceforge)

Based on madedit@sf. Patched a lot from the contributors because the author is not active any more. The patches includes bug fixing from contributors of Madedit and some bloody edge enhancement. Madedit is one of the best free editor I ever know. See detailed introduction at it homepage. I create this page just for those user who need these features but are not able to build from source code by themselves. Use GPL because the author does.

Anyway, just for fun!

NOTE: The release marked as Selfuse is a bloody edge and buggy one with a lot of enhancement without carefully tested(Eg. Drag and Drop(cross-platform), bookmark, etc). Try it at your own risk. From 2.9.1, the implementation of single instance has to be changed and not compatible with 2.8.x, which means madedit.exe could not behave correctly if another madedit.exe(complied with wxwidgets 2.8.x) were running.

Current Feature list(comparing to the original Madedit@sourceforge)
===================================================================
Drag&Drop Edit

Bookmark

Highlight word(shift+F8)

Triple click to select whole line

Search all in current file

Change Highlight after saving

Delete all by using ReplaceAll? with a NULL word

MadPython as Macro Language

Increamental numbering for column edit

Insert mode support for Hex Edit

Paste to replace for all mode(Text, column and Hex)

Close to right/left

History
=======
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

v0.2.9 Mod
1.Fixed several critical/crash bugs in Madedit 0.2.9.
2.New features added.
    Drag&Drop Edit
    Bookmark
    Highlight words(shift+F8)
    Triple click to select whole line
    Search all in current file

2013.6.27 Fixed towlower/towupper bug(MS windows) in Chinese Locale

2013.1.7 Finished feature list: Highlight words(Shift+F8, which is the same as Source Insight). It's easy to use Double click but I did not. Please ask for it if you prefer. Will do a release recently.

Todo:
=====
Partial load for large text files which was mentioned by the original author but never getting done.

Known issues:
=============
1. The cursors of Drag&Drop Edit do not be changed on Linux 2. Sometime lose focus would cause problem in mouse event, you have to restart it(you could not even close it by mouse).
