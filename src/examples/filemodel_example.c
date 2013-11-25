//Compile (not yet) with:
//gcc -g filemodel_example.c -o filemodel_example `pkg-config --cflags --libs elementary`

#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <Eo.h>
#include <Elementary.h>

#if 0 // XXX TODO: implement
#include <elm_view_tree.h> 
#include <elm_view_grid.h>
#else
struct _Elm_View_Tree {};
struct _Elm_View_Grid {};
#endif
static Elm_View_Tree *_tree_v = NULL;
static Elm_View_Grid *_grid_v = NULL;

#if 0 // XXX TODO: implement
#include <elm_model_file_tree.h> // XXX TODO: implement
#include <elm_model_file_grid.h> // XXX TODO: implement
#else
struct _Elm_Model_File_Tree {};
struct _Elm_Model_File_Grid {};
#endif
static Eo_Model_File_Tree *_file_m = NULL; //implements file as a tree data model
static Eo_Model_File_Grid *_grid_m = NULL; //shows the selected group_node content as a grid

extern Elm_Model_File_Tree* elm_model_file_tree_new(const char *path);
extern Elm_Model_File_Grid* elm_model_file_grid_new(const char *path);

extern Elm_View_Tree* elm_view_tree_add(Evas_Object* parent, Eo_Model_File_Tree* file_model);
extern Elm_View_Tree* elm_view_grid_add(Evas_Object* parent, Eo_Model_File_Grid* file_model);

#define elm_view_tree_mode_set(mode) 0, EO_TYPECHECK(int, 5)
#define elm_view_tree_value_get(evas) 0, EO_TYPECHECK(Evas_Object**, evas)

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

   _file_m = elm_model_file_tree_new("/tmp");
   _grid_m = elm_model_file_grid_new(_file_m);
   
   _tree_v = elm_view_tree_add(win, _file_m);
   _grid_v = elm_view_grid_add(win, _grid_m);

   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   eo_do(_tree_v, elm_model_tree_set(ELM_TREE_VIEW_VIEWMODE_GROUP)); //hide files, show only directories
   eo_do(_tree_v, elm_view_tree_evas_object_get(&widget));

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
