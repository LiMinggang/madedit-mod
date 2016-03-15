/*
* This file is part of SpellChecker plugin for Code::Blocks Studio
* Copyright (C) 2009 Daniel Anselmi
*
* SpellChecker plugin is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* SpellChecker plugin is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with SpellChecker. If not, see <http://www.gnu.org/licenses/>.
*
*/
#ifndef __HUNSPELL_CHECK_INTERFACE__
#define __HUNSPELL_CHECK_INTERFACE__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/string.h"

// spell checker/thingie
#include "wx/process.h"
#include "wx/txtstrm.h"
#include "wx/file.h"

#include "SpellCheckUserInterface.h"
#include "PersonalDictionary.h"

// Get rid of the warning about identifiers being truncated in the debugger.  Using the STL collections
// will produce this everywhere.  Must disable at beginning of stdafx.h because it doesn't work if
// placed elsewhere.
#ifdef __VISUALC__
#pragma warning(disable:4786)
#endif
class Hunspell;
class HunspellInterface : public wxSpellCheckEngineInterface
{
public:
    HunspellInterface(wxSpellCheckUserInterface* pDlg = NULL);
    ~HunspellInterface();

    // Spell Checker functions
    virtual wxString GetSpellCheckEngineName()
    {
        return _T("Hunspell");
    }
    virtual int InitializeSpellCheckEngine();
    virtual int UninitializeSpellCheckEngine();
    virtual int SetOption(SpellCheckEngineOption& Option);
    virtual void UpdatePossibleValues(SpellCheckEngineOption& OptionDependency, SpellCheckEngineOption& OptionToUpdate);
    virtual wxString CheckSpelling(wxString strText);
    virtual bool IsSpellingOk(wxString strText);
    wxArrayString GetSuggestions(const wxString& strMisspelledWord);

    virtual bool IsWordInDictionary(const wxString& strWord);
    virtual bool IsWordInPersonalDictionary(const wxString& strWord);
    virtual int AddWordToDictionary(const wxString& strWord);
    virtual int RemoveWordFromDictionary(const wxString& strWord);
    virtual wxArrayString GetWordListAsArray();
    void OpenPersonalDictionary(const wxString& strPersonalDictionaryFile=wxEmptyString);    
    virtual void SetEnablePersonalDictionary(bool enable);
#if __cplusplus <= 199711L
    virtual void SetSyntaxDictionary(boost::shared_ptr<PersonalDictionary>& syntaxKeywordDict)
#else
    virtual void SetSyntaxDictionary(std::shared_ptr<PersonalDictionary>& syntaxKeywordDict)
#endif
    {
        m_SyntaxKeywordDict = syntaxKeywordDict;
    }
    PersonalDictionary* GetPersonalDictionary()
    {
        return &m_PersonalDictionary;
    }
    void AddCustomMySpellDictionary(const wxString& strDictionaryName, const wxString& strDictionaryFileRoot);
    void CleanCustomMySpellDictionaries()
    {
        m_CustomMySpellDictionaryMap.clear();
    }
    virtual wxString GetCharacterEncoding();
    virtual int GetUserCorrection(const wxString& strMisspelling);

private:
    void PopulateDictionaryMap(StringToStringMap* pLookupMap, const wxString& strDictionaryPath);
    void AddDictionaryElement(StringToStringMap* pLookupMap, const wxString& strDictionaryPath, const wxString& strDictionaryName, const wxString& strDictionaryFileRoot);
    wxString GetSelectedLanguage();
    wxString GetAffixFileName();
    wxString GetAffixFileName(const wxString& strDictionaryName);
    wxString GetDictionaryFileName();
    wxString GetDictionaryFileName(const wxString& strDictionaryName);

    Hunspell* m_pHunspell;

    StringToStringMap m_DictionaryLookupMap;
    StringToStringMap m_CustomMySpellDictionaryMap;
    wxString m_strDictionaryPath;

    PersonalDictionary m_PersonalDictionary;
#if __cplusplus <= 199711L
    boost::shared_ptr<PersonalDictionary> m_SyntaxKeywordDict;
#else
    std::shared_ptr<PersonalDictionary> m_SyntaxKeywordDict;
#endif
    bool m_EnablePersonalDictionary;
};

#endif  // __MYSPELL_CHECK_INTERFACE__
