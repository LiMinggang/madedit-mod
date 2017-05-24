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
#include "hunspell/hunspell.hxx"
#include "HunspellInterface.h"

#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/textfile.h>
#include <wx/config.h>

HunspellInterface::HunspellInterface(wxSpellCheckUserInterface* pDlg /* = nullptr */)
{
    m_pSpellUserInterface = pDlg;

    if (m_pSpellUserInterface != nullptr)
        m_pSpellUserInterface->SetSpellCheckEngine(this);

    m_pHunspell = nullptr;
    m_bPersonalDictionaryModified = false;
    m_EnablePersonalDictionary = false;
}

HunspellInterface::~HunspellInterface()
{
    if (m_bPersonalDictionaryModified)
    {
        //if (wxYES == ::wxMessageBox(_T("Would you like to save any of your changes to your personal dictionary?"), _T("Save Changes"), wxYES_NO | wxICON_QUESTION))
        m_PersonalDictionary.SavePersonalDictionary();
    }

    UninitializeSpellCheckEngine();

    delete m_pSpellUserInterface;
    m_pSpellUserInterface = nullptr;
}

int HunspellInterface::InitializeSpellCheckEngine()
{
    UninitializeSpellCheckEngine();

    wxString strAffixFile = GetAffixFileName();
    wxString strDictionaryFile = GetDictionaryFileName();

    if ( wxFileName::FileExists(strAffixFile) && wxFileName::FileExists(strDictionaryFile) )
    {
        wxCharBuffer affixFileCharBuffer = ConvertToUnicode(strAffixFile);
        wxCharBuffer dictionaryFileCharBuffer = ConvertToUnicode(strDictionaryFile);
        m_pHunspell = new Hunspell(affixFileCharBuffer, dictionaryFileCharBuffer);
    }

    m_bEngineInitialized = (m_pHunspell != nullptr);
    return m_bEngineInitialized;
}

int HunspellInterface::UninitializeSpellCheckEngine()
{
    wxDELETE(m_pHunspell);
    m_bEngineInitialized = false;
    return true;
}

int HunspellInterface::SetOption(SpellCheckEngineOption& Option)
{
    // MySpell doesn't really have any options that I know of other than the affix and
    // dictionary files.  To change those, a new MySpell instance must be created though

    // First make sure that either the affix or dict file have changed
    if (Option.GetName() == _T("dictionary-path"))
    {
        // Dictionary path and language are now invalid, so clear them out
        m_Options.erase(_T("dict-file"));
        m_Options.erase(_T("affix-file"));

        m_strDictionaryPath = Option.GetValueAsString();
        PopulateDictionaryMap(&m_DictionaryLookupMap, m_strDictionaryPath);
        //return true;  // Even though the option didn't change, it isn't an error, so return true
    }
    else if (Option.GetName() == _T("language"))
    {
        m_Options.erase(_T("dict-file"));
        m_Options.erase(_T("affix-file"));

        //return true;  // Even though the option didn't change, it isn't an error, so return true
    }
    else if (Option.GetName() == _T("affix-file"))
    {
        // Dictionary path and language are now invalid, so clear them out
        m_strDictionaryPath = _T("");
        m_Options.erase(_T("dictionary-path"));
        m_Options.erase(_T("language"));
    }
    else if (Option.GetName() == _T("dict-file"))
    {
        // Dictionary path and language are now invalid, so clear them out
        m_strDictionaryPath = _T("");
        m_Options.erase(_T("dictionary-path"));
        m_Options.erase(_T("language"));
    }
    else
        return false; // We don't understand this option so return the error

    // We'll something changed so tear down the old spell check engine and create a new one
    return InitializeSpellCheckEngine();
}

bool HunspellInterface::IsSpellingOk(wxString strText)
{
    if (m_pHunspell == nullptr)
        return false;

    //int nDiff = 0;

    strText += _T(" ");

    wxString strDelimiters = _T(" \t\r\n.,?!@#$%^&*()-=_+[]{}\\|;:\"'`<>/~0123456789");
    wxStringTokenizer tkz(strText, strDelimiters);
    while ( tkz.HasMoreTokens() )
    {
        wxString token = tkz.GetNextToken();
        //int TokenStart = tkz.GetPosition() - token.Length() - 1;
        //TokenStart += nDiff;  // Take into account any changes to the size of the strText

        // process token here
        if (!IsWordInDictionary(token))
        {
            // If this word is in the always ignore list, then just move on
            if (m_AlwaysIgnoreList.Index(token) != wxNOT_FOUND)
                continue;
            return false;
        }
    }

    return true;
}

wxString HunspellInterface::CheckSpelling(wxString strText)
{
    if (m_pHunspell == nullptr)
        return wxEmptyString;

    int nDiff = 0;

    strText += _T(" ");

    wxString strDelimiters = _T(" \t\r\n.,?!@#$%^&*()-=_+[]{}\\|;:\"<>/~0123456789");
    wxStringTokenizer tkz(strText, strDelimiters);
    while ( tkz.HasMoreTokens() )
    {
        wxString token = tkz.GetNextToken();
        int TokenStart = tkz.GetPosition() - token.Length() - 1;
        TokenStart += nDiff;  // Take into account any changes to the size of the strText

        // process token here
        if (!IsWordInDictionary(token))
        {
            // If this word is in the always ignore list, then just move on
            if (m_AlwaysIgnoreList.Index(token) != wxNOT_FOUND)
                continue;

            /* dealt with by IsWordInDictionary - JACS
                   // If this word is in the personal dictionary, then just move on
                   if (m_PersonalDictionary.IsWordInDictionary(token))
                     continue;
            */

            bool bReplaceFromMap = false;
            StringToStringMap::iterator WordFinder = m_AlwaysReplaceMap.find(token);
            if (WordFinder != m_AlwaysReplaceMap.end())
                bReplaceFromMap = true;

            int nUserReturnValue = 0;

            if (!bReplaceFromMap)
            {
                // Define the context of the word
                DefineContext(strText, TokenStart, token.Length());

                // Print out the misspelling and get a replasment from the user
                // Present the dialog so the user can tell us what to do with this word
                nUserReturnValue = GetUserCorrection(token);  //Show function will show the dialog and not return until the user makes a decision
            }

            if (nUserReturnValue == wxSpellCheckUserInterface::ACTION_CLOSE)
            {
                break;
            }
            else if ((nUserReturnValue == wxSpellCheckUserInterface::ACTION_REPLACE) || bReplaceFromMap)
            {
                wxString strReplacementText = (bReplaceFromMap) ? (*WordFinder).second : m_pSpellUserInterface->GetReplacementText();
                // Increase/Decreate the character difference so that the next loop is on track
                nDiff += strReplacementText.Length() - token.Length();
                // Replace the misspelled word with the replacement */
                strText.replace(TokenStart, token.Length(), strReplacementText);
            }
        }
    }

    strText = strText.Left(strText.Len() - 1);

    return strText;
}

wxArrayString HunspellInterface::GetSuggestions(const wxString& strMisspelledWord)
{
    wxArrayString wxReturnArray;
    wxReturnArray.Empty();

    if (m_pHunspell)
    {
        wxCharBuffer misspelledWordCharBuffer = ConvertToUnicode(strMisspelledWord);
        if ( misspelledWordCharBuffer.data() != nullptr)
        {
            std::vector<std::string> wlst;
            std::string misspelledWord(misspelledWordCharBuffer.data());
            wlst = m_pHunspell->suggest(misspelledWord);
            for (size_t i=0; i < wlst.size(); i++)
            {
                wxReturnArray.Add(ConvertFromUnicode(wlst[i].c_str()));
            }
        }
    }

    return wxReturnArray;
}

bool HunspellInterface::IsWordInDictionary(const wxString& strWord)
{
    if (m_pHunspell == nullptr)
        return false;

    wxCharBuffer wordCharBuffer = ConvertToUnicode(strWord);
    if ( wordCharBuffer.data() == nullptr )
        return false;
    std::string word(wordCharBuffer.data());
    return ((m_pHunspell->spell(word)) || (m_SyntaxKeywordDict && m_SyntaxKeywordDict->IsWordInDictionary(strWord)) || (m_PersonalDictionary.IsWordInDictionary(strWord)) );
}

bool HunspellInterface::IsWordInPersonalDictionary(const wxString& strWord)
{
    if (m_pHunspell == nullptr)
        return false;

    wxCharBuffer wordCharBuffer = ConvertToUnicode(strWord);
    if ( wordCharBuffer.data() == nullptr )
        return false;
    return (m_PersonalDictionary.IsWordInDictionary(strWord));
}

int HunspellInterface::AddWordToDictionary(const wxString& strWord)
{
    m_PersonalDictionary.AddWord(strWord);
    m_bPersonalDictionaryModified = true;
    return true;
}

int HunspellInterface::RemoveWordFromDictionary(const wxString& strWord)
{
    m_PersonalDictionary.RemoveWord(strWord);
    m_bPersonalDictionaryModified = true;
    return true;
}

wxArrayString HunspellInterface::GetWordListAsArray()
{
    return m_PersonalDictionary.GetWordListAsArray();
}

// Since MySpell doesn't have a concept of a personal dictionary, we can create a file
// to hold new words and if spell check fails then we check this map before asking the user
// It's not the best (as it won't support the affix feature of MySpell), but it'll work

void HunspellInterface::PopulateDictionaryMap(StringToStringMap* pLookupMap, const wxString& strDictionaryPath)
{
    if (pLookupMap == nullptr)
        pLookupMap = &m_DictionaryLookupMap;

    pLookupMap->clear();

    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Afrikaans (South Africa)"), _T("af_ZA"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Bulgarian (Bulgaria)"), _T("bg_BG"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Catalan (Spain)"), _T("ca_ES"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Czech (Czech Republic)"), _T("cs_CZ"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Welsh (Wales)"), _T("cy_GB"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Danish (Denmark)"), _T("da_DK"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("German (Austria)"), _T("de_AT"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("German (Switzerland)"), _T("de_CH"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("German (Germany-orig dict)"), _T("de_DE"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("German (Germany-old & neu ortho)"), _T("de_DE_comb"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("German (Germany-neu ortho)"), _T("de_DE_neu"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Greek (Greece)"), _T("el_GR"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("English (Australia)"), _T("en_AU"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("English (Canada)"), _T("en_CA"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("English (United Kingdom)"), _T("en_GB"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("English (New Zealand)"), _T("en_NZ"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("English (United States)"), _T("en_US"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Esperanto (anywhere)"), _T("eo_l3"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Spanish (Spain-etal)"), _T("es_ES"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Spanish (Mexico)"), _T("es_MX"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Faroese (Faroe Islands)"), _T("fo_FO"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("French (France)"), _T("fr_FR"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Irish (Ireland)"), _T("ga_IE"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Scottish Gaelic (Scotland)"), _T("gd_GB"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Galician (Spain)"), _T("gl_ES"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Hebrew (Israel)"), _T("he_IL"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Croatian (Croatia)"), _T("hr_HR"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Hungarian (Hungary)"), _T("hu_HU"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Interlingua (x-register)"), _T("ia"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Indonesian (Indonesia)"), _T("id_ID"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Italian (Italy)"), _T("it_IT"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Kurdish (Turkey)"), _T("ku_TR"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Latin (x-register)"), _T("la"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Lithuanian (Lithuania)"), _T("lt_LT"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Latvian (Latvia)"), _T("lv_LV"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Malagasy (Madagascar)"), _T("mg_MG"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Maori (New Zealand)"), _T("mi_NZ"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Malay (Malaysia)"), _T("ms_MY"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Norwegian Bokmaal (Norway)"), _T("nb_NO"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Dutch (Netherlands)"), _T("nl_NL"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Norwegian Nynorsk (Norway)"), _T("nn_NO"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Chichewa (Malawi)"), _T("ny_MW"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Polish (Poland)"), _T("pl_PL"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Portuguese (Brazil)"), _T("pt_BR"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Portuguese (Portugal)"), _T("pt_PT"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Romanian (Romania)"), _T("ro_RO"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Russian (Russia)"), _T("ru_RU"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Russian ye (Russia)"), _T("ru_RU_ie"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Russian yo (Russia)"), _T("ru_RU_yo"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Kinyarwanda (Rwanda)"), _T("rw_RW"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Slovak (Slovakia)"), _T("sk_SK"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Slovenian (Slovenia)"), _T("sl_SI"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Swedish (Sweden)"), _T("sv_SE"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Swahili (Kenya)"), _T("sw_KE"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Tetum (Indonesia)"), _T("tet_ID"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Tagalog (Philippines)"), _T("tl_PH"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Tswana (South Africa)"), _T("tn_ZA"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Ukrainian (Ukraine)"), _T("uk_UA"));
    AddDictionaryElement(pLookupMap, strDictionaryPath, _T("Zulu (South Africa)"), _T("zu_ZA"));

    // Add the custom MySpell dictionary entries to the map
    StringToStringMap::iterator start = m_CustomMySpellDictionaryMap.begin();
    StringToStringMap::iterator stop = m_CustomMySpellDictionaryMap.end();
    while (start != stop)
    {
        AddDictionaryElement(pLookupMap, strDictionaryPath, (*start).first, (*start).second);
        ++start;
    }
}

void HunspellInterface::UpdatePossibleValues(SpellCheckEngineOption& OptionDependency, SpellCheckEngineOption& OptionToUpdate)
{
    if ((OptionDependency.GetName().IsSameAs(_T("dictionary-path"))) && (OptionToUpdate.GetName().IsSameAs(_T("language"))))
    {
        StringToStringMap tempLookupMap;
        wxString strDictionaryPath = OptionDependency.GetValueAsString();
        PopulateDictionaryMap(&tempLookupMap, strDictionaryPath);

        StringToStringMap::iterator start = tempLookupMap.begin();
        StringToStringMap::iterator stop = tempLookupMap.end();
        while (start != stop)
        {
            OptionToUpdate.AddPossibleValue((*start).first);
            ++start;
        }
    }
    else
    {
        wxMessageOutput* msgOut = wxMessageOutput::Get();
        if (msgOut)
            msgOut->Printf(_("Unsure how to update the possible values for %s based on the value of %s"), OptionDependency.GetText().c_str(), OptionToUpdate.GetText().c_str());
    }
}

void HunspellInterface::AddDictionaryElement(StringToStringMap* pLookupMap, const wxString& strDictionaryPath, const wxString& strDictionaryName, const wxString& strDictionaryFileRoot)
{
    wxFileName strAffixFileName(strDictionaryPath + wxFILE_SEP_PATH + strDictionaryFileRoot + _T(".aff"));
    wxFileName strDictionaryFileName(strDictionaryPath + wxFILE_SEP_PATH + strDictionaryFileRoot + _T(".dic"));
    if (strAffixFileName.FileExists() && strDictionaryFileName.FileExists())
    {
        (*pLookupMap)[strDictionaryName] = strDictionaryFileRoot;
    }
}

wxString HunspellInterface::GetSelectedLanguage()
{
    OptionsMap::iterator it = m_Options.find(_T("language"));
    if (it != m_Options.end())
    {
        return it->second.GetValueAsString();
    }
    else
    {
        return wxEmptyString;
    }
}

wxString HunspellInterface::GetAffixFileName()
{
    OptionsMap::iterator it = m_Options.find(_T("affix-file"));
    if (it != m_Options.end())
    {
        return it->second.GetValueAsString();
    }
    else
    {
        wxString strLanguage = GetSelectedLanguage();
        if (strLanguage != wxEmptyString)
        {
            return GetAffixFileName(strLanguage);
        }
        else
        {
            return wxEmptyString;
        }
    }
}

wxString HunspellInterface::GetAffixFileName(const wxString& strDictionaryName)
{
    StringToStringMap::iterator finder = m_DictionaryLookupMap.find(strDictionaryName);
    if (finder != m_DictionaryLookupMap.end())
    {
        return (m_strDictionaryPath + wxFILE_SEP_PATH + (*finder).second + _T(".aff"));
    }
    else
    {
        return wxEmptyString;
    }
}

wxString HunspellInterface::GetDictionaryFileName()
{
    OptionsMap::iterator it = m_Options.find(_T("dict-file"));
    if (it != m_Options.end())
    {
        return it->second.GetValueAsString();
    }
    else
    {
        wxString strLanguage = GetSelectedLanguage();
        if (strLanguage != wxEmptyString)
        {
            return GetDictionaryFileName(strLanguage);
        }
        else
        {
            return wxEmptyString;
        }
    }
}

wxString HunspellInterface::GetDictionaryFileName(const wxString& strDictionaryName)
{
    StringToStringMap::iterator finder = m_DictionaryLookupMap.find(strDictionaryName);
    if (finder != m_DictionaryLookupMap.end())
    {
        return (m_strDictionaryPath + wxFILE_SEP_PATH + (*finder).second + _T(".dic"));
    }
    else
    {
        return wxEmptyString;
    }
}

void HunspellInterface::AddCustomMySpellDictionary(const wxString& strDictionaryName, const wxString& strDictionaryFileRoot)
{
    m_CustomMySpellDictionaryMap[strDictionaryName] = strDictionaryFileRoot;
}

void HunspellInterface::OpenPersonalDictionary(const wxString& strPersonalDictionaryFile)
{
	m_PersonalDictionary.SetDictionaryFileName(m_strDictionaryPath + wxFILE_SEP_PATH + strPersonalDictionaryFile);
    m_PersonalDictionary.LoadPersonalDictionary();
}

wxString HunspellInterface::GetCharacterEncoding()
{
    if (m_pHunspell == nullptr)
        return wxEmptyString;

    wxString encoding(wxConvUTF8.cMB2WC(m_pHunspell->get_dic_encoding()), *wxConvCurrent);
    return encoding;
}

int HunspellInterface::GetUserCorrection(const wxString& strMisspelling)
{
    m_AlwaysIgnoreList.Add(strMisspelling);
    return wxSpellCheckUserInterface::ACTION_IGNORE_ALWAYS;
}

void HunspellInterface::SetEnablePersonalDictionary(bool enable)
{
    if(enable)
    {
        m_EnablePersonalDictionary = true;

        wxFileName sPath(GetDictionaryFileName());
        sPath.MakeAbsolute();
        wxString dictName = wxT("MadDict.")+sPath.GetName();
        m_PersonalDictionary.SetDictionaryFileName(sPath.GetPath() + wxFILE_SEP_PATH + dictName);
        wxTextFile DictFile(m_PersonalDictionary.GetDictionaryFileName());
        if (!DictFile.Exists())
        {
            DictFile.Create();
        }
        m_PersonalDictionary.LoadPersonalDictionary();
    }
    else
    {
        m_EnablePersonalDictionary = false;
        if (m_bPersonalDictionaryModified)
        {
            //if (wxYES == ::wxMessageBox(_T("Would you like to save any of your changes to your personal dictionary?"), _T("Save Changes"), wxYES_NO | wxICON_QUESTION))
            m_PersonalDictionary.SavePersonalDictionary();
            m_bPersonalDictionaryModified = false;
        }
    }
}

///////////// Options /////////////////
// "dictionary-path" - location of dictionary files
// "language" - selected language
//
// - OR -
//
// "dict-file" - dictionary file
// "affix-file" - affix file

