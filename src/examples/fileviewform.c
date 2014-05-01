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
};
typedef struct _Form_Widget Form_Widget;


struct _Form_Example_Data
{
   Eo *evf; /**< Elm View Form object */
   Evas_Object *mainwin;
   Evas_Object *bigbox;

   Form_Widget widget;
};
typedef struct _Form_Example_Data Form_Example_Data;

#include "fileviewform_evas_callbacks.i"

/**
 * Local class
 * begin
 */
EAPI Eo_Op FILE_VIEW_FORM_CLASS_BASE_ID = 0;

enum {
     FILE_VIEW_OBJ_SUB_ID_ADD_WIDGETS,
     FILE_VIEW_OBJ_SUB_ID_GET_VIEW_OBJECT,
     FILE_VIEW_OBJ_SUB_ID_LAST
};

#define FILE_VIEW_FORM_CLASS_ID(sub_id) (FILE_VIEW_FORM_CLASS_BASE_ID + sub_id)

#define form_constructor() EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR)
#define form_add_widget(propname) FILE_VIEW_FORM_CLASS_ID(FILE_VIEW_OBJ_SUB_ID_ADD_WIDGETS), EO_TYPECHECK(char *, propname)
#define form_view_object_get(object) FILE_VIEW_FORM_CLASS_ID(FILE_VIEW_OBJ_SUB_ID_GET_VIEW_OBJECT), EO_TYPECHECK(Eo **, object)

#define FILE_VIEW_FORM_CLASS file_view_form_class_get()
const Eo_Class *file_view_form_class_get(void);

#define MY_CLASS FILE_VIEW_FORM_CLASS
#define MY_CLASS_NAME "File View Form Example"

static void
_form_constructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Form_Example_Data *priv = class_data;
   
   eo_do_super(obj, MY_CLASS, eo_constructor());
   
   priv->evf = eo_add_custom(ELM_OBJ_VIEW_FORM_CLASS, NULL, elm_view_form_constructor(obj));

   /**
    * @brief Window setup
    */
   priv->mainwin = elm_win_util_standard_add("form_test", "Form Test");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(priv->mainwin, EINA_TRUE); /**< sets the window to be destroyed automatically by signal */
   elm_win_focus_highlight_enabled_set(priv->mainwin, EINA_TRUE); /**< enavle focus */
   evas_object_smart_callback_add(priv->mainwin, "focus,in", _win_focused_cb, "mainwindow");
   evas_object_smart_callback_add(priv->mainwin, "delete,request", _main_win_del_cb, "mainwindow"); /**< define window delete callback */
   evas_object_resize(priv->mainwin, 250, 220);
   evas_object_show(priv->mainwin);

   /**
    * @brief Box setup
    */
   priv->bigbox = elm_box_add(priv->mainwin);
   //elm_box_horizontal_set(priv->bigbox, EINA_TRUE);
   evas_object_size_hint_weight_set(priv->bigbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(priv->mainwin, priv->bigbox);
   evas_object_show(priv->bigbox);

   /**
    * @brief Label widget setup
    */
   priv->widget.label = elm_label_add(priv->mainwin);
   elm_label_line_wrap_set(priv->widget.label, ELM_WRAP_CHAR);
   evas_object_size_hint_weight_set(priv->widget.label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(priv->widget.label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(priv->bigbox, priv->widget.label);
   evas_object_show(priv->widget.label);

}

static void
_form_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_destructor());
}

static void
_form_add_widget(Eo *obj, void *class_data, va_list *list)
{
   Form_Example_Data *priv = class_data;
   char *propname = va_arg(*list, char *);

   eo_do(priv->evf, elm_view_form_widget_add(propname, priv->widget.label)); //FIXME
}

static void
_form_view_object_get(Eo *obj, void *class_data, va_list *list)
{
   Form_Example_Data *priv = class_data;
   Eo **evf = va_arg(*list, Eo **);
   *evf = priv->evf;
}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _form_constructor),
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _form_destructor),
        EO_OP_FUNC(FILE_VIEW_FORM_CLASS_ID(FILE_VIEW_OBJ_SUB_ID_ADD_WIDGETS), _form_add_widget),
        EO_OP_FUNC(FILE_VIEW_FORM_CLASS_ID(FILE_VIEW_OBJ_SUB_ID_GET_VIEW_OBJECT), _form_view_object_get),
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}
static const Eo_Op_Description _op_descs[] = {
     EO_OP_DESCRIPTION(FILE_VIEW_OBJ_SUB_ID_ADD_WIDGETS, "Add form widgets"),
     EO_OP_DESCRIPTION(FILE_VIEW_OBJ_SUB_ID_GET_VIEW_OBJECT, "Get view form object"),
     EO_OP_DESCRIPTION_SENTINEL
};
static Eo_Class_Description _class_descs = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&FILE_VIEW_FORM_CLASS_BASE_ID, _op_descs, FILE_VIEW_OBJ_SUB_ID_LAST),
     NULL,
     sizeof(Form_Example_Data),
     _class_constructor,
     NULL
};
EO_DEFINE_CLASS(file_view_form_class_get, &_class_descs, ELM_OBJ_VIEW_FORM_CLASS, NULL);

/**
 * Local class
 * end
 */


EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *formmodel;
   Eo *evf;

   ecore_init();

   formmodel = eo_add_custom(FILE_VIEW_FORM_CLASS, NULL, form_constructor());
   
   eo_do(formmodel, form_add_widget("filename"));
   
   // Get elm_view_form object so we can invoke its functions from main
   eo_do(formmodel, form_view_object_get(&evf));
   
   Eina_Value *nameset = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(nameset, "/some/filename");
   
   elm_run();
   elm_shutdown();
   ecore_shutdown();
#if 0

   eo_do(formmodel, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(formmodel, eo_event_callback_add(EMODEL_PROPERTIES_CHANGE_EVT, _properties_cb, NULL));

   eo_do(formmodel, form_add_widgets());

   // Get elm_view_form object so we can invoke its functions from main
   eo_do(formmodel, form_view_object_get(&evf));

   Eina_Value *nameset = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(nameset, "I hear dead people.");

   eo_do(evf, elm_view_form_widget_set("first label", nameset));
 
   eo_do(formmodel, emodel_properties_get());


   elm_run();
   elm_shutdown();
   ecore_shutdown();
#endif
   return 0;
}
ELM_MAIN()

