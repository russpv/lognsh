#include "globals.h"

volatile sig_atomic_t	g_last_signal = 0;
static e_dbg_lvl		g_debug = DEBUG_OFF;

void set_debug_level(e_dbg_lvl lvl) 
{
    g_debug = lvl;
}

e_dbg_lvl get_debug_level(void) 
{
    return g_debug;
}
