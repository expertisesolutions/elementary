//Compile with:
// gcc -o fileviewlist fileviewlist.c `pkg-config --cflags --libs emodel`

//#ifdef HAVE_CONFIG_H
# include "../../elementary_config.h"
//#endif

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <emodel_eio.eo.h>
#include <stdio.h>

#define EMODEL_TEST_FILENAME_PATH "/tmp"

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *filemodel, *fileview;
   Evas_Object *win;
   Evas_Object *genlist;

   win = elm_win_util_standard_add("viewlist", "Viewlist");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   ecore_init();

   genlist = elm_genlist_add(win);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(genlist);

   filemodel = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   fileview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(genlist, filemodel));
   eo_do(fileview, elm_view_list_property_connect("filename", "elm.text"));
   eo_do(fileview, elm_view_list_property_connect("icon", "elm.swallow.icon"));

   evas_object_resize(win, 320, 520);
   elm_win_resize_object_add(win, genlist);
   evas_object_show(win);

   elm_run();
   eo_unref(fileview);
   //eo_unref(filemodel);
   elm_shutdown();

   ecore_shutdown();

   return 0;
}
ELM_MAIN()

