#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>
//#include <Emodel.h>

#include "elm_priv.h"
#include <assert.h>

/**
 * @brief Elm View List Class
 *
 */
EAPI Eo_Op ELM_VIEW_LIST_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_VIEW_LIST_CLASS
#define MY_CLASS_NAME "View List"

struct _Elm_View_List_Private;
typedef struct _Elm_View_List_Private Elm_View_List_Private;

struct _View_List_ItemData;
typedef struct _View_List_ItemData View_List_ItemData;


struct _Elm_View_List_Private
{
   Evas_Object *list;
   View_List_ItemData *rootdata;
   Elm_Genlist_Item_Class *itc;
   Eo *model;
};

struct _View_List_ItemData
{
  Elm_View_List_Private *self;
  Elm_Object_Item *item;
  Eo *model;
};

/* --- Genlist Callbacks --- */
static void
_item_del(void *data, Evas_Object *obj EINA_UNUSED)
{
   free(data);
}


static char *
_item_text_get(void *data, Evas_Object *obj, const char *part)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, NULL);
   View_List_ItemData *idata = data;
   EINA_SAFETY_ON_NULL_RETURN_VAL(idata, NULL);

   if (!strcmp(part, "elm.text"))
     {
     }

   return NULL;
}

/**
 * @brief Elm View List Class impl.
 *
 */
static void
_elm_view_list_add(Eo *obj, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());

   Elm_View_List_Private *self = (Elm_View_List_Private *)class_data;
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   EINA_SAFETY_ON_NULL_RETURN(list);
   Evas_Object *parent = va_arg(*list, Evas_Object *);
   EINA_SAFETY_ON_NULL_RETURN(parent);

   self->model = va_arg(*list, Eo *);
   EINA_SAFETY_ON_NULL_RETURN(self->model);

   self->list = elm_genlist_add(parent);

   self->itc = elm_genlist_item_class_new();
   self->itc->item_style = "default";
   self->itc->func.text_get = _item_text_get;
   self->itc->func.content_get = NULL;
   self->itc->func.state_get = NULL;
   self->itc->func.del = _item_del;
}

static void
_elm_view_list_destructor(Eo *obj, void *class_data, va_list *list EINA_UNUSED)
{
   Elm_View_List_Private *self = (Elm_View_List_Private *)class_data;
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(obj);
   eo_do_super(obj, MY_CLASS, eo_destructor());
}

static void
_elm_view_list_evas_object_get(Eo *obj, void *class_data, va_list *list)
{
   Elm_View_List_Private *self = class_data;
   EINA_SAFETY_ON_NULL_RETURN(self);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   EINA_SAFETY_ON_NULL_RETURN(list);
   Evas_Object **e = va_arg(*list, Evas_Object **);
   *e = self->list;
}

static void
_view_list_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_list_add),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_list_destructor),
      EO_OP_FUNC(ELM_VIEW_LIST_ID(ELM_OBJ_VIEW_LIST_SUB_ID_EVAS_OBJECT_GET), _elm_view_list_evas_object_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}


static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_LIST_SUB_ID_EVAS_OBJECT_GET, "Return Evas object list"),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description view_list_class_descs = {
   EO_VERSION,
   MY_CLASS_NAME,
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_VIEW_LIST_BASE_ID, op_descs, ELM_OBJ_VIEW_LIST_SUB_ID_LAST),
   NULL,
   sizeof(Elm_View_List_Private),
   _view_list_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_view_list_class_get, &view_list_class_descs, EO_BASE_CLASS, NULL);
