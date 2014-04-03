
#ifdef EFL_EO_API_SUPPORT
#ifndef ELM_VIEW_LIST_H
#define ELM_VIEW_LIST_H

#define ELM_OBJ_VIEW_LIST_CLASS elm_obj_view_list_class_get()

const Eo_Class *elm_obj_view_list_class_get(void) EINA_CONST;

extern EAPI Eo_Op ELM_VIEW_LIST_BASE_ID;

enum {
     ELM_OBJ_VIEW_LIST_SUB_ID_EVAS_OBJECT_GET,
     ELM_OBJ_VIEW_LIST_SUB_ID_LAST
};

#define ELM_VIEW_LIST_ID(sub_id) (ELM_VIEW_LIST_BASE_ID + sub_id)
/**
 * @def elm_view_list_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
#define elm_view_list_add(parent, model) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(Evas_Object *, parent), EO_TYPECHECK(Eo *, model)

/**
 * @def elm_view_list_evas_object_get
 * @since 1.8
 *
 * @return evas_widget
 *
 */
#define elm_view_list_evas_object_get(widget) ELM_VIEW_LIST_ID(ELM_OBJ_VIEW_LIST_SUB_ID_EVAS_OBJECT_GET), EO_TYPECHECK(Evas_Object **, widget)

#endif
#endif // ELM_VIEW_LIST_H
