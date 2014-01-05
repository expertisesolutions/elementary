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
   Elm_Genlist_Item_Class *itp;
   Elm_View_Tree_Mode mode;
   Eo *model;
   Eina_List *items;
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
static inline void _add_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path);
#define _VIEW_TRACE  printf("**%s -> %s:%d**\n",__FILE__, __FUNCTION__, __LINE__);


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

   printf("Child add depth=%u\n", depth);

   EINA_LIST_FOREACH(items, l, idata)
   {
      assert(idata);
      pindices = elm_model_tree_path_get_indices(idata->path, &pdepth);
      if (pdepth == depth-1 && memcmp(indices, pindices, sizeof(unsigned int) * pdepth) == 0)
      {
         _VIEW_TRACE
         return idata->item;
      }
   }

   return NULL;
}


static Eina_Bool
_model_tree_selected_cb(void *data, Elm_Model_Tree_Path *path)
{
   assert(data);
   assert(path);
   _VIEW_TRACE
   return EINA_TRUE;
}

static Eina_Bool
_model_tree_child_append_cb(void *data, Elm_Model_Tree_Path *path)
{
   assert(data);
   assert(path);

   Elm_View_Tree_Private *self = data;

   _add_path(self, path);
   _VIEW_TRACE
   return EINA_TRUE;
}

/*/////////////////////////////////////////////////////////////////////////////////
static Eina_Bool
_model_node_deleted_cb(void *data, Elm_Model_Tree_Path path)
{
   _VIEW_TRACE
   return EINA_TRUE;

   }

static Eina_Bool
_model_node_changed_cb(void *data, Elm_Model_Tree_Path path)
{
   _VIEW_TRACE
   return EINA_TRUE;
}

static Eina_Bool
_model_reordered_cb(void *data)
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
   assert(obj);
   assert(part);
   assert(data);

   View_Tree_ItemData *idata = data;
   Elm_View_Tree_Private *self = idata->self;

   if (!strcmp(part, "elm.text"))
   {
      eo2_do(self->model, value = elm_model_tree_value_get(idata->path));
      if (value) label = eina_value_to_string(value);
   }

   return label;
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   assert(data);
   assert(obj);
   assert(part);
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "file");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static Evas_Object *
_expandable_content_get(void *data, Evas_Object *obj, const char *part)
{
   assert(data);
   assert(obj);
   assert(part);
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "folder");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
_item_del(void *data, Evas_Object *obj)
{
   assert(obj);
   free(data);
}

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
   printf("sel item data [%p] on genlist obj [%p], item pointer [%p]\n",
          data, obj, event_info);
}


////////////////////////////////////////////////////////////////////////////
static inline void
_add_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path)
{
   assert(self);
   assert(path);

   Elm_Object_Item *pItem = _get_parent_item(self->items, path);
   View_Tree_ItemData *idata = malloc(sizeof(View_Tree_ItemData));
   memset(idata, 0, sizeof(View_Tree_ItemData));
   idata->self = self;
   idata->path = path;
   idata->children = 0;
   idata->item = elm_genlist_item_append(self->list, self->itc, idata, pItem, ELM_GENLIST_ITEM_NONE, _item_sel_cb, idata);
   elm_genlist_item_expanded_set(idata->item, EINA_FALSE);
   self->items = eina_list_append(self->items, idata);

 /*XXX segfault if update item class

   if (pItem) {
      View_Tree_ItemData *pdata = elm_object_item_data_get(pItem);
      if (pdata && pdata->children == 0) {
         elm_genlist_item_item_class_update(pItem, self->itp);
         elm_genlist_item_expanded_set(pdata->item, EINA_TRUE);
      }
      pdata->children++;
   }
   */
}

static inline void
_update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent)
{
   assert(self);
   assert(path);

   Eina_List *l, *cl = NULL;
   Elm_Model_Tree_Path *c_path;
   View_Tree_ItemData *idata = malloc(sizeof(View_Tree_ItemData));
   memset(idata, 0, sizeof(View_Tree_ItemData));

   idata->self = self;
   idata->path = path;
   idata->children = 0;

   eo2_do(self->model, cl = elm_model_tree_children_get(path)); //XXX need implement children_get in model_tree

   if (!cl) {
      idata->item = elm_genlist_item_append(self->list, self->itc, idata, parent, ELM_GENLIST_ITEM_NONE, _item_sel_cb, idata);
      elm_genlist_item_expanded_set(idata->item, EINA_FALSE);
   } else {
      idata->children = eina_list_count(cl);
      idata->item = elm_genlist_item_append(self->list, self->itp, idata, parent, ELM_GENLIST_ITEM_TREE, _item_sel_cb, idata);
      elm_genlist_item_expanded_set(idata->item, EINA_TRUE);
   }

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
   assert(self);
   Elm_Model_Tree_Path *path = elm_model_tree_path_new_from_string("");
   _update_path(self, path, NULL);
   _VIEW_TRACE
}

static void
_elm_view_tree_add(Eo *obj, Elm_View_Tree_Private *self, Evas_Object* parent, Eo* model)
{
   _VIEW_TRACE
   assert(self);
   assert(parent);
   assert(model);
   eo2_do_super(obj, EO3_GET_CLASS(ELM_VIEW_TREE_CLASS), eo2_constructor());
   self->list = elm_genlist_add(parent);
   self->model = model;
   self->mode = ELM_VIEW_TREE_VIEWMODE_ALL;

   self->itc = elm_genlist_item_class_new();
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_label_get;
   self->itc->func.content_get = _item_content_get;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;

   self->itp = elm_genlist_item_class_new();
   self->itp->item_style = "default";
   self->itp->func.text_get = _item_label_get;
   self->itp->func.content_get = _expandable_content_get;
   self->itp->func.state_get = NULL;
   self->itp->func.del = _item_del;

   _update_tree_widget(self);
   eo2_do(self->model, elm_model_tree_select_callback_add(self, _model_tree_selected_cb));
   eo2_do(self->model, elm_model_tree_child_append_callback_add(self, _model_tree_child_append_cb));
/*
   eo2_do(self->model, elm_model_tree_node_deleted_callback_add(_model_node_deleted_cb, list);
   eo2_do(self->model, elm_model_tree_node_changed_callback_add(_model_node_changed_cb, list);
   eo2_do(self->model, elm_model_tree_reordered_callback_add(_model_reordered_cb, list);
*/
   _VIEW_TRACE
}

static void
_elm_view_tree_destructor(Eo *obj, Elm_View_Tree_Private *self)
{
   assert(self);
   assert(obj);
   elm_genlist_item_class_free(self->itc);
   //XXX destruct evas obj?
}

static Evas_Object*
_elm_view_tree_evas_object_get(Eo *obj, Elm_View_Tree_Private *self)
{
   assert(self);
   assert(obj);
   return self->list;
}

static void
_elm_view_tree_mode_set(Eo *obj, Elm_View_Tree_Private *self, Elm_View_Tree_Mode mode)
{
   assert(self);
   assert(mode);
   assert(obj);
   if (self->mode != mode)
   {
      self->mode = mode;
      _update_tree_widget(self);
   }
}

EO3_DEFINE_CLASS(ELM_VIEW_TREE_CLASS, ((EO3_BASE_CLASS)), Elm_View_Tree_Private)
