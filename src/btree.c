/*
btree.c
Created by Steven Shea on 3/14/15.
for CMPS 1600, project 2
*/
#include "../include/animal/animal_game.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



slsBTree *sls_btree_new(slsCopyFn copy_fn, slsFreeFn free_fn) {
  slsBTree *tree = NULL;
  tree = calloc(sizeof(slsBTree), 1);
  /* ensure tree is allocated properly */
  if (tree) {
    tree->copy_fn = copy_fn;
    tree->free_fn = free_fn;

    tree->head = NULL;
  } else {
    fprintf(stderr, "%s tree is NULL!\n", __func__);
  }
  return tree;
}

void sls_btree_destroy(slsBTree *tree) {
  if (tree) {
    if (tree->head) {
      sls_bnode_destroy(tree->head);
    }

    free(tree);
  } else {
    fprintf(stderr, "%s tree is NULL!\n", __func__);
  }
}

slsBNode *sls_bnode_new(slsBTree *tree, void const *val, slsBNode *left,
                        slsBNode *right) {
  slsBNode *node = NULL;
  if (!tree) {
    fprintf(stderr, "ERROR %s: tree must be nonnull!\n", __func__);
    return NULL;
  }
  node = calloc(sizeof(slsBNode), 1);
  if (node) {
    node->tree = tree;
    node->val = tree->copy_fn(val);
    if (left) {
      sls_bnode_insert_left(node, left);
    }
    if (right) {
      sls_bnode_insert_right(node, right);
    }
  } else {
    fprintf(stderr, "ERROR %s: malloc error!\n", __func__);
  }
  return node;
}

void sls_bnode_destroy(slsBNode *node) {
  if (node) {

    assert(node != node->parent);

    assert((!node->parent) || node->val != node->parent->val);

    /**
     * recursively destroy child nodes
     */
    if (node->left) {
      sls_bnode_destroy(node->left);
      node->left = NULL;
    }
    if (node->right) {
      sls_bnode_destroy(node->right);
      node->right = NULL;
    }

    /* try freeing node data */
    if (node->tree) {
      node->tree->free_fn(node->val);
      node->val = NULL;
    } else {
      fprintf(stderr, "WARNING %s: node->tree is null. Cannot free node data\n",
              __func__);
    }

    free(node);
  } else {
    fprintf(stderr, "WARNING %s %i:\n\tnode is null\n", __FILE__, __LINE__);
  }
}

slsBNode **sls_bnode_select_child(slsBNode *node, slsChildSelector dirrection)
{
  if (!node) {
    return NULL;
  }
  return dirrection == SLS_CHILD_RIGHT?
    &(node->right):
    &(node->left);
}

slsBNode *sls_bnode_insert(slsBNode *node, slsBNode *child,
                           slsChildSelector dirrection) {
  if (!node || !child) {
    fprintf(stderr, "ERROR %s %i:\n\tnode is null", __FILE__, __LINE__);
    return NULL;
  }




  return child;
}

slsBNode *sls_bnode_insert_left(slsBNode *node, slsBNode *left) {
  slsBNode *new_node = sls_bnode_insert(node, left, SLS_CHILD_LEFT);
  
  return new_node;
}

slsBNode *sls_bnode_insert_right(slsBNode *node, slsBNode *right) {
  slsBNode *new_node = sls_bnode_insert(node, right, SLS_CHILD_RIGHT);

  return new_node;
}
