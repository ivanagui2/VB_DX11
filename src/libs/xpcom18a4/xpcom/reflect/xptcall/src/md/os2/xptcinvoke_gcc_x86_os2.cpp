/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/* Platform specific code to invoke XPCOM methods on native objects */

/* This file is basically a copy of xptcinvoke_gcc_x86_unix.cpp with some
 * irrelevant parts stipped off and some OS/2 specific modifications added,
 * so it needs to be kept in sync with the original. */

#include "xptcprivate.h"
#include "xptc_platforms_unixish_x86.h"
#include "xptc_gcc_x86_unix.h"

extern "C" {
static
void ATTRIBUTE_USED __attribute__ ((regparm(3)))
invoke_copy_to_stack(PRUint32 paramCount, nsXPTCVariant* s, PRUint32* d)
{
    for(PRUint32 i = paramCount; i >0; i--, d++, s++)
    {
        if(s->IsPtrData())
        {
            *((void**)d) = s->ptr;
            continue;
        }

        switch(s->type)
        {
        case nsXPTType::T_I64    : *((PRInt64*) d) = s->val.i64; d++;    break;
        case nsXPTType::T_U64    : *((PRUint64*)d) = s->val.u64; d++;    break;
        case nsXPTType::T_DOUBLE : *((double*)  d) = s->val.d;   d++;    break;
        default                  : *((void**)d)    = s->val.p;           break;
        }
    }
}
} // extern "C"

/*
  XPTC_PUBLIC_API(nsresult)
  XPTC_InvokeByIndex(nsISupports* that, PRUint32 methodIndex,
		     PRUint32 paramCount, nsXPTCVariant* params);

  Each param takes at most two 4-byte words.
  It doesn't matter if we push too many words, and calculating the exact
  amount takes time.

  that        = ebp + 0x08
  methodIndex = ebp + 0x0c
  paramCount  = ebp + 0x10
  params      = ebp + 0x14

  NOTE NOTE NOTE:
  As of 2002-04-29 this function references no global variables nor does
  it call non-static functions so preserving and loading the PIC register
  is unnecessary. Define MOZ_PRESERVE_PIC if this changes. See mozilla
  bug 140412 for details. However, avoid this if you can. It's slower.
*/

#if defined(__declspec)
#define SYMBOL_EXPORT(sym) \
    ".stabs \"" sym ",0=" sym ",code\", 0x6c,0,0,-42\n\t"
#else
#define SYMBOL_EXPORT(sym)
#endif 

__asm__ (
	".text\n\t"
/* alignment here seems unimportant here; this was 16, now it's 2 which
   is what xptcstubs uses. */
	".align 2\n\t"
	".globl " SYMBOL_UNDERSCORE "XPTC_InvokeByIndex\n\t"
	".type  " SYMBOL_UNDERSCORE "XPTC_InvokeByIndex,@function\n"
        SYMBOL_EXPORT(SYMBOL_UNDERSCORE "XPTC_InvokeByIndex")
	SYMBOL_UNDERSCORE "XPTC_InvokeByIndex:\n\t"
	"pushl %ebp\n\t"
	"movl  %esp, %ebp\n\t"
#ifdef MOZ_PRESERVE_PIC 
	"pushl %ebx\n\t"
	"call  0f\n\t"
	".subsection 1\n"
	"0:\n\t"
	"movl (%esp), %ebx\n\t"
	"ret\n\t"
	".previous\n\t"
	"addl  $_GLOBAL_OFFSET_TABLE_, %ebx\n\t"
#endif
	"movl  0x10(%ebp), %eax\n\t"
	"leal  0(,%eax,8),%edx\n\t"
	"movl  %esp, %ecx\n\t"
	"subl  %edx, %ecx\n\t"
/* Since there may be 64-bit data, it occurs to me that aligning this
   space might be a performance gain. However, I don't think the rest
   of mozilla worries about such things. In any event, do it here.
	"andl  $0xfffffff8, %ecx\n\t"
 */
	"movl  %ecx, %esp\n\t"          /* make stack space */
	"movl  0x14(%ebp), %edx\n\t"
	"call  " SYMBOL_UNDERSCORE "invoke_copy_to_stack\n\t"
	"movl  0x08(%ebp), %ecx\n\t"	/* 'that' */
#ifdef CFRONT_STYLE_THIS_ADJUST
	"movl  (%ecx), %edx\n\t"
	"movl  0x0c(%ebp), %eax\n\t"    /* function index */
	"shll  $3, %eax\n\t"	        /* *= 8 */
	"addl  $8, %eax\n\t"	        /* += 8 skip first entry */
	"addl  %eax, %edx\n\t"
	"movswl (%edx), %eax\n\t"       /* 'this' offset */
	"addl  %eax, %ecx\n\t"
	"pushl %ecx\n\t"
	"addl  $4, %edx\n\t"	        /* += 4, method pointer */
#else /* THUNK_BASED_THIS_ADJUST */
	"pushl %ecx\n\t"
	"movl  (%ecx), %edx\n\t"
	"movl  0x0c(%ebp), %eax\n\t"    /* function index */
	"leal  (%edx,%eax,4), %edx\n\t"
#endif
	"call  *(%edx)\n\t"
#ifdef MOZ_PRESERVE_PIC
	"movl  -4(%ebp), %ebx\n\t"
#endif
	"movl  %ebp, %esp\n\t"
	"popl  %ebp\n\t"
	"ret\n"
	".size " SYMBOL_UNDERSCORE "XPTC_InvokeByIndex, . -" SYMBOL_UNDERSCORE "XPTC_InvokeByIndex\n\t"
);

