
#ifndef ELM_MODEL_LIST_H
#define ELM_MODEL_LIST_H

#include "Eo.h"

/**
 * @def elm_model_list_item_append
 * @since 1.8
 *
 * append a new item in list model
 *
 */
EAPI void elm_model_list_item_append();


/**
 * @def elm_model_list_item_prepend
 * @since 1.8
 *
 * prepend a new item in list model
 *
 */
EAPI void elm_model_list_item_prepend();


/**
 * @def elm_model_list_item_append_relative
 * @since 1.8
 *
 * append a new item in list model
 *
 */
EAPI void elm_model_list_item_append_relative();


/**
 * @def elm_model_list_item_prepend_relative
 * @since 1.8
 *
 * prepend a new item in list model
 *
 */
EAPI void elm_model_list_item_prepend_relative();


/**
 * @def elm_model_list_item_delete
 * @since 1.8
 *
 * delete a item in list model
 *
 */
EAPI void elm_model_list_item_delete();

/**
 * @def elm_model_list_item_select
 * @since 1.8
 *
 * select a item in list model
 *
 */
EAPI void elm_model_list_item_select();

/**
 * @def elm_model_list_value_get
 * @since 1.8
 *
 * get a item value in list model
 *
 */
EAPI void elm_model_list_value_get();

/**
 * @def elm_model_list_value_set
 * @since 1.8
 *
 * set a item value in list model
 *
 */
EAPI void elm_model_list_value_set();

/**
 * @def elm_model_list_item_inserted
 * @since 1.8
 *
 * Emitted when a new item is inserted in list model
 *
 */

/**
 * @def elm_model_list_item_deleted
 * @since 1.8
 *
 * Emitted when a list model item is deleted
 *
 */

/**
 * @def elm_model_list_item_selected
 * @since 1.8
 *
 * Emitted when a list model item is selected
 *
 */

/**
 * @def elm_model_list_item_changed
 * @since 1.8
 *
 * Emitted when a item of list model is changed
 *
 */

/**
 * @def elm_model_list_reordered
 * @since 1.8
 *
 * Emitted when a item of list model is reordered
 *
 */

/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_LIST_INTERFACE elm_model_list                         					\
   , function(elm_model_list_item_append, elm_model_list_index, eina_value)                         		\
   , function(elm_model_list_item_prepend, elm_model_list_index, eina_value)                        		\
   , function(elm_model_list_item_append_relative, elm_model_list_index, elm_model_list_index, eina_value)   	\
   , function(elm_model_list_item_prepend_relative, elm_model_list_index, elm_model_list_index, eina_value)  	\
   , function(elm_model_list_item_delete, void, elm_model_list_index)   					\
   , function(elm_model_list_item_select, void, elm_model_list_index)   					\
   , function(elm_model_list_value_get, eina_value, elm_model_list_index)					\
   , function(elm_model_list_value_set, void, elm_model_list_index, eina_value)
   /* , event(elm_model_list_item_inserted)                                \ */
   /* , event(elm_model_list_item_deleted)                                 \ */
   /* , event(elm_model_list_item_selected)                                \ */
   /* , event(elm_model_list_item_changed)                                 \ */
   /* , event(elm_model_list_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_LIST_INTERFACE)

#endif // ELM_MODEL_LIST_H
