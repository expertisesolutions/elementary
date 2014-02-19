
#ifndef ELM_VIEW_LIST_H
#define ELM_VIEW_LIST_H


extern EAPI Eo_Op ELM_OBJ_VIEW_LIST_BASE_ID;

enum {
     ELM_OBJ_VIEW_LIST_SUB_ID_LIST_ADD,
     ELM_OBJ_VIEW_LIST_SUB_ID_LIST_EVAS_OBJECT_GET,
     ELM_OBJ_VIEW_LIST_SUB_ID_LIST_LAST
};

#define ELM_OBJ_VIEW_LIST_ID(sub_id) (ELM_OBJ_VIEW_LIST_BASE_ID + sub_id) 
/**
 * @def elm_view_list_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
//EAPI void elm_view_list_add(Evas_Object* parent, Eo* model);
//_model_tree_child_prepend(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
//_elm_view_list_add(Eo *obj, Elm_View_List_Private *self, Evas_Object* parent, Eo* model)
#define elm_view_list_add(parent, model) ELM_OBJ_VIEW_LIST_ID(ELM_OBJ_VIEW_LIST_SUB_ID_LIST_ADD), EO_TYPECHECK(Evas_Object *, parent), EO_TYPECHECK(Eo *, model)

/**
 * @def elm_view_list_evas_object_get
 * @since 1.8
 *
 * @return evas_widget
 *
 */
//EAPI Evas_Object* elm_view_list_evas_object_get();

/**
 * @brief EO3 Class Declaration
 */
/* #define ELM_VIEW_LIST_CLASS elm_view_list                     \ */
/*    , constructor(elm_view_list_add, _elm_view_list_add, Evas_Object*, Eo*)  \ */
/*    , destructor(_elm_view_list_destructor) \ */
/*    , function(elm_view_list_evas_object_get, _elm_view_list_evas_object_get, Evas_Object*) */

/* EO3_DECLARE_CLASS(ELM_VIEW_LIST_CLASS) */

#define ELM_OBJ_VIEW_LIST_CLASS elm_obj_view_list_class_get()
const Eo_Class *elm_obj_view_list_class_get(void);

#endif // ELM_VIEW_LIST_H
