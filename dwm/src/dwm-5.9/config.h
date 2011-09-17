/* See LICENSE file for copyright and license details. */

#include "gaplessgrid.h"
#include "movestack.h"
#include "bstack.h"
#include "push.h"
#include "cycle.h"

/* appearance */
static const char font[]            = "-*-fixed-*-*-*-*-12-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#262626";
static const char normbgcolor[]     = "#262626";
static const char normfgcolor[]     = "#ffffff";
static const char selbordercolor[]  = "#ebac54";
static const char selbgcolor[]      = "#ebac54";
static const char selfgcolor[]      = "#ffffff";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.6; /* factor of master area size [0.05..0.95] */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[TILE]",     tile },    /* first entry is default */
	{ "[FREE]",     NULL },    /* no layout function means floating behavior */
	{ "[MONOCLE]",  monocle },
	{ "[GRID]",     gaplessgrid },
 	{ "[BSTACK-V]", bstack },
 	{ "[BSTACK-H]", bstackhoriz },
};

/* key definitions */

#define BOTTOM_BAR_HEIGHT 16
#define MODKEY Mod4Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "sakura", NULL };
static const char *xfeject[] = { "eject", NULL };
static const char *xfaudioraisevolume[] = { "amixer", "-q", "sset", "Master", "3%+", "unmute", NULL };
static const char *xfaudiolowervolume[] = { "amixer", "-q", "sset", "Master", "3%-", "unmute", NULL };
static const char *xfaudiomute[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *xfaudioprev[] = { "sonata", "prev", NULL };
static const char *xfaudioplay[] = { "sonata", "pp", NULL };
static const char *xfaudionext[] = { "sonata", "next", NULL };
static const char *dzen[] = { "dwm-bar-show", NULL };

static Key keys[] = {
	/* modifier                     key         function        argument */
	{ MODKEY,                       XK_r,       spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,       togglebar,      {0} },
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,     view,           {0} },
	{ MODKEY,             		XK_c,       killclient,     {0} },
	{ MODKEY,                       XK_g,       setlayout,      {.v = &layouts[3] } },
	{ MODKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_n,       setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_v,       setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
    	{ MODKEY|ControlMask,           XK_j,       pushdown,       {0} },
    	{ MODKEY|ControlMask,           XK_k,       pushup,         {0} },
	{ MODKEY|ShiftMask,             XK_j,       movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,       movestack,      {.i = -1 } },
        { MODKEY,                       XK_Left,    cycle,          {.i = -1 } },
        { MODKEY,                       XK_Right,   cycle,          {.i = +1 } },
        { MODKEY|ControlMask,           XK_Left,    tagcycle,       {.i = -1 } },
        { MODKEY|ControlMask,           XK_Right,   tagcycle,       {.i = +1 } },
	TAGKEYS(                        XK_ampersand,              0)
	TAGKEYS(                        XK_eacute,                 1)
	TAGKEYS(                        XK_quotedbl,               2)
	TAGKEYS(                        XK_apostrophe,             3)
	TAGKEYS(                        XK_parenleft,              4)
	TAGKEYS(                        XK_minus,                  5)
	TAGKEYS(                        XK_egrave,                 6)
	TAGKEYS(                        XK_underscore,             7)
	TAGKEYS(                        XK_ccedilla,               8) 
	{ MODKEY|ShiftMask,             XK_q,       quit,           {0} },
	{ MODKEY,                       XK_x,       spawn,       {.v = dzen } },
	{ 0,				0x1008ff2c, spawn,       {.v = xfeject } },
	{ 0,				XK_F32,     spawn,       {.v = xfaudioraisevolume } },
	{ 0,				XK_F31,     spawn,       {.v = xfaudiolowervolume } },
	{ 0,				XK_F30,     spawn,       {.v = xfaudiomute } },
	{ 0,				XK_F33,     spawn,       {.v = xfaudioprev } },
	{ 0,				XK_F34,     spawn,       {.v = xfaudioplay } },
	{ 0,				XK_F35,     spawn,       {.v = xfaudionext } },
	{ AltMask,			XK_Tab,     focusstack,       {.i = +1 } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
