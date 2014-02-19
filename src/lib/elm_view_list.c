#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

/**
 * @brief Elm View List Class 
 *
 */
#define MY_VIEW_LIST_CLASS ELM_OBJ_VIEW_LIST_CLASS
EAPI Eo_Op ELM_OBJ_VIEW_LIST_BASE_ID = 0;

struct _Elm_View_List_Private
{
   Evas_Object *list;
   Elm_Genlist_Item_Class *itc;
   Eo *model;
};
typedef struct _Elm_View_List_Private Elm_View_List_Private;

struct _View_List_ItemData
{
        Elm_View_List_Private *self;
        Elm_Model_List_Index index;
};
typedef struct _View_List_ItemData View_List_ItemData;


/*/////////////////////////////////////////////////////////////////////////////////
#define _VIEW_TRACE  printf("%s -> %s:%d/n",__FILE__, __FUNCTION__, __LINE__);

static Eina_Bool
_model_item_selected_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
               Elm_Model_List_Index index)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_item_inserted_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
               Elm_Model_List_Index index)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_item_deleted_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
               Elm_Model_List_Index index)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_item_changed_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
               Elm_Model_List_Index index)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_reordered_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info)
{
  _VIEW_TRACE
  return EINA_TRUE;
}
*//////////////////////////////////////////////////////////////////////////////////

////////////////////genlist callbacks
static char *
_item_label_get(void *data, Evas_Object *obj, const char *part)
{
   Eina_Value *value = NULL;
   char *label = NULL;
   View_List_ItemData *idata = data;
   Elm_View_List_Private *self;
   assert(data);
   self = idata->self;
   assert(self);

   if (!strcmp(part, "elm.text"))
   {
      //TODO/FIXME/XXX -  elm_model_list_const
      //eo_do(self->model, elm_model_list_value_get(idata->index, &value));
      label = eina_value_to_string(value); //XXX get value type before??
   }

   return label;
}

static void
_item_del(void *data, Evas_Object *obj, const char *part)
{
   assert(data);
   free(data);
}

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
   printf("sel item data [%p] on genlist obj [%p], item pointer [%p]\n",
          data, obj, event_info);
}

//////////////////////////////////////////////////////////////////////////////////
static inline void
_update_list_widget(Elm_View_List_Private *self)
{
   Elm_Object_Item *item;
   Elm_Model_List_Index i;
   long len = 0;
   assert(self);
   //TODO/FIXME/XXX - elm_model_list_const
   //eo2_do(self->model, len = elm_model_list_len());

   for (i = 0; i < len; i++) {
      View_List_ItemData *data = malloc(sizeof(View_List_ItemData));
      data->self = self;
      data->index = i;

      item = elm_genlist_item_append(self->list, self->itc, data, NULL, ELM_GENLIST_ITEM_NONE, _item_sel_cb, data);
      elm_genlist_item_expanded_set(item, EINA_FALSE);
   }
}


/**
 * @brief Elm View List Class impl.
 *
 */
static void
_elm_view_list_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_List_Private *self = (Elm_View_List_Private *)class_data;
   Evas_Object *parent = va_arg(*list, Evas_Object *);
   Eo *model = va_arg(*list, Eo *);

   assert(self);
   assert(parent);
   assert(model);
   /* eo2_do_super(obj, EO3_GET_CLASS(ELM_VIEW_LIST_CLASS), eo2_constructor()); */
   self->list = elm_genlist_add(parent);
   self->model = model;

   self->itc = elm_genlist_item_class_new();
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_label_get;
   self->itc->func.content_get = NULL;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;

   _update_list_widget(self);
/*
   eo2_do(self->model, elm_model_list_item_selected_callback_add(_model_item_selected_cb, list);
   eo2_do(self->model, elm_model_list_item_inserted_callback_add(_model_item_inserted_cb, list);
   eo2_do(self->model, elm_model_list_item_deleted_callback_add(_model_item_deleted_cb, list);
   eo2_do(self->model, elm_model_list_item_changed_callback_add(_model_item_changed_cb, list);
   eo2_do(self->model, elm_model_list_reordered_callback_add(_model_reordered_cb, list);
*/
}

static void
_elm_view_list_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_List_Private *self = (Elm_View_List_Private *)class_data;
   assert(self);
   assert(obj);
   eo_do_super(obj, MY_VIEW_LIST_CLASS, eo_destructor());
   //XXX destruct evas obj?
}

static Evas_Object*
_elm_view_list_evas_object_get(Eo *obj, Elm_View_List_Private *self)
{
   assert(self);
   assert(obj);
   return self->list;
}

static void
_view_list_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(ELM_OBJ_VIEW_LIST_ID(ELM_OBJ_VIEW_LIST_SUB_ID_LIST_ADD), _elm_view_list_add),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_list_destructor),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}


static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_LIST_SUB_ID_LIST_ADD, "Add new list on parent."),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description model_tree_const_class_descs = {
   EO_VERSION,
   "Model Tree",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_VIEW_LIST_BASE_ID, op_descs, ELM_OBJ_VIEW_LIST_SUB_ID_LIST_LAST),
   NULL,
   sizeof(Elm_View_List_Private),
   _view_list_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_view_list_class_get, &model_tree_const_class_descs, EO_BASE_CLASS, NULL);

/* EO3_DEFINE_CLASS(ELM_VIEW_LIST_CLASS, ((EO3_BASE_CLASS)), Elm_View_List_Private) */
