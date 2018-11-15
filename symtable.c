#include "symtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Tnode *createNewNode (Tdata data)
{
    Tnode *pNewNode = malloc(sizeof(Tnode));
    if (pNewNode == NULL) exit(99); // #TODO + delete binary tree?

    pNewNode->left = NULL;
    pNewNode->right = NULL;
    pNewNode->data = data;
    return pNewNode;
}

Tnode *insertNode (Tnode *root, Tdata data)
{
    if (root == NULL) // empty tree
        root = createNewNode(data);
    else if (root->data.i >= data.i) // left subtree
        root->left = insertNode(root->left, data);
    else // right subtree
        root->right = insertNode(root->right, data);
    return root;
}

bool isInBinaryTree (Tnode *root, Tdata data)
{
    if (root == NULL)
        return false;
    else if (root->data.i == data.i)
        return true;
    else if (root->data.i > data.i)
        isInBinaryTree(root->left, data);
    else
        isInBinaryTree(root->right, data);

    return false;

}

int maximum (int a, int b)
{
    if (a >= b)
        return a;
    else
        return b;
}

int binaryTreeHeight (Tnode *root)
{
    if (root == NULL) return 0;

    int maxHeight = maximum(binaryTreeHeight(root->left), binaryTreeHeight(root->right));
    return ++maxHeight;
}

Tnode *copyBinaryTree (Tnode *origRoot)
{
    if (origRoot == NULL) return NULL;

    Tnode *copyRoot = malloc(sizeof(Tnode));
    if (copyRoot == NULL) exit(99); // #TODO + delete binary tree?

    copyRoot->data = origRoot->data;
    copyRoot->left = copyBinaryTree(origRoot->left);
    copyRoot->right = copyBinaryTree(origRoot->right);

    return copyRoot;
}

void deleteBinaryTree (Tnode *root)
{
    if (root == NULL) return;

    deleteBinaryTree(root->left);
    deleteBinaryTree(root->right);
    free(root);
}
