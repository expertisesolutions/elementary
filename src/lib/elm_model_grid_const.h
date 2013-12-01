
#ifndef ELM_MODEL_GRID_CONST_H
#define ELM_MODEL_GRID_CONST_H

typedef long Elm_Model_Grid_Row;
typedef long Elm_Model_Grid_Column;

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * @arg row The row to select.
 *
 * Select a @p row of this grid model.
 *
 */
EAPI void elm_model_grid_row_select(Elm_Model_Grid_Row row) EINA_CONST; 

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * @param row The row coordinate of the cell.
 * @param column The column coordinate of the cell.
 * 
 * Select a cell from this grid model.
 */
EAPI void elm_model_grid_cell_select(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column);

/**
 * @def elm_model_grid_columns_get
 * @since 1.8
 *
 * @return The list of columns as an Eina_List of Elm_Column_Grid_Column.
 * 
 * Get the list of columns of this grid model.
 */
EAPI Eina_List elm_model_grid_columns_get();

/**
 * @def elm_model_grid_rows_count
 * @since 1.8
 *
 * @return The number of rows in this grid, as integer.
 * 
 * Get the numbers of rows of this grid model.
 *
 */
EAPI int elm_model_grid_rows_count();

/**
 * @def elm_model_grid_columns_count
 * @since 1.8
 *
 * @return The number of columns in this grid, as integer.
 *
 * Get the numbers of columns of this grid model.
 */
EAPI int elm_model_grid_columns_count();

/**
 * @def elm_model_grid_value_get
 * @since 1.8
 *
 * Get the value of a cell from this grid model.
 *
 * @param row The row coordinate of the cell.
 * @param column The column coordinate of the cell.
 * @return The value, as @ref Eina_Value.
 */
EAPI Eina_Value elm_model_grid_value_get(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column);

/**
 * @def elm_model_grid_selected_row_get
 * @since 1.8
 *
 * Get the row of a selected cell from this grid model.
 *
 * @return The selected row, as Elm_Model_Grid_Row.
 */
EAPI Elm_Model_Grid_Row elm_model_grid_selected_row_get();

/**
 * @def elm_model_grid_selected_column_get
 * @since 1.8
 *
 * Get the column of a selected cell from this grid model.
 *
 * @return The selected column, as Elm_Model_Grid_Column.
 */
EAPI Elm_Model_Grid_Column elm_model_grid_selected_column_get();


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
#define ELM_MODEL_GRID_CONST_INTERFACE elm_model_grid_const         \
   , function(elm_model_grid_row_select, void, Elm_Model_Grid_Row)  \
   , function(elm_model_grid_rows_count, int)                       \
   , function(elm_model_grid_cell_select, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column) \
   , function(elm_model_grid_columns_get, Eina_List)                \
   , function(elm_model_grid_columns_count, int)                    \
   , function(elm_model_grid_selected_row_get, Elm_Model_Grid_Row)            \
   , function(elm_model_grid_selected_column_get, Elm_Model_Grid_Column)      \
   , function(elm_model_grid_value_get, Eina_Value, Elm_Model_Grid_Row, Elm_Model_Grid_Column)
   /* , event(elm_model_grid_row_selected) \ */
   /* , event(elm_model_grid_reordered) */
   /* , event(elm_model_grid_row_inserted) \ */
   /* , event(elm_model_grid_row_deleted)  \ */
   /* , event(elm_model_grid_row_changed)  \ */

EO3_DECLARE_INTERFACE(ELM_MODEL_GRID_CONST_INTERFACE)

#endif // ELM_MODEL_GRID_CONST_H
