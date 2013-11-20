
#ifndef ELM_MODEL_LIST_H
#define ELM_MODEL_LIST_H

#include "Eo.h"

/**
 * @def elm_model_list_foreach
 * @since 1.8
 *
 *
 */
EAPI void elm_model_list_foreach();


/**
 * @def elm_model_list_mode_get
 * @since 1.8
 *
 *
 */
EAPI void elm_model_list_mode_get();


/**
 * @def elm_model_list_set_mode
 * @since 1.8
 *
 *
 */
EAPI void elm_model_list_set_mode();


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
 * @def elm_model_list_item_insert
 * @since 1.8
 *
 * insert a new item in list model at position x
 *
 */
EAPI void elm_model_list_item_insert();

/**
 * @def elm_model_list_item_delete
 * @since 1.8
 *
 * delete a item in list model
 *
 */
EAPI void elm_model_list_item_delete();

/**
 * @def elm_model_list_item_seledt
 * @since 1.8
 *
 * select a item in list model
 *
 */
EAPI void elm_model_list_item_seledt();

/**
 * @def elm_model_list_item_replace
 * @since 1.8
 *
 * replace a item in list model
 *
 */
EAPI void elm_model_list_item_replace();

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
#define ELM_MODEL_LIST_INTERFACE elm_model_list                         \
   , function(elm_model_list_foreach, void)                             \
   , function(elm_model_list_get_mode, void)                            \
   , function(elm_model_list_set_mode, void)                            \
   , function(elm_model_list_item_append, void)                         \
   , function(elm_model_list_item_prepend, void)                        \
   , function(elm_model_list_item_insert, void)                         \
   , function(elm_model_list_item_delete, void)                         \
   , function(elm_model_list_item_select, void)                         \
   , function(elm_model_list_item_replace, void)                        \
   , function(elm_model_list_value_get, void)                           \
   , function(elm_model_list_value_set, void)
   /* , event(elm_model_list_item_inserted)                                \ */
   /* , event(elm_model_list_item_deleted)                                 \ */
   /* , event(elm_model_list_item_selected)                                \ */
   /* , event(elm_model_list_item_changed)                                 \ */
   /* , event(elm_model_list_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_LIST_INTERFACE)

#endif // ELM_MODEL_LIST_H
