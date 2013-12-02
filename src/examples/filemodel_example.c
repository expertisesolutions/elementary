
#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <assert.h>
#include <Eo.h>
#include <Elementary.h>

/*#define ELM_VIEW_TREE_CLASS EO_BASE_CLASS*/
/*#define ELM_VIEW_GRID_CLASS EO_BASE_CLASS*/

typedef enum 
{
  ELM_VIEW_TREE_VIEWMODE_GROUP
} Elm_View_Tree_Mode;

/* extern */ Eo* elm_view_tree_add(Evas_Object* parent, Eo* file_model) {}
/* extern */ Eo* elm_view_grid_add(Evas_Object* parent, Eo* file_model) {}
/* extern */ Eo* elm_view_tree_evas_object_get(Evas_Object** widget) {}
/* extern */ Eo* elm_view_grid_evas_object_get(Evas_Object** widget) {}
/* extern */ Eo* elm_view_tree_model_tree_set(Elm_View_Tree_Mode mode) {}


////////////////////////////////////////////////////////////////////////////////

struct _Model_File_Tree_Private {};
typedef struct _Model_File_Tree_Private Model_File_Tree_Private;

Eo*
_model_file_tree_constructor(Eo *obj, Model_File_Tree_Private *data, const char* root)
{
  return NULL;
}

Eo*
_model_file_tree_destructor(Eo *obj, Model_File_Tree_Private *data)
{
  return NULL;
}

void
_model_file_tree_select(Elm_Model_Tree_Path path)
{
}

Eina_Value
_model_file_tree_value_get(Elm_Model_Tree_Path path)
{
  Eina_Value tmp;
  return tmp;
}

Elm_Model_Tree_Path _model_file_tree_root_get()
{
  Elm_Model_Tree_Path tmp;
  return tmp;
}

Eina_List* _model_file_tree_children_get(Elm_Model_Tree_Path path)
{
  return NULL;
}

Elm_Model_Tree_Path _model_file_tree_selected_get()
{
  Elm_Model_Tree_Path tmp;
  return tmp;
}

#define MODEL_FILE_TREE_CLASS model_file_tree                                             \
    , constructor(model_file_tree_constructor, _model_file_tree_constructor, const char*) \
    , destructor(_model_file_tree_destructor)                                             \
    , function_override(elm_model_tree_select, _model_file_tree_select)                   \
    , function_override(elm_model_tree_value_get, _model_file_tree_value_get)             \
    , function_override(elm_model_tree_root_get, _model_file_tree_root_get)               \
    , function_override(elm_model_tree_children_get, _model_file_tree_children_get)       \
    , function_override(elm_model_tree_selected_get, _model_file_tree_selected_get)

EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS, ((ELM_MODEL_TREE_CONST_INTERFACE)), Model_File_Tree_Private)


////////////////////////////////////////////////////////////////////////////////

struct _Model_File_Grid_Private {};
typedef struct _Model_File_Grid_Private Model_File_Grid_Private;

Eo*
_model_file_grid_constructor(Eo *obj, Model_File_Tree_Private *data, const char *root)
{
  return NULL;
}

Eo*
_model_file_grid_construct_from_tree(Eo *obj, Model_File_Tree_Private *data, Eo *model_tree)
{
  return NULL;
}

Eo*
_model_file_grid_destructor(Eo *obj, Model_File_Tree_Private *data)
{
  return NULL;
}

void
_model_file_grid_row_select(Elm_Model_Grid_Row row)
{
}
  
void
_model_file_grid_cell_select(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column)
{
}

Eina_List
_model_file_grid_columns_get()
{
}

int
_model_file_grid_rows_count()
{
  return -1;
}

int
_model_file_grid_columns_count()
{
  return -1;
}

Elm_Model_Grid_Row _model_file_grid_selected_row_get()
{
  return (Elm_Model_Grid_Row)-1;
}

Elm_Model_Grid_Column _model_file_grid_selected_column_get()
{
  return (Elm_Model_Grid_Column)-1;
}

Eina_Value
_model_file_grid_value_get(Elm_Model_Grid_Row row, Elm_Model_Grid_Column column)
{
  Eina_Value tmp;
  return tmp;
}

#define MODEL_FILE_GRID_CLASS model_file_grid                                                    \
   , constructor(model_file_grid_constructor, _model_file_tree_constructor, const char*)         \
   , constructor(model_file_grid_construct_from_tree, _model_file_grid_construct_from_tree, Eo*) \
   , destructor(_model_file_tree_destructor)                                                     \
   , function_override(elm_model_grid_row_select, _model_file_grid_row_select)                   \
   , function_override(elm_model_grid_rows_count, _model_file_grid_rows_count)                   \
   , function_override(elm_model_grid_cell_select, _model_file_grid_cell_select)                 \
   , function_override(elm_model_grid_columns_get, _model_file_grid_columns_get)                 \
   , function_override(elm_model_grid_columns_count, _model_file_grid_columns_count)             \
   , function_override(elm_model_grid_selected_row_get, _model_file_grid_selected_row_get)       \
   , function_override(elm_model_grid_selected_column_get, _model_file_grid_selected_column_get) \
   , function_override(elm_model_grid_value_get, _model_file_grid_value_get)

EO3_DECLARE_CLASS(MODEL_FILE_GRID_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_GRID_CLASS, ((ELM_MODEL_GRID_CONST_INTERFACE)), Model_File_Grid_Private)


////////////////////////////////////////////////////////////////////////////////

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   Evas_Object *box;
   Evas_Object *widget;
   Eo *_tree_m; //implements file as a tree data model
   Eo *_grid_m; //shows the selected group_node content as a grid
   Eo *_tree_v;
   Eo *_grid_v;

   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   _tree_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                            NULL, model_file_tree_constructor("/tmp"));
   _grid_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_GRID_CLASS),
                            NULL, model_file_grid_construct_from_tree(_tree_m));

   /* _tree_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_TREE_CLASS), */
   /*                          NULL, elm_view_tree_add(win, _tree_m)); */
   /* _grid_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_GRID_CLASS), */
   /*                          NULL, elm_view_grid_add(win, _grid_m)); */
   
   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   eo2_do(_tree_v, elm_view_tree_model_tree_set(ELM_VIEW_TREE_VIEWMODE_GROUP)); //hide files, show only directories
   eo2_do(_tree_v, elm_view_tree_evas_object_get(&widget));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_genlist_mode_set(widget, ELM_LIST_LIMIT);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);

   //file grid widget
   eo2_do(_grid_v, elm_view_grid_evas_object_get(&widget));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);
 
   evas_object_resize(win, 800, 400);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
