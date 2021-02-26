/****************************************************************************************
Name: BST.h
Description: This file contains the function definitions of a binary search tree. The
				functions will be used to interact with the application written by
				Coach Ragsdale, Wordcnt.cpp.
Author: Caleb Manor
****************************************************************************************/

#include <fstream>
using namespace std;

#define BSTLength 30
typedef char BSTtype[BSTLength];

struct Node {
	BSTtype info;
	Node * left = nullptr;
	Node * right = nullptr;
	int frequency = 1;
};

class BST {
public:
	BST();
	~BST();
	bool BSTadd(BSTtype);
	bool BSTdelete(BSTtype);
	void BSTprint(ofstream &);
	void BSTprint(char, ostream &);
	int getBSTHeight();
	int getBSTNodeCount();
	bool BSTexists(BSTtype);
	bool BSTinc(BSTtype);

private:
	void deleteLeaf(Node *, Node *);
	void deleteOneChild(Node *, Node *);
	void deleteTwoChild(Node *, Node *);
	void PostOrderDestroy(Node *);
	void InOrderPrint(Node *, ostream&);
	void PartialPrint(char, Node *, ostream &);
	int HeightCalculator(Node *);

	Node * root;
	int height = 0;
	int nodeCount = 0;
};