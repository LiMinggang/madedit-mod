madedit-mod
===========
[![License](https://img.shields.io/badge/license-GPLv3.0%2B-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![Build Status](https://img.shields.io/travis/LiMinggang/madedit-mod/master.svg?label=Linux)](https://travis-ci.org/LiMinggang/madedit-mod)
[![GitHub Latest release](https://img.shields.io/github/release/LiMinggang/madedit-mod/all.svg)](https://github.com/LiMinggang/madedit-mod/releases)
[![GitHub issues](https://img.shields.io/github/issues/LiMinggang/madedit-mod.svg)](https://github.com/LiMinggang/madedit-mod/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2020.svg)](https://github.com/LiMinggang/madedit-mod/commits/master)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/19986/badge.svg)](https://scan.coverity.com/projects/19986)
[![Ohloh Project Status](https://www.openhub.net/p/madedit-mod/widgets/project_thin_badge.gif)](https://www.openhub.net/p/madedit-mod)

MadEdit Mod(based on the madedit project @ sourceforge)

Based on [madedit@sf](https://sourceforge.net/projects/madedit/). Patched a lot from the contributors because the author is not active any more. The patches includes bug fixing from contributors of Madedit and some bloody edge enhancement. Madedit is one of the best free editor I ever know. See detailed introduction at it homepage. I create this page just for those user who need these features but are not able to build from source code by themselves. Use GPL because the author does.

You're encouraged to translate MadEdit-Mod into your native language if there's not already a translation.

NOTE:

The release marked as Selfuse is a bloody edge and buggy one with a lot of enhancement without carefully tested(Eg. Drag and Drop(cross-platform), bookmark, etc). Try it at your own risk. From 2.9.1, the implementation of single instance has to be changed and not compatible with 2.8.x, which means madedit.exe could not behave correctly if another madedit.exe(complied with wxwidgets 2.8.x) were running.

Current Feature list(comparing to the original Madedit@sourceforge)
===================================================================
* Original features from MadEdit(Hex Edit, Column Edit, many encodings support, etc) with lots of bug fixes
* More encodings supported, eg, GB18030, CP874, CP866, KOI8-R and KOI8-U Macintosh encodings(Roman, Japanese, Chinese-trad, Korean, Arabic, Hebrew, Greek, Cyrillic, Thai, Chinese-simp, Central-Eur, Croatian, Icelandic, Romanian) if your system supports
* Pan Chinese character support in search(CJK non-sensitive)
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

Translations
=============================
* English
* Chinese Simplified (zh_CN)
* Chinese Traditional(zh_TW)
* German(de_DE)              (Not done)
* Greek(el)
* Italian(it_IT)             (Not done)
* Japanese(ja_JP)
* Polish(pl_PL)              (Not done)
* Russian(ru_RU)             (Not done)
* Spanish(es)                (Not done)

News
=======
Mod v0.4.18
* Note: Rollback *nix build to static link with wxWidgets because UI issues reported
* New: Automatically choose extension for new file according to syntax used
* New: Sort windows by clicking column tab(Windows list dialog)
* New: Sort windows by clicking column tab(Save query dialog)
* New: Update icon for Convert File Encoding menu item #297
* New: Zero length match indicator in FindTextPrevious
* Fix: Fix bug of windows sorting in Windows list dialog
* Fix: Minor fix for Pan Chinese support
* Fix: Enable/diable Pan Chinese on clicking Hex searching
* Fix: Pan Chinese on Search in files
* Fix: Binary file detect bug in FindInFiles
* Fix: Can't remember Main Window size larger than 1024*768 #296
* Fix: Replace buggy wxChoice with wxComboBox for Dictionary selection
* Fix: Syntax combo box status is not updated while switching among opened files
* Fix: Unicode converting bug if character is greater than 0x10000 in ConvertChinese chars(Windows only)
* Fix: Safe mode in Chinese convertion #293
* Fix: Regex search bug in FindTextNext
* Fix: Regex search bug in FindTextPrevious
* Update wxWidgets to 3.1.3
* Update boost to 1.71
* Update Python to 2.7.17
* Update hunspell to latest
* Update gb18030 to iconv 1.16
* Update Simplified Chinese translation
* Patch for wxWidgets 3.1.3 issue under Windows 10

Todo
=====
* Partial load for large text files which was mentioned by the original author but never getting done.

Known issues
=============
1. The Macro recording function will save all your commands to one document which
    means it will not record the "switching" between documents.
2. Search in selection in column mode is not exact the selection highlighted.
3. HighlightWord would not work if the caret is at the end of the line
4. Scroll function on Right click menu of scroll bars only works with Windows
5. '*' is allowed in file name under Linux/Unix but is used as delimiter for file name and line number, use -d and -w to use it as wildcard
6. Alt+LeftDown to toggle bookmark does not work on Ubuntu because Alt was a special key(?) for the OS
7. Preview html will hang on some html files(wxWidgets html view bug)
8. wxWidgets GTK3 may have issue under some themes of some Ubuntu dists, eg, Elementry OS
9. MadPython does not support locale convertion and 'print' can NOT print non-ASCII char correctly

License
=============
* Mixed
1. GPL for the code from MadEdit and developed by myself
2. MIT License for the code from Astyle
3. Boost Software License for boost.python
4. Python License for miniPython
5. LGPL for hunspell
6. Others by the original authors

Thanks
=============
* [Nikoss](https://github.com/nikoss)   Greek Translation
* [ZhTw2013](https://github.com/zhtw2013) Traditional Chinese Translation
* [Tilt](mailto:tiltstr@gmail.com) Japanese Translation

Download
=============
[Madedit-Mod@sf](https://sourceforge.net/projects/madedit-mod/files/?source=navbar)

