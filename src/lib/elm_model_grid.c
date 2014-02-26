
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_grid.h"

#include <assert.h>

/// Const Interface ///

#define MY_MODEL_GRID_CONST_CLASS ELM_OBJ_MODEL_GRID_CONST_CLASS
EAPI Eo_Op ELM_OBJ_MODEL_GRID_CONST_BASE_ID = 0;

static void
_model_grid_row_select(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row row = va_arg(*list, Elm_Model_Grid_Row);
}

static void
_model_grid_rows_count(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   int *ret = va_arg(*list, int *);
   *ret = 1;
}

static void
_model_grid_cell_select(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row row = va_arg(*list, Elm_Model_Grid_Row);
   Elm_Model_Grid_Column column = va_arg(*list, Elm_Model_Grid_Column);
}

static void
_model_grid_columns_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Eina_List **ret = va_arg(*list, Eina_List **);
   *ret = NULL;
}

static void
_model_grid_columns_count(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   int *ret = va_arg(*list, int *);
   *ret = -1;
}

static void
_model_grid_selected_row_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row *ret = va_arg(*list, Elm_Model_Grid_Row *);
   *ret = (Elm_Model_Grid_Row)-1;
}

static void
_model_grid_selected_column_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Column *ret = va_arg(*list, Elm_Model_Grid_Column *);
   *ret = (Elm_Model_Grid_Column)-1;
}

static void
_model_grid_value_get(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Eina_Value **ret = va_arg(*list, Eina_Value **);
   *ret = NULL;
}

static void
_model_grid_constructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
}
static void
_model_grid_destructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
}

static void
_model_grid_const_class_constructor(Eo_Class *klass)
{

   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _model_grid_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _model_grid_destructor),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROW_SELECT), _model_grid_row_select),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_CELL_SELECT), _model_grid_cell_select),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_GET), _model_grid_columns_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROWS_COUNT), _model_grid_rows_count),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_COUNT), _model_grid_columns_count),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_VALUE_GET), _model_grid_value_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_ROW_GET), _model_grid_selected_row_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_COLUMN_GET), _model_grid_selected_column_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description model_grid_const_op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROW_SELECT, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_CELL_SELECT, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_GET, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROWS_COUNT, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_COUNT, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_VALUE_GET, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_ROW_GET, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_COLUMN_GET, "!!description here"),
   EO_OP_DESCRIPTION_SENTINEL
};

/*
static const Eo_Event_Description *model_grid_const_event_descs[] = {
     ELM_MODEL_TREE_CONST_SELECTED_EVT,
     NULL
};
*/

static Eo_Class_Description model_grid_const_class_descs = {
   EO_VERSION,
   "Model Tree Const",
   EO_CLASS_TYPE_REGULAR_NO_INSTANT,
   //EO_CLASS_TYPE_REGULAR_NO_INSTANT, //TODO: check this
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_MODEL_GRID_CONST_BASE_ID, model_grid_const_op_descs, ELM_OBJ_MODEL_GRID_CONST_SUB_ID_LAST),
   NULL,//model_grid_const_event_descs, // event_descs
   0, //provate data sizeof
   _model_grid_const_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_model_grid_const_class_get, &model_grid_const_class_descs, EO_BASE_CLASS, NULL);





/// Mutable Interface ///

void
_model_grid_row_append(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row *ret = va_arg(*list, Elm_Model_Grid_Row *);
   *ret = (Elm_Model_Grid_Row)-1;
}

void
_model_grid_row_prepend(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row *ret = va_arg(*list, Elm_Model_Grid_Row *);
   *ret = (Elm_Model_Grid_Row)-1;
}

void
_model_grid_row_append_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row row = va_arg(*list, Elm_Model_Grid_Row);
   Elm_Model_Grid_Row *ret = va_arg(*list, Elm_Model_Grid_Row *);
   *ret = (Elm_Model_Grid_Row)-1;
}

void
_model_grid_row_prepend_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row row = va_arg(*list, Elm_Model_Grid_Row);
   Elm_Model_Grid_Row *ret = va_arg(*list, Elm_Model_Grid_Row *);
   *ret = (Elm_Model_Grid_Row)-1;
}

void
_model_grid_row_delete(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row row = va_arg(*list, Elm_Model_Grid_Row);
}

void
_model_grid_value_set(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   //TODO: Implement
   Elm_Model_Grid_Row row = va_arg(*list, Elm_Model_Grid_Row);
   Elm_Model_Grid_Column column = va_arg(*list, Elm_Model_Grid_Column);
   Eina_Value *value = va_arg(*list, Eina_Value *);
}

#define MY_MODEL_GRID_CLASS ELM_OBJ_MODEL_GRID_CLASS
EAPI Eo_Op ELM_OBJ_MODEL_GRID_BASE_ID = 0;

static void
_model_grid_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND), _model_grid_row_append),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND), _model_grid_row_prepend),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND_RELATIVE), _model_grid_row_append_relative),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND_RELATIVE), _model_grid_row_prepend_relative),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_DELETE), _model_grid_row_delete),
      EO_OP_FUNC(ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_VALUE_SET), _model_grid_value_set),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description model_grid_op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND_RELATIVE, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND_RELATIVE, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_DELETE, "!!description here"),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_GRID_SUB_ID_VALUE_SET, "!!description here"),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description model_grid_class_descs = {
   EO_VERSION,
   "Model Grid",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_MODEL_GRID_BASE_ID, model_grid_op_descs, ELM_OBJ_MODEL_GRID_SUB_ID_LAST),
   NULL, // event_descs
   0, //private data sizeof
   _model_grid_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_model_grid_class_get, &model_grid_class_descs, ELM_OBJ_MODEL_GRID_CONST_CLASS, NULL);


/// Class definition ///

/* EO3_DEFINE_CLASS(ELM_MODEL_GRID_CONST_CLASS, ((EO3_BASE_CLASS)), NULL) */

/* EO3_DEFINE_CLASS(ELM_MODEL_GRID_CLASS, ((ELM_MODEL_GRID_CONST_CLASS)), NULL) */
