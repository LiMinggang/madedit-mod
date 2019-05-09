/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 11266 $
 * $Id: asstreamiterator.cpp 11266 2018-01-15 17:54:23Z jenslody $
 * $HeadURL: file:///svn/p/codeblocks/code/trunk/src/plugins/astyle/asstreamiterator.cpp $
 */

#include "asstreamiterator.h"

#include <vector>

#include "../src/MadEdit/MadEdit.h"
//#include "cbstyledtextctrl.h"
//#include "globals.h"

ASStreamIterator::ASStreamIterator(MadEdit* cbe, const wxChar* in) :
  m_Ed(cbe),
  m_CharPtr(in),
  m_SavedCharPtr(0),
  m_CurChar(0),
  m_CurLine(0),
  m_FoundBookmark(false),
  m_FoundBreakpoint(false)
{
}

ASStreamIterator::~ASStreamIterator()
{
}

bool ASStreamIterator::hasMoreLines() const
{
    return (*m_CharPtr) != 0;
}

int ASStreamIterator::getStreamLength() const
{
    return static_cast<int>(0/*m_Ed->GetControl()->GetLength()*/);
}

std::string ASStreamIterator::nextLine(/*cb_unused*/ bool emptyLineWasDeleted)
{
    // hack: m_CurLine = 0 is a special case we should not evaluate here
    if (m_Ed && m_CurLine/* && m_Ed->HasBookmark(m_CurLine)*/)
        m_FoundBookmark = true;

    if (m_Ed && m_CurLine/* && m_Ed->HasBreakpoint(m_CurLine)*/)
        m_FoundBreakpoint = true;

    return readLine();
}

std::string ASStreamIterator::peekNextLine()
{
    if (!m_SavedCharPtr){
        m_SavedCharPtr = m_CharPtr;
        m_SavedCurChar = m_CurChar;
    }

    return readLine();
}

void ASStreamIterator::peekReset()
{
    m_CharPtr = m_SavedCharPtr;
    m_CurChar = m_SavedCurChar;
    m_SavedCharPtr = 0;
}

std::streamoff ASStreamIterator::tellg()
{
    return static_cast<std::streamoff>(m_CurChar);
}

// private

std::string ASStreamIterator::readLine()
{
    static std::vector<wxChar> buf;
    buf.clear();

    while (*m_CharPtr != 0)
    {
        if ( !IsEOL(*m_CharPtr) )
            buf.push_back(*m_CharPtr);

        ++m_CharPtr;
        ++m_CurChar;

        if ( IsEOL(*m_CharPtr) )
        {
            // if CRLF (two chars) peek next char (avoid duplicating empty-lines)
            if (*m_CharPtr != *(m_CharPtr + 1) && IsEOL(*(m_CharPtr + 1)))
            {
                ++m_CharPtr;
                ++m_CurChar;
            }

            break;
        }
    }

    buf.push_back(0);
    ++m_CurLine;

	return static_cast<std::string>( /*cbU2C(&buf[0])*/wxString((&buf[0])).mb_str());
}
