#include "tree.h"

BST::BST(void){

	n = 0;
	name = "BST";
	root = NULL;
}

BST::~BST(void){

	if(root != NULL) delete_tree_1(root);
}

struct Node* BST::create_Node(int k, int v)
{
    Node* node;
    node = new Node; //allocate a memory block for the node

    node->k   = k;
    node->v   = v;
    node->p   = -1;
    node->left   = NULL;
    node->right  = NULL;
    node->parent = NULL;
    node->left_flag = 0;
    node->h = 0; //leaf node's height = 0

    return node;
}

void BST::copy_Node(Node *dest, Node *src) //only key, value, priority; maintain the same position in the tree
{
    dest->k   = src->k;
    dest->v   = src->v;
    dest->p   = src->p;
}

int BST::height(Node *node){

	if(node == NULL) return -1;
	return node->h;
}

void BST::update_parent(Node *node, Node *parent, bool left_flag){

	node->parent = parent;
	node->left_flag = left_flag;
}

void BST::update_height(Node *node){

	if(node == NULL) return;
	node->h = max(height(node->left), height(node->right))+1;
}

Node* BST::rotate_right(Node *B)
{
	Node *A = B->left;
    Node *y = A->right;
    Node *B_parent = B->parent;
    bool B_left_flag = B->left_flag;

    // Perform rotation
    A->right = B;
    B->left = y;

    if(B_parent != NULL){

    	if(B_left_flag){

    		B_parent->left = A;
    	}
    	else B_parent->right = A;
    }
    else root = A;

    // Update heights: from child to parent
    update_height(B);
    update_height(A);

    // Update parent
    if(y != NULL) update_parent(y, B, 1);
    update_parent(A, B_parent, B_left_flag);
    update_parent(B, A, 0);

    // Return new root
    return A;
}

Node* BST::rotate_left(Node *A)
{
	Node *B = A->right;
	Node *y = B->left;
    Node *A_parent = A->parent;
    bool A_left_flag = A->left_flag;

    // Perform rotation
    A->right = y;
    B->left = A;

    if(A_parent != NULL){

    	if(A_left_flag){

    		A_parent->left = B;
    	}
    	else A_parent->right = B;
    }
    else root = B;

    // Update heights: from child to parent
    update_height(A);
    update_height(B);

    // Update parent
    if(y != NULL) update_parent(y, A, 0);
    update_parent(B, A_parent, A_left_flag);
    update_parent(A, B, 1);

    // Return new root
    return B;
}

int BST::search(int k, int *v){

	Node *node = NULL;
	int counter = search_1(k, &node);
	if(counter > 0) *v = node->v;
	else *v = 0;
	return counter;
}

int BST::search_1(int k, Node **node)
{
	Node *node_1 = NULL;
	*node = root;
	int counter = 0;

	while(*node != NULL){

		counter++;
		if(k < (*node)->k){

			node_1 = *node;
			*node = (*node)->left;
		}
	    else if(k > (*node)->k){

	    	node_1 = *node;
	    	*node = (*node)->right;
	    }
	    else return counter;
	}

	*node = node_1; //last node on search path
	return -counter;
}

int BST::insert(int k, int v){

	Node *new_node = NULL;
	bool update = 0;
	int counter = insert_1(k, v, &new_node, update);
	counter = counter + update_path_height(new_node);
	return counter;
}

int BST::insert_1(int k, int v, Node **new_node, bool &update)
{
	*new_node = NULL;
	update = 0;

	if(n == 0){

		*new_node = create_Node(k, v);
		root = *new_node;
		n++;
		return 1;
	}

	//initialization
	Node *node;
	node = root;
	Node *parent = NULL;
	bool left_flag = 0;
	int counter = 0;

	while(node != NULL){

		counter++; //enter an existing node

		if(k < node->k) {

			parent = node;
			node = node->left;
			left_flag = 1;
		}
	    else if(k > node->k){

	    	parent = node;
	    	node = node->right;
	    	left_flag = 0;
	    }
	    else{

	    	node->v = v;
	    	update = 1;
	    	return counter;
	    }
	}

	//key does not exist => create a new node
	*new_node = create_Node(k, v);
	counter++;

	if(left_flag){

		parent->left = *new_node;
	}
	else{

		parent->right = *new_node;
	}
	update_parent(*new_node, parent, left_flag);
	n++;

	return counter;
}

int BST::update_path_height(Node *node){

	if(node == NULL) return 0;
	update_height(node);

	while(node->parent != NULL){

		update_height(node->parent);
		node = node->parent;
	}

	return 0;
}

Node *BST::transfer_child(Node *node){ //caution: assume at most one child

	if(node == NULL) return NULL;

	Node *child;
	if(node->left == NULL) child = node->right;
	else if(node->right == NULL) child = node->left;

	if(node != root){

		if(node->left_flag){

			node->parent->left = child;
		}
		else node->parent->right = child;

		if(child != NULL){

			update_parent(child, node->parent, node->left_flag);
		}
	}
	else{

		if(child != NULL) child->parent = NULL;
		root = child;
	}

	return child;
}

int BST::delete_Node(Node *node, Node **parent){

	if(node == NULL) return 0;

	int counter = 0;

	if(node->left == NULL || node->right == NULL){

		Node *child = transfer_child(node);

		if(child != NULL){

			counter++; //access one child
		}

		*parent = node->parent;

		delete node;
		node = NULL;
		n--;
	} //if(node->left == NULL || node->right == NULL)
	else{

		//both left and right children exist
		//=> find the inorder successor
		Node *node_1;

		node_1 = node->right;
		counter++;

		while(node_1->left != NULL){

			counter++;
			node_1 = node_1->left;
		}

		copy_Node(node, node_1);

		counter = counter + delete_Node(node_1, parent); //node_1 has no left child => return to case 1
	}

	return counter;
}

int BST::delete_key(int k)
{
	Node *node = NULL, *parent = NULL;
	int counter = search_1(k, &node);
	if(counter <= 0) return counter;
	counter = counter + delete_Node(node, &parent);
	counter = counter + update_path_height(parent);
	return counter;
}

void BST::print_tree_info(void){

	cout << name << ": n = " << n;
	if(root != NULL){

		cout << ", root = " << root->k << ", height = " << tree_height();
	}
	cout << endl;
}

void BST::print_tree(void){

	print_tree_info();

	int counter = 0;
	if(root == NULL){

		printf("Empty tree...\n");
	}
	else print_tree_1('T', root, counter);
	printf("\n");

	return;
}

void BST::indentation(int counter){

	for(int i = 0; i < counter; i++) printf("\t");
}

void BST::print_Node(Node *node){

	if(node->parent != NULL) printf("(%d)", node->parent->k);
	if(node->p < 0) printf("<%d,%d>(%d)\n", node->k, node->v, node->h);
	else printf("<%d(%.2f),%d>(%d)\n", node->k, node->p, node->v, node->h);
}

void BST::print_tree_1(char c, Node *node, int counter){

	indentation(counter);
	if(node->h < 0) printf("Bug: height should be non-negative!\n");
	printf("%c", c);
	print_Node(node);
	counter++;

	if(node->left != NULL){

		if(node->right != NULL){

			if(node->left->k >= node->right->k) printf("Bug: keys should be ordered!\n");
		}

		print_tree_1('L',node->left, counter);
	}

	if(node->right != NULL){

		print_tree_1('R',node->right, counter);
	}

	if(node->left == NULL && node->right == NULL){

		if(node->h != 0) printf("Bug: leaf node's height should be zero!\n");
	}
}

void BST::delete_tree_1(Node *node){

	if(node->left != NULL) delete_tree_1(node->left);
	if(node->right != NULL) delete_tree_1(node->right);

	delete node;
	node = NULL;
}

Treap::Treap(void){

	n = 0;
	name = "Treap";
	root = NULL;
}

Treap::~Treap(void){

	if(root != NULL) delete_tree_1(root);
}

int Treap::insert(int k, int v){

	Node *new_node = NULL;
	bool update = 0;
	int counter = 0, rotate_counter = 0;
	counter = counter + insert_1(k, v, &new_node, update);
	if(counter <= 0 || update == 1) return counter;
	counter = counter + update_path_height(new_node);

	std::uniform_real_distribution<double> distribution(0.0,1.0);
	new_node->p = distribution(generator);

	while(new_node != root){

		if(new_node->p > new_node->parent->p){

			rotate_counter++;

			if(new_node->left_flag){

				new_node = rotate_right(new_node->parent);
			}
			else{

				new_node = rotate_left(new_node->parent);
			}
		}
		else break;
	}

	counter = counter + update_path_height(new_node);
	return counter + rotate_c*rotate_counter;
}

int Treap::delete_key(int k){

	Node *node = NULL, *parent = NULL;
	int counter = 0, rotate_counter = 0;
	counter = counter + search_1(k, &node);
	if(counter <= 0) return counter;

	while(node->h > 0){ //while not at leaf

		rotate_counter++;
		if(node->left == NULL) rotate_left(node);
		else if(node->right == NULL) rotate_right(node);
		else if(node->left->p > node->right->p) rotate_right(node);
		else rotate_left(node);
	}

	counter = counter + delete_Node(node, &parent);
	counter = counter + update_path_height(parent);
	return counter + rotate_c*rotate_counter;
}

AVL::AVL(void){

	n = 0;
	name = "AVL";
	root = NULL;
}

AVL::~AVL(void){

	if(root != NULL) delete_tree_1(root);
}

int AVL::insert(int k, int v){

	Node *new_node = NULL;
	bool update = 0;

	int counter = insert_1(k, v, &new_node, update);
	counter = counter + update_path_height(new_node);
	return counter;
}

int AVL::delete_key(int k){

	Node *node = NULL, *parent = NULL;
	int counter = search_1(k, &node);
	if(counter <= 0) return counter;
	counter = counter + delete_Node(node, &parent);
	counter = counter + update_path_height(parent);
	return counter;
}

int AVL::update_path_height(Node *node){

	if(node == NULL) return 0;

	int rotate_num = 0, rotate_counter = 0, counter = 0;
	counter++;
	node = AVL_rotate(node, rotate_num);
	if(abs(balance_factor(node)) >= 2){

		printf("Bug: AVL tree should be balanced!\n");
		print_tree();
	}
	rotate_counter = rotate_counter + rotate_num;

	//perform retracing
	while(node->parent != NULL){

		counter++;
		node = AVL_rotate(node->parent, rotate_num);
		if(abs(balance_factor(node)) >= 2){

			printf("Bug: AVL tree should be balanced!\n");
			print_tree();
		}
		rotate_counter = rotate_counter + rotate_num;
		if(node == NULL) break;
	}

	return counter + rotate_c*rotate_counter;
}

Node *AVL::AVL_rotate(Node *node, int &rotate_num){

	if(node == NULL) return NULL;

	if(balance_factor(node) >= 2){

		if(balance_factor(node->right) >= 0){

			rotate_num = 1;
			return rotate_left(node);
		}
		if(balance_factor(node->right) == -1){

			rotate_num = 2;
			rotate_right(node->right);
			return rotate_left(node);
		}

		printf("Bug: undefined case at node: ");
		print_Node(node);
	}

	if(balance_factor(node) <= -2){

		if(balance_factor(node->left) <= 0){

			rotate_num = 1;
			return rotate_right(node);
		}
		if(balance_factor(node->left) == 1){

			rotate_num = 2;
			rotate_left(node->left);
			return rotate_right(node);
		}

		printf("Bug: undefined case at node: ");
		print_Node(node);
	}

	rotate_num = 0;
	update_height(node);
	return node;
}

int AVL::balance_factor(Node *node){

	return height(node->right) - height(node->left);
}

Splay::Splay(void){

	n = 0;
	name = "Splay";
	root = NULL;
}

Splay::~Splay(void){

	if(root != NULL) delete_tree_1(root);
}

Node *Splay::Splay_rotate(Node *node, int &rotate_num){

	if(node == NULL || node == root){

		rotate_num = 0;
		return node;
	}

	//zig
	if(node->parent == root){

		rotate_num = 1;

		if(node->left_flag){

			return rotate_right(node->parent);
		}
		else return rotate_left(node->parent);
	}
	else{

		Node *node_1 = NULL;
		rotate_num = 2;

		//zig-zig
		if(node->left_flag == node->parent->left_flag){

			if(node->left_flag){

				node_1 = rotate_right(node->parent->parent);
				return rotate_right(node_1);
			}
			else{

				node_1 = rotate_left(node->parent->parent);
				return rotate_left(node_1);
			}
		}
		else{ //zig-zag

			if(node->left_flag){

				node_1 = rotate_right(node->parent);
				return rotate_left(node_1->parent);
			}
			else{

				node_1 = rotate_left(node->parent);
				return rotate_right(node_1->parent);
			}
		}
	}
}

int Splay::search(int k, int *v){

	Node *node = NULL;
	int rotate_num = 0, rotate_counter = 0, counter = 0;
	counter = counter + search_1(k, &node);

	while(node != root){

		node = Splay_rotate(node, rotate_num);
		rotate_counter = rotate_counter + rotate_num;
	}

	if(counter > 0){

		if(root->k != k) printf("Bug: found node should be at the root!\n");
		*v = root->v;
		return counter + rotate_c*rotate_counter;
	}
	else{

		*v = 0;
		return -(abs(counter) + rotate_c*rotate_counter);
	}
}

int Splay::insert(int k, int v){

	Node *new_node = NULL;
	bool update = 0;
	int rotate_num = 0, rotate_counter = 0, counter = 0;
	counter = counter + insert_1(k, v, &new_node, update);
	counter = counter + update_path_height(new_node);

	while(new_node != root){

		new_node = Splay_rotate(new_node, rotate_num);
		rotate_counter = rotate_counter + rotate_num;
	}
	if(root->k != k) printf("Bug: inserted node should be at the root!\n");

	return counter + rotate_c*rotate_counter;
}

int Splay::delete_key(int k){

	Node *node = NULL, *parent = NULL;
	int rotate_num = 0, rotate_counter = 0, counter = 0;
	counter = counter + search_1(k, &node);

	if(counter > 0){ //found: splay parent node

		counter = counter + delete_Node(node, &parent);
		counter = counter + update_path_height(parent);

		while(parent != root && parent != NULL){

			parent = Splay_rotate(parent, rotate_num);
			rotate_counter = rotate_counter + rotate_num;
		}

		return counter + rotate_c*rotate_counter;
	}
	else{ //not found: splay last node on search path

		while(node != root){

			node = Splay_rotate(node, rotate_num);
			rotate_counter = rotate_counter + rotate_num;
		}

		return -(abs(counter) + rotate_c*rotate_counter);
	}
}
