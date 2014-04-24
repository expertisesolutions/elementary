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
typedef struct _Elm_View_Form_List_Data Elm_View_Form_List_Data;

struct _Elm_View_Form_List_Data
{
   Eo *listmodel;
   Eo *listview;
   Evas_Object *listwin;
};

struct _Elm_View_Form_Private
{
   Evas_Object *win;
   Evas_Object *bigbox;
   Elm_View_Form_List_Data list_data;
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
static void
_win_focused_cb(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   const char *name = (const char *)data;
   fprintf(stdout, "%s:%d Window focused: %p:%s\n", __FUNCTION__, __LINE__, obj, name);
}

static void
_main_win_del_cb(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   const char *name = (const char *)data;
   fprintf(stdout, "%s:%d Window delete request: %p:%s\n", __FUNCTION__, __LINE__, obj, name);
}


/**
 * @brief constructor
 *
 * Setup main window and place a bigbox inside 
 */
static void
_elm_view_form_setup(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());

   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;

   const char *name = va_arg(*list, const char *); /**< window name */
   const char *title = va_arg(*list, const char *); /**< window title */

   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(name);
   EINA_SAFETY_ON_NULL_RETURN(title);

   /* create main window */
   priv->win = elm_win_util_standard_add(name, title);
   EINA_SAFETY_ON_NULL_RETURN(priv->win);

   /* enable focus */
   elm_win_focus_highlight_enabled_set(priv->win, EINA_TRUE);
   evas_object_smart_callback_add(priv->win, "focus,in", _win_focused_cb, "mainwindow");

   /* define window delete callback */
   evas_object_smart_callback_add(priv->win, "delete,request", _main_win_del_cb, NULL);

   /* create central box which will be used later do place widgets, trees, etc.. */
   priv->bigbox = elm_box_add(priv->win);
   EINA_SAFETY_ON_NULL_RETURN(priv->bigbox);
   
   /* set some defult box properties but do not display it yet */
   elm_box_horizontal_set(priv->bigbox, EINA_TRUE);
   evas_object_size_hint_weight_set(priv->bigbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(priv->win, priv->bigbox);
   //evas_object_show(priv->bigbox);

}

/**
 * @brief destructor
 */
static void
_elm_view_form_destructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_destructor());
}

/**
 * @brief create list object and place it inside of bigbox
 * 
 */
static void
_elm_view_form_list_create(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{
   Elm_View_Form_Private *priv = (Elm_View_Form_Private *)class_data;
   
   priv->list_data.listwin = va_arg(*list, Evas_Object *);
   priv->list_data.listmodel = va_arg(*list, Eo *);
   
   EINA_SAFETY_ON_NULL_RETURN(priv->list_data.listwin);
   EINA_SAFETY_ON_NULL_RETURN(priv->list_data.listmodel);

   /* create new list object */
   priv->list_data.listview = eo_add_custom(ELM_OBJ_VIEW_LIST_CLASS, 
                                            NULL, elm_view_list_add(priv->list_data.listwin, priv->list_data.listmodel));

   //TODO: adjust listwin window size relative to bigbox. Use evas_object_resize()

   /* place the list window inside of our bigbox */
   elm_box_pack_end(priv->bigbox, priv->list_data.listwin);
}

static void
_view_form_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_form_setup),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_form_destructor),
      EO_OP_FUNC(ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_LIST), _elm_view_form_list_create),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_VIEW_FORM_SUB_ID_LIST, "Create object list"),
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
