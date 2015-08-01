/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ASSTREAMITERATOR_H
#define ASSTREAMITERATOR_H

#include <iostream>

#include <wx/string.h>

#include "astyle/astyle.h"

class MadEdit;
class ASStreamIterator : public astyle::ASSourceIterator
{
    public:
        ASStreamIterator(MadEdit* cbe, const wxChar* in);
        virtual ~ASStreamIterator();

        bool hasMoreLines() const;
        int  getStreamLength() const;
        std::string nextLine(bool emptyLineWasDeleted = false);
        std::string peekNextLine();
        void peekReset();
        std::streamoff tellg();

        bool FoundBookmark() const   { return m_FoundBookmark;    }
        void ClearFoundBookmark()    { m_FoundBookmark = false;   }
        bool FoundBreakpoint() const { return m_FoundBreakpoint;  }
        void ClearFoundBreakpoint()  { m_FoundBreakpoint = false; }

    protected:
        inline bool IsEOL(wxChar ch) { return (ch == _T('\r') || ch == _T('\n')); }

        MadEdit*           m_Ed;
        const wxChar*       m_CharPtr;
        const wxChar*       m_SavedCharPtr;
        int                 m_CurChar;
        int                 m_CurLine;
        bool                m_FoundBookmark;
        bool                m_FoundBreakpoint;

    private:
        std::string readLine();
};

#endif // ASSTREAMITERATOR_H
