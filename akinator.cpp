#include <stdio.h>
#include "binary_tree.h"

int BinTreeCompareElems(bin_tree_elem_t elem_a, bin_tree_elem_t elem_b);
BinTreeErrors RunAkinator(bin_tree_t* tree);
BinTreeErrors RunAkinatorNode(bin_tree_node_t* node);

int main()
{
    bin_tree_t tree = {};
    BinTreeInit(&tree, BinTreeCompareElems);
    BinTreeGetTreeFromFile(&tree, "akinator_db.txt");
    RunAkinator(&tree);
    BinTreeGraphDumpTree(&tree, "akinator_graph.txt");
}

BinTreeErrors RunAkinator(bin_tree_t* tree)
{
    if (tree == NULL) return BIN_TREE_ERR_NULL_POINTER;
    printf("Welcome to PEKUS akinator! Answer questions [y/n]\n");
    if (RunAkinatorNode(tree->root) != BIN_TREE_ERR_OK) return BIN_TREE_ERR_NODE_NULL_POINTER;

    return BIN_TREE_ERR_OK;
}

BinTreeErrors RunAkinatorNode(bin_tree_node_t* node)
{
    if (node == NULL) return BIN_TREE_ERR_NODE_NULL_POINTER;

    if (node->left == NULL && node->right == NULL) {printf("It is %s!\n", node->data); return BIN_TREE_ERR_OK;}

    printf("%s\n", node->data);
    char answer = 0;
    int num_of_syms = scanf(" %c", &answer);
    while (num_of_syms != 1 || !(answer == 'y' || answer == 'n'))
    {
        int ch = 0;
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Retry [y/n]!\n");
        num_of_syms = scanf(" %c", &answer);
    }

    BinTreeErrors err = BIN_TREE_ERR_OK;

    if (answer == 'y') err = RunAkinatorNode(node->left);
    else if (answer == 'n') err = RunAkinatorNode(node->right);
    else return BIN_TREE_ERR_DUPLICATE_ELEM;

    return err;

}








int BinTreeCompareElems(bin_tree_elem_t elem_a, bin_tree_elem_t elem_b)
{
    if (elem_a < elem_b) return -1;
    else if (elem_a > elem_b) return 1;
    else return 0;
}
