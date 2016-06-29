madedit-mod
===========

MadEdit Mod(based on the madedit project @ sourceforge)

Based on [madedit@sf](https://sourceforge.net/projects/madedit/). Patched a lot from the contributors because the author is not active any more. The patches includes bug fixing from contributors of Madedit and some bloody edge enhancement. Madedit is one of the best free editor I ever know. See detailed introduction at it homepage. I create this page just for those user who need these features but are not able to build from source code by themselves. Use GPL because the author does.

Anyway, just for fun!

NOTE: The release marked as Selfuse is a bloody edge and buggy one with a lot of enhancement without carefully tested(Eg. Drag and Drop(cross-platform), bookmark, etc). Try it at your own risk. From 2.9.1, the implementation of single instance has to be changed and not compatible with 2.8.x, which means madedit.exe could not behave correctly if another madedit.exe(complied with wxwidgets 2.8.x) were running.

Current Feature list(comparing to the original Madedit@sourceforge)
===================================================================
* Original features from MadEdit(Hex Edit, Column Edit, many encodings support, etc) with lots of bug fixes
* Drag&Drop Edit(Cross platform)
* Bookmark(From Gogo)
* Bookmark enhancement:Edit functions(Copy/Cut/Delete/Replace bookmarked lines), Bookmark in search
* Print bookmark sign with content
* Highlight word(shift+F8 & Left double click)
* Triple click to select whole line
* Line function enhancement(Cut/Delete/Join/Delete empty)
* Trim leading/trailing spaces/tabs
* Click on line number area to select one line or all(click with Control pressed)
* Search all in current file
* Search/Replace all in all opened documents
* Open files and go to #line by command line
* Embedded Python as Macro language for automation(Run/Rec/Playback, local script list)
* Numbering(Insert incremental numbers with prefix and postfix string)
* Delete all by using ReplaceAll with NULL
* Replace in all mode(Normal/Column/Hex) by paste according to insert mode
* Auto fill column data according to rows selected while pasting in column mode, don't need to prepare lines any more
* Column Align(to left/right)
* Right click on file tab(Close all to the right/left/but this file)
* Fully functional Hex edit(support inserting)
* Spell Check/Suggestions, ignore list and personal dictionary
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
* Silent mode which can be used in scripting(-s -m script_name)
* Run a MadPython script after opening the file(-m script_name)
* RTL(Right-to-left) support
* Typewriter mode
* Windows list dialog
* Always on Top/Whole screen mode
* Auto save/backup
* Interactive scripting(Message box and Input dialog suppored)
* 64 Bit build (Win64 and x86_x64), GTK+3 support

News
=======
Mod v0.4.3
1.New: Use dialog to let user choose one for all instead of annoying one by one dialogs
2.New: Let the user to do the backup selfly #198
3.New: Use Icon to replace the checkbox on QuickSearch Bar #181
4.New: Remember pos of all tool Bars
5.New: Reset tool bars position
6.New: Insert to the top instead of append to the bottom of search results, expand the last automatically and collapse the last
7.New: Add InputBox, MsgBox to MadPython so that user can get input or output some message
8.New: Confirm Undo at the first time if the changes had been saved to disk
9.New: Add Scripts list to right click menu
10.New: Remove file from recentlist if could not be opened
11.New: An Enhanced "Auto-complete" for selection(Option->Edit) #176
12.New: Scroll function on Righ click menu of scroll bars #157
13.New: Support selection replace in Typewriter mode
14.New: Hope typewrite mode could work when use "Backspace" or "Delete" #203
15.New: Double click to highlight word
16.New: Enlarge input area of single mode
17.New: Enhancement for Post-It mode #182
18.Fix: 0.4.2.1 can not remember the word wrap setting #201
19.Fix: The fix width in the Option menu can't Open #199
20.Fix: Improve windows list code, fix a bug of inner-tab drag/reposition(wxAuiNoteBook, http://trac.wxwidgets.org/ticket/17551)
21.Fix: Assertion failure of FindInFilesDialog
22.Fix: Assertion failure of MadMacroDlg
23.Fix: Improve 80 column indicator
24.Other code improvement
25.Upgrade boost to 1.61
26.Update hunspell to 1.41
27.Update Chinese translation

Todo:
=====
* Partial load for large text files which was mentioned by the original author but never getting done.
* FreeBSD packaging (with FreeBSD 10.01)

Known issues:
=============
1. The Macro recording function will save all your commands to one document which
    means it will not record the "switching" between documents.
2. Search in selection in column mode is not exact the selection highlighted.
3. Regular expression which relies on characters before/after the matched string, 
    Eg. (?<=.)\s(?=.), will has issue to be replaced one by one. It's tricky and
    NP++ has the same issue too.
4. HighlightWord would not work if the caret is at the end of the line
