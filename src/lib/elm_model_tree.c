
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_tree.h"
#include <assert.h>

struct _Elm_Model_Tree_Path
{
   unsigned int path;
};

Elm_Model_Tree_Path*
elm_model_tree_path_new()
{
   return NULL;
}

Elm_Model_Tree_Path*
elm_model_tree_path_new_from_string()
{
   return NULL;
}

void
elm_model_tree_path_append_index()
{
}

void
elm_model_tree_path_prepend_index()
{
}

char*
elm_model_tree_path_to_string()
{
   Elm_Model_Tree_Path *path;
}

unsigned int
elm_model_tree_path_get_depth()
{
  Elm_Model_Tree_Path *path;
}

unsigned int*
elm_model_tree_path_get_indexes()
{
   return NULL;
}

EO3_DEFINE_INTERFACE(ELM_MODEL_TREE_CONST_INTERFACE, ((EO3_NO_BASE_CLASS)))

EO3_DEFINE_INTERFACE(ELM_MODEL_TREE_INTERFACE, ((ELM_MODEL_TREE_CONST_INTERFACE)))


