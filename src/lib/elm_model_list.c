
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_list.h"

#include <assert.h>

/// Const interface ///

static void
_model_list_item_select(Elm_Model_List_Index index)
{
}

static Elm_Model_List_Index
_model_list_selected_get()
{
   return (Elm_Model_List_Index)-1;
}

static Eina_Value*
_model_list_value_get(Elm_Model_List_Index index)
{
   return NULL;
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

EO3_DEFINE_CLASS(ELM_MODEL_LIST_CONST_CLASS, ((EO3_BASE_CLASS)), NULL)

EO3_DEFINE_CLASS(ELM_MODEL_LIST_CLASS, ((ELM_MODEL_LIST_CONST_CLASS)), NULL)


