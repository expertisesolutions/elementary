#ifndef ELM_VIEW_FORM_H
#define ELM_VIEW_FORM_H

extern EAPI Eo_Op ELM_VIEW_FORM_BASE_ID;

#define ELM_OBJ_VIEW_FORM_CLASS elm_obj_view_form_class_get()
const Eo_Class *elm_obj_view_form_class_get(void) EINA_CONST;

enum {
   ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD,
   ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET,
   ELM_OBJ_VIEW_FORM_SUB_ID_LAST
};

#define ELM_VIEW_FORM_ID(sub_id) (ELM_VIEW_FORM_BASE_ID + sub_id)


/**
 * @def elm_view_form_constructor
 * @since 1.11
 *
 * @param model
 *
 */
#define elm_view_form_constructor(model) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(Eo *, model)

/**
 * @def elm_view_form_widget_add
 * @since 1.11
 *
 * @param propname
 * @param evas
 *
 */
#define elm_view_form_widget_add(propname, evas) ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_ADD), EO_TYPECHECK(const char *, propname), EO_TYPECHECK(Evas_Object *, evas)


/**
 * @def elm_view_form_widget_set
 * @since 1.11
 *
 * @param propname
 * @param value
 *
 */
#define elm_view_form_widget_set(propname, value) ELM_VIEW_FORM_ID(ELM_OBJ_VIEW_FORM_SUB_ID_WIDGET_SET), EO_TYPECHECK(const char *, propname), EO_TYPECHECK(Eina_Value *, value)

#endif
