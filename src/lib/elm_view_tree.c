#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_view_tree.h"
#include "elm_priv.h"
#include <assert.h>

struct _Elm_View_Tree_Private
{
   Evas_Object *list;
   Eo *model;
   Elm_Genlist_Item_Class *itc;
   Elm_Genlist_Item_Class *itp;
   Elm_View_Tree_Mode mode;
};
typedef struct _Elm_View_Tree_Private Elm_View_Tree_Private;

#define _VIEW_TRACE  printf("%s -> %s:%d/n",__FILE__, __FUNCTION__, __LINE__);

static Eina_Bool
_model_selected_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Tree_Path *path)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

/*/////////////////////////////////////////////////////////////////////////////////
static Eina_Bool
_model_node_inserted_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Tree_Path path)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_node_deleted_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Tree_Path path)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_node_changed_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Tree_Path path)
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
   char buf[256];
   Elm_Model_Tree_Path *path = data;
   Eina_Value *value;

   if (!strcmp(part, "elm.text"))
   {
//     value = eo2_do(obj, elm_model_tree_value_get(path)); //get self and path
     snprintf(buf, sizeof(buf), "%s", value); //cast Eina_Value to string
   }

   return strdup(buf);
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "file");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static Evas_Object *
_expandable_content_get(void *data, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "folder");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

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
   assert(self);
   assert(path);

   Eina_List *l, *cl;
   Elm_Model_Tree_Path *c_path;

   eo2_do(self->model, cl = elm_model_tree_children_get(path));
   Elm_Object_Item *item;

   if (!cl) {
      item = elm_genlist_item_append(self->list, self->itc, path, parent, ELM_GENLIST_ITEM_NONE, _item_sel_cb, NULL);
      elm_genlist_item_expanded_set(item, EINA_FALSE);
      return;
   }

   item = elm_genlist_item_append(self->list, self->itp, path, parent, ELM_GENLIST_ITEM_TREE, _item_sel_cb, NULL);
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
}

static void
_elm_view_tree_add(Eo *obj, Elm_View_Tree_Private *self, Evas_Object* parent, Eo* model)
{
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
   self->itc->func.del = NULL;

   self->itp = elm_genlist_item_class_new();
   self->itp->item_style = "default";
   self->itp->func.text_get = _item_label_get;
   self->itp->func.content_get = _expandable_content_get;
   self->itp->func.state_get = NULL;
   self->itp->func.del = NULL;

   _update_tree_widget(self);
/*
   eo2_do(self->model, elm_model_tree_select_callback_add(_model_selected_cb, self));
   eo2_do(self->model, elm_model_tree_node_inserted_callback_add(_model_node_inserted_cb, list);
   eo2_do(self->model, elm_model_tree_node_deleted_callback_add(_model_node_deleted_cb, list);
   eo2_do(self->model, elm_model_tree_node_changed_callback_add(_model_node_changed_cb, list);
   eo2_do(self->model, elm_model_tree_reordered_callback_add(_model_reordered_cb, list);
*/
}

static void
_elm_view_tree_destructor(Eo *obj, Elm_View_Tree_Private *self)
{
   assert(self);
   assert(obj);
   //XXX destruct evas obj?
   //XXX destruct item_class
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
