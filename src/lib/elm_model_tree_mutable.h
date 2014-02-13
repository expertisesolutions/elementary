#ifndef ELM_MODEL_TREE_MUTABLE_H
#define ELM_MODEL_TREE_MUTABLE_H



#define ELM_MODEL_TREE_CLASS elm_obj_model_tree_class_get()
const Eo_Class *elm_obj_model_tree_class_get(void);

extern EAPI Eo_Op ELM_MODEL_TREE_BASE_ID;

enum {
    ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND,
    ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND,
    ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND_RELATIVE,
    ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND_RELATIVE,
    ELM_OBJ_MUTABLE_SUB_ID_TREE_DELETE,
    ELM_OBJ_MUTABLE_SUB_ID_TREE_VALUE_SET,
    ELM_OBJ_MUTABLE_SUB_ID_TREE_CHILD_APPEND_EVT,
    ELM_OBJ_MUTABLE_SUB_ID_LAST
};

#define ELM_OBJ_MODEL_TREE_ID(sub_id) (ELM_MODEL_TREE_BASE_ID + sub_id)

//#define elm_model_file_tree_constructor(value) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(Eina_Value *, value)
//      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _model_tree_constructor),
/**
 * @def elm_model_tree_item_append
 * @since 1.8
 *
 * @param path The path to a node where the new child shall be appended.
 * @param value The value to append. The value is not copied so it's
 * this class responsibility to deleted it.
 * @return The path to the newly created node.
 *
 * Append a new child cointaining @p value to the list of children of
 * the node pointed by @p path.
 */
#define elm_model_tree_child_append(unused, path, value, ret) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND), EO_TYPECHECK(void *, unused), EO_TYPECHECK(Elm_Model_Tree_Path*, path), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_Tree_Path **, ret)

/**
 * @def elm_model_tree_item_prepend
 * @since 1.8
 *
 * @param path The path to a node where the new child shall be prepended.
 * @param value The value to prepend.
 * @return The path to the newly created node.
 *
 * Prepend a new child cointaining @p value to the list of children of
 * the node pointed by @p path.
 */
#define elm_model_tree_child_prepend(path, value, ret) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND), EO_TYPECHECK(Elm_Model_Tree_Path *, path), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_Tree_Path **, ret)

/**
 * @def elm_model_tree_child_append_relative
 * @since 1.8
 *
 * @param path The path to a node where the new child shall be appended.
 * @param value The value to set to the newly created node, as @ref Eina_Value.
 * @return The path to the newly created node.
 *
 * Append @p value as a sibling node of @p path.
 */
#define elm_model_tree_child_append_relative(path, value, ret) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND_RELATIVE), EO_TYPECHECK(Elm_Model_Tree_Path*, path), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_Tree_Path **, ret)

/**
 * @def elm_model_tree_child_prepend_relative
 * @since 1.8
 *
 * @param path The path to a sibling node where the new node shall be prepended.
 * @param value The value to be set in the newly created node, as @ref Eina_Value.
 * @return The path to the newly created node.
 *
 * Prepend @p value as a sibling node of @p path.
 */
//-static Elm_Model_Tree_Path*
//-_model_tree_child_prepend_relative(Eo *object, void *_unused EINA_UNUSED,
//-                                   Elm_Model_Tree_Path *path, Eina_Value *value)

#define elm_model_tree_child_prepend_relative(path, value, ret) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND_RELATIVE), EO_TYPECHECK(Elm_Model_Tree_Path*, path), EO_TYPECHECK(Eina_Value *, value), EO_TYPECHECK(Elm_Model_Tree_Path **, ret)

/**
 * @def elm_model_tree_delete
 * @since 1.8
 *
 * @param path Path to the sub-tree to delete.
 *
 * Delete the sub-tree pointed by @p path and all its children.
 */
#define elm_model_tree_delete(path) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_TREE_DELETE), EO_TYPECHECK(Elm_Model_Tree_Path*, path)

/**
 * @def elm_model_tree_value_set
 * @since 1.8
 *
 * @param path The path to the node to receive the new value.
 * @param value The value, as @ref Eina_Value.
 *
 * Set @p value to the node pointed by @p path.
 */
#define elm_model_tree_value_set(unused, path, value) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_TREE_VALUE_SET), EO_TYPECHECK(void *, unused), EO_TYPECHECK(Elm_Model_Tree_Path*, path), EO_TYPECHECK(Eina_Value *, value)

// Append Event
//#define elm_model_tree_child_append_evt(path) ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_TREE_CHILD_APPEND_EVT), EO_TYPECHECK(Elm_Model_Tree_Path *, path)

extern const Eo_Event_Description _TREE_CHILD_APPEND_EVT;
#define TREE_CHILD_APPEND_EVT (&(_TREE_CHILD_APPEND_EVT))

extern const Eo_Event_Description _TREE_DELETE_EVT;
#define TREE_DELETE_EVT (&(_TREE_DELETE_EVT))

extern const Eo_Event_Description _TREE_VALUE_SET_EVT;
#define TREE_VALUE_SET_EVT (&(_TREE_VALUE_SET_EVT))

#endif // ELM_MODEL_TREE_MUTABLE_H
