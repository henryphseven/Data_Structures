#ifndef COMPLETE_TEST_H
#define COMPLETE_TEST_H

#include <string>
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "tree.h"

#define base_n 2
#define n_start 4
#define base_alpha 0.1

enum Tree_Type {BST_Tree, AVL_Tree, Treap_Tree, Splay_Tree};

int test_n(int i) {return int(pow((double)base_n, i+n_start))-1;}

float test_alpha(int i) {

	return base_alpha*(i+1);
}

string test_name(string r, int i){

	return "test_" + r + "_" + to_string(i+n_start) + ".txt";
}

typedef struct{

	string name;
	int n; //tree size
	float alpha; //ratio of updates to searches

} test_case;

typedef struct{

	int kmin, kmax, vmin, vmax;
	double kmu, ksig, vmu, vsig;

} domain;

domain d = {0, 99999999, 0, 100, 50000000, 50000000.0/2, 75, 25.0/2}; //int kmin, kmax, vmin, vmax;

typedef struct{

	int worst;
	float average;

} op_time;

typedef struct{

	op_time search, height, insert, delete_key, insert_all, delete_all, height0;

} test_result;

int value(double v, int min, int max){

	if(v > max) return max;
	else if(v < min) return min;
	else return (int)v;
}

void set_test_case(test_case *tc, string name, int n, float alpha){

	tc->name = name;
	tc->n = n;
	tc->alpha = alpha;
}

void test_generator(int n, domain *d, string name){

	if(n > d->kmax-d->kmin+1){

		printf("# of keys (n) is large than domain range. Please reduce n or increase domain range.\n");
		return;
	}

	int k, v;
	set<int> k_set;
	set<int>::iterator it;

	ofstream ofile(name, ios::out);

	if(!ofile.is_open()) cout << "Fail to open " << name << endl;
	else{

		ofile << n << "\n";

		std::default_random_engine generator;
		std::uniform_int_distribution<int> k_distribution(d->kmin, d->kmax);
		//std::normal_distribution<double> k_distribution(d->kmu, d->ksig);
		//std::uniform_int_distribution<int> v_distribution(d->vmin, d->vmax);
		std::normal_distribution<double> v_distribution(d->vmu, d->vsig);

		for(int i = 0; i < n; i++){

			do{

				k = k_distribution(generator);
				//k = value(k_distribution(generator), d->kmin, d->kmax);
				it = k_set.find(k);

			}while(it != k_set.end());

			k_set.insert(k);
			ofile << k << ",";

			v = value(v_distribution(generator), d->vmin, d->vmax);
			//int value(double v, int min, int max)
			ofile << v << ";";
		}
	}

	ofile.close();
}

void test_generator_1(string test_case, string name){ //transform random order to sequential order

	char data[20];
	bool read_key = 0;

	int k, v, n;
	map<int,int> k_set;
	map<int,int>::iterator it;

	ifstream ifile(test_case, ios::in);

	if(!ifile.is_open()) {

		cout << "Fail to open" << test_case << endl;
		return;
	}
	else{

		ifile.getline(data, sizeof(data),'\n');
		n = atoi(data);

		while(ifile.getline(data,sizeof(data),';')){

				stringstream ss(data);
				string num;
				char num_1[10];

				read_key = 1;

				while(getline(ss, num, ',')){

					num.copy(num_1, num.length(), 0);
					num_1[num.length()] = '\0';

					if(read_key){

						k = atoi(num_1);
						read_key = 0;
					}
					else v = atoi(num_1);
			    }

				k_set.insert(pair<int,int>(k, v));

		} //while(ifile.getline(data,sizeof(data),';'))

		ifile.close();
	}

	ofstream ofile(name, ios::out);

	if(!ofile.is_open()){

		cout << "Fail to open " << name << endl;
		return;
	}
	else{

		ofile << n << "\n";

		for(it = k_set.begin(); it != k_set.end(); ++it){

			ofile << it->first << ",";
			ofile << it->second << ";";
		}

		ofile.close();
	}
}

void init_test_result(test_result *tr){

	tr->search.worst = 0;
	tr->search.average = 0;

	tr->height.worst = 0;
	tr->height.average = 0;

	tr->insert.worst = 0;
	tr->insert.average = 0;

	tr->delete_key.worst = 0;
	tr->delete_key.average = 0;

	tr->insert_all.worst = 0;
	tr->insert_all.average = 0;

	tr->delete_all.worst = 0;
	tr->delete_all.average = 0;

	tr->height0.worst = 0;
	tr->height0.average = 0;
}

void output_result(ofstream &ofile, test_result *tr){

	ofile << tr->search.worst << ",";
	ofile << tr->search.average << ",";

	ofile << tr->height.worst << ",";
	ofile << tr->height.average << ",";

	ofile << tr->insert.worst << ",";
	ofile << tr->insert.average << ",";

	ofile << tr->delete_key.worst << ",";
	ofile << tr->delete_key.average << ",";

	ofile << tr->insert_all.worst << ",";
	ofile << tr->insert_all.average << ",";

	ofile << tr->delete_all.worst << ",";
	ofile << tr->delete_all.average << ",";

	ofile << tr->height0.worst << ",";
	ofile << tr->height0.average << endl;
}

int single_test(test_case *tc, Tree *tree, test_result *tr, int cycle_num, bool debug){

	std::default_random_engine generator;
	std::uniform_int_distribution<int> k_distribution(d.kmin, d.kmax);
	//std::normal_distribution<double> k_distribution(d.kmu, d.ksig);
	//std::uniform_int_distribution<int> v_distribution(d.vmin, d.vmax);
	std::normal_distribution<double> v_distribution(d.vmu, d.vsig);

	init_test_result(tr); //initialize test result storage space

	set<int> *k_set;
    k_set = new set<int>();
    set<int>::iterator k_it;

	char data[20];
	bool read_key;
	int k, v, t, h, n;
	double u;
	string test_case = tc->name;
	float alpha = tc->alpha; //alpha = # of updates / total # of operations
	int base_op_num = 20;
	int update_num = (int)(base_op_num*alpha/2.0);
	int search_num = base_op_num - 2*update_num;

	ifstream ifile(test_case, ios::in);

	if(!ifile.is_open()) {

		cout << "Fail to open" << test_case << endl;
		return -1;
	}
	else{

		ifile.getline(data, sizeof(data),'\n');
		n = atoi(data);
		//if(n != tc->n) printf("Bug: may read the wrong file!\n");
		std::uniform_real_distribution<double> u_distribution(0.0,1.0);

		while(ifile.getline(data,sizeof(data),';')){

				stringstream ss(data);
				string num;
				char num_1[10];

				read_key = 1;

				while(getline(ss, num, ',')){

					num.copy(num_1, num.length(), 0);
					num_1[num.length()] = '\0';

					if(read_key){

						k = atoi(num_1);
						read_key = 0;
					}
					else v = atoi(num_1);
			    }

				//insert <key,value>
				t = tree->insert(k, v);
				if(t < 0){

					cout << "Bug: insert " << k << " should succeed!" << endl;
					tree->print_tree_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				t = abs(t);
				tr->insert_all.average = tr->insert_all.average + t;
				tr->insert_all.worst = max(tr->insert_all.worst, t);

				tr->height0.average = tr->height0.average + tree->tree_height();

				k_set->insert(k);

		} //while(ifile.getline(data,sizeof(data),';'))

		ifile.close();

		//at this time point, all data entries should have been inserted to trees
		if(tree->get_element_num() != n || k_set->size() != (unsigned int)n){

			printf("Bug: n = %d, tree size = %d, key set size = %d\n", n, tree->get_element_num(), (int)k_set->size());
			return -1;
		}

		tr->insert_all.average = tr->insert_all.average/n;
		tr->height0.average = tr->height0.average/n;
		tr->height0.worst = tree->tree_height();

		if(debug){

			tree->print_tree();
		}

		//perform cycle_num*base_op_num operations:
		//random deletion alpha/2 -> random insertion alpha/2 -> random search 1-alpha
		for(int p = 0; p < cycle_num; p++){

				int op_counter;

				//Step 1: random update: deletion -> insertion
				op_counter = 0;
				while(op_counter < update_num){

					//deletion
					u = u_distribution(generator);
					h = (int)(tree->get_element_num()*u);
					k_it = k_set->begin();
					std::advance(k_it, h);
					k = *k_it;

					t = tree->delete_key(k);
					if(t < 0){

						cout << "Bug: delete " << k << " should succeed!" << endl;
						tree->print_tree_info();
						cout << "Test Case: " << test_case << endl;
						return -1;
					}
					if(debug) cout << "delete " << k << " cost: " << t << endl;
					t = abs(t);
					tr->delete_key.average = tr->delete_key.average + t;
					tr->delete_key.worst = max(tr->delete_key.worst, t);

					tr->height.average = tr->height.average + tree->tree_height();
					tr->height.worst = max(tr->height.worst, tree->tree_height());

					k_set->erase(k);

					//insertion
					do{

						k = k_distribution(generator);
						//k = value(k_distribution(generator), d.kmin, d.kmax);
						k_it = k_set->find(k);

					}while(k_it != k_set->end());

					v = value(v_distribution(generator), d.vmin, d.vmax);

					t = tree->insert(k, v);
					if(t < 0){

						cout << "Bug: insert " << k << " should succeed!" << endl;
						tree->print_tree_info();
						cout << "Test Case: " << test_case << endl;
						return -1;
					}
					if(debug) cout << "insert " << k << " cost: " << t << endl;
					t = abs(t);
					tr->insert.average = tr->insert.average + t;
					tr->insert.worst = max(tr->insert.worst, t);

					tr->height.average = tr->height.average + tree->tree_height();
					tr->height.worst = max(tr->height.worst, tree->tree_height());

					k_set->insert(k);

					if(tree->get_element_num() != n || k_set->size() != (unsigned int)n){

						printf("Bug: n = %d, tree size = %d, key set size = %d\n", n, tree->get_element_num(), (int)k_set->size());
						return -1;
					}

					op_counter++;
				}

				//Step 2: random successful search
				op_counter = 0;
				while(op_counter < search_num){

					u = u_distribution(generator);
					h = (int)(tree->get_element_num()*u);
					k_it = k_set->begin();
					std::advance(k_it, h);
					k = *k_it;

					t = tree->search(k, &v);
					if(t < 0){

						cout << "Bug: search " << k << " should succeed!" << endl;
						tree->print_tree_info();
						cout << "Test Case: " << test_case << endl;
						if(debug) return -1;
					}
					if(debug) cout << "search " << k << " cost: " << t << endl;
					t = abs(t);
					tr->search.average = tr->search.average + t;
					tr->search.worst = max(tr->search.worst, t);

					tr->height.average = tr->height.average + tree->tree_height();
					tr->height.worst = max(tr->height.worst, tree->tree_height());

					op_counter++;
				}

				if(tree->get_element_num() != n || k_set->size() != (unsigned int)n){

					printf("Bug: n = %d, tree size = %d, key set size = %d\n", n, tree->get_element_num(), (int)k_set->size());
					return -1;
				}
		}

		if(debug) tree->print_tree();

		if(cycle_num <= 0) cycle_num = 1;
		tr->delete_key.average = tr->delete_key.average/(cycle_num*update_num);
		tr->insert.average = tr->insert.average/(cycle_num*update_num);
		tr->search.average = tr->search.average/(cycle_num*search_num);
		tr->height.average = tr->height.average/(cycle_num*base_op_num);

		while(tree->get_element_num() > 0){

				//randomly delete keys
				u = u_distribution(generator);
				h = (int)(tree->get_element_num()*u);

				//sequentially delete keys
				//k_set is ordered
				//h = k_set->size()-1;

				k_it = k_set->begin();
				std::advance(k_it, h);
				k = *k_it;

				t = abs(tree->delete_key(k));
				if(t < 0){

					cout << "Bug: delete " << k << " should succeed!" << endl;
					tree->print_tree_info();
					cout << "Test Case: " << test_case << endl;
					return -1;
				}
				t = abs(t);
				tr->delete_all.average = tr->delete_all.average + t;
				tr->delete_all.worst = max(tr->delete_all.worst, t);

				k_set->erase(k);
		}

		if(debug) tree->print_tree();

		if(tree->get_element_num() != 0){

			printf("Bug: tree should be empty!\n");
			return -1;
		}
		tr->delete_all.average = tr->delete_all.average/n;

	} //if(!ifile.is_open())

	if(debug){

		cout << tree->get_name() << " test result:\n";
		cout << "When n = " << n << ", alpha = " << alpha << ":\n";
		cout << "search: worst = " << tr->search.worst << ", average = " << tr->search.average << endl;
		cout << "height: worst = " << tr->height.worst << ", average = " << tr->height.average << endl;
		cout << "insert: worst = " << tr->insert.worst << ", average = " << tr->insert.average << endl;
		cout << "delete: worst = " << tr->delete_key.worst << ", average = " << tr->delete_key.average << endl;
		cout << "insert all: worst = " << tr->insert_all.worst << ", average = " << tr->insert_all.average << endl;
		cout << "delete all: worst = " << tr->delete_all.worst << ", average = " << tr->delete_all.average << endl;
		cout << "height0: worst = " << tr->height0.worst << ", average = " << tr->height0.average << endl;
	}

	delete k_set;
	return 0;
}

Tree *create_tree(Tree_Type tt){

	Tree *tree;

	switch(tt){

	case BST_Tree:
		tree = new BST();
		break;
	case AVL_Tree:
		tree = new AVL();
		break;
	case Treap_Tree:
		tree = new Treap();
		break;
	case Splay_Tree:
		tree = new Splay();
		break;
	}

	return tree;
}

void basic_test(string case_name, float alpha, Tree_Type tt, int cycle_num){

	Tree *tree;
	test_result tr;
	test_case tc;
	set_test_case(&tc, case_name, 0, alpha);
	tree = create_tree(tt);
	single_test(&tc, tree, &tr, cycle_num, 1);
	printf("\n");
	delete tree;
}

void basic_test_all(string case_name){

	for(int i = 0; i < 4; i++) basic_test(case_name, 0.5, (Tree_Type)i, 1);
}

void complete_test(string r, bool gen_case){

	const int n_size = 10, alpha_size = 9; //n: 2^4-1~2^13-1, alpha: 0.1, ... , 0.9
	//const int n_size = 10, alpha_size = 1;
	test_case tc[n_size][alpha_size];
	test_result tr[4][n_size][alpha_size];

	//example: key: student id of MCS, value: final-term score of data structure
	//generate case for different n
	if(gen_case){

		for(int i = 0; i < n_size; i++){

			test_generator(test_n(i), &d, test_name(r, i));
		}
	}

	Tree *tree;
	Tree_Type tt;
	int rc;

	for(int i = 0; i < n_size; i++){
		for(int j = 0; j < alpha_size; j++){

			//record test case information
			set_test_case(&tc[i][j], test_name(r, i), test_n(i), test_alpha(j));

			//single_test(test_case *tc, Tree *tree, test_result *tr, int cycle_num, bool debug)
			for(int k = 0; k < 4; k++){

				tt = (Tree_Type)k;
				tree = create_tree(tt);
				rc = single_test(&tc[i][j], tree, &tr[k][i][j], 3*test_n(i), 0);
				//rc = single_test(&tc[i][j], tree, &tr[k][i][j], 0, 0);
				if(rc < 0) return;
				delete tree;
			}

			printf("test case: n = %d, alpha = %g is completed!\n", test_n(i), test_alpha(j));
		} //for(int j = 0; j < alpha_size; j++)
	} //for(int i = 0; i < n_size; i++)

	//output results to a meta-table
	string result_name = "result_"+r+".txt";
	ofstream ofile(result_name, ios::out);
	if(!ofile.is_open()) cout << "Fail to open " << result_name << endl;
	else{

		ofile << "test_case,n,log_n,alpha,algorithm,";
		ofile << "search_worst,search_average,";
		ofile << "height_worst,height_average,";
		ofile << "insert_worst,insert_average,";
		ofile << "delete_worst,delete_average,";
		ofile << "insert_all_worst,insert_all_average,";
		ofile << "delete_all_worst,delete_all_average,";
		ofile << "height0_worst,height0_average\n";

		string tree_names[4] = {"BST", "AVL", "Treap", "Splay"};

		for(int i = 0; i < n_size; i++){
			for(int j = 0; j < alpha_size; j++){
				for(int k = 0; k < 4; k++){

					tt = (Tree_Type)k;
					ofile << tc[i][j].name << "," << tc[i][j].n << "," << i+n_start << "," << tc[i][j].alpha << ",";
					ofile << tree_names[k] << "," ;
					output_result(ofile, &tr[k][i][j]);
				}
			}
		}
	}

	ofile.close();
}

#endif
