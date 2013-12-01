
#ifndef ELM_VIEW_TREE_H
#define ELM_VIEW_TREE_H

/**
 * @brief EO3 Class Declaration
 */
#define ELM_VIEW_TREE elm_view_tree                     \
   , constructor(elm_view_tree_add, _add, Evas_Object*, Eo*)         \
   , destructor(_destructor) \
   , function(elm_view_tree_node_selected, _node_selected, Eina_Bool, Elm_Model_Tree_Path)      \
   , function(elm_view_tree_node_inserted, _node_inserted, Eina_Bool, Elm_Model_Tree_Path)      \
   , function(elm_view_tree_node_deleted,  _node_deleted,  Eina_Bool, Elm_Model_Tree_Path)      \
   , function(elm_view_tree_node_changed,  _node_changed,  Eina_Bool, Elm_Model_Tree_Path)      \
   , function(elm_view_tree_reordered, _reordered)

EO3_DECLARE_CLASS(ELM_VIEW_TREE)


#endif // ELM_VIEW_TREE_H

