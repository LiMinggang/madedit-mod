#include "SpellCheckUserInterface.h"

wxSpellCheckUserInterface::wxSpellCheckUserInterface(wxSpellCheckEngineInterface* pSpellChecker /*= NULL*/)
{
    m_strMisspelledWord = _T("");
    m_strReplaceWithText = _T("");
    m_strContext = _T("");
    m_pSpellCheckEngine = pSpellChecker;

    m_nLastAction = ACTION_INITIAL;
}

wxSpellCheckUserInterface::~wxSpellCheckUserInterface()
{
}
