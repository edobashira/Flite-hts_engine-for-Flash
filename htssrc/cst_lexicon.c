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
/*               Date:  December 1999                                    */
/*************************************************************************/
/*                                                                       */
/*  Lexicon related functions                                            */
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

#include "cst_features.h"
#include "cst_lexicon.h"

CST_VAL_REGISTER_TYPE_NODEL(lexicon,cst_lexicon)

#define WP_SIZE 64

static int no_syl_boundaries(const cst_item *i, const cst_val *p);
static cst_val *lex_lookup_addenda(const char *wp,const cst_lexicon *l,
                                   int *found);

static int lex_match_entry(const char *a, const char *b);
static int lex_lookup_bsearch(const cst_lexicon *l,const char *word);
static int find_full_match(const cst_lexicon *l,
			   int i,const char *word);

#ifndef FLITE_PLUS_HTS_ENGINE
cst_lexicon *new_lexicon()
{
    cst_lexicon *l = cst_alloc(cst_lexicon,1);
    l->syl_boundary = no_syl_boundaries;
    return l;
}

void delete_lexicon(cst_lexicon *lex)
{   /* But I doubt if this will ever be called, lexicons are mapped */
    /* This probably isn't complete */
    if (lex)
    {
	cst_free(lex->data);
	cst_free(lex);
    }
}

#if 0
void lexicon_register(cst_lexicon *lex)
{
    /* Add given lexicon to list of known lexicons */
    cst_lexicon **old_lexs;
    int i;
    
    old_lexs = flite_lexicons;
    flite_num_lexicons++;
    flite_lexicons = cst_alloc(cst_lexicon *,flite_num_lexicons);
    for (i=0; i<flite_num_lexicons-1; i++)
	flite_lexicons[i] = old_lexs[i];
    flite_lexicons[i] = lex;
    cst_free(old_lexs);
}

cst_lexicon *lexicon_select(const char *name)
{
    int i;

    for (i=0; i < flite_num_lexicons; i++)
	if (cst_streq(name,flite_lexicons[i]->name))
	    return flite_lexicons[i];
    return NULL;
}
#endif

static int no_syl_boundaries(const cst_item *i, const cst_val *p)
{
    /* This is a default function that will normally be replace */
    /* for each lexicon                                         */
    (void)i;
    (void)p;
    return FALSE;
}

int in_lex(const cst_lexicon *l, const char *word, const char *pos)
{
    /* return TRUE is its in the lexicon */
    int r = FALSE, i;
    char *wp;

    wp = cst_alloc(char,strlen(word)+2);
    cst_sprintf(wp,"%c%s",(pos ? pos[0] : '0'),word);

    for (i=0; l->addenda[i]; i++)
    {
	if (((wp[0] == '0') || (wp[0] == l->addenda[i][0][0])) &&
	    (cst_streq(wp+1,l->addenda[i][0]+1)))
	{
	    r = TRUE;
	    break;
	}
    }

    if (!r && (lex_lookup_bsearch(l,wp) >= 0))
	r = TRUE;

    cst_free(wp);
    return r;
}
#endif /* !FLITE_PLUS_HTS_ENGINE */

cst_val *lex_lookup(const cst_lexicon *l, const char *word, const char *pos)
{
    int index;
    int p;
    const unsigned char *q;
    char *wp;
    cst_val *phones = 0;
    int found = FALSE;

    wp = cst_alloc(char,strlen(word)+2);
    cst_sprintf(wp,"%c%s",(pos ? pos[0] : '0'),word);

    if (l->addenda)
	phones = lex_lookup_addenda(wp,l,&found);

    if (!found)
    {
	index = lex_lookup_bsearch(l,wp);

	if (index >= 0)
	{
	    if (l->phone_hufftable)
	    {
		for (p=index-2; l->data[p]; p--)
		    for (q=l->phone_hufftable[l->data[p]]; *q; q++)
			phones = cons_val(string_val(l->phone_table[*q]),
				  phones);
	    }
	    else  /* no compression -- should we still support this ? */
	    {
		for (p=index-2; l->data[p]; p--)
		    phones = cons_val(string_val(l->phone_table[l->data[p]]),
				      phones);
	    }
	    phones = val_reverse(phones);
	}
	else if (l->lts_rule_set)
	{
	    phones = lts_apply(word,
			       "",  /* more features if we had them */
			       l->lts_rule_set);
	}
	else if (l->lts_function)
	{
	    phones = (l->lts_function)(l,word,"");
	}
    }

    cst_free(wp);
    
    return phones;
}

static cst_val *lex_lookup_addenda(const char *wp,const cst_lexicon *l, 
				   int *found)
{
    /* For those other words */
    int i,j;
    cst_val *phones;
    
    phones = NULL;

    for (i=0; l->addenda[i]; i++)
    {
	if (((wp[0] == '0') || (wp[0] == l->addenda[i][0][0])) &&
	    (cst_streq(wp+1,l->addenda[i][0]+1)))
	{
	    for (j=1; l->addenda[i][j]; j++)
		phones = cons_val(string_val(l->addenda[i][j]),phones);
	    *found = TRUE;
	    return val_reverse(phones);
	}
    }
    
    return NULL;
}

static int lex_uncompress_word(char *ucword,int max_size,
			       int p,const cst_lexicon *l)
{
    int i,j=0,length;
    unsigned char *cword;

    if (l->entry_hufftable == 0)
        /* can have "compressed" lexicons without compression */
	cst_sprintf(ucword,"%s",&l->data[p]);
    else
    {
	cword = &l->data[p];
	for (i=0,j=0; cword[i]; i++)
	{
	    length = strlen(l->entry_hufftable[cword[i]]);
	    if (j+length+1<max_size)
	    {
		memmove(ucword+j,l->entry_hufftable[cword[i]],length);
		j += length;
	    }
	    else
		break;
	}
	ucword[j] = '\0';
    }

    return j;
}
static int lex_data_next_entry(const cst_lexicon *l,int p,int end)
{
    for (p++; p < end; p++)
	if (l->data[p-1] == 255)
	    return p;
    return end;
}
static int lex_data_prev_entry(const cst_lexicon *l,int p,int start)
{
    for (p--; p > start; p--)
	if (l->data[p-1] == 255)
	    return p;
    return start;
}
static int lex_data_closest_entry(const cst_lexicon *l,int p,int start,int end)
{
    int d;

    d=0;
    while ((p-d > start) && 
	   (p+d < end))
    {
	if (l->data[(p+d)-1] == 255)
	    return p+d;
	else if (l->data[(p-d)-1] == 255)
	    return p-d;
	d++;
    }
    return p-d;
}

static int lex_lookup_bsearch(const cst_lexicon *l, const char *word)
{
    int start,mid,end,c;
    /* needs to be longer than longest word in lexicon */
    char word_pos[WP_SIZE];

    start = 0;
    end = l->num_bytes;
    while (start < end) {
	mid = (start + end)/2;

	/* find previous entry start */
	mid = lex_data_closest_entry(l,mid,start,end);
	lex_uncompress_word(word_pos,WP_SIZE,mid,l);

	c = lex_match_entry(word_pos,word);

	if (c == 0)
	    return find_full_match(l,mid,word);
	else if (c > 0)
	    end = mid;
	else
	    start = lex_data_next_entry(l,mid + 1,end);

#if 0
	if (l->data[start-1] == 255)
	{
	    lex_uncompress_word(word_pos,WP_SIZE,start,l);
	    printf("start %s %d ",word_pos,start);
	}
	else
	    printf("start NULL %d ",start);
	if (l->data[mid-1] == 255)
	{
	    lex_uncompress_word(word_pos,WP_SIZE,mid,l);
	    printf("mid %s %d ",word_pos,mid);
	}
	else
	    printf("mid NULL %d ",mid);
	if (l->data[end-1] == 255)
	{
	    lex_uncompress_word(word_pos,WP_SIZE,end,l);
	    printf("end %s %d ",word_pos,end);
	}
	else
	    printf("end NULL %d ",end);
	printf("\n");
#endif

    }
    return -1;
}

static int find_full_match(const cst_lexicon *l,
			   int i,const char *word)

{
    /* found word, now look for actual match including pos */
    int w, match=i;
    /* needs to be longer than longest word in lexicon */
    char word_pos[WP_SIZE];

    for (w=i; w > 0; )
    {
	lex_uncompress_word(word_pos,WP_SIZE,w,l);
	if (!cst_streq(word+1,word_pos+1))
	    break;
	else if (cst_streq(word,word_pos))
	    return w;
	match = w;  /* if we can't find an exact match we'll take this one */
        /* go back to last entry */
	w = lex_data_prev_entry(l,w,0);
    }

    for (w=i; w < l->num_bytes;)
    {
	lex_uncompress_word(word_pos,WP_SIZE,w,l);
	if (!cst_streq(word+1,word_pos+1))
	    break;
	else if (cst_streq(word,word_pos))
	    return w;
        /* go to next entry */
	w = lex_data_next_entry(l,w,l->num_bytes);
    }

    return match;
}

static int lex_match_entry(const char *a, const char *b)
{
    int c;

    c = strcmp(a+1,b+1);

    return c;
}


