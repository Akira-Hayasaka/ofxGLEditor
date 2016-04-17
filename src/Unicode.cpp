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
 * http://www.codeguru.com/cpp/misc/misc/multi-lingualsupport/article.php/c10451/
 *
 * Code originally in Fluxus http://www.pawfal.org/fluxus
 * Copyright (C) Dave Griffiths
 */
#include "Unicode.h"

using namespace std;

#define MASKBITS   0x3F
#define MASKBYTE   0x80
#define MASK2BYTES 0xC0
#define MASK3BYTES 0xE0
#define MASK4BYTES 0xF0
#define MASK5BYTES 0xF8
#define MASK6BYTES 0xFC

//--------------------------------------------------------------
unsigned int wchar_width(int input) {
	if((input & MASK6BYTES) == MASK6BYTES) {
		return 6;
	}
	else if((input & MASK5BYTES) == MASK5BYTES) {
		return 5;
	}
	else if((input & MASK4BYTES) == MASK4BYTES) {
		return 4;
	}
	else if((input & MASK3BYTES) == MASK3BYTES) {
		return 3;
	}
	else if((input & MASK2BYTES) == MASK2BYTES) {
		return 2;
	}
	else {
		return 1;
	}
}

//--------------------------------------------------------------
std::string wchar_to_string(char32_t input) {
	string output;
	// 0xxxxxxx
	if(input < 0x80) {
		output.push_back((char)input);
	}
	// 110xxxxx 10xxxxxx
	else if(input < 0x800) {
		output.push_back((char)(MASK2BYTES | input >> 6));
		output.push_back((char)(MASKBYTE | (input & MASKBITS)));
	}
	// 1110xxxx 10xxxxxx 10xxxxxx
	else if(input < 0x10000) {
		output.push_back((char)(MASK3BYTES | (input >> 12)));
		output.push_back((char)(MASKBYTE | (input >> 6 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input & MASKBITS)));
	}
	// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if(input < 0x200000) {
		output.push_back((char)(MASK4BYTES | (input >> 18)));
		output.push_back((char)(MASKBYTE | (input >> 12 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input >> 6 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input & MASKBITS)));
	}
	// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if(input < 0x4000000) {
		output.push_back((char)(MASK5BYTES | (input >> 24)));
		output.push_back((char)(MASKBYTE | (input >> 18 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input >> 12 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input >> 6 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input & MASKBITS)));
	}
	// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if(input < 0x8000000) {
		output.push_back((char)(MASK6BYTES | (input >> 30)));
		output.push_back((char)(MASKBYTE | (input >> 18 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input >> 12 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input >> 6 & MASKBITS)));
		output.push_back((char)(MASKBYTE | (input & MASKBITS)));
	}
	return output;
}

//--------------------------------------------------------------
char32_t string_to_wchar(const std::string &input) {
	char32_t output = 0;
	// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	if(input.size() > 5 && (input[0] & MASK6BYTES) == MASK6BYTES) {
		output = ((input[0] & 0x01) << 30) |
		         ((input[1] & MASKBITS) << 24) |
		         ((input[2] & MASKBITS) << 18) |
		         ((input[3] & MASKBITS) << 12) |
		         ((input[4] & MASKBITS) << 6) |
		          (input[5] & MASKBITS);
	}
	// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if(input.size() > 4 && (input[0] & MASK5BYTES) == MASK5BYTES) {
		output = ((input[0] & 0x03) << 24) |
		         ((input[1] & MASKBITS) << 18) |
		         ((input[2] & MASKBITS) << 12) |
		         ((input[3] & MASKBITS) << 6) |
		          (input[4] & MASKBITS);
	}
	// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	else if(input.size() > 3 && (input[0] & MASK4BYTES) == MASK4BYTES) {
		output = ((input[0] & 0x07) << 18) |
		         ((input[1] & MASKBITS) << 12) |
		         ((input[2] & MASKBITS) << 6) |
		          (input[3] & MASKBITS);
	}
	// 1110xxxx 10xxxxxx 10xxxxxx
	else if(input.size() > 2 && (input[0] & MASK3BYTES) == MASK3BYTES) {
		output = ((input[0] & 0x0F) << 12) |
				 ((input[1] & MASKBITS) << 6) |
				  (input[2] & MASKBITS);
	}
	// 110xxxxx 10xxxxxx
	else if(input.size() > 1 && (input[0] & MASK2BYTES) == MASK2BYTES) {
		output = ((input[0] & 0x1F) << 6) |
		          (input[1] & MASKBITS);
	}
	// 0xxxxxxx
	else {
		output = input[0];
	}
	return output;
}

//--------------------------------------------------------------
string wstring_to_string(const u32string &input) {
	string output;
	for(char32_t c: input) {
		output.append(wchar_to_string(c));
	}
	return output;
}

//--------------------------------------------------------------
u32string string_to_wstring(const string &input) {
	u32string output;
	for(unsigned int i = 0; i < input.size();) {
		char32_t ch;
		// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		if((input[i] & MASK6BYTES) == MASK6BYTES) {
			ch = ((input[i] & 0x01) << 30) |
			     ((input[i+1] & MASKBITS) << 24) |
				 ((input[i+2] & MASKBITS) << 18) |
				 ((input[i+3] & MASKBITS) << 12) |
				 ((input[i+4] & MASKBITS) << 6) |
				  (input[i+5] & MASKBITS);
			i += 6;
		}
		// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if((input[i] & MASK5BYTES) == MASK5BYTES) {
			ch = ((input[i] & 0x03) << 24) |
			     ((input[i+1] & MASKBITS) << 18) |
				 ((input[i+2] & MASKBITS) << 12) |
				 ((input[i+3] & MASKBITS) << 6) |
				 (input[i+4] & MASKBITS);
			i += 5;
		}
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if((input[i] & MASK4BYTES) == MASK4BYTES) {
			ch = ((input[i] & 0x07) << 18) |
			     ((input[i+1] & MASKBITS) << 12) |
				 ((input[i+2] & MASKBITS) << 6) |
				  (input[i+3] & MASKBITS);
			i += 4;
		}
		// 1110xxxx 10xxxxxx 10xxxxxx
		else if((input[i] & MASK3BYTES) == MASK3BYTES) {
			ch = ((input[i] & 0x0F) << 12) |
			     ((input[i+1] & MASKBITS) << 6) |
				  (input[i+2] & MASKBITS);
		 i += 3;
		}
		// 110xxxxx 10xxxxxx
		else if((input[i] & MASK2BYTES) == MASK2BYTES) {
			ch = ((input[i] & 0x1F) << 6) |
			      (input[i+1] & MASKBITS);
			i += 2;
		}
		// 0xxxxxxx
		else {
			ch = input[i];
			i += 1;
		}
		output.push_back(ch);
	}
	return output;
}
