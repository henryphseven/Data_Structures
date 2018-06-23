#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "tree.h"
#define array_size(a) sizeof(a)/sizeof(a[0])

void unit_test(Tree *t, int keys[], int values[], int n, int deleted_key){

	cout << "Let's test " << t->get_name() << "...\n";
	int counter = 0, value;

	for(int i = 0; i < n; i++) {

		counter = counter + abs(t->insert(keys[i], values[i]));
		printf("After inserting <%d,%d>...\n", keys[i], values[i]);
		t->print_tree();
	}

	if(deleted_key > 0){

		t->search(deleted_key, &value);
		t->delete_key(deleted_key);
		printf("After deleting <%d,%d>...\n", deleted_key, value);
		t->print_tree();
	}
	else{

		int count;

		for(int i = 0; i < n; i++) {

			count = t->search(keys[i], &value);
			if(count > 0){

				if(value != values[i]) printf("Search <%d,%d> failed!\n", keys[i], values[i]);
				else printf("Search <%d,%d> succeeded!\n", keys[i], values[i]);
			}
			counter = counter + abs(count);
		}

		for(int i = 0; i < n; i++) {

			counter = counter + abs(t->delete_key(keys[i]));
			printf("After deleting <%d,%d>...\n", keys[i], values[i]);
			t->print_tree();
		}

		printf("Amortized time of the %d operations is %.1f\n\n", 3*n, float(counter)/(3*n));
	}
}

void unit_test_1(Tree *t, int keys[], int n, int deleted_key){

	unit_test(t, keys, keys, n, deleted_key);
}
//void unit_test(Tree *t, int keys[], int values[], int n, int deleted_key)

void simple_test(void){ //checked ok

	Tree *tree;

	tree = new BST();
	int bst_case[] = {11,3,19,2,5,17,23,29};
	unit_test_1(tree, bst_case, array_size(bst_case), -1);
	delete tree;

	tree = new BST();
	int bst_case1[] = {5,2,23,3,17,29,11,19,13};
	unit_test_1(tree, bst_case1, array_size(bst_case1), -1);
	delete tree;

	tree = new BST();
	int bst_case2[] = {25,11,31,7,21,27,35,9,15,23,29,13,17};
	unit_test_1(tree, bst_case2, array_size(bst_case2), 21);
	delete tree;

	tree = new BST();
	unit_test_1(tree, bst_case2, array_size(bst_case2), 11);
	delete tree;

	tree = new AVL();
	unit_test_1(tree, bst_case1, array_size(bst_case1), -1);
	delete tree;

	tree = new Treap();
	int trp_case[] = {2,3,5,7,11,13,17,19,23,29,31};
	unit_test_1(tree, trp_case, array_size(trp_case), -1);
	delete tree;

	tree = new Splay();
	int splay_case[] = {6,8,7,4,5,2,3,10,9,0,1,12,11};
	unit_test_1(tree, splay_case, array_size(splay_case), -1);
	delete tree;

	tree = new Splay();
	int splay_case1[] = {10,9,8,7,6,5,4,3,2,0,1};
	unit_test_1(tree, splay_case1, array_size(splay_case1), -1);
	delete tree;

	tree = new Splay();
	int splay_case2[] = {4,5,8,7,1,2};
	unit_test_1(tree, splay_case2, array_size(splay_case2), 2);
	delete tree;
}

#endif
