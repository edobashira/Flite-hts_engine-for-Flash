/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 1999                             */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2000                                     */
/*************************************************************************/
/*                                                                       */
/*  Regexes, this is just a front end to Henry Spencer's regex code      */
/*  Includes a mapping of fsf format regex's to hs format (escaping)     */
/*                                                                       */
/*************************************************************************/

/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis System (HTS)             */
/*           flite+hts_engine developed by HTS Working Group         */
/*           http://hts-engine.sourceforge.net/                      */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2005-2009  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/*                2005-2008  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include "cst_alloc.h"
#include "cst_regex.h"

static const unsigned char cst_rx_white_rxprog[] = {
	156, 
	6, 0, 20, 1, 0, 3, 11, 0, 11, 4, 
	0, 0, 32, 10, 9, 13, 0, 2, 0, 3, 
	0, 0, 0
};
static const cst_regex cst_rx_white_rx = {
	0,
	1,
	NULL,
	0,
	24,
	(char *) cst_rx_white_rxprog
};

static const unsigned char cst_rx_identifier_rxprog[] = {
	156, 
	6, 0, 136, 1, 0, 3, 4, 0, 57, 65, 
	66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 
	76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 
	86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 
	102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 
	122, 95, 0, 11, 0, 70, 4, 0, 0, 48, 
	49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 
	66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 
	76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 
	86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 
	102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 
	122, 95, 0, 2, 0, 3, 0, 0, 0
};
static const cst_regex cst_rx_identifier_rx = {
	0,
	1,
	NULL,
	0,
	140,
	(char *) cst_rx_identifier_rxprog
};

static const unsigned char cst_rx_digits_rxprog[] = {
	156, 
	6, 0, 40, 1, 0, 3, 4, 0, 14, 48, 
	49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 
	10, 0, 17, 4, 0, 0, 48, 49, 50, 51, 
	52, 53, 54, 55, 56, 57, 0, 2, 0, 3, 
	0, 0, 0
};
static const cst_regex cst_rx_digits_rx = {
	0,
	1,
	NULL,
	0,
	44,
	(char *) cst_rx_digits_rxprog
};

static const unsigned char cst_rx_dotted_abbrev_rxprog[] = {
	156, 
	6, 0, 147, 1, 0, 3, 6, 0, 76, 21, 
	0, 3, 6, 0, 64, 4, 0, 56, 65, 66, 
	67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 
	77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 
	87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 
	103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 
	113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 
	0, 8, 0, 5, 46, 0, 31, 0, 3, 7, 
	0, 73, 6, 0, 3, 9, 0, 3, 4, 0, 
	56, 65, 66, 67, 68, 69, 70, 71, 72, 73, 
	74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 
	84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
	110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
	120, 121, 122, 0, 2, 0, 3, 0, 0, 0
};
static const cst_regex cst_rx_dotted_abbrev_rx = {
	0,
	1,
	NULL,
	0,
	151,
	(char *) cst_rx_dotted_abbrev_rxprog
};

static const unsigned char cst_rx_lowercase_rxprog[] = {
	156, 
	6, 0, 42, 1, 0, 3, 11, 0, 33, 4, 
	0, 0, 97, 98, 99, 100, 101, 102, 103, 104, 
	105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 
	115, 116, 117, 118, 119, 120, 121, 122, 0, 2, 
	0, 3, 0, 0, 0
};
static const cst_regex cst_rx_lowercase_rx = {
	0,
	1,
	NULL,
	0,
	46,
	(char *) cst_rx_lowercase_rxprog
};

static const unsigned char cst_rx_alpha_rxprog[] = {
	156, 
	6, 0, 68, 1, 0, 3, 11, 0, 59, 4, 
	0, 0, 65, 66, 67, 68, 69, 70, 71, 72, 
	73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 
	83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 
	99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
	109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 
	119, 120, 121, 122, 0, 2, 0, 3, 0, 0, 
	0
};
static const cst_regex cst_rx_alpha_rx = {
	0,
	1,
	NULL,
	0,
	72,
	(char *) cst_rx_alpha_rxprog
};

static const unsigned char cst_rx_alphanum_rxprog[] = {
	156, 
	6, 0, 78, 1, 0, 3, 11, 0, 69, 4, 
	0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 
	56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 
	73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 
	83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 
	99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
	109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 
	119, 120, 121, 122, 0, 2, 0, 3, 0, 0, 
	0
};
static const cst_regex cst_rx_alphanum_rx = {
	0,
	1,
	NULL,
	0,
	82,
	(char *) cst_rx_alphanum_rxprog
};

static const unsigned char cst_rx_uppercase_rxprog[] = {
	156, 
	6, 0, 42, 1, 0, 3, 11, 0, 33, 4, 
	0, 0, 65, 66, 67, 68, 69, 70, 71, 72, 
	73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 
	83, 84, 85, 86, 87, 88, 89, 90, 0, 2, 
	0, 3, 0, 0, 0
};
static const cst_regex cst_rx_uppercase_rx = {
	0,
	1,
	NULL,
	0,
	46,
	(char *) cst_rx_uppercase_rxprog
};

static const unsigned char cst_rx_commaint_rxprog[] = {
	156, 
	6, 0, 224, 1, 0, 3, 4, 0, 14, 48, 
	49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 
	6, 0, 17, 4, 0, 17, 48, 49, 50, 51, 
	52, 53, 54, 55, 56, 57, 0, 6, 0, 3, 
	9, 0, 3, 6, 0, 17, 4, 0, 17, 48, 
	49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 
	6, 0, 3, 9, 0, 3, 8, 0, 5, 44, 
	0, 6, 0, 62, 21, 0, 3, 6, 0, 50, 
	4, 0, 14, 48, 49, 50, 51, 52, 53, 54, 
	55, 56, 57, 0, 4, 0, 14, 48, 49, 50, 
	51, 52, 53, 54, 55, 56, 57, 0, 4, 0, 
	14, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
	57, 0, 8, 0, 5, 44, 0, 31, 0, 3, 
	7, 0, 59, 6, 0, 3, 9, 0, 3, 4, 
	0, 14, 48, 49, 50, 51, 52, 53, 54, 55, 
	56, 57, 0, 4, 0, 14, 48, 49, 50, 51, 
	52, 53, 54, 55, 56, 57, 0, 4, 0, 14, 
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 
	0, 6, 0, 34, 22, 0, 3, 6, 0, 25, 
	8, 0, 5, 46, 0, 11, 0, 17, 4, 0, 
	0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
	57, 0, 32, 0, 6, 6, 0, 3, 9, 0, 
	3, 2, 0, 3, 0, 0, 0
};
static const cst_regex cst_rx_commaint_rx = {
	0,
	1,
	NULL,
	0,
	228,
	(char *) cst_rx_commaint_rxprog
};

static const unsigned char cst_rx_int_rxprog[] = {
	156, 
	6, 0, 40, 1, 0, 3, 6, 0, 8, 8, 
	0, 8, 45, 0, 6, 0, 3, 9, 0, 3, 
	11, 0, 17, 4, 0, 0, 48, 49, 50, 51, 
	52, 53, 54, 55, 56, 57, 0, 2, 0, 3, 
	0, 0, 0
};
static const cst_regex cst_rx_int_rx = {
	0,
	1,
	NULL,
	0,
	44,
	(char *) cst_rx_int_rxprog
};

static const unsigned char cst_rx_double_rxprog[] = {
	156, 
	6, 0, 199, 1, 0, 3, 6, 0, 8, 8, 
	0, 8, 45, 0, 6, 0, 3, 9, 0, 3, 
	21, 0, 3, 6, 0, 51, 22, 0, 3, 6, 
	0, 42, 11, 0, 17, 4, 0, 0, 48, 49, 
	50, 51, 52, 53, 54, 55, 56, 57, 0, 8, 
	0, 5, 46, 0, 10, 0, 17, 4, 0, 0, 
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 
	0, 32, 0, 66, 6, 0, 29, 23, 0, 3, 
	6, 0, 20, 11, 0, 17, 4, 0, 0, 48, 
	49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 
	33, 0, 37, 6, 0, 34, 24, 0, 3, 6, 
	0, 25, 8, 0, 5, 46, 0, 11, 0, 17, 
	4, 0, 0, 48, 49, 50, 51, 52, 53, 54, 
	55, 56, 57, 0, 34, 0, 3, 31, 0, 3, 
	6, 0, 50, 25, 0, 3, 6, 0, 41, 4, 
	0, 6, 101, 69, 0, 6, 0, 9, 4, 0, 
	9, 45, 43, 0, 6, 0, 3, 9, 0, 3, 
	11, 0, 17, 4, 0, 0, 48, 49, 50, 51, 
	52, 53, 54, 55, 56, 57, 0, 35, 0, 6, 
	6, 0, 3, 9, 0, 3, 2, 0, 3, 0, 
	0, 0
};
static const cst_regex cst_rx_double_rx = {
	0,
	1,
	NULL,
	0,
	203,
	(char *) cst_rx_double_rxprog
};

const cst_regex *cst_rx_white = &cst_rx_white_rx;
const cst_regex *cst_rx_identifier = &cst_rx_identifier_rx;
const cst_regex *cst_rx_digits = &cst_rx_digits_rx;
const cst_regex *cst_rx_dotted_abbrev = &cst_rx_dotted_abbrev_rx;
const cst_regex *cst_rx_lowercase = &cst_rx_lowercase_rx;
const cst_regex *cst_rx_alpha = &cst_rx_alpha_rx;
const cst_regex *cst_rx_alphanum = &cst_rx_alphanum_rx;
const cst_regex *cst_rx_uppercase = &cst_rx_uppercase_rx;
const cst_regex *cst_rx_commaint = &cst_rx_commaint_rx;
const cst_regex *cst_rx_int = &cst_rx_int_rx;
const cst_regex *cst_rx_double = &cst_rx_double_rx;

/* For acces by const models */
const cst_regex *cst_regex_table[] = {
	&cst_rx_dotted_abbrev_rx
};

#ifndef FLITE_PLUS_HTS_ENGINE
static char *regularize(const char *unregex,int match);

void
cst_regex_init()
{
}
#endif /* !FLITE_PLUS_HTS_ENGINE */

int cst_regex_match(const cst_regex *r, const char *str)
{
    cst_regstate *s;

    if (r == NULL) return 0;
    s = hs_regexec(r, str);
    if (s) {
	cst_free(s);
	return 1;
    } else
	return 0;
}

#ifndef FLITE_PLUS_HTS_ENGINE
cst_regstate *cst_regex_match_return(const cst_regex *r, const char *str)
{

    if (r == NULL) return 0;
    return hs_regexec(r, str);
}

cst_regex *new_cst_regex(const char *str)
{
    cst_regex *r;
    char *reg_str = regularize(str,1);

    r = hs_regcomp(reg_str);
    cst_free(reg_str);

    return r;
}

void delete_cst_regex(cst_regex *r)
{
    if (r)
	hs_regdelete(r);
}

/* These define the different escape conventions for the FSF's */
/* regexp code and Henry Spencer's */

static const char *fsf_magic="^$*+?[].\\";
static const char *fsf_magic_backslashed="()|<>";
static const char *spencer_magic="^$*+?[].()|\\\n";
static const char *spencer_magic_backslashed="<>";

/* Adaptation of rjc's mapping of fsf format to henry spencer's format */
/* of escape sequences, as taken from EST_Regex.cc in EST              */
static char *regularize(const char *unregex,int match)
{
    char *reg = cst_alloc(char, strlen(unregex)*2+3);
    char *r=reg;
    const char *e;
    int magic=0,last_was_bs=0;
    const char * in_brackets=NULL;
    const char *ex = (unregex?unregex:"");

    if (match && *ex != '^')
	*(r++) = '^';

    for(e=ex; *e ; e++)
    {
	if (*e == '\\' && !last_was_bs)
	{
	    last_was_bs=1;
	    continue;
	}

	magic=strchr((last_was_bs?fsf_magic_backslashed:fsf_magic), *e)!=NULL;

	if (in_brackets)
	{
	    *(r++) = *e;
	    if (*e  == ']' && (e-in_brackets)>1)
		in_brackets=0;
	}
	else if (magic)
	{
	    if (strchr(spencer_magic_backslashed, *e))
		*(r++) = '\\';

	    *(r++) = *e;
	    if (*e  == '[')
		in_brackets=e;
	}
	else 
	{
	    if (strchr(spencer_magic, *e))
		*(r++) = '\\';

	    *(r++) = *e;
	}
	last_was_bs=0;
    }
  
    if (match && (e==ex || *(e-1) != '$'))
    {
	if (last_was_bs)
	    *(r++) = '\\';
	*(r++) = '$';
    }

    *r='\0';

    return reg;
}
#endif /* !FLITE_PLUS_HTS_ENGINE */
