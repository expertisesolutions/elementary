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
   Elm_View_Tree_Mode mode;
};
typedef struct _Elm_View_Tree_Private Elm_View_Tree_Private;

/*/////////////////////////////////////////////////////////////////////////////////
#define _VIEW_TRACE  printf("%s -> %s:%d/n",__FILE__, __FUNCTION__, __LINE__);

static Eina_Bool
_model_node_selected_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Tree_Path path)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

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

static inline void
_update_tree_widget(Elm_View_Tree_Private* self)
{
   assert(self);
   //foreach model itens and update nodes to widget
   //XXX emit update/reorder signal
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
   _update_tree_widget(self);
/*
   eo2_do(self->model, elm_model_tree_node_selected_callback_add(_model_node_selected_cb, list);
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
