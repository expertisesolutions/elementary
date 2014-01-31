
#ifndef ELM_VIEW_TREE_H
#define ELM_VIEW_TREE_H

/**
 * @def base class id
 */
extern EAPI Eo_Op ELM_VIEW_TREE_BASE_ID;

typedef enum
{
   ELM_VIEW_TREE_VIEWMODE_ALL,
   ELM_VIEW_TREE_VIEWMODE_ONLYPARENTS /**< groupnode or has_child */
} Elm_View_Tree_Mode;

/**
 * @brief EO Class functions identifiers
 */
enum {
   EVT_SUB_ID_TREE_DESTRUCTOR,
   EVT_SUB_ID_TREE_GETCONTENT_SET,
   EVT_SUB_ID_TREE_MODE_SET,
   EVT_SUB_ID_TREE_ADD, 
   EVT_SUB_ID_TREE_EVAS_OBJECT_GET, 
   EVT_SUB_ID_TREE_LAST
};

typedef Evas_Object  *(*Elm_View_Tree_Content_Get_Cb)(Eo *model, Elm_Model_Tree_Path *path, Evas_Object *obj, const char *part);

/**
 * @def base class id touple
 */
#define ELM_VIEW_ID(sub_id) (ELM_VIEW_TREE_BASE_ID + sub_id)

/**
 * @brief EO Class functions declaration.
 */
#define elm_view_tree_add(parent, model) ELM_VIEW_ID(EVT_SUB_ID_TREE_ADD), EO_TYPECHECK(Evas_Object *, parent), EO_TYPECHECK(Eo *, model)
#define elm_view_tree_destructor() ELM_VIEW_ID(EVT_SUB_ID_TREE_DESTRUCTOR)
#define elm_view_tree_mode_set(mode) ELM_VIEW_ID(EVT_SUB_ID_TREE_MODE_SET), EO_TYPECHECK(Elm_View_Tree_Mode, mode)
#define elm_view_tree_getcontent_set(get_content_cb) ELM_VIEW_ID(EVT_SUB_ID_TREE_GETCONTENT_SET), EO_TYPECHECK(Elm_View_Tree_Content_Get_Cb, get_content_cb)
#define elm_view_tree_evas_object_get(widget) ELM_VIEW_ID(EVT_SUB_ID_TREE_EVAS_OBJECT_GET), EO_TYPECHECK(Evas_Object **, widget)

/**
 * @brief EO Class instance object
 */
#define ELM_VIEW_TREE_CLASS elm_view_tree_class_get()
const Eo_Class *elm_view_tree_class_get(void);

#endif // ELM_VIEW_TREE_H

