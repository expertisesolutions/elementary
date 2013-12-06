
#ifndef ELM_VIEW_TREE_H
#define ELM_VIEW_TREE_H

/**
 * @def elm_view_tree_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
EAPI void elm_view_tree_add(Evas_Object* parent, Eo* model);

/**
 * @brief EO3 Class Declaration
 */
#define ELM_VIEW_TREE elm_view_tree                     \
   , constructor(elm_view_tree_add, _elm_view_tree_add, Evas_Object*, Eo*)  \
   , destructor(_elm_view_tree_destructor)

EO3_DECLARE_CLASS(ELM_VIEW_TREE)


#endif // ELM_VIEW_TREE_H

