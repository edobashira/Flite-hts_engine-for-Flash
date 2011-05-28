/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2000                            */
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
/*               Date:  September 2000                                   */
/*************************************************************************/
/*                                                                       */
/*  Basic user level functions                                           */
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

#include "cst_tokenstream.h"
#include "flite.h"

#ifndef FLITE_PLUS_HTS_ENGINE
int flite_init()
{
    cst_regex_init();

    return 0;
}
#endif /* !FLITE_PLUS_HTS_ENGINE */

static cst_utterance *flite_synth_foo(cst_utterance *u,
				      cst_voice *voice,
				      cst_uttfunc synth)
{		       
    utt_init(u, voice);
    if ((*synth)(u) == NULL)
    {
	delete_utterance(u);
	return NULL;
    }
    else
	return u;
}

cst_utterance *flite_synth_text(const char *text, cst_voice *voice)
{
    cst_utterance *u;

    u = new_utterance();
    utt_set_input_text(u,text);
    return flite_synth_foo(u, voice, utt_synth);
}

#ifndef FLITE_PLUS_HTS_ENGINE
cst_utterance *flite_synth_phones(const char *text, cst_voice *voice)
{
    cst_utterance *u;

    u = new_utterance();
    utt_set_input_text(u,text);
    return flite_synth_foo(u, voice, utt_synth_phones);
}

cst_wave *flite_text_to_wave(const char *text, cst_voice *voice)
{
    cst_utterance *u;
    cst_wave *w;

    if ((u = flite_synth_text(text,voice)) == NULL)
	return NULL;

    w = copy_wave(utt_wave(u));
    delete_utterance(u);
    return w;
}

float flite_file_to_speech(const char *filename, 
			   cst_voice *voice,
			   const char *outtype)
{
    cst_utterance *utt;
    cst_tokenstream *ts;
    const char *token;
    cst_item *t;
    cst_relation *tokrel;
    float d, durs = 0;
    int num_tokens;
    cst_breakfunc breakfunc = default_utt_break;

    if ((ts = ts_open(filename,
	      get_param_string(voice->features,"text_whitespace",NULL),
	      get_param_string(voice->features,"text_singlecharsymbols",NULL),
	      get_param_string(voice->features,"text_prepunctuation",NULL),
	      get_param_string(voice->features,"text_postpunctuation",NULL)))
	== NULL)
    {
	cst_errmsg("failed to open file \"%s\" for reading\n",
		   filename);
	return 1;
    }

    if (feat_present(voice->features,"utt_break"))
	breakfunc = val_breakfunc(feat_val(voice->features,"utt_break"));

    /* If its a file to write to delete it as we're going to */
    /* incrementally append to it                            */
    if (!cst_streq(outtype,"play") && !cst_streq(outtype,"none"))
    {
	cst_wave *w;
	w = new_wave();
	cst_wave_resize(w,0,1);
	cst_wave_set_sample_rate(w,16000);
	cst_wave_save_riff(w,outtype);  /* an empty wave */
	delete_wave(w);
    }

    num_tokens = 0;
    utt = new_utterance();
    tokrel = utt_relation_create(utt, "Token");
    while (!ts_eof(ts) || num_tokens > 0)
    {
	token = ts_get(ts);
	if ((strlen(token) == 0) ||
	    (num_tokens > 500) ||  /* need an upper bound */
	    (relation_head(tokrel) && 
	     breakfunc(ts,token,tokrel)))
	{
	    /* An end of utt */
	    d = flite_tokens_to_speech(utt,voice,outtype);
	    utt = NULL;
	    if (d < 0)
		goto out;
	    durs += d;

	    if (ts_eof(ts))
		goto out;

	    utt = new_utterance();
	    tokrel = utt_relation_create(utt, "Token");
	    num_tokens = 0;
	}
	num_tokens++;

	t = relation_append(tokrel, NULL);
	item_set_string(t,"name",token);
	item_set_string(t,"whitespace",ts->whitespace);
	item_set_string(t,"prepunctuation",ts->prepunctuation);
	item_set_string(t,"punc",ts->postpunctuation);
	item_set_int(t,"file_pos",ts->file_pos);
	item_set_int(t,"line_number",ts->line_number);
    }

out:
    delete_utterance(utt);
    ts_close(ts);
    return durs;
}

float flite_text_to_speech(const char *text,
			   cst_voice *voice,
			   const char *outtype)
{
    cst_utterance *u;
    cst_wave *w;
    float durs;

    u = flite_synth_text(text,voice);
    if (u == NULL)
	return -1;

    w = utt_wave(u);

    durs = (float)w->num_samples/(float)w->sample_rate;
	     
    if (cst_streq(outtype,"play"))
	play_wave(w);
    else if (!cst_streq(outtype,"none"))
	cst_wave_save_riff(w,outtype);
    delete_utterance(u);

    return durs;
}

float flite_phones_to_speech(const char *text,
			     cst_voice *voice,
			     const char *outtype)
{
    cst_utterance *u;
    cst_wave *w;
    float durs;

    u = flite_synth_phones(text,voice);
    if (u == NULL)
	    return -1;
    w = utt_wave(u);

    durs = (float)w->num_samples/(float)w->sample_rate;
	     
    if (cst_streq(outtype,"play"))
	play_wave(w);
    else if (!cst_streq(outtype,"none"))
	cst_wave_save_riff(w,outtype);
    delete_utterance(u);

    return durs;
}

float flite_tokens_to_speech(cst_utterance *u,
			     cst_voice *voice,
			     const char *outtype)
{
    cst_wave *w;
    float durs;

    u = flite_synth_foo(u,voice,utt_synth_tokens);
    if (u == NULL)
	    return -1;
    w = utt_wave(u);

    durs = (float)w->num_samples/(float)w->sample_rate;

    if (cst_streq(outtype,"play"))
	play_wave(w);
    else if (!cst_streq(outtype,"none"))
	cst_wave_append_riff(w,outtype);
    delete_utterance(u);

    return durs;
}
#endif /* !FLITE_PLUS_HTS_ENGINE */
