#ifndef __WX_SPELL_CHECKER_PERSONAL_DICTIONARY__
#define __WX_SPELL_CHECKER_PERSONAL_DICTIONARY__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/string.h"

class PersonalDictionary
{
public:
	PersonalDictionary(wxString strFileName = _T(".wxSpellCheckerPersonalDictionary"));
	~PersonalDictionary();

	bool LoadPersonalDictionary();
	bool SavePersonalDictionary();
	bool IsWordInDictionary(const wxString& strWord);
	void AddWord(const wxString& strWord);
	void RemoveWord(const wxString& strWord);
	wxArrayString GetWordListAsArray();
	wxString GetDictionaryFileName()
	{
		return m_strDictionaryFileName;
	}
	void SetDictionaryFileName(const wxString& filename)
	{
		m_strDictionaryFileName = filename;
	}

private:
	wxSortedArrayString m_DictionaryWords;
	wxString m_strDictionaryFileName;
};

#endif // __WX_SPELL_CHECKER_PERSONAL_DICTIONARY__
