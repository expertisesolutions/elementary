//Compile (not yet) with:
//gcc -g filemodel_example.c -o filemodel_example `pkg-config --cflags --libs elementary`

#include <Elementary.h>

static Elm_Tree_View_Class *_tree_v = NULL;
static Elm_List_View_Class *_list_v = NULL;

static Eo_File_Model_Class *_file_m = NULL; //implements file as a tree_data_model
static Eo_List_Node_Model_Class *_list_m = NULL; //show the selected group_node content as a list

static void
_item_sel_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info)
{
   Evas_Object *label = (Evas_Object *) data;

   printf("selected item data [%p] on model obj [%p]\n", data, obj, event_info);
   //TODO: set label with file content

   (void) desc;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   Evas_Object *box;
   Evas_Object *widget;
   Evas_Object *label;
   int i;

   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   _file_m = eo_file_model_class_new("/tmp");
   _list_m = eo_list_node_model_class_new(_file_m);
   
   _tree_v = elm_tree_view_add(win, _file_m);
   _list_v = elm_list_view_add(win, _list_m);

   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   eo_do(_tree_v, elm_tree_view_mode_set, ELM_TREE_VIEW_VIEWMODE_GROUP); //hide files, show only directories
   eo_do(_tree_v, elm_tree_view_evas_object_get, &widget); //or tree_view inherits genlist

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_genlist_mode_set(widget, ELM_LIST_LIMIT);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);


   //file list widget
   eo_do(_list_v, elm_list_view_evas_object_get, &widget); //or tree_view inherits genlist

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_genlist_mode_set(widget, ELM_LIST_LIMIT);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);

   //label
   label = elm_label_add (win);
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, label);
   evas_object_show(label);
 
   //add callback
   eo_do(_lis_m, eo_event_callback_add(EO_EV_LISTMODEL_ITEM_SELECTED, _item_sel_cb, label));


   evas_object_resize(win, 800, 400);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
