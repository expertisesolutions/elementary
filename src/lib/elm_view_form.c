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

#define MY_CLASS ELM_VIEW_FORM_CLASS
#define MY_CLASS_NAME "View_Form"

typedef struct _Elm_View_Form_Data Elm_View_Form_Data;
typedef struct _Elm_View_Form_Widget Elm_View_Form_Widget;

/**
 * @brief Local-use callbacks
 */
typedef void (*Elm_View_Form_Event_Cb)(Elm_View_Form_Widget *, Elm_View_Form_Data *, Evas_Object *);
typedef void (*Elm_View_Form_Widget_Object_Set_Cb)(Eo *, Evas_Object *, Eina_Value *, const char *);

struct _Elm_View_Form_Widget
{
   const char *widget_propname;
   Eina_Value *widget_value;
   Evas_Object *widget_obj;
   Elm_View_Form_Event_Cb widget_obj_value_update_cb;
   Elm_View_Form_Widget_Object_Set_Cb widget_obj_set_cb;
};

struct _Elm_View_Form_Data
{
   Eo *model_obj;
   Eina_Value *properties;
   Eina_List *l;
};

static Eina_Bool
_emodel_property_change_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Elm_View_Form_Data *priv = data;
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
_elm_evas_object_entry_value_update(Elm_View_Form_Widget *widget, Elm_View_Form_Data *_pd EINA_UNUSED, Evas_Object *obj)
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
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)data;
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
_elm_view_widget_add(Elm_View_Form_Data *_pd, const char *propname, Evas_Object *widget_obj)
{
   int action = EVAS_CALLBACK_LAST;
   Elm_View_Form_Data *priv = _pd;
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
   priv->l = eina_list_append(_pd->l, w);
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
_elm_view_form_constructor(Eo *obj, Elm_View_Form_Data *_pd, Eo *model)
{
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)_pd;
   eo_do_super(obj, MY_CLASS, eo_constructor());
   priv->model_obj = model;
   if (priv->model_obj != NULL)
     {
        eo_ref(priv->model_obj);
        eo_do(priv->model_obj, eo_event_callback_add(EMODEL_EVENT_PROPERTY_CHANGE, _emodel_property_change_cb, priv));
     }
}

static void
_elm_view_form_eo_base_constructor(Eo *obj, Elm_View_Form_Data *_pd EINA_UNUSED)
{
   eo_error_set(obj);
   fprintf(stderr, "only custom constructor can be used with '%s' class", MY_CLASS_NAME);
}


/**
 * @brief destructor
 */
static void
_elm_view_form_eo_base_destructor(Eo *obj, Elm_View_Form_Data *_pd)
{
   void *data = NULL;
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)_pd;

   EINA_LIST_FREE(priv->l, data)
        free(data);
   eina_list_free(priv->l);
   eo_do_super(obj, MY_CLASS, eo_destructor());
}


static void
_elm_view_form_model_set(Eo *obj EINA_UNUSED, Elm_View_Form_Data *_pd, Eo *model)
{
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)_pd;
   if (priv->model_obj != NULL)
     {
        eo_do(priv->model_obj, eo_event_callback_del(EMODEL_EVENT_PROPERTY_CHANGE, _emodel_property_change_cb, priv));
        eo_unref(priv->model_obj);
     }

   priv->model_obj = model;

   if (priv->model_obj != NULL)
     {
        eo_ref(priv->model_obj);
        eo_do(priv->model_obj, eo_event_callback_add(EMODEL_EVENT_PROPERTY_CHANGE, _emodel_property_change_cb, priv));
     }
}

static void
_elm_view_form_widget_add(Eo *obj EINA_UNUSED, Elm_View_Form_Data *_pd, const char *propname, Evas_Object *evas)
{
   Eina_Bool status;
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)_pd;

   EINA_SAFETY_ON_NULL_RETURN(evas);
   EINA_SAFETY_ON_NULL_RETURN(propname);

   status = _elm_view_widget_add(priv, propname, evas);
   EINA_SAFETY_ON_FALSE_RETURN(status);
}

static void
_elm_view_form_widget_set(Eo *obj EINA_UNUSED, Elm_View_Form_Data *_pd, const char *propname, Eina_Value *value)
{
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)_pd;
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
//
static void
_elm_view_form_widget_get(Eo *obj EINA_UNUSED, Elm_View_Form_Data *_pd, const char *propname)
{
   Emodel_Property_EVT evt;
   Elm_View_Form_Data *priv = (Elm_View_Form_Data *)_pd;
   EINA_SAFETY_ON_NULL_RETURN(propname);

   for(Eina_List *l = priv->l; l; l = eina_list_next(l))
     {
        Elm_View_Form_Widget *w = eina_list_data_get(l);
        if(!strncmp(propname, w->widget_propname, strlen(w->widget_propname)))
          {
             ////FIXME
             evt.prop = propname;
             evt.value = w->widget_value;
             eo_do(priv->model_obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
          }
     }
}

#endif
#include "elm_view_form.eo.c"
