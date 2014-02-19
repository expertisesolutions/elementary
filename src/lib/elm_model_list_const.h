
#ifndef ELM_MODEL_LIST_CONST_H
#define ELM_MODEL_LIST_CONST_H


extern EAPI Eo_Op ELM_OBJ_MODEL_LIST_CONST_BASE_ID;

enum {
   ELM_OBJ_MODEL_LIST_CONST_SUB_ID_LIST_LEN,
   ELM_OBJ_MODEL_LIST_CONST_SUB_ID_SELECTED_GET,
   ELM_OBJ_MODEL_LIST_CONST_SUB_ID_VALUE_GET,
   ELM_OBJ_MODEL_LIST_CONST_SUB_ID_ITEM_SELECT,
   ELM_OBJ_MODEL_LIST_CONST_SUB_ID_LAST
};

#define ELM_OBJ_MODEL_LIST_CONST_ID(sub_id) (ELM_OBJ_MODEL_LIST_CONST_BASE_ID + sub_id) 

typedef long Elm_Model_List_Index;

/**
 * @def elm_model_list_item_select
 * @since 1.8
 *
 * @param index The position to select.
 *
 * Select the item at position @p index.
 */
#define elm_model_list_item_select(index) ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_ITEM_SELECT), EO_TYPECHECK(Elm_Model_List_Index, index)

/**
 * @def elm_model_list_value_get
 * @since 1.8
 *
 * @param index The position from where the value will be read.
 * @return A pionter to an @ref Eina_Value.
 *
 * Get the value of the item in position @p index.
 */
#define elm_model_list_value_get(index, ret) ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_VALUE_GET), EO_TYPECHECK(Elm_Model_List_Index, index), EO_TYPECHECK(Eina_Value **, ret)

/**
 * @def elm_model_list_selected_get
 * @since 1.8
 *
 * @return index of selected item.
 *
 * Get the index of selected item.
 */
#define elm_model_list_selected_get(ret) ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_SELECTED_GET), EO_TYPECHECK(Elm_Model_List_Index *, ret)

/**
 * @def elm_model_list_len
 * @since 1.8
 *
 * @return number of items.
 *
 */
#define elm_model_list_len(ret) ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_LIST_LEN), EO_TYPECHECK(long *, ret)


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

#if 0
/**
 * @brief Const-list Model class-declaration.
 */
#define ELM_MODEL_LIST_CONST_CLASS elm_model_list_const                                         \
, function(elm_model_list_item_select, _model_list_item_select, void, Elm_Model_List_Index)    \
, function(elm_model_list_selected_get, _model_list_selected_get, Elm_Model_List_Index)        \
, function(elm_model_list_value_get, _model_list_value_get, Eina_Value*, Elm_Model_List_Index) \
, function(elm_model_list_len, _model_list_len, long)
    /* , event(elm_model_list_item_selected, Elm_Model_List_Index)         \ */
    /* , event(elm_model_list_item_inserted, Elm_Model_List_Index)         \ */
    /* , event(elm_model_list_item_deleted, Elm_Model_List_Index)          \ */
    /* , event(elm_model_list_item_changed, Elm_Model_List_Index)          \ */
    /* , event(elm_model_list_reordered, int) */
#endif
//EO3_DECLARE_CLASS(ELM_MODEL_LIST_CONST_CLASS)
#define ELM_OBJ_MODEL_LIST_CONST_CLASS elm_obj_model_list_const_class_get()
const Eo_Class *elm_obj_model_list_const_class_get(void);

#endif // ELM_MODEL_LIST_CONST_H
