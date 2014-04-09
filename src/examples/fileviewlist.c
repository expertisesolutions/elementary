//Compile with:
// gcc -o fileviewlist fileviewlist.c `pkg-config --cflags --libs emodel`

//#ifdef HAVE_CONFIG_H
# include "../../elementary_config.h"
//#endif

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <stdio.h>

#define EMODEL_TEST_FILENAME_PATH "/tmp"

//int main(int argc, char **argv)

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *filemodel, *fileview;
   Ecore_Timer *timer;
   Evas_Object *win;
   Evas_Object *list;
   int i;

   win = elm_win_util_standard_add("viewlist", "Viewlist");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   ecore_init();

   filemodel = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   fileview = eo_add_custom(ELM_OBJ_VIEW_LIST_CLASS, NULL, elm_view_list_add(win, filemodel));


   evas_object_resize(win, 320, 320);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   eo_unref(filemodel);
   ecore_shutdown();
}
ELM_MAIN()

