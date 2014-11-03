#include "SpellCheckEngineInterface.h"
#include "SpellCheckUserInterface.h"

wxSpellCheckEngineInterface::wxSpellCheckEngineInterface()
{
    m_AlwaysReplaceMap.clear();
    m_AlwaysIgnoreList.IsEmpty();
    m_pSpellUserInterface = NULL;
    m_bPersonalDictionaryModified = false;
    m_bEngineInitialized = false;
}

wxSpellCheckEngineInterface::~wxSpellCheckEngineInterface()
{
    if (m_pSpellUserInterface != NULL)
    {
        delete m_pSpellUserInterface;
        m_pSpellUserInterface = NULL;
    }
}

void wxSpellCheckEngineInterface::SetSpellCheckUserInterface(wxSpellCheckUserInterface* pDlg)
{
    // delete the old user interface
    if (m_pSpellUserInterface != NULL)
        delete m_pSpellUserInterface;

    // set the new user interface and tell it that this is it's spell check engine
    m_pSpellUserInterface = pDlg;
    if (m_pSpellUserInterface != NULL)
        pDlg->SetSpellCheckEngine(this);
}

int wxSpellCheckEngineInterface::GetUserCorrection(const wxString& strMisspelling)  //Show function will show the dialog and not return until the user makes a decision
{
    // Populate the listbox with suggestions
    // At this point, we have to wait for user input.  This is where having the dialog and the spell checking as different classes would be handy
    int nLastAction = m_pSpellUserInterface->PresentSpellCheckUserInterface(strMisspelling);

    if (nLastAction == wxSpellCheckUserInterface::ACTION_REPLACE_ALWAYS)
    {
        m_AlwaysReplaceMap[m_pSpellUserInterface->GetMisspelledWord()] = m_pSpellUserInterface->GetReplacementText();
    }
    else if (nLastAction == wxSpellCheckUserInterface::ACTION_IGNORE_ALWAYS)
    {
        m_AlwaysIgnoreList.Add(m_pSpellUserInterface->GetMisspelledWord());
    }
    else if (nLastAction == wxSpellCheckUserInterface::ACTION_CLOSE)
    {
        return wxSpellCheckUserInterface::ACTION_CLOSE;
    }

    return ((nLastAction == wxSpellCheckUserInterface::ACTION_REPLACE) ||
            (nLastAction == wxSpellCheckUserInterface::ACTION_REPLACE_ALWAYS)) ? wxSpellCheckUserInterface::ACTION_REPLACE : wxSpellCheckUserInterface::ACTION_IGNORE;
}


void wxSpellCheckEngineInterface::DefineContext(const wxString& strText, long nOffset, long nLength)
{
    // This is kind of a kludge, but to determine the context of this word,
    //  grab the 50 characters before the nOffset and the 50 characters + nLength after nOffset
    // Then, do a Find from the front and the back of this 100 + nLength character string looking
    //  for a space character.  Trim off this space character and all characters preceding/after
    //  it based on if it's before or after the misspelled word.
    // Also, if there are NOT 50 characters before the nOffset don't bother trimming off at the space.
    // Likewise for less than 50 characters following the misspelled word.
    // Special allowances should be made to not have newline characters (\r or \n) in the context.

    if (strText.Length() < 50)
    {
        m_Context.SetContext(strText);
        m_Context.SetOffset(nOffset);
        m_Context.SetLength(nLength);
    }
    else
    {
        wxString strLocalText = strText;
        strLocalText.Replace(_T("\r"), _T(" "));
        strLocalText.Replace(_T("\n"), _T(" "));

        long nStartPosition = 0;
        bool bTrimFront = false;
        long nOffsetTrimmed = nOffset;
        if (nOffset > 50)
        {
            nStartPosition = nOffset - 50;
            nOffsetTrimmed = 50;
            bTrimFront = TRUE;
        }

        bool bTrimEnd = false;
        long nEndPosition = wxString::npos;
        if ((unsigned)(nStartPosition + nLength + 50) < strLocalText.Length())
        {
            nEndPosition = (nLength + 50);
            bTrimEnd = TRUE;
        }

        nEndPosition += nOffset - nStartPosition;  // Without this, we're only grabbing the number of characters for starting at the misspelled word
        wxString strContext;
        if ((unsigned)nEndPosition == wxString::npos)
            strContext = strLocalText.Mid(nStartPosition);
        else
            strContext = strLocalText.Mid(nStartPosition, nEndPosition);
        // Remove characters before the first space character
        if (bTrimFront)
        {
            if (strContext.Contains(_T(" ")))
            {
                nOffsetTrimmed -= strContext.Find(' ') + 1; // Figure out how much of the offset was trimmed off by remove the characters before the first space
                strContext = strContext.AfterFirst(' ');
            }
        }

        // Remove characters before the first space character
        if (bTrimEnd)
        {
            if (strContext.Contains(_T(" ")))
                strContext = strContext.BeforeLast(' ');
        }

        m_Context.SetContext(strContext);
        m_Context.SetOffset(nOffsetTrimmed);
        m_Context.SetLength(nLength);
    }
}

bool wxSpellCheckEngineInterface::AddOptionToMap(SpellCheckEngineOption& option)
{
    // Return a boolean indicating whether or now the spell check engine options actually changed
    bool bOptionsChanged = false;
    wxString strOptionName = option.GetName();

    if (!strOptionName.IsEmpty())
    {
        OptionsMap::iterator it = m_Options.find(strOptionName);
        // If either the option isn't in the map yet or the value is different, then update the options
        if ((it == m_Options.end()) || (it->second.GetValueAsString() != option.GetValueAsString()))
        {
            m_Options[strOptionName] = option;
            bOptionsChanged = true;
        }
    }
    return bOptionsChanged;
}

void wxSpellCheckEngineInterface::ApplyOptions()
{
    for (OptionsMap::iterator it = m_Options.begin(); it != m_Options.end(); it++)
    {
        SetOption(it->second);
    }
}

void wxSpellCheckEngineInterface::ShowOption(const wxString& strOption, bool bShow /*= true*/)
{
    OptionsMap::iterator it = m_Options.find(strOption);
    if (it != m_Options.end())
        it->second.SetShowOption(bShow);
}

const wxCharBuffer wxSpellCheckEngineInterface::ConvertToUnicode(const wxString& inputString)
{
#if wxUSE_UNICODE
    wxString strEncoding = GetCharacterEncoding();
    if (strEncoding != wxEmptyString)
    {
        wxCSConv conv(strEncoding);
        wxCharBuffer returnBuffer(conv.cWC2MB(inputString.wc_str(*wxConvCurrent)));
        return returnBuffer;
    }
    else
    {
        wxCharBuffer returnBuffer(wxConvUTF8.cWC2MB(inputString.wc_str(*wxConvCurrent)));
        return returnBuffer;
    }
#else
    wxCharBuffer returnBuffer = inputString.c_str();
    return returnBuffer;
#endif
}

wxString wxSpellCheckEngineInterface::ConvertFromUnicode(const char* inputBuffer)
{
#if wxUSE_UNICODE
    wxString strEncoding = GetCharacterEncoding();
    if (strEncoding != wxEmptyString)
    {
        wxCSConv conv(strEncoding);
        wxString returnString(conv.cMB2WC(inputBuffer), *wxConvCurrent);
        return returnString;
    }
    else
    {
        wxString returnString(wxConvUTF8.cMB2WC(inputBuffer), *wxConvCurrent);
        return returnString;
    }
#else
    wxString returnString(inputBuffer);
    return returnString;
#endif
}

