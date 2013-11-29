//Compile (not yet) with:
//gcc -g filemodel_example.c -o filemodel_example `pkg-config --cflags --libs elementary`

#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <assert.h>
#include <Eo.h>
#include <Elementary.h>

#define MODEL_FILE_GRID_CLASS EO_BASE_CLASS

/*#define ELM_VIEW_TREE_CLASS EO_BASE_CLASS*/
/*#define ELM_VIEW_GRID_CLASS EO_BASE_CLASS*/

/* extern */ Eo* elm_view_tree_add(Evas_Object* parent, Eo* file_model) {};
/* extern */ Eo* elm_view_grid_add(Evas_Object* parent, Eo* file_model) {};
/* extern */ Eo* elm_view_tree_evas_object_get(Evas_Object** widget) {};
/* extern */ Eo* elm_view_grid_evas_object_get(Evas_Object** widget) {};


////////////////////////////////////////////////////////////////////////////////

struct _Model_File_Tree_Private {};
typedef struct _Model_File_Tree_Private Model_File_Tree_Private;

Eo*
_model_file_tree_constructor_path(Eo *obj, Model_File_Tree_Private *data, const char* path)
{
  return obj;
}

Eo*
_model_file_tree_destructor(Eo *obj, Model_File_Tree_Private *data)
{
  return obj;
}

void
_model_file_tree_select(Elm_Model_Tree_Path path)
{
}

Eina_Value
_model_file_tree_value_get(Elm_Model_Tree_Path path)
{
  Eina_Value x;
  return x;
}

#define MODEL_FILE_TREE_CLASS model_file_tree                           \
, constructor(model_file_tree_constructor_path, _model_file_tree_constructor_path, const char*) \
, function_override(elm_model_tree_select, _model_file_tree_select)

//
//, function_override(elm_model_tree_value_get, _model_file_tree_value_get)
//, destructor(_model_file_tree_destructor)
//
EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS, ((ELM_MODEL_TREE_CONST_INTERFACE)), Model_File_Tree_Private)

////////////////////////////////////////////////////////////////////////////////

/* extern */ Eo* model_file_grid_constructor(Eo *model) {};
/* extern */ Eo* model_file_tree_root_set(const char * path) {};
/* extern */ void elm_view_tree_model_tree_set(int mode) {};

enum 
{
  ELM_TREE_VIEW_VIEWMODE_GROUP
};
   
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
                            NULL, model_file_tree_constructor_path("/tmp"));
   /*_grid_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_GRID_CLASS),
     NULL, model_file_grid_constructor(_tree_m));*/
   
   /*_tree_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_TREE_CLASS),
     NULL, elm_view_tree_add(win, _tree_m));*/
   /*_grid_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_GRID_CLASS),
     NULL, elm_view_grid_add(win, _grid_m));*/

   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   eo2_do(_tree_v, elm_view_tree_model_tree_set(ELM_TREE_VIEW_VIEWMODE_GROUP)); //hide files, show only directories
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
