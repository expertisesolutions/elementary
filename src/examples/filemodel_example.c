//Compile (not yet) with:
//gcc -g filemodel_example.c -o filemodel_example `pkg-config --cflags --libs elementary`

#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <Eo.h>
#include <Elementary.h>

typedef Eo Elm_Tree_View_Class;
typedef Eo Elm_Grid_View_Class;

static Elm_Tree_View_Class *_tree_v = NULL;
static Elm_Grid_View_Class *_grid_v = NULL;

typedef Eo Eo_File_Model_Class;
typedef Eo Eo_Grid_Dir_Model_Class;

static Eo_File_Model_Class *_file_m = NULL; //implements file as a tree_data_model
static Eo_Grid_Dir_Model_Class *_grid_m = NULL; //show the selected group_node content as a grid

Eo_File_Model_Class*
eo_file_model_class_new(const char* path);

Eo_Grid_Dir_Model_Class*
eo_grid_dir_model_class_new(Eo_File_Model_Class* file_model);

Elm_Tree_View_Class*
elm_tree_view_add(Evas_Object* parent, Eo_File_Model_Class* file_model);

Elm_Tree_View_Class*
elm_tree_view_add(Evas_Object* parent, Eo_File_Model_Class* file_model);

Elm_Grid_View_Class*
elm_grid_view_add(Evas_Object* parent, Eo_Grid_Dir_Model_Class* grid_model);

#define elm_tree_view_mode_set(mode) 0, EO_TYPECHECK(int, 5)
#define elm_tree_view_evas_object_get(evas) 0, EO_TYPECHECK(Evas_Object**, evas)

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   Evas_Object *box;
   Evas_Object *widget;
   int i;

   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   _file_m = eo_file_model_class_new("/tmp");
   _grid_m = eo_grid_dir_model_class_new(_file_m);
   
   _tree_v = elm_tree_view_add(win, _file_m);
   _grid_v = elm_grid_view_add(win, _grid_m);

   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   eo_do(_tree_v, elm_tree_view_mode_set(ELM_TREE_VIEW_VIEWMODE_GROUP)); //hide files, show only directories
   eo_do(_tree_v, elm_tree_view_evas_object_get(&widget));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_genlist_mode_set(widget, ELM_LIST_LIMIT);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);

   //file grid widget
   eo_do(_grid_v, elm_grid_view_evas_object_get(&widget));

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
