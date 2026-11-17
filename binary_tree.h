#include <stdlib.h>

//#define BIN_TREE_TYPE_INT
#define BIN_TREE_TYPE_STR

#if defined(BIN_TREE_TYPE_INT)
    typedef int bin_tree_elem_t;
    #define BIN_TREE_ELEM_SPEC "%d"
    const bin_tree_elem_t POISON = 0xBEDA;
#elif defined(BIN_TREE_TYPE_STR)
    typedef char* bin_tree_elem_t;
    #define BIN_TREE_ELEM_SPEC "%s"
    const bin_tree_elem_t POISON = NULL;
#else
    #error "Define BIN_TREE_TYPE_INT or BIN_TREE_TYPE_STR"
#endif

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
    BIN_TREE_ERR_WRONG_NUM_OF_NODES,
    ERR_CANT_GET_SIZE_OF_FILE,
    ERR_CANT_GET_INFO_OF_FILE,
    BIN_TREE_SYNTAX_ERR
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

const int LEN_OF_POINTER_NAME = 32;

BinTreeErrors BinTreeInit(bin_tree_t* tree, int (*comparator_func)(bin_tree_elem_t, bin_tree_elem_t));
BinTreeErrors BinTreeRemove(bin_tree_t* tree);
BinTreeErrors BinTreeVerify(bin_tree_t* tree);
BinTreeErrors BinTreeAddElem(bin_tree_t* tree, bin_tree_elem_t elem);
BinTreeErrors BinTreeLispDumpTree(bin_tree_t* tree, const char* lisp_filename);
BinTreeErrors BinTreeGraphDumpTree(bin_tree_t* tree, const char* dot_filename);
BinTreeErrors BinTreeGetTreeFromFile(bin_tree_t* tree, const char* lisp_filename);

