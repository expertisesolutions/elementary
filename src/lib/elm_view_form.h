#ifndef ELM_VIEW_FORM_H
#define ELM_VIEW_FORM_H

#define ELM_OBJ_VIEW_FORM_CLASS elm_obj_view_form_class_get()

const Eo_Class *elm_obj_view_form_class_get(void) EINA_CONST;

enum {
   ELM_OBJ_VIEW_FORM_SUB_ID_LAST
};

#define ELM_VIEW_FORM_ID(sub_id) (ELM_VIEW_FORM_BASE_ID + sub_id)
/**
 * @def elm_view_form_setup
 * @since 1.10
 *
 * @param name
 * @param title
 *
 */
#define elm_view_form_setup(name, title) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, name), EO_TYPECHECK(const char *, title)

#endif
