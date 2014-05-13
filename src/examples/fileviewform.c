//Compile with:
// gcc -o fileviewform fileviewform.c `pkg-config --cflags --libs emodel`

# include "../../elementary_config.h"

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <elm_view_form.h>
#include <stdio.h>

#define FILEMODEL_PATH "/tmp"

struct _Form_Widget
{
   Evas_Object *win, *bigbox;
   Evas_Object *label;
   Evas_Object *entry;
};
typedef struct _Form_Widget Form_Widget;

/**
 * Window callbacks
 */
static void
_win_focused_cb(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   const char *name = (const char *)data;
   fprintf(stdout, "%s:%d Window focused: %p:%s\n", __FUNCTION__, __LINE__, obj, name);
}
static void
_main_win_del_cb(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   const char *name = (const char *)data;
   fprintf(stdout, "%s:%d Window deleted: %p:%s\n", __FUNCTION__, __LINE__, obj, name);
}

/**
 * property change callback
 */
static Eina_Bool
_prop_change_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;
   fprintf(stdout, "property '%s' changed to '%s'\n", evt->prop, eina_value_to_string(evt->value));
   return EINA_TRUE;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{

   Form_Widget w;
   Eo *model;
   Eo *evf;
   Eina_Value *value;

   memset(&w, 0, sizeof(Form_Widget));

   ecore_init();

   model = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(FILEMODEL_PATH));
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, 1);

   evf = eo_add_custom(ELM_OBJ_VIEW_FORM_CLASS, NULL, elm_view_form_constructor(model));
   EINA_SAFETY_ON_NULL_RETURN_VAL(evf, 1);

   eo_do(model, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));

   // for entry widget
   static Elm_Entry_Filter_Limit_Size limit_size = {
        .max_char_count = 32, // max chars number
        .max_byte_count = 0
   };
   /**
    * @brief Window setup
    */
   w.win = elm_win_util_standard_add("form_test", "Form Test");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(w.win, EINA_TRUE); /**< sets the window to be destroyed automatically by signal */
   elm_win_focus_highlight_enabled_set(w.win, EINA_TRUE); /**< enavle focus */
   evas_object_smart_callback_add(w.win, "focus,in", _win_focused_cb, "window");
   evas_object_smart_callback_add(w.win, "delete,request", _main_win_del_cb, "window"); /**< define window delete callback */
   evas_object_resize(w.win, 250, 220);
   evas_object_show(w.win);

   /**
    * @brief Box setup
    */
   w.bigbox = elm_box_add(w.win);
   evas_object_size_hint_weight_set(w.bigbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(w.win, w.bigbox);
   evas_object_show(w.bigbox);

   /**
    * Top label
    */
   w.label = elm_label_add(w.win);
   elm_label_line_wrap_set(w.label, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(w.label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(w.label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(w.bigbox, w.label);
   evas_object_show(w.label);

   /*
    * text entry
    */
   w.entry = elm_entry_add(w.win);
   evas_object_size_hint_weight_set(w.entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(w.entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_entry_single_line_set(w.entry, EINA_TRUE);
   elm_entry_markup_filter_append(w.entry, elm_entry_filter_limit_size,
                                  &limit_size);
   elm_box_pack_end(w.bigbox, w.entry);
   evas_object_show(w.entry);

   // Add both widgets
   eo_do(evf, elm_view_form_widget_add("filename", w.label));
   eo_do(evf, elm_view_form_widget_add("entrytext", w.entry));

   // Set text value for label
   value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(value, FILEMODEL_PATH);
   eo_do(evf, elm_view_form_widget_set("filename", value));

   // Set default text value for entry
   value = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(value, "Default text");
   eo_do(evf, elm_view_form_widget_set("entrytext", value));

   // get both values
   eo_do(evf, elm_view_form_widget_get("filename"));
   eo_do(evf, elm_view_form_widget_get("entrytext"));

   // cleanup
   elm_run();
   eo_unref(model);
   eo_unref(evf);
   elm_shutdown();
   ecore_shutdown();
   return 0;
}
ELM_MAIN()

