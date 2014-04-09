#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Eo.h>
#include <Emodel.h>
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

/**
 * @brief Elm View List Class
 *
 */
EAPI Eo_Op ELM_VIEW_LIST_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_VIEW_LIST_CLASS
#define MY_CLASS_NAME "View List"

struct _Elm_View_List_Private;
typedef struct _Elm_View_List_Private Elm_View_List_Private;

struct _View_List_ItemData;
typedef struct _View_List_ItemData View_List_ItemData;

struct _Elm_View_List_Private
{
   Evas_Object *list;
   View_List_ItemData *rootdata;
   Elm_Genlist_Item_Class *itc;
   Eo *model;
};

struct _View_List_ItemData
{
  Elm_View_List_Private *self;
  Elm_Object_Item *item;
  Eo *model;
  Eina_Hash *parts;
  int index;
};

static void _emodel_child_get(void *data, Eo *child, void *event_info);

/* --- Genlist Callbacks --- */
static void
_item_sel_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   View_List_ItemData *idata = elm_object_item_data_get(item);
   EINA_SAFETY_ON_NULL_RETURN(idata);
}

static void
_item_del(void *data, Evas_Object *obj EINA_UNUSED)
{
   free(data);
}

static void
_hash_free(void *data)
{
   eina_value_free(data);
}


static char *
_item_text_get(void *data, Evas_Object *obj, const char *part)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, NULL);
   View_List_ItemData *idata = data;
   Elm_View_List_Private *self = idata->self;
   Eina_Value *value;

   if (idata->parts == NULL)
     {
        idata->parts = eina_hash_string_superfast_new(_hash_free);
        eina_hash_add(idata->parts, part, eina_value_new(EINA_VALUE_TYPE_STRING));
        //FIXME get parent model
        eo_do(self->model, emodel_children_slice_get(_emodel_child_get, idata->index, 1, idata));
        return NULL;
     }

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata->model, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(idata->parts, NULL);
   value = eina_hash_find(idata->parts, part);
   if (value)
     {
        char *text = eina_value_to_string(value);
        if (text)
          {
              return strdup(text);
          }
        return NULL;
     }

   if (idata->model)
        eo_do(idata->model, emodel_property_get(part));

   eina_hash_add(idata->parts, part, eina_value_new(EINA_VALUE_TYPE_STRING));
   return NULL;
}

/* --- Emodel Callbacks --- */
static Eina_Bool
_emodel_property_change_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Eina_Value *value = NULL;
   View_List_ItemData *idata = data;
   Emodel_Property_EVT *property = event_info;

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, EINA_TRUE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(property, EINA_TRUE);

   value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_copy(property->value, value);
   eina_hash_set(idata->parts, property->prop, value);

   if (idata->item) {
      elm_genlist_item_update(idata->item);
   }

   return EINA_TRUE;
}

static Eina_Bool
_emodel_properties_change_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   return EINA_TRUE;
}

static Eina_Bool
_emodel_children_count_get_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   View_List_ItemData *pdata = data;
   unsigned int i, *len = event_info;
   Elm_View_List_Private *self = pdata->self;

   for (i = 0; i < *len; i++)
     {
        View_List_ItemData *idata = malloc(sizeof(View_List_ItemData));
        memset(idata, 0, sizeof(View_List_ItemData));
        EINA_SAFETY_ON_NULL_RETURN_VAL(idata, EINA_TRUE);
        idata->self = self;
        idata->index = i;
        idata->item = elm_genlist_item_append(self->list, self->itc, idata, NULL,
                                                       ELM_GENLIST_ITEM_NONE, _item_sel_cb, NULL);
     }

   return EINA_TRUE;
}


Eina_Bool
_get_parts_fn(const Eina_Hash *hash EINA_UNUSED, const void *key, void *data EINA_UNUSED, void *fdata)
{
   View_List_ItemData *idata = fdata;
   const char *part = key;

   eo_do(idata->model, emodel_property_get(part));
   return EINA_TRUE;
}

static void
_emodel_child_get(void *data, Eo *child, void *event_info EINA_UNUSED)
{
   View_List_ItemData *idata = data;
   idata->model = child;
   eo_do(idata->model, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _emodel_property_change_cb, idata));
   eo_do(idata->model, eo_event_callback_add(EMODEL_PROPERTIES_CHANGE_EVT, _emodel_properties_change_cb, idata));

   EINA_SAFETY_ON_NULL_RETURN(idata);
   if (idata->parts)
     eina_hash_foreach(idata->parts, _get_parts_fn, idata);
}

/**
 * @brief Elm View List Class impl.
 */
static void
_elm_view_list_add(Eo *obj, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
   Elm_View_List_Private *self = (Elm_View_List_Private *)class_data;

   Evas_Object *parent = va_arg(*list, Evas_Object *);
   EINA_SAFETY_ON_NULL_RETURN(parent);

   self->model = va_arg(*list, Eo *);
   EINA_SAFETY_ON_NULL_RETURN(self->model);
   eo_ref(self->model);

   self->rootdata = malloc(sizeof(View_List_ItemData));
   self->rootdata->self = self;
   self->rootdata->model = self->model;

   self->list = elm_genlist_add(parent);

   self->itc = elm_genlist_item_class_new();
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_text_get;
   self->itc->func.content_get = NULL;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;

   eo_do(self->model, eo_event_callback_add(EMODEL_CHILDREN_COUNT_GET_EVT, _emodel_children_count_get_cb, self->rootdata));
   eo_do(self->model, emodel_children_count_get());

   evas_object_size_hint_weight_set(self->list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(parent, self->list);
   evas_object_show(self->list);
}

static void
_elm_view_list_destructor(Eo *obj, void *class_data, va_list *list EINA_UNUSED)
{
   Elm_View_List_Private *self = (Elm_View_List_Private *)class_data;
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   elm_genlist_item_class_free(self->itc);
   free(self->rootdata);
   self->rootdata = NULL;
   eo_unref(self->model);

   eo_do_super(obj, MY_CLASS, eo_destructor());
}

static void
_elm_view_list_evas_object_get(Eo *obj, void *class_data, va_list *list)
{
   Elm_View_List_Private *self = class_data;
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   EINA_SAFETY_ON_NULL_RETURN(list);
   Evas_Object **e = va_arg(*list, Evas_Object **);
   *e = self->list;
}

static void
_view_list_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_list_add),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_list_destructor),
      EO_OP_FUNC(ELM_VIEW_LIST_ID(ELM_OBJ_VIEW_LIST_SUB_ID_EVAS_OBJECT_GET), _elm_view_list_evas_object_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}


static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_LIST_SUB_ID_EVAS_OBJECT_GET, "Return Evas object list"),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description view_list_class_descs = {
   EO_VERSION,
   MY_CLASS_NAME,
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_VIEW_LIST_BASE_ID, op_descs, ELM_OBJ_VIEW_LIST_SUB_ID_LAST),
   NULL,
   sizeof(Elm_View_List_Private),
   _view_list_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_view_list_class_get, &view_list_class_descs, EO_BASE_CLASS, NULL);
