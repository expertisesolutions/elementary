
#ifndef ELM_MODEL_GRID_H
#define ELM_MODEL_GRID_H

#include "Eo.h"
#include "Eina.h"

typedef struct _Elm_Model_Grid_Row Elm_Model_Grid_Row; // XXX
typedef struct _Elm_Model_Grid_Column Elm_Model_Grid_Column; //XXX stringshare??

/**
 * @def elm_model_grid_row_append
 * @since 1.8
 *
 * @return The index of the newly created row.
 * 
 * Append a new row to this grid model.
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_append();

/**
 * @def elm_model_grid_row_prepend
 * @since 1.8
 *
 * @return The index of the newly created row.
 * 
 * Prepend a new row to this grid model.
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_prepend();

/**
 * @def elm_model_grid_row_append_relative
 * @since 1.8
 * 
 * @arg row The row from where the new row will be appended.
 * @return The index of the newly created row.
 * 
 * Append a new row to this grid model after row @p row.
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_append_relative(Elm_Model_Grid_Row row);

/**
 * @def elm_model_grid_row_prepend_relative
 * @since 1.8
 *
 * @arg row The row from where the new row will be prepended.
 * @return The index of the newly created row.
 *
 * Prepend a new row to this grid model before row @p row.
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_prepend_relative(Elm_Model_Grid_Row row);

/**
 * @def elm_model_grid_row_delete
 * @since 1.8
 *
 * @arg row The row to delete.
 * 
 * Delete a @p row of this grid model.
 *
 */
EAPI void elm_model_grid_row_delete(Elm_Model_Grid_Row row);

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * @arg row The row to select.
 *
 * Select a @p row of this grid model.
 *
 */
EAPI void elm_model_grid_row_select(Elm_Model_Grid_Row row);

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
 * get a column index list from grid model
 *
 * TODO document XXX
 */
EAPI Eina_List elm_model_grid_columns_get();

/**
 * @def elm_model_grid_rows_count
 * @since 1.8
 *
 * Get the numbers of rows of this grid model.
 *
 * @return The number of rows in this grid, as integer.
 */
EAPI int elm_model_grid_rows_count();

/**
 * @def elm_model_grid_columns_count
 * @since 1.8
 *
 * Get the numbers of columns of this grid model.
 *
 * @return The number of columns in this grid, as integer.
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
 * @def elm_model_grid_value_set
 * @since 1.8
 *
 * @param row The row coordinate of the cell.
 * @param column The column coordinate of the cell.
 * @param value The value, as @ref Eina_Value.
 *
 * Set @p value to the cell at position (@p row, @p column).
 */
EAPI void elm_model_grid_value_set(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column, Eina_Value value);

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
 * @def elm_model_grid_row_selected
 * @since 1.8
 *
 * Raised when a grid model row is selected.
 */

/**
 * @def elm_model_grid_row_changed
 * @since 1.8
 *
 * Raised when a row of this grid model is changed.
 */

/**
 * @def elm_model_grid_reordered
 * @since 1.8
 *
 * Raised when this grid model gets reordered.
 */

// XXX Constructor Grid receive a num of column or eina list with stringshare??

/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_GRID_INTERFACE elm_model_grid                     \
   , function(elm_model_grid_row_append, Elm_Model_Grid_Row)        \
   , function(elm_model_grid_row_prepend, Elm_Model_Grid_Row)       \
   , function(elm_model_grid_row_append_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row)  \
   , function(elm_model_grid_row_prepend_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row) \
   , function(elm_model_grid_row_delete, void, Elm_Model_Grid_Row)  \
   , function(elm_model_grid_row_select, void, Elm_Model_Grid_Row)  \
   , function(elm_model_grid_rows_count, int)                       \
   , function(elm_model_grid_cell_select, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column) /* ?? */ \
   , function(elm_model_grid_columns_get, Eina_List)  /*??*//*XXX*/ \
   , function(elm_model_grid_columns_count, int)                    \
   , function(elm_model_grid_value_get, Eina_Value, Elm_Model_Grid_Row, Elm_Model_Grid_Column) \
   , function(elm_model_grid_value_set, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column, Eina_Value)
   /* , event(elm_model_grid_row_inserted)                                \ */
   /* , event(elm_model_grid_row_deleted)                                 \ */
   /* , event(elm_model_grid_row_selected)                                \ */
   /* , event(elm_model_grid_row_changed)                                 \ */
   /* , event(elm_model_grid_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_GRID_INTERFACE)

#endif // ELM_MODEL_GRID_H
