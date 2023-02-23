/*
   AVL tree implementation.

   Author: A. Tafliovich
   Based heavily on materials developed by F. Estrada.
*/

#include "AVL_tree.h"

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.
 */
int height(AVL_Node* node)
{
  if (node == NULL)
  {
    return 0;
  }
  return (node->height);
}
/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void update_height(AVL_Node* node)
{
  if (node->left == NULL)
  {
    if (node->right == NULL)
    {
      node->height = 1;
      return;
    }
    node->height = height(node->right) + 1;
    return;
  }
  else if(node->right == NULL)
  {
    node->height = height(node->left) + 1;
    return;
  }
  int left_height = height(node->left);
  int right_height = height(node->right);
  if (left_height >= right_height)
  {
    node->height = left_height + 1;
    return;
  }
  else if (right_height > left_height)
  {
    node->height = right_height +1;
    return;
  }
}
/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 of node is NULL.
 */
int balance_factor(AVL_Node* node)
{
  if (node == NULL)
  {
    return 0;
  }
  return (height(node->left) - height(node->right));
}
/* Returns the result of performing the corresponding rotation in the AVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
AVL_Node* right_rotation(AVL_Node* node)
{
  AVL_Node* holder;
  holder = node->left;
  node->left = holder->right;
  holder->right = node;
  update_height(node);
  update_height(holder);
  return holder;
}
// single rotations: left/counter-clockwise
AVL_Node* left_rotation(AVL_Node* node)
{
  AVL_Node* holder = node->right;
  node->right = holder->left;
  holder->left = node;
  update_height(node);
  update_height(holder);
  return holder;
}
// double rotation: right/clockwise then left/counter-clockwise
AVL_Node* right_left_rotation(AVL_Node* node)
{
  node->right = right_rotation(node->right);
  return left_rotation(node);
}
// double rotation: left/counter-clockwise then right/clockwise
AVL_Node* left_right_rotation(AVL_Node* node)
{
  node->left = left_rotation(node->left);
  return right_rotation(node);
}
/* Returns the successor node of 'node'. */
AVL_Node* successor(AVL_Node* node)
{
  AVL_Node *succ = node->right;
  while (succ->left != NULL)
  {
    succ = succ->left;
  }
  return succ;
}

/* Creates and returns an AVL tree node with key 'key', value 'value', height
 * of 1, and left and right subtrees NULL.
 */
AVL_Node* create_node(int key, void* value)
{
  AVL_Node *new_node = malloc(sizeof(AVL_Node));
  new_node->key = key;
  new_node->value = value;
  new_node->height = 1;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

/*************************************************************************
 ** Provided functions
 *************************************************************************/
void print_tree_inorder_(AVL_Node* node, int offset) {
  if (node == NULL) return;
  print_tree_inorder_(node->right, offset + 1);
  printf("%*s %d [%d]\n", offset, "", node->key, node->height);
  print_tree_inorder_(node->left, offset + 1);
}

void print_tree_inorder(AVL_Node* node) {
  print_tree_inorder_(node, 0);
}

void delete_tree(AVL_Node* node) {
  if (node == NULL) return;
  delete_tree(node->left);
  delete_tree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

AVL_Node* search(AVL_Node* node, int key)
{
  if (node == NULL || node->key == key)
  {
    return node;
  }
  if (key < node->key)
  {
    return search(node->left, key);
  }
  if (key > node->key)
  {
    return search(node->right, key);
  }
  return node;
}

AVL_Node* insert(AVL_Node* node, int key, void* value)
{
  if (node == NULL)
  {
    return create_node(key, value);
  }
  else if (node->key == key)
  {
    return node;
  }
  else if (key < node->key)
  {
    node->left = insert(node->left, key, value);
    update_height(node);
  }
  else if (key > node->key)
  {
    node->right = insert(node->right, key, value);
    update_height(node);
  }
  int balance = balance_factor(node);
  AVL_Node *new_head = node;
  if (balance < -1)
  {
    if (balance_factor(node->right) > 0)
    {
      new_head = right_left_rotation(node);
    }
    else
    {
      new_head = left_rotation(node);
    }
  }
  else if (balance > 1)
  {
    if (balance_factor(node->left) < 0)
    {
      new_head = left_right_rotation(node);
    }
    else
    {
      new_head = right_rotation(node);
    }
  }
  return new_head;
}

AVL_Node* delete(AVL_Node* node, int key)
{
  if (node == NULL)
  {
    return node;
  }
  else if (key < node->key)
  {
    node->left = delete(node->left, key);
  }
  else if (key > node->key)
  {
    node->right = delete(node->right, key);
  }
  else if(node->key == key)
  {
    if(node->left == NULL && node->right == NULL)
    {
      free(node);
      return NULL;
    }
    else if(node->left == NULL || node->right == NULL)
    {
      if (node->left == NULL)
      {
        return node->right;
      }
      else if (node->right == NULL)
      {
        return node->left;
      }
    }
    else if (node->left != NULL && node->right != NULL)
    {
      AVL_Node *succ = successor(node);
      int key_update = succ->key;
      void *value_update = node->value;
      delete(node, succ->key);
      node->key = key_update;
      node->value = value_update;
    }
  }
  update_height(node);
  int balance = balance_factor(node);
  AVL_Node *new_head = node;
  if (balance < -1)
  {
    if (balance_factor(node->right) > 0)
    {
      new_head = right_left_rotation(node);
    }
    else
    {
      new_head = left_rotation(node);
    }
  }
  if (balance > 1)
  {
    if (balance_factor(node->left) < 0)
    {
      new_head = left_right_rotation(node);
    }
    else
    {
      new_head = right_rotation(node);
    }
  }
  return new_head;
}
