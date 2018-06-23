#ifndef TREE_H
#define TREE_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <iomanip>

using namespace std;

typedef struct Node{

	int k; //key
	int v; //value
	double p; //priority: only used for Treap
	Node *left, *right; //child pointer
	Node *parent; //parent pointer
	bool left_flag; //1 if the node is left child of its parent, 0 if right child
	int h; //height: longest path from leaf to node, root's height = tree height

} Node;

class Tree{

public:
	virtual ~Tree(){};
	string get_name(void) {return name;}
	int get_element_num(void) {return n;}

	//return # of instructions in normal case; return -#of instruction in failure
	virtual int search(int k, int *v) = 0;
	virtual int insert(int k, int v) = 0;
	virtual int delete_key(int k) = 0;
	virtual void print_tree(void) = 0;
	virtual void print_tree_info(void) = 0;
	virtual int tree_height(void) = 0;

protected:
	int n; //number of nodes in the tree
	string name;
};

class BST : public Tree{

public:
	BST(void);
	~BST(void);

	//return # of instructions in normal case; return -#of instruction in failure
	virtual int search(int k, int *v);
	virtual int insert(int k, int v);
	virtual int delete_key(int k);
	virtual void print_tree(void);
	virtual void print_tree_info(void);
	virtual int tree_height(void){

		if(root != NULL) return root->h;
		return 0;
	}

protected:
	Node *root;
	const int rotate_c = 1;

	int search_1(int k, Node **node);
	int insert_1(int k, int v, Node **new_node, bool &update);
	Node* create_Node(int k, int v);
	void update_parent(Node *node, Node *parent, bool left_flag);
	void update_height(Node *node);
	int update_path_height(Node *node); //update heights of nodes from node to root
	Node *transfer_child(Node *node); //return which child is transferred to grandparent
	int delete_Node(Node *node, Node **parent);
	void copy_Node(Node *dest, Node *src);
	void delete_tree_1(Node *node);
	int height(Node *node);
	Node* rotate_right(Node *B);
	Node* rotate_left(Node *A);
	void print_tree_1(char c, Node *node, int counter);
	void indentation(int counter);
	void print_Node(Node *node);
};

class Treap : public BST{

public:
	Treap(void);
	~Treap(void);

	//return # of instructions in normal case; return -#of instruction in failure
	virtual int insert(int k, int v);
	virtual int delete_key(int k);

private:
	std::default_random_engine generator;
};

class AVL : public BST{

public:
	AVL(void);
	~AVL(void);

	//return # of instructions in normal case; return -#of instruction in failure
	virtual int insert(int k, int v);
	virtual int delete_key(int k);

private:
	int update_path_height(Node *node); //update heights of nodes from node to root
	Node *AVL_rotate(Node *node, int &rotate_num);
	int balance_factor(Node *node);
};

class Splay : public BST{

public:
	Splay(void);
	~Splay(void);

	//return # of instructions in normal case; return -#of instruction in failure
	virtual int search(int k, int *v);
	virtual int insert(int k, int v);
	virtual int delete_key(int k);

private:
	Node *Splay_rotate(Node *node, int &rotate_num);
};

#endif
