
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_list.h"

#include <assert.h>

/// Const interface ///

static void
_model_list_item_select(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   return;
}

static void
_model_list_selected_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_List_Index *index = va_arg(*list, Elm_Model_List_Index *);
   EINA_SAFETY_ON_NULL_RETURN(index);
   *index = (Elm_Model_List_Index)-1; 
}

static void
_model_list_len(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   long *len = va_arg(*list, long *);
   EINA_SAFETY_ON_NULL_RETURN(len);
   *len = 0L;
}

static void
_model_list_value_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Eina_Value **value = va_arg(*list, Eina_Value **);
   EINA_SAFETY_ON_NULL_RETURN(*value);
   *value = EINA_FALSE;
}

/// Mutable interface ///

static Elm_Model_List_Index
_model_list_item_append(Eina_Value *value)
{
   return (Elm_Model_List_Index)-1;
}

static Elm_Model_List_Index
_model_list_item_prepend(Eina_Value *value)
{
   return (Elm_Model_List_Index)-1;
}

static Elm_Model_List_Index
_model_list_item_append_relative(Elm_Model_List_Index index, Eina_Value *value)
{
   return (Elm_Model_List_Index)-1;
}

static Elm_Model_List_Index
_model_list_item_prepend_relative(Elm_Model_List_Index index, Eina_Value *value)
{
   return (Elm_Model_List_Index)-1;
}

static void
_model_list_item_delete(Elm_Model_List_Index index)
{
}

/// Class definition ///

/* EO3_DEFINE_CLASS(ELM_MODEL_LIST_CONST_CLASS, ((EO3_BASE_CLASS)), NULL) */

/* EO3_DEFINE_CLASS(ELM_MODEL_LIST_CLASS, ((ELM_MODEL_LIST_CONST_CLASS)), NULL) */
