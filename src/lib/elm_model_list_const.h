
#ifndef ELM_MODEL_LIST_CONST_H
#define ELM_MODEL_LIST_CONST_H

typedef long Elm_Model_List_Index;

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
 * @def elm_model_list_item_selected
 * @since 1.8
 *
 * Raised when an item of this list model is selected.
 */

/**
 * @def elm_model_list_reordered
 * @since 1.8
 *
 * Raised when this list model is reordered.
 */

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
 * @def elm_model_list_item_changed
 * @since 1.8
 *
 * Raised when an item of this list model is changed.
 */

/**
 * @brief EO3 Interface Declaration
 */
#define ELM_MODEL_LIST_CONST_INTERFACE elm_model_list_const             \
  , function(elm_model_list_item_select, void, Elm_Model_List_Index)    \
  , function(elm_model_list_value_get, Eina_Value, Elm_Model_List_Index)

/* , event(elm_model_list_item_selected) \ */
   /* , event(elm_model_list_item_inserted) \ */
   /* , event(elm_model_list_item_deleted)  \ */
   /* , event(elm_model_list_item_changed)  \ */
   /* , event(elm_model_list_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_LIST_CONST_INTERFACE)

#endif // ELM_MODEL_LIST_CONST_H
