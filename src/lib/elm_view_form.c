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
typedef struct _Elm_View_Form_Widget Elm_View_Form_Widget;
typedef Eina_List Eina_List_Properties;

/**
 * @brief Elm View Form Class
 */
EAPI Eo_Op ELM_VIEW_FORM_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_VIEW_FORM_CLASS
#define MY_CLASS_NAME "View Form"
 
struct _Elm_View_Form_Widget
{
   const char *widget_propname;
   const char *widget_name;
   Evas_Object *widget_obj;
   size_t widget_len;
};

struct _Elm_View_Form_Private
{
   Eo *model_obj;
   Eina_Value *properties;
   const char *model_name;
   Eina_List *l;
   Eina_List_Properties *lprop;
};

/**
 * @brief Helper functions
 */
static Eina_Bool
_elm_view_widget_add(Elm_View_Form_Private *priv, const char *widget_name, char *propname, Evas_Object *widget_obj)
{
   /**
    * First call
    */
   Elm_View_Form_Widget *w = calloc(1, sizeof(Elm_View_Form_Widget));
   EINA_SAFETY_ON_NULL_RETURN_VAL(w, EINA_FALSE);

   w->widget_len = strlen(widget_name);
   w->widget_name = widget_name;
   w->widget_propname = propname;
   w->widget_obj = widget_obj;

   priv->l = eina_list_append(priv->l, w);

   return EINA_TRUE;
}

/**
 * @brief constructor
 */
static void
_elm_view_form_constructor(Eo *obj, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   Eo *model_obj = va_arg(*list, Eo *);
   
   eo_do_super(obj, MY_CLASS, eo_constructor());

   EINA_SAFETY_ON_NULL_RETURN(model_obj);

   priv->model_obj = model_obj;
   priv->model_name = eo_class_name_get(priv->model_obj);
}

/**
 * @brief destructor
 */
static void
_elm_view_form_destructor(Eo *obj, void *class_data, va_list *list EINA_UNUSED)
{
   void *data = NULL;
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   /**
    * Free item data
    */
   EINA_LIST_FREE(priv->l, data)
        free(data);
   eina_list_free(priv->l);

   eo_do_super(obj, MY_CLASS, eo_destructor());
}

static void
_elm_view_form_widget_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Eina_Bool status;
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   char *propname = va_arg(*list, const char *);
   Evas_Object *evas = va_arg(*list, Evas_Object *);
   const char *objname = eo_class_name_get(evas);

   EINA_SAFETY_ON_NULL_RETURN(evas);
   EINA_SAFETY_ON_NULL_RETURN(propname);

   status = _elm_view_widget_add(priv, objname, propname, evas);
   EINA_SAFETY_ON_FALSE_RETURN(status);
}

static void
_elm_view_form_widget_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Emodel_Property_EVT evt;
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   char *propname = va_arg(*list, const char *);
   Eina_Value *value = va_arg(*list, Eina_Value *);

   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        Elm_View_Form_Widget *w = eina_list_data_get(l);

        if(!w || !w->widget_name || !w->widget_obj) continue;
        if(strncmp(w->widget_propname, propname, strlen(w->widget_propname))) continue;

        if(!strncmp(w->widget_name, "Elm_Label", w->widget_len))
          {
             evt.prop = propname;
             evt.value = value;
             elm_object_text_set(w->widget_obj, eina_value_to_string(value));
             eo_do(priv->model_obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
          }
        else if(!strncmp(w->widget_name, "Elm_Clock", w->widget_len))
          {
             //TODO: implement
             fprintf(stdout, "Warning: property '%s' for Evas object '%p' not handled yet.\n", propname, w->widget_obj);
          }
        else if(!strncmp(w->widget_name, "Elm_Button", w->widget_len))
          {
             //TODO: implement
             fprintf(stdout, "Warning: property '%s' for Evas object '%p' not handled yet.\n", propname, w->widget_obj);
          }
        else if(!strncmp(w->widget_name, "Elm_Bubble", w->widget_len))
          {
             //TODO: implement
             fprintf(stdout, "Warning: property '%s' for Evas object '%p' not handled yet.\n", propname, w->widget_obj);
          }
        else if(!strncmp(w->widget_name, "Elm_Box", w->widget_len))
          {
             //TODO: implement
             fprintf(stdout, "Warning: property '%s' for Evas object '%p' not handled yet.\n", propname, w->widget_obj);
          }
        else
          {
             fprintf(stderr, "Error: no Evas object found for property '%s'\n", propname);
          }
     }
}
      
static void
_view_form_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_form_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_form_destructor),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD), _elm_view_form_widget_add),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET), _elm_view_form_widget_set),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD, "Add new widget"),
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET, "Set property value."),
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
