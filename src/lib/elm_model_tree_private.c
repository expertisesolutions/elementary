#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_tree_private.h"

#include <assert.h>

struct _Elm_Model_Tree_Node
{
  struct _Elm_Model_Tree_Node *parent;
  struct _Elm_Model_Tree_Node *first_child;
  struct _Elm_Model_Tree_Node *next_sibling;
  Eina_Value *value;
};

Elm_Model_Tree_Node*
_tree_node_append(Eina_Value *value, Elm_Model_Tree_Node *parent)
{
   Elm_Model_Tree_Node *ptr, *node;

   node = calloc(1, sizeof(Elm_Model_Tree_Node));
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   node->first_child = node->next_sibling = NULL;
   node->value = value;
   if(parent)
     {
        if(parent->first_child == NULL)
          {
             parent->first_child = node;
          }
        else 
          {
             // find last child
             for(ptr = parent->first_child;
                 ptr->next_sibling != NULL;
                 ptr = ptr->next_sibling)
                ;
             ptr->next_sibling = node;
          }
     }
   node->parent = parent;
   return node;
}

Elm_Model_Tree_Node*
_tree_node_prepend(Eina_Value *value, Elm_Model_Tree_Node *parent)
{
   Elm_Model_Tree_Node *node;

   node = malloc(sizeof(Elm_Model_Tree_Node));
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   node->first_child = node->next_sibling = NULL;
   node->value = value;
   if(parent)
     {
        node->next_sibling = parent->first_child;
        parent->first_child = node;
     }
   node->parent = parent;
   return node;
}

void
_tree_node_del(Elm_Model_Tree_Node *node)
{
   Elm_Model_Tree_Node *child, *tmp, *ptr;
   if(node == NULL) return;

   // delete all sub-trees
   if(node->first_child != NULL)
     {
        for(child = node->first_child; child != NULL; child = tmp)
          {
             tmp = child->next_sibling;
             _tree_node_del(child);
          }
     }
   // remove node from parent's children
   if(node->parent)
     {
        if(node == node->parent->first_child)
          {
             node->parent->first_child = node->next_sibling;
          }
        for(tmp = NULL, ptr = node->parent->first_child;
            ptr != NULL;
            ptr = ptr->next_sibling)
          {
             if(ptr == node)
               {
                  if(tmp) tmp->next_sibling = ptr;
               }
             tmp = ptr;
          }
     }
   eina_value_free(node->value);
   free(node);
}

Elm_Model_Tree_Path*
_tree_node_path(Elm_Model_Tree_Node *node)
{
   Elm_Model_Tree_Path *path;
   Elm_Model_Tree_Node *ptr;

   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   path = elm_model_tree_path_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   for(ptr = node; ptr->parent != NULL; ptr = ptr->parent)
     {
        elm_model_tree_path_prepend_index(path, _tree_node_index(ptr));
     }
   return path;
}

Elm_Model_Tree_Node*
_tree_node_find(Elm_Model_Tree_Node *root, Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *child, *ptr;
   unsigned int i, j, depth, *indices;

   EINA_SAFETY_ON_NULL_RETURN_VAL(root, NULL);
   if(path == NULL) return NULL;

   depth = elm_model_tree_path_get_depth(path);
   indices = elm_model_tree_path_get_indices(path);
   for(ptr = root, i = 0; i < depth; i++)
     {
        for(j = 0, child = ptr->first_child;
            j < indices[i] && child != NULL;
            j++, child = child->next_sibling)
           ;
        EINA_SAFETY_ON_NULL_RETURN_VAL(child, NULL);
        ptr = child;
     }
   return ptr;
}

unsigned int
_tree_node_index(Elm_Model_Tree_Node *node)
{
   Elm_Model_Tree_Node *child;
   unsigned int index = 0;
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, (unsigned int)-1);
   if(node->parent == NULL) return (unsigned int)-1;
   EINA_SAFETY_ON_NULL_RETURN_VAL(node->parent->first_child, (unsigned int)-1);
   for(child = node->parent->first_child;
       child != NULL;
       child = child->next_sibling)
     {
        if(child == node) { return index; }
        index++;
     }
   return (unsigned int)-1;
}

Eina_Value*
_tree_node_value_get(Elm_Model_Tree_Node *node)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   return node->value;
}

Eina_Bool
_tree_node_value_set(Elm_Model_Tree_Node *node, Eina_Value *value)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, EINA_FALSE);
   if(node->value != NULL)
     {
        eina_value_free(node->value);
     }
   node->value = value;
   return EINA_TRUE;
}

