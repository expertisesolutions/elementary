
#ifndef ELM_MODEL_LIST_H
#define ELM_MODEL_LIST_H

#include "Eo.h"
#include "Eina.h"
typedef struct _Elm_Model_List_Index Elm_Model_List_Index; // XXX

/**
 * @def elm_model_list_item_append
 * @since 1.8
 *
 * append a new item in list model
 *
 */
EAPI Elm_Model_List_Index elm_model_list_item_append(Eina_Value);


/**
 * @def elm_model_list_item_prepend
 * @since 1.8
 *
 * prepend a new item in list model
 *
 */
EAPI Elm_Model_List_Index elm_model_list_item_prepend(Eina_Value);


/**
 * @def elm_model_list_item_append_relative
 * @since 1.8
 *
 * append a new item in list model
 *
 */
EAPI Elm_Model_List_Index elm_model_list_item_append_relative(Elm_Model_List_Index, Eina_Value);


/**
 * @def elm_model_list_item_prepend_relative
 * @since 1.8
 *
 * prepend a new item in list model
 *
 */
EAPI Elm_Model_List_Index elm_model_list_item_prepend_relative(Elm_Model_List_Index, Eina_Value);

/**
 * @def elm_model_list_item_delete
 * @since 1.8
 *
 * delete a item in list model
 *
 */
EAPI void elm_model_list_item_delete(Elm_Model_List_Index);

/**
 * @def elm_model_list_item_select
 * @since 1.8
 *
 * select a item in list model
 *
 */
EAPI void elm_model_list_item_select(Elm_Model_List_Index);

/**
 * @def elm_model_list_value_get
 * @since 1.8
 *
 * get a item value in list model
 *
 */
EAPI Eina_Value elm_model_list_value_get(Elm_Model_List_Index);

/**
 * @def elm_model_list_value_set
 * @since 1.8
 *
 * set a item value in list model
 *
 */
EAPI void elm_model_list_value_set(Elm_Model_List_Index, Eina_Value);

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
   , function(elm_model_list_item_append, Elm_Model_List_Index, Eina_Value)                         		\
   , function(elm_model_list_item_prepend, Elm_Model_List_Index, Eina_Value)                        		\
   , function(elm_model_list_item_append_relative, Elm_Model_List_Index, Elm_Model_List_Index, Eina_Value)   	\
   , function(elm_model_list_item_prepend_relative, Elm_Model_List_Index, Elm_Model_List_Index, Eina_Value)  	\
   , function(elm_model_list_item_delete, void, Elm_Model_List_Index)   					\
   , function(elm_model_list_item_select, void, Elm_Model_List_Index)   					\
   , function(elm_model_list_value_get, Eina_Value, Elm_Model_List_Index)					\
   , function(elm_model_list_value_set, void, Elm_Model_List_Index, Eina_Value)
   /* , event(elm_model_list_item_inserted)                                \ */
   /* , event(elm_model_list_item_deleted)                                 \ */
   /* , event(elm_model_list_item_selected)                                \ */
   /* , event(elm_model_list_item_changed)                                 \ */
   /* , event(elm_model_list_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_LIST_INTERFACE)

#endif // ELM_MODEL_LIST_H
