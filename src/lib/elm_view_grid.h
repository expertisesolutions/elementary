
#ifndef ELM_VIEW_GRID_H
#define ELM_VIEW_GRID_H

/**
 * @def elm_view_grid_add
 * @since 1.8
 *
 * @param parent
 * @param model
 *
 */
EAPI void elm_view_grid_add(Evas_Object* parent, Eo* model);

/**
 * @def elm_view_grid_evas_object_get
 * @since 1.8
 *
 * @return evas_widget
 *
 */
EAPI Evas_Object* elm_view_grid_evas_object_get();


/**
 * @brief EO3 Class Declaration
 */
#define ELM_VIEW_GRID_CLASS elm_view_grid                     \
   , constructor(elm_view_grid_add, _elm_view_grid_add, Evas_Object*, Eo*) \
   , destructor(_elm_view_grid_destructor) \
   , function(elm_view_grid_evas_object_get, _elm_view_grid_evas_object_get, Evas_Object*)

EO3_DECLARE_CLASS(ELM_VIEW_GRID_CLASS)

#endif // ELM_VIEW_GRID_H

