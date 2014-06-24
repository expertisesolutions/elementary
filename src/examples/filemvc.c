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

static void
_widget_init(Evas_Object *widget, Evas_Object *box)
{
   if (box != NULL) {
     elm_box_pack_end(box, widget);
     elm_object_text_set(widget, "content");
   }
   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(widget);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Emodel_Test_Filemvc_Data priv;
   //Eo *fileview, *treemodel, *treeview;
   //Eo formview;
   Evas_Object *win, *box, *genlist;

   memset(&priv, 0, sizeof(Emodel_Test_Filemvc_Data));

   //win
   win = elm_win_util_standard_add("viewlist", "Viewlist");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);

   ecore_init();

   //treemodel
   priv.treemodel = eo_add_custom(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   eo_do(priv.treemodel, emodel_eio_children_filter_set(_filter_cb, NULL));

   //treeview
   genlist = elm_genlist_add(win);
   priv.treeview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(priv.treemodel, genlist, ELM_GENLIST_ITEM_TREE, NULL));
   eo_do(priv.treeview, elm_view_list_property_connect("filename", "elm.text"),
                   elm_view_list_property_connect("icon", "elm.swallow.icon"));
   _widget_init(genlist, NULL);
   elm_box_pack_end(box, genlist);

   //listview
   genlist = elm_genlist_add(win);
   priv.fileview = eo_add_custom(ELM_VIEW_LIST_CLASS, NULL, elm_view_list_constructor(NULL, genlist, ELM_GENLIST_ITEM_NONE, "double_label"));
   eo_do(priv.fileview, elm_view_list_property_connect("filename", "elm.text"),
                   elm_view_list_property_connect("path", "elm.text.sub"),
                   elm_view_list_property_connect("icon", "elm.swallow.icon"));
   evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _cleanup_cb, &priv);
   _widget_init(genlist, NULL);
   elm_box_pack_end(box, genlist);

   eo_do(priv.treemodel, eo_event_callback_add(EMODEL_EVENT_CHILD_SELECTED, _tree_selected_cb, priv.fileview));
   Evas_Object *bxr, *rootpath_label, *size_label, *entry;
   //formview
   bxr = elm_box_add(win);
   _widget_init(bxr, NULL);
   elm_box_pack_end(box, bxr);

   /* Rootpath Label widget */
   rootpath_label = elm_label_add(win);
   elm_label_line_wrap_set(rootpath_label, ELM_WRAP_CHAR);
   _widget_init(rootpath_label, bxr);

   /* Size Label widget */
   size_label = elm_label_add(win);
   elm_label_line_wrap_set(size_label, ELM_WRAP_CHAR);
   _widget_init(size_label, bxr);

   /* Entry widget */
   entry = elm_entry_add(win);
   elm_entry_single_line_set(entry, EINA_TRUE);
//   elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limit_size);
   _widget_init(entry, bxr);

//   formview = eo_add_custom(ELM_OBJ_VIEW_FORM_CLASS, NULL, elm_view_form_constructor(treemodel));

   //showall
   evas_object_resize(win, 600, 520);
   evas_object_show(box);
   evas_object_show(win);

   elm_run();
   elm_shutdown();
   ecore_shutdown();

   return 0;
}
ELM_MAIN()

