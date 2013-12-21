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
};
typedef struct _Elm_View_Tree_Private Elm_View_Tree_Private;

struct _View_Tree_ItemData
{
        Elm_View_Tree_Private *self;
        Elm_Model_Tree_Path *path;
};
typedef struct _View_Tree_ItemData View_Tree_ItemData;


static inline void _update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent);
#define _VIEW_TRACE  printf("**%s -> %s:%d**\n",__FILE__, __FUNCTION__, __LINE__);

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
   Elm_Model_Tree_Path *root_path = elm_model_tree_path_new_from_string("");
   _update_path(data, root_path, NULL);
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
   View_Tree_ItemData *idata = data;
   Elm_View_Tree_Private *self;
   assert(data);
   assert(obj);
   assert(part);
   self = idata->self;
   assert(self);

   if (!strcmp(part, "elm.text"))
   {
      eo2_do(self->model, value = elm_model_tree_value_get(idata->path));
      label = eina_value_to_string(value); //XXX get value type before??
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

/*
static void
_item_del(void *data, Evas_Object *obj, const char *part)
{
  _VIEW_TRACE
   assert(data);
   free(data);
  _VIEW_TRACE
}
*/

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
   printf("sel item data [%p] on genlist obj [%p], item pointer [%p]\n",
          data, obj, event_info);
}


////////////////////////////////////////////////////////////////////////////


static inline void
_update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent)
{
  _VIEW_TRACE
   assert(self);
   assert(path);

   Eina_List *l, *cl = NULL;
   Elm_Model_Tree_Path *c_path;
   View_Tree_ItemData *idata = malloc(sizeof(View_Tree_ItemData));
   idata->self = self;
   idata->path = path;

   eo2_do(self->model, cl = elm_model_tree_children_get(path));
   Elm_Object_Item *item;

   if (!cl) {
      item = elm_genlist_item_append(self->list, self->itc, idata, parent, ELM_GENLIST_ITEM_NONE, _item_sel_cb, idata);
      elm_genlist_item_expanded_set(item, EINA_FALSE);
      return;
   }

   item = elm_genlist_item_append(self->list, self->itp, idata, parent, ELM_GENLIST_ITEM_TREE, _item_sel_cb, idata);
   assert(item);
   elm_genlist_item_expanded_set(item, EINA_TRUE);

   //foreach model itens and update nodes to widget
   EINA_LIST_FOREACH(cl, l, c_path)
   {
      _update_path(self, c_path, item);
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
//   self->itc->func.del = _item_del;

   self->itp = elm_genlist_item_class_new();
   self->itp->item_style = "default";
   self->itp->func.text_get = _item_label_get;
   self->itp->func.content_get = _expandable_content_get;
   self->itp->func.state_get = NULL;
//   self->itp->func.del = _item_del;

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
   //XXX destruct evas obj?
   //XXX destruct item_class?
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
