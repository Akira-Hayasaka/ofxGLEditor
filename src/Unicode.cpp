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
#include "Unicode.h"

using namespace std;

#define         MASKBITS                0x3F
#define         MASKBYTE                0x80
#define         MASK2BYTES              0xC0
#define         MASK3BYTES              0xE0
#define         MASK4BYTES              0xF0
#define         MASK5BYTES              0xF8
#define         MASK6BYTES              0xFC

typedef unsigned short   Unicode2Bytes;
typedef unsigned int     Unicode4Bytes;
typedef unsigned char    byte;

string wstring_to_string(const wstring &input) {
	string output;
	for(unsigned int i = 0; i < input.size(); i++) {
		// 0xxxxxxx
		if(input[i] < 0x80) {
			output.push_back((byte)input[i]);
		}
		// 110xxxxx 10xxxxxx
		else if(input[i] < 0x800) {
			output.push_back((byte)(MASK2BYTES | input[i] >> 6));
			output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
		}
		// 1110xxxx 10xxxxxx 10xxxxxx
		else if(input[i] < 0x10000) {
			output.push_back((byte)(MASK3BYTES | (input[i] >> 12)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 6 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
		}
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if(input[i] < 0x200000) {
			output.push_back((byte)(MASK4BYTES | (input[i] >> 18)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 12 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 6 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
		}
		// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if(input[i] < 0x4000000) {
			output.push_back((byte)(MASK5BYTES | (input[i] >> 24)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 18 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 12 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 6 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
		}
		// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if(input[i] < 0x8000000) {
			output.push_back((byte)(MASK6BYTES | (input[i] >> 30)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 18 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 12 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] >> 6 & MASKBITS)));
			output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
		}
	}
	return output;
}

wstring string_to_wstring(const string &input) {
	wstring output;
	for(unsigned int i = 0; i < input.size();) {
		Unicode4Bytes ch;
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
		else { //if(input[i] < MASKBYTE)
			ch = input[i];
			i += 1;
		}
		output.push_back(ch);
	}
	return output;
}
