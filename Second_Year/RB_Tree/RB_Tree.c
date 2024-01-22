#include "RB_Tree.h"

/////////////////////////////////////API///////////////////////////////////////
int CreateTree(RB_Tree** pTree)
{
	*pTree = MALLOC(sizeof(RB_Tree));
	if (*pTree == NULL)
		return ENOMEM;

	RB_Tree* tree = *pTree;

	tree->nil_ = CreateNode(tree, 0);
	if (tree->nil_ == NULL){
		free(tree);
		return ENOMEM;
	}

	tree->nil_->color_ = BLACK;
	tree->numNodes_ = 1;
    tree->root_ = tree->nil_;

	return NOERRORS;
}

int InsertNode(RB_Tree* tree, T data)
{
	if (tree == NULL)
		return BAD_ARGS;

	RB_Node *freeNode = NULL;
	FindNode(tree, data, &freeNode);

	if (freeNode != NULL)
		return 0;

	RB_Node *newNode = CreateNode(tree, data);
	if (newNode == NULL)
		return ENOMEM;

	if (tree->root_ != tree->nil_)
		ConnectNode(tree, newNode, tree->root_);

	InsertCase1(newNode);

	tree->root_ = GetRoot(newNode);

	tree->numNodes_++;

	return 0;
}

int DeleteNode(RB_Tree* tree, RB_Node *deleteNode)
{
	if (tree == NULL || deleteNode == NULL)
		return BAD_ARGS;

	RB_Node *x, *y;
	
	if (!deleteNode || deleteNode == tree->nil_)
		return NOERRORS;

	if (deleteNode->links_[LEFT] == tree->nil_ || deleteNode->links_[RIGHT] == tree->nil_)
		y = deleteNode;
	else {
		y = deleteNode->links_[RIGHT];
		while (y->links_[LEFT] != tree->nil_)
			y = y->links_[LEFT];
	}

	if (y->links_[LEFT] != tree->nil_)
		x = y->links_[LEFT];
	else
		x = y->links_[LEFT];

	x->links_[PARENT] = y->links_[PARENT];
	if (y->links_[PARENT])
		if (y == y->links_[PARENT]->links_[LEFT])
			y->links_[PARENT]->links_[LEFT] = x;
		else
			y->links_[PARENT]->links_[RIGHT] = x;
	else
		tree->root_ = x;

	if (y != deleteNode)
		deleteNode->data_ = y->data_;

	if (y->color_ == BLACK)
		CheckDelete(tree, x);

	free(y);
	tree->numNodes_--;
	y = NULL;

	return NOERRORS;
}

int DumpTree(const RB_Tree* const tree, const char* const outputFileName)
{
	if (tree == NULL || outputFileName == NULL)
		return BAD_ARGS;


	FILE *file_dot = fopen(outputFileName, "wb");

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color_ = \"#000000\", fontsize = 20];\n"
                      "\tedge[color_ = \"#000000\", fontsize = 20];\n");

    PrintTree(tree, tree->root_, file_dot);

    fprintf(file_dot, "}");
    
    fclose(file_dot);

	system("dot -Tjpg -o Tree.jpg RB_Tree.dot");

	return NOERRORS;
}

int foreach(RB_Tree* tree, RB_Node* node,
			int(*function)(RB_Tree*, RB_Node*, void*), void* data)
{
	if (tree == NULL || node == NULL || function == NULL || data == NULL)
		return BAD_ARGS;

	if (node->links_[LEFT] != tree->nil_)
		foreach(tree, node->links_[LEFT], function, data);

	if (node->links_[RIGHT] != tree->nil_)
		foreach(tree, node->links_[RIGHT], function, data);

	return NOERRORS;
}

int DestroyTree(RB_Tree* tree)
{
	if (tree == NULL)
		return BAD_ARGS;

	if (tree->root_ != NULL && tree->root_ != tree->nil_)
		DestroySubTree(tree, tree->root_);

    if (tree->nil_ != NULL)
        free(tree->nil_);

    free(tree);

	return NOERRORS;
}

//////////////////////////////Secondary functions//////////////////////////////
RB_Node* CreateNode(RB_Tree* tree, T data)
{
	RB_Node *node = MALLOC(sizeof(RB_Node));
	if (node == NULL)
		return NULL;

	node->links_[PARENT] = NULL;
	node->links_[LEFT] = tree->nil_;
	node->links_[RIGHT] = tree->nil_;

	node->color_ = RED;
	node->data_ = data;

	return node;
}

int ConnectNode(RB_Tree* tree, RB_Node* newNode, RB_Node* node)
{
	if (node->data_ > newNode->data_) {
		if (node->links_[LEFT] == tree->nil_) {
			newNode->links_[PARENT] = node;
			node->links_[LEFT] = newNode;
		}
		else
			return ConnectNode(tree, newNode, node->links_[LEFT]);
	}
	else {
		if (node->links_[RIGHT] == tree->nil_) {
			newNode->links_[PARENT] = node;
			node->links_[RIGHT] = newNode;
		}
		else
			return ConnectNode(tree, newNode, node->links_[RIGHT]);
	}

	return NOERRORS;
}

void InsertCase1(RB_Node* node)
{
	if (node->links_[PARENT] == NULL)
		node->color_ = BLACK;
	else
		InsertCase2(node);
}

void InsertCase2(RB_Node* node)
{
	if (node->links_[PARENT]->color_ == BLACK)
		return;
	else
		InsertCase3(node);
}

void InsertCase3(RB_Node* node)
{
	RB_Node *uncle = GetUncle(node);

	if (uncle != NULL && uncle->color_ == RED)
	{
		node->links_[PARENT]->color_ = BLACK;
		uncle->color_ = BLACK;

		RB_Node *grandParent = GetGrandparent(node);
		grandParent->color_ = RED;

		InsertCase1(grandParent);
	}
	else
		InsertCase4(node);
}

void InsertCase4(RB_Node* node)
{
	RB_Node *grandParent = GetGrandparent(node);

	if (node == node->links_[PARENT]->links_[RIGHT] &&
	 	node->links_[PARENT] == grandParent->links_[LEFT]){
		LeftRotate(node->links_[PARENT]);
		node = node->links_[LEFT];
	}
	else if (node->links_[PARENT] == grandParent->links_[RIGHT] && 
			 node == node->links_[PARENT]->links_[LEFT])
	{
		RightRotate(node->links_[PARENT]);
		node = node->links_[RIGHT];
	}

	InsertCase5(node);
}

void InsertCase5(RB_Node* node)
{
	RB_Node *grandParent = GetGrandparent(node);

	node->links_[PARENT]->color_ = BLACK;
	grandParent->color_ = RED;

	if (node == node->links_[PARENT]->links_[LEFT] && 
		node->links_[PARENT] == grandParent->links_[LEFT])
		RightRotate(grandParent);
	else
		LeftRotate(grandParent);
}

void LeftRotate(RB_Node* node)
{
	RB_Node* temp = node->links_[RIGHT];

	temp->links_[PARENT] = node->links_[PARENT];
	if (node->links_[PARENT] != NULL) {
		if (node->links_[PARENT]->links_[LEFT] == node)
			node->links_[PARENT]->links_[LEFT] = temp;
		else
			node->links_[PARENT]->links_[RIGHT] = temp;
	}

	node->links_[RIGHT] = temp->links_[LEFT];
	if (temp->links_[LEFT] != NULL)
		temp->links_[LEFT]->links_[PARENT] = node;

	node->links_[PARENT] = temp;
	temp->links_[LEFT] = node;
}

void RightRotate(RB_Node* node)
{
	RB_Node* temp = node->links_[LEFT];

	temp->links_[PARENT] = node->links_[PARENT];
	if (node->links_[PARENT] != NULL) {
		if (node->links_[PARENT]->links_[LEFT] == node)
			node->links_[PARENT]->links_[LEFT] = temp;
		else
			node->links_[PARENT]->links_[RIGHT] = temp;
	}

	node->links_[LEFT] = temp->links_[RIGHT];
	if (temp->links_[RIGHT] != NULL)
		temp->links_[RIGHT]->links_[PARENT] = node;

	node->links_[PARENT] = temp;
	temp->links_[RIGHT] = node;
}

void CheckDelete(RB_Tree* tree, RB_Node* checkNode)
{
	while (checkNode != tree->root_ && checkNode->color_ == BLACK)
	{
		if (checkNode == checkNode->links_[PARENT]->links_[LEFT])
		{
			RB_Node *brother = checkNode->links_[PARENT]->links_[RIGHT];
			if (brother->color_ == RED) {
				brother->color_ = BLACK;
				checkNode->links_[PARENT]->color_ = RED;
				LeftRotate(checkNode->links_[PARENT]);
				brother = checkNode->links_[PARENT]->links_[RIGHT];
			}
			if (brother->links_[LEFT]->color_ == BLACK && 
				brother->links_[RIGHT]->color_ == BLACK) {
				brother->color_ = RED;
				checkNode = checkNode->links_[PARENT];
			}
			else {
				if (brother->links_[RIGHT]->color_ == BLACK) {
					brother->links_[LEFT]->color_ = BLACK;
					brother->color_ = RED;
					RightRotate(brother);
					brother = checkNode->links_[PARENT]->links_[RIGHT];
				}
				brother->color_ = checkNode->links_[PARENT]->color_;
				checkNode->links_[PARENT]->color_ = BLACK;
				brother->links_[RIGHT]->color_ = BLACK;
				LeftRotate(checkNode->links_[PARENT]);
				checkNode = tree->root_;
			}
		}
		else {
			RB_Node *brother = checkNode->links_[PARENT]->links_[LEFT];
			if (brother->color_ == RED)
			{
				brother->color_ = BLACK;
				checkNode->links_[PARENT]->color_ = RED;
				RightRotate(checkNode->links_[PARENT]);
				brother = checkNode->links_[PARENT]->links_[LEFT];
			}
			if (brother->links_[RIGHT]->color_ == BLACK && 
				brother->links_[LEFT]->color_ == BLACK) {
				brother->color_ = RED;
				checkNode = checkNode->links_[PARENT];
			}
			else
			{
				if (brother->links_[LEFT]->color_ == BLACK) {
					brother->links_[RIGHT]->color_ = BLACK;
					brother->color_ = RED;
					LeftRotate(brother);
					brother = checkNode->links_[PARENT]->links_[LEFT];
				}

				brother->color_ = checkNode->links_[PARENT]->color_;
				checkNode->links_[PARENT]->color_ = BLACK;
				brother->links_[LEFT]->color_ = BLACK;
				RightRotate(checkNode->links_[PARENT]);
				checkNode = tree->root_;
			}
		}
	}
	checkNode->color_ = BLACK;
}

void PrintTree(const RB_Tree* const tree, const RB_Node* const node, FILE* OutputDot)
{
    fprintf(OutputDot, "\n\t\t\"key_%d\" [label = \"%d\", ", node->data_, node->data_);
    if (node->color_ == RED) 
        fprintf(OutputDot, "color = \"#FF0000\", fontcolor = \"#FFFFFF\" "
                         "style = \"filled\", fillcolor = \"#FF0000\"]\n");
    else 
        fprintf(OutputDot, "color = \"#000000\", fontcolor = \"#FFFFFF\" "
                         "style = \"filled\", fillcolor = \"#000000\"]\n");

    if (node->links_[LEFT] != tree->nil_) {
        fprintf(OutputDot, "\t\t\"key_%d\" -> \"key_%d\";\n", node->data_, node->links_[LEFT]->data_);
        PrintTree(tree, node->links_[LEFT], OutputDot);
        fprintf(OutputDot, "\t\t{\n\t\t\trank = same;\n"
                         "\t\t\t\"key_%d\" -> ", node->links_[LEFT]->data_);
    } 
    else {
		fprintf(OutputDot,
				"\n\t\t\"nil_left_key_%d\" [label = \"nil\", shape = \"diamond\", "
				"style = \"filled\", fillcolor = \"#000000\" color = \"#FFFFFF\", " 
				"fontcolor = \"#000000\"];\n",
				node->data_);
		fprintf(OutputDot, "\t\t\"key_%d\" -> \"nil_left_key_%d\";\n", node->data_, node->data_);
        fprintf(OutputDot, "\t\t{\n\t\t\trank = same;\n"
                         "\t\t\t\"nil_left_key_%d\" -> ", node->data_);
    }

    if (node->links_[RIGHT] != tree->nil_) {
		fprintf(OutputDot, "\"key_%d\" [color=invis];\n\t\t}\n", node->links_[RIGHT]->data_);
		fprintf(OutputDot, "\t\t\"key_%d\" -> \"key_%d\";\n", node->data_, node->links_[RIGHT]->data_);
		PrintTree(tree, node->links_[RIGHT], OutputDot);
	}
    else {
        fprintf(OutputDot, "\"nil_right_key_%d\" [color=invis];\n\t\t}\n", node->data_);
		fprintf(OutputDot,
				"\n\t\t\"nil_right_key_%d\" [label = \"nil\", shape = \"diamond\", "
				"style = \"filled\", fillcolor = \"#000000\" color = \"#FFFFFF\", "
				"fontcolor = \"#000000\"];\n",
				node->data_);
		fprintf(OutputDot, "\t\t\"key_%d\" -> \"nil_right_key_%d\";\n", node->data_, node->data_);
    } 
}

void DestroySubTree(RB_Tree* tree, RB_Node* node)
{
	if (node->links_[LEFT] != tree->nil_ && node->links_[LEFT] != NULL)
		DestroySubTree(tree, node->links_[LEFT]);

	if (node->links_[RIGHT] != tree->nil_ && node->links_[RIGHT] != NULL)
		DestroySubTree(tree, node->links_[RIGHT]);

    tree->numNodes_--;
    free(node);
	node = NULL;
}

void FindNode(RB_Tree *tree, T data, RB_Node **rbNode)
{
	*rbNode = tree->root_;

	while (*rbNode != tree->nil_)
	{
		if ((*rbNode)->data_ > data)
			*rbNode = (*rbNode)->links_[LEFT];
		else if ((*rbNode)->data_ < data)
			*rbNode = (*rbNode)->links_[RIGHT];
		else
			return;
	}

	*rbNode = NULL;
}

RB_Node *GetRoot(const RB_Node* const node)
{
	RB_Node *root = (RB_Node *)node;
	while (root->links_[PARENT] != NULL)
		root = root->links_[PARENT];

	return root;
}

void* MyMalloc(size_t size)
{
	static int count = 0;
	count++;

	if (count % 50 == 0) //Just because
		return NULL;

	return malloc(size);
}

RB_Node* GetGrandparent(const RB_Node* node)
{
		return node->links_[PARENT]->links_[PARENT];	
}

RB_Node* GetUncle(const RB_Node* node)
{
	RB_Node* grandParent = GetGrandparent(node);

	if (node->links_[PARENT] == grandParent->links_[LEFT])
		return grandParent->links_[RIGHT];
	else
		return grandParent->links_[LEFT];
}
