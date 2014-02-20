#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_tree.h"

#include "elm_model_tree_private.h"

#include <assert.h>

#define MY_MODEL_TREE_CONST_CLASS ELM_OBJ_MODEL_TREE_CONST_CLASS
EAPI Eo_Op ELM_OBJ_MODEL_TREE_CONST_BASE_ID = 0;

EAPI const Eo_Event_Description _ELM_MODEL_TREE_CONST_SELECT_EVT =
   EO_EVENT_DESCRIPTION("tree const, select","Select the node pointed by path.");

struct _Elm_Model_Tree
{
   Elm_Model_Tree_Node *root;
   Elm_Model_Tree_Path* selected;
   Eina_Lock lock;
};

typedef struct _Elm_Model_Tree Elm_Model_Tree;


/*
 * Const Class definition
 */
static void
_model_tree_constructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_Model_Tree_Node *root;
   Elm_Model_Tree *model = class_data;
   Eina_Value *value = va_arg(*list, Eina_Value *);

   eo_do_super(obj, MY_MODEL_TREE_CONST_CLASS, eo_constructor());

   EINA_SAFETY_ON_NULL_RETURN(model);
   root = _tree_node_append(value, NULL);
   EINA_SAFETY_ON_NULL_RETURN(root);
   model->root = root;
   model->selected = NULL;
   eina_lock_new(&model->lock);
}

static void
_model_tree_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{
   Elm_Model_Tree *model = class_data;

   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   _tree_node_del(model->root);
   elm_model_tree_path_free(model->selected);
   eina_lock_release(&model->lock);
   eina_lock_free(&model->lock);
   eo_do_super(obj, MY_MODEL_TREE_CONST_CLASS, eo_destructor());
}

static void
_model_tree_select(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = class_data;
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);

   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   model->selected = node ? path : NULL;
   eina_lock_release(&model->lock);
   eo_do(obj, eo_event_callback_call(ELM_MODEL_TREE_CONST_SELECT_EVT, ret, NULL));

   if (node) *ret = EINA_TRUE;
   else *ret = EINA_FALSE;
}

static void
_model_tree_value_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_Model_Tree_Node *node;

   Elm_Model_Tree *model = class_data;
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value **value = va_arg(*list, Eina_Value **);

   EINA_SAFETY_ON_NULL_GOTO(path, release);
   EINA_SAFETY_ON_NULL_GOTO(model, release);

   eina_lock_take(&model->lock);

   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(node, release);

   *value = _tree_node_value_get(node);
   eina_lock_release(&model->lock);
   return;

release:
   *value = NULL;
   eina_lock_release(&model->lock);
}

static void
_model_tree_children_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   (void)va_arg(*list, Elm_Model_Tree_Path *); //skip unused arg
   Elm_Model_Tree *model = (Elm_Model_Tree *)class_data;
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_List **children = va_arg(*list, Eina_List **);

   Elm_Model_Tree_Node *node = NULL;

   EINA_SAFETY_ON_NULL_GOTO(path, release);
   EINA_SAFETY_ON_NULL_GOTO(model, release);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(node, release);
   *children = _tree_node_children_path_get(node);
   EINA_SAFETY_ON_NULL_GOTO(*children, release);
   eina_lock_release(&model->lock);
   return;

release:
   *children = NULL;
   eina_lock_release(&model->lock);
}

static void
_model_tree_children_count(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   (void)va_arg(*list, Elm_Model_Tree_Path *); //skip unused arg
   Elm_Model_Tree *model = (Elm_Model_Tree *)class_data;
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   unsigned int *n = va_arg(*list, unsigned int *);
   Elm_Model_Tree_Node *node;

   EINA_SAFETY_ON_NULL_GOTO(path, release);
   EINA_SAFETY_ON_NULL_GOTO(model, release);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(node, release);
   *n = _tree_node_children_count(node);
   eina_lock_release(&model->lock);
   return;

release:
   *n = 0;
   eina_lock_release(&model->lock);
}

static void
_model_tree_selected_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Elm_Model_Tree *model = class_data;
   EINA_SAFETY_ON_NULL_RETURN(model);
   Elm_Model_Tree_Path **ret = va_arg(*list, Elm_Model_Tree_Path **);
   *ret = model->selected;
}

static void
_model_tree_release(Eo *obj, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);

   Elm_Model_Tree *model = eo_data_scope_get(obj, MY_MODEL_TREE_CONST_CLASS);
   if(path == NULL) return;
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_RETURN(node);
   _tree_node_del(node);
   eina_lock_release(&model->lock);
   
   //TODO/FIXME/XXX 
   //eo_do(obj, eo_event_callback_call(TREE_DELETE_EVT, path, NULL));
   //eo2_do(object, elm_model_tree_delete_callback_call(path));
}

static void
_model_tree_const_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _model_tree_constructor),
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _model_tree_destructor),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECT), _model_tree_select),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_VALUE_GET), _model_tree_value_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_CHILDREN_GET), _model_tree_children_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_CHILDREN_COUNT), _model_tree_children_count),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECTED_GET), _model_tree_selected_get),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_CONST_ID(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_RELEASE), _model_tree_release),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description model_tree_const_op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECT, "Select the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_VALUE_GET, "Get the value of the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_CHILDREN_GET, "Get the children of the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_CHILDREN_COUNT, "Count number of children of the node pointed by path."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_SELECTED_GET, "Get the path of the selected node."),
   EO_OP_DESCRIPTION(ELM_OBJ_MODEL_TREE_CONST_SUB_ID_RELEASE, "Release the sub-tree pointed by path and all its children. Release does not delete the nodes, it just frees their nodes."),
   EO_OP_DESCRIPTION_SENTINEL
};

static const Eo_Event_Description *model_tree_const_event_descs[] = {
     ELM_MODEL_TREE_CONST_SELECT_EVT,
     NULL
};

static Eo_Class_Description model_tree_const_class_descs = {
   EO_VERSION,
   "Model Tree Const",
   EO_CLASS_TYPE_REGULAR_NO_INSTANT,
   EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_MODEL_TREE_CONST_BASE_ID, model_tree_const_op_descs, ELM_OBJ_MODEL_TREE_CONST_SUB_ID_LAST),
   model_tree_const_event_descs, // event_descs
   sizeof(Elm_Model_Tree),
   _model_tree_const_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_model_tree_const_class_get, &model_tree_const_class_descs, EO_BASE_CLASS, NULL);


/**
 * @see elm_model_tree_mutable.h
 */
EAPI Eo_Op ELM_MODEL_TREE_BASE_ID = 0;

#define MY_MODEL_TREE_CLASS ELM_MODEL_TREE_CLASS

EAPI const Eo_Event_Description _TREE_CHILD_APPEND_EVT =
   EO_EVENT_DESCRIPTION("child,append","Append a new child cointaining value to the \
                        list of children of the node pointed by path.");

EAPI const Eo_Event_Description _TREE_DELETE_EVT =
   EO_EVENT_DESCRIPTION("tree,delete","Delete the sub-tree pointed path and all its children.");

EAPI const Eo_Event_Description _TREE_VALUE_SET_EVT =
   EO_EVENT_DESCRIPTION("value,set","Set value to the node pointed by path");

/*
 * Mutable Class definition
 */
static void
_model_tree_child_append(Eo *obj, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Node *node, *parent;
   Elm_Model_Tree *model = (Elm_Model_Tree *)eo_data_scope_get(obj, ELM_OBJ_MODEL_TREE_CONST_CLASS);

   //skip arg
   //TODO/FIXME/XXX: Let's let compiler warn us about this 
   //unused variable, thus must be fixed soon.
   void *unused = va_arg(*list, void *); 

   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_Tree_Path **ret = va_arg(*list, Elm_Model_Tree_Path **); //param[out]

   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(parent, release);
   node = _tree_node_append(value, parent);
   EINA_SAFETY_ON_NULL_GOTO(parent, release);
   EINA_SAFETY_ON_NULL_GOTO(node, release);
   *ret = _tree_node_path(node);
   eina_lock_release(&model->lock);

   eo_do(obj, eo_event_callback_call(TREE_CHILD_APPEND_EVT, *ret, NULL));
   return;

release:
   eina_lock_release(&model->lock);
   *ret = NULL;
}

static void
_model_tree_child_prepend(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Node *node, *parent;
   Elm_Model_Tree *model = eo_data_scope_get(obj, MY_MODEL_TREE_CLASS);

   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_Tree_Path **ret = va_arg(*list, Elm_Model_Tree_Path **); //param[out]

   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);

   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(parent, release);
   node = _tree_node_prepend(value, parent);
   EINA_SAFETY_ON_NULL_GOTO(node, release);
   *ret = _tree_node_path(node);
   eina_lock_release(&model->lock);

   eo_do(obj, eo_event_callback_call(TREE_CHILD_APPEND_EVT, *ret, NULL));
   return;

release:
   eina_lock_release(&model->lock);
   *ret = NULL;
}

static void
_model_tree_child_append_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   return; // TODO implement
}

static void
_model_tree_child_prepend_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   return; // TODO implement
}

static void
_model_tree_delete(Eo *obj, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree *model = eo_data_scope_get(obj, ELM_OBJ_MODEL_TREE_CONST_CLASS);
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);

   EINA_SAFETY_ON_NULL_RETURN(path);
   EINA_SAFETY_ON_NULL_RETURN(model);
   eo_do(obj, elm_model_tree_release(path));

   //TODO/FIXME/XXX 
   eo_do(obj, eo_event_callback_call(TREE_DELETE_EVT, path, NULL));
}

static void
_model_tree_value_set(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get(obj, ELM_OBJ_MODEL_TREE_CONST_CLASS);

   //TODO/FIXME/XXX: Let's let compiler warn us about this 
   //unused variable, thus must be fixed soon.
   void *unused = va_arg(*list, void *); // skip unused argument
   
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);

   if(path == NULL) return;
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   _tree_node_value_set(node, value);
   eina_lock_release(&model->lock);

   //TODO/FIXME/XXX: nedd to implement TREE_VALUE_SET_EVT && call
   //eo2_do(obj, _model_tree_value_set_callback_call(path, value)); //eo2 original
   //eo_do(obj, eo_event_callback_call(TREE_VALUE_SET_EVT, "xxx", NULL)); //new eo
}

static void
_mutable_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND), _model_tree_child_append),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND), _model_tree_child_prepend),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND_RELATIVE), _model_tree_child_append_relative),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND_RELATIVE), _model_tree_child_prepend_relative),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_TREE_DELETE), _model_tree_delete),
      EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_TREE_VALUE_SET), _model_tree_value_set),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}


static const Eo_Op_Description mutable_op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND, "Append new child"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND, "Prepend new child"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND_RELATIVE, "Append as a siblind node"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND_RELATIVE, "Prepend as a sibling node"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_TREE_DELETE, "Delete node"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_TREE_VALUE_SET, "Set value to the node"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_TREE_CHILD_APPEND_EVT, "Append new child event."),
   EO_OP_DESCRIPTION_SENTINEL
};

static const Eo_Event_Description *mutable_event_descs[] = {
   TREE_CHILD_APPEND_EVT,
   TREE_DELETE_EVT, //TODO/FIXME/XXX: implement this
   TREE_VALUE_SET_EVT, //TODO/FIXME/XXX: Implement this
   NULL
};

static Eo_Class_Description mutable_class_descs = {
   EO_VERSION,
   "Model Tree",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_MODEL_TREE_BASE_ID, mutable_op_descs, ELM_OBJ_MUTABLE_SUB_ID_LAST),
   mutable_event_descs, // event_descs
   0,
   _mutable_class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_model_tree_class_get, &mutable_class_descs, ELM_OBJ_MODEL_TREE_CONST_CLASS, NULL);

