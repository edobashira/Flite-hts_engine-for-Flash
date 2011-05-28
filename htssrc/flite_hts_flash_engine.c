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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "AS3.h"
#include "flite_hts_engine.h"

#define INPUT_BUFF_SIZE 1024

/* Usage: output usage */
void Usage(void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, "The HMM-based speech synthesis system (HTS)\n");
   fprintf(stderr,
           "flite+hts_engine version 1.00 (http://hts-engine.sourceforge.net/)\n");
   fprintf(stderr, "Copyright (C) 2005-2009  Nagoya Institute of Technology\n");
   fprintf(stderr, "              2005-2008  Tokyo Institute of Technology\n");
   fprintf(stderr, "All rights reserved.\n");
   HTS_show_copyright(stderr);
   fprintf(stderr, "\n");
   fprintf(stderr, "CMU Flite\n");
   fprintf(stderr, "Flite version 1.3 (http://www.speech.cs.cmu.edu/flite/)\n");
   fprintf(stderr, "Copyright (C) 1999-2005  Carnegie Mellon University\n");
   fprintf(stderr, "All rights reserved.\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "flite_hts_engine - An HMM-based speech synthesis engine\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       flite_hts_engine [ options ] [ infile ] \n");
   fprintf(stderr,
           "  options:                                                                   [  def][ min--max]\n");
   fprintf(stderr,
           "    -td tree       : decision trees file for state duration                  [  N/A]\n");
   fprintf(stderr,
           "    -tf tree       : decision trees file for Log F0                          [  N/A]\n");
   fprintf(stderr,
           "    -tm tree       : decision trees file for spectrum                        [  N/A]\n");
   fprintf(stderr,
           "    -md pdf        : model file for state duration                           [  N/A]\n");
   fprintf(stderr,
           "    -mf pdf        : model file for Log F0                                   [  N/A]\n");
   fprintf(stderr,
           "    -mm pdf        : model file for spectrum                                 [  N/A]\n");
   fprintf(stderr,
           "    -df win        : window files for calculation delta of Log F0            [  N/A]\n");
   fprintf(stderr,
           "    -dm win        : window files for calculation delta of spectrum          [  N/A]\n");
   fprintf(stderr,
           "    -o  wav        : filename of output wav audio (generated speech)         [  N/A]\n");
   fprintf(stderr,
           "    -s  i          : sampling frequency                                      [16000][   1--48000]\n");
   fprintf(stderr,
           "    -p  i          : frame period (point)                                    [   80][   1--]\n");
   fprintf(stderr,
           "    -a  f          : all-pass constant                                       [ 0.42][ 0.0--1.0]\n");
   fprintf(stderr,
           "    -g  i          : gamma = -1 / i (if i=0 then gamma=0)                    [    0][   0-- ]\n");
   fprintf(stderr,
           "    -b  f          : postfiltering coefficient                               [  0.0][-0.8--0.8]\n");
   fprintf(stderr,
           "    -l             : regard input as log gain and output linear one (LSP)    [  N/A]\n");
   fprintf(stderr,
           "    -u  f          : voiced/unvoiced threshold                               [  0.5][ 0.0--1.0]\n");
   fprintf(stderr,
           "    -ef tree       : decision tree file for GV of Log F0                     [  N/A]\n");
   fprintf(stderr,
           "    -em tree       : decision tree file for GV of spectrum                   [  N/A]\n");
   fprintf(stderr,
           "    -cf pdf        : filename of GV for Log F0                               [  N/A]\n");
   fprintf(stderr,
           "    -cm pdf        : filename of GV for spectrum                             [  N/A]\n");
   fprintf(stderr,
           "    -jf f          : weight of GV for Log F0                                 [  0.7][ 0.0--2.0]\n");
   fprintf(stderr,
           "    -jm f          : weight of GV for spectrum                               [  1.0][ 0.0--2.0]\n");
   fprintf(stderr,
           "    -k  tree       : GV switch                                               [  N/A]\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr,
           "    text file                                                                [stdin]\n");
   fprintf(stderr, "  note:\n");
   fprintf(stderr,
           "    option '-d' may be repeated to use multiple delta parameters.\n");
   fprintf(stderr,
           "    generated spectrum and log F0 sequences are saved in natural\n");
   fprintf(stderr, "    endian, binary (float) format.\n");
   fprintf(stderr, "\n");

   exit(0);
}

/* Error: output error message */
void Error(const int error, char *message, ...)
{
   va_list arg;

   fflush(stdout);
   fflush(stderr);

   if (error > 0)
      fprintf(stderr, "\nError: ");
   else
      fprintf(stderr, "\nWarning: ");

   va_start(arg, message);
   vfprintf(stderr, message, arg);
   va_end(arg);

   fflush(stderr);

   if (error > 0)
      exit(error);
}

/* Getfp: wrapper for fopen */
FILE *Getfp(const char *name, const char *opt)
{
   FILE *fp = fopen(name, opt);

   if (fp == NULL)
	Error(2, "Getfp: Cannot open %s.\n", name);		  

   return (fp);
}

AS3_Val _swfMain;
double _oldtime;

void trace(char *fmt, ...)
{
	va_list		argptr;
	char		msg[10000] = "TRACE: ";
	AS3_Val as3Str;	
	va_start (argptr,fmt);
	vsprintf (&msg[7],fmt,argptr);
	va_end (argptr);
	as3Str = AS3_String(msg);
	AS3_Trace(as3Str);
	AS3_Release(as3Str);
}

/* Does a FILE * read against a ByteArray */
static int readByteArray(void *cookie, char *dst, int size)
{
	return AS3_ByteArray_readBytes(dst, (AS3_Val)cookie, size);
}
 
/* Does a FILE * write against a ByteArray */
static int writeByteArray(void *cookie, const char *src, int size)
{
	return AS3_ByteArray_writeBytes((AS3_Val)cookie, (char *)src, size);
}
 
/* Does a FILE * lseek against a ByteArray */
static fpos_t seekByteArray(void *cookie, fpos_t offs, int whence)
{
	return AS3_ByteArray_seek((AS3_Val)cookie, offs, whence);
}
 
/* Does a FILE * close against a ByteArray */
static int closeByteArray(void * cookie)
{
	AS3_Val zero = AS3_Int(0); 
	/* just reset the position */
	AS3_SetS((AS3_Val)cookie, "position", zero);
	AS3_Release(zero);
	return 0;
}

FILE* as3OpenWriteFile(const char* filename)
{
	FILE* ret;
	AS3_Val byteArray;
	AS3_Val params = AS3_Array("AS3ValType",AS3_String(filename));	
	byteArray = AS3_CallS("fileWriteSharedObject", _swfMain, params);
	AS3_Release(params);
	//This opens a file for writing on a ByteArray, as explained in http://blog.debit.nl/?p=79
	//It does NOT reset its length to 0, so this must already have been done.
	ret = funopen((void *)byteArray, readByteArray, writeByteArray, seekByteArray, closeByteArray);	
	return ret;
}

void as3UpdateFileSharedObject(const char* filename)
{
	AS3_Val params = AS3_Array("AS3ValType", AS3_String(filename));
	AS3_CallS("fileUpdateSharedObject", _swfMain, params);
	AS3_Release(params);
}

void as3ReadFileSharedObject(const char* filename)
{
	AS3_Val params = AS3_Array("AS3ValType", AS3_String(filename));
	AS3_CallS("fileReadSharedObject", _swfMain, params);
	AS3_Release(params);
}




Flite_HTS_Engine swcEngine;

AS3_Val swcInit(void *data, AS3_Val args)
{
	int argc = 35;
	char* argv[] = {"yeah!",
				  "-td","tree-dur.inf",
				  "-tf","tree-lf0.inf",				  
				  "-tm","tree-mgc.inf",
				  "-md","dur.pdf",
				  "-mf","lf0.pdf",
				  "-mm","mgc.pdf",
				  "-df","lf0.win1",
				  "-df","lf0.win2",
				  "-df","lf0.win3",
				  "-dm","mgc.win1",
				  "-dm","mgc.win2",
				  "-dm","mgc.win3",
				  "-cf","gv-lf0.pdf",
				  "-cm","gv-mgc.pdf",
				  "-ef","tree-gv-lf0.inf",
				  "-em","tree-gv-mgc.inf",
				  "-k","gv-switch.inf"};
	trace("swcInit");
	
	AS3_ArrayValue(args, "AS3ValType", &_swfMain);	
		
	trace("Flite_HTS_Engine_initialize");	
	
	swcmain(argc,argv);
	return AS3_Ram();
}




AS3_Val swcSynth(void *data, AS3_Val args)
{	
	char* buffer;
	FILE * file;
	void * dest; 
	int fileSize = 0;
	AS3_ArrayValue(args, "AS3ValType,StrType", &dest,&buffer);
	file = funopen((void *)dest, readByteArray, writeByteArray, seekByteArray, closeByteArray);		 			
	fprintf(stderr,"Synthesis: %s on filesize %d",buffer,fileSize);
	Flite_HTS_Engine_synthesis(&swcEngine, buffer, file);	
	return NULL;
}


int swcmain(int argc, char **argv)
{
   int i;
   int sampling_rate = 16000;
   int fperiod = 80;
   double alpha = 0.42;
   double stage = 0.0;          /* gamma = -1.0/stage */
   double beta = 0.0;
   int audio_buff_size = 1600;
   double uv_threshold = 0.5;
   HTS_Boolean use_log_gain = FALSE;
   double gv_weight_lf0 = 0.7;
   double gv_weight_mcp = 1.0;
   /*FILE *txtfp = stdin, *wavfp = NULL;
   char buff[INPUT_BUFF_SIZE];*/

   /* file pointers of models */
   FILE *fp_ms_lf0 = NULL;
   FILE *fp_ms_mcp = NULL;
   FILE *fp_ms_dur = NULL;

   /* file pointers of trees */
   FILE *fp_ts_lf0 = NULL;
   FILE *fp_ts_mcp = NULL;
   FILE *fp_ts_dur = NULL;

   /* file pointers of windows */
   FILE **fp_ws_lf0;
   FILE **fp_ws_mcp;
   int num_ws_lf0 = 0, num_ws_mcp = 0;

   /* file pointers of global variance */
   FILE *fp_ms_gvl = NULL;
   FILE *fp_ms_gvm = NULL;

   /* file pointers of global variance trees */
   FILE *fp_ts_gvl = NULL;
   FILE *fp_ts_gvm = NULL;

   /* file pointer of global variance switch */
   FILE *fp_gv_switch = NULL;   

   /* delta window handler for log f0 */
   fp_ws_lf0 = (FILE **) calloc(argc, sizeof(FILE *));
   /* delta window handler for mel-cepstrum */
   fp_ws_mcp = (FILE **) calloc(argc, sizeof(FILE *));

   /* read command */
   while (--argc) {
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 't':
            switch (*(*argv + 2)) {
            case 'f':
            case 'p':
               if (fp_ts_lf0 == NULL)
                  fp_ts_lf0 = Getfp(*++argv, "r");
               break;
            case 'm':
               if (fp_ts_mcp == NULL)
                  fp_ts_mcp = Getfp(*++argv, "r");
               break;
            case 'd':
               if (fp_ts_dur == NULL)
                  fp_ts_dur = Getfp(*++argv, "r");
               break;
            default:
               Error(1, "flite_hts_engine: Invalid option '-t%c'.\n",
                     *(*argv + 2));
            }
            --argc;
            break;
         case 'm':
            switch (*(*argv + 2)) {
            case 'f':
            case 'p':
               if (fp_ms_lf0 == NULL)
                  fp_ms_lf0 = Getfp(*++argv, "rb");
               break;
            case 'm':
               if (fp_ms_mcp == NULL)
                  fp_ms_mcp = Getfp(*++argv, "rb");
               break;
            case 'd':
               if (fp_ms_dur == NULL)
                  fp_ms_dur = Getfp(*++argv, "rb");
               break;
            default:
               Error(1, "flite_hts_engine: Invalid option '-m%c'.\n",
                     *(*argv + 2));
            }
            --argc;
            break;
         case 'd':
            switch (*(*argv + 2)) {
            case 'f':
            case 'p':
               fp_ws_lf0[num_ws_lf0++] = Getfp(*++argv, "r");
               break;
            case 'm':
               fp_ws_mcp[num_ws_mcp++] = Getfp(*++argv, "r");
               break;
            default:
               Error(1, "flite_hts_engine: Invalid option '-d%c'.\n",
                     *(*argv + 2));
            }
            --argc;
            break;
         case 'o':
            //wavfp = Getfp(*++argv, "wb");
			 //wavfp = as3OpenWriteFile(*++argv);
            --argc;
            break;
         case 'h':
            Usage();
            break;
         case 's':
            sampling_rate = atoi(*++argv);
            --argc;
            break;
         case 'p':
            fperiod = atoi(*++argv);
            --argc;
            break;
         case 'a':
            alpha = atof(*++argv);
            --argc;
            break;
         case 'g':
            stage = atoi(*++argv);
            --argc;
            break;
         case 'l':
            use_log_gain = TRUE;
            break;
         case 'b':
            beta = atof(*++argv);
            --argc;
            break;
         case 'u':
            uv_threshold = atof(*++argv);
            --argc;
            break;
         case 'e':
            switch (*(*argv + 2)) {
            case 'f':
            case 'p':
               if (fp_ts_gvl == NULL)
                  fp_ts_gvl = Getfp(*++argv, "r");
               break;
            case 'm':
               if (fp_ts_gvm == NULL)
                  fp_ts_gvm = Getfp(*++argv, "r");
               break;
            default:
               Error(1, "flite_hts_engine: Invalid option '-e%c'.\n",
                     *(*argv + 2));
            }
            --argc;
            break;
         case 'c':
            switch (*(*argv + 2)) {
            case 'f':
            case 'p':
               if (fp_ms_gvl == NULL)
                  fp_ms_gvl = Getfp(*++argv, "rb");
               break;
            case 'm':
               if (fp_ms_gvm == NULL)
                  fp_ms_gvm = Getfp(*++argv, "rb");
               break;
            default:
               Error(1, "flite_hts_engine: Invalid option '-c%c'.\n",
                     *(*argv + 2));
            }
            --argc;
            break;
         case 'j':
            switch (*(*argv + 2)) {
            case 'f':
            case 'p':
               gv_weight_lf0 = atof(*++argv);
               break;
            case 'm':
               gv_weight_mcp = atof(*++argv);
               break;
            default:
               Error(1, "flite_hts_engine: Invalid option '-j%c'.\n",
                     *(*argv + 2));
            }
            --argc;
            break;
         case 'k':
            if (fp_gv_switch == NULL)
               fp_gv_switch = Getfp(*++argv, "r");
            --argc;
            break;
         default:
            Error(1, "flite_hts_engine: Invalid option '-%c'.\n", *(*argv + 1));
         }
      } else {
         //txtfp = Getfp(*argv, "r");
      }
   }
   /* number of models,trees check */
   if (fp_ms_lf0 == NULL || fp_ms_mcp == NULL || fp_ms_dur == NULL ||
       fp_ts_lf0 == NULL || fp_ts_mcp == NULL || fp_ts_dur == NULL ||
       num_ws_lf0 == 0 || num_ws_mcp == 0) {
      Error(1, "flite_hts_engine: specify models(trees) for each parameter.\n");
   }

   ///* initialize */
   Flite_HTS_Engine_initialize(&swcEngine, sampling_rate, fperiod, alpha, stage,
                               beta, audio_buff_size, uv_threshold,
                               use_log_gain, gv_weight_mcp, gv_weight_lf0);

   /* load */
   Flite_HTS_Engine_load(&swcEngine, fp_ms_dur, fp_ts_dur, fp_ms_mcp, fp_ts_mcp,
                         fp_ws_mcp, num_ws_mcp, fp_ms_lf0, fp_ts_lf0, fp_ws_lf0,
                         num_ws_lf0, fp_ms_gvm, fp_ts_gvm, fp_ms_gvl, fp_ts_gvl,
                         fp_gv_switch);

  
   ///* free */
   //Flite_HTS_Engine_clear(&engine);

   //fclose(fp_ms_mcp);
   //fclose(fp_ms_lf0);
   //fclose(fp_ms_dur);
   //fclose(fp_ts_mcp);
   //fclose(fp_ts_lf0);
   //fclose(fp_ts_dur);
   //for (i = 0; i < num_ws_mcp; i++)
   //   fclose(fp_ws_mcp[i]);
   //for (i = 0; i < num_ws_lf0; i++)
   //   fclose(fp_ws_lf0[i]);
   //free(fp_ws_mcp);
   //free(fp_ws_lf0);
   //if (fp_ms_gvm != NULL)
   //   fclose(fp_ms_gvm);
   //if (fp_ts_gvm != NULL)
   //   fclose(fp_ts_gvm);
   //if (fp_ms_gvl != NULL)
   //   fclose(fp_ms_gvl);
   //if (fp_ts_gvl != NULL)
   //   fclose(fp_ts_gvl);
   //if (fp_gv_switch != NULL)
   //   fclose(fp_gv_switch);
   //if (wavfp != NULL)
   //   fclose(wavfp);
   //fclose(txtfp);
   return 0;
}


int main (int c, char **v)
{
	int i;
	AS3_Val swcEntries[] = 
	{
		AS3_Function(NULL, swcInit),
		AS3_Function(NULL, swcSynth)		
	};
	// construct an object that holds refereces to the functions
	AS3_Val result = AS3_Object(
		"swcInit:AS3ValType,swcSynth:AS3ValType",
		swcEntries[0],
		swcEntries[1]);

	for(i = 0; i < sizeof(swcEntries)/sizeof(swcEntries[0]); i++)
		AS3_Release(swcEntries[i]);
	
	// notify that we initialized -- THIS DOES NOT RETURN!
	AS3_LibInit(result);
}



