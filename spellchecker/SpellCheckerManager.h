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
#ifndef SPELLCHECKERMANAGER_H
#define SPELLCHECKERMANAGER_H
#ifndef _MSC_VER 
#if __cplusplus <= 199711L
#define CPLUSEPLUSE98 1
#include <boost/shared_ptr.hpp>
#else 
#include <memory>
#endif
#else
#if _MSC_VER < 1800
#define CPLUSEPLUSE98 1
#include <boost/shared_ptr.hpp>
#else 
#include <memory>
#endif
#endif

#include <vector>
#include <map>
#include <wx/string.h>

class wxConfigBase;
class wxSpellCheckEngineInterface;

#if CPLUSEPLUSE98
typedef std::map<wxString, boost::shared_ptr<wxSpellCheckEngineInterface> > SpellCheckerMap;
#else
typedef std::map<wxString, std::shared_ptr<wxSpellCheckEngineInterface> > SpellCheckerMap;
#endif

class SpellCheckerManager
{
private:
    SpellCheckerManager();
    SpellCheckerManager(SpellCheckerManager &){}
    virtual ~SpellCheckerManager();
public:
    static SpellCheckerManager & Instance()
    {
        static SpellCheckerManager dictMgr;
        return dictMgr;
    }

    bool GetEnableOnlineChecker()
    {
        return m_EnableOnlineChecker;
    }
    void SetEnableOnlineChecker(bool val)
    {
        m_EnableOnlineChecker = val;
    }
    bool GetEnableSpellTooltips()
    {
        return m_EnableSpellTooltips;
    }
    void SetEnableSpellTooltips(bool val)
    {
        m_EnableSpellTooltips = val;
    }
    bool GetEnableThesaurusTooltips()
    {
        return m_EnableThesaurusTooltips;
    }
    void SetEnableThesaurusTooltips(bool val)
    {
        m_EnableThesaurusTooltips = val;
    }
    const wxString GetDictionaryName()const
    {
        return m_strDictionaryName;
    }
    void SetDictionaryName(wxString val)
    {
        m_strDictionaryName = val;
    }
    const bool GetEnablePersonalDictionary()const
    {
        return m_EnablePersonalDictionary;
    }
	void SetEnablePersonalDictionary(bool val);
    int GetSelectedDictionaryNumber()const
    {
        return selectedDictionary;
    }

    void Load();
    void Save();
    wxString GetLanguageName(const wxString& language_id);
    wxString GetDictionaryName(const wxString& dictDesc);

    void ScanForDictionaries();

    const std::vector<wxString> &GetPossibleDictionaries()const
    {
        return m_dictionaries;
    }
    const wxString GetDictionaryPath()const
    {
        return m_DictPath;
    }
    const wxString GetThesaurusPath()const
    {
        return m_ThesPath;
    }
    const wxString GetBitmapPath()const;
    void SetDictionaryPath(const wxString &path){m_DictPath = path;}
    void SetThesaurusPath(const wxString &path){m_ThesPath = path;}
    void SetBitmapPath(const wxString &path){m_BitmPath = path;}
    const wxString GetPersonalDictionaryFilename()const;
#if CPLUSEPLUSE98
    boost::shared_ptr<wxSpellCheckEngineInterface> &GetSpellChecker();
#else
    std::shared_ptr<wxSpellCheckEngineInterface> &GetSpellChecker();
#endif
private:
    const wxString GetRawDictionaryPath()const{return m_DictPath;}
    const wxString GetRawThesaurusPath()const{return m_ThesPath;}
    const wxString GetRawBitmapPath()const{return m_BitmPath;}
    
    void ScanForDictionaries(const wxString &path);
    void PopulateLanguageNamesMap();
    void DetectDictionaryPath();
    void DetectThesaurusPath();
    wxString m_DictPath;
    wxString m_ThesPath;
    wxString m_BitmPath;
    bool m_EnableOnlineChecker;
    bool m_EnablePersonalDictionary;
    bool m_EnableSpellTooltips;
    bool m_EnableThesaurusTooltips;
    wxString m_strDictionaryName;
    std::vector<wxString> m_dictionaries;
    int selectedDictionary;
    std::map<wxString, wxString> m_LanguageNamesMap;
    std::map<wxString, wxString> m_DictionaryNamesMap;
    SpellCheckerMap m_SpellCheckerMap;
    wxConfigBase    *m_Config;
};

#endif // SPELLCHECKERMANAGER_H
