#include <config.h>

#include <string.h>

#include "debug.h"

#include "openvswitch/dynamic-string.h"
#include "openvswitch/vlog.h"
#include "smap.h"

VLOG_DEFINE_THIS_MODULE(debug)

static struct debug_config config;

bool
debug_enabled(void)
{
    return config.enabled;
}

bool debug_sampling_enabled(void)
{
    return config.collector_set_id != 0;
}

void
init_debug_config(const struct nbrec_nb_global *nb)
{

    const struct smap *options = &nb->options;
    bool enabled = smap_get_bool(options, "debug_drop_mode", false);
    uint32_t collector_set_id = smap_get_uint(options,
                                              "debug_drop_collector_set",
                                              0);

    uint32_t observation_domain_id = smap_get_uint(options,
                                                   "debug_drop_domain_id",
                                                   0);

    if (enabled != config.enabled ||
        collector_set_id != config.collector_set_id ||
        observation_domain_id != config.observation_domain_id ||
        !config.drop_action.string) {

        if (observation_domain_id >= UINT8_MAX) {
            VLOG_ERR("Observation domain id must be an 8-bit number");
            return;
        }

        if (!enabled && collector_set_id) {
            VLOG_WARN("Debug collection set configured, "
                      "assuming debug_drop_mode");
            enabled = true;
        }

        config.enabled = enabled;
        config.collector_set_id = collector_set_id;
        config.observation_domain_id = observation_domain_id;

        ds_clear(&config.drop_action);

        VLOG_INFO("Debug drop mode: %s", debug_enabled() ? "enabled" :
                                                           "disabled");
        if (debug_sampling_enabled()) {
            ds_put_format(&config.drop_action,
                          "sample(probability=65535,"
                          "collector_set=%d,"
                          "obs_domain=%d,"
                          "obs_point=$cookie); ",
                          config.collector_set_id,
                          config.observation_domain_id);

            ds_put_format(&config.drop_action, "/* drop */");
            VLOG_INFO("Debug drop sampling: enabled");
        } else {
            ds_put_format(&config.drop_action, "drop;");
            VLOG_INFO("Debug drop sampling: disabled");
        }
    }
}

void
destroy_debug_config(void)
{
    if (config.drop_action.string) {
        ds_destroy(&config.drop_action);
        ds_init(&config.drop_action);
    }
}

const char *
debug_drop_action(void) {
    if (OVS_UNLIKELY(debug_sampling_enabled())) {
        return ds_cstr_ro(&config.drop_action);
    } else {
        return "drop;";
    }
}

const char *
debug_implicit_drop_action(void)
{
    if (OVS_UNLIKELY(debug_sampling_enabled())) {
        return ds_cstr_ro(&config.drop_action);
    } else {
        return "/* drop */";
    }
}
