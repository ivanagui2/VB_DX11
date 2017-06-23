/* $Id: errmsg.cpp 62477 2016-07-22 18:27:37Z vboxsync $ */
/** @file
 * IPRT - Status code messages.
 */

/*
 * Copyright (C) 2006-2016 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */


/*********************************************************************************************************************************
*   Header Files                                                                                                                 *
*********************************************************************************************************************************/
#include <iprt/err.h>
#include "internal/iprt.h"

#include <iprt/asm.h>
#include <iprt/string.h>
#include <iprt/err.h>
#include <VBox/err.h>


/*********************************************************************************************************************************
*   Global Variables                                                                                                             *
*********************************************************************************************************************************/
/** Array of messages.
 * The data is generated by a sed script.
 */
static const RTSTATUSMSG  g_aStatusMsgs[] =
{
#if !defined(IPRT_NO_ERROR_DATA) && !defined(DOXYGEN_RUNNING)
# include "errmsgdata.h"
#else
    { "Success.", "Success.", "VINF_SUCCESS", 0 },
#endif
    { NULL, NULL, NULL, 0 }
};


/** Temporary buffers to format unknown messages in.
 * @{
 */
static char                 g_aszUnknownStr[4][64];
static RTSTATUSMSG          g_aUnknownMsgs[4] =
{
    { &g_aszUnknownStr[0][0], &g_aszUnknownStr[0][0], &g_aszUnknownStr[0][0], 0 },
    { &g_aszUnknownStr[1][0], &g_aszUnknownStr[1][0], &g_aszUnknownStr[1][0], 0 },
    { &g_aszUnknownStr[2][0], &g_aszUnknownStr[2][0], &g_aszUnknownStr[2][0], 0 },
    { &g_aszUnknownStr[3][0], &g_aszUnknownStr[3][0], &g_aszUnknownStr[3][0], 0 }
};
/** Last used index in g_aUnknownMsgs. */
static volatile uint32_t    g_iUnknownMsgs;
/** @} */


/**
 * Get the message corresponding to a given status code.
 *
 * @returns Pointer to read-only message description.
 * @param   rc      The status code.
 */
RTDECL(PCRTSTATUSMSG) RTErrGet(int rc)
{
    unsigned iFound = ~0U;
    unsigned i;
    for (i = 0; i < RT_ELEMENTS(g_aStatusMsgs) - 1; i++)
    {
        if (g_aStatusMsgs[i].iCode == rc)
        {
            /*
             * Found a match.
             * Since this isn't a unique key, we must check that it's not
             * one of those start/end #defines before we return.
             */
#define STR_ENDS_WITH(a_psz, a_cch, a_sz) \
    ( (a_cch) >= sizeof(a_sz) && !strncmp((a_psz) + (a_cch) - sizeof(a_sz) + 1, RT_STR_TUPLE(a_sz)) )
            size_t const cchDefine = strlen(g_aStatusMsgs[i].pszDefine);
            if (   !STR_ENDS_WITH(g_aStatusMsgs[i].pszDefine, cchDefine, "_FIRST")
                && !STR_ENDS_WITH(g_aStatusMsgs[i].pszDefine, cchDefine, "_LAST")
                && !STR_ENDS_WITH(g_aStatusMsgs[i].pszDefine, cchDefine, "_LOWEST")
                && !STR_ENDS_WITH(g_aStatusMsgs[i].pszDefine, cchDefine, "_HIGHEST")
               )
                return &g_aStatusMsgs[i];
            iFound = i;
        }
    }
    if (iFound != ~0U)
        return &g_aStatusMsgs[iFound];

    /*
     * Need to use the temporary stuff.
     */
    int iMsg = ASMAtomicXchgU32(&g_iUnknownMsgs, (g_iUnknownMsgs + 1) % RT_ELEMENTS(g_aUnknownMsgs));
    RTStrPrintf(&g_aszUnknownStr[iMsg][0], sizeof(g_aszUnknownStr[iMsg]), "Unknown Status %d (%#x)", rc, rc);
    return &g_aUnknownMsgs[iMsg];
}
RT_EXPORT_SYMBOL(RTErrGet);

