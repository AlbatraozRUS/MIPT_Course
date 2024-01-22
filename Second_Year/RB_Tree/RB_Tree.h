#include <stdlib.h>
#include <stdio.h>

#pragma once

typedef int T;

#define TEST
#ifdef TEST
#define MALLOC(x) MyMalloc(x)
#else
#define MALLOC(x) malloc(x);
#endif

enum COLOR {
	BLACK,
	RED
};

enum LINKS {
	LEFT,
	RIGHT,
	PARENT
};

enum ERRORS {
	NOERRORS =  0,
	BAD_ARGS = -1,
	ENOMEM = -2,
};

typedef struct RB_Node {
	int data_;

	struct RB_Node* links_[3];	

	int color_;

} RB_Node;

typedef struct RB_Tree {
	RB_Node* root_;
	RB_Node* nil_;
	size_t numNodes_;
} RB_Tree;

/////////////////////////////////////API///////////////////////////////////////
int CreateTree(RB_Tree** pTree);
int InsertNode(RB_Tree* tree, T data);
int DeleteNode(RB_Tree* tree, RB_Node *deleteNode);
int DumpTree(const RB_Tree* const tree, const char* const outputFileName);
int DestroyTree(RB_Tree* tree);
int foreach(RB_Tree* tree, RB_Node* node, 
			int(*function)(RB_Tree*, RB_Node*, void*), void* data);

//////////////////////////////Secondary functions//////////////////////////////
RB_Node* CreateNode(RB_Tree* tree, T data);
void FindNode(RB_Tree *tree, T data, struct RB_Node **rbNode);
int ConnectNode(RB_Tree* tree, RB_Node* newNode, RB_Node* node);

RB_Node *GetGrandparent(const RB_Node *node);
RB_Node *GetUncle(const RB_Node *node);

void LeftRotate(RB_Node *node);
void RightRotate(RB_Node *node);

void InsertCase1(RB_Node *node);
void InsertCase2(RB_Node *node);
void InsertCase3(RB_Node *node);
void InsertCase4(RB_Node *node);
void InsertCase5(RB_Node *node);

void CheckDelete(RB_Tree* tree, RB_Node* checkNode);
void PrintTree(const RB_Tree* const tree, const RB_Node* const node, FILE *dot_out);
RB_Node* GetRoot(const RB_Node* const node);
void* MyMalloc(size_t size);
void DestroySubTree(RB_Tree *tree, RB_Node *node);
