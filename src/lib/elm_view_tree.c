#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Eo.h>
#include <Elementary.h>

#include "elm_priv.h"
#include <assert.h>

#define MY_CLASS ELM_VIEW_TREE_CLASS
EAPI Eo_Op ELM_VIEW_TREE_BASE_ID = 0;

struct _Elm_View_Tree_Private
{
   Evas_Object *list;
   Eo *model;
   Elm_Genlist_Item_Class *itc;
   Elm_View_Tree_Mode mode;
   Eina_List *items;
   Elm_Model_Tree_Path *rootpath;
   Elm_View_Tree_Content_Get_Cb get_content_cb;
   Elm_View_Tree_Expanded_Get_Cb get_expanded_cb;
};
typedef struct _Elm_View_Tree_Private Elm_View_Tree_Private;

struct _View_Tree_ItemData
{
  Elm_View_Tree_Private *self;
  Elm_Model_Tree_Path *path;
  Elm_Object_Item *item;
  int children;
};
typedef struct _View_Tree_ItemData View_Tree_ItemData;

static void _update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path);
static void _append_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, View_Tree_ItemData *pdata);
static void _item_sel_cb(void *data, Evas_Object *obj, void *event_info);
//static View_Tree_ItemData *_get_parent_data(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path);

/* --- Private Func --- */
static View_Tree_ItemData *
_get_data(Eina_List *items, Elm_Model_Tree_Path *path)
{
   View_Tree_ItemData *data;
   Eina_List *l = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(items, NULL);
   EINA_LIST_FOREACH(items, l, data)
     {
        EINA_SAFETY_ON_NULL_RETURN_VAL(data, NULL);
        if (data->path == path)
          return data;
     }
   return NULL;
}

static View_Tree_ItemData *
_get_parent_data(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path)
{
   View_Tree_ItemData *idata;
   Eina_List *l = NULL;
   unsigned int depth, *indices;
   unsigned int pdepth, *pindices;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   indices = elm_model_tree_path_get_indices(path, &depth);
   if (depth == 0 || depth == (unsigned int) -1)
     return NULL;

   if (depth == 1)
     return _get_data(self->items, self->rootpath);

   EINA_LIST_FOREACH(self->items, l, idata)
     {
        EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);
        pindices = elm_model_tree_path_get_indices(idata->path, &pdepth);
        if (pdepth == depth-1 && memcmp(indices, pindices, sizeof(unsigned int) * pdepth) == 0)
          return idata;
     }
   return NULL;
}

Eina_Bool
_expanded_is(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path)
{
   unsigned int cc = 0;
   EINA_SAFETY_ON_NULL_RETURN_VAL(self, EINA_TRUE);
   if (self->get_expanded_cb)
     return self->get_expanded_cb(self->model, path);

   eo_do(self->model, elm_model_tree_children_count(NULL, path, &cc));
   return cc > 0;
}

static void
_append_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, View_Tree_ItemData *pdata)
{
   View_Tree_ItemData *idata;
   Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE;
   Elm_Object_Item *pitem = NULL;
   EINA_SAFETY_ON_NULL_RETURN(path);
   EINA_SAFETY_ON_NULL_RETURN(self);

   //if (_get_data(self->items, path) != NULL) return;

   if (pdata)
     {
        if (elm_genlist_item_type_get(pdata->item) != ELM_GENLIST_ITEM_TREE)
          {
             Elm_Object_Item *pItem_prev = elm_genlist_item_prev_get(pdata->item);
             Elm_Object_Item *pItem_parent = elm_genlist_item_parent_get(pdata->item);

             View_Tree_ItemData *sdata = malloc(sizeof(View_Tree_ItemData));
             memcpy(sdata, pdata, sizeof(View_Tree_ItemData));
             elm_object_item_del(pdata->item); //TODO don't free or copy??
             pdata = sdata;
             if (pItem_prev != pItem_parent)
               pdata->item = elm_genlist_item_insert_after(self->list, self->itc, pdata, pItem_parent,
                                                    pItem_prev,
                                                    ELM_GENLIST_ITEM_TREE,
                                                    _item_sel_cb, NULL);
             else
               pdata->item = elm_genlist_item_prepend(self->list, self->itc, pdata, pItem_parent,
                                               ELM_GENLIST_ITEM_TREE,
                                               _item_sel_cb, NULL);

             self->items = eina_list_append(self->items, pdata);
             elm_genlist_item_update(pdata->item);
          }

        if (!elm_genlist_item_expanded_get(pdata->item))
          {
             elm_genlist_item_expanded_set(pdata->item, EINA_TRUE);
             //elm_model_tree_path_free(path);
             return;
          }
        pdata->children++;
        pitem = pdata->item;
     }

   idata = malloc(sizeof(View_Tree_ItemData));
   EINA_SAFETY_ON_NULL_RETURN(idata);
   idata->self = self;
   idata->path = path;
   idata->children = 0;

   if (_expanded_is(self, path))
     type = ELM_GENLIST_ITEM_TREE;
   else
     type = ELM_GENLIST_ITEM_NONE;

   idata->item = elm_genlist_item_append(self->list, self->itc, idata, pitem, type, _item_sel_cb, NULL);
   elm_genlist_item_expanded_set(idata->item, EINA_FALSE);
   self->items = eina_list_append(self->items, idata);
}

static void
_update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path)
{
   //   Eina_List *l, *cl = NULL;
   //   Elm_Model_Tree_Path *c_path;
   View_Tree_ItemData *data;

   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(path);

   printf("%s:%d\n", __FUNCTION__, __LINE__);
   data = _get_data(self->items, path);
   printf("%s:%d\n", __FUNCTION__, __LINE__);
   if(!data)
     {
        _append_path(self, path, _get_parent_data(self, path));
        return;
     }

   elm_genlist_item_update(data->item);
   /*
   //foreach model itens and update nodes to widget
   eo2_do(self->model, cl = elm_model_tree_children_get (idata->path));
   EINA_LIST_FOREACH(cl, l, c_path)
   {
   _update_path(self, c_path);
   }
   */
}

/* --- Tree Model Callbacks --- */
static Eina_Bool
_model_tree_selected_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED) 
{
   return EINA_TRUE; // TODO implement
}

static Eina_Bool
_model_tree_child_append_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{

   Elm_Model_Tree_Path *path = (Elm_Model_Tree_Path*)event_info;
   Elm_View_Tree_Private *self = data;
   _append_path(self, path, _get_parent_data(self, path));

   return EINA_TRUE;
}

/*
static Eina_Bool
_model_node_deleted_cb(void *data, Elm_Model_Tree_Path path)
{
   return EINA_TRUE;
}

static Eina_Bool
_model_node_changed_cb(void *data, Elm_Model_Tree_Path path)
{
   return EINA_TRUE;
}

static Eina_Bool
_model_reordered_cb(void *data)
{
   return EINA_TRUE;
}
*/

/* --- Genlist Callbacks --- */
static void
_item_del(void *data, Evas_Object *obj EINA_UNUSED)
{
   //printf("ELM_TREE_VIEW_DBG: %s:%d data=%p\n",__FUNCTION__, __LINE__, data);
   //TODO: resolve some memory leaks and find a way to free memory in expanded/contracted
   View_Tree_ItemData *idata = data;
   idata->self->items = eina_list_remove(idata->self->items, idata);
   //elm_model_tree_path_free(idata->path);
   free(data);
}


static char *
_item_label_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part)
{
   char *label = NULL;
   Eina_Value *value = NULL;
   View_Tree_ItemData *idata = data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);

   if (!strcmp(part, "elm.text"))
     {
        eo_do(idata->self->model, elm_model_tree_value_get(idata->path, &value));
        if (value)
          label = eina_value_to_string(value);
     }

   return label;
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   Elm_View_Tree_Private *self;
   View_Tree_ItemData *idata = data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);
   self = idata->self;
   EINA_SAFETY_ON_NULL_RETURN_VAL(self, NULL);

   if (self->get_content_cb != NULL)
     return self->get_content_cb(self->model, idata->path, obj, part);

   Evas_Object *ic = elm_icon_add(obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ic, NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        if (idata->children == 0)
          elm_icon_standard_set(ic, "file");
        else
          elm_icon_standard_set(ic, "folder");
     }

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
_item_sel_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   View_Tree_ItemData *idata = elm_object_item_data_get(item);
}

static void
_expand_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   elm_genlist_item_expanded_set(item, EINA_TRUE);
}

static void
_contract_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *item = event_info;
   elm_genlist_item_expanded_set(item, EINA_FALSE);
}

static void
_expanded_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Model_Tree_Path *c_path;
   Eina_List *l, *children = NULL;
   Elm_Object_Item *item = event_info;
   Elm_View_Tree_Private *self = data;

   View_Tree_ItemData *idata = elm_object_item_data_get(item);

   EINA_SAFETY_ON_NULL_RETURN(idata);

   eo_do(self->model, elm_model_tree_children_get(NULL, idata->path, &children));
   EINA_SAFETY_ON_NULL_RETURN(children);
   idata->children = eina_list_count(children);

   EINA_LIST_FOREACH(children, l, c_path)
     {
        _append_path(self, c_path, idata);
     }

     eina_list_free(children);
}

static void
_contracted_cb(void *data EINA_UNUSED, Evas_Object *o EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

/*  -- Tree View -- */
static void
_elm_view_tree_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
   Elm_View_Tree_Private *self = class_data;
   Evas_Object *parent = va_arg(*list, Evas_Object *);
   Eo *model = va_arg(*list, Eo *);

   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(model);
   EINA_SAFETY_ON_NULL_RETURN(parent);

   self->list = elm_genlist_add(parent);
   EINA_SAFETY_ON_NULL_RETURN(self->list);

   self->model = model;
   self->mode = ELM_VIEW_TREE_VIEWMODE_ALL;
   self->get_content_cb = NULL;
   self->get_expanded_cb = NULL;

   self->itc = elm_genlist_item_class_new();
   EINA_SAFETY_ON_NULL_RETURN(self->itc);
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_label_get;
   self->itc->func.content_get = _item_content_get;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;
   
    self->rootpath = elm_model_tree_path_new_from_string("");
   _update_path(self, self->rootpath);

   evas_object_size_hint_weight_set(self->list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(self->list, "expand,request", _expand_request_cb, self);
   evas_object_smart_callback_add(self->list, "contract,request", _contract_request_cb, self);
   evas_object_smart_callback_add(self->list, "expanded", _expanded_cb, self);
   evas_object_smart_callback_add(self->list, "contracted", _contracted_cb, self);


   //TODO: check this - compiler warnings
   eo_do(self->model, eo_event_callback_add(ELM_MODEL_TREE_CONST_SELECTED_EVT, _model_tree_selected_cb, self));
   eo_do(self->model, eo_event_callback_add(ELM_MODEL_TREE_CONST_CHILD_APPENDED_EVT, _model_tree_child_append_cb, self));
/*
   eo2_do(self->model, elm_model_tree_node_deleted_callback_add(_model_node_deleted_cb, list);
   eo2_do(self->model, elm_model_tree_node_changed_callback_add(_model_node_changed_cb, list);
   eo2_do(self->model, elm_model_tree_reordered_callback_add(_model_reordered_cb, list);
*/
}

static void
_elm_view_tree_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{
   View_Tree_ItemData *idata;
   Elm_View_Tree_Private *self = (Elm_View_Tree_Private *)class_data;

   EINA_LIST_FREE(self->items, idata)
     {
        //elm_model_tree_path_free(idata->path); //TODO copy path mem in append path
        free(idata);
     }

   elm_model_tree_path_free(self->rootpath);
   elm_genlist_item_class_free(self->itc);
   eo_unref(self->model);
   //XXX destruct evas obj?
   eo_do_super(obj, MY_CLASS, eo_destructor());
}


static void
_elm_view_tree_evas_object_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Tree_Private *self = class_data;
   Evas_Object **e = va_arg(*list, Evas_Object **);
   *e = self->list;
}

static void
_elm_view_tree_mode_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Tree_Private *self = class_data;
   Elm_View_Tree_Mode mode = va_arg(*list, Elm_View_Tree_Mode);

   EINA_SAFETY_ON_NULL_RETURN(self);

   if (self->mode != mode)
     {
        self->mode = mode;
        //_update_path(self, NULL);
     }
}

static void
_elm_view_tree_getcontent_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Tree_Private *self = class_data;
   self->get_content_cb = va_arg(*list, Elm_View_Tree_Content_Get_Cb);
}

static void
_elm_view_tree_getexpanded_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Tree_Private *self = class_data;
   self->get_expanded_cb = va_arg(*list, Elm_View_Tree_Expanded_Get_Cb);
}

static void
_view_tree_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_tree_destructor),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_GETCONTENT_SET), _elm_view_tree_getcontent_set),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_GETEXPANDED_SET), _elm_view_tree_getexpanded_set),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_MODE_SET), _elm_view_tree_mode_set),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_ADD), _elm_view_tree_add),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_EVAS_OBJECT_GET), _elm_view_tree_evas_object_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description view_tree_op_descs[] = {
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_GETCONTENT_SET, "Set content callback"),
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_GETEXPANDED_SET, "Set expanded callback"),
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_MODE_SET, "Set view mode"),
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_ADD, "Setup tree object"),
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_EVAS_OBJECT_GET, "Return Evas object list"),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description view_tree_class_descs = {
   EO_VERSION,
   "View Tree",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_VIEW_TREE_BASE_ID, view_tree_op_descs, EVT_SUB_ID_TREE_LAST),
   NULL,
   sizeof(Elm_View_Tree_Private),
   _view_tree_class_constructor,
   NULL
};


EO_DEFINE_CLASS(elm_view_tree_class_get, &view_tree_class_descs, EO_BASE_CLASS, NULL);
