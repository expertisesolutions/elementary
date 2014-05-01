#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Eo.h>
#include <Emodel.h>
#include <Elementary.h>

#include "elm_view_form.h"
#include "elm_view_list.h"

#include "elm_priv.h"
#include <assert.h>

typedef struct _Elm_View_Form_Private Elm_View_Form_Private;
typedef struct _Elm_View_Form_Item Elm_View_Form_Item;
typedef struct _Elm_View_Form_Properties Elm_View_Form_Properties;
 

struct _Elm_View_Form_Properties
{
   Eina_Value *v;
   const char *prop;
};

struct _Elm_View_Form_Widget
{
   const char *widget_name;
   Evas_Object *widget_obj;
};
typedef struct _Elm_View_Form_Widget Elm_View_Form_Widget;

struct _Elm_View_Form_Item
{
   char *propname;
   Eina_List *widget_list;
};

struct _Elm_View_Form_Private
{
   Eo *model_obj;
   Eina_Value *properties;
   const char *model_name;
   Eina_List *l;
   Elm_View_Form_Properties proplist[2];
};

enum {
   VIEW_FORM_PROP_LABEL,
   VIEW_FORM_PROP_CLOCK,
};

static Eina_Bool
_elm_view_widget_add(Elm_View_Form_Private *priv, 
                           const char *widget_name, char *propname, Evas_Object *widget_obj)
{
   Eina_List *l;

   /**
    * First call
    */
   if(!priv->l) 
     {
        Elm_View_Form_Item *item = calloc(1, sizeof(Elm_View_Form_Item));
        EINA_SAFETY_ON_NULL_RETURN_VAL(item, EINA_FALSE);

        Elm_View_Form_Widget *widget = calloc(1, sizeof(Elm_View_Form_Widget));
        EINA_SAFETY_ON_NULL_RETURN_VAL(widget, EINA_FALSE);

        widget->widget_name = widget_name;
        widget->widget_obj = widget_obj;

        item->propname = propname;
        item->widget_list = eina_list_append(item->widget_list, widget);
        priv->l = eina_list_append(priv->l, item);
     } 
   else
     {
        Elm_View_Form_Item *nItem;
        Elm_View_Form_Widget *nWidget;
        for(l = priv->l; l; l = eina_list_next(l))
          {
             nItem = (Elm_View_Form_Item *)l->data;

             /**
              * List is already created and property already exists so we append a new Evas_Object
              */
             if(!strncmp(nItem->propname, propname, strlen(nItem->propname)))
               {
                  nWidget = calloc(1, sizeof(Elm_View_Form_Widget));
                  EINA_SAFETY_ON_NULL_RETURN_VAL(nWidget, EINA_FALSE);

                  nWidget->widget_name = widget_name;
                  nWidget->widget_obj = widget_obj;
                  nItem->widget_list = eina_list_append(nItem->widget_list, nWidget);
               }

             /**
              * List is already created and property is new so we create a new tupple
              */
             else
               {
                  nItem = calloc(1, sizeof(Elm_View_Form_Item));
                  EINA_SAFETY_ON_NULL_RETURN_VAL(nItem, EINA_FALSE);

                  nWidget = calloc(1, sizeof(Elm_View_Form_Widget));
                  EINA_SAFETY_ON_NULL_RETURN_VAL(nWidget, EINA_FALSE);

                  nWidget->widget_name = widget_name;
                  nWidget->widget_obj = widget_obj;

                  nItem->propname = propname;
                  nItem->widget_list = eina_list_append(nItem->widget_list, nWidget);

                  l = eina_list_append(l, nItem);
                  l = eina_list_next(l);
               }
          }
     }
   return EINA_TRUE;
}

/**
 * @brief Elm View Form Class
 *
 */
EAPI Eo_Op ELM_VIEW_FORM_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_VIEW_FORM_CLASS
#define MY_CLASS_NAME "View Form"

/**
 * @brief window callbacks
 * 
 */
static Eina_Bool
_elm_view_form_property_change_cb(void *data, Eo *obj EINA_UNUSED, 
                                  const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)data;
   EINA_SAFETY_ON_NULL_RETURN_VAL(priv, EINA_FALSE);

#if 0 
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = data;

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   evt.value = _emodel_property_value_get(priv, evt.prop);
   eina_value_set(evt.value, priv->path);
   eio_file_direct_stat(priv->path, _eio_stat_done_cb, _eio_property_set_error_cb, priv);

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
#endif
   return EINA_TRUE;
}


/**
 * @brief constructor
 */
static void
_elm_view_form_constructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{

   Eina_Value *v;
   char *prop = NULL;

   eo_do_super(obj, MY_CLASS, eo_constructor());

   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;

   Eo *model_obj = va_arg(*list, Eo *);

   EINA_SAFETY_ON_NULL_RETURN(model_obj);

   priv->model_obj = model_obj;
   priv->model_name = eo_class_name_get(priv->model_obj);
}

//const Eo_Class_Description *desc = klass->desc;
/**
 * @brief destructor
 */
static void
_elm_view_form_destructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_destructor());
}


static void
_elm_view_form_property_pair_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   int propname_id = va_arg(*list, int);
   const char *propname = va_arg(*list, const char *);

   EINA_SAFETY_ON_NULL_RETURN(propname);



}

static void
_elm_view_form_widget_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   char *propname = va_arg(*list, const char *);
   Evas_Object *evas = va_arg(*list, Evas_Object *);
   const char *objname = eo_class_name_get(evas);

   EINA_SAFETY_ON_NULL_RETURN(propname);
   EINA_SAFETY_ON_NULL_RETURN(evas);

   EINA_SAFETY_ON_FALSE_RETURN(_elm_view_widget_add(priv, objname, propname, evas));

   fprintf(stdout, "Added widget '%s' for model '%s'\n", objname, priv->model_name);
}
      
static void
_elm_view_form_widget_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
#if 0
   Emodel_Property_EVT evt;
   Eina_Value *v;
   const char *name;
   Eina_List *l;
   Elm_View_Form_Item *item;

   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;

   evt.prop = va_arg(*list, const char *); //FIXME
   evt.value = va_arg(*list, Eina_Value *);

   for(l = priv->l; l; l = eina_list_next(l))
     {
        item = (Elm_View_Form_Item *)l->data;

        /**
         * Label widget
         */
        if(!strncmp(item->widget_name, "Elm_Label", strlen(evt.prop)))
          {
                  const char *src;
                  eina_value_get(evt.value, &src);
                  elm_object_text_set(item->evas_obj, src);
                  eo_do(priv->model_obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
          }

        /**
         * Clock widget
         */
        if(!strncmp(item->widget_name, "Elm_Clock", strlen(evt.prop)))
          {
             //TODO
          }
     }
   //XXX
#endif
}

static void
_view_form_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_form_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_form_destructor),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_PROPERTY_PAIR_ADD), _elm_view_form_property_pair_add),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD), _elm_view_form_widget_add),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET), _elm_view_form_widget_set),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_PROPERTY_PAIR_ADD, "Add property pair"),
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD, "Add new widget"),
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET, "Set widget data"),
   EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description view_form_class_descs = {
   EO_VERSION,
   MY_CLASS_NAME,
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_VIEW_FORM_BASE_ID, op_descs, ELM_OBJ_VIEW_FORM_SUB_ID_LAST),
   NULL,
   sizeof(Elm_View_Form_Private),
   _view_form_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_view_form_class_get, &view_form_class_descs, EO_BASE_CLASS, NULL);
