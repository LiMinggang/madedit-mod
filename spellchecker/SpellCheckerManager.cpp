/*
* This file is part of SpellChecker for MadEdit Mod
* Copyright (C) 2014 Minggang Li
*
* SpellChecker is free software; you can redistribute it and/or modify
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
#include "SpellCheckerManager.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/utils.h>
#include <wx/arrstr.h>
#include <wx/confbase.h>

#include "HunspellInterface.h"
#if CPLUSEPLUSE98
using boost::shared_ptr;
#else
using std::shared_ptr;
#endif
extern wxString g_MadEditHomeDir;

#define CFG_SPELLCHECK_ENABLE_ONLINE_CHECK      _T("EnableOnlineChecker")
#define CFG_SPELLCHECK_ENABLE_PERSONAL_DICT     _T("EnablePersonalDictionary")
#define CFG_SPELLCHECK_SPELL_TOOLTIPS_CHECK     _T("SpellTooltips")
#define CFG_SPELLCHECK_THESAURUS_TOOLTIPS_CHECK _T("ThesTooltips")
#define CFG_SPELLCHECK_DICTIONARY_NAME          _T("Dictionary")
#define CFG_SPELLCHECK_DICTIONARY_PATH          _T("DictPath")
#define CFG_SPELLCHECK_THESAURI_PATH            _T("ThesPath")
#define CFG_SPELLCHECK_BITMAPS_PATH             _T("BitmPath")

const wxString DEFAULT_ARRAY_SEP     = _T(";");
wxArrayString GetArrayFromString(const wxString& text, const wxString& separator = DEFAULT_ARRAY_SEP, bool trimSpaces = true)
{
    wxArrayString out;
    wxString search = text;
    size_t seplen = separator.Length();
    while (true)
    {
        int idx = search.Find(separator);
        if (idx == -1)
        {
            if (trimSpaces)
            {
                search.Trim(false);
                search.Trim(true);
            }
            if (!search.IsEmpty())
                out.Add(search);
            break;
        }
        wxString part = search.Left(idx);
        search.Remove(0, idx + seplen);
        if (trimSpaces)
        {
            part.Trim(false);
            part.Trim(true);
        }
        if (!part.IsEmpty())
            out.Add(part);
    }
    return out;
}
SpellCheckerManager::SpellCheckerManager()
    :selectedDictionary(-1)
{
    m_Config=wxConfigBase::Get(false);
    Load();
    ScanForDictionaries();
    PopulateLanguageNamesMap();
}
SpellCheckerManager::~SpellCheckerManager()
{
    //dtor
}
const wxString SpellCheckerManager::GetBitmapPath()const
{
    wxString bitmPath = m_BitmPath;
    if (wxDirExists(bitmPath) && !wxFindFirstFile(bitmPath + wxFILE_SEP_PATH + wxT("*.png"), wxFILE).IsEmpty())
        return bitmPath;
    return wxEmptyString;
}

void SpellCheckerManager::ScanForDictionaries()
{
    ScanForDictionaries(GetDictionaryPath());
}
void SpellCheckerManager::ScanForDictionaries(const wxString &path)
{
    m_dictionaries.clear();
    selectedDictionary = wxNOT_FOUND;
    //wxString filespec(_T("??_??.dic"));
    wxString filespec(_T("*.dic"));

    wxDir dir;
    if(path.IsEmpty())
    {
        m_EnableOnlineChecker = false;
        return;
    }
    if ( dir.Open(path) )
    {
        wxString strfilename;
        bool cont = dir.GetFirst(&strfilename, filespec, wxDIR_FILES );
        while ( cont )
        {
            wxFileName fname(strfilename);
            wxString afffilename = path + wxFileName::GetPathSeparator() + fname.GetName() + _T(".aff");
            if ( wxFileName::FileExists(afffilename) )
            {
                if ( fname.GetName() == m_strDictionaryName )
                    selectedDictionary = m_dictionaries.size();
                m_dictionaries.push_back(fname.GetName());

            }
            cont = dir.GetNext(&strfilename);
        }
    }
    // disable online checker if there are no dictionaries found
    if (selectedDictionary == wxNOT_FOUND)
    {
        m_EnableOnlineChecker = false;
    }
}

void SpellCheckerManager::DetectDictionaryPath()
{
    wxArrayString dictPaths;
    dictPaths.Add(m_DictPath);
    //Manager::Get()->GetMacrosManager()->ReplaceEnvVars(dictPaths[0]);
#ifdef __WXMSW__
    {
        wxString programs = wxT("C:\\Program Files");
        wxGetEnv(wxT("ProgramFiles"), &programs);
        dictPaths.Add(programs + wxT("\\Mozilla Firefox\\dictionaries"));
        dictPaths.Add(programs + wxT("\\Mozilla\\Firefox\\dictionaries"));
        dictPaths.Add(programs + wxT("\\Mozilla Thunderbird\\dictionaries"));
        dictPaths.Add(programs + wxT("\\Mozilla\\Thunderbird\\dictionaries"));
        wxString libreOffice = wxFindFirstFile(programs + wxT("\\*LibreOffice*"), wxDIR);
        wxString openOffice = wxFindFirstFile(programs + wxT("\\*OpenOffice*"), wxDIR);
        wxArrayString langs = GetArrayFromString(wxT("en;fr;es;de"));
        for (size_t i = 0; i < langs.GetCount(); ++i)
        {
            if (!libreOffice.IsEmpty())
                dictPaths.Add(libreOffice + wxT("\\share\\extensions\\dict-") + langs[i]);
            if (!openOffice.IsEmpty())
                dictPaths.Add(openOffice + wxT("\\share\\extensions\\dict-") + langs[i]);
        }
    }
#else
    {
        dictPaths.Add(wxT("/usr/share/hunspell"));
        dictPaths.Add(wxT("/usr/share/myspell"));
        dictPaths.Add(wxT("/usr/share/myspell/dicts"));
    }
#endif
	dictPaths.Add(g_MadEditHomeDir+wxT("Dictionaries"));

    //dictPaths.Add(m_pPlugin->GetOnlineCheckerConfigPath());
    for (size_t i = 0; i < dictPaths.GetCount(); ++i)
    {
        if (wxDirExists(dictPaths[i]) && !wxFindFirstFile(dictPaths[i] + wxFILE_SEP_PATH + wxT("*.dic"), wxFILE).IsEmpty())
        {
            if (i != 0)
                m_DictPath = dictPaths[i];
            break;
        }
    }
}

void SpellCheckerManager::DetectThesaurusPath()
{
    wxArrayString thesPaths;
    thesPaths.Add(m_ThesPath);
#ifdef __WXMSW__
    {
        wxString programs = wxT("C:\\Program Files");
        wxGetEnv(wxT("ProgramFiles"), &programs);
        wxString libreOffice = wxFindFirstFile(programs + wxT("\\*LibreOffice*"), wxDIR);
        wxString openOffice = wxFindFirstFile(programs + wxT("\\*OpenOffice*"), wxDIR);
        wxArrayString langs = GetArrayFromString(wxT("en;fr;es;de"));
        for (size_t i = 0; i < langs.GetCount(); ++i)
        {
            if (!libreOffice.IsEmpty())
                thesPaths.Add(libreOffice + wxT("\\share\\extensions\\dict-") + langs[i]);
            if (!openOffice.IsEmpty())
                thesPaths.Add(openOffice + wxT("\\share\\extensions\\dict-") + langs[i]);
        }
    }
#else
    {
        thesPaths.Add(wxT("/usr/share/myspell/dicts"));
        thesPaths.Add(wxT("/usr/share/mythes"));
    }
#endif
    //thesPaths.Add(m_pPlugin->GetOnlineCheckerConfigPath());
    for (size_t i = 0; i < thesPaths.GetCount(); ++i)
    {
        if (wxDirExists(thesPaths[i]) && !wxFindFirstFile(thesPaths[i] + wxFILE_SEP_PATH + wxT("th*.dat"), wxFILE).IsEmpty())
        {
            if (i != 0)
                m_ThesPath = thesPaths[i];
            break;
        }
    }
}
const wxString SpellCheckerManager::GetPersonalDictionaryFilename()const
{
    wxString dfile = _T("")/* = ConfigManager::LocateDataFile(GetDictionaryName() + _T("_personaldictionary.dic"), sdConfig )*/;
    /*if (dfile == _T(""))
        dfile = ConfigManager::GetFolder(sdConfig) + wxFILE_SEP_PATH + GetDictionaryName() + _T("_personaldictionary.dic");*/
    return dfile;
}
void SpellCheckerManager::Load()
{
    m_EnableOnlineChecker = true;
    const wxLanguageInfo* langInfo = wxLocale::GetLanguageInfo(wxLANGUAGE_DEFAULT); // current system locale
    if (langInfo)
        m_strDictionaryName = langInfo->CanonicalName;
    if (!m_strDictionaryName.StartsWith(_T("en"))) // default language is English (system designation preferred)
        m_strDictionaryName = _T("en_US");
    {
        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/SpellChecker"));
        wxString dictDir = g_MadEditHomeDir+wxT("Dictionaries");
        m_Config->Read(CFG_SPELLCHECK_ENABLE_ONLINE_CHECK, &m_EnableOnlineChecker, false);
        m_Config->Read(CFG_SPELLCHECK_ENABLE_PERSONAL_DICT, &m_EnablePersonalDictionary, false);
        m_Config->Read(CFG_SPELLCHECK_SPELL_TOOLTIPS_CHECK, &m_EnableSpellTooltips, false);
        m_Config->Read(CFG_SPELLCHECK_THESAURUS_TOOLTIPS_CHECK, &m_EnableThesaurusTooltips, false);
        m_Config->Read(CFG_SPELLCHECK_DICTIONARY_NAME, &m_strDictionaryName, _T("en_US"));
        m_Config->Read(CFG_SPELLCHECK_DICTIONARY_PATH, &m_DictPath, dictDir);
        if(!wxDir::Exists(m_DictPath)) m_DictPath = wxEmptyString;
        m_Config->Read(CFG_SPELLCHECK_THESAURI_PATH, &m_ThesPath, dictDir);
        if(!wxDir::Exists(m_ThesPath)) m_ThesPath = wxEmptyString;
        m_Config->Read(CFG_SPELLCHECK_BITMAPS_PATH, &m_BitmPath, dictDir);
        if(!wxDir::Exists(m_BitmPath)) m_BitmPath = wxEmptyString;
        m_Config->SetPath(oldpath);
    }

    DetectDictionaryPath();
    DetectThesaurusPath();
}
void SpellCheckerManager::Save()
{
    wxString oldpath=m_Config->GetPath();
    m_Config->SetPath(wxT("/SpellChecker"));
    m_Config->Write(CFG_SPELLCHECK_ENABLE_ONLINE_CHECK, m_EnableOnlineChecker);
    m_Config->Write(CFG_SPELLCHECK_ENABLE_PERSONAL_DICT, m_EnablePersonalDictionary);
    m_Config->Write(CFG_SPELLCHECK_SPELL_TOOLTIPS_CHECK, m_EnableSpellTooltips);
    m_Config->Write(CFG_SPELLCHECK_THESAURUS_TOOLTIPS_CHECK, m_EnableThesaurusTooltips);
    m_Config->Write(CFG_SPELLCHECK_DICTIONARY_NAME, m_strDictionaryName);
    m_Config->Write(CFG_SPELLCHECK_DICTIONARY_PATH, m_DictPath);
    m_Config->Write(CFG_SPELLCHECK_THESAURI_PATH, m_ThesPath);
    m_Config->Write(CFG_SPELLCHECK_BITMAPS_PATH, m_BitmPath);
    m_Config->SetPath(oldpath);
}
void SpellCheckerManager::PopulateLanguageNamesMap()
{
    m_LanguageNamesMap[_T("af_ZA")] = _T("Afrikaans (South Africa)");
    m_LanguageNamesMap[_T("bg_BG")] = _T("Bulgarian (Bulgaria)");
    m_LanguageNamesMap[_T("ca_ES")] = _T("Catalan (Spain)");
    m_LanguageNamesMap[_T("cz_CZ")] = _T("Czech (Czech Republic)");
    m_LanguageNamesMap[_T("cy_GB")] = _T("Welsh (Wales)");
    m_LanguageNamesMap[_T("da_DK")] = _T("Danish (Denmark)");
    m_LanguageNamesMap[_T("de_AT")] = _T("German (Austria)");
    m_LanguageNamesMap[_T("de_CH")] = _T("German (Switzerland)");
    m_LanguageNamesMap[_T("de_DE")] = _T("German (Germany-orig dict)");
    m_LanguageNamesMap[_T("de_DE_comb")] =_T("German (Germany-old & neu ortho)");
    m_LanguageNamesMap[_T("de_DE_neu")] =_T("German (Germany-neu ortho)");
    m_LanguageNamesMap[_T("el_GR")] = _T("Greek (Greece)");
    m_LanguageNamesMap[_T("en")]    = _T("English");
    m_LanguageNamesMap[_T("en_AU")] = _T("English (Australia)");
    m_LanguageNamesMap[_T("en_CA")] = _T("English (Canada)");
    m_LanguageNamesMap[_T("en_GB")] = _T("English (United Kingdom)");
    m_LanguageNamesMap[_T("en_NZ")] = _T("English (New Zealand)");
    m_LanguageNamesMap[_T("en_US")] = _T("English (United States)");
    m_LanguageNamesMap[_T("en_ZA")] = _T("English (South Africa)");
    m_LanguageNamesMap[_T("eo_l3")] = _T("Esperanto (Anywhere)");
    m_LanguageNamesMap[_T("es_ES")] = _T("Spanish (Spain-etal)");
    m_LanguageNamesMap[_T("es_MX")] = _T("Spanish (Mexico)");
    m_LanguageNamesMap[_T("fo_FO")] = _T("Faroese (Faroese Islands)");
    m_LanguageNamesMap[_T("fr_FR")] = _T("French (France)");
    m_LanguageNamesMap[_T("ga_IE")] = _T("Irish (Ireland)");
    m_LanguageNamesMap[_T("gd_GB")] = _T("Scottish Gaelic (Scotland)");
    m_LanguageNamesMap[_T("gl_ES")] = _T("Galician (Spain)");
    m_LanguageNamesMap[_T("he_IL")] = _T("Hebrew (Israel)");
    m_LanguageNamesMap[_T("hr_HR")] = _T("Croatian (Croatia)");
    m_LanguageNamesMap[_T("hu_HU")] = _T("Hungarian (Hungaria)");
    m_LanguageNamesMap[_T("ia")]    =_T("Interligua (x-register)");
    m_LanguageNamesMap[_T("id_ID")] = _T("Indonesian (Indonesia)");
    m_LanguageNamesMap[_T("it_IT")] = _T("Italian (Italy)");
    m_LanguageNamesMap[_T("la")]    =_T("Latin (x-register)");
    m_LanguageNamesMap[_T("lt_LT")] = _T("Lithuanian (Lithuania)");
    m_LanguageNamesMap[_T("lv_LV")] = _T("Latvian (Latvia)");
    m_LanguageNamesMap[_T("mg_MG")] = _T("Malagasy (Madagascar)");
    m_LanguageNamesMap[_T("mi_NZ")] = _T("Maori (New Zealand)");
    m_LanguageNamesMap[_T("ms_MY")] = _T("Malay (Malaysia)");
    m_LanguageNamesMap[_T("nb_NO")] = _T("Norwegian Bokmaal (Norway)");
    m_LanguageNamesMap[_T("nl_NL")] = _T("Dutch (Nederlands)");
    m_LanguageNamesMap[_T("nn_NO")] = _T("Norwegian Nynorsk (Norway)");
    m_LanguageNamesMap[_T("ny_MW")] = _T("Chichewa (Malawi)");
    m_LanguageNamesMap[_T("pl_PL")] = _T("Polish (Poland)");
    m_LanguageNamesMap[_T("pt_BR")] = _T("Portuguese (Brazil)");
    m_LanguageNamesMap[_T("pt_PT")] = _T("Portuguese (Portugal)");
    m_LanguageNamesMap[_T("ro_RO")] = _T("Romanian (Romania)");
    m_LanguageNamesMap[_T("ru_RU")] = _T("Russian (Russia)");
    m_LanguageNamesMap[_T("ru_RU_ye")] =_T("Russian ye (Russia)");
    m_LanguageNamesMap[_T("ru_RU_yo")] =_T("Russian yo (Russia)");
    m_LanguageNamesMap[_T("rw_RW")] = _T("Kinyarwanda (Rwanda)");
    m_LanguageNamesMap[_T("sk_SK")] = _T("Slovak (Slovakia)");
    m_LanguageNamesMap[_T("sl_SI")] = _T("Slovenian (Slovenia)");
    m_LanguageNamesMap[_T("sv_SW")] = _T("Swedish (Sweden)");
    m_LanguageNamesMap[_T("sw_KE")] = _T("Swahili (Kenya)");
    m_LanguageNamesMap[_T("tet_ID")] = _T("Tetum (Indonesia)");
    m_LanguageNamesMap[_T("tl_PH")] = _T("Tagalog (Philippines)");
    m_LanguageNamesMap[_T("tn_ZA")] = _T("Tswana (South Africa)");
    m_LanguageNamesMap[_T("uk_UA")] = _T("Ukrainian (Ukraine)");
    m_LanguageNamesMap[_T("zu_ZA")] = _T("Zulu (South Africa)");

    m_DictionaryNamesMap[_T("Afrikaans (South Africa)")        ] = _T("af_ZA");
    m_DictionaryNamesMap[_T("Bulgarian (Bulgaria)")            ] = _T("bg_BG");
    m_DictionaryNamesMap[_T("Catalan (Spain)")                 ] = _T("ca_ES");
    m_DictionaryNamesMap[_T("Czech (Czech Republic)")          ] = _T("cz_CZ");
    m_DictionaryNamesMap[_T("Welsh (Wales)")                   ] = _T("cy_GB");
    m_DictionaryNamesMap[_T("Danish (Denmark)")                ] = _T("da_DK");
    m_DictionaryNamesMap[_T("German (Austria)")                ] = _T("de_AT");
    m_DictionaryNamesMap[_T("German (Switzerland)")            ] = _T("de_CH");
    m_DictionaryNamesMap[_T("German (Germany-orig dict)")      ] = _T("de_DE");
    m_DictionaryNamesMap[_T("German (Germany-old & neu ortho)")] = _T("de_DE_comb");
    m_DictionaryNamesMap[_T("German (Germany-neu ortho)")      ] = _T("de_DE_neu");
    m_DictionaryNamesMap[_T("Greek (Greece)")                  ] = _T("el_GR");
    m_DictionaryNamesMap[_T("English")                         ] = _T("en");
    m_DictionaryNamesMap[_T("English (Australia)")             ] = _T("en_AU");
    m_DictionaryNamesMap[_T("English (Canada)")                ] = _T("en_CA");
    m_DictionaryNamesMap[_T("English (United Kingdom)")        ] = _T("en_GB");
    m_DictionaryNamesMap[_T("English (New Zealand)")           ] = _T("en_NZ");
    m_DictionaryNamesMap[_T("English (United States)")         ] = _T("en_US");
    m_DictionaryNamesMap[_T("English (South Africa)")          ] = _T("en_ZA");
    m_DictionaryNamesMap[_T("Esperanto (Anywhere)")            ] = _T("eo_l3");
    m_DictionaryNamesMap[_T("Spanish (Spain-etal)")            ] = _T("es_ES");
    m_DictionaryNamesMap[_T("Spanish (Mexico)")                ] = _T("es_MX");
    m_DictionaryNamesMap[_T("Faroese (Faroese Islands)")       ] = _T("fo_FO");
    m_DictionaryNamesMap[_T("French (France)")                 ] = _T("fr_FR");
    m_DictionaryNamesMap[_T("Irish (Ireland)")                 ] = _T("ga_IE");
    m_DictionaryNamesMap[_T("Scottish Gaelic (Scotland)")      ] = _T("gd_GB");
    m_DictionaryNamesMap[_T("Galician (Spain)")                ] = _T("gl_ES");
    m_DictionaryNamesMap[_T("Hebrew (Israel)")                 ] = _T("he_IL");
    m_DictionaryNamesMap[_T("Croatian (Croatia)")              ] = _T("hr_HR");
    m_DictionaryNamesMap[_T("Hungarian (Hungaria)")            ] = _T("hu_HU");
    m_DictionaryNamesMap[_T("Interligua (x-register)")         ] = _T("ia");
    m_DictionaryNamesMap[_T("Indonesian (Indonesia)")          ] = _T("id_ID");
    m_DictionaryNamesMap[_T("Italian (Italy)")                 ] = _T("it_IT");
    m_DictionaryNamesMap[_T("Latin (x-register)")              ] = _T("la");
    m_DictionaryNamesMap[_T("Lithuanian (Lithuania)")          ] = _T("lt_LT");
    m_DictionaryNamesMap[_T("Latvian (Latvia)")                ] = _T("lv_LV");
    m_DictionaryNamesMap[_T("Malagasy (Madagascar)")           ] = _T("mg_MG");
    m_DictionaryNamesMap[_T("Maori (New Zealand)")             ] = _T("mi_NZ");
    m_DictionaryNamesMap[_T("Malay (Malaysia)")                ] = _T("ms_MY");
    m_DictionaryNamesMap[_T("Norwegian Bokmaal (Norway)")      ] = _T("nb_NO");
    m_DictionaryNamesMap[_T("Dutch (Nederlands)")              ] = _T("nl_NL");
    m_DictionaryNamesMap[_T("Norwegian Nynorsk (Norway)")      ] = _T("nn_NO");
    m_DictionaryNamesMap[_T("Chichewa (Malawi)")               ] = _T("ny_MW");
    m_DictionaryNamesMap[_T("Polish (Poland)")                 ] = _T("pl_PL");
    m_DictionaryNamesMap[_T("Portuguese (Brazil)")             ] = _T("pt_BR");
    m_DictionaryNamesMap[_T("Portuguese (Portugal)")           ] = _T("pt_PT");
    m_DictionaryNamesMap[_T("Romanian (Romania)")              ] = _T("ro_RO");
    m_DictionaryNamesMap[_T("Russian (Russia)")                ] = _T("ru_RU");
    m_DictionaryNamesMap[_T("Russian ye (Russia)")             ] = _T("ru_RU_ye");
    m_DictionaryNamesMap[_T("Russian yo (Russia)")             ] = _T("ru_RU_yo");
    m_DictionaryNamesMap[_T("Kinyarwanda (Rwanda)")            ] = _T("rw_RW");
    m_DictionaryNamesMap[_T("Slovak (Slovakia)")               ] = _T("sk_SK");
    m_DictionaryNamesMap[_T("Slovenian (Slovenia)")            ] = _T("sl_SI");
    m_DictionaryNamesMap[_T("Swedish (Sweden)")                ] = _T("sv_SW");
    m_DictionaryNamesMap[_T("Swahili (Kenya)")                 ] = _T("sw_KE");
    m_DictionaryNamesMap[_T("Tetum (Indonesia)")               ] = _T("tet_ID");
    m_DictionaryNamesMap[_T("Tagalog (Philippines)")           ] = _T("tl_PH");
    m_DictionaryNamesMap[_T("Tswana (South Africa)")           ] = _T("tn_ZA");
    m_DictionaryNamesMap[_T("Ukrainian (Ukraine)")             ] = _T("uk_UA");
    m_DictionaryNamesMap[_T("Zulu (South Africa)")             ] = _T("zu_ZA");
}
wxString SpellCheckerManager::GetLanguageName(const wxString& language_id)
{
    if(language_id.empty())
        return language_id;

    std::map<wxString, wxString>::iterator it;
    // m_LanguageNamesMap[] is probably obsolete because of FindLanguageInfo()... consider removing m_LanguageNamesMap[]
    it = m_LanguageNamesMap.find(language_id);
    if (it != m_LanguageNamesMap.end() )
        return it->second;

    wxString id_fix = language_id;
    id_fix.Replace(wxT("-"), wxT("_")); // some dictionaries are distributed with hyphens

    it = m_LanguageNamesMap.find(id_fix);
    if (it != m_LanguageNamesMap.end() )
        return it->second;

    const wxLanguageInfo* langInfo = wxLocale::FindLanguageInfo(language_id); // ask wxWidgets if it knows the name
    if (langInfo)
        return langInfo->Description;
    langInfo = wxLocale::FindLanguageInfo(id_fix);
    if (langInfo)
        return langInfo->Description;

    id_fix = id_fix.BeforeLast(wxT('_')); // may be "*_v2", or root language may be known even if this specification is not

    it = m_LanguageNamesMap.find(id_fix);
    if (it != m_LanguageNamesMap.end() )
        return it->second + wxT(" (") + language_id + wxT(")"); // but may be incorrect, so specify the original name

    langInfo = wxLocale::FindLanguageInfo(id_fix);
    if (langInfo)
        return langInfo->Description + wxT(" (") + language_id + wxT(")");

    return language_id;
}

wxString SpellCheckerManager::GetDictionaryName(const wxString& dictDesc)
{
    if(dictDesc.empty())
        return dictDesc;

    std::map<wxString, wxString>::iterator it;
    // m_DictionaryNamesMap[] is probably obsolete because of FindLanguageInfo()... consider removing m_DictionaryNamesMap[]
    it = m_DictionaryNamesMap.find(dictDesc);
    if (it != m_DictionaryNamesMap.end() )
        return it->second;

    return dictDesc;
}

shared_ptr<wxSpellCheckEngineInterface> &SpellCheckerManager::GetSpellChecker()
{
    SpellCheckerMap::iterator it = m_SpellCheckerMap.find(m_strDictionaryName);
    if (it != m_SpellCheckerMap.end() )
        return it->second;
    std::pair<SpellCheckerMap::iterator, bool> ret;
    ret = m_SpellCheckerMap.insert(std::pair<wxString, shared_ptr<wxSpellCheckEngineInterface> >(m_strDictionaryName,
                                   shared_ptr<wxSpellCheckEngineInterface>(new HunspellInterface())));
    wxASSERT(ret.second);
    shared_ptr<wxSpellCheckEngineInterface> spellCheckerPtr = (ret.first)->second;

    SpellCheckEngineOption DictionaryFileOption(
        _T("dict-file"), _T("Dictionary File"),
        m_DictPath+wxFileName::GetPathSeparator()+m_strDictionaryName+wxT(".dic"), SpellCheckEngineOption::FILE
    );
    spellCheckerPtr->AddOptionToMap(DictionaryFileOption);
    SpellCheckEngineOption AffixFileOption(
        _T("affix-file"), _T("Affix File"),
        m_DictPath+wxFileName::GetPathSeparator()+m_strDictionaryName+wxT(".aff"), SpellCheckEngineOption::FILE
    );
    spellCheckerPtr->AddOptionToMap(AffixFileOption);
    spellCheckerPtr->ApplyOptions();
    spellCheckerPtr->InitializeSpellCheckEngine();
    spellCheckerPtr->SetEnablePersonalDictionary(m_EnablePersonalDictionary);

    return (ret.first)->second;
}

void SpellCheckerManager::SetEnablePersonalDictionary(bool val)
{
    m_EnablePersonalDictionary = val;
    SpellCheckerMap::iterator it = m_SpellCheckerMap.begin();
    while(it != m_SpellCheckerMap.end())
    {
		it->second->SetEnablePersonalDictionary(val);
        ++it;
    }
}

