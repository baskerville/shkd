#include <linux/input.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "keycodes.h"
#include "helpers.h"
#include "shkd.h"

reg_key_t reg_dict[] = {
    {"esc"             , KEY_ESC}             ,
    {"1"               , KEY_1}               ,
    {"2"               , KEY_2}               ,
    {"3"               , KEY_3}               ,
    {"4"               , KEY_4}               ,
    {"5"               , KEY_5}               ,
    {"6"               , KEY_6}               ,
    {"7"               , KEY_7}               ,
    {"8"               , KEY_8}               ,
    {"9"               , KEY_9}               ,
    {"0"               , KEY_0}               ,
    {"minus"           , KEY_MINUS}           ,
    {"equal"           , KEY_EQUAL}           ,
    {"backspace"       , KEY_BACKSPACE}       ,
    {"tab"             , KEY_TAB}             ,
    {"q"               , KEY_Q}               ,
    {"w"               , KEY_W}               ,
    {"e"               , KEY_E}               ,
    {"r"               , KEY_R}               ,
    {"t"               , KEY_T}               ,
    {"y"               , KEY_Y}               ,
    {"u"               , KEY_U}               ,
    {"i"               , KEY_I}               ,
    {"o"               , KEY_O}               ,
    {"p"               , KEY_P}               ,
    {"leftbracket"     , KEY_LEFTBRACE}       ,
    {"rightbracket"    , KEY_RIGHTBRACE}      ,
    {"enter"           , KEY_ENTER}           ,
    {"a"               , KEY_A}               ,
    {"s"               , KEY_S}               ,
    {"d"               , KEY_D}               ,
    {"f"               , KEY_F}               ,
    {"g"               , KEY_G}               ,
    {"h"               , KEY_H}               ,
    {"j"               , KEY_J}               ,
    {"k"               , KEY_K}               ,
    {"l"               , KEY_L}               ,
    {"semicolon"       , KEY_SEMICOLON}       ,
    {"apostrophe"      , KEY_APOSTROPHE}      ,
    {"grave"           , KEY_GRAVE}           ,
    {"backslash"       , KEY_BACKSLASH}       ,
    {"z"               , KEY_Z}               ,
    {"x"               , KEY_X}               ,
    {"c"               , KEY_C}               ,
    {"v"               , KEY_V}               ,
    {"b"               , KEY_B}               ,
    {"n"               , KEY_N}               ,
    {"m"               , KEY_M}               ,
    {"comma"           , KEY_COMMA}           ,
    {"dot"             , KEY_DOT}             ,
    {"slash"           , KEY_SLASH}           ,
    {"kpasterisk"      , KEY_KPASTERISK}      ,
    {"space"           , KEY_SPACE}           ,
    {"capslock"        , KEY_CAPSLOCK}        ,
    {"f1"              , KEY_F1}              ,
    {"f2"              , KEY_F2}              ,
    {"f3"              , KEY_F3}              ,
    {"f4"              , KEY_F4}              ,
    {"f5"              , KEY_F5}              ,
    {"f6"              , KEY_F6}              ,
    {"f7"              , KEY_F7}              ,
    {"f8"              , KEY_F8}              ,
    {"f9"              , KEY_F9}              ,
    {"f10"             , KEY_F10}             ,
    {"numlock"         , KEY_NUMLOCK}         ,
    {"scrolllock"      , KEY_SCROLLLOCK}      ,
    {"kp7"             , KEY_KP7}             ,
    {"kp8"             , KEY_KP8}             ,
    {"kp9"             , KEY_KP9}             ,
    {"kpminus"         , KEY_KPMINUS}         ,
    {"kp4"             , KEY_KP4}             ,
    {"kp5"             , KEY_KP5}             ,
    {"kp6"             , KEY_KP6}             ,
    {"kpplus"          , KEY_KPPLUS}          ,
    {"kp1"             , KEY_KP1}             ,
    {"kp2"             , KEY_KP2}             ,
    {"kp3"             , KEY_KP3}             ,
    {"kp0"             , KEY_KP0}             ,
    {"kpdot"           , KEY_KPDOT}           ,
    {"zenkakuhankaku"  , KEY_ZENKAKUHANKAKU}  ,
    {"102nd"           , KEY_102ND}           ,
    {"f11"             , KEY_F11}             ,
    {"f12"             , KEY_F12}             ,
    {"ro"              , KEY_RO}              ,
    {"katakana"        , KEY_KATAKANA}        ,
    {"hiragana"        , KEY_HIRAGANA}        ,
    {"henkan"          , KEY_HENKAN}          ,
    {"katakanahiragana", KEY_KATAKANAHIRAGANA},
    {"muhenkan"        , KEY_MUHENKAN}        ,
    {"kpjpcomma"       , KEY_KPJPCOMMA}       ,
    {"kpenter"         , KEY_KPENTER}         ,
    {"kpslash"         , KEY_KPSLASH}         ,
    {"sysrq"           , KEY_SYSRQ}           ,
    {"linefeed"        , KEY_LINEFEED}        ,
    {"home"            , KEY_HOME}            ,
    {"up"              , KEY_UP}              ,
    {"pageup"          , KEY_PAGEUP}          ,
    {"left"            , KEY_LEFT}            ,
    {"right"           , KEY_RIGHT}           ,
    {"end"             , KEY_END}             ,
    {"down"            , KEY_DOWN}            ,
    {"pagedown"        , KEY_PAGEDOWN}        ,
    {"insert"          , KEY_INSERT}          ,
    {"delete"          , KEY_DELETE}          ,
    {"macro"           , KEY_MACRO}           ,
    {"mute"            , KEY_MUTE}            ,
    {"volumedown"      , KEY_VOLUMEDOWN}      ,
    {"volumeup"        , KEY_VOLUMEUP}        ,
    {"power"           , KEY_POWER}           ,
    {"kpequal"         , KEY_KPEQUAL}         ,
    {"kpplusminus"     , KEY_KPPLUSMINUS}     ,
    {"pause"           , KEY_PAUSE}           ,
    {"scale"           , KEY_SCALE}           ,
    {"kpcomma"         , KEY_KPCOMMA}         ,
    {"hangeul"         , KEY_HANGEUL}         ,
    {"hanguel"         , KEY_HANGUEL}         ,
    {"hanja"           , KEY_HANJA}           ,
    {"yen"             , KEY_YEN}             ,
    {"compose"         , KEY_COMPOSE}         ,
    {"stop"            , KEY_STOP}            ,
    {"again"           , KEY_AGAIN}           ,
    {"props"           , KEY_PROPS}           ,
    {"undo"            , KEY_UNDO}            ,
    {"front"           , KEY_FRONT}           ,
    {"copy"            , KEY_COPY}            ,
    {"open"            , KEY_OPEN}            ,
    {"paste"           , KEY_PASTE}           ,
    {"find"            , KEY_FIND}            ,
    {"cut"             , KEY_CUT}             ,
    {"help"            , KEY_HELP}            ,
    {"menu"            , KEY_MENU}            ,
    {"calc"            , KEY_CALC}            ,
    {"setup"           , KEY_SETUP}           ,
    {"sleep"           , KEY_SLEEP}           ,
    {"wakeup"          , KEY_WAKEUP}          ,
    {"file"            , KEY_FILE}            ,
    {"sendfile"        , KEY_SENDFILE}        ,
    {"deletefile"      , KEY_DELETEFILE}      ,
    {"xfer"            , KEY_XFER}            ,
    {"prog1"           , KEY_PROG1}           ,
    {"prog2"           , KEY_PROG2}           ,
    {"www"             , KEY_WWW}             ,
    {"msdos"           , KEY_MSDOS}           ,
    {"coffee"          , KEY_COFFEE}          ,
    {"screenlock"      , KEY_SCREENLOCK}      ,
    {"direction"       , KEY_DIRECTION}       ,
    {"cyclewindows"    , KEY_CYCLEWINDOWS}    ,
    {"mail"            , KEY_MAIL}            ,
    {"bookmarks"       , KEY_BOOKMARKS}       ,
    {"computer"        , KEY_COMPUTER}        ,
    {"back"            , KEY_BACK}            ,
    {"forward"         , KEY_FORWARD}         ,
    {"closecd"         , KEY_CLOSECD}         ,
    {"ejectcd"         , KEY_EJECTCD}         ,
    {"ejectclosecd"    , KEY_EJECTCLOSECD}    ,
    {"nextsong"        , KEY_NEXTSONG}        ,
    {"playpause"       , KEY_PLAYPAUSE}       ,
    {"previoussong"    , KEY_PREVIOUSSONG}    ,
    {"stopcd"          , KEY_STOPCD}          ,
    {"record"          , KEY_RECORD}          ,
    {"rewind"          , KEY_REWIND}          ,
    {"phone"           , KEY_PHONE}           ,
    {"iso"             , KEY_ISO}             ,
    {"config"          , KEY_CONFIG}          ,
    {"homepage"        , KEY_HOMEPAGE}        ,
    {"refresh"         , KEY_REFRESH}         ,
    {"exit"            , KEY_EXIT}            ,
    {"move"            , KEY_MOVE}            ,
    {"edit"            , KEY_EDIT}            ,
    {"scrollup"        , KEY_SCROLLUP}        ,
    {"scrolldown"      , KEY_SCROLLDOWN}      ,
    {"kpleftparen"     , KEY_KPLEFTPAREN}     ,
    {"kprightparen"    , KEY_KPRIGHTPAREN}    ,
    {"new"             , KEY_NEW}             ,
    {"redo"            , KEY_REDO}            ,
    {"f13"             , KEY_F13}             ,
    {"f14"             , KEY_F14}             ,
    {"f15"             , KEY_F15}             ,
    {"f16"             , KEY_F16}             ,
    {"f17"             , KEY_F17}             ,
    {"f18"             , KEY_F18}             ,
    {"f19"             , KEY_F19}             ,
    {"f20"             , KEY_F20}             ,
    {"f21"             , KEY_F21}             ,
    {"f22"             , KEY_F22}             ,
    {"f23"             , KEY_F23}             ,
    {"f24"             , KEY_F24}             ,
    {"playcd"          , KEY_PLAYCD}          ,
    {"pausecd"         , KEY_PAUSECD}         ,
    {"prog3"           , KEY_PROG3}           ,
    {"prog4"           , KEY_PROG4}           ,
    {"dashboard"       , KEY_DASHBOARD}       ,
    {"suspend"         , KEY_SUSPEND}         ,
    {"close"           , KEY_CLOSE}           ,
    {"play"            , KEY_PLAY}            ,
    {"fastforward"     , KEY_FASTFORWARD}     ,
    {"bassboost"       , KEY_BASSBOOST}       ,
    {"print"           , KEY_PRINT}           ,
    {"hp"              , KEY_HP}              ,
    {"camera"          , KEY_CAMERA}          ,
    {"sound"           , KEY_SOUND}           ,
    {"question"        , KEY_QUESTION}        ,
    {"email"           , KEY_EMAIL}           ,
    {"chat"            , KEY_CHAT}            ,
    {"search"          , KEY_SEARCH}          ,
    {"connect"         , KEY_CONNECT}         ,
    {"finance"         , KEY_FINANCE}         ,
    {"sport"           , KEY_SPORT}           ,
    {"shop"            , KEY_SHOP}            ,
    {"alterase"        , KEY_ALTERASE}        ,
    {"cancel"          , KEY_CANCEL}          ,
    {"brightnessdown"  , KEY_BRIGHTNESSDOWN}  ,
    {"brightnessup"    , KEY_BRIGHTNESSUP}    ,
    {"media"           , KEY_MEDIA}           ,
    {"switchvideomode" , KEY_SWITCHVIDEOMODE} ,
    {"kbdillumtoggle"  , KEY_KBDILLUMTOGGLE}  ,
    {"kbdillumdown"    , KEY_KBDILLUMDOWN}    ,
    {"kbdillumup"      , KEY_KBDILLUMUP}      ,
    {"send"            , KEY_SEND}            ,
    {"reply"           , KEY_REPLY}           ,
    {"forwardmail"     , KEY_FORWARDMAIL}     ,
    {"save"            , KEY_SAVE}            ,
    {"documents"       , KEY_DOCUMENTS}       ,
    {"battery"         , KEY_BATTERY}         ,
    {"bluetooth"       , KEY_BLUETOOTH}       ,
    {"wlan"            , KEY_WLAN}            ,
    {"uwb"             , KEY_UWB}             ,
    {"unknown"         , KEY_UNKNOWN}         ,
    {"video_next"      , KEY_VIDEO_NEXT}      ,
    {"video_prev"      , KEY_VIDEO_PREV}      ,
    {"brightness_cycle", KEY_BRIGHTNESS_CYCLE},
    {"brightness_zero" , KEY_BRIGHTNESS_ZERO} ,
    {"display_off"     , KEY_DISPLAY_OFF}     ,
    {"wimax"           , KEY_WIMAX}           ,
    {"rfkill"          , KEY_RFKILL}          ,
    {"micmute"         , KEY_MICMUTE}         ,
    {"ok"              , KEY_OK}              ,
    {"select"          , KEY_SELECT}          ,
    {"goto"            , KEY_GOTO}            ,
    {"clear"           , KEY_CLEAR}           ,
    {"power2"          , KEY_POWER2}          ,
    {"option"          , KEY_OPTION}          ,
    {"info"            , KEY_INFO}            ,
    {"time"            , KEY_TIME}            ,
    {"vendor"          , KEY_VENDOR}          ,
    {"archive"         , KEY_ARCHIVE}         ,
    {"program"         , KEY_PROGRAM}         ,
    {"channel"         , KEY_CHANNEL}         ,
    {"favorites"       , KEY_FAVORITES}       ,
    {"epg"             , KEY_EPG}             ,
    {"pvr"             , KEY_PVR}             ,
    {"mhp"             , KEY_MHP}             ,
    {"language"        , KEY_LANGUAGE}        ,
    {"title"           , KEY_TITLE}           ,
    {"subtitle"        , KEY_SUBTITLE}        ,
    {"angle"           , KEY_ANGLE}           ,
    {"zoom"            , KEY_ZOOM}            ,
    {"mode"            , KEY_MODE}            ,
    {"keyboard"        , KEY_KEYBOARD}        ,
    {"screen"          , KEY_SCREEN}          ,
    {"pc"              , KEY_PC}              ,
    {"tv"              , KEY_TV}              ,
    {"tv2"             , KEY_TV2}             ,
    {"vcr"             , KEY_VCR}             ,
    {"vcr2"            , KEY_VCR2}            ,
    {"sat"             , KEY_SAT}             ,
    {"sat2"            , KEY_SAT2}            ,
    {"cd"              , KEY_CD}              ,
    {"tape"            , KEY_TAPE}            ,
    {"radio"           , KEY_RADIO}           ,
    {"tuner"           , KEY_TUNER}           ,
    {"player"          , KEY_PLAYER}          ,
    {"text"            , KEY_TEXT}            ,
    {"dvd"             , KEY_DVD}             ,
    {"aux"             , KEY_AUX}             ,
    {"mp3"             , KEY_MP3}             ,
    {"audio"           , KEY_AUDIO}           ,
    {"video"           , KEY_VIDEO}           ,
    {"directory"       , KEY_DIRECTORY}       ,
    {"list"            , KEY_LIST}            ,
    {"memo"            , KEY_MEMO}            ,
    {"calendar"        , KEY_CALENDAR}        ,
    {"red"             , KEY_RED}             ,
    {"green"           , KEY_GREEN}           ,
    {"yellow"          , KEY_YELLOW}          ,
    {"blue"            , KEY_BLUE}            ,
    {"channelup"       , KEY_CHANNELUP}       ,
    {"channeldown"     , KEY_CHANNELDOWN}     ,
    {"first"           , KEY_FIRST}           ,
    {"last"            , KEY_LAST}            ,
    {"ab"              , KEY_AB}              ,
    {"next"            , KEY_NEXT}            ,
    {"restart"         , KEY_RESTART}         ,
    {"slow"            , KEY_SLOW}            ,
    {"shuffle"         , KEY_SHUFFLE}         ,
    {"break"           , KEY_BREAK}           ,
    {"previous"        , KEY_PREVIOUS}        ,
    {"digits"          , KEY_DIGITS}          ,
    {"teen"            , KEY_TEEN}            ,
    {"twen"            , KEY_TWEN}            ,
    {"videophone"      , KEY_VIDEOPHONE}      ,
    {"games"           , KEY_GAMES}           ,
    {"zoomin"          , KEY_ZOOMIN}          ,
    {"zoomout"         , KEY_ZOOMOUT}         ,
    {"zoomreset"       , KEY_ZOOMRESET}       ,
    {"wordprocessor"   , KEY_WORDPROCESSOR}   ,
    {"editor"          , KEY_EDITOR}          ,
    {"spreadsheet"     , KEY_SPREADSHEET}     ,
    {"graphicseditor"  , KEY_GRAPHICSEDITOR}  ,
    {"presentation"    , KEY_PRESENTATION}    ,
    {"database"        , KEY_DATABASE}        ,
    {"news"            , KEY_NEWS}            ,
    {"voicemail"       , KEY_VOICEMAIL}       ,
    {"addressbook"     , KEY_ADDRESSBOOK}     ,
    {"messenger"       , KEY_MESSENGER}       ,
    {"displaytoggle"   , KEY_DISPLAYTOGGLE}   ,
    {"spellcheck"      , KEY_SPELLCHECK}      ,
    {"logoff"          , KEY_LOGOFF}          ,
    {"dollar"          , KEY_DOLLAR}          ,
    {"euro"            , KEY_EURO}            ,
    {"frameback"       , KEY_FRAMEBACK}       ,
    {"frameforward"    , KEY_FRAMEFORWARD}    ,
    {"context_menu"    , KEY_CONTEXT_MENU}    ,
    {"media_repeat"    , KEY_MEDIA_REPEAT}    ,
    {"10channelsup"    , KEY_10CHANNELSUP}    ,
    {"10channelsdown"  , KEY_10CHANNELSDOWN}  ,
    {"images"          , KEY_IMAGES}          ,
    {"del_eol"         , KEY_DEL_EOL}         ,
    {"del_eos"         , KEY_DEL_EOS}         ,
    {"ins_line"        , KEY_INS_LINE}        ,
    {"del_line"        , KEY_DEL_LINE}        ,
    {"fn_esc"          , KEY_FN_ESC}          ,
    {"fn_f1"           , KEY_FN_F1}           ,
    {"fn_f2"           , KEY_FN_F2}           ,
    {"fn_f3"           , KEY_FN_F3}           ,
    {"fn_f4"           , KEY_FN_F4}           ,
    {"fn_f5"           , KEY_FN_F5}           ,
    {"fn_f6"           , KEY_FN_F6}           ,
    {"fn_f7"           , KEY_FN_F7}           ,
    {"fn_f8"           , KEY_FN_F8}           ,
    {"fn_f9"           , KEY_FN_F9}           ,
    {"fn_f10"          , KEY_FN_F10}          ,
    {"fn_f11"          , KEY_FN_F11}          ,
    {"fn_f12"          , KEY_FN_F12}          ,
    {"fn_1"            , KEY_FN_1}            ,
    {"fn_2"            , KEY_FN_2}            ,
    {"fn_d"            , KEY_FN_D}            ,
    {"fn_e"            , KEY_FN_E}            ,
    {"fn_f"            , KEY_FN_F}            ,
    {"fn_s"            , KEY_FN_S}            ,
    {"fn_b"            , KEY_FN_B}            ,
    {"brl_dot1"        , KEY_BRL_DOT1}        ,
    {"brl_dot2"        , KEY_BRL_DOT2}        ,
    {"brl_dot3"        , KEY_BRL_DOT3}        ,
    {"brl_dot4"        , KEY_BRL_DOT4}        ,
    {"brl_dot5"        , KEY_BRL_DOT5}        ,
    {"brl_dot6"        , KEY_BRL_DOT6}        ,
    {"brl_dot7"        , KEY_BRL_DOT7}        ,
    {"brl_dot8"        , KEY_BRL_DOT8}        ,
    {"brl_dot9"        , KEY_BRL_DOT9}        ,
    {"brl_dot10"       , KEY_BRL_DOT10}       ,
    {"numeric_0"       , KEY_NUMERIC_0}       ,
    {"numeric_1"       , KEY_NUMERIC_1}       ,
    {"numeric_2"       , KEY_NUMERIC_2}       ,
    {"numeric_3"       , KEY_NUMERIC_3}       ,
    {"numeric_4"       , KEY_NUMERIC_4}       ,
    {"numeric_5"       , KEY_NUMERIC_5}       ,
    {"numeric_6"       , KEY_NUMERIC_6}       ,
    {"numeric_7"       , KEY_NUMERIC_7}       ,
    {"numeric_8"       , KEY_NUMERIC_8}       ,
    {"numeric_9"       , KEY_NUMERIC_9}       ,
    {"numeric_star"    , KEY_NUMERIC_STAR}    ,
    {"numeric_pound"   , KEY_NUMERIC_POUND}   ,
    {"camera_focus"    , KEY_CAMERA_FOCUS}    ,
    {"wps_button"      , KEY_WPS_BUTTON}      ,
    {"touchpad_toggle" , KEY_TOUCHPAD_TOGGLE} ,
    {"touchpad_on"     , KEY_TOUCHPAD_ON}     ,
    {"touchpad_off"    , KEY_TOUCHPAD_OFF}    ,
    {"camera_zoomin"   , KEY_CAMERA_ZOOMIN}   ,
    {"camera_zoomout"  , KEY_CAMERA_ZOOMOUT}  ,
    {"camera_up"       , KEY_CAMERA_UP}       ,
    {"camera_down"     , KEY_CAMERA_DOWN}     ,
    {"camera_left"     , KEY_CAMERA_LEFT}     ,
    {"camera_right"    , KEY_CAMERA_RIGHT}    ,
    {"min_interesting" , KEY_MIN_INTERESTING} ,
    {"max"             , KEY_MAX}
};

mod_key_t mod_dict[] = {
    {"shift"  , KEY_LEFTSHIFT , 1<<1},
    {"shift"  , KEY_RIGHTSHIFT, 1<<1},
    {"ctrl"   , KEY_LEFTCTRL  , 1<<2},
    {"ctrl"   , KEY_RIGHTCTRL , 1<<2},
    {"alt"    , KEY_LEFTALT   , 1<<3},
    {"alt"    , KEY_RIGHTALT  , 1<<3},
    {"meta"   , KEY_LEFTMETA  , 1<<4},
    {"meta"   , KEY_RIGHTMETA , 1<<4},
    {"fn"     , KEY_FN        , 1<<5},
    {"release", 0             , RELEASE_MASK}
};

bool reg_key_from_name(const char *n, reg_key_t *k)
{
    /* parse =xxx as an integer keycode */
    if (n[0] == '=') {
        int code = 0;
        if (sscanf(&n[1], "%d", &code) == 1) {
            *k = (reg_key_t){"", code};
            return true;
        } else {
            return false;
        }
    }

    /* search for the key name */
    for (unsigned int i = 0; i < LENGTH(reg_dict); i++)
        if (strcmp(reg_dict[i].name, n) == 0) {
            *k = reg_dict[i];
            return true;
        }

    return false;
}

bool reg_key_from_keycode(unsigned short c, reg_key_t *k)
{
    for (unsigned int i = 0; i < LENGTH(reg_dict); i++)
        if (reg_dict[i].keycode == c) {
            *k = reg_dict[i];
            return true;
        }

    return false;
}

bool mod_key_from_name(const char *n, mod_key_t *k, bool *cf)
{
    unsigned int m = 0;

    for (unsigned int i = 0; i < LENGTH(mod_dict); i++)
        if (strcmp(mod_dict[i].name, n) == 0) {
            *k = mod_dict[i];
            cf[mod_dict[i].keycode] = true;
            m++;
        }

    if (m > 0)
        return true;
    else
        return false;
}

bool mod_key_from_keycode(unsigned short c, mod_key_t *k)
{
    for (unsigned int i = 0; i < LENGTH(mod_dict); i++)
        if (mod_dict[i].keycode == c) {
            *k = mod_dict[i];
            return true;
        }

    return false;
}

hotkey_t *find_hotkey(unsigned char modmask, unsigned short keycode, const char* mode)
{
    for (hotkey_t *hk = hotkeys; hk != NULL; hk = hk->prev)
        if (hk->modmask == modmask && hk->keycode == keycode &&
            (!hk->mode || /* key is global, or else mode is set and matches */
            (mode && !strcmp(hk->mode, mode)))) {
            return hk;
        }
    return NULL;
}

hotkey_t *make_hotkey(unsigned char modmask, unsigned short keycode,
    const char* mode, const char* next_mode, const char *command)
{
    hotkey_t *hk = malloc(sizeof(hotkey_t));
    hk->prev = NULL;
    hk->modmask = modmask;
    hk->keycode = keycode;

    hk->mode = mode ? strdup(mode) : NULL;
    hk->next_mode = next_mode ? strdup(next_mode) : NULL;

    strncpy(hk->command, command, sizeof(hk->command));
    return hk;
}
