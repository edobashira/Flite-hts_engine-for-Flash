/*************************************************************************/
/*                                                                       */
/*                           Cepstral, LLC                               */
/*                        Copyright (c) 2001                             */
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
/*  CEPSTRAL, LLC AND THE CONTRIBUTORS TO THIS WORK DISCLAIM ALL         */
/*  WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED       */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL         */
/*  CEPSTRAL, LLC NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL,        */
/*  INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER          */
/*  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION    */
/*  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR  */
/*  IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                       */
/*************************************************************************/
/*             Author:  David Huggins-Daines <dhd@cepstral.com>          */
/*               Date:  August 2001                                      */
/*************************************************************************/
/*                                                                       */
/*  File I/O wrappers for normal platforms.                              */
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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cst_file.h"
#include "cst_error.h"
#include "cst_alloc.h"

#ifndef FLITE_PLUS_HTS_ENGINE
cst_file cst_fopen(const char *path, int mode)
{
    char cmode[4];

    /* This is kind of hacky. */
    if ((mode & CST_OPEN_WRITE) && (mode & CST_OPEN_READ))
	strcpy(cmode, "r+");
    else if ((mode & CST_OPEN_APPEND) && (mode & CST_OPEN_READ))
	strcpy(cmode, "a+");
    else if (mode & CST_OPEN_WRITE)
	strcpy(cmode, "w");
    else if (mode & CST_OPEN_APPEND)
	strcpy(cmode, "a");
    else if (mode & CST_OPEN_READ)
	strcpy(cmode, "r");

    if (mode & CST_OPEN_BINARY)
	strcat(cmode, "b");

    return fopen(path, cmode);
}

long cst_fwrite(cst_file fh, const void *buf, long size, long count)
{
    return fwrite(buf, size, count, fh);
}

long cst_fread(cst_file fh, void *buf, long size, long count)
{
    return fread(buf, size, count, fh);
}

long cst_filesize(cst_file fh)
{
	/* FIXME: guaranteed to break on text files on Win32 */
	long pos, epos;

	pos = ftell(fh);
	fseek(fh, 0, SEEK_END);
	epos = ftell(fh);
	fseek(fh, pos, SEEK_SET);

	return epos;
}

int cst_fgetc(cst_file fh)
{
    return fgetc(fh);
}

long cst_ftell(cst_file fh)
{
    return ftell(fh);
}

long cst_fseek(cst_file fh, long pos, int whence)
{
    int w = 0;

    if (whence == CST_SEEK_ABSOLUTE)
	w = SEEK_SET;
    else if (whence == CST_SEEK_RELATIVE)
	w = SEEK_CUR;
    else if (whence == CST_SEEK_ENDREL)
	w = SEEK_END;

    return fseek(fh, pos, w);
}

int cst_fprintf(cst_file fh, char *fmt, ...)
{
    va_list args;
    int rv;

    va_start(args, fmt);
    rv = vfprintf(fh, fmt, args);
    va_end(args);

    return rv;
}
#endif /* !FLITE_PLUS_HTS_ENGINE */

int cst_sprintf(char *s, const char *fmt, ...)
{
    va_list args;
    int rv;

    va_start(args, fmt);
    rv = vsprintf(s, fmt, args);
    va_end(args);

    return rv;
}

#ifndef FLITE_PLUS_HTS_ENGINE
int cst_fclose(cst_file fh)
{
    return fclose(fh);
}
#endif /* !FLITE_PLUS_HTS_ENGINE */
