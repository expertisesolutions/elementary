
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
 * append a new row in grid model
 *
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_append();


/**
 * @def elm_model_grid_row_prepend
 * @since 1.8
 *
 * prepend a new row in grid model
 *
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_prepend();


/**
 * @def elm_model_grid_row_append_relative
 * @since 1.8
 *
 * append a new row in grid model
 *
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_append_relative(Elm_Model_Grid_Row);


/**
 * @def elm_model_grid_row_prepend_relative
 * @since 1.8
 *
 * prepend a new row in grid model
 *
 */
EAPI Elm_Model_Grid_Row elm_model_grid_row_prepend_relative(Elm_Model_Grid_Row);

/**
 * @def elm_model_grid_row_delete
 * @since 1.8
 *
 * delete a row in grid model
 *
 */
EAPI void elm_model_grid_row_delete(Elm_Model_Grid_Row);

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * select a row in grid model
 *
 */
EAPI void elm_model_grid_row_select(Elm_Model_Grid_Row);

/**
 * @def elm_model_grid_row_select
 * @since 1.8
 *
 * select a cell in grid model
 *
 */
EAPI void elm_model_grid_cell_select(Elm_Model_Grid_Row, Elm_Model_Grid_Column);

/**
 * @def elm_model_grid_columns_get
 * @since 1.8
 *
 * get a column index list from grid model
 *
 */
EAPI Eina_List elm_model_grid_columns_get();

/**
 * @def elm_model_grid_rows_count
 * @since 1.8
 *
 * get a numbers of rows in grid model
 *
 */
EAPI int elm_model_grid_rows_count();

/**
 * @def elm_model_grid_columns_count
 * @since 1.8
 *
 * get a numbers of columns in grid model
 *
 */
EAPI int elm_model_grid_columns_count();

/**
 * @def elm_model_grid_value_get
 * @since 1.8
 *
 * get a row value in grid model
 *
 */
EAPI Eina_Value elm_model_grid_value_get(Elm_Model_Grid_Row, Elm_Model_Grid_Column);

/**
 * @def elm_model_grid_value_set
 * @since 1.8
 *
 * set a row value in grid model
 *
 */
EAPI void elm_model_grid_value_set(Elm_Model_Grid_Row, Elm_Model_Grid_Column, Eina_Value);

/**
 * @def elm_model_grid_row_inserted
 * @since 1.8
 *
 * Emitted when a new row is inserted in grid model
 *
 */

/**
 * @def elm_model_grid_row_deleted
 * @since 1.8
 *
 * Emitted when a grid model row is deleted
 *
 */

/**
 * @def elm_model_grid_row_selected
 * @since 1.8
 *
 * Emitted when a grid model row is selected
 *
 */

/**
 * @def elm_model_grid_row_changed
 * @since 1.8
 *
 * Emitted when a row of grid model is changed
 *
 */

/**
 * @def elm_model_grid_reordered
 * @since 1.8
 *
 * Emitted when a row of grid model is reordered
 *
 */

//Constructor Grid receive a num of column or eina list with stringshare??

/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_GRID_INTERFACE elm_model_grid                         				\
   , function(elm_model_grid_row_append, Elm_Model_Grid_Row)                         			\
   , function(elm_model_grid_row_prepend, Elm_Model_Grid_Row)                        			\
   , function(elm_model_grid_row_append_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row)   		\
   , function(elm_model_grid_row_prepend_relative, Elm_Model_Grid_Row, Elm_Model_Grid_Row)  		\
   , function(elm_model_grid_row_delete, void, Elm_Model_Grid_Row)   					\
   , function(elm_model_grid_row_select, void, Elm_Model_Grid_Row)   					\
   , function(elm_model_grid_rows_count, int) 								\
   , function(elm_model_grid_cell_select, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column)  /* ?? */ 	\
   , function(elm_model_grid_columns_get, Eina_List)  /*??*/		 				\
   , function(elm_model_grid_columns_count, int) 							\
   , function(elm_model_grid_value_get, Eina_Value, Elm_Model_Grid_Row, Elm_Model_Grid_Column)		\
   , function(elm_model_grid_value_set, void, Elm_Model_Grid_Row, Elm_Model_Grid_Column, Eina_Value)
   /* , event(elm_model_grid_row_inserted)                                \ */
   /* , event(elm_model_grid_row_deleted)                                 \ */
   /* , event(elm_model_grid_row_selected)                                \ */
   /* , event(elm_model_grid_row_changed)                                 \ */
   /* , event(elm_model_grid_reordered) */

EO3_DECLARE_INTERFACE(ELM_MODEL_GRID_INTERFACE)

#endif // ELM_MODEL_GRID_H
