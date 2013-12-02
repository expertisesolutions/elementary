
#ifndef ELM_MODEL_TREE_PRIVATE_H
#define ELM_MODEL_TREE_PRIVATE_H

#include "elm_model_tree.h"

struct _Elm_Model_Tree_Node;
typedef struct _Elm_Model_Tree_Node Elm_Model_Tree_Node;

/**
 * @brief Create a node instance and append it to the list of children
 * of its parent.
 * @param value The node value.
 * @param parent The parent node.
 * @return The node instance.
 */
Elm_Model_Tree_Node* _tree_node_append(Eina_Value *value, Elm_Model_Tree_Node *parent);

/**
 * @brief Create a node instance and prepend it to the list of children
 * of its parent.
 * @param value The node value.
 * @param parent The parent node.
 * @return The node instance.
 */
Elm_Model_Tree_Node* _tree_node_prepend(Eina_Value *value, Elm_Model_Tree_Node *parent);

/**
 * @brief Delete a node instance.
 * @param node The node instance.
 */
void _tree_node_del(Elm_Model_Tree_Node *node);

/**
 * @brief Gets the path to a node. 
 * @param node The node instance.
 * @return The path to the node.
 */
Elm_Model_Tree_Path* _tree_node_path(Elm_Model_Tree_Node *node);

/**
 * @brief Resolves a path and return a reference to that node.
 * @param root The root node.
 * @param path The path.
 * @return Return a reference to the node pointed by path if path
 * could be found. Otherwise it returns NULL.
 */
Elm_Model_Tree_Node* _tree_node_find(Elm_Model_Tree_Node *root, Elm_Model_Tree_Path *path);

/**
 * @brief Get the index of a node in relation to its siblings, i.e.,
 * its position in the list of children of its parent.
 * @param node The node instance.
 * @return If the node is the root node or in case of failure this
 * function returns (unsigned)-1, otherwise it returns the index.
 */
unsigned int _tree_node_index(Elm_Model_Tree_Node *node);

/**
 * @brief Get the value of a node.
 * @param node The node instance.
 * @return The value.
 */
Eina_Value* _tree_node_value_get(Elm_Model_Tree_Node *node);

/**
 * @brief Set the value of a node.
 * @param node The node instance.
 * @param value The value.
 * @return This function returns EINA_TRUE if the value could be
 * successfully set, otherwise it returns EINA_FALSE.
 */
Eina_Bool _tree_node_value_set(Elm_Model_Tree_Node *node, Eina_Value *value);

#endif // ELM_MODEL_TREE_PRIVATE_H
