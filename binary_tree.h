#include <stdio.h>
#include <stdlib.h>

#define PRINTING_SPEC "%d"
typedef int bin_tree_elem_t;

enum BinTreeErrors
{
    BIN_TREE_ERR_OK = 0,

    BIN_TREE_ERR_NULL_POINTER,
    BIN_TREE_ERR_NODE_NULL_POINTER,
    BIN_TREE_ERR_LEFT_NULL_POINTER,
    BIN_TREE_ERR_RIGHT_NULL_POINTER,
    BIN_TREE_ERR_PARENT_NULL_POINTER,

    BIN_TREE_ERR_LENGTH_TOO_BIG,

    BIN_TREE_ERR_NODE_CALLOC_ERROR,
    BIN_TREE_ERR_LEFT_CALLOC_ERROR,
    BIN_TREE_ERR_RIGHT_CALLOC_ERROR,

    BIN_TREE_ERR_GOT_POISON,
    BIN_TREE_ERR_FILE_NOT_OPENED,
    BIN_TREE_ERR_DUPLICATE_ELEM,
    BIN_TREE_ERR_WRONG_NUM_OF_NODES
};

struct bin_tree_node_t
{
    bin_tree_elem_t data;
    bin_tree_node_t* left;
    bin_tree_node_t* right;
    bin_tree_node_t* parent;
};

struct bin_tree_t
{
    bin_tree_node_t* root;
    unsigned int num_of_nodes;
    int (*bin_tree_comparator)(bin_tree_elem_t, bin_tree_elem_t);
};

const int MAX_CAPACITY = 50000;
const bin_tree_elem_t POISON = 0xBEDA;
const int LEN_OF_POINTER_NAME = 32;

BinTreeErrors BinTreeInit(bin_tree_t* tree, int (*comparator_func)(bin_tree_elem_t, bin_tree_elem_t));
BinTreeErrors BinTreeRemove(bin_tree_t* tree);
BinTreeErrors BinTreeVerify(bin_tree_t* tree);
BinTreeErrors BinTreeAddElem(bin_tree_t* tree, bin_tree_elem_t elem);
BinTreeErrors BinTreeLispDumpTree(bin_tree_t* tree, const char* lisp_filename);
BinTreeErrors BinTreeGraphDumpTree(bin_tree_t* tree, const char* dot_filename);

