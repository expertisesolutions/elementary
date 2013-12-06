
#ifndef ELM_VIEW_LIST_H
#define ELM_VIEW_LIST_H

/**
 * @brief EO3 Class Declaration
 */
#define ELM_VIEW_LIST elm_view_list                     \
   , constructor(elm_view_list_add, Evas_Object*, Eo*)  \
   , destructor(elm_view_list_destructor)

EO3_DECLARE_CLASS(ELM_VIEW_LIST)


#endif // ELM_VIEW_LIST_H

