#ifndef EO_LIST_MODEL_CLASS_H
#define EO_LIST_MODEL_CLASS_H

#include "Eo.h"

extern EAPI Eo_Op ELM_LIST_MODEL_BASE_ID;

enum
{
   EO_LIST_MODEL_SUB_ID_FOREACH,
   EO_LIST_MODEL_SUB_ID_COUNT, //?
   EO_LIST_MODEL_SUB_ID_GET_MODE,
   EO_LIST_MODEL_SUB_ID_SET_MODE,
   EO_LIST_MODEL_SUB_ID_ITEM_APPEND,
   EO_LIST_MODEL_SUB_ID_ITEM_PREPEND,
   EO_LIST_MODEL_SUB_ID_ITEM_INSERT,
   EO_LIST_MODEL_SUB_ID_ITEM_DELETE,
   EO_LIST_MODEL_SUB_ID_ITEM_REPLACE,
   EO_LIST_MODEL_SUB_ID_ITEM_SELECT,
   EO_LIST_MODEL_SUB_ID_VALUE_GET, //Item_get?
   EO_LIST_MODEL_SUB_ID_VALUE_SET, // Item_set?
   EO_LIST_MODEL_SUB_ID_LAST
};

#define EO_LIST_MODEL_ID(sub_id) (EO_LIST_MODEL_BASE_ID + sub_id)

#define EO_LIST_MODEL_CLASS eo_list_model_class_get()

const Eo_Class *eo_list_model_class_get(void) EINA_CONST;

/**
 * @def eo_list_model_foreach
 * @since 1.8
 *
 * 
 *
 */
#define eo_list_model_foreach() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_FOREACH)

/**
 * @def eo_list_model_mode_get
 * @since 1.8
 *
 *
 */
#define eo_list_model_get_mode() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_GET_MODE)

/**
 * @def eo_list_model_set_mode
 * @since 1.8
 *
 *
 */
#define eo_list_model_set_mode() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_SET_MODE)

/**
 * @def eo_list_model_item_append
 * @since 1.8
 *
 * append a new item in list model
 *
 */
#define eo_list_model_item_append() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_ITEM_APPEND)

/**
 * @def eo_list_model_item_prepend
 * @since 1.8
 *
 * prepend a new item in list model
 *
 */
#define eo_list_model_item_prepend() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_ITEM_PREPEND)

/**
 * @def eo_list_model_item_insert
 * @since 1.8
 *
 * insert a new item in list model at position x
 *
 */
#define eo_list_model_item_insert() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_ITEM_INSERT)

/**
 * @def eo_list_model_item_delete
 * @since 1.8
 *
 * delete a item in list model
 *
 */
#define eo_list_model_item_delete() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_ITEM_DELETE)

/**
 * @def eo_list_model_item_seledt
 * @since 1.8
 *
 * select a item in list model
 *
 */
#define eo_list_model_item_select() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_ITEM_SELECT)

/**
 * @def eo_list_model_item_replace
 * @since 1.8
 *
 * replace a item in list model
 *
 */
#define eo_list_model_item_replace() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_ITEM_REPLACE)

/**
 * @def eo_list_model_value_get
 * @since 1.8
 *
 * get a item value in list model
 *
 */
#define eo_list_model_value_get() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_VALUE_GET)

/**
 * @def eo_list_model_value_set
 * @since 1.8
 *
 * set a item value in list model
 *
 */
#define eo_list_model_value_set() EO_LIST_MODEL_ID(EO_LIST_MODEL_SUB_ID_VALUE_SET)

// SIGNALS

/*
extern const Eo_Event_Description _EO_LIST_MODEL_UPDATED;
#define EO_LIST_MODEL_UPDATED (&(_EO_LIST_MODEL_UPDATED))
*/

extern const Eo_Event_Description _EO_LIST_MODEL_ITEM_INSERTED;
/**
 * @def EO_LIST_MODEL_ITEM_INSERTED
 * @since 1.8
 *
 * Emitted when a new item is inserted in list model
 *
 */
#define EO_LIST_MODEL_ITEM_INSERTED (&(_EO_LIST_MODEL_ITEM_INSERTED))


extern const Eo_Event_Description _EO_LIST_MODEL_ITEM_DELETED;
/**
 * @def EO_LIST_MODEL_ITEM_DELETED
 * @since 1.8
 *
 * Emitted when a list model item is deleted
 *
 */
#define EO_LIST_MODEL_ITEM_DELETED(&(_EO_LIST_MODEL_ITEM_DELETED))


extern const Eo_Event_Description _EO_LIST_MODEL_ITEM_SELECTED;
/**
 * @def EO_LIST_MODEL_ITEM_SELECTED
 * @since 1.8
 *
 * Emitted when a list model item is selected
 *
 */
#define EO_LIST_MODEL_ITEM_SELECTED(&(_EO_LIST_MODEL_ITEM_SELECTED))


extern const Eo_Event_Description _EO_LIST_MODEL_ITEM_CHANGED;
/**
 * @def EO_LIST_MODEL_ITEM_CHANGED
 * @since 1.8
 *
 * Emitted when a item of list model is changed
 *
 */
#define EO_LIST_MODEL_ITEM_CHANGED(&(_EO_LIST_MODEL_ITEM_CHANGED))


extern const Eo_Event_Description _EO_LIST_MODEL_REORDERED;
/**
 * @def EO_LIST_MODEL_REORDERED
 * @since 1.8
 *
 * Emitted when a item of list model is reordered
 *
 */
#define EO_LIST_MODEL_REORDERED(&(_EO_LIST_MODEL_REORDERED))

