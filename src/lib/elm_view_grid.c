#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

struct _Elm_View_Grid_Private
{
   Evas_Object *list;
   Eo *model;
};
typedef struct _Elm_View_Grid_Private Elm_View_Grid_Private;

///////////////////////////////////////////////////////////////////////////////////
#define _VIEW_TRACE  printf("%s -> %s:%d/n",__FILE__, __FUNCTION__, __LINE__);

static Eina_Bool
_model_cell_selected_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Grid_Row row, Elm_Model_Grid_Column column)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_row_inserted_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Grid_Row row)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_row_deleted_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Grid_Row row)
{
  _VIEW_TRACE
  return EINA_TRUE;
}

static Eina_Bool
_model_row_changed_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info,
                Elm_Model_Grid_Row row)
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
///////////////////////////////////////////////////////////////////////////////////

static inline void
_update_grid_widget(Eo* model, Evas_Object *list)
{
   //foreach model itens and update rows to widget
}

static void
_elm_view_grid_add(Eo *obj, Elm_View_Grid_Private *self, Evas_Object* parent, Eo* model)
{
   assert(self);
   assert(parent);
   assert(model);
   eo2_do_super(obj, EO3_GET_CLASS(ELM_VIEW_GRID_CLASS), eo2_constructor());
   self->list = elm_genlist_add(parent);
   self->model = model;
   _update_widget(self->model, self->list);
   eo2_do(self->model, elm_model_grid_row_selected_callback_add(_model_row_selected_cb, list);
   eo2_do(self->model, elm_model_grid_row_inserted_callback_add(_model_row_inserted_cb, list);
   eo2_do(self->model, elm_model_grid_row_deleted_callback_add(_model_row_deleted_cb, list);
   eo2_do(self->model, elm_model_grid_row_changed_callback_add(_model_row_changed_cb, list);
   eo2_do(self->model, elm_model_grid_reordered_callback_add(_model_reordered_cb, list);

  eo2_do_super(obj, EO3_GET_CLASS(ELM_VIEW_GRID_CLASS), eo2_constructor());
}

static void
_elm_view_grid_destructor(Eo *obj, Elm_View_Grid_Private *self)
{

}

EO3_DEFINE_CLASS(ELM_VIEW_GRID_CLASS, ((EO3_BASE_CLASS)) Elm_View_Grid_Private)
