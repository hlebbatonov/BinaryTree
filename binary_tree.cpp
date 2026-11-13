#include "binary_tree.h"

#define VERIFY_NULL_POINTER(pointer) do                    \
{                                                          \
    if (pointer == NULL) return BIN_TREE_ERR_NULL_POINTER; \
} while(0)

#define VERIFY_ERROR(err) do {if (err != BIN_TREE_ERR_OK) return err;} while(0)

#define $(x) #x

void          BinTreeRemoveSubtree(bin_tree_node_t* node);
BinTreeErrors BinTreeVerifySubtree(bin_tree_node_t* node, unsigned int* curr_num_of_elems);
BinTreeErrors BinTreeLispDumpNode(bin_tree_node_t* node, FILE* lisp_file);
BinTreeErrors BinTreeGraphDumpSubtree(bin_tree_node_t* node, FILE* html_file);
const char* BinTreePointerToString(bin_tree_node_t* pointer, char* buffer);


BinTreeErrors BinTreeInit(bin_tree_t* tree, int (*comparator_func)(bin_tree_elem_t, bin_tree_elem_t))
{
    VERIFY_NULL_POINTER(tree);
    VERIFY_NULL_POINTER(comparator_func);
    tree->root = NULL;
    tree->num_of_nodes = 0;
    tree->bin_tree_comparator = comparator_func;
    return BIN_TREE_ERR_OK;
}

BinTreeErrors BinTreeRemove(bin_tree_t* tree)
{
    VERIFY_NULL_POINTER(tree);

    BinTreeRemoveSubtree(tree->root);
    tree->num_of_nodes = 0;

    return BIN_TREE_ERR_OK;
}

void BinTreeRemoveSubtree(bin_tree_node_t* node)
{
    if (node == NULL) return ;

    BinTreeRemoveSubtree(node->left);
    BinTreeRemoveSubtree(node->right);

    node->data = POISON;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    free(node);
}

BinTreeErrors BinTreeAddElem(bin_tree_t* tree, bin_tree_elem_t elem)
{
    bin_tree_node_t* new_node = (bin_tree_node_t*)calloc(1, sizeof(bin_tree_node_t));
    if (new_node == NULL) return BIN_TREE_ERR_NODE_CALLOC_ERROR;

    new_node->data = elem;
    new_node->left = NULL;
    new_node->right = NULL;

    if (tree->root == NULL)
    {
        tree->root = new_node;
        tree->num_of_nodes = 1;
        return BIN_TREE_ERR_OK;
    }
    VERIFY_ERROR(BinTreeVerify(tree));

    bin_tree_node_t* curr_node = tree->root;
    bin_tree_node_t* parent_node = NULL;
    int cmp_res = 0;

    while (curr_node != NULL)
    {
        parent_node = curr_node;
        cmp_res = tree->bin_tree_comparator(elem, curr_node->data);

        if (cmp_res < 0) curr_node = curr_node->left;
        else if (cmp_res > 0) curr_node = curr_node->right;
        else {free(new_node); return BIN_TREE_ERR_DUPLICATE_ELEM;}
    }
    new_node->parent = parent_node;
    if (cmp_res < 0) parent_node->left = new_node;
    else if (cmp_res > 0) parent_node->right = new_node;
    tree->num_of_nodes++;
    return BIN_TREE_ERR_OK;
}

BinTreeErrors BinTreeVerify(bin_tree_t* tree)
{
    VERIFY_NULL_POINTER(tree);
    VERIFY_NULL_POINTER(tree->bin_tree_comparator);
    unsigned int curr_num_of_elems = 0;
    VERIFY_ERROR(BinTreeVerifySubtree(tree->root, &curr_num_of_elems));
    if (curr_num_of_elems != tree->num_of_nodes) return BIN_TREE_ERR_WRONG_NUM_OF_NODES;
    return BIN_TREE_ERR_OK;
}

BinTreeErrors BinTreeVerifySubtree(bin_tree_node_t* node, unsigned int* curr_num_of_elems)
{
    if (node == NULL) return BIN_TREE_ERR_OK;
    (*curr_num_of_elems)++;
    if (node->data == POISON) return BIN_TREE_ERR_GOT_POISON;
    if (*curr_num_of_elems > MAX_CAPACITY) return BIN_TREE_ERR_LENGTH_TOO_BIG;
    VERIFY_ERROR(BinTreeVerifySubtree(node->left, curr_num_of_elems));
    VERIFY_ERROR(BinTreeVerifySubtree(node->right, curr_num_of_elems));
    return BIN_TREE_ERR_OK;
}

//SECTION - Dump functions

BinTreeErrors BinTreeLispDumpTree(bin_tree_t* tree, const char* lisp_filename)
{
    if (tree == NULL)
    {
        fprintf(stderr, "Error: got NULL tree pointer\n");
        return BIN_TREE_ERR_NULL_POINTER;
    }

    if (lisp_filename == NULL)
    {
        fprintf(stderr, "Error: program filename crashed\n");
        return BIN_TREE_ERR_FILE_NOT_OPENED;
    }

    FILE* lisp_file = fopen(lisp_filename, "w");
    if (lisp_file == NULL)
    {
        fprintf(stderr, "Error: no such file <%s>\n", lisp_filename);
        return BIN_TREE_ERR_FILE_NOT_OPENED;
    }

    BinTreeLispDumpNode(tree->root, lisp_file);

    fclose(lisp_file);
    return BIN_TREE_ERR_OK;
}

BinTreeErrors BinTreeLispDumpNode(bin_tree_node_t* node, FILE* lisp_file)
{
    if (lisp_file == NULL) return BIN_TREE_ERR_FILE_NOT_OPENED;

    fprintf(lisp_file, "(");
    if (node == NULL) {fprintf(lisp_file, ")"); return BIN_TREE_ERR_OK;}

    fprintf(lisp_file, PRINTING_SPEC, node->data);
    BinTreeLispDumpNode(node->left, lisp_file);
    BinTreeLispDumpNode(node->right, lisp_file);

    fprintf(lisp_file, ")");
    return BIN_TREE_ERR_OK;
}

BinTreeErrors BinTreeGraphDumpTree(bin_tree_t* tree, const char* dot_filename)
{
    if (tree == NULL) {fprintf(stderr, "Error: got NULL tree pointer\n"); return BIN_TREE_ERR_NULL_POINTER;}

    if (dot_filename == NULL)
    {
        fprintf(stderr, "Error: program filename crashed\n");
        return BIN_TREE_ERR_FILE_NOT_OPENED;
    }

    FILE* dot_file = fopen(dot_filename, "w");
    if (dot_file == NULL)
    {
        fprintf(stderr, "Error: no such file <%s>\n", dot_filename);
        return BIN_TREE_ERR_FILE_NOT_OPENED;
    }

    //Head
    fprintf(dot_file, "digraph Tree_Dump\n{\n");
    fprintf(dot_file, "rankdir=TB;\n");
    fprintf(dot_file, "splines=true;\n");
    fprintf(dot_file, "nodesep=1.0;\n");
    fprintf(dot_file, "ranksep=0.6;\n");
    fprintf(dot_file, "node [shape=record, style=\"filled,rounded\", fontname=\"Arial\", group=1];\n\n");

    BinTreeGraphDumpSubtree(tree->root, dot_file);

    fprintf(dot_file, "\n");
    fprintf(dot_file, "\n}");
    fclose(dot_file);
    return BIN_TREE_ERR_OK;
}

BinTreeErrors BinTreeGraphDumpSubtree(bin_tree_node_t* node, FILE* dot_file)
{
    if (dot_file == NULL) {fprintf(stderr, "Error: no dot file\n"); return BIN_TREE_ERR_FILE_NOT_OPENED;}
    if (node == NULL) return BIN_TREE_ERR_OK;

    char left_str[LEN_OF_POINTER_NAME]   = {};
    char right_str[LEN_OF_POINTER_NAME]  = {};
    char parent_str[LEN_OF_POINTER_NAME] = {};

    if (node->left != NULL || node->right != NULL)
    {
        fprintf(dot_file, "node_%p [label=\"{data: %d | pointer: %p| {left: %s | right: %s} | parent: %s}\", \
        fillcolor=\"#E3F2FD\"];\n", (void*)node, node->data, (void*)node, BinTreePointerToString(node->left, left_str),
        BinTreePointerToString(node->right, right_str), BinTreePointerToString(node->parent, parent_str));

        if (node->left != NULL)
        {
            fprintf(dot_file, "node_%p -> node_%p [color=\"#4CAF50\", label=\"L\"];\n",
                (void*)node, (void*)node->left);
            BinTreeGraphDumpSubtree(node->left, dot_file);
        }
        else
        {
            fprintf(dot_file, "null_left_%p [style=invis];\n", (void*)node);
            fprintf(dot_file, "node_%p -> null_left_%p [style=invis];\n", (void*)node, (void*)node);
        }

        if (node->right != NULL)
        {
            fprintf(dot_file, "node_%p -> node_%p [color=\"#F44336\", label=\"R\"];\n",
                (void*)node, (void*)node->right);
            BinTreeGraphDumpSubtree(node->right, dot_file);
        }
        else
        {
            fprintf(dot_file, "null_right_%p [style=invis];\n", (void*)node);
            fprintf(dot_file, "node_%p -> null_right_%p [style=invis];\n", (void*)node, (void*)node);
        }
    }
    else
        fprintf(dot_file, "node_%p [label=\"{data: %d | pointer: %p| {left: %s | right: %s} | parent: %s}\", \
        fillcolor=\"#e0d6f1\"];\n", (void*)node, node->data, (void*)node, BinTreePointerToString(node->left, left_str),
        BinTreePointerToString(node->right, right_str), BinTreePointerToString(node->parent, parent_str));

    return BIN_TREE_ERR_OK;
}

const char* BinTreePointerToString(bin_tree_node_t* pointer, char* buffer)
{
    if (buffer == NULL) return NULL;

    if (pointer == NULL)
        return "NULL";

    snprintf(buffer, LEN_OF_POINTER_NAME, "%p", (void*)pointer);
    return buffer;
}

//!SECTION
/*
BinTreeErrors BinTreeGetTreeFromFile(bin_tree_elem_t* tree, const char* lisp_filename)
{
    if (tree == NULL)
    {
        fprintf(stderr, "Error: got NULL tree pointer\n");
        return BIN_TREE_ERR_NULL_POINTER;
    }

    if (lisp_filename == NULL)
    {
        fprintf(stderr, "Error: program filename crashed\n");
        return BIN_TREE_ERR_FILE_NOT_OPENED;
    }

    FILE* lisp_file = fopen(lisp_filename, "r");
    if (lisp_file == NULL)
    {
        fprintf(stderr, "Error: no such file <%s>\n", lisp_filename);
        return BIN_TREE_ERR_FILE_NOT_OPENED;
    }

    BinTreeLispGotNodeFromFile(tree->root, lisp_file);

    fclose(lisp_file);
    return BIN_TREE_ERR_OK;

}
*/
