
#ifndef ELM_MODEL_GRID_CONST_H
#define ELM_MODEL_GRID_CONST_H

typedef long Elm_Model_Grid_Row;
typedef long Elm_Model_Grid_Column;

extern EAPI Eo_Op ELM_OBJ_MODEL_GRID_CONST_BASE_ID;

enum {
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROW_SELECT,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_CELL_SELECT,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_GET,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROWS_COUNT,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_COUNT,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_VALUE_GET,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_ROW_GET,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_COLUMN_GET,
   ELM_OBJ_MODEL_GRID_CONST_SUB_ID_LAST,
};

extern EAPI Eo_Op ELM_OBJ_MODEL_GRID_CONST_BASE_ID;

#define ELM_OBJ_MODEL_GRID_CONST_ID(sub_id) (ELM_OBJ_MODEL_GRID_CONST_BASE_ID + sub_id)

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * @arg row The row to select.
 *
 * Select a @p row of this grid model.
 *
 */
#define elm_model_grid_row_select(row) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROW_SELECT), EO_TYPECHECK(Elm_Model_Grid_Row, row)

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * @param row The row coordinate of the cell.
 * @param column The column coordinate of the cell.
 *
 * Select a cell from this grid model.
 */
#define elm_model_grid_cell_select(row, column) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_CELL_SELECT), EO_TYPECHECK(Elm_Model_Grid_Row, row), EO_TYPECHECK(Elm_Model_Grid_Column, column)

/**
 * @def elm_model_grid_columns_get
 * @since 1.8
 *
 * @return The list of columns as an Eina_List of Elm_Column_Grid_Column.
 *
 * Get the list of columns of this grid model.
 */
#define  elm_model_grid_columns_get(ret) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_GET), EO_TYPECHECK(Eina_List **, ret)

/**
 * @def elm_model_grid_rows_count
 * @since 1.8
 *
 * @return The number of rows in this grid, as integer.
 *
 * Get the numbers of rows of this grid model.
 *
 */
#define elm_model_grid_rows_count(ret) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_ROWS_COUNT), EO_TYPECHECK(int *, ret)

/**
 * @def elm_model_grid_columns_count
 * @since 1.8
 *
 * @return The number of columns in this grid, as integer.
 *
 * Get the numbers of columns of this grid model.
 */
#define elm_model_grid_columns_count(ret) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMNS_COUNT), EO_TYPECHECK(int *, ret)

/**
 * @def elm_model_grid_value_get
 * @since 1.8
 *
 * Get the value of a cell from this grid model.
 *
 * @param row The row coordinate of the cell.
 * @param column The column coordinate of the cell.
 * @return The value, as a pointer to an @ref Eina_Value.
 */
#define elm_model_grid_value_get(row, column, ret) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_VALUE_GET), EO_TYPECHECK(Elm_Model_Grid_Row, row), EO_TYPECHECK(Elm_Model_Grid_Column, column), EO_TYPECHECK(Eina_Value **, ret)

/**
 * @def elm_model_grid_selected_row_get
 * @since 1.8
 *
 * Get the row of a selected cell from this grid model.
 *
 * @return The selected row, as Elm_Model_Grid_Row.
 */
#define elm_model_grid_selected_row_get(ret) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_SELECTED_ROW_GET), EO_TYPECHECK(Elm_Model_Grid_Row *, ret)

/**
 * @def elm_model_grid_selected_column_get
 * @since 1.8
 *
 * Get the column of a selected cell from this grid model.
 *
 * @return The selected column, as Elm_Model_Grid_Column.
 */
#define elm_model_grid_selected_column_get(ret) ELM_OBJ_MODEL_GRID_CONST_ID(ELM_OBJ_MODEL_GRID_CONST_SUB_ID_COLUMN_GET), EO_TYPECHECK(Elm_Model_Grid_Column_Get *, ret)


/**
 * @def elm_model_grid_row_selected
 * @since 1.8
 *
 * Raised when a row is selected.
 */

/**
 * @def elm_model_grid_row_reordered
 * @since 1.8
 *
 * Raised when the grid is reordered.
 */

/**
 * @def elm_model_grid_row_inserted
 * @since 1.8
 *
 * Raised when a new row is inserted into this grid model.
 */

/**
 * @def elm_model_grid_row_deleted
 * @since 1.8
 *
 * Raised when this grid model is deleted.
 */

/**
 * @def elm_model_grid_row_changed
 * @since 1.8
 *
 * Raised when a row of this grid model is changed.
 */

/**
 * @brief EO3 Class Declaration
 */
/* #define ELM_MODEL_GRID_CONST_CLASS elm_model_grid_const                 \ */
/*  , function(elm_model_grid_row_select, _model_grid_row_select, void, Elm_Model_Grid_Row) \ */
/*  , function(elm_model_grid_rows_count, _model_grid_rows_count, int)     \ */
/*  , function(elm_model_grid_cell_select, _model_grid_cell_select, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column) \ */
/*  , function(elm_model_grid_columns_get, _model_grid_columns_get, Eina_List*) \ */
/*  , function(elm_model_grid_columns_count, _model_grid_columns_count, int) \ */
/*  , function(elm_model_grid_selected_row_get, _model_grid_selected_row_get, Elm_Model_Grid_Row) \ */
/*  , function(elm_model_grid_selected_column_get, _model_grid_selected_column_get, Elm_Model_Grid_Column) \ */
/*  , function(elm_model_grid_value_get, _model_grid_value_get, Eina_Value*, Elm_Model_Grid_Row, Elm_Model_Grid_Column) \ */
/*  , event(elm_model_grid_cell_select, Elm_Model_Grid_Row, Elm_Model_Grid_Column) \ */
/*  , event(elm_model_grid_row_select, Elm_Model_Grid_Row) */
/* /\* , event(elm_model_grid_row_deleted, Elm_Model_Grid_Row)    \ */
/*    , event(elm_model_grid_row_changed, Elm_Model_Grid_Row)*\/ */

/* EO3_DECLARE_CLASS(ELM_MODEL_GRID_CONST_CLASS) // XXX NON-INSTANTIABLE */

#define ELM_OBJ_MODEL_GRID_CONST_CLASS elm_obj_model_grid_const_class_get()
const Eo_Class *elm_obj_model_grid_const_class_get(void);

#endif // ELM_MODEL_GRID_CONST_H
