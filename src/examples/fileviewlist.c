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

struct _Emodel_Test_Fileview_Data
{
   Eo *filemodel;
   Eo *fileview;
};
typedef struct _Emodel_Test_Fileview_Data Emodel_Test_Fileview_Data;

static void
_cleanup_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Emodel_Test_Fileview_Data *priv = (Emodel_Test_Fileview_Data *)data;
   eo_unref(priv->fileview);
   eo_unref(priv->filemodel);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Emodel_Test_Fileview_Data priv;
   Evas_Object *win = NULL;
   Evas_Object *genlist = NULL;

   memset(&priv, 0, sizeof(Emodel_Test_Fileview_Data));

   elm_init(argc, argv);
   ecore_init();

   win = elm_win_util_standard_add("viewlist", "Viewlist");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   genlist = elm_genlist_add(win);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(genlist);

   priv.filemodel = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   priv.fileview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(priv.filemodel, genlist, ELM_GENLIST_ITEM_TREE, NULL));
   evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _cleanup_cb, &priv);

   eo_do(priv.fileview, elm_view_list_property_connect("filename", "elm.text"));
   eo_do(priv.fileview, elm_view_list_property_connect("icon", "elm.swallow.icon"));

   evas_object_resize(win, 320, 520);
   elm_win_resize_object_add(win, genlist);
   evas_object_show(win);

   elm_run();

   elm_shutdown();
   ecore_shutdown();

   return 0;
}
ELM_MAIN()

