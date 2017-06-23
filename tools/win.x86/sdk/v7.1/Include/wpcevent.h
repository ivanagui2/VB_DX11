///////////////////////////////////////////////////////////////////////////////
//
//  File:  WpcEvent.h
//
//  Comments:
//      This file defines the Windows Parental Controls interfaces and events
//
//  Copyright (C) 2005 Microsoft Corporation  All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once 

#include <evntprov.h>

//
// Reasons
//
typedef enum tagWPCFLAG_ISBLOCKED
{
    WPCFLAG_ISBLOCKED_NOTBLOCKED            = 0x00000000,
    WPCFLAG_ISBLOCKED_IMBLOCKED             = 0x00000001,
    WPCFLAG_ISBLOCKED_EMAILBLOCKED          = 0x00000002,
    WPCFLAG_ISBLOCKED_MEDIAPLAYBACKBLOCKED  = 0x00000004,
    WPCFLAG_ISBLOCKED_WEBBLOCKED            = 0x00000008,
    WPCFLAG_ISBLOCKED_GAMESBLOCKED          = 0x00000010,
    WPCFLAG_ISBLOCKED_CONTACTBLOCKED        = 0x00000020, 
    WPCFLAG_ISBLOCKED_FEATUREBLOCKED        = 0x00000040, 
    WPCFLAG_ISBLOCKED_DOWNLOADBLOCKED       = 0x00000080, 
    WPCFLAG_ISBLOCKED_RATINGBLOCKED         = 0x00000100, 
    WPCFLAG_ISBLOCKED_DESCRIPTORBLOCKED     = 0x00000200, 
    WPCFLAG_ISBLOCKED_EXPLICITBLOCK         = 0x00000400,
    WPCFLAG_ISBLOCKED_BADPASS               = 0x00000800,
    WPCFLAG_ISBLOCKED_MAXHOURS              = 0x00001000,
    WPCFLAG_ISBLOCKED_SPECHOURS             = 0x00002000,
    
    WPCFLAG_ISBLOCKED_SETTINGSCHANGEBLOCKED = 0x00004000,

    WPCFLAG_ISBLOCKED_ATTACHMENTBLOCKED     = 0x00008000,
    WPCFLAG_ISBLOCKED_SENDERBLOCKED         = 0x00010000,
    WPCFLAG_ISBLOCKED_RECEIVERBLOCKED       = 0x00020000,
    WPCFLAG_ISBLOCKED_NOTEXPLICITLYALLOWED  = 0x00040000,
    WPCFLAG_ISBLOCKED_NOTINLIST             = 0x00080000,
    WPCFLAG_ISBLOCKED_CATEGORYBLOCKED       = 0x00100000,
    WPCFLAG_ISBLOCKED_CATEGORYNOTINLIST     = 0x00200000,
    WPCFLAG_ISBLOCKED_NOTKIDS               = 0x00400000,
    WPCFLAG_ISBLOCKED_UNRATED               = 0x00800000,
    WPCFLAG_ISBLOCKED_NOACCESS              = 0x01000000,
    WPCFLAG_ISBLOCKED_INTERNALERROR         = 0xFFFFFFFF
} WPCFLAG_ISBLOCKED;

typedef enum tagWPCFLAG_LOGOFF_TYPE
{
    WPCFLAG_LOGOFF_TYPE_LOGOUT              = 0x00000000,
    WPCFLAG_LOGOFF_TYPE_RESTART             = 0x00000001,
    WPCFLAG_LOGOFF_TYPE_SHUTDOWN            = 0x00000002,
    WPCFLAG_LOGOFF_TYPE_FUS                 = 0x00000004,
    WPCFLAG_LOGOFF_TYPE_FORCEDFUS           = 0x00000008
} WPCFLAG_LOGOFF_TYPE;

typedef enum tagWPCFLAG_IM_LEAVE_FLAG
{
    WPCFLAG_IM_LEAVE_NORMAL           = 0x00000000,
    WPCFLAG_IM_LEAVE_FORCED              = 0x00000001,
    WPCFLAG_IM_LEAVE_CONVERSATION_END  = 0x00000002 // This marks the end of the entire conversation
} WPCFLAG_IM_LEAVE;


//Event Data Lengths
//System

#define ARRAY_SEP_CHAR 0x9
typedef enum tagWPC_ARGS_SETTINGSCHANGEEVENT
{
    WPC_ARGS_SETTINGSCHANGEEVENT_CLASS = 0,
    WPC_ARGS_SETTINGSCHANGEEVENT_SETTING,
    WPC_ARGS_SETTINGSCHANGEEVENT_OWNER,
    WPC_ARGS_SETTINGSCHANGEEVENT_OLDVAL,
    WPC_ARGS_SETTINGSCHANGEEVENT_NEWVAL,
    WPC_ARGS_SETTINGSCHANGEEVENT_REASON,
    WPC_ARGS_SETTINGSCHANGEEVENT_OPTIONAL,
    WPC_ARGS_SETTINGSCHANGEEVENT_CARGS
} WPC_ARGS_SETTINGSCHANGEEVENT;


typedef enum tagWPC_ARGS_SAFERAPPBLOCKED
{
    WPC_ARGS_SAFERAPPBLOCKED_TIMESTAMP = 0,
    WPC_ARGS_SAFERAPPBLOCKED_USERID,
    WPC_ARGS_SAFERAPPBLOCKED_PATH,
    WPC_ARGS_SAFERAPPBLOCKED_RULEID,
    WPC_ARGS_SAFERAPPBLOCKED_CARGS
} WPC_ARGS_SAFERAPPBLOCKED;

//Email
typedef enum tagWPC_ARGS_EMAILRECEIEVEDEVENT
{
    WPC_ARGS_EMAILRECEIEVEDEVENT_SENDER = 0,
    WPC_ARGS_EMAILRECEIEVEDEVENT_APPNAME,
    WPC_ARGS_EMAILRECEIEVEDEVENT_APPVERSION,
    WPC_ARGS_EMAILRECEIEVEDEVENT_SUBJECT,
    WPC_ARGS_EMAILRECEIEVEDEVENT_REASON,
    WPC_ARGS_EMAILRECEIEVEDEVENT_RECIPCOUNT,
    WPC_ARGS_EMAILRECEIEVEDEVENT_RECIPIENT,
    WPC_ARGS_EMAILRECEIEVEDEVENT_ATTACHCOUNT,
    WPC_ARGS_EMAILRECEIEVEDEVENT_ATTACHMENTNAME,
    WPC_ARGS_EMAILRECEIEVEDEVENT_RECEIVEDTIME,
    WPC_ARGS_EMAILRECEIEVEDEVENT_EMAILACCOUNT,
    WPC_ARGS_EMAILRECEIEVEDEVENT_CARGS
} WPC_ARGS_EMAILRECEIEVEDEVENT;

typedef enum tagWPC_ARGS_EMAILSENTEVENT
{
    WPC_ARGS_EMAILSENTEVENT_SENDER = 0,
    WPC_ARGS_EMAILSENTEVENT_APPNAME,
    WPC_ARGS_EMAILSENTEVENT_APPVERSION,
    WPC_ARGS_EMAILSENTEVENT_SUBJECT,
    WPC_ARGS_EMAILSENTEVENT_REASON,
    WPC_ARGS_EMAILSENTEVENT_RECIPCOUNT,
    WPC_ARGS_EMAILSENTEVENT_RECIPIENT,
    WPC_ARGS_EMAILSENTEVENT_ATTACHCOUNT,
    WPC_ARGS_EMAILSENTEVENT_ATTACHMENTNAME,
    WPC_ARGS_EMAILSENTEVENT_EMAILACCOUNT,
    WPC_ARGS_EMAILSENTEVENT_CARGS
} WPC_ARGS_EMAILSENTEVENT;


typedef enum tagWPC_ARGS_EMAILCONTACTEVENT
{
    WPC_ARGS_EMAILCONTACTEVENT_APPNAME = 0,
    WPC_ARGS_EMAILCONTACTEVENT_APPVERSION,
    WPC_ARGS_EMAILCONTACTEVENT_OLDNAME,
    WPC_ARGS_EMAILCONTACTEVENT_OLDID,
    WPC_ARGS_EMAILCONTACTEVENT_NEWNAME,
    WPC_ARGS_EMAILCONTACTEVENT_NEWID,
    WPC_ARGS_EMAILCONTACTEVENT_REASON,
    WPC_ARGS_EMAILCONTACTEVENT_EMAILACCOUNT,
    WPC_ARGS_EMAILCONTACTEVENT_CARGS
} WPC_ARGS_EMAILCONTACTEVENT;

//Media
typedef enum tagWPC_MEDIA_TYPE
{   
    WPC_MEDIA_TYPE_OTHER    = 0,
    WPC_MEDIA_TYPE_DVD,
    WPC_MEDIA_TYPE_RECORDED_TV,
    WPC_MEDIA_TYPE_AUDIO_FILE,
    WPC_MEDIA_TYPE_CD_AUDIO,
    WPC_MEDIA_TYPE_VIDEO_FILE,
    WPC_MEDIA_TYPE_PICTURE_FILE,
    WPC_MEDIA_TYPE_MAX
}   WPC_MEDIA_TYPE;


typedef enum tagWPC_MEDIA_EXPLICIT_TYPE
{   
    WPC_MEDIA_EXPLICIT_FALSE = 0,
    WPC_MEDIA_EXPLICIT_TRUE,
    WPC_MEDIA_EXPLICIT_UNKNOWN
}   WPC_MEDIA_EXPLICIT;

typedef enum tagWPC_ARGS_MEDIAPLAYBACKEVENT
{
    WPC_ARGS_MEDIAPLAYBACKEVENT_APPNAME = 0,
    WPC_ARGS_MEDIAPLAYBACKEVENT_APPVERSION,
    WPC_ARGS_MEDIAPLAYBACKEVENT_MEDIATYPE,
    WPC_ARGS_MEDIAPLAYBACKEVENT_PATH,
    WPC_ARGS_MEDIAPLAYBACKEVENT_TITLE,
    WPC_ARGS_MEDIAPLAYBACKEVENT_PML,
    WPC_ARGS_MEDIAPLAYBACKEVENT_ALBUM,
    WPC_ARGS_MEDIAPLAYBACKEVENT_EXPLICIT,
    WPC_ARGS_MEDIAPLAYBACKEVENT_REASON,
    WPC_ARGS_MEDIAPLAYBACKEVENT_CARGS
} WPC_ARGS_MEDIAPLAYBACKEVENT;

typedef enum tagWPC_ARGS_MEDIADOWNLOADEVENT
{
    WPC_ARGS_MEDIADOWNLOADEVENT_APPNAME = 0,
    WPC_ARGS_MEDIADOWNLOADEVENT_APPVERSION,
    WPC_ARGS_MEDIADOWNLOADEVENT_MEDIATYPE,
    WPC_ARGS_MEDIADOWNLOADEVENT_PATH,
    WPC_ARGS_MEDIADOWNLOADEVENT_TITLE,
    WPC_ARGS_MEDIADOWNLOADEVENT_PML,
    WPC_ARGS_MEDIADOWNLOADEVENT_ALBUM,
    WPC_ARGS_MEDIADOWNLOADEVENT_EXPLICIT,
    WPC_ARGS_MEDIADOWNLOADEVENT_REASON,
    WPC_ARGS_MEDIADOWNLOADEVENT_CARGS
} WPC_ARGS_MEDIADOWNLOADEVENT;

//IM
typedef enum tagWPC_ARGS_CONVERSATIONINITEVENT
{
    WPC_ARGS_CONVERSATIONINITEVENT_APPNAME = 0,
    WPC_ARGS_CONVERSATIONINITEVENT_APPVERSION,
    WPC_ARGS_CONVERSATIONINITEVENT_ACCOUNTNAME,
    WPC_ARGS_CONVERSATIONINITEVENT_CONVID,
    WPC_ARGS_CONVERSATIONINITEVENT_REQUESTINGIP,
    WPC_ARGS_CONVERSATIONINITEVENT_SENDER,
    WPC_ARGS_CONVERSATIONINITEVENT_REASON,
    WPC_ARGS_CONVERSATIONINITEVENT_RECIPCOUNT,
    WPC_ARGS_CONVERSATIONINITEVENT_RECIPIENT,
    WPC_ARGS_CONVERSATIONINITEVENT_CARGS
} WPC_ARGS_CONVERSATIONINITEVENT;

typedef enum tagWPC_ARGS_CONVERSATIONJOINEVENT
{
    WPC_ARGS_CONVERSATIONJOINEVENT_APPNAME = 0,
    WPC_ARGS_CONVERSATIONJOINEVENT_APPVERSION,
    WPC_ARGS_CONVERSATIONJOINEVENT_ACCOUNTNAME,
    WPC_ARGS_CONVERSATIONJOINEVENT_CONVID,
    WPC_ARGS_CONVERSATIONJOINEVENT_JOININGIP,
    WPC_ARGS_CONVERSATIONJOINEVENT_JOININGUSER,
    WPC_ARGS_CONVERSATIONJOINEVENT_REASON,
    WPC_ARGS_CONVERSATIONJOINEVENT_MEMBERCOUNT,
    WPC_ARGS_CONVERSATIONJOINEVENT_MEMBER,
    WPC_ARGS_CONVERSATIONJOINEVENT_SENDER,
    WPC_ARGS_CONVERSATIONJOINEVENT_CARGS
} WPC_ARGS_CONVERSATIONJOINEVENT;

typedef enum tagWPC_ARGS_CONVERSATIONLEAVEEVENT
{
    WPC_ARGS_CONVERSATIONLEAVEEVENT_APPNAME = 0,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_APPVERSION,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_ACCOUNTNAME,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_CONVID,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_LEAVINGIP,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_LEAVINGUSER,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_REASON,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_MEMBERCOUNT,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_MEMBER,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_FLAGS,
    WPC_ARGS_CONVERSATIONLEAVEEVENT_CARGS
} WPC_ARGS_CONVERSATIONLEAVEEVENT;

typedef enum tagWPCFLAG_IM_FEATURE
{
    WPCFLAG_IM_FEATURE_NONE =     0x00,
    WPCFLAG_IM_FEATURE_VIDEO =    0x01,
    WPCFLAG_IM_FEATURE_AUDIO =    0x02,
    WPCFLAG_IM_FEATURE_GAME =     0x04,
    WPCFLAG_IM_FEATURE_SMS =      0x08,
    WPCFLAG_IM_FEATURE_FILESWAP = 0x10,
    WPCFLAG_IM_FEATURE_URLSWAP =  0x20,
    WPCFLAG_IM_FEATURE_SENDING = 0x80000000, // Top bit means sending or receiving.
    WPCFLAG_IM_FEATURE_ALL =      0xFFFFFFFF
} WPCFLAG_IM_FEATURE;

typedef enum tagWPC_ARGS_IMFEATUREEVENT
{
    WPC_ARGS_IMFEATUREEVENT_APPNAME = 0,
    WPC_ARGS_IMFEATUREEVENT_APPVERSION,
    WPC_ARGS_IMFEATUREEVENT_ACCOUNTNAME,
    WPC_ARGS_IMFEATUREEVENT_CONVID,
    WPC_ARGS_IMFEATUREEVENT_MEDIATYPE,
    WPC_ARGS_IMFEATUREEVENT_REASON,
    WPC_ARGS_IMFEATUREEVENT_RECIPCOUNT,
    WPC_ARGS_IMFEATUREEVENT_RECIPIENT,
    WPC_ARGS_IMFEATUREEVENT_SENDER,
    WPC_ARGS_IMFEATUREEVENT_SENDERIP,
    WPC_ARGS_IMFEATUREEVENT_DATA,
    WPC_ARGS_IMFEATUREEVENT_CARGS
} WPC_ARGS_IMFEATUREEVENT;

typedef enum tagWPC_ARGS_IMCONTACTEVENT
{
    WPC_ARGS_IMCONTACTEVENT_APPNAME = 0,
    WPC_ARGS_IMCONTACTEVENT_APPVERSION,
    WPC_ARGS_IMCONTACTEVENT_ACCOUNTNAME,
    WPC_ARGS_IMCONTACTEVENT_OLDNAME,
    WPC_ARGS_IMCONTACTEVENT_OLDID,
    WPC_ARGS_IMCONTACTEVENT_NEWNAME,
    WPC_ARGS_IMCONTACTEVENT_NEWID,
    WPC_ARGS_IMCONTACTEVENT_REASON,
    WPC_ARGS_IMCONTACTEVENT_CARGS
} WPC_ARGS_IMCONTACTEVENT;
//Games
typedef enum tagWPC_ARGS_GAMESTARTEVENT
{
    WPC_ARGS_GAMESTARTEVENT_APPID = 0,
    WPC_ARGS_GAMESTARTEVENT_INSTANCEID,
    WPC_ARGS_GAMESTARTEVENT_APPVERSION,
    WPC_ARGS_GAMESTARTEVENT_PATH,
    WPC_ARGS_GAMESTARTEVENT_RATING,
    WPC_ARGS_GAMESTARTEVENT_RATINGSYSTEM,
    WPC_ARGS_GAMESTARTEVENT_REASON,
    WPC_ARGS_GAMESTARTEVENT_DESCCOUNT,
    WPC_ARGS_GAMESTARTEVENT_DESCRIPTOR,
    WPC_ARGS_GAMESTARTEVENT_PID,
    WPC_ARGS_GAMESTARTEVENT_CARGS
} WPC_ARGS_GAMESTARTEVENT;

//Web
// Don't change anything in this--IE7 relies on it!!
typedef enum tagWPC_ARGS_FILEDOWNLOADEVENT
{
    WPC_ARGS_FILEDOWNLOADEVENT_URL = 0,
    WPC_ARGS_FILEDOWNLOADEVENT_APPNAME,
    WPC_ARGS_FILEDOWNLOADEVENT_VERSION,
    WPC_ARGS_FILEDOWNLOADEVENT_BLOCKED,
    WPC_ARGS_FILEDOWNLOADEVENT_PATH,
    WPC_ARGS_FILEDOWNLOADEVENT_CARGS
} WPC_ARGS_FILEDOWNLOADEVENT;

typedef enum tagWPC_ARGS_URLVISITEVENT
{
    WPC_ARGS_URLVISITEVENT_URL = 0,
    WPC_ARGS_URLVISITEVENT_APPNAME,
    WPC_ARGS_URLVISITEVENT_VERSION,
    WPC_ARGS_URLVISITEVENT_REASON,
    WPC_ARGS_URLVISITEVENT_RATINGSYSTEMID,
    WPC_ARGS_URLVISITEVENT_CATCOUNT,
    WPC_ARGS_URLVISITEVENT_CATEGORY,
    WPC_ARGS_URLVISITEVENT_CARGS
} WPC_ARGS_URLVISITEVENT;

typedef enum tagWPC_ARGS_CUSTOMEVENT
{
    WPC_ARGS_CUSTOMEVENT_PUBLISHER = 0,
    WPC_ARGS_CUSTOMEVENT_APPNAME,
    WPC_ARGS_CUSTOMEVENT_APPVERSION,
    WPC_ARGS_CUSTOMEVENT_EVENT,
    WPC_ARGS_CUSTOMEVENT_VALUE1,
    WPC_ARGS_CUSTOMEVENT_VALUE2,
    WPC_ARGS_CUSTOMEVENT_VALUE3,
    WPC_ARGS_CUSTOMEVENT_BLOCKED,
    WPC_ARGS_CUSTOMEVENT_REASON,
    WPC_ARGS_CUSTOMEVENT_CARGS
} WPC_ARGS_CUSTOMEVENT;

typedef enum tagWPC_ARGS_WEBOVERRIDEEVENT
{
    WPC_ARGS_WEBOVERRIDEEVENT_USERID = 0,
    WPC_ARGS_WEBOVERRIDEEVENT_URL,
    WPC_ARGS_WEBOVERRIDEEVENT_REASON,
    WPC_ARGS_WEBOVERRIDEEVENT_CARGS
} WPC_ARGS_WEBOVERRIDEEVENT;

typedef enum tagWPC_ARGS_APPOVERRIDEEVENT
{
    WPC_ARGS_APPOVERRIDEEVENT_USERID = 0,
    WPC_ARGS_APPOVERRIDEEVENT_PATH,
    WPC_ARGS_APPOVERRIDEEVENT_REASON,
    WPC_ARGS_APPOVERRIDEEVENT_CARGS
} WPC_ARGS_APPOVERRIDEEVENT;

//
// This is the list of settings that are written out to the log file as the setting id.
//
typedef enum tagWPC_SETTINGS
{
    WPC_SETTINGS_WPC_EXTENSION_PATH = 0,
    WPC_SETTINGS_WPC_EXTENSION_SILO,
    WPC_SETTINGS_WPC_EXTENSION_IMAGE_PATH,
    WPC_SETTINGS_WPC_EXTENSION_DISABLEDIMAGE_PATH,
    WPC_SETTINGS_WPC_EXTENSION_NAME,
    WPC_SETTINGS_WPC_EXTENSION_SUB_TITLE,
    WPC_SETTINGS_SYSTEM_CURRENT_RATING_SYSTEM,
    WPC_SETTINGS_SYSTEM_LAST_LOG_VIEW,
    WPC_SETTINGS_SYSTEM_LOG_VIEW_REMINDER_INTERVAL,
    WPC_SETTINGS_SYSTEM_HTTP_EXEMPTION_LIST,
    WPC_SETTINGS_SYSTEM_URL_EXEMPTION_LIST,
    WPC_SETTINGS_SYSTEM_FILTER_ID,
    WPC_SETTINGS_SYSTEM_FILTER_NAME, 
    WPC_SETTINGS_SYSTEM_LOCALE,
    WPC_SETTINGS_ALLOW_BLOCK,
    WPC_SETTINGS_GAME_BLOCKED,
    WPC_SETTINGS_GAME_ALLOW_UNRATED,
    WPC_SETTINGS_GAME_MAX_ALLOWED,
    WPC_SETTINGS_GAME_DENIED_DESCRIPTORS,
    WPC_SETTINGS_USER_WPC_ENABLED,
    WPC_SETTINGS_USER_LOGGING_REQUIRED,
    WPC_SETTINGS_USER_HOURLY_RESTRICTIONS,
    WPC_SETTINGS_USER_OVERRRIDE_REQUESTS,
    WPC_SETTINGS_USER_LOGON_HOURS,
    WPC_SETTINGS_USER_APP_RESTRICTIONS,
    WPC_SETTINGS_WEB_FILTER_ON,
    WPC_SETTINGS_WEB_DOWNLOAD_BLOCKED,
    WPC_SETTINGS_WEB_FILTER_LEVEL,
    WPC_SETTINGS_WEB_BLOCKED_CATEGORY_LIST,
    WPC_SETTINGS_WEB_BLOCK_UNRATED,
    WPC_SETTINGS_WPC_ENABLED,
    WPC_SETTINGS_WPC_LOGGING_REQUIRED,
    WPC_SETTINGS_RATING_SYSTEM_PATH,
    WPC_SETTINGS_WPC_PROVIDER_CURRENT,
    WPC_SETTING_COUNT
} WPC_SETTINGS;
//**********************************************************************`
//* This is an include file generated by Message Compiler.             *`
//*                                                                    *`
//* Copyright (c) Microsoft Corporation. All Rights Reserved.          *`
//**********************************************************************`
#pragma once
//+
// Provider Microsoft-Windows-ParentalControls Event Count 17
//+
EXTERN_C __declspec(selectany) const GUID WPCPROV = {0x01090065, 0xb467, 0x4503, {0x9b, 0x28, 0x53, 0x37, 0x66, 0x76, 0x10, 0x87}};

//
// Channel
//
#define WPCCHANNEL 0x10

//
// Opcodes
//
#define WPC_SETTINGS_LOCATE 0x14
#define WPC_SETTINGS_MODIFY 0x15
#define WPC_APP_LAUNCH 0x16
#define WPC_SYSTEM 0x17
#define WPC_WEB 0x18

//
// Tasks
//
#define WPCPROV_TASK_SettingChange 0x1
#define WPCPROV_TASK_GameStart 0x2
#define WPCPROV_TASK_UrlVisit 0x3
#define WPCPROV_TASK_EmailReceived 0x4
#define WPCPROV_TASK_EmailSent 0x5
#define WPCPROV_TASK_MediaPlayback 0x6
#define WPCPROV_TASK_IMInvitation 0x7
#define WPCPROV_TASK_IMJoin 0x8
#define WPCPROV_TASK_IMLeave 0x9
#define WPCPROV_TASK_FileDownload 0xa
#define WPCPROV_TASK_IMFeature 0xb
#define WPCPROV_TASK_Custom 0xd
#define WPCPROV_TASK_EmailContact 0xe
#define WPCPROV_TASK_IMContact 0xf
#define WPCPROV_TASK_AppBlocked 0x10
#define WPCPROV_TASK_AppOverride 0x11
#define WPCPROV_TASK_WebOverride 0x12
//
// Keyword
//
#define WPCPROV_KEYWORD_WPC 0x10
#define WPCPROV_KEYWORD_ThirdParty 0x20

//
// Event Descriptors
//
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_SYS_SETTINGCHANGE = {0x1, 0x0, 0x10, 0x4, 0x15, 0x1, 0x8000000000000010};
#define WPCEVENT_SYS_SETTINGCHANGE_value 0x1
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_GAME_START = {0x2, 0x0, 0x10, 0x4, 0x16, 0x2, 0x8000000000000030};
#define WPCEVENT_GAME_START_value 0x2
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_WEB_URLVISIT = {0x3, 0x0, 0x10, 0x4, 0x18, 0x3, 0x8000000000000010};
#define WPCEVENT_WEB_URLVISIT_value 0x3
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_EMAIL_RECEIVED = {0x4, 0x0, 0x10, 0x4, 0x16, 0x4, 0x8000000000000030};
#define WPCEVENT_EMAIL_RECEIVED_value 0x4
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_EMAIL_SENT = {0x5, 0x0, 0x10, 0x4, 0x16, 0x5, 0x8000000000000030};
#define WPCEVENT_EMAIL_SENT_value 0x5
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_MEDIA_PLAYBACK = {0x6, 0x0, 0x10, 0x4, 0x16, 0x6, 0x8000000000000030};
#define WPCEVENT_MEDIA_PLAYBACK_value 0x6
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_IM_INVITATION = {0x7, 0x0, 0x10, 0x4, 0x16, 0x7, 0x8000000000000030};
#define WPCEVENT_IM_INVITATION_value 0x7
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_IM_JOIN = {0x8, 0x0, 0x10, 0x4, 0x16, 0x8, 0x8000000000000030};
#define WPCEVENT_IM_JOIN_value 0x8
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_IM_LEAVE = {0x9, 0x0, 0x10, 0x4, 0x16, 0x9, 0x8000000000000030};
#define WPCEVENT_IM_LEAVE_value 0x9
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_WEB_FILEDOWNLOAD = {0xa, 0x0, 0x10, 0x4, 0x18, 0xa, 0x8000000000000030};
#define WPCEVENT_WEB_FILEDOWNLOAD_value 0xa
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_IM_FEATURE = {0xb, 0x0, 0x10, 0x4, 0x16, 0xb, 0x8000000000000030};
#define WPCEVENT_IM_FEATURE_value 0xb
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_CUSTOM = {0xd, 0x0, 0x10, 0x4, 0x17, 0xd, 0x8000000000000030};
#define WPCEVENT_CUSTOM_value 0xd
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_EMAIL_CONTACT = {0xe, 0x0, 0x10, 0x4, 0x16, 0xe, 0x8000000000000030};
#define WPCEVENT_EMAIL_CONTACT_value 0xe
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_IM_CONTACT = {0xf, 0x0, 0x10, 0x4, 0x16, 0xf, 0x8000000000000030};
#define WPCEVENT_IM_CONTACT_value 0xf
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_SYSTEM_APPBLOCKED = {0x10, 0x0, 0x10, 0x4, 0x16, 0x10, 0x8000000000000010};
#define WPCEVENT_SYSTEM_APPBLOCKED_value 0x10
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_APPOVERRIDE = {0x11, 0x0, 0x10, 0x4, 0x16, 0x11, 0x8000000000000010};
#define WPCEVENT_APPOVERRIDE_value 0x11
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WPCEVENT_WEBOVERRIDE = {0x12, 0x0, 0x10, 0x4, 0x16, 0x12, 0x8000000000000010};
#define WPCEVENT_WEBOVERRIDE_value 0x12
#define MSG_Keyword_WPC                      0x10000005L
#define MSG_Keyword_ThirdParty               0x10000006L
#define MSG_Opcode_Locate                    0x30000014L
#define MSG_Opcode_Modify                    0x30000015L
#define MSG_Opcode_Launch                    0x30000016L
#define MSG_Opcode_System                    0x30000017L
#define MSG_Opcode_Web                       0x30000018L
#define MSG_Task_SettingChange               0x70000001L
#define MSG_Task_GameStart                   0x70000002L
#define MSG_Task_UrlVisit                    0x70000003L
#define MSG_Task_EmailReceived               0x70000004L
#define MSG_Task_EmailSent                   0x70000005L
#define MSG_Task_MediaPlayback               0x70000006L
#define MSG_Task_IMInvitation                0x70000007L
#define MSG_Task_IMJoin                      0x70000008L
#define MSG_Task_IMLeave                     0x70000009L
#define MSG_Task_FileDownload                0x7000000AL
#define MSG_Task_IMFeature                   0x7000000BL
#define MSG_Task_Custom                      0x7000000DL
#define MSG_Task_EmailContact                0x7000000EL
#define MSG_Task_IMContact                   0x7000000FL
#define MSG_Task_AppBlocked                  0x70000010L
#define MSG_Task_AppOverride                 0x70000011L
#define MSG_Task_WebOverride                 0x70000012L
#define MSG_Publisher_Name                   0x90000001L
#define MSG_Event_SettingChange              0xB0000001L
#define MSG_Event_GameStart                  0xB0000002L
#define MSG_Event_UrlVisit                   0xB0000003L
#define MSG_Event_EmailReceived              0xB0000004L
#define MSG_Event_EmailSent                  0xB0000005L
#define MSG_Event_MediaPlayback              0xB0000006L
#define MSG_Event_IMInvitation               0xB0000007L
#define MSG_Event_IMJoin                     0xB0000008L
#define MSG_Event_IMLeave                    0xB0000009L
#define MSG_Event_FileDownload               0xB000000AL
#define MSG_Event_IMFeature                  0xB000000BL
#define MSG_Event_Custom                     0xB000000DL
#define MSG_Event_EmailContact               0xB000000EL
#define MSG_Event_IMContact                  0xB000000FL
#define MSG_Event_AppBlocked                 0xB0000010L
#define MSG_Event_AppOverride                0xB0000011L
#define MSG_Event_WebOverride                0xB0000012L

