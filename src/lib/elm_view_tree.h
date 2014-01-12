
#ifndef ELM_VIEW_TREE_H
#define ELM_VIEW_TREE_H

typedef enum
{
   ELM_VIEW_TREE_VIEWMODE_ALL,
   ELM_VIEW_TREE_VIEWMODE_ONLYPARENTS //groupnode or has_child
} Elm_View_Tree_Mode;

typedef Evas_Object  *(*Elm_View_Tree_Content_Get_Cb)(Eo *model, Elm_Model_Tree_Path *path, Evas_Object *obj, const char *part);

/**
 * @def elm_view_tree_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
//EAPI void elm_view_tree_add(Evas_Object* parent, Eo* model);

/**
 * @def elm_view_tree_evas_object_get
 * @since 1.8
 *
 * @return evas_widget
 *
 */
//EAPI Evas_Object* elm_view_tree_evas_object_get();

/**
 * @def elm_view_tree_mode_set
 * @since 1.8
 *
 * @param mode
 *
 */
//EAPI void elm_view_tree_mode_set(Elm_View_Tree_Mode mode);

/**
 * @brief EO3 Class Declaration
 */
#define ELM_VIEW_TREE_CLASS elm_view_tree                     \
   , constructor(elm_view_tree_add, _elm_view_tree_add, Evas_Object*, Eo*)  \
   , destructor(_elm_view_tree_destructor) \
   , function(elm_view_tree_evas_object_get, _elm_view_tree_evas_object_get, Evas_Object*) \
   , function(elm_view_tree_getcontent_set, _elm_view_tree_getcontent_set, void, Elm_View_Tree_Content_Get_Cb) \
   , function(elm_view_tree_mode_set, _elm_view_tree_mode_set, void, Elm_View_Tree_Mode)

EO3_DECLARE_CLASS(ELM_VIEW_TREE_CLASS)

#endif // ELM_VIEW_TREE_H

