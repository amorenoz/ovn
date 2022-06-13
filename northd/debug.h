/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NORTHD_DEBUG_H
#define NORTHD_DEBUG_H 1

#include <stdint.h>
#include <stdbool.h>

#include "lib/ovn-nb-idl.h"

struct debug_config {
    bool enabled;
};

void init_debug_config(const struct nbrec_nb_global *nb);

bool debug_enabled(void);

#endif /* NORTHD_DEBUG_H */
