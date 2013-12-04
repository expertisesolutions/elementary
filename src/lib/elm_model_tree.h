
#ifndef ELM_MODEL_TREE_H
#define ELM_MODEL_TREE_H

#include "elm_model_tree_const.h"
#include "elm_model_tree_mutable.h"

EAPI Elm_Model_Tree_Path* elm_model_tree_path_new();

EAPI void elm_model_tree_path_free(Elm_Model_Tree_Path *path);

EAPI Elm_Model_Tree_Path* elm_model_tree_path_new_from_string(const char *str);

EAPI void elm_model_tree_path_append_index(Elm_Model_Tree_Path *path, unsigned int index);

EAPI void elm_model_tree_path_prepend_index(Elm_Model_Tree_Path *path, unsigned int index);

EAPI char* elm_model_tree_path_to_string(Elm_Model_Tree_Path *path);

EAPI unsigned int elm_model_tree_path_get_depth(Elm_Model_Tree_Path *path);

EAPI unsigned int* elm_model_tree_path_get_indices(Elm_Model_Tree_Path *path);

EAPI unsigned int elm_model_tree_path_get_index(Elm_Model_Tree_Path *path, unsigned int depth);
  
#endif // ELM_MODEL_TREE_H
