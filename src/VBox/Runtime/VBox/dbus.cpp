/* $Id: dbus.cpp 62477 2016-07-22 18:27:37Z vboxsync $ */
/** @file
 *
 * Module to dynamically load libdbus and load all symbols
 * which are needed by VirtualBox.
 */

/*
 * Copyright (C) 2008-2016 Oracle Corporation
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

#define LOG_GROUP RTLOGGROUP_LDR
#include <VBox/dbus.h>

/* Declarations of the functions that we need from libdbus-1 */
#define VBOX_DBUS_GENERATE_BODY
#include <VBox/dbus-calls.h>