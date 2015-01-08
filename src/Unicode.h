/*
 * Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 *
 * Algorithm by Marius Bancila
 * http://www.codeguru.com/cpp/misc/misc/multi-lingualsupport/article.php/c10451/
 *
 * Code originally in Fluxus http://www.pawfal.org/fluxus
 * Copyright (C) Dave Griffiths
 */
#include <string>

/// split wide chars into UTF-8 bytes
std::string wstring_to_string(const std::wstring &input);

/// parse UTF-8 bytes into wide chars
std::wstring string_to_wstring(const std::string &input);
