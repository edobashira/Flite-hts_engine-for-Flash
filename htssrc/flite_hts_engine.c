/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis System (HTS)             */
/*           flite+hts_engine developed by HTS Working Group         */
/*           http://hts.sp.nitech.ac.jp/                             */
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

#include "cst_synth.h"
#include "cst_utt_utils.h"
#include "cst_math.h"
#include "cst_file.h"
#include "cst_val.h"
#include "cst_string.h"
#include "cst_alloc.h"
#include "cst_item.h"
#include "cst_relation.h"
#include "cst_utterance.h"
#include "cst_tokenstream.h"
#include "cst_string.h"
#include "cst_regex.h"
#include "cst_features.h"
#include "cst_utterance.h"
#include "flite.h"
#include "flite_version.h"
#include "cst_synth.h"
#include "cst_utt_utils.h"

#define REGISTER_VOX register_cmu_us_kal
#define UNREGISTER_VOX unregister_cmu_us_kal

#include "flite_hts_engine.h"

#define MAXBUFLEN 1024

static void Flite_HTS_Engine_create_label(Flite_HTS_Engine * f, cst_item * item,
                                          char *label)
{
   char seg_pp[8];
   char seg_p[8];
   char seg_c[8];
   char seg_n[8];
   char seg_nn[8];
   int sub_phrases = 0;
   int lisp_total_phrases = 0;
   int tmp1 = 0;
   int tmp2 = 0;
   int tmp3 = 0;
   int tmp4 = 0;

   /* load segments */
   strcpy(seg_pp, ffeature_string(item, "p.p.name"));
   strcpy(seg_p, ffeature_string(item, "p.name"));
   strcpy(seg_c, ffeature_string(item, "name"));
   strcpy(seg_n, ffeature_string(item, "n.name"));
   strcpy(seg_nn, ffeature_string(item, "n.n.name"));

   if (strcmp(seg_c, "pau") == 0) {
      /* for pause */
      if (item_next(item) != NULL) {
         sub_phrases =
             ffeature_int(item,
                          "n.R:SylStructure.parent.R:Syllable.sub_phrases");
         tmp1 =
             ffeature_int(item,
                          "n.R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_syls");
         tmp2 =
             ffeature_int(item,
                          "n.R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_words");
         lisp_total_phrases =
             ffeature_int(item,
                          "n.R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_phrases");
      } else {
         sub_phrases =
             ffeature_int(item,
                          "p.R:SylStructure.parent.R:Syllable.sub_phrases");
         tmp1 =
             ffeature_int(item,
                          "p.R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_syls");
         tmp2 =
             ffeature_int(item,
                          "p.R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_words");
         lisp_total_phrases =
             ffeature_int(item,
                          "p.R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_phrases");
      }
      sprintf(label,
              "%s^%s-%s+%s=%s@x_x/A:%d_%d_%d/B:x-x-x@x-x&x-x#x-x$x-x!x-x;x-x|x/C:%d+%d+%d/D:%s_%d/E:x+x@x+x&x+x#x+x/F:%s_%d/G:%d_%d/H:x=x@%d=%d|%s/I:%d=%d/J:%d+%d-%d",
              strcmp(seg_pp, "0") == 0 ? "x" : seg_pp,
              strcmp(seg_p, "0") == 0 ? "x" : seg_p,
              seg_c,
              strcmp(seg_n, "0") == 0 ? "x" : seg_n,
              strcmp(seg_nn, "0") == 0 ? "x" : seg_nn,
              ffeature_int(item, "p.R:SylStructure.parent.R:Syllable.stress"),
              ffeature_int(item, "p.R:SylStructure.parent.R:Syllable.accented"),
              ffeature_int(item,
                           "p.R:SylStructure.parent.R:Syllable.syl_numphones"),
              ffeature_int(item, "n.R:SylStructure.parent.R:Syllable.stress"),
              ffeature_int(item, "n.R:SylStructure.parent.R:Syllable.accented"),
              ffeature_int(item,
                           "n.R:SylStructure.parent.R:Syllable.syl_numphones"),
              ffeature_string(item,
                              "p.R:SylStructure.parent.parent.R:Word.gpos"),
              ffeature_int(item,
                           "p.R:SylStructure.parent.parent.R:Word.word_numsyls"),
              ffeature_string(item,
                              "n.R:SylStructure.parent.parent.R:Word.gpos"),
              ffeature_int(item,
                           "n.R:SylStructure.parent.parent.R:Word.word_numsyls"),
              ffeature_int(item,
                           "p.R:SylStructure.parent.parent.R:Phrase.parent.lisp_num_syls_in_phrase"),
              ffeature_int(item,
                           "p.R:SylStructure.parent.parent.R:Phrase.parent.lisp_num_words_in_phrase"),
              sub_phrases + 1, lisp_total_phrases - sub_phrases,
              ffeature_string(item,
                              "R:SylStructure.parent.parent.R:Phrase.parent.daughtern.R:SylStructure.daughtern.endtone"),
              ffeature_int(item,
                           "n.R:SylStructure.parent.parent.R:Phrase.parent.lisp_num_syls_in_phrase"),
              ffeature_int(item,
                           "n.R:SylStructure.parent.parent.R:Phrase.parent.lisp_num_words_in_phrase"),
              tmp1, tmp2, lisp_total_phrases);
   } else {
      /* for no pause */
      tmp1 = ffeature_int(item, "R:SylStructure.pos_in_syl");
      tmp2 =
          ffeature_int(item, "R:SylStructure.parent.R:Syllable.syl_numphones");
      tmp3 = ffeature_int(item, "R:SylStructure.parent.R:Syllable.pos_in_word");
      tmp4 =
          ffeature_int(item,
                       "R:SylStructure.parent.parent.R:Word.word_numsyls");
      sub_phrases =
          ffeature_int(item, "R:SylStructure.parent.R:Syllable.sub_phrases");
      lisp_total_phrases =
          ffeature_int(item,
                       "R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_phrases");
      sprintf(label,
              "%s^%s-%s+%s=%s@%d_%d/A:%d_%d_%d/B:%d-%d-%d@%d-%d&%d-%d#%d-%d$%d-%d!%d-%d;%d-%d|%s/C:%d+%d+%d/D:%s_%d/E:%s+%d@%d+%d&%d+%d#%d+%d/F:%s_%d/G:%d_%d/H:%d=%d@%d=%d|%s/I:%d=%d/J:%d+%d-%d",
              strcmp(seg_pp, "0") == 0 ? "x" : seg_pp, strcmp(seg_p,
                                                              "0") ==
              0 ? "x" : seg_p, seg_c, strcmp(seg_n, "0") == 0 ? "x" : seg_n,
              strcmp(seg_nn, "0") == 0 ? "x" : seg_nn, tmp1 + 1, tmp2 - tmp1,
              ffeature_int(item, "R:SylStructure.parent.R:Syllable.p.stress"),
              ffeature_int(item, "R:SylStructure.parent.R:Syllable.p.accented"),
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.p.syl_numphones"),
              ffeature_int(item, "R:SylStructure.parent.R:Syllable.stress"),
              ffeature_int(item, "R:SylStructure.parent.R:Syllable.accented"),
              tmp2, tmp3 + 1, tmp4 - tmp3, ffeature_int(item,
                                                        "R:SylStructure.parent.R:Syllable.syl_in")
              + 1, ffeature_int(item,
                                "R:SylStructure.parent.R:Syllable.syl_out") + 1,
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.ssyl_in") + 1,
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.ssyl_out") + 1,
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.asyl_in") + 1,
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.asyl_out") + 1,
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.lisp_distance_to_p_stress"),
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.lisp_distance_to_n_stress"),
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.lisp_distance_to_p_accent"),
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.lisp_distance_to_n_accent"),
              ffeature_string(item,
                              "R:SylStructure.parent.R:Syllable.syl_vowel"),
              ffeature_int(item, "R:SylStructure.parent.R:Syllable.n.stress"),
              ffeature_int(item, "R:SylStructure.parent.R:Syllable.n.accented"),
              ffeature_int(item,
                           "R:SylStructure.parent.R:Syllable.n.syl_numphones"),
              ffeature_string(item,
                              "R:SylStructure.parent.parent.R:Word.p.gpos"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Word.p.word_numsyls"),
              ffeature_string(item, "R:SylStructure.parent.parent.R:Word.gpos"),
              tmp4, ffeature_int(item,
                                 "R:SylStructure.parent.parent.R:Word.pos_in_phrase")
              + 1, ffeature_int(item,
                                "R:SylStructure.parent.parent.R:Word.words_out"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Word.content_words_in")
              + 1, ffeature_int(item,
                                "R:SylStructure.parent.parent.R:Word.content_words_out")
              + 1, ffeature_int(item,
                                "R:SylStructure.parent.parent.R:Word.lisp_distance_to_p_content"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Word.lisp_distance_to_n_content"),
              ffeature_string(item,
                              "R:SylStructure.parent.parent.R:Word.n.gpos"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Word.n.word_numsyls"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.p.lisp_num_syls_in_phrase"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.p.lisp_num_words_in_phrase"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.lisp_num_syls_in_phrase"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.lisp_num_words_in_phrase"),
              sub_phrases + 1, lisp_total_phrases - sub_phrases,
              ffeature_string(item,
                              "R:SylStructure.parent.parent.R:Phrase.parent.daughtern.R:SylStructure.daughtern.endtone"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.n.lisp_num_syls_in_phrase"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.n.lisp_num_words_in_phrase"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_syls"),
              ffeature_int(item,
                           "R:SylStructure.parent.parent.R:Phrase.parent.lisp_total_words"),
              lisp_total_phrases);
   }
}

/* Flite_HTS_Engine_initialize: initialize system */
void Flite_HTS_Engine_initialize(Flite_HTS_Engine * f,
                                 int sampling_rate,
                                 int fperiod,
                                 double alpha,
                                 int stage,
                                 double beta,
                                 int audio_buff_size,
                                 double uv_threshold,
                                 HTS_Boolean use_log_gain,
                                 double gv_weight_mcp, double gv_weight_lf0)
{
   HTS_Engine_initialize(&f->engine, 2);
   HTS_Engine_set_sampling_rate(&f->engine, sampling_rate);
   HTS_Engine_set_fperiod(&f->engine, fperiod);
   HTS_Engine_set_alpha(&f->engine, alpha);
   HTS_Engine_set_gamma(&f->engine, stage);
   HTS_Engine_set_log_gain(&f->engine, use_log_gain);
   HTS_Engine_set_beta(&f->engine, beta);
   HTS_Engine_set_audio_buff_size(&f->engine, audio_buff_size);
   HTS_Engine_set_msd_threshold(&f->engine, 1, uv_threshold);
   HTS_Engine_set_gv_weight(&f->engine, 0, gv_weight_mcp);
   HTS_Engine_set_gv_weight(&f->engine, 1, gv_weight_lf0);
   printf("Initialized HTS Engine\n");
}

/* Flite_HTS_Engine_load: load files */
void Flite_HTS_Engine_load(Flite_HTS_Engine * f, FILE * fp_ms_dur,
                           FILE * fp_ts_dur, FILE * fp_ms_mcp, FILE * fp_ts_mcp,
                           FILE ** fp_ws_mcp, int num_ws_mcp, FILE * fp_ms_lf0,
                           FILE * fp_ts_lf0, FILE ** fp_ws_lf0, int num_ws_lf0,
                           FILE * fp_ms_gvm, FILE * fp_ts_gvm, FILE * fp_ms_gvl,
                           FILE * fp_ts_gvl, FILE * fp_gv_switch)
{
   HTS_Engine_load_duration_from_fp(&f->engine, &fp_ms_dur, &fp_ts_dur, 1);
   HTS_Engine_load_parameter_from_fp(&f->engine, &fp_ms_mcp, &fp_ts_mcp,
                                     fp_ws_mcp, 0, FALSE, num_ws_mcp, 1);
   HTS_Engine_load_parameter_from_fp(&f->engine, &fp_ms_lf0, &fp_ts_lf0,
                                     fp_ws_lf0, 1, TRUE, num_ws_lf0, 1);
   if (fp_ms_gvm != NULL) {
      if (fp_ts_gvm != NULL)
         HTS_Engine_load_gv_from_fp(&f->engine, &fp_ms_gvm, &fp_ts_gvm, 0, 1);
      else
         HTS_Engine_load_gv_from_fp(&f->engine, &fp_ms_gvm, NULL, 0, 1);
   }
   if (fp_ms_gvl != NULL) {
      if (fp_ts_gvl != NULL)
         HTS_Engine_load_gv_from_fp(&f->engine, &fp_ms_gvl, &fp_ts_gvl, 1, 1);
      else
         HTS_Engine_load_gv_from_fp(&f->engine, &fp_ms_gvl, NULL, 1, 1);
   }
   if (fp_gv_switch != NULL)
      HTS_Engine_load_gv_switch_from_fp(&f->engine, fp_gv_switch);
}

cst_voice *REGISTER_VOX(const char *voxdir);
cst_voice *UNREGISTER_VOX(cst_voice * vox);

/* Flite_HTS_Engine_synthesis: speech synthesis */
void Flite_HTS_Engine_synthesis(Flite_HTS_Engine * f, char *txt, FILE * wavfp)
{
   int i;
   cst_voice *v = NULL;
   cst_utterance *u = NULL;
   cst_item *s = NULL;
   char **label_data = NULL;
   int label_size = 0;

   /* text analysis part */
   v = REGISTER_VOX(NULL);
   if (v == NULL)
      return;
   u = flite_synth_text(txt, v);
   if (u == NULL)
      return;
   for (s = relation_head(utt_relation(u, "Segment")); s; s = item_next(s))
      label_size++;
   if (label_size <= 0)
      return;
   label_data = (char **) calloc(label_size, sizeof(char *));
   for (i = 0, s = relation_head(utt_relation(u, "Segment")); s;
        s = item_next(s), i++) {
      label_data[i] = (char *) calloc(MAXBUFLEN, sizeof(char));
      Flite_HTS_Engine_create_label(f, s, label_data[i]);
   }

   /* speech synthesis part */
   HTS_Engine_load_label_from_string_list(&f->engine, label_data, label_size);
   HTS_Engine_create_sstream(&f->engine);
   HTS_Engine_create_pstream(&f->engine);
   HTS_Engine_create_gstream(&f->engine);
   if (wavfp != NULL)   
      HTS_Engine_save_riff(&f->engine, wavfp);
   
   HTS_Engine_refresh(&f->engine);

   for (i = 0; i < label_size; i++)
      free(label_data[i]);
   free(label_data);

   delete_utterance(u);
   UNREGISTER_VOX(v);
}

/* Flite_HTS_Engine_clear: free system */
void Flite_HTS_Engine_clear(Flite_HTS_Engine * f)
{
   HTS_Engine_clear(&f->engine);
}
