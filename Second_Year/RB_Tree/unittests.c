#include "RB_Tree.h"

#include <stdio.h>
#include "time.h"
#include "limits.h"

void Test_InvMem();
void Test_Insert();
void Test_foreach();
void Test_BadArgs();
void Test_Delete();
void Test_Random();


int main(){

    Test_InvMem();
    Test_BadArgs();
    Test_foreach();
    Test_Insert();
    Test_Delete();
    Test_Random();

    return 0;
}

void Test_InvMem()
{
    int isError = 0;
    RB_Tree* tree = NULL;
    for (size_t i = 1; i < 26; i++){
        if (CreateTree(&tree) == ENOMEM){
            isError = 1;
        }
        else 
            DestroyTree(tree);
    tree = NULL;
    }
    if (isError)
        fprintf(stderr, "Test_InvMem success\n");
    else
        fprintf(stderr, "Test_InvMem failed\n");
}

void Test_BadArgs()
{
    if (InsertNode(NULL, 1) != BAD_ARGS || DeleteNode(NULL, NULL) != BAD_ARGS ||
        DumpTree(NULL, NULL) != BAD_ARGS || DestroyTree(NULL) != BAD_ARGS ||
        foreach (NULL, NULL, NULL, NULL) != BAD_ARGS)
        fprintf(stderr, "Test_BadArgs failed!");

    fprintf(stderr, "Test_BadArgs success\n");
}

int sum(RB_Tree* tree, RB_Node *node, void *data)
{
    *((int*)data) += node->data_;
    return *((int*)data);
}

void Test_foreach()
{
    RB_Tree* tree = NULL;
    CreateTree(&tree);
    for (size_t i = 1; i < 10; i++)
        InsertNode(tree, i);

    int tree_sum = 0;

    foreach(tree, tree->root_, &sum, &tree_sum);

    DestroyTree(tree);

    fprintf(stderr, "Test_foreach success\n");
}

void Test_Delete()
{
    RB_Tree* tree = NULL;
    CreateTree(&tree);
    for (size_t i = 1; i <= 100; i++)
        InsertNode(tree, i);
    
    for (size_t i = 1; i < 35; i++){
        if (i % 4 == 0)
            DeleteNode(tree, tree->root_->links_[LEFT]);
        if (i % 3 == 0)
            DeleteNode(tree, tree->root_->links_[RIGHT]);
        if (i % 5 == 0)
            DeleteNode(tree, tree->root_->links_[RIGHT]);
        
    }

    DestroyTree(tree);

    fprintf(stderr, "Test_Delete success\n");
}

void Test_Insert()
{
    RB_Tree* tree = NULL;
    CreateTree(&tree);

    for (size_t i = 50; i <= 99; i++)
        InsertNode(tree, i);
    
    InsertNode(tree, 50);

    for (size_t i = 1; i <= 49; i++)
        InsertNode(tree, i);
    
    DumpTree(tree, "RB_Tree.dot");

    DestroyTree(tree);

    fprintf(stderr, "Test_Insert success\n");
}

void Test_Random()
{
    RB_Tree* tree = NULL;
    CreateTree(&tree);
    const int BIG_NUMBER = INT_MAX / 10;
    const int SMALL_NUMBER = INT_MIN / 10;
    const int STEP = 10;

    int baseNumber = rand() % (BIG_NUMBER + 1 - SMALL_NUMBER) + SMALL_NUMBER;

    InsertNode(tree, baseNumber);
    InsertNode(tree, baseNumber + STEP * 2);
    InsertNode(tree, baseNumber + STEP * 1);
    InsertNode(tree, baseNumber + STEP * 3);
    InsertNode(tree, baseNumber - STEP * 2);
    InsertNode(tree, baseNumber - STEP * 1);
    InsertNode(tree, baseNumber + STEP * 4);
    InsertNode(tree, baseNumber + STEP * 5);
    InsertNode(tree, baseNumber + STEP * 6);
    InsertNode(tree, baseNumber + STEP * 6);

    DestroyTree(tree);

    fprintf(stderr, "Test_Random success\n");
}