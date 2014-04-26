//Compile with:
// gcc -o fileviewform fileviewform.c `pkg-config --cflags --libs emodel`

# include "../../elementary_config.h"

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <elm_view_form.h>
#include <stdio.h>

#define EMODEL_TEST_FILENAME_PATH "/tmp"

struct _Form_Example_Data
{
   Eo *model;
   Evas_Object *mainwin;
   Evas_Object *bigbox;
};
typedef struct _Form_Example_Data Form_Example_Data;

static 
Form_Example_Data *_getformdata(void)
{
   static Form_Example_Data fdata;
   static Eina_Bool lock = EINA_FALSE;

   if(lock == EINA_FALSE)
     {
        lock = EINA_TRUE;
        memset(&fdata, 0, sizeof(Form_Example_Data));
     }

   return &fdata;
}

#include "fileviewform_evas_callbacks.i"

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *filemodel;
   Eo *form;
   Evas_Object *formwin;
   Evas_Object *widget;
   Evas_Object *label;

   ecore_init();

   Form_Example_Data *priv = _getformdata();

   priv->mainwin = elm_win_util_standard_add("form_test", "Form_test");
   EINA_SAFETY_ON_NULL_RETURN_VAL(priv->mainwin, -1);

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   /* sets the window to be destroyed automatically by signal */
   elm_win_autodel_set(priv->mainwin, EINA_TRUE);
   /* enable focus */
   elm_win_focus_highlight_enabled_set(priv->mainwin, EINA_TRUE);
   evas_object_smart_callback_add(priv->mainwin, "focus,in", _win_focused_cb, "mainwindow");

   /* define window delete callback */
   evas_object_smart_callback_add(priv->mainwin, "delete,request", _main_win_del_cb, NULL);

   priv->bigbox = elm_box_add(priv->mainwin);
   elm_box_horizontal_set(priv->bigbox, EINA_TRUE);
   evas_object_size_hint_weight_set(priv->bigbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(priv->mainwin, priv->bigbox);
   evas_object_show(priv->bigbox);

   evas_object_resize(priv->mainwin, 200, 170);
   evas_object_show(priv->mainwin);

   label = elm_label_add(priv->bigbox);

#if 0
   elm_object_text_set(label, "test");
   elm_label_line_wrap_set(label, ELM_WRAP_CHAR);
   elm_box_pack_end(priv->bigbox, label);
   evas_object_show(label);
   elm_run();
#endif
#if 0
   form = eo_add_custom(ELM_OBJ_VIEW_FORM_CLASS, NULL, elm_view_form_setup("viewform", "Viewform"));

   fprintf(stdout, "Got: form=%p\n", form);

   formwin = elm_win_util_standard_add("form_viewform", "form_Viewform");
   fprintf(stdout, "Got: formwin=%p\n", formwin);

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(formwin, EINA_TRUE);

   //eo_do(form, elm_view_list_evas_object_get(&widget));

   filemodel = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   fprintf(stdout, "Got: filemodel=%p\n", filemodel);
   eo_do(form, elm_view_form_list_create(formwin, filemodel));

   elm_run();
   elm_shutdown();

   ecore_shutdown();
#endif
   return 0;
}
ELM_MAIN()

