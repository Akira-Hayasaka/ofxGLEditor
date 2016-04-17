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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 *
 * Algorithm by Marius Bancila
 * http://www.codeguru.com/cpp/misc/misc/multi-lingualsupport/article.php/c10451
 *
 * Expanded from code originally in Fluxus http://www.pawfal.org/fluxus
 * Copyright (C) Dave Griffiths
 *
 * This seems to be a little faster than the C++11 std::wstring_convert class
 * or at least eliminates calling the converter class constructor when used
 * in a loop.
 *
 */
#include <string>

/// get the number of bytes for a UTF-8 wchar from a (suspected) leading byte
unsigned int wchar_width(int input);

/// split a single wide char value into UTF-8 bytes
std::string wchar_to_string(char32_t input);

/// split a set of UTF-8 bytes into the first single wide char value found
char32_t string_to_wchar(const std::string &input);

/// split wide chars into UTF-8 bytes
std::string wstring_to_string(const std::u32string &input);

/// parse UTF-8 bytes into wide chars
std::u32string string_to_wstring(const std::string &input);
