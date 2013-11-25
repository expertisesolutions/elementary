
#ifndef ELM_MODEL_LIST_H
#define ELM_MODEL_LIST_H

#include "Eo.h"
#include "Eina.h"
typedef struct _Elm_Model_List_Index Elm_Model_List_Index; // XXX

/**
 * @def elm_model_list_item_append
 * @since 1.8
 *
 * @param value The value to be appended to the list.
 * @return The position in the list where the value was inserted.
 *
 * Append @p value to this list model.
 */
EAPI Elm_Model_List_Index elm_model_list_item_append(Eina_Value value);


/**
 * @def elm_model_list_item_prepend
 * @since 1.8
 *
 * @param value The value to be prepended to the list.
 * @return The position in the list where the value was inserted.
 * 
 * Prepend @p value to this list model.
 */
EAPI Elm_Model_List_Index elm_model_list_item_prepend(Eina_Value value);


/**
 * @def elm_model_list_item_append_relative
 * @since 1.8
 * 
 * @param index The position from where the new item will be appended.
 * @param value The value to be appended.
 * @return The position in the list where the value was inserted.
 *
 * Append @p value to this list model after position @p index.
 */
EAPI Elm_Model_List_Index elm_model_list_item_append_relative(Elm_Model_List_Index index,
                                                              Eina_Value value);

/**
 * @def elm_model_list_item_prepend_relative
 * @since 1.8
 *
 * @param index The position from where the new item will be prepended.
 * @param value The value to be prepended.
 * @return The position in the list where the value was inserted.
 *
 * Prepend @p value to this list model before position @p index.
 */
EAPI Elm_Model_List_Index elm_model_list_item_prepend_relative(Elm_Model_List_Index index,
                                                               Eina_Value value);

/**
 * @def elm_model_list_item_delete
 * @since 1.8
 *
 * @param The index the item to delete.
 * 
 * Delete the item at position @p index.
 */
EAPI void elm_model_list_item_delete(Elm_Model_List_Index index);

/**
 * @def elm_model_list_item_select
 * @since 1.8
 *
 * @param index The position to select.
 * 
 * Select the item at position @p index.
 */
EAPI void elm_model_list_item_select(Elm_Model_List_Index index);

/**
 * @def elm_model_list_value_get
 * @since 1.8
 *
 * @param index The position from where the value will be read.
 * @return The value, as @ref Eina_Value.
 * 
 * Get the value of the item in position @p index.
 */
EAPI Eina_Value elm_model_list_value_get(Elm_Model_List_Index index);

/**
 * @def elm_model_list_value_set
 * @since 1.8
 * 
 * @param index The position where the value will be written to.
 * @param value The value to write, as @ref Eina_Value.
 * 
 * Set a value to an item of this list model.
 */
EAPI void elm_model_list_value_set(Elm_Model_List_Index index, Eina_Value value);

/**
 * @def elm_model_list_item_inserted
 * @since 1.8
 *
 * Raised when a new item is inserted into this list model.
 */

/**
 * @def elm_model_list_item_deleted
 * @since 1.8
 *
 * Raised when an item of this list model is deleted.
 */

/**
 * @def elm_model_list_item_selected
 * @since 1.8
 *
 * Raised when an item of this list model is selected.
 */

/**
 * @def elm_model_list_item_changed
 * @since 1.8
 *
 * Raised when an item of this list model is changed.
 */

/**
 * @def elm_model_list_reordered
 * @since 1.8
 *
 * Raised when this list model is reordered.
 */

/**
 * @brief EO3 Interface Declaration
 */
#define ELM_MODEL_LIST_INTERFACE elm_model_list                                   \
   , function(elm_model_list_item_append, Elm_Model_List_Index, Eina_Value)       \
   , function(elm_model_list_item_prepend, Elm_Model_List_Index, Eina_Value)      \
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
