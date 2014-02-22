
#ifndef ELM_MODEL_GRID_MUTABLE_H
#define ELM_MODEL_GRID_MUTABLE_H

extern EAPI Eo_Op ELM_OBJ_MODEL_GRID_BASE_ID;


enum {
   ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND,
   ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND,
   ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND_RELATIVE,
   ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND_RELATIVE,
   ELM_OBJ_MODEL_GRID_SUB_ID_ROW_DELETE,
   ELM_OBJ_MODEL_GRID_SUB_ID_VALUE_SET,
   ELM_OBJ_MODEL_GRID_SUB_ID_LAST
};

#define ELM_OBJ_MODEL_GRID_ID(sub_id) (ELM_OBJ_MODEL_GRID_BASE_ID + sub_id)
/**
 * @def elm_model_grid_row_append
 * @since 1.8
 *
 * @return The index of the newly created row.
 *
 * Append a new row to this grid model.
 */
#define elm_model_grid_row_append(ret) ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND), EO_TYPECHECK(Elm_Model_Grid_Row *, ret)

/**
 * @def elm_model_grid_row_prepend
 * @since 1.8
 *
 * @return The index of the newly created row.
 *
 * Prepend a new row to this grid model.
 */
//EAPI Elm_Model_Grid_Row elm_model_grid_row_prepend();
#define elm_model_grid_row_prepend(ret) ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND), EO_TYPECHECK(Elm_Model_Grid_Row *, ret)

/**
 * @def elm_model_grid_row_append_relative
 * @since 1.8
 *
 * @arg row The row from where the new row will be appended.
 * @return The index of the newly created row.
 *
 * Append a new row to this grid model after row @p row.
 */
//EAPI Elm_Model_Grid_Row elm_model_grid_row_append_relative(Elm_Model_Grid_Row row);
#define elm_model_grid_row_append_relative(row, ret) ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_APPEND_RELATIVE), EO_TYPECHECK(Elm_Model_Grid_Row, row), EO_TYPECHECK(Elm_Model_Grid_Row *, ret)

/**
 * @def elm_model_grid_row_prepend_relative
 * @since 1.8
 *
 * @arg row The row from where the new row will be prepended.
 * @return The index of the newly created row.
 *
 * Prepend a new row to this grid model before row @p row.
 */
///EAPI Elm_Model_Grid_Row elm_model_grid_row_prepend_relative(Elm_Model_Grid_Row row);
#define elm_model_grid_row_prepend_relative(ret) ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_PREPEND_RELATIVE), EO_TYPECHECK(Elm_Model_Grid_Row, row), EO_TYPECHECK(Elm_Model_Grid_Row *, ret)

/**
 * @def elm_model_grid_row_delete
 * @since 1.8
 *
 * @arg row The row to delete.
 *
 * Delete a @p row of this grid model.
 *
 */
//EAPI void elm_model_grid_row_delete(Elm_Model_Grid_Row row);
#define elm_model_grid_row_delete(ret) ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_ROW_DELETE), EO_TYPECHECK(Elm_Model_Grid_Row, row)

/**
 * @def elm_model_grid_value_set
 * @since 1.8
 *
 * @param row The row coordinate of the cell.
 * @param column The column coordinate of the cell.
 * @param value The value.
 *
 * Set @p value to the cell at position (@p row, @p column).
 */
//EAPI void elm_model_grid_value_set(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column, Eina_Value* value);
#define elm_model_grid_value_set(row, column, value) ELM_OBJ_MODEL_GRID_ID(ELM_OBJ_MODEL_GRID_SUB_ID_VALUE_SET), EO_TYPECHECK(Elm_Model_Grid_Row, row), EO_TYPECHECK(Elm_Model_Grid_Column, column), EO_TYPECHECK(Eina_Value *, value)

/**
 * @brief EO3 Class Declaration
 */
/* #define ELM_MODEL_GRID_CLASS elm_model_grid                                           \ */
/*  , function(elm_model_grid_row_append, _model_grid_row_append, Elm_Model_Grid_Row)    \ */
/*  , function(elm_model_grid_row_prepend, _model_grid_row_prepend, Elm_Model_Grid_Row)  \ */
/*  , function(elm_model_grid_row_append_relative, _model_grid_row_append_relative,      \ */
/*             Elm_Model_Grid_Row, Elm_Model_Grid_Row) */
/* , function(elm_model_grid_row_prepend_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row) \ */
/* , function(elm_model_grid_row_delete, void, Elm_Model_Grid_Row)                         \ */
/* , function(elm_model_grid_value_set, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column, Eina_Value) */

/* EO3_DECLARE_CLASS(ELM_MODEL_GRID_CLASS) */

#define ELM_OBJ_MODEL_GRID_CLASS elm_obj_model_grid_class_get()
const Eo_Class *elm_obj_model_grid_class_get(void);

#endif // ELM_MODEL_GRID_MUTABLE_H
