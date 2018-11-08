#ifndef BINARY_TREE_H
#define BINARY_TREE_H

/**
 * @brief Struct of a data in a binary tree node.
 *
 * #TODO - search for correct data for Symbol Table.
 *              name
 *              type1 (int, double, ...)
 *              type2 (constant, variable, procedure)
 *              number and types of arguments in procedures
 *
 * @param i asd.
 * @param c asd.
 */
typedef struct data {
    int i;
    char c;
} Tdata;

/**
 * @brief Struct of a binary tree node.
 *
 * @param left Pointer to the left child node.
 * @param right Pointer to the right child node.
 * @param data Data which the node contains.
 */
typedef struct node {
    struct node *left;
    struct node *right;
    Tdata data;
} Tnode;

/**
 * #TODO - orders alphabetically?
 *
 * Alocates memory for new binary tree node. Inserts data into the node and sets it's child nodes to NULL.
 *
 * @brief Creates new binary tree node.
 *
 * @param data Pointer to Tdata which are inserted into the node.
 * @return Pointer to the new node.
 */
Tnode *createNewNode (Tdata data);

/**
 * The function creates a new node and places it into the binary tree so it's ordered #TODO alphabetically?
 *
 *@brief Inserts a node in the binary tree.
 *
 * @param root Pointer to the binary tree root where the function inserts node.
 * @param data Data which function inserts into the node.
 * @return Pointer to the root node.
 */
Tnode *insertNode (Tnode *root, Tdata data);

/**
 * @brief Searches for the match of data in binary tree.
 *
 * @param root Pointer to the binary tree root where the function searches for the match.
 * @param data Data which the function searches for.
 * @return True if it founds the match, False if not.
 */
bool isInBinaryTree (Tnode *root, Tdata data);

/**
 * Function compares two integers and selects the greater one. If the integers are equal, the function chooses the first one.
 * Auxiliary function for binaryTreeHeight
 *
 * @brief Selects the greater integer.
 *
 * @param a First number to compare
 * @param b Second number to compare
 * @return The greater number.
 */
int maximum (int a, int b);

/**
 *@brief Determines the maximum height of a binary tree.
 *
 * @param root Pointer to the root of the binary tree.
 * @return Maximum height of the binary tree.
 */
int binaryTreeHeight (Tnode *root);

/**
 * The function creates new binary tree and copies the structure and data of the original binary tree in the new one.
 *
 * @brief Copies binary tree.
 *
 * @param origRoot Pointer to the root of the binary tree that is going to be copied.
 * @return Pointer to the root of the new copied binary tree.
 */
Tnode *copyBinaryTree (Tnode *origRoot);

/**
 * Deletes every node in the Binary Tree. The function traverses tree in post order. After the function, the root should be set to NULL.
 *
 *@brief Deletes the binary tree.
 *
 * @param root Pointer to the root of the Binary Tree which is going to be deleted.
 */
void deleteBinaryTree (Tnode *root);


#endif