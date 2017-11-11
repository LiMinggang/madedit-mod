#ifndef __SPELL_CHECK_ENGINE_INTERFACE__
#define __SPELL_CHECK_ENGINE_INTERFACE__
#ifndef _MSC_VER 
#if __cplusplus <= 199711L
#define CPLUSEPLUSE98 1
#ifndef nullptr
#define nullptr (0)
#endif
#endif
#else
#if _MSC_VER < 1800
#define CPLUSEPLUSE98 1
#ifndef nullptr
#define nullptr (0)
#endif
#endif
#endif
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/hashmap.h>
#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
#include <wx/string.h>

#include "SpellCheckEngineOption.h"
// Get rid of the warning about identifiers being truncated in the debugger.  Using the STL collections
// will produce this everywhere.  Must disable at beginning of stdafx.h because it doesn't work if
// placed elsewhere.
#ifdef __VISUALC__
#pragma warning(disable:4786)
#endif

#if CPLUSEPLUSE98
#include <boost/shared_ptr.hpp>
#ifndef nullptr
	#define nullptr (0)
#endif
#else
#include <memory>
#endif

WX_DECLARE_STRING_HASH_MAP(wxString, StringToStringMap);
WX_DECLARE_STRING_HASH_MAP(SpellCheckEngineOption, OptionsMap);

class wxSpellCheckUserInterface;
class PersonalDictionary;

class wxSpellCheckEngineInterface
{
public:
    wxSpellCheckEngineInterface();
    virtual ~wxSpellCheckEngineInterface();

protected:
    StringToStringMap m_AlwaysReplaceMap; // We need a map here to store the misspelled word and its replacement
    wxArrayString m_AlwaysIgnoreList;
    bool m_bPersonalDictionaryModified;
    bool m_bEngineInitialized;

    // Create a pointer to the dialog to present.
    // The main reason that this is here rather than created only if needed, is that I wanted repeat appearances
    //  of this dialog to remember if the user moved the dialog.
    wxSpellCheckUserInterface* m_pSpellUserInterface;
    OptionsMap m_Options;

public:
    // Base class administration functions
    void SetSpellCheckUserInterface(wxSpellCheckUserInterface* pDlg);
    wxSpellCheckUserInterface* GetSpellCheckUserInterface() const
    {
        return m_pSpellUserInterface;
    }

    // Spell Checker functions

    // Spell Checker Engine Administration functions
    virtual wxString GetSpellCheckEngineName() = 0;
    virtual int InitializeSpellCheckEngine() = 0;
    virtual int UninitializeSpellCheckEngine() = 0;
    virtual int SetOption(SpellCheckEngineOption& Option) = 0;
#if CPLUSEPLUSE98
    virtual void SetSyntaxDictionary(boost::shared_ptr<PersonalDictionary>& syntaxKeywordDict) = 0;
#else
    virtual void SetSyntaxDictionary(std::shared_ptr<PersonalDictionary>& syntaxKeywordDict) = 0;
#endif
    virtual bool AddOptionToMap(SpellCheckEngineOption& option);
    virtual void ApplyOptions();  // Go through all the options in the options map and apply them to the spell check engine
    OptionsMap* GetOptions()
    {
        return &m_Options;
    }
    virtual void UpdatePossibleValues(SpellCheckEngineOption& WXUNUSED(OptionDependency), SpellCheckEngineOption& WXUNUSED(OptionToUpdate))
    {
        wxASSERT(false);
    }
    bool IsInitialized()
    {
        return m_bEngineInitialized;
    }

    // Spell Checker Search/Suggestion functions
    virtual bool IsWordInDictionary(const wxString& strWord) = 0;
    virtual wxString CheckSpelling(wxString strText) = 0;
    virtual bool IsSpellingOk(wxString strText) = 0;
    virtual wxArrayString GetSuggestions(const wxString& strMisspelledWord) = 0;

    // Spell Checker Dictionary Manipulation functions (the above functions work on a read-only dictionary
    //  but these functions require write-access or usage of a personal dictionary
    virtual bool IsWordInPersonalDictionary(const wxString& strWord) = 0;
    virtual int AddWordToDictionary(const wxString& strWord) = 0;
    virtual int RemoveWordFromDictionary(const wxString& strWord) = 0;
    virtual wxArrayString GetWordListAsArray() = 0;
    bool IsPersonalDictionaryModified()
    {
        return m_bPersonalDictionaryModified;
    }
    virtual int GetUserCorrection(const wxString& strMisspelling);
    virtual void DefineContext(const wxString& strText, long nOffset, long nLength);
    void ShowOption(const wxString& strOption, bool bShow = true);

    // Helper functions for Unicode compatibility
    const wxCharBuffer ConvertToUnicode(const wxString& inputString);
    wxString ConvertFromUnicode(const char* inputBuffer);

    virtual wxString GetCharacterEncoding() = 0;
    virtual void SetEnablePersonalDictionary(bool enable) = 0;

    class MisspellingContext
    {
    protected:
        wxString m_strContext;  // fragment of the document containing word prior to and following the misspelled word
        long m_nOffset;         // zero based offset into the strContext where the misspell word begins
        long m_nLength;         // length of the misspelled word within strContext

    public:
        MisspellingContext()
        {
            m_strContext = _T("");
            m_nOffset = 0;
            m_nLength = 0;
        }
        wxString GetContext()
        {
            return m_strContext;
        }
        long GetOffset()
        {
            return m_nOffset;
        }
        long GetLength()
        {
            return m_nLength;
        }
        void SetContext(const wxString& strContext)
        {
            m_strContext = strContext;
        }
        void SetOffset(long nOffset)
        {
            m_nOffset = nOffset;
        }
        void SetLength(long nLength)
        {
            m_nLength = nLength;
        }
    };

    virtual MisspellingContext GetCurrentMisspellingContext()
    {
        return m_Context;
    }

protected:
    MisspellingContext m_Context;
};

#endif  // __SPELL_CHECK_ENGINE_INTERFACE__
