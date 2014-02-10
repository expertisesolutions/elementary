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
   Elm_Genlist_Item_Class *itc;
   Elm_View_Tree_Mode mode;
   Eo *model;
   Eina_List *items;
   Elm_View_Tree_Content_Get_Cb get_content_cb;
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

static inline void _update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent);
static void _item_sel_cb(void *data, Evas_Object *obj, void *event_info);

Elm_Object_Item *
_get_parent_item(Eina_List *items, Elm_Model_Tree_Path *path)
{
   View_Tree_ItemData *idata;
   Eina_List *l = NULL;
   unsigned int depth, *indices;
   unsigned int pdepth, *pindices;

   indices = elm_model_tree_path_get_indices(path, &depth);
   if (depth == 0 || depth == (unsigned int) -1)
     return NULL;

   EINA_LIST_FOREACH(items, l, idata)
     {
        EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);
        pindices = elm_model_tree_path_get_indices(idata->path, &pdepth);
        if (pdepth == depth-1 && memcmp(indices, pindices, sizeof(unsigned int) * pdepth) == 0)
          return idata->item;
     }

   return NULL;
}

static Eina_Bool
_model_tree_selected_cb(void *data EINA_UNUSED, Elm_Model_Tree_Path *path EINA_UNUSED)
{
   return EINA_TRUE; // TODO implement
}

static Eina_Bool
_model_tree_child_append_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info) // signature change
{

   Elm_Model_Tree_Path *path = (Elm_Model_Tree_Path*)event_info;
   Elm_Object_Item *pItem = NULL;
   Elm_View_Tree_Private *self = data;
   View_Tree_ItemData *idata = malloc(sizeof(View_Tree_ItemData));
   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, EINA_TRUE);

   idata->self = self;
   idata->path = path;
   idata->children = 0;

   pItem = _get_parent_item(self->items, idata->path);
   if (pItem)
     {
        View_Tree_ItemData *pdata = elm_object_item_data_get(pItem);
        EINA_SAFETY_ON_NULL_RETURN_VAL(pdata, EINA_TRUE);
        if (!pdata->children)
          {
             Elm_Object_Item *pItem_prev = elm_genlist_item_prev_get(pItem);
             Elm_Object_Item *pItem_parent = elm_genlist_item_parent_get(pItem);
             View_Tree_ItemData *npdata = malloc(sizeof(View_Tree_ItemData));
             memcpy(npdata, pdata, sizeof(View_Tree_ItemData));

             elm_object_item_del(pItem);
             if (pItem_prev != pItem_parent)
               pItem = elm_genlist_item_insert_after(self->list, self->itc, npdata, pItem_parent,
                                                    pItem_prev,
                                                    ELM_GENLIST_ITEM_TREE,
                                                    _item_sel_cb, NULL);
             else
               pItem = elm_genlist_item_prepend(self->list, self->itc, npdata, pItem_parent,
                                               ELM_GENLIST_ITEM_TREE,
                                               _item_sel_cb, NULL);

             npdata->item = pItem;
             elm_genlist_item_expanded_set(pItem, EINA_TRUE);
             elm_genlist_item_update(pItem);
          }
        pdata->children++;
     }

   idata->item = elm_genlist_item_append(self->list, self->itc, idata, pItem, ELM_GENLIST_ITEM_NONE, _item_sel_cb, NULL);
//   elm_genlist_item_expanded_set(idata->item, EINA_FALSE);
   self->items = eina_list_append(self->items, idata);

   return EINA_TRUE;
}

/*/////////////////////////////////////////////////////////////////////////////////
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
*//////////////////////////////////////////////////////////////////////////////////

////////////////////genlist callbacks
static char *
_item_label_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part)
{
   char *label = NULL;
   Eina_Value *value = NULL;
   View_Tree_ItemData *idata = data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);

   if (!strcmp(part, "elm.text"))
     {
        eo2_do(idata->self->model, elm_model_tree_value_get(idata->path, &value));
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
_item_del(void *data, Evas_Object *obj EINA_UNUSED)
{
   free(data);
}

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
   Elm_Object_Item *item = event_info;
   View_Tree_ItemData *idata = elm_object_item_data_get(item);

   printf("sel item data [%p] on genlist obj [%p], item pointer [%p], path [%s] children [%u]\n",
          data, obj, event_info, elm_model_tree_path_to_string(idata->path), idata->children);
}

static void
_expand_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   printf("expand request on item: %p\n", event_info);
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
_contract_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   printf("contract request on item: %p\n", event_info);
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

static void
_expanded_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
/*   Eina_List *l;
   Elm_Object_Item *glit = event_info;
   Node_Data *it_data, *d = elm_object_item_data_get(glit);
   Evas_Object *list = elm_object_item_widget_get(glit);

   Elm_Genlist_Item_Class *ic;

   EINA_LIST_FOREACH(d->children, l, it_data)
     {
        Elm_Object_Item *nitem;
        Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE;
        printf("expanding item: #%d from parent #%d\n", it_data->value, d->value);
        if (it_data->favorite)
          ic = _itfav;
        else if (it_data->children)
          {
             ic = _itp;
             type = ELM_GENLIST_ITEM_TREE;
          }
        else
          ic = _itc;

        nitem = elm_genlist_item_append(list, ic, it_data, glit,
                                        type, _item_sel_cb, NULL);
        elm_genlist_item_expanded_set(nitem, EINA_FALSE);
     }*/
}

static void
_contracted_cb(void *data EINA_UNUSED, Evas_Object *o EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

////////////////////////////////////////////////////////////////////////////
static inline void
_update_path(Elm_View_Tree_Private *self, Elm_Model_Tree_Path *path, Elm_Object_Item *parent)
{
   Eina_List *l, *cl = NULL;
   Elm_Model_Tree_Path *c_path;
   View_Tree_ItemData *idata = malloc(sizeof(View_Tree_ItemData));

   EINA_SAFETY_ON_NULL_RETURN(idata);
   idata->self = self;
   idata->path = path;
   idata->children = 0;

   idata->item = elm_genlist_item_append(self->list, self->itc, idata, parent, ELM_GENLIST_ITEM_NONE, _item_sel_cb, NULL);
   elm_genlist_item_expanded_set(idata->item, EINA_FALSE);

   self->items = eina_list_append(self->items, idata);
   //foreach model itens and update nodes to widget
   EINA_LIST_FOREACH(cl, l, c_path)
     {
        _update_path(self, c_path, idata->item);
     }
}

static inline void
_update_tree_widget(Elm_View_Tree_Private* self)
{
   Elm_Model_Tree_Path *path = elm_model_tree_path_new_from_string("");
   _update_path(self, path, NULL);
}

static void
_elm_view_tree_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());

   Evas_Object *parent = va_arg(*list, Evas_Object *);
   Eo *model = va_arg(*list, Eo *);
   Elm_View_Tree_Private *self = class_data;
   
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(model);
   EINA_SAFETY_ON_NULL_RETURN(parent);
   
   self->list = elm_genlist_add(parent);
   EINA_SAFETY_ON_NULL_RETURN(self->list);

   self->model = model;
   self->mode = ELM_VIEW_TREE_VIEWMODE_ALL;
   self->get_content_cb = NULL;
   
   self->itc = elm_genlist_item_class_new();
   EINA_SAFETY_ON_NULL_RETURN(self->itc);
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_label_get;
   self->itc->func.content_get = _item_content_get;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;

   evas_object_size_hint_weight_set(self->list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(self->list, "expand,request", _expand_request_cb, self);
   evas_object_smart_callback_add(self->list, "contract,request", _contract_request_cb, self);
   evas_object_smart_callback_add(self->list, "expanded", _expanded_cb, self);
   evas_object_smart_callback_add(self->list, "contracted", _contracted_cb, self);

   _update_tree_widget(self);

   eo_do(self->model, eo_event_callback_add(ELM_MODEL_TREE_CONST_SELECT_EVT, _model_tree_selected_cb, self));
   eo_do(self->model, eo_event_callback_add(TREE_CHILD_APPEND_EVT, _model_tree_child_append_cb, self));
/*
   eo2_do(self->model, elm_model_tree_node_deleted_callback_add(_model_node_deleted_cb, list);
   eo2_do(self->model, elm_model_tree_node_changed_callback_add(_model_node_changed_cb, list);
   eo2_do(self->model, elm_model_tree_reordered_callback_add(_model_reordered_cb, list);
*/
}


static void
_elm_view_tree_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{

   Elm_View_Tree_Private *self = class_data;
   EINA_SAFETY_ON_NULL_RETURN(self);
   elm_genlist_item_class_free(self->itc);
   eo_do_super(obj, MY_CLASS, eo_destructor()); 
   //XXX destruct evas obj?
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
   EINA_SAFETY_ON_NULL_RETURN(mode);
   
   if (self->mode != mode)
     {
        self->mode = mode;
        _update_tree_widget(self);
     }
}

static void
_elm_view_tree_getcontent_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Tree_Private *self = class_data;
   self->get_content_cb = va_arg(*list, Elm_View_Tree_Content_Get_Cb);
}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_tree_destructor),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_GETCONTENT_SET), _elm_view_tree_getcontent_set),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_MODE_SET), _elm_view_tree_mode_set),
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_ADD), _elm_view_tree_add), 
      EO_OP_FUNC(ELM_VIEW_ID(EVT_SUB_ID_TREE_EVAS_OBJECT_GET), _elm_view_tree_evas_object_get), 
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_GETCONTENT_SET, "Set content callback"), 
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_MODE_SET, "Set view mode"), 
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_ADD, "Setup tree object"),
   EO_OP_DESCRIPTION(EVT_SUB_ID_TREE_EVAS_OBJECT_GET, "Return Evas object list"), 
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description class_descs = {
   EO_VERSION,
   "View Tree",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_VIEW_TREE_BASE_ID, op_descs, EVT_SUB_ID_TREE_LAST),
   NULL,
   sizeof(Elm_View_Tree_Private),
   _class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_view_tree_class_get, &class_descs, EO_BASE_CLASS, NULL);

