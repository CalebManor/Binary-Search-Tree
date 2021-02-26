/****************************************************************************************
Name: BST.cpp
Description: This file is the implementation of the functions defined in BST.h. The
				functions are used to implement a binary search tree using data gathered
				from the application Wordcnt.cpp. 
Author: Caleb Manor
****************************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "BST.h"

/****************************************************************************************
CONSTRUCTOR
****************************************************************************************/
BST::BST() : root(nullptr)
{}

/****************************************************************************************
DESTRUCTOR
****************************************************************************************/
BST::~BST() {

	PostOrderDestroy(root);
	root = nullptr;

}

/****************************************************************************************
Function: PostOrderDestroy
Description: This function is called by the destructor and uses a post order traversal
				to destroy the entire tree. 
****************************************************************************************/
void BST::PostOrderDestroy(Node * current) {

	if (current != nullptr) {
		PostOrderDestroy(current->left);
		PostOrderDestroy(current->right);
		delete current;
	}

}

/****************************************************************************************
Function: BSTadd
Description: This function will be used to add a word to the binary search tree.
****************************************************************************************/
bool BST::BSTadd(BSTtype word) {

	Node * leaf = new Node;

	if (leaf == nullptr)
		return false;

	strcpy_s(leaf->info, word);
	leaf->left = leaf->right = nullptr;

	if (root == nullptr) {
		root = leaf;
		nodeCount++;
		return true;
	}

	else {

		Node * current = root;
		Node * parent = root;

		while (current != nullptr) {
			parent = current;

			if (strcmp(leaf->info, current->info) < 0)
				current = current->left;

			else
				current = current->right;
		}

		if (strcmp(leaf->info, parent->info) < 0)
			parent->left = leaf;

		else if (strcmp(leaf->info, parent->info) > 0)
			parent->right = leaf; 
	}

	nodeCount++;
	return true;

}

/****************************************************************************************
Function: BSTdelete
Decription: This function will be used to delete an item from the binary search tree. 
				the logic covers cases where the item to be deleted is a leaf, one child,
				or two child case, and will call the appropriate accompanying deletion
				function. 
****************************************************************************************/
bool BST::BSTdelete(BSTtype searchedWord) {
	Node * current = root;
	Node * parent = current;

	while (current != nullptr) {

		if (strcmp(current->info, searchedWord) == 0) {

			/****************************************************************************
			LEAF CASE
			****************************************************************************/
			if (current->left == nullptr && current->right == nullptr) {
				if (current == parent) {
					parent = nullptr;
				}

				deleteLeaf(current, parent);
				nodeCount--;
				return true;
			}

			/***************************************************************************
			TWO CHILD CASE
			***************************************************************************/
			else if (current->left != nullptr && current->right != nullptr) {
				if (current == parent) {
					parent = nullptr;
				}

				deleteTwoChild(current, parent);
				nodeCount--;
				return true;
			}

			/***************************************************************************
			ONE CHILD CASE
			***************************************************************************/
			else {
				if (current == parent) {
					parent = nullptr;
				}

				deleteOneChild(current, parent);
				nodeCount--;
				return true;
			}

			parent = current;

		}

		else if (strcmp(current->info, searchedWord) < 0) {
			parent = current;
			current = current->right;
		}

		else if (strcmp(current->info, searchedWord) > 0) {
			parent = current;
			current = current->left;
		}
	}

	return false;

}

/****************************************************************************************
Function: deleteLeaf
Decription: This function provides the logic required to delete a leaf from the binary
				search tree. 
****************************************************************************************/
void BST::deleteLeaf(Node * leaf, Node * parent) {

	if (parent == nullptr) {
		delete leaf;
		root = nullptr;
	}

	else {
		if (parent->left == leaf)
			parent->left = nullptr;

		else
			parent->right = nullptr;

		delete leaf;
	}

}

/****************************************************************************************
Function: deleteOneChild
Description: This function provides the logic requried to delete a node with one child
				from the binary search tree. The function will determine if the
				oneChild case is the root, or if it is a node with the one child on the
				left side or the right side, and will make the deletion appropriately. 
****************************************************************************************/
void BST::deleteOneChild(Node * oneChild, Node * parent) {

	if (oneChild == root) {
		if (root->left != nullptr)
			root = root->left;

		else
			root = root->right;

		delete oneChild;
	}

	else {
		Node * subTree = oneChild;

		if (oneChild->left != nullptr) {
			subTree = oneChild->left;

			if (strcmp(oneChild->info, parent->info) < 0)
				parent->left = subTree;

			else
				parent->right = subTree;

			delete oneChild;
		}

		else {
			subTree = oneChild->right;

			if (strcmp(oneChild->info, parent->info) < 0)
				parent->left = subTree;

			else
				parent->right = subTree;

			delete oneChild;
		}
	}

}

/****************************************************************************************
Function: deleteTwoChild
Description: This function provides the logic required to delete a node with 2 children.
				The deletion will be made by finding the closest predecessor, then it
				will copy that closest predecessor into the twoChild node. The node with
				the original closest predecessor will then be deleted by using the 
				deleteLeaf or deleteOneChild functions.
****************************************************************************************/
void BST::deleteTwoChild(Node * twoChild, Node * parent) {

	Node * predecessor = twoChild;

	if (predecessor->left != nullptr)
		predecessor = predecessor->left;

	while (predecessor->right != nullptr) {
		parent = predecessor;
		predecessor = predecessor->right;
	}

	strcpy_s(twoChild->info, predecessor->info);
	twoChild->frequency = predecessor->frequency;

	if (predecessor->left == nullptr && predecessor->right == nullptr)
		deleteLeaf(predecessor, parent);

	else
		deleteOneChild(predecessor, parent);

}

/****************************************************************************************
Function: BSTprint
Description: This function is called by the application and will simply call another
				function to perform a full in-order print of the binary search tree.
****************************************************************************************/
void BST::BSTprint(ofstream & fout) {
	InOrderPrint(root, fout);
}

/****************************************************************************************
Function: InOrderPrint
Description: This function will perform an in-order print of the entire BST.
****************************************************************************************/
void BST::InOrderPrint(Node * current, ostream & fout) {

	if (current != nullptr) {
		InOrderPrint(current->left, fout);
		fout << left << setw(25)
			<< current->info << current->frequency << endl;
		InOrderPrint(current->right, fout);
	}

}

/****************************************************************************************
Function: BSTprint
Description: This function will be called by the application to print words from the
				binary search tree that start with the same letter inputted by the user.
				It will call a separate function to perform the partial print of elements
				in the tree. 
****************************************************************************************/
void BST::BSTprint(char letter, ostream & fout) {
	PartialPrint(letter, root, fout);
}

/****************************************************************************************
Function: PartialPrint
Description: This function will be called to print only the words that start with the 
				letter inputted by the user. 
****************************************************************************************/
void BST::PartialPrint(char letter, Node * current, ostream & fout) {

	if (current != nullptr) {
		PartialPrint(letter, current->left, fout);
		if (current->info[0] == letter)
			fout << current->info << endl;

		PartialPrint(letter, current->right, fout);
	}

}

/****************************************************************************************
Function: getBSTHeight
Description: This function is used to send the height of the binary search tree to the
				application. If the root is null, then the tree is empty and a height
				of 0 will be sent to the application. Otherwise, a second function 
				will be called to calculate the height of the tree. 
****************************************************************************************/
int BST::getBSTHeight() {

	if (root == nullptr) {
		height = 0;
		return height;
	}

	height = HeightCalculator(root);
	return height;

}

/****************************************************************************************
Function: HeightCalculator
Description: This function will be used to determine the height of the binary search tree.
				The logic will find the depths the right and left subtrees, compare the
				two depths and will return the largest depth incremented by one. 
****************************************************************************************/
int BST::HeightCalculator(Node * current) {

	if (current == nullptr)
		return 0;

	else {
		int leftDepth = HeightCalculator(current->left);
		int rightDepth = HeightCalculator(current->right);

		if (leftDepth > rightDepth)
			return leftDepth + 1;
		else
			return rightDepth + 1;
	}

}

/****************************************************************************************
Function: getBSTNodeCount
Description: This function returns the node counter being kept throughout the 
				implementation of the binary search tree. 
****************************************************************************************/
int BST::getBSTNodeCount() {
	return nodeCount;
}

/****************************************************************************************
Function: BSTexists
Description: This function is used to simply determine if a word is found in the BST
				by searching through the tree. 
****************************************************************************************/
bool BST::BSTexists(BSTtype searchedWord) {

	Node * current = root;

	if (root == nullptr)
		return false;

	else {
		while (current != nullptr) {
			if (strcmp(current->info, searchedWord) == 0)
				return true;

			else if (strcmp(current->info, searchedWord) < 0)
				current = current->right;

			else
				current = current->left;
		}
		return false;
	}

}

/****************************************************************************************
Function: BSTinc
Description: This function is used to search through the tree and see if a word exists
				in the BST and then the frequency field of that word will be incremented.
				This is used so there are no duplicate words entered into the BST.
****************************************************************************************/
bool BST::BSTinc(BSTtype searchedWord) {

	Node * current = root;

	while (current != nullptr) {
		if (strcmp(current->info, searchedWord) == 0) {
			current->frequency++;
			return true;
		}

		else if (strcmp(current->info, searchedWord) < 0)
			current = current->right;

		else
			current = current->left;
	}

}