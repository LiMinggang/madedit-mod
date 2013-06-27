madedit-mod
===========

MadEdit Mod(based on the madedit project @ sourceforge)

Based on madedit@sf. Patched a lot from the contributors because the author is not active any more. The patches includes bug fixing from contributors of Madedit and some bloody edge enhancement. Madedit is one of the best free editor I ever know. See detailed introduction at it homepage. I create this page just for those user who need these features but are not able to build from source code by themselves. Use GPL because the author does. 

Anyway, just for fun! 

NOTE: The release marked as Selfuse is a bloody edge and buggy one with a lot of enhancement without carefully tested(Eg. Drag and Drop(cross-platform), bookmark, etc). Try it at your own risk. From 2.9.1, the implementation of single instance has to be changed and not compatible with 2.8.x, which means madedit.exe could not behave correctly if another madedit.exe(complied with wxwidgets 2.8.x) were running. 
Current Feature list(comparing to the original Madedit@sourceforge)

Drag&Drop Edit 

Bookmark 

Highlight word(shift+F8) 

Triple click to select whole line 

Search all in current file 

Change Highlight after saving 

Delete all by using ReplaceAll? with a NULL word 

History 
2013.6.27 Fixed towlower/towupper bug(MS windows) in Chinese Locale

2013.1.7 Finished feature list: Highlight words(Shift+F8, which is the same as Source Insight). It's easy to use Double click but I did not. Please ask for it if you prefer. Will do a release recently. 

Todo: Partial load for large text files which was mentioned by the original author but never getting done. 

Known issues: 1. The cursors of Drag&Drop Edit do not be changed on Linux 2. Sometime lose focus would cause problem in mouse event, you have to restart it(you could not even close it by mouse).
