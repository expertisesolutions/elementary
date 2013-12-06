
#ifndef ELM_VIEW_GRID_H
#define ELM_VIEW_GRID_H

/**
 * @brief EO3 Class Declaration
 */
#define ELM_VIEW_GRID elm_view_grid                     \
   , constructor(elm_view_grid_add, Evas_Object*, Eo*)  \
   , destructor(_destructor)

EO3_DECLARE_CLASS(ELM_VIEW_GRID)


#endif // ELM_VIEW_GRID_H

