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
#define ELM_VIEW_TREE_CLASS EO_BASE_CLASS
#define ELM_VIEW_GRID_CLASS EO_BASE_CLASS

extern Eo* elm_view_tree_add(Evas_Object* parent, Eo* file_model) {};
extern Eo* elm_view_grid_add(Evas_Object* parent, Eo* file_model) {};
extern Eo* elm_view_tree_evas_object_get(Evas_Object** widget) {};
extern Eo* elm_view_grid_evas_object_get(Evas_Object** widget) {};
#endif

#if 0 // XXX TODO: implement
#include "model_file_tree.h" // XXX TODO: implement
#include "model_file_grid.h" // XXX TODO: implement
#else
#define MODEL_FILE_TREE_CLASS EO_BASE_CLASS
#define MODEL_FILE_GRID_CLASS EO_BASE_CLASS

extern Eo* model_file_grid_constructor(Eo *model) {};
extern Eo* model_file_tree_root_set(const char * path) {};
extern void elm_view_tree_model_tree_set(int mode) {};
#endif


EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   Evas_Object *box;
   Evas_Object *widget;
   Eo *_file_m; //implements file as a tree data model
   Eo *_grid_m; //shows the selected group_node content as a grid
   Eo *_tree_v;
   Eo *_grid_v;

   int i;

   enum 
   {
     ELM_TREE_VIEW_VIEWMODE_GROUP
   };
   
   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   _file_m = eo2_add(MODEL_FILE_TREE_CLASS, NULL);
   _grid_m = eo2_add_custom(MODEL_FILE_GRID_CLASS, NULL, model_file_grid_constructor(_file_m));
   eo2_do(_file_m, model_file_tree_root_set("/tmp"));
   
   _tree_v = eo2_add_custom(ELM_VIEW_TREE_CLASS, NULL, elm_view_tree_add(win, _file_m));
   _grid_v = eo2_add_custom(ELM_VIEW_GRID_CLASS, NULL, elm_view_grid_add(win, _grid_m));

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
