/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */

static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */

static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "Fira Code Retina:size=10" };
static const char dmenufont[]       = "Fira Code Retina:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "vanitygaps.h"

static const int layoutaxis[] = {
        SPLIT_VERTICAL,   /* layout axis: 1 = x, 2 = y;
			     negative values mirror the layout,
			     setting the master area to the right / bottom
			     instead of left / top */
        TOP_TO_BOTTOM,    /* master axis: 1 = x (from left to right),
			                  2 = y (from top to bottom),
			                  3 = z (monocle) */
        TOP_TO_BOTTOM,    /* stack axis:  1 = x (from left to right),
					  2 = y (from top to bottom),
					  3 = z (monocle) */
};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[fx]",      flextile },    /* first entry is default */
	{ "[fl]",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[] = {"firefox", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */

// Basal keys
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd }},
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd }},

// Left-bottom keyrow, for applications
	{ MODKEY,			XK_z,	   spawn,	   {.v = termcmd }},
	{ MODKEY,			XK_x,	   spawn,	   {.v = browsercmd }},

	{ MODKEY,                       XK_b,      togglebar,      {0} },

// Home row keys, moving windows
	{ MODKEY,                       XK_n,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_e,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_o,      setmfact,       {.f = +0.05} },

	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_y,      incnmaster,     {.i = -1 } },

// Like Emacs
	{ MODKEY,   	                XK_k,      killclient,     {0} },

	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_t,      setflexlayout,  {.i = 261} },  	// default tiling
	{ MODKEY,	                XK_g,      setflexlayout,  {.i =   7 } }, 	// grid
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },// floating
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
// Like Emacs
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },

// Patched in
        { Mod1Mask|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
        { Mod1Mask|Mod4Mask,              XK_0,      togglegaps,     {0} },
        { Mod1Mask|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },

        { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
        { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
        { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },



};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

