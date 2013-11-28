
#ifndef ELM_MODEL_TREE_CONST_H
#define ELM_MODEL_TREE_CONST_H

/**
 * @def elm_model_tree_select
 * @since 1.8
 *
 * @param path Path to the node to be selected.
 * 
 * Select the node pointed by @p path.
 */
EAPI void elm_model_tree_select(Elm_Model_Tree_Path path);

/**
 * @def elm_model_tree_value_get
 * @since 1.8
 *
 * @param path The node from which @p value is read.
 * @return The value read from the node, as @ref Eina_Value.
 * 
 * Get the value of the node pointed by @p path.
 *
 */
EAPI Eina_Value elm_model_tree_value_get(Elm_Model_Tree_Path path);

/**
 * @def elm_model_tree_selected
 * @since 1.8
 *
 * Raised when a tree model is selected.
 */

/**
 * @def elm_model_tree_reordered
 * @since 1.8
 *
 * Raised when a three model is reordered.
 */

/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_TREE_CONST_INTERFACE elm_model_tree_const                     \
   , function(elm_model_tree_select, void, Elm_Model_Tree_Path)                 \
   , function(elm_model_tree_value_get, Eina_Value, Elm_Model_Tree_Path)        \
   /* , event(elm_model_tree_node_selected) \ */
   /* , event(elm_model_tree_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_TREE_CONST_INTERFACE)

#endif // ELM_MODEL_TREE_CONST_H

