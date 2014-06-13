//Compile with:
// gcc -o fileviewform fileviewform.c `pkg-config --cflags --libs emodel`
// To run:
//    $ ./mvc_form_01 [directory path]

# include "../../elementary_config.h"

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <elm_view_form.h>
#include <stdio.h>

#define FILEMODEL_PATH "/tmp" /**< Default directory path if none is provided by user */
#define DEFAULT_THUMBNAIL "../../data/images/logo.png" /**< Default PNG image file */

struct _Form_Child_Data
{
   Eo *evf, *model; /**< File model */
   Eina_Value *value_prev; /**< Previous Eina_Value for root directory update */
   Eina_Bool is_dir; /**< Is child directory of file? */
};
typedef struct _Form_Child_Data Form_Child_Data;

static void
_win_focused_cb(void *data, Evas_Object *obj, void *event)
{
}
static void
_main_win_del_cb(void *data, Evas_Object *obj, void *event)
{
}

/**
 * @brief Child callback
 */
static Eina_Bool
_child_selected_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Form_Child_Data *child_evt = data;
   Emodel_Children_EVT *evt = event_info;
   EINA_SAFETY_ON_NULL_RETURN_VAL(child_evt->evf, EINA_FALSE);
   eo_do(child_evt->evf, elm_view_form_model_set(evt->child));

   return EINA_TRUE;
}

/**
 * @brief Thumb widget error callback
 * From here we set a default image (E logo) if couldn't load the original one.
 * @see DEFAULT_THUMBNAIL
 */
static void
_generation_error_cb(void *data, Evas_Object *thumb, void *event_info)
{
   fprintf(stderr, "thumbnail generation error, loading default %s.\n", DEFAULT_THUMBNAIL);
   elm_thumb_file_set(thumb, DEFAULT_THUMBNAIL, NULL);
   elm_thumb_reload(thumb);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *fileview, *evf, *model;
   Evas_Object *win, *panes, *panes_h, *bigbox;
   Evas_Object *rootpath_label, *thumb, *entry, *genlist;
   Evas_Object *size_label;
   char *dirname;
   Form_Child_Data *child_data = calloc(1, sizeof(Form_Child_Data));
   EINA_SAFETY_ON_NULL_RETURN_VAL(child_data, 1);

   ecore_init();
   eio_init();

   if(argv[1] != NULL) dirname = argv[1];
   else dirname = FILEMODEL_PATH;


   /* File tree is used as example */
   model = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(dirname));
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, 1);

   evf = eo_add_custom(ELM_VIEW_FORM_CLASS, NULL, elm_view_form_constructor(NULL));
   EINA_SAFETY_ON_NULL_RETURN_VAL(evf, 1);

   child_data->evf = evf;
   child_data->model = model;
   eo_do(model, eo_event_callback_add(EMODEL_EVENT_CHILD_SELECTED, _child_selected_cb, child_data));

   /* for entry widget */
   static Elm_Entry_Filter_Limit_Size limit_size = {
        .max_char_count = 256, // max chars number
        .max_byte_count = 0
   };

   /* Main window setup */
   win = elm_win_util_standard_add("form_test", "Form Test");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE); /**< sets the window to be destroyed automatically by signal */
   elm_win_focus_highlight_enabled_set(win, EINA_TRUE); /**< enavle focus */
   evas_object_smart_callback_add(win, "focus,in", _win_focused_cb, "window");
   evas_object_smart_callback_add(win, "delete,request", _main_win_del_cb, "window"); /**< define window delete callback */
   evas_object_resize(win, 920, 720);
   evas_object_show(win);

   /* Box setup */
   bigbox = elm_box_add(win);
   evas_object_size_hint_weight_set(bigbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bigbox);

   /* Genlist for file view */
   genlist = elm_genlist_add(win);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(genlist);

   /* File view setup */
   fileview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(genlist, model));
   eo_do(fileview, elm_view_list_property_connect("filename", "elm.text"));
   eo_do(fileview, elm_view_list_property_connect("icon", "elm.swallow.icon"));

   /* Panes layout */
   panes = elm_panes_add(win);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, panes);
   evas_object_show(panes);

   /* Right-side panes */
   panes_h = elm_panes_add(win);
   elm_panes_horizontal_set(panes_h, EINA_TRUE);
   evas_object_size_hint_weight_set(panes_h, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_h, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panes_h);

   /* Rootpath Label widget */
   rootpath_label = elm_label_add(win);
   elm_label_line_wrap_set(rootpath_label, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(rootpath_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(rootpath_label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(rootpath_label);
   elm_box_pack_end(bigbox, rootpath_label);

   size_label = elm_label_add(win);
   elm_label_line_wrap_set(size_label, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(size_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(size_label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(size_label);
   elm_box_pack_end(bigbox, size_label);

   /* Entry widget */
   entry = elm_entry_add(win);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_entry_single_line_set(entry, EINA_TRUE);
   elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limit_size);
   evas_object_show(entry);
   elm_box_pack_end(bigbox, entry);

   /* Thumb widget */
   elm_need_ethumb();
   thumb = elm_thumb_add(win);
   evas_object_smart_callback_add(thumb, "generate,error", _generation_error_cb, NULL);
   evas_object_size_hint_weight_set(thumb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(thumb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(thumb);
   elm_thumb_editable_set(thumb, EINA_FALSE);

   /* Add box on top right panel */
   elm_object_part_content_set(panes_h, "left", bigbox);
   elm_object_part_content_set(panes_h, "bottom", thumb);
   /* Add file view in left panel */
   elm_object_part_content_set(panes, "left", genlist);
   /* Add right panel in main panel */
   elm_object_part_content_set(panes, "right", panes_h);

   /* Define widget properties */
   eo_do(evf, elm_view_form_widget_add("filename", rootpath_label));
   eo_do(evf, elm_view_form_widget_add("size", size_label));
   eo_do(evf, elm_view_form_widget_add("path", entry));
   eo_do(evf, elm_view_form_widget_add("path", thumb));

   /* cleanup */
   elm_run();
   elm_shutdown();
   eo_unref(model);
   eo_unref(evf);
   eio_shutdown();
   ecore_shutdown();
   free(child_data);
   return 0;
}
ELM_MAIN()

