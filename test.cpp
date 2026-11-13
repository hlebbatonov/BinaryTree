#include "binary_tree.h"

int BinTreeCompareElems(bin_tree_elem_t elem_a, bin_tree_elem_t elem_b);

int main()
{
    bin_tree_t tree = {};
    BinTreeErrors err = BinTreeInit(&tree, BinTreeCompareElems);
    BinTreeAddElem(&tree, 50);
    BinTreeAddElem(&tree, 40);
    BinTreeAddElem(&tree, 60);
    BinTreeAddElem(&tree, 20);
    BinTreeAddElem(&tree, 35);
    BinTreeAddElem(&tree, 67);
    BinTreeAddElem(&tree, 52);
    BinTreeAddElem(&tree, 100);
    BinTreeAddElem(&tree, 80);
    BinTreeAddElem(&tree, 37);
    BinTreeAddElem(&tree, 25);
    BinTreeAddElem(&tree, 29);

    for (int i = 0; i < 1500; i++)
    {
        int random_val = rand() % 100000 + 1;
        BinTreeAddElem(&tree, random_val);
    }

    BinTreeLispDumpTree(&tree, "1.txt");
    BinTreeGraphDumpTree(&tree, "2.txt");
    BinTreeRemove(&tree);
}

int BinTreeCompareElems(bin_tree_elem_t elem_a, bin_tree_elem_t elem_b)
{
    if (elem_a < elem_b) return -1;
    else if (elem_a > elem_b) return 1;
    else return 0;
}
