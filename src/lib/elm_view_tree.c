#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

struct _Elm_View_Tree_Private
{
   Evas_Object *list;
   Elm_Genlist_Item_Class *itc;
   Elm_View_Tree_Mode mode;
   Eo *model;
   Eina_List *items;
   Elm_View_Tree_Content_Get_Cb get_content_cb;
};
typedef struct _Elm_View_Tree_Private Elm_View_Tree_Private;

struct _View_Tree_ItemData
{
  Elm_View_Tree_Private *self;
  Elm_Model_Tree_Path *path;
  Elm_Object_Item *item;
  int children;
};
typedef struct _View_Tree_ItemData View_Tree_ItemData;

static inline void _update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent);
static void _item_sel_cb(void *data, Evas_Object *obj, void *event_info);

Elm_Object_Item *
_get_parent_item(Eina_List *items, Elm_Model_Tree_Path *path)
{
   View_Tree_ItemData *idata;
   Eina_List *l = NULL;
   unsigned int depth, *indices;
   unsigned int pdepth, *pindices;

   indices = elm_model_tree_path_get_indices(path, &depth);
   if (depth == 0 || depth == (unsigned int) -1)
     return NULL;

   EINA_LIST_FOREACH(items, l, idata)
     {
        EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);
        pindices = elm_model_tree_path_get_indices(idata->path, &pdepth);
        if (pdepth == depth-1 && memcmp(indices, pindices, sizeof(unsigned int) * pdepth) == 0)
          return idata->item;
     }

   return NULL;
}

static Eina_Bool
_model_tree_selected_cb(void *data, Elm_Model_Tree_Path *path)
{
   return EINA_TRUE; // TODO implement
}

static Eina_Bool
_model_tree_child_append_cb(void *data, Elm_Model_Tree_Path *path)
{
   Elm_Object_Item *pItem = NULL;
   View_Tree_ItemData *idata = NULL;
   Elm_View_Tree_Private *self = data;

   pItem = _get_parent_item(self->items, path);

   idata = malloc(sizeof(View_Tree_ItemData));
   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, EINA_TRUE);

   idata->self = self;
   idata->path = path;
   idata->children = 0;
   idata->item = elm_genlist_item_append(self->list, self->itc, idata, pItem, ELM_GENLIST_ITEM_NONE, _item_sel_cb, idata);
   elm_genlist_item_expanded_set(idata->item, EINA_FALSE);
   self->items = eina_list_append(self->items, idata);

   if(pItem)
     {
        View_Tree_ItemData *pdata = elm_object_item_data_get(pItem);
        pdata->children++;
        if (pdata && pdata->children == 1)
          elm_genlist_item_update(pItem);
     }

   return EINA_TRUE;
}

/*/////////////////////////////////////////////////////////////////////////////////
static Eina_Bool
_model_node_deleted_cb(void *data, Elm_Model_Tree_Path path)
{
   return EINA_TRUE;
}

static Eina_Bool
_model_node_changed_cb(void *data, Elm_Model_Tree_Path path)
{
   return EINA_TRUE;
}

static Eina_Bool
_model_reordered_cb(void *data)
{
   return EINA_TRUE;
}
*//////////////////////////////////////////////////////////////////////////////////

////////////////////genlist callbacks
static char *
_item_label_get(void *data, Evas_Object *obj, const char *part)
{
   char *label = NULL;
   Eina_Value *value = NULL;
   View_Tree_ItemData *idata = data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);

   if (!strcmp(part, "elm.text"))
     {
        eo2_do(idata->self->model, value = elm_model_tree_value_get(idata->path));
        if (value)
          label = eina_value_to_string(value);
     }

   return label;
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   Elm_View_Tree_Private *self;
   View_Tree_ItemData *idata = data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);
   self = idata->self;
   EINA_SAFETY_ON_NULL_RETURN_VAL(self, NULL);

   if (self->get_content_cb != NULL)
     return self->get_content_cb(self->model, idata->path, obj, part);

   Evas_Object *ic = elm_icon_add(obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ic, NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        if (idata->children == 0)
          elm_icon_standard_set(ic, "file");
        else
          elm_icon_standard_set(ic, "folder");
     }

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
_item_del(void *data, Evas_Object *obj)
{
   free(data);
}

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
   View_Tree_ItemData *idata = data;

   printf("sel item data [%p] on genlist obj [%p], item pointer [%p], path [%s] children [%u]\n",
          data, obj, event_info, elm_model_tree_path_to_string(idata->path), idata->children);
}

////////////////////////////////////////////////////////////////////////////
static inline void
_update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent)
{
   Eina_List *l, *cl = NULL;
   Elm_Model_Tree_Path *c_path;
   View_Tree_ItemData *idata = malloc(sizeof(View_Tree_ItemData));

   EINA_SAFETY_ON_NULL_RETURN(idata);
   idata->self = self;
   idata->path = path;
   idata->children = 0;

   idata->item = elm_genlist_item_append(self->list, self->itc, idata, parent, ELM_GENLIST_ITEM_NONE, _item_sel_cb, idata);
   elm_genlist_item_expanded_set(idata->item, EINA_FALSE);

   self->items = eina_list_append(self->items, idata);
   //foreach model itens and update nodes to widget
   EINA_LIST_FOREACH(cl, l, c_path)
     {
        _update_path(self, c_path, idata->item);
     }
}

static inline void
_update_tree_widget(Elm_View_Tree_Private* self)
{
   Elm_Model_Tree_Path *path = elm_model_tree_path_new_from_string("");
   _update_path(self, path, NULL);
}

static void
_elm_view_tree_add(Eo *obj, Elm_View_Tree_Private *self, Evas_Object* parent, Eo* model)
{
   eo2_do_super(obj, EO3_GET_CLASS(ELM_VIEW_TREE_CLASS), eo2_constructor());
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(model);
   EINA_SAFETY_ON_NULL_RETURN(parent);

   self->list = elm_genlist_add(parent);
   EINA_SAFETY_ON_NULL_RETURN(self->list);
   self->model = model;
   self->mode = ELM_VIEW_TREE_VIEWMODE_ALL;
   self->get_content_cb = NULL;

   self->itc = elm_genlist_item_class_new();
   EINA_SAFETY_ON_NULL_RETURN(self->itc);
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_label_get;
   self->itc->func.content_get = _item_content_get;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;

   _update_tree_widget(self);
   eo2_do(self->model, elm_model_tree_select_callback_add(self, _model_tree_selected_cb));
   eo2_do(self->model, elm_model_tree_child_append_callback_add(self, _model_tree_child_append_cb));
/*
   eo2_do(self->model, elm_model_tree_node_deleted_callback_add(_model_node_deleted_cb, list);
   eo2_do(self->model, elm_model_tree_node_changed_callback_add(_model_node_changed_cb, list);
   eo2_do(self->model, elm_model_tree_reordered_callback_add(_model_reordered_cb, list);
*/
}

static void
_elm_view_tree_destructor(Eo *obj, Elm_View_Tree_Private *self)
{
   elm_genlist_item_class_free(self->itc);
   //XXX destruct evas obj?
}

static Evas_Object*
_elm_view_tree_evas_object_get(Eo *obj, Elm_View_Tree_Private *self)
{
   return self->list;
}

static void
_elm_view_tree_mode_set(Eo *obj, Elm_View_Tree_Private *self, Elm_View_Tree_Mode mode)
{
   if (self->mode != mode)
     {
        self->mode = mode;
        _update_tree_widget(self);
     }
}

static void
_elm_view_tree_getcontent_set(Eo *obj, Elm_View_Tree_Private *self, Elm_View_Tree_Content_Get_Cb get_content_cb)
{
   self->get_content_cb = get_content_cb;
}

EO3_DEFINE_CLASS(ELM_VIEW_TREE_CLASS, ((EO3_BASE_CLASS)), Elm_View_Tree_Private)
