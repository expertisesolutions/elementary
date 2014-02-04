
#ifndef ELM_VIEW_GRID_H
#define ELM_VIEW_GRID_H

#define ELM_OBJ_VIEW_GRID_CLASS elm_obj_view_grid_class_get()
const Eo_Class *elm_obj_view_grid_class_get(void);

extern EAPI Eo_Op ELM_OBJ_VIEW_GRID_BASE_ID;

enum {
     ELM_OBJ_VIEW_GRID_SUB_ID_ADD,
     ELM_OBJ_VIEW_GRID_SUB_ID_DESTRUCTOR,
     ELM_OBJ_VIEW_GRID_SUB_ID_EVAS_OBJECT_GET,
     ELM_OBJ_VIEW_GRID_SUB_ID_LAST
};

#define ELM_OBJ_VIEW_GRID_CLASS_ID(sub_id) (ELM_OBJ_VIEW_GRID_BASE_ID + sub_id)

/**
 * @def elm_view_grid_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
#define elm_view_grid_add(parent, model) ELM_OBJ_VIEW_GRID_CLASS_ID(ELM_OBJ_VIEW_GRID_SUB_ID_ADD), EO_TYPECHECK(Evas_Object *, parent), EO_TYPECHECK(Eo *, model)

/**
 * @def elm_view_grid_evas_object_get
 * @since 1.8
 *
 * @return evas_widget
 *
 */
#define elm_view_grid_evas_object_get(evas) ELM_OBJ_VIEW_GRID_CLASS_ID(ELM_OBJ_VIEW_GRID_SUB_ID_EVAS_OBJECT_GET), EO_TYPECHECK(Evas_Object **, evas)

#endif // ELM_VIEW_GRID_H

