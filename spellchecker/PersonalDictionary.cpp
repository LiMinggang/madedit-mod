#include "PersonalDictionary.h"

#include <wx/filename.h>
#include <wx/textfile.h>

PersonalDictionary::PersonalDictionary(wxString strFileName /*= _T(".wxSpellCheckerPersonalDictionary")*/) : m_bEnableSort(true)
{
    m_strDictionaryFileName = strFileName;
    m_DictionaryWords.Clear();
}

PersonalDictionary::~PersonalDictionary()
{
}

bool PersonalDictionary::LoadPersonalDictionary()
{
    wxFileName sPath(m_strDictionaryFileName);
    sPath.MakeAbsolute();

    wxTextFile DictFile(sPath.GetFullPath());
    if (!DictFile.Exists())
    {
        return false;
    }
    if (!DictFile.Open())
    {
        wxMessageOutput* msgOut = wxMessageOutput::Get();
        if (msgOut)
            msgOut->Printf(_T("Unable to open personal dictionary file\n"));
        return false;
    }

    m_DictionaryWords.Clear();
    if (DictFile.GetLineCount() > 0)
    {
        wxString strWord;
        for ( strWord = DictFile.GetFirstLine(); !DictFile.Eof(); strWord = DictFile.GetNextLine() )
        {
            strWord.Trim(FALSE); // Trim on the left
            strWord.Trim(TRUE);  // Trim on the right
            if (strWord.IsEmpty() || strWord == _T(";"))
                continue;

            m_DictionaryWords.Add(strWord.Lower());
        }
        // Handle the last line
        strWord.Trim(FALSE); // Trim on the left
        strWord.Trim(TRUE);  // Trim on the right

        if (!(strWord.IsEmpty()) && (strWord != _T(";")))
            m_DictionaryWords.Add(strWord);
    }

    DictFile.Close();
    return true;
}

bool PersonalDictionary::SavePersonalDictionary()
{
    wxFileName sPath(m_strDictionaryFileName);
    sPath.MakeAbsolute();

    wxTextFile DictFile(sPath.GetFullPath());

    // Remove any existing personal dictionary files
    if (DictFile.Exists())
        ::wxRemoveFile(sPath.GetFullPath());

    if (!DictFile.Create())
    {
        wxMessageOutput* msgOut = wxMessageOutput::Get();
        if (msgOut)
            msgOut->Printf(_T("Unable to open personal dictionary file\n"));
        return false;
    }

    //DictFile.Clear();
    for (size_t i=0; i<m_DictionaryWords.GetCount(); i++)
        DictFile.AddLine(m_DictionaryWords[i]);

    DictFile.Write();
    DictFile.Close();
    return true;
}

bool PersonalDictionary::IsWordInDictionary(const wxString& strWord)
{
    wxString word = strWord.Lower();
    if(word.IsEmpty()) return false;
    return (m_DictionaryWords.Index(strWord.Lower()) != wxNOT_FOUND);
}

void PersonalDictionary::AddWord(const wxString& strWord)
{
    wxString word = strWord.Lower();
    if(word.IsEmpty()) return;
    m_DictionaryWords.Add(strWord.Lower());
	if(m_bEnableSort)
    	m_DictionaryWords.Sort();
}

void PersonalDictionary::RemoveWord(const wxString& strWord)
{
    wxString word = strWord.Lower();
    if(word.IsEmpty()) return;
    m_DictionaryWords.Remove(strWord.Lower());
}

wxArrayString PersonalDictionary::GetWordListAsArray()
{
    return m_DictionaryWords;
}

