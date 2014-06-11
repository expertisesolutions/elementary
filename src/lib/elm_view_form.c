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

/**
 * @brief Elm View Form Class
 */
EAPI Eo_Op ELM_VIEW_FORM_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_VIEW_FORM_CLASS
#define MY_CLASS_NAME "View_Form"

typedef struct _Elm_View_Form_Private Elm_View_Form_Private;
typedef struct _Elm_View_Form_Widget Elm_View_Form_Widget;

/**
 * @brief Local-use callbacks
 */
typedef void (*Elm_View_Form_Event_Cb)(Elm_View_Form_Widget *, Elm_View_Form_Private *, Evas_Object *);
typedef void (*Elm_View_Form_Widget_Object_Set_Cb)(Eo *, Evas_Object *, Eina_Value *, const char *);

struct _Elm_View_Form_Widget
{
   const char *widget_propname;
   Eina_Value *widget_value;
   Evas_Object *widget_obj;
   Elm_View_Form_Event_Cb widget_obj_value_update_cb;
   Elm_View_Form_Widget_Object_Set_Cb widget_obj_set_cb;
};

struct _Elm_View_Form_Private
{
   Eo *model_obj;
   Eina_Value *properties;
   Eina_List *l;
};

static Eina_Bool
_emodel_property_change_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Elm_View_Form_Private *priv = data;
   Emodel_Property_EVT *evt = event_info;

   EINA_SAFETY_ON_NULL_RETURN_VAL(priv, EINA_TRUE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(evt, EINA_TRUE);

   //update all widgets with this property
   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        Elm_View_Form_Widget *w = eina_list_data_get(l);
        if (strcmp(w->widget_propname, evt->prop) == 0)
          {
              w->widget_value = evt->value;
              w->widget_obj_set_cb(priv->model_obj, w->widget_obj, evt->value, evt->prop);
          }
     }

   return EINA_TRUE;
}


/**
 * @brief Get Evas_Object property name.
 */
/*static const char *
_elm_form_view_property_from_object_get(Elm_View_Form_Private *priv, Evas_Object *obj)
{
   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        Elm_View_Form_Widget *w = eina_list_data_get(l);
        if(w->widget_obj == obj) return w->widget_propname;
     }
   return NULL;
}
*/
/**
 * @brief Set widget.
 * Works, so far, for widget(s): Entry, Label
 */
static void
_elm_evas_object_entry(Eo *obj, Evas_Object *entry, Eina_Value *value, const char *propname)
{
   const char *text = eina_value_to_string(value);
   EINA_SAFETY_ON_NULL_RETURN(text);

   elm_object_text_set(entry, text);
   eo_do(obj, emodel_property_set(propname, value));

   fprintf(stdout, "%s:%d: new text: %s:%p\n", __FUNCTION__, __LINE__, text, entry);
}

/**
 * @brief Update widget's Eina_Value.
 *    Works, so far, for widget(s): Entry, Label
 */
static void
_elm_evas_object_entry_value_update(Elm_View_Form_Widget *widget, Elm_View_Form_Private *priv EINA_UNUSED, Evas_Object *obj)
{
   const char *text = elm_object_text_get(obj);
   EINA_SAFETY_ON_NULL_RETURN(text);

   if (widget->widget_value == NULL)
     widget->widget_value = eina_value_new(EINA_VALUE_TYPE_STRING);

   eina_value_set(widget->widget_value, text);
   fprintf(stdout, "%s:%d: new text: %s:%p\n", __FUNCTION__, __LINE__, text, widget->widget_obj);
}


static void
_elm_evas_object_thumb(Eo *obj, Evas_Object *thumb, Eina_Value *value, const char *propname)
{
   const char *filename = eina_value_to_string(value);
   EINA_SAFETY_ON_NULL_RETURN(filename);
   EINA_SAFETY_ON_TRUE_RETURN(strlen(filename) >= PATH_MAX);

   elm_thumb_file_set(thumb, filename, NULL);
   elm_thumb_reload(thumb);

   eo_do(obj, emodel_property_set(propname, value));
   fprintf(stdout, "%s:%d: new text: %s:%p\n", __FUNCTION__, __LINE__, filename, thumb);
}

/**
 * @brief Evas object callback implementation.
 *    Updates Widget's value if not the same object
 *    and the widget itself.
 */
static void
_elm_evas_object_changed_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)data;
   Elm_View_Form_Widget *w = NULL;

   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        w = eina_list_data_get(l);
        if (w->widget_obj == obj)
           break;
     }

   EINA_SAFETY_ON_NULL_RETURN(w);

   w->widget_obj_value_update_cb ?
         w->widget_obj_value_update_cb(w, priv, obj) : 0;

   eo_do(obj, emodel_property_set(w->widget_propname, w->widget_value));
}

/**
 * @brief Add new widget object.
 *    Adds new widget object on the list
 *    and perform initial setup.
 */
static Eina_Bool
_elm_view_widget_add(Elm_View_Form_Private *priv, char *propname, Evas_Object *widget_obj)
{
   int action = EVAS_CALLBACK_LAST;
   Elm_View_Form_Widget *w = calloc(1, sizeof(Elm_View_Form_Widget));
   EINA_SAFETY_ON_NULL_RETURN_VAL(w, EINA_FALSE);
   const char *objname = elm_object_widget_type_get(widget_obj);
   EINA_SAFETY_ON_NULL_RETURN_VAL(objname, EINA_FALSE);

   if(!strncmp(objname, "Elm_Entry", 9))
     {
        action = EVAS_CALLBACK_KEY_DOWN;
        w->widget_obj_set_cb = _elm_evas_object_entry;
        w->widget_obj_value_update_cb = _elm_evas_object_entry_value_update; /**< simple widget text set */
     }
   else if(!strncmp(objname, "Elm_Label", 9))
     {
        w->widget_obj_set_cb = _elm_evas_object_entry;
        w->widget_obj_value_update_cb = _elm_evas_object_entry_value_update; /**< simple widget text set (fits Label too!) */
     }
   else if(!strncmp(objname, "Elm_Thumb", 9))
     {
        w->widget_obj_set_cb = _elm_evas_object_thumb;
        w->widget_obj_value_update_cb = NULL; /**< no update */
     }
   else
     {
        // Widget yet not supported
        EINA_SAFETY_ON_NULL_RETURN_VAL(NULL, EINA_FALSE);
     }

   w->widget_value = NULL;
   w->widget_propname = propname;
   w->widget_obj = widget_obj;
   priv->l = eina_list_append(priv->l, w);
   eo_do(priv->model_obj, emodel_property_get(propname));

   if(action != EVAS_CALLBACK_LAST)
     {
        evas_object_event_callback_add(w->widget_obj, action, _elm_evas_object_changed_cb, priv);
     }

   return EINA_TRUE;
}
/**
 * Helper functions - End
 */


/**
 * @brief constructor
 */
static void
_elm_view_form_constructor(Eo *obj, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   Eo *model_obj = va_arg(*list, Eo *);
   eo_do_super(obj, MY_CLASS, eo_constructor());
   priv->model_obj = model_obj;
   if (priv->model_obj != NULL)
     {
        eo_ref(priv->model_obj);
        eo_do(priv->model_obj, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _emodel_property_change_cb, priv));
     }
}

/**
 * @brief destructor
 */
static void
_elm_view_form_destructor(Eo *obj, void *class_data, va_list *list EINA_UNUSED)
{
   void *data = NULL;
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;

   EINA_LIST_FREE(priv->l, data)
        free(data);
   eina_list_free(priv->l);
   eo_do_super(obj, MY_CLASS, eo_destructor());
}


static void
_elm_view_form_model_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   Eo *model_obj = va_arg(*list, Eo *);
   if (priv->model_obj != NULL)
     {
        eo_do(priv->model_obj, eo_event_callback_del(EMODEL_PROPERTY_CHANGE_EVT, _emodel_property_change_cb, priv));
        eo_unref(priv->model_obj);
     }

   priv->model_obj = model_obj;

   if (priv->model_obj != NULL)
     {
        eo_ref(priv->model_obj);
        eo_do(priv->model_obj, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _emodel_property_change_cb, priv));
     }
}

static void
_elm_view_form_widget_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Eina_Bool status;
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   char *propname = va_arg(*list, const char *);
   Evas_Object *evas = va_arg(*list, Evas_Object *);

   EINA_SAFETY_ON_NULL_RETURN(evas);
   EINA_SAFETY_ON_NULL_RETURN(propname);

   status = _elm_view_widget_add(priv, propname, evas);
   EINA_SAFETY_ON_FALSE_RETURN(status);
}

static void
_elm_view_form_widget_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   char *propname = va_arg(*list, const char *);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   EINA_SAFETY_ON_NULL_RETURN(propname);
   EINA_SAFETY_ON_NULL_RETURN(value);

   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        Elm_View_Form_Widget *w = eina_list_data_get(l);
        if(strncmp(w->widget_propname, propname, strlen(w->widget_propname))) continue;

        w->widget_value = value;
        w->widget_obj_set_cb(priv->model_obj, w->widget_obj, value, propname);
     }
}

static void
_elm_view_form_widget_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Emodel_Property_EVT evt;
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   const char *propname = va_arg(*list, const char *);
   EINA_SAFETY_ON_NULL_RETURN(propname);

   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        Elm_View_Form_Widget *w = eina_list_data_get(l);
        if(!strncmp(propname, w->widget_propname, strlen(w->widget_propname)))
          {
             ////FIXME
             evt.prop = propname;
             evt.value = w->widget_value;
             eo_do(priv->model_obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
          }
     }
}

static void
_view_form_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_form_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_form_destructor),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_MODEL_SET), _elm_view_form_model_set),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD), _elm_view_form_widget_add),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET), _elm_view_form_widget_set),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_GET), _elm_view_form_widget_get),
      EO_OP_FUNC_SENTINEL
   };
   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_MODEL_SET, "Set model"),
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD, "Add new widget"),
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET, "Set property's value."),
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_GET, "Get property's value"),
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

