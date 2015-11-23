///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:		MadRecentList.cpp
// Description: Porting Recent List from wxMEdit
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadRecentList.h"
#include <wx/menu.h>
#include "wx/confbase.h"
#include "wx/filename.h"

// Ugly global
extern wxString MadStrLower(const wxString &);
bool MadRecentList::ItemEQ(const wxString& item1, const wxString& item2)
{
    if (m_caseSensitive)
        return item1 == item2;
    return (MadStrLower(item1)==MadStrLower(item2));
}

static const wxChar *s_MRUEntryFormat = wxT("&%d %s");

static inline wxChar* MYcopystring(const wxString& s)
{
    wxChar* copy = new wxChar[s.length() + 1];
    return wxStrcpy(copy, s.c_str());
}

/* static */
#if (wxMAJOR_VERSION >= 3)
wxString MadRecentList::NormalizeFileName(const wxFileName& fn)
{
    // We specifically exclude wxPATH_NORM_LONG here as it can take a long time
    // (several seconds) for network file paths under MSW, resulting in huge
    // delays when opening a program using wxFileHistory. We also exclude
    // wxPATH_NORM_ENV_VARS as the file names here are supposed to be "real"
    // file names and not have any environment variables in them.
    //wxFileName fnNorm(fn);
    //fnNorm.Normalize(wxPATH_NORM_DOTS |
    //                 wxPATH_NORM_TILDE |
    //                 wxPATH_NORM_CASE |
    //                 wxPATH_NORM_ABSOLUTE);
    //return fnNorm.GetFullPath();
    if (m_caseSensitive)
        return fn.GetFullPath();
    return MadStrLower(fn.GetFullPath());
}
#endif
// Recent List management
void MadRecentList::AddFileToHistory(const wxString& file)
#if (wxMAJOR_VERSION < 3)
{
    size_t i;

    // Check we don't already have this item
    for (i = 0; i < m_fileHistoryN; ++i)
    {
        if ( m_fileHistory[i] && ItemEQ(file, m_fileHistory[i]) )
        {
            // we do have it, move it to the top of the history
            RemoveFileFromHistory (i);
            AddFileToHistory (file);
            return;
        }
    }

    // if we already have a full history, delete the one at the end
    if ( m_fileMaxFiles == m_fileHistoryN )
    {
        RemoveFileFromHistory (m_fileHistoryN - 1);
        AddFileToHistory (file);
        return;
    }

    // Add to the project file history:
    // Move existing files (if any) down so we can insert file at beginning.
    if (m_fileHistoryN < m_fileMaxFiles)
    {
        wxList::compatibility_iterator node = m_fileMenus.GetFirst();
        while (node)
        {
            wxMenu* menu = (wxMenu*) node->GetData();
            if ( m_fileHistoryN == 0 && menu->GetMenuItemCount() )
            {
                menu->AppendSeparator();
            }
            menu->Append(GetBaseId()+m_fileHistoryN, _("[EMPTY]"));
            node = node->GetNext();
        }
        ++m_fileHistoryN;
    }
    // Shuffle files down
    for (i = (m_fileHistoryN-1); i > 0; --i)
    {
        m_fileHistory[i] = m_fileHistory[i-1];
    }
    m_fileHistory[0] = MYcopystring(file);

    for (i = 0; i < m_fileHistoryN; ++i)
    {
        if ( m_fileHistory[i] )
        {
            wxString itemInMenu = m_fileHistory[i];

            // we need to quote '&' characters which are used for mnemonics
            itemInMenu.Replace(_T("&"), _T("&&"));
            wxString buf;
            buf.Printf(s_MRUEntryFormat, i + 1, itemInMenu.c_str());
            wxList::compatibility_iterator node = m_fileMenus.GetFirst();
            while (node)
            {
                wxMenu* menu = (wxMenu*) node->GetData();
                menu->SetLabel(GetBaseId() + i, buf);
                node = node->GetNext();
            }
        }
    }
}
#else
{
    // Check if we don't already have this file. Notice that we avoid
    // wxFileName::operator==(wxString) here as it converts the string to
    // wxFileName and then normalizes it using all normalizations which is too
    // slow (see the comment above), so we use our own quick normalization
    // functions and a string comparison.
    const wxFileName fnNew(file);
    const wxString newFile = NormalizeFileName(fnNew);
    size_t i,
           numFiles = m_fileHistory.size();
    for ( i = 0; i < numFiles; i++ )
    {
        if ( newFile == NormalizeFileName(m_fileHistory[i]) )
        {
            // we do have it, move it to the top of the history
            RemoveFileFromHistory(i);
            numFiles--;
            break;
        }
    }

    // if we already have a full history, delete the one at the end
    if ( numFiles == m_fileMaxFiles )
    {
        RemoveFileFromHistory(--numFiles);
    }

    // add a new menu item to all file menus (they will be updated below)
    for ( wxList::compatibility_iterator node = m_fileMenus.GetFirst();
        node;
        node = node->GetNext() )
    {
        wxMenu * const menu = (wxMenu *)node->GetData();

        if ( !numFiles && menu->GetMenuItemCount() )
            menu->AppendSeparator();

        // label doesn't matter, it will be set below anyhow, but it can't
        // be empty (this is supposed to indicate a stock item)
        menu->Append(m_idBase + numFiles, " ");
    }

    // insert the new file in the beginning of the file history
    m_fileHistory.insert(m_fileHistory.begin(), file);
    numFiles++;

    // update the labels in all menus
    for ( i = 0; i < numFiles; i++ )
    {
        // if in same directory just show the filename; otherwise the full path
        const wxFileName fnOld(m_fileHistory[i]);

        wxString pathInMenu;
        if ( fnOld.GetPath() == fnNew.GetPath() )
        {
            pathInMenu = fnOld.GetFullName();
        }
        else // file in different directory
        {
            // absolute path; could also set relative path
            pathInMenu = m_fileHistory[i];
        }

        for ( wxList::compatibility_iterator node = m_fileMenus.GetFirst();
              node;
              node = node->GetNext() )
        {
            wxMenu * const menu = (wxMenu *)node->GetData();

            menu->SetLabel(m_idBase + i, GetMRUEntryLabel(i, pathInMenu));
        }
    }
}
#endif

