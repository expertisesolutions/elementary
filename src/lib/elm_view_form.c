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

struct _Elm_View_Form_Item
{
   const char *propname;
   const char *widget_name;
   Evas_Object *evas_obj;
};

struct _Elm_View_Form_Properties
{
   Eina_Value *v;
   const char *prop;
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
   Eo *model = va_arg(*list, Eo *);

   EINA_SAFETY_ON_NULL_RETURN(model);

   priv->properties = eina_value_array_new(EINA_VALUE_TYPE_STRING, 0);
   eina_value_array_insert(priv->properties, VIEW_FORM_PROP_LABEL, "label_set");
   eina_value_array_insert(priv->properties, VIEW_FORM_PROP_CLOCK, "clock_set");

   /**
    * setup Label widget property
    */
   v = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_array_get(priv->properties, 0, &prop);
   priv->proplist[0].v = v; 
   priv->proplist[0].prop = prop; 


   priv->model_obj = model;
   priv->model_name = eo_class_name_get(priv->model_obj);

   //eo_do(priv->model_obj, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _elm_view_form_property_change_cb, priv));
   //DISPATCH EMODEL EVENT

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
_elm_view_form_widget_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   const char *propname = va_arg(*list, const char *);
   Evas_Object *evas = va_arg(*list, Evas_Object *);

   EINA_SAFETY_ON_NULL_RETURN(propname);
   EINA_SAFETY_ON_NULL_RETURN(evas);
   
   Elm_View_Form_Item *item = calloc(1, sizeof(Elm_View_Form_Item));
   EINA_SAFETY_ON_NULL_RETURN(item);

   item->propname = propname;
   item->evas_obj = evas;
   item->widget_name = eo_class_name_get(item->evas_obj);

   priv->l = eina_list_append(priv->l, item);

   fprintf(stdout, "Added widget '%s' for model '%s'\n", item->widget_name, priv->model_name);
}
      
static void
_emodel_override_property_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Emodel_Property_EVT evt;
   Eina_List *l;
   Elm_View_Form_Item *item;

   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;

   evt.prop = va_arg(*list, const char *);
   evt.value = va_arg(*list, Eina_Value *);

   for(l = priv->l; l; l = eina_list_next(l))
     {
        item = (Elm_View_Form_Item *)l->data;

        /**
         * Label widget
         */
        if(!strncmp(item->widget_name, "Elm_Label", strlen(evt.prop)))
          {
             if(!strncmp(item->propname, "label_set", strlen("label_set")))
               {
                  const char *src;
                  eina_value_get(evt.value, &src);
                  elm_object_text_set(item->evas_obj, src);
                  eo_do(priv->model_obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
               }
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
}

static void
_view_form_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_form_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_form_destructor),
      EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_PROPERTY_SET), _emodel_override_property_set),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD), _elm_view_form_widget_add),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD, "Add new widget"),
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

EO_DEFINE_CLASS(elm_obj_view_form_class_get, &view_form_class_descs, EO_BASE_CLASS, EMODEL_CLASS, NULL);
