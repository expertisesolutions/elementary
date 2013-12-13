
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_grid.h"

#include <assert.h>

/// Const Interface ///

static void
_model_grid_row_select(Elm_Model_Grid_Row row)
{
}

static int
_model_grid_rows_count()
{
   return -1;
}

static void
_model_grid_cell_select(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column)
{
}

static Eina_List*
_model_grid_columns_get()
{
   return NULL;
}

static int
_model_grid_columns_count()
{
   return -1;
}

static Elm_Model_Grid_Row
_model_grid_selected_row_get()
{
   return (Elm_Model_Grid_Row)-1;
}

static Elm_Model_Grid_Column
_model_grid_selected_column_get()
{
   return (Elm_Model_Grid_Column)-1;
}

static Eina_Value*
_model_grid_value_get(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column)
{
   return NULL;
}

/// Mutable Interface ///

static Elm_Model_Grid_Row
_model_grid_row_append()
{
   return (Elm_Model_Grid_Row)-1;
}

static Elm_Model_Grid_Row
_model_grid_row_prepend()
{
   return (Elm_Model_Grid_Row)-1;
}

static Elm_Model_Grid_Row
_model_grid_row_append_relative(Elm_Model_Grid_Row row)
{
   return (Elm_Model_Grid_Row)-1;
}

/// Class definition ///

EO3_DEFINE_CLASS(ELM_MODEL_GRID_CONST_CLASS, ((EO3_BASE_CLASS)), NULL)

EO3_DEFINE_CLASS(ELM_MODEL_GRID_CLASS, ((ELM_MODEL_GRID_CONST_CLASS)), NULL)
