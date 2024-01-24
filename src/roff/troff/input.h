/* Copyright (C) 2001-2022 Free Software Foundation, Inc.
     Written by James Clark (jjc@jclark.com)

This file is part of groff.

groff is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or
(at your option) any later version.

groff is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */


/* special character codes */

#ifndef IS_EBCDIC_HOST

const int ESCAPE_QUESTION = 015;
const int BEGIN_TRAP = 016;
const int END_TRAP = 017;
const int PAGE_EJECTOR = 020;
const int ESCAPE_NEWLINE = 021;
const int ESCAPE_AMPERSAND = 022;
const int ESCAPE_UNDERSCORE = 023;
const int ESCAPE_BAR = 024;
const int ESCAPE_CIRCUMFLEX = 025;
const int ESCAPE_LEFT_BRACE = 026;
const int ESCAPE_RIGHT_BRACE = 027;
const int ESCAPE_LEFT_QUOTE = 030;
const int ESCAPE_RIGHT_QUOTE = 031;
const int ESCAPE_HYPHEN = 032;
const int ESCAPE_BANG = 033;
const int ESCAPE_c = 034;
const int ESCAPE_e = 035;
const int ESCAPE_PERCENT = 036;
const int ESCAPE_SPACE = 037;

const int INPUT_DELETE = 0177;

const int TITLE_REQUEST = 0200;
const int COPY_FILE_REQUEST = 0201;
const int TRANSPARENT_FILE_REQUEST = 0202;
#ifdef COLUMN
const int VJUSTIFY_REQUEST = 0203;
#endif /* COLUMN */
const int ESCAPE_E = 0204;
const int LAST_PAGE_EJECTOR = 0205;
const int ESCAPE_RIGHT_PARENTHESIS = 0206;
const int ESCAPE_TILDE = 0207;
const int ESCAPE_COLON = 0210;
const int PUSH_GROFF_MODE = 0211;
const int PUSH_COMP_MODE = 0212;
const int POP_GROFFCOMP_MODE = 0213;
const int BEGIN_QUOTE = 0214;
const int END_QUOTE = 0215;
const int DOUBLE_QUOTE = 0216;
const int INPUT_NO_BREAK_SPACE = 0240;
const int INPUT_SOFT_HYPHEN= 0255;

#else /* IS_EBCDIC_HOST */

const int INPUT_DELETE = 007;

const int ESCAPE_QUESTION = 010;
const int BEGIN_TRAP = 011;
const int END_TRAP = 013;
const int PAGE_EJECTOR = 015;
const int ESCAPE_NEWLINE = 016;
const int ESCAPE_AMPERSAND = 017;
const int ESCAPE_UNDERSCORE = 020;
const int ESCAPE_BAR = 021;
const int ESCAPE_CIRCUMFLEX = 022;
const int ESCAPE_LEFT_BRACE = 023;
const int ESCAPE_RIGHT_BRACE = 024;
const int ESCAPE_LEFT_QUOTE = 027;
const int ESCAPE_RIGHT_QUOTE = 030;
const int ESCAPE_HYPHEN = 031;
const int ESCAPE_BANG = 032;
const int ESCAPE_c = 033;
const int ESCAPE_e = 034;
const int ESCAPE_PERCENT = 035;
const int ESCAPE_SPACE = 036;

const int TITLE_REQUEST = 060;
const int COPY_FILE_REQUEST = 061;
const int TRANSPARENT_FILE_REQUEST = 062;
#ifdef COLUMN
const int VJUSTIFY_REQUEST = 063;
#endif /* COLUMN */
const int ESCAPE_E = 064;
const int LAST_PAGE_EJECTOR = 065;
const int ESCAPE_RIGHT_PARENTHESIS = 066;
const int ESCAPE_TILDE = 067;
const int ESCAPE_COLON = 070;
const int PUSH_GROFF_MODE = 071;
const int PUSH_COMP_MODE = 072;
const int POP_GROFFCOMP_MODE = 073;
const int BEGIN_QUOTE = 074;
const int END_QUOTE = 075;
const int DOUBLE_QUOTE = 076;

const int INPUT_NO_BREAK_SPACE = 0101;
const int INPUT_SOFT_HYPHEN= 0312;

#endif /* IS_EBCDIC_HOST */

extern void do_glyph_color(symbol);
extern void do_fill_color(symbol);
extern bool is_codepoint_composite(const char *n);

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
