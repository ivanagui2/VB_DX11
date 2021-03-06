/* $Id: VideoRec.h 65435 2017-01-24 16:53:55Z vboxsync $ */
/** @file
 * Encodes the screen content in VPX format.
 */

/*
 * Copyright (C) 2012-2017 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ____H_VIDEOREC
#define ____H_VIDEOREC

struct VIDEORECCONTEXT;
typedef struct VIDEORECCONTEXT *PVIDEORECCONTEXT;

struct VIDEORECSTREAM;
typedef struct VIDEORECSTREAM *PVIDEORECSTREAM;

int VideoRecContextCreate(uint32_t cScreens, PVIDEORECCONTEXT *ppCtx);
int VideoRecContextDestroy(PVIDEORECCONTEXT pCtx);

int  VideoRecStreamInit(PVIDEORECCONTEXT pCtx, uint32_t uScreen, const char *pszFile,
                        uint32_t uWidth, uint32_t uHeight, uint32_t uRate, uint32_t uFPS,
                        uint32_t uMaxTimeS, uint32_t uMaxFileSizeMB, const char *pszOptions);

bool VideoRecIsEnabled(PVIDEORECCONTEXT pCtx);
int  VideoRecSendAudioFrame(PVIDEORECCONTEXT pCtx, const void *pvData, size_t cbData, uint64_t uTimestampMs);
int  VideoRecSendVideoFrame(PVIDEORECCONTEXT pCtx, uint32_t uScreen,
                            uint32_t x, uint32_t y, uint32_t uPixelFormat, uint32_t uBPP,
                            uint32_t uBytesPerLine, uint32_t uSrcWidth, uint32_t uSrcHeight,
                            uint8_t *puSrcData, uint64_t uTimeStampMs);
bool VideoRecIsReady(PVIDEORECCONTEXT pCtx, uint32_t uScreen, uint64_t uTimeStampMs);
bool VideoRecIsLimitReached(PVIDEORECCONTEXT pCtx, uint32_t uScreen, uint64_t tsNowMs);

#endif /* !____H_VIDEOREC */

