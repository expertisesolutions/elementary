#ifndef ELM_MODEL_TREE_MUTABLE_H
#define ELM_MODEL_TREE_MUTABLE_H

/**
 * @def elm_model_tree_item_append
 * @since 1.8
 *
 * @param path The path to a node where the new child shall be appended.
 * @param value The value to append. The value is not copied so it's
 * this class responsibility to deleted it.
 * @return The path to the newly created node.
 *
 * Append a new child cointaining @p value to the list of children of
 * the node pointed by @p path.
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_child_append(Elm_Model_Tree_Path *path,
                                                      Eina_Value *value);

/**
 * @def elm_model_tree_item_prepend
 * @since 1.8
 *
 * @param path The path to a node where the new child shall be prepended.
 * @param value The value to prepend.
 * @return The path to the newly created node.
 * 
 * Prepend a new child cointaining @p value to the list of children of
 * the node pointed by @p path.
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_child_prepend(Elm_Model_Tree_Path *path,
                                                       Eina_Value *value);

/**
 * @def elm_model_tree_child_append_relative
 * @since 1.8
 *
 * @param path The path to a node where the new child shall be appended.
 * @param value The value to set to the newly created node, as @ref Eina_Value.
 * @return The path to the newly created node.
 * 
 * Append @p value as a sibling node of @p path.
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_child_append_relative(Elm_Model_Tree_Path *path,
                                                               Eina_Value *value);

/**
 * @def elm_model_tree_child_prepend_relative
 * @since 1.8
 *
 * @param path The path to a sibling node where the new node shall be prepended.
 * @param value The value to be set in the newly created node, as @ref Eina_Value.
 * @return The path to the newly created node.
 * 
 * Prepend @p value as a sibling node of @p path.
 */
EAPI Elm_Model_Tree_Path* elm_model_tree_child_prepend_relative(Elm_Model_Tree_Path *path,
                                                                Eina_Value *value);

/**
 * @def elm_model_tree_delete
 * @since 1.8
 *
 * @param path Path to the sub-tree to delete.
 * 
 * Delete the sub-tree pointed by @p path and all its children.
 */
EAPI void elm_model_tree_delete(Elm_Model_Tree_Path *path);

/**
 * @def elm_model_tree_value_set
 * @since 1.8
 *
 * @param path The path to the node to receive the new value.
 * @param value The value, as @ref Eina_Value.
 * 
 * Set @p value to the node pointed by @p path.
 */
EAPI void elm_model_tree_value_set(Elm_Model_Tree_Path *path, Eina_Value *value);

/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_TREE_CLASS elm_model_tree                             \
 , function(elm_model_tree_child_append, _model_tree_child_append,      \
            Elm_Model_Tree_Path*, Elm_Model_Tree_Path*, Eina_Value*)    \
 , function(elm_model_tree_child_prepend, _model_tree_child_prepend,    \
            Elm_Model_Tree_Path*, Elm_Model_Tree_Path*, Eina_Value*)    \
 , function(elm_model_tree_child_append_relative, _model_tree_child_append_relative, \
            Elm_Model_Tree_Path*, Elm_Model_Tree_Path*, Eina_Value*)    \
 , function(elm_model_tree_child_prepend_relative, _model_tree_child_prepend_relative, \
            Elm_Model_Tree_Path*, Elm_Model_Tree_Path*, Eina_Value*)    \
 , function(elm_model_tree_delete, _model_tree_delete,                  \
            void, Elm_Model_Tree_Path*)                                 \
 , function(elm_model_tree_value_set, _model_tree_value_set,            \
            void, Elm_Model_Tree_Path*, Eina_Value*)                    \
 , event(elm_model_tree_child_append, Elm_Model_Tree_Path*)             \
 , event(elm_model_tree_delete, Elm_Model_Tree_Path*)                   \
 , event(elm_model_tree_value_set, Elm_Model_Tree_Path*, Eina_Value*)

EO3_DECLARE_CLASS(ELM_MODEL_TREE_CLASS)

#endif // ELM_MODEL_TREE_MUTABLE_H
