#ifndef __SPELL_CHECK_ENGINE_OPTION__
#define __SPELL_CHECK_ENGINE_OPTION__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dynarray.h>
#include <wx/variant.h>

WX_DECLARE_OBJARRAY(wxVariant, VariantArray);

class SpellCheckEngineOption
{
public:
    enum
    {
        UNDEFINED = 0,
        STRING = 1,
        LONG = 2,
        DOUBLE = 3,
        BOOLEAN = 4,
        DIR = 5,
        FILE = 6
    };

    SpellCheckEngineOption();
    SpellCheckEngineOption(wxString strName);
    SpellCheckEngineOption(wxString strName, wxString strText);
    SpellCheckEngineOption(wxString strName, wxString strText, wxString strValue, int nType = SpellCheckEngineOption::STRING);
    SpellCheckEngineOption(wxString strName, wxString strText, long nValue);
    SpellCheckEngineOption(wxString strName, wxString strText, double dblValue);
    SpellCheckEngineOption(wxString strName, wxString strText, bool bValue);

    void SetName(wxString strName)
    {
        m_strOptionName = strName;
        if (m_strDialogText.IsEmpty()) m_strDialogText = strName;
    }
    wxString GetName()
    {
        return m_strOptionName;
    }
    void SetText(wxString strText)
    {
        m_strDialogText = strText;
        if (m_strOptionName.IsEmpty()) m_strOptionName = strText;
    }
    wxString GetText()
    {
        return m_strDialogText;
    }
    void AddPossibleValue(wxString strValue);
    void AddPossibleValue(long nValue);
    void AddPossibleValue(double dblValue);
    void AddPossibleValue(bool bValue);
    VariantArray* GetPossibleValuesArray()
    {
        return &m_PossibleValuesArray;
    }
    void SetOptionType(int nType);
    int GetOptionType()
    {
        return m_nOptionType;
    }
    wxString GetStringValue();
    long GetLongValue();
    double GetDoubleValue();
    bool GetBoolValue();
    void SetShowOption(bool bShow)
    {
        m_bShowOption = bShow;
    }
    bool GetShowOption()
    {
        return m_bShowOption;
    }
    wxString GetValueAsString()
    {
        return m_OptionValue.MakeString();
    }
    void SetValue(wxString strValue, int nType = SpellCheckEngineOption::STRING);
    void SetValue(long nValue);
    void SetValue(double dblValue);
    void SetValue(bool bValue);
    void SetDependency(wxString strOptionName)
    {
        m_strDependency = strOptionName;
    }
    wxString GetDependency()
    {
        return m_strDependency;
    }

private:
    wxString m_strOptionName; // Option name as the spell check engine knows it
    wxString m_strDialogText; // Option name suitable for the options dialog
    VariantArray m_PossibleValuesArray; // This can be used to populate choice boxes on the options dialog
    wxString m_strDependency; // This is the name of the option on which this option depends
    wxVariant m_OptionValue;
    int m_nOptionType;  // Since we want to keep track of where this is a file or directory, we can't always use wxVariant.GetType()
    bool m_bShowOption; // When presenting the options to the user, show this option on the dialog
};

#endif // __SPELL_CHECK_ENGINE_OPTION__
