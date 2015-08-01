/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef FORMATTERSETTINGS_H
#define FORMATTERSETTINGS_H

#include "astyle/astyle.h"

class FormatterSettings
{
	public:
		FormatterSettings();
		virtual ~FormatterSettings();

		void ApplyTo(astyle::ASFormatter& formatter);
};

#endif // FORMATTERSETTINGS_H
