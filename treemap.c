#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap* map = (TreeMap*) malloc(sizeof(TreeMap));
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while (current != NULL) {
        if (tree->lower_than(current->pair->key, key)) {
            parent = current;
            current = current->right;
        } else if (tree->lower_than(key, current->pair->key)) {
            parent = current;
            current = current->left;
        } else {
            return;
        }
    }

    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));
    newNode->pair = (Pair*) malloc(sizeof(Pair));
    newNode->pair->key = key;
    newNode->pair->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    if (parent == NULL) {
        tree->root = newNode;
    } else if (tree->lower_than(key, parent->pair->key)) {
        parent->left = newNode;
        newNode->parent = parent;
    } else {
        parent->right = newNode;
        newNode->parent = parent;
    }

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        if (node == tree->root) {
            tree->root = NULL;
        } else {
            if (node == node->parent->left) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = node->left ? node->left : node->right;
        if (node == tree->root) {
            tree->root = child;
            child->parent = NULL;
        } else {
            if (node == node->parent->left) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        }
        free(node);
        return;
    }

    TreeNode* minimum_node = minimum(node->right);
    node->pair = minimum_node->pair;
    removeNode(tree, minimum_node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
      TreeNode* current = tree->root;

   while (current != NULL) {
        if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            tree->current = current;
            return current->pair;
              }
    }
    return NULL;

}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
   if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * node = tree->root;
    while (node->left != NULL) {
        node = node->left;
    }
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
 if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode * current = tree->current;
    TreeNode * next = NULL;

    if (current->right != NULL) {
        next = minimum(current->right);
    }
    else {
        TreeNode * parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        next = parent;
    }

    tree->current = next;
    return (next != NULL) ? next->pair : NULL;
}
