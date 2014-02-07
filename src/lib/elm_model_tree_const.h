#ifndef ELM_MODEL_TREE_CONST_H
#define ELM_MODEL_TREE_CONST_H

//#define ELM_OBJ_TREE_MUTABLE_CLASS elm_obj_tree_mutable_class_get()
//const Eo_Class *elm_obj_tree_mutable_class_get(void);

#define ELM_OBJ_MODEL_TREE_CONST_CLASS elm_obj_model_tree_const_class_get()
const Eo_Class *elm_obj_model_tree_const_class_get(void);

extern EAPI Eo_Op ELM_OBJ_MODEL_TREE_CONST_BASE_ID;

enum {
     ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECT, //event
     ELM_OBJ_MODEL_TREE_CONST_SUB_ID_VALUE_GET,
     ELM_OBJ_MODEL_TREE_CONST_SUB_ID_CHILDREN_GET,
     ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECTED_GET,
     ELM_OBJ_MODEL_TREE_CONST_SUB_ID_RELEASE,
     ELM_OBJ_MODEL_TREE_CONST_SUB_ID_LAST
};

#define ELM_OBJ_MODEL_TREE_CONST_ID(sub_id) (ELM_OBJ_MODEL_TREE_CONST_BASE_ID + sub_id) 
 
#define elm_model_tree_constructor(value) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(Eina_Value *, value)

/**
 * @def elm_model_tree_select
 * @since 1.8
 *
 * @param path Path to the node to be selected. If @p path is NULL
 * the currently selected node is unselected.
 * 
 * Select the node pointed by @p path.
 */
//EAPI Eina_Bool elm_model_tree_select(Elm_Model_Tree_Path *path);
#define elm_model_tree_select(path, ret) ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECT), EO_TYPECHECK(Elm_Model_Tree_Path *, path), EO_TYPECHECK(Eina_Bool *, ret)

/**
 * @def elm_model_tree_value_get
 * @since 1.8
 *
 * @param path The node from which @p value is read.
 * @return The value read from the node, as @ref Eina_Value.
 * 
 * Get the value of the node pointed by @p path.
 *
 */
//EAPI Eina_Value* elm_model_tree_value_get(Elm_Model_Tree_Path *path);
#define elm_model_tree_value_get(path, value) ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_VALUE_GET), EO_TYPECHECK(Elm_Model_Tree_Path *, path), EO_TYPECHECK(Eina_Value **, value) 

/**
 * @def elm_model_tree_children_get
 * @since 1.8
 *
 * @param path The node from which @p children is listed
 * @return The a list children path, as Eina_list of Elm_Model_Tree_Path.
 *
 * Get the children of the node pointed by @p path.
 *
 */
//EAPI Eina_List* elm_model_tree_children_get(Elm_Model_Tree_Path *path);
#define elm_model_tree_children_get(path, ret) ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_CHILDREN_GET), EO_TYPECHECK(Elm_Model_Tree_Path *, path), EO_TYPECHECK(Eina_List **, ret) 

/**
 * @def elm_model_tree_selected_get
 * @since 1.8
 *
 * @return The path of the selected node, as Elm_Model_Tree_Path.
 *
 * Get the path of the selected node.
 *
 */
//EAPI Elm_Model_Tree_Path* elm_model_tree_selected_get();
#define elm_model_tree_selected_get(ret) ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECTED_GET), EO_TYPECHECK(Elm_Model_Tree_Path **, ret)


/**
 * @def elm_model_tree_release
 * @since 1.8
 *
 * @param path Path to the sub-tree to release.
 * 
 * Release the sub-tree pointed by @p path and all its
 * children. Release does not delete the nodes, it just frees their
 * nodes.
 */
//EAPI void elm_model_tree_release(Elm_Model_Tree_Path *path);
#define elm_model_tree_release(path) ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_RELEASE), EO_TYPECHECK(Elm_Model_Tree_Path *, path)

/**
 * @brief Model tree const event
 */
extern const Eo_Event_Description _ELM_MODEL_TREE_CONST_SELECT_EVT;
#define ELM_MODEL_TREE_CONST_SELECT_EVT (&(_ELM_MODEL_TREE_CONST_SELECT_EVT))

#if 0
/**
 * @brief EO3 Class Declaration
 */
#define ELM_MODEL_TREE_CONST_CLASS elm_model_tree_const                                           \
 , constructor(elm_model_tree_constructor, _model_tree_constructor, Eina_Value*)                  \
 , destructor(_model_tree_destructor)                                                             \
 , function(elm_model_tree_select, _model_tree_select, Eina_Bool, Elm_Model_Tree_Path*)           \
 , function(elm_model_tree_value_get, _model_tree_value_get, Eina_Value*, Elm_Model_Tree_Path*)   \
 , function(elm_model_tree_children_get, _model_tree_children_get, Eina_List*, Elm_Model_Tree_Path*) \
 , function(elm_model_tree_selected_get, _model_tree_selected_get, Elm_Model_Tree_Path*)          \
 , function(elm_model_tree_release, _model_tree_release, void, Elm_Model_Tree_Path*)              \
 , event(elm_model_tree_select, Elm_Model_Tree_Path*)

EO3_DECLARE_CLASS(ELM_MODEL_TREE_CONST_CLASS) // XXX NON-INSTANTIABLE
#endif
#endif // ELM_MODEL_TREE_CONST_H
