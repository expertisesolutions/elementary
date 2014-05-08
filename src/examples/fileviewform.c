//Compile with:
// gcc -o fileviewform fileviewform.c `pkg-config --cflags --libs emodel`

# include "../../elementary_config.h"

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <elm_view_form.h>
#include <stdio.h>

#define FILENAME_PATH "/tmp"

struct _Form_Widget
{
   Evas_Object *label;
   Evas_Object *label2;
   Evas_Object *clock;
};
typedef struct _Form_Widget Form_Widget;

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
 * Local model class
 * begin
 */
EAPI Eo_Op FILE_VIEW_FORM_CLASS_BASE_ID = 0;

#define form_constructor() EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR)

#define FILE_VIEW_FORM_CLASS file_view_form_class_get()
const Eo_Class *file_view_form_class_get(void) EINA_CONST;

#define MY_CLASS FILE_VIEW_FORM_CLASS
#define MY_CLASS_NAME "File View Form Example"

static void
_form_constructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
}

static void
_form_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_destructor());
}


static void
_form_property_set(Eo *obj, void *class_data, va_list *list)
{
#if 0
   Emodel_Property_EVT evt;
   
   evt.prop= va_arg(*list, const char*);
   evt.value = va_arg(*list, Eina_Value *);

   EINA_SAFETY_ON_NULL_RETURN(evt.prop);
   EINA_SAFETY_ON_NULL_RETURN(evt.value);

   //TODO: modify this , we should not callback_call, instead we should call 
   //evf function and then dispatch the change_evt
   eo_do(obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
#endif
}

static void
_form_property_get(Eo *obj, void *class_data EINA_UNUSED, va_list *list)
{
#if 0
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = class_data;
   const char *prop_arg = va_arg(*list, const char*);

   EINA_SAFETY_ON_NULL_RETURN(prop_arg);  
   EINA_SAFETY_ON_NULL_RETURN(priv);  
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);  

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   if (!strncmp(prop_arg, evt.prop, strlen(evt.prop))) 
     {
        evt.value = _emodel_property_value_get(priv, evt.prop);
        eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
        return;
     }

   priv->file = eio_file_direct_stat(priv->path, _eio_stat_done_cb, _eio_error_cb, priv);
#endif
}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _form_constructor),
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _form_destructor),
        EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_PROPERTY_SET), _form_property_set),
        EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_PROPERTY_GET), _form_property_get),
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}
static const Eo_Op_Description _op_descs[] = {
     EO_OP_DESCRIPTION_SENTINEL
};
static Eo_Class_Description _class_descs = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&FILE_VIEW_FORM_CLASS_BASE_ID, _op_descs, 0),
     NULL,
     0L,
     _class_constructor,
     NULL
};
EO_DEFINE_CLASS(file_view_form_class_get, &_class_descs, EO_BASE_CLASS, EMODEL_CLASS, NULL);
/**
 * Local class
 * end
 */

static Eina_Bool
_prop_change_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;

   if(!strncmp(evt->prop, "toplabel", strlen("toplabel")) || !strncmp(evt->prop, "bottomlabel", strlen("bottomlabel")))
     {
        fprintf(stdout, "property \"%s\" changed to \"%s\"\n", evt->prop, eina_value_to_string(evt->value));
     }

   return EINA_TRUE;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *model;
   Eo *evf;
   Eina_Value *nameset;
   Eina_Value *nameset2;
   Evas_Object *mainwin, *bigbox;
   Evas_Object *label_A, *label_B;

   ecore_init();

   model = eo_add_custom(FILE_VIEW_FORM_CLASS, NULL, form_constructor());
   evf = eo_add_custom(ELM_OBJ_VIEW_FORM_CLASS, NULL, elm_view_form_constructor(model));
   
   eo_do(model, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   
   /**
    * @brief Window setup
    */
   mainwin = elm_win_util_standard_add("form_test", "Form Test");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(mainwin, EINA_TRUE); /**< sets the window to be destroyed automatically by signal */
   elm_win_focus_highlight_enabled_set(mainwin, EINA_TRUE); /**< enavle focus */
   evas_object_smart_callback_add(mainwin, "focus,in", _win_focused_cb, "mainwindow");
   evas_object_smart_callback_add(mainwin, "delete,request", _main_win_del_cb, "mainwindow"); /**< define window delete callback */
   evas_object_resize(mainwin, 250, 220);
   evas_object_show(mainwin);
   
   /**
    * @brief Box setup
    */
   bigbox = elm_box_add(mainwin);
   evas_object_size_hint_weight_set(bigbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(mainwin, bigbox);
   evas_object_show(bigbox);
   
   /**
    * @brief Label widget setup
    */
   label_A = elm_label_add(mainwin);
   elm_label_line_wrap_set(label_A, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(label_A, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label_A, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bigbox, label_A);
   evas_object_show(label_A);
   
   label_B = elm_label_add(mainwin);
   elm_label_line_wrap_set(label_B, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(label_B, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label_B, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bigbox, label_B);
   evas_object_show(label_B);
   
   eo_do(evf, elm_view_form_widget_add("toplabel", label_A)); 
   eo_do(evf, elm_view_form_widget_add("bottomlabel", label_B)); 

   nameset = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(nameset, "Top Label");

   eo_do(evf, elm_view_form_widget_set("toplabel", nameset));

   nameset2 = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(nameset2, "Bottom Label");
   eo_do(evf, elm_view_form_widget_set("bottomlabel", nameset2));

   // cleanup 
   elm_run();
   eo_unref(model);
   eo_unref(evf);
   elm_shutdown();
   ecore_shutdown();

   return 0;
}
ELM_MAIN()

