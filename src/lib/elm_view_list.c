#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_view_list.h"
#include "elm_priv.h"
#include <assert.h>

struct _Elm_View_List_Private
{
   Evas_Object *list;
   Eo *model;
};
typedef struct _Elm_View_List_Private Elm_View_List_Private;

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

static inline void
_update_list_widget(Elm_View_List_Private *self)
{
   assert(self);
   //foreach model itens and update items to widget
   //XXX emit update/reorder signal
}

static void
_elm_view_list_add(Eo *obj, Elm_View_List_Private *self, Evas_Object* parent, Eo* model)
{
   assert(self);
   assert(parent);
   assert(model);
   eo2_do_super(obj, EO3_GET_CLASS(ELM_VIEW_LIST_CLASS), eo2_constructor());
   self->list = elm_genlist_add(parent);
   self->model = model;
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
_elm_view_list_destructor(Eo *obj, Elm_View_List_Private *self)
{
   assert(self);
   assert(obj);
   //XXX destruct evas obj?
}

static Evas_Object*
_elm_view_list_evas_object_get(Eo *obj, Elm_View_List_Private *self)
{
   assert(self);
   assert(obj);
   return self->list;
}

EO3_DEFINE_CLASS(ELM_VIEW_LIST_CLASS, ((EO3_BASE_CLASS)), Elm_View_List_Private)
