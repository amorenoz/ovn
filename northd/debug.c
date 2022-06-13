#include <config.h>

#include <string.h>

#include "debug.h"

#include "smap.h"

static struct debug_config config;

void
init_debug_config(const struct nbrec_nb_global *nb)
{

    const struct smap *options = &nb->options;
    config.enabled = smap_get_bool(options, "debug_drop_mode", false);
}

bool
debug_enabled(void)
{
    return config.enabled;
}
