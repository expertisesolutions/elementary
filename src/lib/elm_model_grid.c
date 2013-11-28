
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_grid.h"
#include <assert.h>

EO3_DEFINE_INTERFACE(ELM_MODEL_GRID_CONST_INTERFACE, ((EO3_BASE_CLASS)))

EO3_DEFINE_INTERFACE(ELM_MODEL_GRID_INTERFACE, ((ELM_MODEL_GRID_CONST_INTERFACE)))
