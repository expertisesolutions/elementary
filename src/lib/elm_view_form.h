#ifndef ELM_VIEW_FORM_H
#define ELM_VIEW_FORM_H

#define ELM_OBJ_VIEW_FORM_CLASS elm_obj_view_form_class_get()

const Eo_Class *elm_obj_view_form_class_get(void) EINA_CONST;

enum {
   ELM_OBJ_VIEW_FORM_SUB_ID_LAST
};

#define ELM_VIEW_FORM_ID(sub_id) (ELM_VIEW_FORM_BASE_ID + sub_id)
/**
 * @def elm_view_list_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
#define elm_view_form_add(name, title) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, name), EO_TYPECHECK(const char *, title)

#endif
