#ifndef ELM_MODEL_TREE_CONST_H
#define ELM_MODEL_TREE_CONST_H

/**
 * @def elm_model_tree_select
 * @since 1.8
 *
 * @param path Path to the node to be selected. If @p path is NULL
 * the currently selected node is unselected.
 * 
 * Select the node pointed by @p path.
 */
EAPI Eina_Bool elm_model_tree_select(Elm_Model_Tree_Path *path);

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
EAPI Eina_Value* elm_model_tree_value_get(Elm_Model_Tree_Path *path);

/**
 * @def elm_model_tree_root_get
 * @since 1.8
 *
 * @return The path of the root node, as Elm_Model_Tree_Path.
 *
 * Get the path of the root node.
 *
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_root_get();

/**
 * @def elm_model_tree_children_get
 * @since 1.8
 *
 * @param path The node from which @p children is listed
 * @return The a list children path, as Eina_list of Elm_Model_Tree_Path.
 *
 * Get the children of the node pointed by @p path.
 *
 */
EAPI Eina_List* elm_model_tree_children_get(Elm_Model_Tree_Path*);

/**
 * @def elm_model_tree_selected_get
 * @since 1.8
 *
 * @return The path of the selected node, as Elm_Model_Tree_Path.
 *
 * Get the path of the selected node.
 *
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_selected_get();


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
 * @def elm_model_tree_child_inserted
 * @since 1.8
 *
 * Raised when a new child is inserted.
 */

/**
 * @def elm_model_tree_deleted
 * @since 1.8
 *
 * Raised when a tree model is deleted.
 */

/**
 * @def elm_model_tree_changed
 * @since 1.8
 *
 * Raised when the tree model is changed.
 */


/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_TREE_CONST_CLASS elm_model_tree_const                                           \
 , constructor(elm_model_tree_constructor, _model_tree_constructor, Eina_Value*) \
 , destructor(_model_tree_destructor)                                   \
 , function(elm_model_tree_select, _model_tree_select, Eina_Bool, Elm_Model_Tree_Path*)           \
 , function(elm_model_tree_value_get, _model_tree_value_get, Eina_Value*, Elm_Model_Tree_Path*)   \
 , function(elm_model_tree_children_get, _model_tree_children_get, Eina_List*, Elm_Model_Tree_Path*) \
 , function(elm_model_tree_selected_get, _model_tree_selected_get, Elm_Model_Tree_Path*)          \
 , event(elm_model_tree_select, Elm_Model_Tree_Path*)

/*, event(elm_model_tree_node_inserted, Elm_Model_Tree_Path*)                  \ */
/* , event(elm_model_tree_node_deleted, Elm_Model_Tree_Path*)                   \ */
/* , event(elm_model_tree_node_changed, Elm_Model_Tree_Path*)                   \ */
/* , event(elm_model_tree_reordered, int) */

EO3_DECLARE_CLASS(ELM_MODEL_TREE_CONST_CLASS) // XXX NON-INSTANTIABLE

#endif // ELM_MODEL_TREE_CONST_H
