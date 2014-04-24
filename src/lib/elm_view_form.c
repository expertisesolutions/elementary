#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Eo.h>
#include <Emodel.h>
#include <Elementary.h>

#include "elm_view_form.h"
#include "elm_priv.h"
#include <assert.h>

struct _Elm_View_Form_Private
{
};
typedef struct _Elm_View_Form_Private Elm_View_Form_Private;

/**
 * @brief Elm View Form Class
 *
 */
EAPI Eo_Op ELM_VIEW_FORM_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_VIEW_FORM_CLASS
#define MY_CLASS_NAME "View Form"

/**
 * @brief constructor
 */
static void
_elm_view_form_add(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
}

/**
 * @brief destructor
 */
static void
_elm_view_form_destructor(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_destructor());
}

static void
_view_form_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _elm_view_form_add),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _elm_view_form_destructor),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_descs[] = {
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
