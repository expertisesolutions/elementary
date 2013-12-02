#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_tree.h"

#include "elm_model_tree_private.h"

#include <assert.h>

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
_model_tree_constructor(Eo *object, Elm_Model_Tree *model, Eina_Value *value)
{
   Elm_Model_Tree_Node *root;
   eo2_do_super(object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS), eo2_constructor());

   EINA_SAFETY_ON_NULL_RETURN(model);
   root = _tree_node_append(value, NULL);
   EINA_SAFETY_ON_NULL_RETURN(root);
   model->root = root;
   model->selected = NULL;
   eina_lock_new(&model->lock);
}

static void
_model_tree_destructor(Eo *object, Elm_Model_Tree *model)
{
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   _tree_node_del(model->root);
   elm_model_tree_path_free(model->selected);
   eina_lock_release(&model->lock);
   eina_lock_free(&model->lock);
   eo2_do_super(object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS), eo2_destructor());
}

static Eina_Bool
_model_tree_select(Eo *object, Elm_Model_Tree *model, Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;

   EINA_SAFETY_ON_NULL_RETURN_VAL(model, EINA_FALSE);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   model->selected = node ? path : NULL;
   eina_lock_release(&model->lock);
   eo2_do(object, elm_model_tree_select_callback_call(path));
   
   return node ? EINA_TRUE : EINA_FALSE;
}

static Eina_Value*
_model_tree_value_get(Eo *object EINA_UNUSED,
                      Elm_Model_Tree *model,
                      Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;
   Eina_Value *value;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   if(node == NULL)
     {
        return NULL;
     }
   value = _tree_node_value_get(node);
   eina_lock_release(&model->lock);
   
   return value;
}

static Eina_List*
_model_tree_children_get(Eo *object EINA_UNUSED,
                         Elm_Model_Tree *model,
                         Elm_Model_Tree_Path *path)
{
   return NULL; // TODO implement
}
 
static Elm_Model_Tree_Path*
_model_tree_selected_get(Eo *object EINA_UNUSED, Elm_Model_Tree *model)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   return model->selected;
}

static void
_model_tree_release(Eo *object, void *_unused EINA_UNUSED, Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get
     (object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   if(path == NULL) return;
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_RETURN(node);
   _tree_node_del(node);
   eina_lock_release(&model->lock);
   eo2_do(object, elm_model_tree_delete_callback_call(path));
}

EO3_DEFINE_CLASS(ELM_MODEL_TREE_CONST_CLASS, ((EO3_BASE_CLASS)), Elm_Model_Tree)


/*
 * Mutable Class definition
 */

static Elm_Model_Tree_Path*
_model_tree_child_append(Eo *object, void *_unused EINA_UNUSED,
                         Elm_Model_Tree_Path *path, Eina_Value *value)
{
   Elm_Model_Tree_Path *ret = NULL;
   Elm_Model_Tree_Node *node, *parent;
   Elm_Model_Tree *model = eo_data_scope_get
     (object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));

   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   eina_lock_take(&model->lock);
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(parent, exit_err);
   node = _tree_node_append(value, parent);
   EINA_SAFETY_ON_NULL_GOTO(node, exit_err);
   ret = _tree_node_path(node);
   eina_lock_release(&model->lock);
   eo2_do(object, elm_model_tree_child_append_callback_call(ret));
   return ret;
   
 exit_err:
   eina_lock_release(&model->lock);
   return NULL;
}

static Elm_Model_Tree_Path*
_model_tree_child_prepend(Eo *object, void* _unused EINA_UNUSED,
                          Elm_Model_Tree_Path *path, Eina_Value *value)
{
   Elm_Model_Tree_Path *ret = NULL;
   Elm_Model_Tree_Node *node, *parent;
   Elm_Model_Tree *model = eo_data_scope_get
     (object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   eina_lock_take(&model->lock);   
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(parent, exit_err);
   node = _tree_node_prepend(value, parent);
   EINA_SAFETY_ON_NULL_GOTO(node, exit_err);
   ret = _tree_node_path(node);
   eina_lock_release(&model->lock);
   eo2_do(object, elm_model_tree_child_append_callback_call(ret));
   return ret;

exit_err:
   eina_lock_release(&model->lock);
   return NULL;
}

static Elm_Model_Tree_Path*
_model_tree_child_append_relative(Eo *object, void *_unused EINA_UNUSED,
                                  Elm_Model_Tree_Path *path, Eina_Value *value)
{
   return NULL; // TODO implement
}

static Elm_Model_Tree_Path*
_model_tree_child_prepend_relative(Eo *object, void *_unused EINA_UNUSED,
                                   Elm_Model_Tree_Path *path, Eina_Value *value)
{
   return NULL; // TODO implement
}

static void
_model_tree_delete(Eo *object, void *_unused EINA_UNUSED, Elm_Model_Tree_Path *path)
{
  
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get
     (object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   EINA_SAFETY_ON_NULL_RETURN(path);
   EINA_SAFETY_ON_NULL_RETURN(model);
   eo2_do(object, elm_model_tree_release(path));
   eo2_do(object, elm_model_tree_delete_callback_call(path));
}

static void
_model_tree_value_set(Eo *object, void *_unused EINA_UNUSED,
                      Elm_Model_Tree_Path *path, Eina_Value *value)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get(object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   
   if(path == NULL) return;
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   _tree_node_value_set(node, value);
   eina_lock_release(&model->lock);
   eo2_do(object, elm_model_tree_value_set_callback_call(path, value));   
}

EO3_DEFINE_CLASS(ELM_MODEL_TREE_CLASS, ((ELM_MODEL_TREE_CONST_CLASS)), NULL)


