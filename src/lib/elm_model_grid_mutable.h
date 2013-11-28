
#ifndef ELM_MODEL_GRID_MUTABLE_H
#define ELM_MODEL_GRID_MUTABLE_H

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
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_GRID_INTERFACE elm_model_grid                                            \
   , function(elm_model_grid_row_append, Elm_Model_Grid_Row)                               \
   , function(elm_model_grid_row_prepend, Elm_Model_Grid_Row)                              \
   , function(elm_model_grid_row_append_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row) /* \*/
   /* , function(elm_model_grid_row_prepend_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row) \ */
   /* , function(elm_model_grid_row_delete, void, Elm_Model_Grid_Row)                         \ */
   /* , function(elm_model_grid_value_set, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column, Eina_Value) */

EO3_DECLARE_INTERFACE(ELM_MODEL_GRID_INTERFACE)

#endif // ELM_MODEL_GRID_MUTABLE_H
