#ifndef ELM_MODEL_TREE_PATH
#define ELM_MODEL_TREE_PATH

struct _Elm_Model_Tree_Path;
typedef struct _Elm_Model_Tree_Path Elm_Model_Tree_Path;

/**
 * @brief TODO: document
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_path_new();

/**
 * @brief TODO: document
 */
EAPI void elm_model_tree_path_free(Elm_Model_Tree_Path *path);

/**
 * @brief TODO: document
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_path_new_from_string(const char *str);

/**
 * @brief TODO: document
 */
EAPI void elm_model_tree_path_append_index(Elm_Model_Tree_Path *path, unsigned int index);

/**
 * @brief TODO: document
 */
EAPI void elm_model_tree_path_prepend_index(Elm_Model_Tree_Path *path, unsigned int index);

/**
 * @brief TODO: document
 */
EAPI char* elm_model_tree_path_to_string(Elm_Model_Tree_Path *path);

/**
 * @brief TODO: document
 */
EAPI unsigned int elm_model_tree_path_get_depth(Elm_Model_Tree_Path *path);

/**
 * @brief TODO: document
 */
EAPI unsigned int* elm_model_tree_path_get_indices(Elm_Model_Tree_Path *path);

/**
 * @brief TODO: document
 */
EAPI unsigned int elm_model_tree_path_get_index(Elm_Model_Tree_Path *path, unsigned int depth);
  

#endif // ELM_MODEL_TREE_PATH
