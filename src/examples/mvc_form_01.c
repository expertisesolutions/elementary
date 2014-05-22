//Compile with:
// gcc -o fileviewform fileviewform.c `pkg-config --cflags --libs emodel`

# include "../../elementary_config.h"

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <elm_view_form.h>
#include <stdio.h>

#define FILEMODEL_PATH "/tmp"
#define DEFAULT_THUMBNAIL "../../data/images/logo.png"

struct _Form_Child_Data
{
   Eo *evf, *model;
   Eina_Value *value_prev;
   Eina_Bool is_dir;
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
 * @brief Parent property change
 */
static Eina_Bool
_prop_change_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;
   fprintf(stdout, "property '%s' changed to '%s'\n", evt->prop, eina_value_to_string(evt->value));
   return EINA_TRUE;
}

/**
 * @brief Child property change
 */ 
static Eina_Bool
_child_prop_change_cb(void *data EINA_UNUSED, Eo *obj, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;
   Form_Child_Data *child_data = data;
   if(!strncmp(evt->prop, "is_dir", 6))
     {
        child_data->is_dir = atoi(eina_value_to_string(evt->value));
        eo_do(obj, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _child_prop_change_cb, child_data));
        eo_do(obj, emodel_property_get("filename"));

     }
   else if(!strncmp(evt->prop, "filename", 8))
     {
        if(EINA_TRUE == child_data->is_dir) //TODO: reload model to new directory
          {
             child_data->value_prev = eina_value_new(EINA_VALUE_TYPE_STRING);
             eina_value_set(child_data->value_prev, dirname(eina_value_to_string(evt->value))); 
             eo_do(child_data->evf, elm_view_form_widget_set("current", evt->value));
          }
        else
          {
             eo_do(child_data->evf, elm_view_form_widget_set("thumb", evt->value));
             if(NULL != child_data->value_prev)
               {
                  eo_do(child_data->evf, elm_view_form_widget_set("current", child_data->value_prev));
               }
          }
     }
   return EINA_TRUE;
}

static Eina_Bool
_child_selected_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Form_Child_Data *child_evt = data;
   Emodel_Children_EVT *evt = event_info;
   EINA_SAFETY_ON_NULL_RETURN_VAL(child_evt->evf, EINA_FALSE);
   eo_ref(evt->child);

   child_evt->is_dir = EINA_FALSE;

   eo_do(evt->child, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _child_prop_change_cb, child_evt));
   eo_do(evt->child, emodel_property_get("is_dir"));
   return EINA_TRUE;
}

/**
 * Thumb widget error callback
 */
static void
_generation_error_cb(void *data, Evas_Object *o, void *event_info)
{
   Eo *evf = data;
   Eina_Value *value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(value, DEFAULT_THUMBNAIL); 
   fprintf(stderr, "%s:%d thumbnail generation error, loading default %s.\n", 
           __FUNCTION__, __LINE__, DEFAULT_THUMBNAIL);
   eo_do(evf, elm_view_form_widget_set("thumb", value));
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *fileview, *evf, *model;
   Eina_Value *value;
   Evas_Object *win, *panes, *panes_h, *bigbox;
   Evas_Object *label, *thumb, *entry, *genlist;
   Form_Child_Data *child_data = calloc(1, sizeof(Form_Child_Data));
   EINA_SAFETY_ON_NULL_RETURN_VAL(child_data, 1);

   ecore_init();

   model = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(FILEMODEL_PATH));
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, 1);

   evf = eo_add_custom(ELM_OBJ_VIEW_FORM_CLASS, NULL, elm_view_form_constructor(model));
   EINA_SAFETY_ON_NULL_RETURN_VAL(evf, 1);

   child_data->evf = evf;
   child_data->model = model;
   eo_do(model, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(model, eo_event_callback_add(EMODEL_CHILD_SELECTED_EVT, _child_selected_cb, child_data));


   /* for entry widget */
   static Elm_Entry_Filter_Limit_Size limit_size = {
        .max_char_count = 32, // max chars number
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
   fileview = eo_add_custom(ELM_OBJ_VIEW_LIST_CLASS, NULL, elm_view_list_add(genlist, model));
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

   /* Label widget */
   label = elm_label_add(win);
   elm_label_line_wrap_set(label, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(label);
   elm_box_pack_end(bigbox, label);

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
   evas_object_smart_callback_add(thumb, "generate,error", _generation_error_cb, evf);
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
   eo_do(evf, elm_view_form_widget_add("current", label));
   eo_do(evf, elm_view_form_widget_add("search", entry));
   eo_do(evf, elm_view_form_widget_add("thumb", thumb));

   /* Define default widget values*/
   value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(value, FILEMODEL_PATH);
   eo_do(evf, elm_view_form_widget_set("current", value));

   value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(value, "Type filename");
   eo_do(evf, elm_view_form_widget_set("search", value));
   
   value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(value, "/usr/share/I_Dont_Exist.png"); 
   eo_do(evf, elm_view_form_widget_set("thumb", value));

    /* query values */
   eo_do(evf, elm_view_form_widget_get("current"));
   eo_do(evf, elm_view_form_widget_get("search"));
   eo_do(evf, elm_view_form_widget_get("thumb"));

   // cleanup
   elm_run();
   eo_unref(model);
   eo_unref(evf);
   elm_shutdown();
   ecore_shutdown();
   free(child_data);
   return 0;
}
ELM_MAIN()

