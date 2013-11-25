
#ifndef ELM_MODEL_TREE_H
#define ELM_MODEL_TREE_H

#include "Eo.h"
#include "Eina.h"
typedef struct _Elm_Model_Tree_Path Elm_Model_Tree_Path; // XXX

/**
 * @def elm_model_tree_item_append
 * @since 1.8
 *
 * Append a new child to a tree node.
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_child_append(Eina_Value);


/**
 * @def elm_model_tree_item_prepend
 * @since 1.8
 *
 * Prepend a new child to the list of children of the selected tree node.
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_child_prepend(Eina_Value);


/**
 * @def elm_model_tree_child_append_relative
 * @since 1.8
 *
 * Append a new child to a tree node on a specific position.
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_child_append_relative(Elm_Model_Tree_Path, Eina_Value);


/**
 * @def elm_model_tree_child_prepend_relative
 * @since 1.8
 *
 * Prepend a new child to a tree node.
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_child_prepend_relative(Elm_Model_Tree_Path, Eina_Value);

/**
 * @def elm_model_tree_delete
 * @since 1.8
 *
 * Delete a tree model pointed by @a path, and all its subtrees.
 *
 */
EAPI void elm_model_tree_delete(Elm_Model_Tree_Path);

/**
 * @def elm_model_tree_select
 * @since 1.8
 *
 * Select the tree pointed by @a path.
 *
 */
EAPI void elm_model_tree_select(Elm_Model_Tree_Path);

/**
 * @def elm_model_tree_value_get
 * @since 1.8
 *
 * Get the value of the node pointed by @a path.
 *
 */
EAPI Eina_Value elm_model_tree_value_get(Elm_Model_Tree_Path);

/**
 * @def elm_model_tree_value_set
 * @since 1.8
 *
 * Set a value to the node pointed by @a path.
 *
 */
EAPI void elm_model_tree_value_set(Elm_Model_Tree_Path, Eina_Value);

/**
 * @def elm_model_tree_child_inserted
 * @since 1.8
 *
 * Raised when a new child is inserted.
 *
 */

/**
 * @def elm_model_tree_deleted
 * @since 1.8
 *
 * Raised when a tree model is deleted.
 *
 */

/**
 * @def elm_model_tree_selected
 * @since 1.8
 *
 * Raised when a tree model is selected.
 *
 */

/**
 * @def elm_model_tree_changed
 * @since 1.8
 *
 * Raised when the tree model is changed.
 *
 */

/**
 * @def elm_model_tree_reordered
 * @since 1.8
 *
 * Raised when a three model is reordered.
 *
 */



/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_TREE_INTERFACE elm_model_tree                               \
   , function(elm_model_tree_child_append, Elm_Model_Tree_Path, Eina_Value)   \
   , function(elm_model_tree_child_prepend, Elm_Model_Tree_Path, Eina_Value)  \
   , function(elm_model_tree_child_append_relative, Elm_Model_Tree_Path, Elm_Model_Tree_Index, Eina_Value)  \
   , function(elm_model_tree_child_prepend_relative, Elm_Model_Tree_Path, Elm_Model_Tree_Index, Eina_Value) \
   , function(elm_model_tree_delete, void, Elm_Model_Tree_Path)               \
   , function(elm_model_tree_select, void, Elm_Model_Tree_Path)   						\
   , function(elm_model_tree_value_get, Eina_Value, Elm_Model_Tree_Path)			\
   , function(elm_model_tree_value_set, void, Elm_Model_Tree_Path, Eina_Value)
   /* , event(elm_model_tree_node_inserted)                                \ */
   /* , event(elm_model_tree_node_deleted)                                 \ */
   /* , event(elm_model_tree_node_selected)                                \ */
   /* , event(elm_model_tree_node_changed)                                 \ */
   /* , event(elm_model_tree_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_TREE_INTERFACE)

#endif // ELM_MODEL_TREE_H
