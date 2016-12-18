madedit-mod
===========

MadEdit Mod(based on the madedit project @ sourceforge)

Based on [madedit@sf](https://sourceforge.net/projects/madedit/). Patched a lot from the contributors because the author is not active any more. The patches includes bug fixing from contributors of Madedit and some bloody edge enhancement. Madedit is one of the best free editor I ever know. See detailed introduction at it homepage. I create this page just for those user who need these features but are not able to build from source code by themselves. Use GPL because the author does.

Anyway, just for fun!

NOTE: The release marked as Selfuse is a bloody edge and buggy one with a lot of enhancement without carefully tested(Eg. Drag and Drop(cross-platform), bookmark, etc). Try it at your own risk. From 2.9.1, the implementation of single instance has to be changed and not compatible with 2.8.x, which means madedit.exe could not behave correctly if another madedit.exe(complied with wxwidgets 2.8.x) were running.

Current Feature list(comparing to the original Madedit@sourceforge)
===================================================================
* Original features from MadEdit(Hex Edit, Column Edit, many encodings support, etc) with lots of bug fixes
* More encodings supported, eg, GB18030, CP874, KOI-R, KOI-U and CP866, Macintosh encodings(Roman, Japanese, Chinese-trad, Korean, Arabic, Hebrew, Greek, Cyrillic, Thai, Chinese-simp, Central-Eur, Croatian, Icelandic, Romanian)
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
Todo:
FreeBSD packaging (with FreeBSD 10.01)
1. configure CC=gcc CXX=g++
2. aclocal version in aclocal.m4
3. stdc++ version

Mod v0.4.6
* New: Spell check for SearchReplaceDialog would change according to active window
* New: Ctrl+Shift+Wheel as shortcut for Page Up/Down
* Fix: Toolbar issue

Mod v0.4.5
* New: File history dialog
* New: Add default key, down key support(the same as Search Dialog) for FindInFiles Dialog
* Fix: Size issue of some UI items #215
* Fix: Two typos #82
* Fix: Don't show EOF but Space/Tab in Search/Replace Dialog
* Fix: CPPCheck warnings
* Fix: Change shortcuts of Go Back/Forward
* Fix: Some greek symbols that require the Alt+ compination don't get created #214
    with Greek extended keyboard layout
    Madeit-Mod -------------------> result <------------ IceApe
    RightAlt+Shift+r --------------> U+00AE <----------------- RightAlt+r
    RightAlt+Shift+[2 times ; ] --> ¡§ <------------------ [couldn't find it]
* Fix: Remove support for Win98
* Fix: Some illegal chars in file name(eg, 0xFFF0~0xFFFF) would cause wxWidgets crash
* Fix: a bug when move up #217
* Fix: Crash if close the last file by double clicking page tab #218
* Other improvements
* Update translation

Todo:
=====
* Partial load for large text files which was mentioned by the original author but never getting done.
* FreeBSD packaging (with FreeBSD 10.01)
1. configure CC=gcc CXX=g++
2. aclocal version in aclocal.m4
3. stdc++ version

Known issues:
=============
1. The Macro recording function will save all your commands to one document which
    means it will not record the "switching" between documents.
2. Search in selection in column mode is not exact the selection highlighted.
3. Regular expression which relies on characters before/after the matched string, 
    Eg. (?<=.)\s(?=.), will has issue to be replaced one by one. It's tricky and
    NP++ has the same issue too.
4. HighlightWord would not work if the caret is at the end of the line

Download:
=============
[Madedit-Mod@sf](https://sourceforge.net/projects/madedit-mod/files/?source=navbar)