#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Eo.h>
#include <Emodel.h>
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

#define MY_CLASS ELM_VIEW_LIST_CLASS
#define MY_CLASS_NAME "View List"

struct _Elm_View_List_Data;
typedef struct _Elm_View_List_Data Elm_View_List_Data;

struct _View_List_ItemData;
typedef struct _View_List_ItemData View_List_ItemData;

struct _Elm_View_List_Data
{
   Evas_Object *genlist;
   View_List_ItemData *rootdata;
   Elm_Genlist_Item_Class *itc;
   Eina_Hash *prop_con;
   Eo *model;
};

struct _View_List_ItemData
{
  Elm_View_List_Data *priv;
  Elm_Object_Item *item;
  Eo *model;
  Eina_Hash *parts;
  View_List_ItemData *parent;
  int index;
};


static void _emodel_child_get(void *data, Eo *child, void *event_info, int error);
static Eina_Bool _emodel_children_count_get_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info);

/* --- Genlist Callbacks --- */
static void
_item_sel_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   View_List_ItemData *idata = elm_object_item_data_get(item);
   EINA_SAFETY_ON_NULL_RETURN(idata);
   eo_do(idata->parent->model, emodel_child_select(idata->model));
}

static void
_item_del(void *data, Evas_Object *obj EINA_UNUSED)
{
   View_List_ItemData *idata = data;
   eo_unref(idata->model);
   eina_hash_free(idata->parts);
   free(data);
}

static void
_hash_free(void *data)
{
   eina_value_free(data);
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part)
{
   //XXX: Add a Custon content function??
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(part, NULL);
   View_List_ItemData *idata = data;
   Elm_View_List_Data *priv = idata->priv;
   Eina_Value *value;

   const char *prop = eina_hash_find(priv->prop_con, part);
   if (prop == NULL) prop = part;

   if (idata->parts == NULL)
     {
        idata->parts = eina_hash_string_superfast_new(_hash_free);
        eina_hash_add(idata->parts, prop, eina_value_new(EINA_VALUE_TYPE_STRING));
        if (idata->parent)
              eo_do(idata->parent->model, emodel_children_slice_fetch(_emodel_child_get, idata->index, 1, idata));
        return NULL;
     }

   if (idata->model == NULL)
        return NULL;

   value = eina_hash_find(idata->parts, prop);
   if (!value)
     {
        eina_hash_add(idata->parts, prop, eina_value_new(EINA_VALUE_TYPE_STRING));
        eo_do(idata->model, emodel_prop_fetch(prop));
        return NULL;
     }

   const char *content_s = eina_value_to_string(value);
   if (!content_s)
      return NULL;

   Evas_Object *ic = NULL;
   if (strncmp("icon ", content_s, 5) == 0)
     {
        content_s += 5;
        ic = elm_icon_add(obj);
        elm_icon_standard_set(ic, content_s);
        evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
     }
   else if (strncmp("photo ", content_s, 6) == 0)
     {
        content_s += 6;
        ic = elm_icon_add(obj);
        elm_photo_file_set(ic, content_s);
     }

   return ic;
}

static char *
_item_text_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(part, NULL);
   View_List_ItemData *idata = data;
   Elm_View_List_Data *priv = idata->priv;
   Eina_Value *value;

   const char *prop = eina_hash_find(priv->prop_con, part);
   if (prop == NULL) prop = part;

   if (idata->parts == NULL)
     {
        idata->parts = eina_hash_string_superfast_new(_hash_free);
        eina_hash_add(idata->parts, prop, eina_value_new(EINA_VALUE_TYPE_STRING));
        if (idata->parent)
              eo_do(idata->parent->model, emodel_children_slice_fetch(_emodel_child_get, idata->index, 1, idata));
        return NULL;
     }

   if (idata->model == NULL)
     return NULL;

   value = eina_hash_find(idata->parts, prop);
   if (value)
     {
        char *text = eina_value_to_string(value);
        if (text)
          {
              //FIXME: don't cache
              //eina_hash_del_by_key(idata->parts, part);
              //eina_value_free(value);
              return strdup(text);
          }
        return NULL;
     }

   eo_do(idata->model, emodel_prop_fetch(prop));
   eina_hash_add(idata->parts, prop, eina_value_new(EINA_VALUE_TYPE_STRING));
   return NULL;
}

static void
_expand_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   View_List_ItemData *idata = elm_object_item_data_get(item);
   if (idata->model)
     {
        eo_do(idata->model, eo_event_callback_add(EMODEL_EVENT_CHILDREN_COUNT_GET, _emodel_children_count_get_cb, idata));
        eo_do(idata->model, emodel_children_count_get());
     }
}

static void
_contract_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   elm_genlist_item_expanded_set(item, EINA_FALSE);
}

static void
_contracted_cb(void *data EINA_UNUSED, Evas_Object *o EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
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

   value = eina_hash_find(idata->parts, property->prop);
   if (value)
     {
        if (eina_value_type_get(property->value) == EINA_VALUE_TYPE_BLOB &&
                        eina_value_type_get(value) == EINA_VALUE_TYPE_BLOB)
          {
              Eina_Value_Blob blob;
              eina_value_get(property->value, &blob);
              eina_value_set(value, blob);
          }
        else
          {
              eina_value_copy(property->value, value);
          }

        if (idata->item)
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
   Elm_View_List_Data *priv = pdata->priv;

   for (i = 0; i < *len; i++)
     {
        View_List_ItemData *idata = malloc(sizeof(View_List_ItemData));
        memset(idata, 0, sizeof(View_List_ItemData));
        EINA_SAFETY_ON_NULL_RETURN_VAL(idata, EINA_TRUE);
        idata->priv = priv;
        idata->index = i;
        idata->parent = pdata;
        idata->item = elm_genlist_item_append(priv->genlist, priv->itc, idata, pdata->item,
                                                       ELM_GENLIST_ITEM_TREE, _item_sel_cb, NULL);
     }

   if (pdata->item && *len > 0)
     {
        elm_genlist_item_expanded_set(pdata->item, EINA_TRUE);
     }

   return EINA_FALSE;
}


Eina_Bool
_get_parts_fn(const Eina_Hash *hash EINA_UNUSED, const void *key, void *data EINA_UNUSED, void *fdata)
{
   View_List_ItemData *idata = fdata;
   const char *part = key;

   eo_do(idata->model, emodel_prop_fetch(part));
   return EINA_TRUE;
}

static void
_emodel_child_get(void *data, Eo *child, void *event_info EINA_UNUSED, int error EINA_UNUSED)
{
   View_List_ItemData *idata = data;
   idata->model = child;
   eo_do(idata->model, eo_event_callback_add(EMODEL_EVENT_PROPERTY_CHANGE, _emodel_property_change_cb, idata));
   eo_do(idata->model, eo_event_callback_add(EMODEL_EVENT_PROPERTIES_CHANGE, _emodel_properties_change_cb, idata));

   EINA_SAFETY_ON_NULL_RETURN(idata);
   if (idata->parts)
     eina_hash_foreach(idata->parts, _get_parts_fn, idata);
}

static void
_priv_model_set(Elm_View_List_Data *priv, Eo *model)
{
   if (priv->model != NULL)
     {
         eo_do(priv->genlist, elm_obj_genlist_clear());
         eo_unref(priv->model);
         priv->model = NULL;
         free(priv->rootdata);
         priv->rootdata = NULL;
     }

   if (model == NULL)
     return;

   priv->model = model;
   eo_ref(priv->model);

   priv->rootdata = malloc(sizeof(View_List_ItemData));
   memset(priv->rootdata, 0, sizeof(View_List_ItemData));
   priv->rootdata->priv = priv;
   priv->rootdata->model = priv->model;

   eo_do(priv->model, eo_event_callback_add(EMODEL_EVENT_CHILDREN_COUNT_GET, _emodel_children_count_get_cb, priv->rootdata));
   eo_do(priv->model, emodel_children_count_get());
}


static void
_elm_view_list_eo_base_constructor(Eo *obj, Elm_View_List_Data *_pd EINA_UNUSED)
{
   eo_error_set(obj);
   fprintf(stderr, "only custom constructor can be used with '%s' class", MY_CLASS_NAME);
}

/**
 * @brief Elm View List Class impl.
 */
static void
_elm_view_list_constructor(Eo *obj, Elm_View_List_Data *_pd, Evas_Object *genlist, Eo *model)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
   Elm_View_List_Data *priv = (Elm_View_List_Data *)_pd;

   priv->genlist = genlist;
   EINA_SAFETY_ON_NULL_RETURN(priv->genlist);
   //eo_ref(priv->genlist);

   _priv_model_set(priv, model);

   priv->itc = elm_genlist_item_class_new();
   priv->itc->item_style = "default";
   priv->itc->func.text_get = _item_text_get;
   priv->itc->func.content_get = _item_content_get;
   priv->itc->func.state_get = NULL;
   priv->itc->func.del = _item_del;
   priv->prop_con = eina_hash_string_superfast_new(free);

   evas_object_smart_callback_add(priv->genlist, "expand,request", _expand_request_cb, priv);
   evas_object_smart_callback_add(priv->genlist, "contract,request", _contract_request_cb, priv);
   evas_object_smart_callback_add(priv->genlist, "contracted", _contracted_cb, priv);
}

static void
_elm_view_list_eo_base_destructor(Eo *obj, Elm_View_List_Data *_pd)
{
   Elm_View_List_Data *priv = (Elm_View_List_Data *)_pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   eo_do(priv->genlist, elm_obj_genlist_clear());
   elm_genlist_item_class_free(priv->itc);
   //evas_object_smart_callback_del(priv->genlist, "expand,request", _expand_request_cb);
   //evas_object_smart_callback_del(priv->genlist, "contract,request", _contract_request_cb);
   //evas_object_smart_callback_del(priv->genlist, "contracted", _contracted_cb);

   free(priv->rootdata);
   priv->rootdata = NULL;
   //eo_unref(priv->genlist);
   eo_unref(priv->model);

   eo_do_super(obj, MY_CLASS, eo_destructor());
}

static void
_elm_view_list_evas_object_get(Eo *obj, Elm_View_List_Data *_pd, Evas_Object **widget)
{
   Elm_View_List_Data *priv = _pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(obj);
   EINA_SAFETY_ON_NULL_RETURN(widget);

   *widget = priv->genlist;
}

static void
_elm_view_list_property_connect(Eo *obj EINA_UNUSED, Elm_View_List_Data *_pd, const char *property, const char *part)
{
   Elm_View_List_Data *priv = (Elm_View_List_Data *)_pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);

   EINA_SAFETY_ON_NULL_RETURN(priv->prop_con);
   EINA_SAFETY_ON_NULL_RETURN(property);
   EINA_SAFETY_ON_NULL_RETURN(part);

   free(eina_hash_set(priv->prop_con, part, strdup(property)));
}

static void
_elm_view_list_model_set(Eo *obj EINA_UNUSED, Elm_View_List_Data *_pd, Eo *model)
{
   Elm_View_List_Data *priv = (Elm_View_List_Data *)_pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(model);
   _priv_model_set(priv, model);
}

static void
_elm_view_list_model_get(Eo *obj EINA_UNUSED, Elm_View_List_Data *_pd, Eo **model)
{
   Elm_View_List_Data *priv = (Elm_View_List_Data *)_pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(model);
   *model = priv->model;
}
#include "elm_view_list.eo.c"
