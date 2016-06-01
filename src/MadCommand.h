///////////////////////////////////////////////////////////////////////////////
// Name:        MadCommand.h
// Description: for building MenuBar & ToolBar & KeyBinding
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADCOMMAND_H_
#define _MADCOMMAND_H_

#include "MadEdit/MadEditCommand.h"
class wxAuiToolBar;

struct CommandData
{
    MadEditCommand command;
    int            menu_level;
    int            menu_id;
    const wxChar   *menuid_name;
    const wxChar   *text;
    const wxChar   *key;
    wxItemKind     kind;
    int            image_idx;
    wxMenu         **menu_ptr;
    const wxChar   *hint;
    wxMenu         **popmenu_ptr;
	wxAuiToolBar   **toolbar_ptr;
    const wxChar   *short_help;
    bool           apd_toolbar_spr;
};

struct ToolBarData
{
    int            toolbar_id;
    long           toolbar_style;
    const wxChar * panel_pos;
    const wxChar * toolbarid_name;
    const wxChar * caption;
    int            pos;
};

extern CommandData CommandTable[];
extern ToolBarData ToolBarTable[];

#endif
