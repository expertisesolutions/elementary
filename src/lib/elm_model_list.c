
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_list.h"

#include <assert.h>

// For const interface
#define MY_MODEL_LIST_CONST_CLASS ELM_OBJ_MODEL_LIST_CONST_CLASS
EAPI Eo_Op ELM_OBJ_MODEL_LIST_CONST_BASE_ID = 0;

// For mutable interface

/// Const interface ///
static void
_model_list_item_select(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   //TODO: implement
   return;
}

static void
_model_list_selected_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Elm_Model_List_Index *index = va_arg(*list, Elm_Model_List_Index *);
   EINA_SAFETY_ON_NULL_RETURN(index);
   *index = (Elm_Model_List_Index)-1; 
}

static void
_model_list_len(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   long *len = va_arg(*list, long *);
   EINA_SAFETY_ON_NULL_RETURN(len);
   *len = 0L;
}

static void
_model_list_value_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Eina_Value **value = va_arg(*list, Eina_Value **);
   EINA_SAFETY_ON_NULL_RETURN(*value);
   *value = EINA_FALSE;
}
      
static void
_model_list_constructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   //TODO: implement
   eo_do_super(obj, MY_MODEL_LIST_CONST_CLASS, eo_constructor());
}

static void
_model_list_destructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   //TODO: implement
   eo_do_super(obj, MY_MODEL_LIST_CONST_CLASS, eo_destructor());
}

static void
_model_list_const_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _model_list_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _model_list_destructor),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_ITEM_SELECT), _model_list_item_select),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_VALUE_GET), _model_list_value_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_SELECTED_GET), _model_list_selected_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_CONST_ID(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_LIST_LEN), _model_list_len),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description model_list_const_op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_ITEM_SELECT, "Select the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_VALUE_GET, "Get the value of the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_SELECTED_GET, "Get the children of the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_CONST_SUB_ID_LIST_LEN, "Count number of children of the node pointed by path."),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description model_list_const_class_descs = {
   EO_VERSION,
   "Model List Const",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_MODEL_LIST_CONST_BASE_ID, model_list_const_op_descs, ELM_OBJ_MODEL_LIST_CONST_SUB_ID_LAST),
   NULL, // event_descs
   0, //private data sizeof
   _model_list_const_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_model_list_const_class_get, &model_list_const_class_descs, EO_BASE_CLASS, NULL);

/// Mutable interface ///

static void 
_model_list_item_append(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_List_Index *ret = va_arg(*list, Elm_Model_List_Index *);

   *ret = (Elm_Model_List_Index)-1;
}

static void 
_model_list_item_prepend(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_List_Index *ret = va_arg(*list, Elm_Model_List_Index *);

   *ret = (Elm_Model_List_Index)-1;
}

static void 
_model_list_item_append_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Elm_Model_List_Index index = va_arg(*list, Elm_Model_List_Index);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_List_Index *ret = va_arg(*list, Elm_Model_List_Index *);

   *ret = (Elm_Model_List_Index)-1;
}

static void 
_model_list_item_prepend_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Elm_Model_List_Index index = va_arg(*list, Elm_Model_List_Index);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_List_Index *ret = va_arg(*list, Elm_Model_List_Index *);

   *ret = (Elm_Model_List_Index)-1;
}

static void 
_model_list_item_delete(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Elm_Model_List_Index index = va_arg(*list, Elm_Model_List_Index);
}

static void
_model_list_value_set(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: implement
   Elm_Model_List_Index index = va_arg(*list, Elm_Model_List_Index);
   Eina_Value *value = va_arg(*list, Eina_Value *);
}


#define MY_MODEL_LIST_CLASS ELM_OBJ_MODEL_LIST_CONST_CLASS
EAPI Eo_Op ELM_OBJ_MODEL_LIST_BASE_ID = 0;

static void
_model_list_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND), _model_list_item_append),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND), _model_list_item_prepend),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND_RELATIVE), _model_list_item_append_relative),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND_RELATIVE), _model_list_item_prepend_relative),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_DELETE), _model_list_item_delete),
      EO_OP_FUNC(ELM_OBJ_MODEL_LIST_ID(ELM_OBJ_MODEL_LIST_SUB_ID_VALUE_SET), _model_list_value_set),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description model_list_op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND, "Append new item."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND, "Prepend new item."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_APPEND_RELATIVE, "Append as a sibling node."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_PREPEND_RELATIVE, "Prepend as a sibling node."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_SUB_ID_ITEM_DELETE, "Delete item."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_LIST_SUB_ID_VALUE_SET, "Set value to the node."),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description model_list_class_descs = {
   EO_VERSION,
   "Model List",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_MODEL_LIST_BASE_ID, model_list_op_descs, ELM_OBJ_MODEL_LIST_SUB_ID_LAST),
   NULL, // event_descs
   0, //private data sizeof
   _model_list_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_model_list_class_get, &model_list_class_descs, ELM_OBJ_MODEL_LIST_CONST_CLASS, NULL);

