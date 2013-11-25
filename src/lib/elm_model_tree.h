
#ifndef ELM_MODEL_TREE_H
#define ELM_MODEL_TREE_H

#include "Eo.h"
#include "Eina.h"
typedef struct _Elm_Model_Tree_Path Elm_Model_Tree_Path; // XXX

/**
 * @def elm_model_tree_item_append
 * @since 1.8
 *
 * append a new item in tree model
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_item_append(Eina_Value);


/**
 * @def elm_model_tree_item_prepend
 * @since 1.8
 *
 * prepend a new item in tree model
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_item_prepend(Eina_Value);


/**
 * @def elm_model_tree_item_append_relative
 * @since 1.8
 *
 * append a new item in tree model
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_item_append_relative(Elm_Model_Tree_Path, Eina_Value);


/**
 * @def elm_model_tree_item_prepend_relative
 * @since 1.8
 *
 * prepend a new item in tree model
 *
 */
EAPI Elm_Model_Tree_Path elm_model_tree_item_prepend_relative(Elm_Model_Tree_Path, Eina_Value);

/**
 * @def elm_model_tree_item_delete
 * @since 1.8
 *
 * delete a item in tree model
 *
 */
EAPI void elm_model_tree_item_delete(Elm_Model_Tree_Path);

/**
 * @def elm_model_tree_item_select
 * @since 1.8
 *
 * select a item in tree model
 *
 */
EAPI void elm_model_tree_item_select(Elm_Model_Tree_Path);

/**
 * @def elm_model_tree_value_get
 * @since 1.8
 *
 * get a item value in tree model
 *
 */
EAPI Eina_Value elm_model_tree_value_get(Elm_Model_Tree_Path);

/**
 * @def elm_model_tree_value_set
 * @since 1.8
 *
 * set a item value in tree model
 *
 */
EAPI void elm_model_tree_value_set(Elm_Model_Tree_Path, Eina_Value);

/**
 * @def elm_model_tree_item_inserted
 * @since 1.8
 *
 * Emitted when a new item is inserted in tree model
 *
 */

/**
 * @def elm_model_tree_item_deleted
 * @since 1.8
 *
 * Emitted when a tree model item is deleted
 *
 */

/**
 * @def elm_model_tree_item_selected
 * @since 1.8
 *
 * Emitted when a tree model item is selected
 *
 */

/**
 * @def elm_model_tree_item_changed
 * @since 1.8
 *
 * Emitted when a item of tree model is changed
 *
 */

/**
 * @def elm_model_tree_reordered
 * @since 1.8
 *
 * Emitted when a item of tree model is reordered
 *
 */



/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_TREE_INTERFACE elm_model_tree                         					\
   , function(elm_model_tree_child_append, Elm_Model_Tree_Path, Eina_Value)                         		\
   , function(elm_model_tree_child_prepend, Elm_Model_Tree_Path, Eina_Value)                        		\
   , function(elm_model_tree_child_append_relative, Elm_Model_Tree_Path, Elm_Model_Tree_Index, Eina_Value)   	\
   , function(elm_model_tree_child_prepend_relative, Elm_Model_Tree_Path, Elm_Model_Tree_Index, Eina_Value)  	\
   , function(elm_model_tree_delete, void, Elm_Model_Tree_Path)  	 					\
   , function(elm_model_tree_select, void, Elm_Model_Tree_Path)   						\
   , function(elm_model_tree_value_get, Eina_Value, Elm_Model_Tree_Path)					\
   , function(elm_model_tree_value_set, void, Elm_Model_Tree_Path, Eina_Value)
   /* , event(elm_model_tree_node_inserted)                                \ */
   /* , event(elm_model_tree_node_deleted)                                 \ */
   /* , event(elm_model_tree_node_selected)                                \ */
   /* , event(elm_model_tree_node_changed)                                 \ */
   /* , event(elm_model_tree_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_TREE_INTERFACE)

#endif // ELM_MODEL_TREE_H
