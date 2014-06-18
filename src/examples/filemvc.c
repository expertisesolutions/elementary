//#ifdef HAVE_CONFIG_H
# include "../../elementary_config.h"
//#endif

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <stdio.h>
#include <Eio.h>

#define EMODEL_TEST_FILENAME_PATH "/tmp"

struct _Emodel_Test_Filemvc_Data
{
   Eo *fileview;
   Eo *treeview;
   Eo *treemodel;
};
typedef struct _Emodel_Test_Filemvc_Data Emodel_Test_Filemvc_Data;

static void
_cleanup_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Emodel_Test_Filemvc_Data *priv = (Emodel_Test_Filemvc_Data *)data;
   eo_unref(priv->fileview);
   eo_unref(priv->treeview);
   eo_unref(priv->treemodel);
}

static Eina_Bool
_filter_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info)
{
   if (info->type == EINA_FILE_DIR) return EINA_TRUE;

   return EINA_FALSE;
}

static Eina_Bool
_list_selected_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   printf("LIST selected model\n");
   return EINA_TRUE;
}

static Eina_Bool
_tree_selected_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   printf("TREE selected model\n");
   Eo *model, *view = data;
   Emodel_Children_EVT *evt = event_info;
   const char *path;

   eo_do(evt->child, emodel_eio_path_get(&path));
   model = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(path));
   eo_do(model, eo_event_callback_add(EMODEL_EVENT_CHILD_SELECTED, _list_selected_cb, NULL));

   eo_do(view, elm_view_list_model_set(model));
   eo_unref(model);
   return EINA_TRUE;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Emodel_Test_Filemvc_Data priv;
   Evas_Object *win, *box, *genlist;

   memset(&priv, 0, sizeof(Emodel_Test_Filemvc_Data));

   win = elm_win_util_standard_add("viewlist", "Viewlist");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);

   ecore_init();

   priv.treemodel = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   eo_do(priv.treemodel, emodel_eio_children_filter_set(_filter_cb, NULL));

   genlist = elm_genlist_add(win);
   priv.treeview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(genlist, priv.treemodel));
   eo_do(priv.treeview, elm_view_list_property_connect("filename", "elm.text"),
                   elm_view_list_property_connect("icon", "elm.swallow.icon"));
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, genlist);
   evas_object_show(genlist);

   genlist = elm_genlist_add(win);
   priv.fileview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(genlist, NULL));
   eo_do(priv.fileview, elm_view_list_property_connect("filename", "elm.text"),
                   elm_view_list_property_connect("icon", "elm.swallow.icon"));
   evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _cleanup_cb, &priv);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, genlist);
   evas_object_show(genlist);

   eo_do(priv.treemodel, eo_event_callback_add(EMODEL_EVENT_CHILD_SELECTED, _tree_selected_cb, priv.fileview));

   evas_object_resize(win, 600, 520);
   evas_object_show(box);
   evas_object_show(win);

   elm_run();
   elm_shutdown();
   ecore_shutdown();

   return 0;
}
ELM_MAIN()

