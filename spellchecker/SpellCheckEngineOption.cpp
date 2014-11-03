#include "SpellCheckEngineOption.h"
#include "wx/filename.h"

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(VariantArray);

SpellCheckEngineOption::SpellCheckEngineOption()
{
    m_strOptionName = _T("");
    m_strDialogText = _T("");
    m_PossibleValuesArray.Clear();
    m_nOptionType = SpellCheckEngineOption::UNDEFINED;
    m_bShowOption = true;
    m_strDependency = _T("");
}

SpellCheckEngineOption::SpellCheckEngineOption(wxString strName)
{
    m_strOptionName = strName;
    m_strDialogText = strName;
    m_PossibleValuesArray.Clear();
    m_nOptionType = SpellCheckEngineOption::UNDEFINED;
    m_bShowOption = true;
    m_strDependency = _T("");
}

SpellCheckEngineOption::SpellCheckEngineOption(wxString strName, wxString strText)
{
    m_strOptionName = strName;
    m_strDialogText = strText;
    m_PossibleValuesArray.Clear();
    m_nOptionType = SpellCheckEngineOption::UNDEFINED;
    m_bShowOption = true;
    m_strDependency = _T("");
}

SpellCheckEngineOption::SpellCheckEngineOption(wxString strName, wxString strText, wxString strValue, int nType/* = SpellCheckEngineOption::STRING*/)
{
    m_strOptionName = strName;
    m_strDialogText = strText;
    m_PossibleValuesArray.Clear();
    m_nOptionType = nType;
    m_bShowOption = true;
    m_strDependency = _T("");

    // Convert relative paths to absolute paths
    if ((m_nOptionType == SpellCheckEngineOption::DIR) || (m_nOptionType == SpellCheckEngineOption::FILE))
    {
        wxFileName path(strValue);

        if (path.IsRelative())
        {
            path.MakeAbsolute();
        }

        wxVariant OptionValue(path.GetFullPath());
        m_OptionValue = OptionValue;
    }
    else
    {
        wxVariant OptionValue(strValue);
        m_OptionValue = OptionValue;
    }
}

SpellCheckEngineOption::SpellCheckEngineOption(wxString strName, wxString strText, long nValue)
{
    m_strOptionName = strName;
    m_strDialogText = strText;
    m_PossibleValuesArray.Clear();
    wxVariant OptionValue(nValue);
    m_OptionValue = OptionValue;
    m_nOptionType = SpellCheckEngineOption::LONG;
    m_bShowOption = true;
    m_strDependency = _T("");
}

SpellCheckEngineOption::SpellCheckEngineOption(wxString strName, wxString strText, double dblValue)
{
    m_strOptionName = strName;
    m_strDialogText = strText;
    m_PossibleValuesArray.Clear();
    wxVariant OptionValue(dblValue);
    m_OptionValue = OptionValue;
    m_nOptionType = SpellCheckEngineOption::DOUBLE;
    m_bShowOption = true;
    m_strDependency = _T("");
}

SpellCheckEngineOption::SpellCheckEngineOption(wxString strName, wxString strText, bool bValue)
{
    m_strOptionName = strName;
    m_strDialogText = strText;
    m_PossibleValuesArray.Clear();
    wxVariant OptionValue(bValue);
    m_OptionValue = OptionValue;
    m_nOptionType = SpellCheckEngineOption::BOOLEAN;
    m_bShowOption = true;
    m_strDependency = _T("");
}

void SpellCheckEngineOption::AddPossibleValue(wxString strValue)
{
    if (m_nOptionType == SpellCheckEngineOption::UNDEFINED)
        m_nOptionType = SpellCheckEngineOption::STRING;

    if ((m_nOptionType == SpellCheckEngineOption::STRING) || (m_nOptionType == SpellCheckEngineOption::DIR) || (m_nOptionType == SpellCheckEngineOption::FILE))
    {
        wxVariant NewPossibleValue(strValue);
        m_PossibleValuesArray.Add(NewPossibleValue);
    }
    else
    {
        // Do an assert or something?
        wxASSERT_MSG(false, _T("An attempt was made to add a possible option value of type wxString but this option is not a wxString"));
    }
}

void SpellCheckEngineOption::AddPossibleValue(long nValue)
{
    if (m_nOptionType == SpellCheckEngineOption::UNDEFINED)
        m_nOptionType = SpellCheckEngineOption::LONG;

    if (m_nOptionType == SpellCheckEngineOption::LONG)
    {
        wxVariant NewPossibleValue(nValue);
        m_PossibleValuesArray.Add(NewPossibleValue);
    }
    else
    {
        // Do an assert or something?
        wxASSERT_MSG(false, _T("An attempt was made to add a possible option value of type long but this option is not a long"));
    }
}

void SpellCheckEngineOption::AddPossibleValue(double dblValue)
{
    if (m_nOptionType == SpellCheckEngineOption::UNDEFINED)
        m_nOptionType = SpellCheckEngineOption::DOUBLE;

    if (m_nOptionType == SpellCheckEngineOption::DOUBLE)
    {
        wxVariant NewPossibleValue(dblValue);
        m_PossibleValuesArray.Add(NewPossibleValue);
    }
    else
    {
        // Do an assert or something?
        wxASSERT_MSG(false, _T("An attempt was made to add a possible option value of type double but this option is not a double"));
    }
}

void SpellCheckEngineOption::AddPossibleValue(bool bValue)
{
    if (m_nOptionType == SpellCheckEngineOption::UNDEFINED)
        m_nOptionType = SpellCheckEngineOption::BOOLEAN;

    if (m_nOptionType == SpellCheckEngineOption::BOOLEAN)
    {
        wxVariant NewPossibleValue(bValue);
        m_PossibleValuesArray.Add(NewPossibleValue);
    }
    else
    {
        // Do an assert or something?
        wxASSERT_MSG(false, _T("An attempt was made to add a possible option value of type boolean but this option is not a boolean"));
    }
}

void SpellCheckEngineOption::SetOptionType(int nType)
{
    // If the type isn't consistent with the current option value or any of the possible values
    // then we need to remove the existing inconsistent data
}

wxString SpellCheckEngineOption::GetStringValue()
{
    if ((m_nOptionType == SpellCheckEngineOption::STRING) || (m_nOptionType == SpellCheckEngineOption::DIR) || (m_nOptionType == SpellCheckEngineOption::FILE))
        return m_OptionValue.GetString();
    else
        return wxEmptyString;
}

long SpellCheckEngineOption::GetLongValue()
{
    if (m_nOptionType == SpellCheckEngineOption::LONG)
        return m_OptionValue.GetLong();
    else
        return 0;
}

double SpellCheckEngineOption::GetDoubleValue()
{
    if (m_nOptionType == SpellCheckEngineOption::DOUBLE)
        return m_OptionValue.GetDouble();
    else
        return 0;
}

bool SpellCheckEngineOption::GetBoolValue()
{
    if (m_nOptionType == SpellCheckEngineOption::BOOLEAN)
        return m_OptionValue.GetBool();
    else
        return false;
}

void SpellCheckEngineOption::SetValue(wxString strValue, int nType/* = SpellCheckEngineOption::STRING*/)
{
    wxVariant OptionValue(strValue);
    m_OptionValue = OptionValue;
    m_nOptionType = nType;
}

void SpellCheckEngineOption::SetValue(long nValue)
{
    wxVariant OptionValue(nValue);
    m_OptionValue = OptionValue;
    m_nOptionType = SpellCheckEngineOption::LONG;
}

void SpellCheckEngineOption::SetValue(double dblValue)
{
    wxVariant OptionValue(dblValue);
    m_OptionValue = OptionValue;
    m_nOptionType = SpellCheckEngineOption::DOUBLE;
}

void SpellCheckEngineOption::SetValue(bool bValue)
{
    wxVariant OptionValue(bValue);
    m_OptionValue = OptionValue;
    m_nOptionType = SpellCheckEngineOption::BOOLEAN;
}

