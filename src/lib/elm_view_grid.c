#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

#define MY_CLASS ELM_OBJ_VIEW_GRID_CLASS
EAPI Eo_Op ELM_OBJ_VIEW_GRID_BASE_ID = 0;

struct _Elm_View_Grid_Private
{
   Evas_Object *list;
   Elm_Genlist_Item_Class *itc;
   Eo *model;
};
typedef struct _Elm_View_Grid_Private Elm_View_Grid_Private;

/*/////////////////////////////////////////////////////////////////////////////////
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
*//////////////////////////////////////////////////////////////////////////////////

static inline void
_update_grid_widget(Elm_View_Grid_Private* self)
{
   assert(self);
   //foreach model itens and update rows to widget
   //XXX emit update/reorder signal
}

static void
_elm_view_grid_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Grid_Private *self = class_data;
   Evas_Object *parent = va_arg(*list, Evas_Object *);
   Eo *model = va_arg(*list, Eo *);

   assert(self);
   assert(parent);
   assert(model);

   eo_do_super(obj, ELM_OBJ_VIEW_GRID_CLASS, eo_constructor());

   self->list = elm_genlist_add(parent);
   self->model = model;
   _update_grid_widget(self);
/*
   eo2_do(self->model, elm_model_grid_row_selected_callback_add(_model_row_selected_cb, list);
   eo2_do(self->model, elm_model_grid_row_inserted_callback_add(_model_row_inserted_cb, list);
   eo2_do(self->model, elm_model_grid_row_deleted_callback_add(_model_row_deleted_cb, list);
   eo2_do(self->model, elm_model_grid_row_changed_callback_add(_model_row_changed_cb, list);
   eo2_do(self->model, elm_model_grid_reordered_callback_add(_model_reordered_cb, list);
*/
}

static void
_elm_view_grid_destructor(Eo *obj, void *class_data, va_list *list EINA_UNUSED)
{
   Elm_View_Grid_Private *self = class_data;
   assert(self);
   assert(obj);
   eo_do_super(obj, MY_CLASS, eo_destructor());
   //XXX destruct evas obj?
}

static void
_elm_view_grid_evas_object_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Grid_Private *self = class_data;
   Evas_Object **evas = va_arg(*list, Evas_Object **);

   assert(self);
   assert(obj);
   *evas = self->list;
}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_grid_add),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_grid_destructor),
      EO_OP_FUNC(ELM_OBJ_VIEW_GRID_CLASS_ID(ELM_OBJ_VIEW_GRID_SUB_ID_EVAS_OBJECT_GET), _elm_view_grid_evas_object_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

const Eo_Op_Description op_descs[] = {
     EO_OP_DESCRIPTION(ELM_OBJ_VIEW_GRID_SUB_ID_EVAS_OBJECT_GET, "Returns Evas Widget object."),
     EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description class_descs = {
   EO_VERSION,
   "Elm View Grid",
   EO_CLASS_TYPE_REGULAR_NO_INSTANT,
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_VIEW_GRID_BASE_ID, op_descs, ELM_OBJ_VIEW_GRID_SUB_ID_LAST),
   NULL,
   sizeof(Elm_View_Grid_Private),
   _class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_view_grid_class_get, &class_descs, EO_BASE_CLASS, NULL);
