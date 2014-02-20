
#ifndef ELM_MODEL_LIST_MUTABLE_H
#define ELM_MODEL_LIST_MUTABLE_H

extern EAPI Eo_Op ELM_OBJ_MODEL_LIST_BASE_ID;


enum {
   ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND,
   ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND,
   ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND_RELATIVE,
   ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND_RELATIVE,
   ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_DELETE,
   ELM_OBJ_MODEL_LIST_SUB_ID_VALUE_SET,
   ELM_OBJ_MODEL_LIST_SUB_ID_LAST
};

#define ELM_OBJ_MODEL_LIST_ID(sub_id) (ELM_OBJ_MODEL_LIST_BASE_ID + sub_id)

/**
 * @def elm_model_list_item_append
 * @since 1.8
 *
 * @param value The value to be appended to the list.
 * @return The position in the list where the value was inserted.
 *
 * Append @p value to this list model.
 */
#define elm_model_list_item_append(value, ret) ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_List_Index *, ret)


/**
 * @def elm_model_list_item_prepend
 * @since 1.8
 *
 * @param value The value to be prepended to the list.
 * @return The position in the list where the value was inserted.
 *
 * Prepend @p value to this list model.
 */
#define elm_model_list_item_prepend(value, ret) ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_List_Index *, ret)

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
#define elm_model_list_item_append_relative(index, value ,ret) ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND_RELATIVE), EO_TYPECHECK(Elm_Model_List_Index, index), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_List_Index *, ret)

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
#define elm_model_list_item_prepend_relative(index, value ,ret) ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND_RELATIVE), EO_TYPECHECK(Elm_Model_List_Index, index), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_List_Index *, ret)

/**
 * @def elm_model_list_item_delete
 * @since 1.8
 *
 * @param The index the item to delete.
 *
 * Delete the item at position @p index.
 */
#define elm_model_list_item_delete(index) ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_DELETE), EO_TYPECHECK(Elm_Model_List_Index, index)

/**
 * @def elm_model_list_value_set
 * @since 1.8
 *
 * @param index The position where the value will be written to.
 * @param value The value to write, as a pointer to an @ref Eina_Value.
 *
 * Set a value to an item of this list model.
 */
#define elm_model_list_value_set(index, value) ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_VALUE_SET), EO_TYPECHECK(Elm_Model_List_Index, index), EO_TYPECHECK(Eina_Value *, value)


#define ELM_OBJ_MODEL_LIST_CLASS elm_obj_model_list_class_get()
const Eo_Class *elm_obj_model_list_class_get(void);

#endif // ELM_MODEL_LIST_MUTABLE_H
