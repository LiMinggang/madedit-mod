///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        MadRecentList.h
// Description: Porting Recent List from wxMedit
// Author:      
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAD_RECENT_LIST_H_
#define _MAD_RECENT_LIST_H_

#include <wx/docview.h>

class MadRecentList: public wxFileHistory
{
public:
    static inline bool OSCaseSensitive()
    {
#ifdef __WXMSW__
        return false;
#else
        return true;
#endif
    }

    MadRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1, bool caseSensitive=OSCaseSensitive() )
        : wxFileHistory(maxFiles, idBase), m_caseSensitive(caseSensitive), m_idBase(idBase)
    {}

    virtual void AddFileToHistory(const wxString& file);
private:
#if (wxMAJOR_VERSION >= 3)
	wxString NormalizeFileName(const wxFileName& fn);
	wxString GetMRUEntryLabel(int n, const wxString& path)
	{
		// we need to quote '&' characters which are used for mnemonics
		wxString pathInMenu(path);
		pathInMenu.Replace("&", "&&");
	
		return wxString::Format("&%d %s", n + 1, pathInMenu);
	}
#endif

    bool ItemEQ(const wxString& item1, const wxString& item2);

    bool m_caseSensitive;
	wxWindowID m_idBase;
};

#endif //_MAD_RECENT_LIST_H_

