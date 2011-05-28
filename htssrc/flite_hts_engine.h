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

#include "HTS_engine.h"

typedef struct _Flite_HTS_Engine {
   HTS_Engine engine;
} Flite_HTS_Engine;

/* Flite_HTS_Engine_initialize: initialize system */
void Flite_HTS_Engine_initialize(Flite_HTS_Engine * f, int sampling_rate,
                                 int fperiod, double alpha, int stage,
                                 double beta, int audio_buff_size,
                                 double uv_threshold, HTS_Boolean use_log_gain,
                                 double gv_weight_mcp, double gv_weight_lf0);

/* Flite_HTS_Engine_load: load files */
void Flite_HTS_Engine_load(Flite_HTS_Engine * f, FILE * fp_ms_dur,
                           FILE * fp_ts_dur, FILE * fp_ms_mcp, FILE * fp_ts_mcp,
                           FILE ** fp_ws_mcp, int num_ws_mcp, FILE * fp_ms_lf0,
                           FILE * fp_ts_lf0, FILE ** fp_ws_lf0, int num_ws_lf0,
                           FILE * fp_ms_gvm, FILE * fp_ts_gvm, FILE * fp_ms_gvl,
                           FILE * fp_ts_gvl, FILE * fp_gv_switch);

/* Flite_HTS_Engine_synthesis: speech synthesis */
void Flite_HTS_Engine_synthesis(Flite_HTS_Engine * f, char *txt, FILE * wavfp);

/* Flite_HTS_Engine_clear: free system */
void Flite_HTS_Engine_clear(Flite_HTS_Engine * f);
