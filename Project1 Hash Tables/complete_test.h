#ifndef COMPLETE_TEST_H
#define COMPLETE_TEST_H

#include "hashing.h"
#include <string>
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#define base_n 2
#define n_start 4
#define base_alpha 0.1

enum Hashing_Method {Chained_Hashing, Linear_Hashing, Cuckoo_Hashing, Coalesced_Hashing};

int test_n(int i) {return int(pow((double)base_n, i+n_start));}

float test_alpha(int i) {

	if(i < 10) return base_alpha*(i+1);
	else return 1+0.5*(i-9);
}

string test_name(string r, int i){

	return "test_" + r + "_" + to_string(i+n_start) + ".txt";
}

typedef struct{

	string name;
	int n, N;
	float alpha;

} test_case;

typedef struct{

	int kmin, kmax, vmin, vmax;
	double kmu, ksig, vmu, vsig;

} domain;

domain d = {0, 99999999, 0, 100, 50000000, 50000000.0/2, 75, 25.0/2}; //int kmin, kmax, vmin, vmax;

/*
 Reza: If you consider the number of searches (or the number of collisions or any other parameters)
 as a measure for evaluation and comparison, it can be better than actual clock time.
 Usually actual clock time is inexact and on small test cases you cannot easily have a comparison.
 */
typedef struct{

	int worst;
	float average;

} op_time;

typedef struct{

	op_time search, usearch, set_key, delete_key, set_all, delete_all;

} test_result;

int value(double v, int min, int max){

	if(v > max) return max;
	else if(v < min) return min;
	else return (int)v;
}

void set_test_case(test_case *tc, string name, int n, int N, float alpha){

	tc->name = name;
	tc->n = n;
	tc->N = N;
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

void init_test_result(test_result *tr){

	tr->search.worst = 0;
	tr->search.average = 0;

	tr->usearch.worst = 0;
	tr->usearch.average = 0;

	tr->set_key.worst = 0;
	tr->set_key.average = 0;

	tr->delete_key.worst = 0;
	tr->delete_key.average = 0;

	tr->set_all.worst = 0;
	tr->set_all.average = 0;

	tr->delete_all.worst = 0;
	tr->delete_all.average = 0;
}

void output_result(ofstream &ofile, test_result *tr){

	ofile << tr->search.worst << ",";
	ofile << tr->search.average << ",";

	ofile << tr->usearch.worst << ",";
	ofile << tr->usearch.average << ",";

	ofile << tr->set_key.worst << ",";
	ofile << tr->set_key.average << ",";

	ofile << tr->delete_key.worst << ",";
	ofile << tr->delete_key.average << ",";

	ofile << tr->set_all.worst << ",";
	ofile << tr->set_all.average << ",";

	ofile << tr->delete_all.worst << ",";
	ofile << tr->delete_all.average << endl;
}

int single_test(string test_case, Hashing *hashing, test_result *tr, int seq_num, bool debug){

	std::default_random_engine generator;
	std::uniform_int_distribution<int> k_distribution(d.kmin, d.kmax);
	//std::normal_distribution<double> k_distribution(d.kmu, d.ksig);
	//std::uniform_int_distribution<int> v_distribution(d.vmin, d.vmax);
	std::normal_distribution<double> v_distribution(d.vmu, d.vsig);

	init_test_result(tr);

    set<int> *k_set;
    k_set = new set<int>();
    std::set<int>::iterator k_it;

	char data[20];
	bool read_key;
	int k, k_1, v, v_1, t, h, n, N;
	float alpha;

	ifstream ifile(test_case, ios::in);

	if(!ifile.is_open()) cout << "Fail to open" << test_case << endl;
	else{

		ifile.getline(data, sizeof(data),'\n');
		n = atoi(data);
		std::uniform_int_distribution<int> h_distribution(0, n-1);

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
				t = hashing->set(k, v);
				if(t < 0){

					cout << "Bug: set " << k << "(" << hashing->h(k) << ") should succeed!" << endl;
					hashing->print_ht_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				t = abs(t);
				tr->set_all.average = tr->set_all.average + t;
				tr->set_all.worst = max(tr->set_all.worst, t);

				k_set->insert(k);

		} //while(ifile.getline(data,sizeof(data),';'))

		if(k_set->size() != (unsigned int)n) cout << "Something wrong with reading " << test_case << endl;

		tr->set_all.average = tr->set_all.average/n;

		//at this time point, all data entries should have been inserted to hash tables
		if(debug) {

			hashing->print_ht();
			N = hashing->get_ht_size();
			alpha = hashing->get_load_factor();
		}

		//perform N set of operations:
		//random usearch -> random search -> random deletion -> random insertion
		for(int p = 0; p < seq_num; p++){

			    //Step 1: random unsuccessful search
				do{

					k = k_distribution(generator);
					//k = value(k_distribution(generator), d.kmin, d.kmax);
					k_it = k_set->find(k);

				}while(k_it != k_set->end());

				t = hashing->search(k, &v);
				if(t >= 0){

					cout << "Bug: usearch " << k << "(" << hashing->h(k) << ") should fail!" << endl;
					hashing->print_ht_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				if(debug) cout << "usearch " << k << "(" << hashing->h(k) << ") cost: " << t << endl;
				t = abs(t);
				tr->usearch.average = tr->usearch.average + t;
				tr->usearch.worst = max(tr->usearch.worst, t);

			    //Step 2: random successful search
				h = h_distribution(generator);
				k_it = k_set->begin();
				std::advance(k_it, h);
				k = *k_it;

				t = hashing->search(k, &v);
				if(t < 0){

					cout << "Bug: search " << k << "(" << hashing->h(k) << ") should succeed!" << endl;
					hashing->print_ht_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				if(debug) cout << "search " << k << "(" << hashing->h(k) << ") cost: " << t << endl;
				t = abs(t);
				tr->search.average = tr->search.average + t;
				tr->search.worst = max(tr->search.worst, t);

				//Step 3: random deletion
				h = h_distribution(generator);
				k_it = k_set->begin();
				std::advance(k_it, h);
				k = *k_it;

				k_1 = k; //for future recover
				hashing->search(k, &v_1); //for future recover

				t = hashing->delete_key(k);
				if(t < 0){

					cout << "Bug: delete " << k << "(" << hashing->h(k) << ") should succeed!" << endl;
					hashing->print_ht_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				if(debug) cout << "delete " << k << "(" << hashing->h(k) << ") cost: " << t << endl;
				t = abs(t);
				tr->delete_key.average = tr->delete_key.average + t;
				tr->delete_key.worst = max(tr->delete_key.worst, t);

				//k_set->erase(k);

				//Step 4: random insertion
				do{

					k = k_distribution(generator);
					//k = value(k_distribution(generator), d.kmin, d.kmax);
					k_it = k_set->find(k);

				}while(k_it != k_set->end());

				v = value(v_distribution(generator), d.vmin, d.vmax);

				t = hashing->set(k, v);
				if(t < 0){

					cout << "Bug: set " << k << "(" << hashing->h(k) << ") should succeed!" << endl;
					hashing->print_ht_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				if(debug) cout << "set " << k << "(" << hashing->h(k) << ") cost: " << t << endl;
				t = abs(t);
				tr->set_key.average = tr->set_key.average + t;
				tr->set_key.worst = max(tr->set_key.worst, t);

				//k_set->insert(k);

				if(k_set->size() != (unsigned int)n) cout << "Bug: hash table size should remain the same" << endl;

				//recover Step 3 and 4, does not count into test result
				hashing->delete_key(k);
				hashing->set(k_1, v_1);
		}

		if(debug) hashing->print_ht();

		tr->search.average = tr->search.average/seq_num;
		tr->usearch.average = tr->usearch.average/seq_num;
		tr->set_key.average = tr->set_key.average/seq_num;
		tr->delete_key.average = tr->delete_key.average/seq_num;

		//delete all keys
		for(k_it = k_set->begin(); k_it != k_set->end(); ++k_it){

				k = *k_it;

				t = abs(hashing->delete_key(k));
				if(t < 0){

					cout << "Bug: delete " << k << "(" << hashing->h(k) << ") should succeed!" << endl;
					hashing->print_ht_info();
					cout << "Test Case: " << test_case << endl;
					if(debug) return -1;
				}
				t = abs(t);
				tr->delete_all.average = tr->delete_all.average + t;
				tr->delete_all.worst = max(tr->delete_all.worst, t);
		}

		if(debug) hashing->print_ht();

		tr->delete_all.average = tr->delete_all.average/n;

	} //if(!ifile.is_open())

	if(debug){

		cout << hashing->get_name() << " test result:\n";
		cout << "When n = " << n << ", N = " << N << ", alpha = " << alpha << ":\n";
		cout << "search: worst = " << tr->search.worst << ", average = " << tr->search.average << endl;
		cout << "usearch: worst = " << tr->usearch.worst << ", average = " << tr->usearch.average << endl;
		cout << "set key: worst = " << tr->set_key.worst << ", average = " << tr->set_key.average << endl;
		cout << "delete key: worst = " << tr->delete_key.worst << ", average = " << tr->delete_key.average << endl;
		cout << "set all: worst = " << tr->set_all.worst << ", average = " << tr->set_all.average << endl;
		cout << "delete all: worst = " << tr->delete_all.worst << ", average = " << tr->delete_all.average << endl;
	}

	delete k_set;
	return 0;
}

void basic_test(string test_case, Hashing_Method hm, int N, int seq_num){

	Hashing *hashing;
	test_result tr;

	switch(hm){

	case Chained_Hashing:
		hashing = new Chained(N);
		break;
	case Linear_Hashing:
		hashing = new Linear(N);
		break;
	case Cuckoo_Hashing:
		hashing = new Cuckoo(ceil(N/2.0));
		break;
	case Coalesced_Hashing:
		hashing = new Coalesced(N, int(0.86*N));
		break;
	}

	single_test(test_case, hashing, &tr, seq_num, 1);
	delete hashing;
}

void basic_test_all(string test_case, int N, int seq_num){

	for(int i = 0; i < 4; i++) basic_test(test_case, (Hashing_Method)i, N, seq_num);
}

void complete_test(string r, bool gen_case){

	const int n_size = 10, alpha_size = 16;
	test_case tc[n_size][alpha_size];
	test_result chained_result[n_size][alpha_size];
	test_result linear_result[n_size][alpha_size];
	test_result cuckoo_result[n_size][alpha_size];
	test_result coalesced_result[n_size][alpha_size];

	//example: key: student id of MCS, value: mid-term score of data structure
	//generate case for different n
	if(gen_case){

		for(int i = 0; i < n_size; i++){

			test_generator(test_n(i), &d, test_name(r, i));
		}
	}

	Hashing *hashing;
	int N, rc;

	for(int i = 0; i < n_size; i++){
		for(int j = 0; j < alpha_size; j++){

			N = int(test_n(i)/test_alpha(j));

			//record test case information
			tc[i][j].name = test_name(r, i);
			tc[i][j].n = test_n(i);
			tc[i][j].N = N;
			tc[i][j].alpha = test_alpha(j);

			//void single_test(string test_case, Hashing *hashing, test_result *tr, int seq_num, bool debug)
			hashing = new Chained(N);
			rc = single_test(test_name(r, i), hashing, &chained_result[i][j], 3*test_n(i), 0);
			if(rc < 0) return;
			delete hashing;

			hashing = new Linear(N);
			rc = single_test(test_name(r, i), hashing, &linear_result[i][j], 3*test_n(i), 0);
			if(rc < 0) return;
			delete hashing;

			hashing = new Cuckoo(ceil(N/2.0));
			rc = single_test(test_name(r, i), hashing, &cuckoo_result[i][j], 3*test_n(i), 0);
			if(rc < 0) return;
			delete hashing;

			hashing = new Coalesced(N, int(0.86*N));
			rc = single_test(test_name(r, i), hashing, &coalesced_result[i][j], 3*test_n(i), 0);
			if(rc < 0) return;
			delete hashing;
		} //for(int j = 0; j < alpha_size; j++)
	} //for(int i = 0; i < n_size; i++)

	//output results to a meta-table
	string result_name = "result_"+r+".txt";
	ofstream ofile(result_name, ios::out);
	if(!ofile.is_open()) cout << "Fail to open " << result_name << endl;
	else{

		ofile << "test_case,n,N,alpha,algorithm,";
		ofile << "search_worst,search_average,";
		ofile << "usearch_worst,usearch_average,";
		ofile << "set_key_worst,set_key_average,";
		ofile << "delete_key_worst,delete_key_average,";
		ofile << "set_all_worst,set_all_average,";
		ofile << "delete_all_worst,delete_all_average\n";

		for(int i = 0; i < n_size; i++){
			for(int j = 0; j < alpha_size; j++){

				ofile << tc[i][j].name << "," << tc[i][j].n << "," << tc[i][j].N << "," << tc[i][j].alpha << ",";
				ofile << "Chained," ;
				output_result(ofile, &chained_result[i][j]);

				ofile << tc[i][j].name << "," << tc[i][j].n << "," << tc[i][j].N << "," << tc[i][j].alpha << ",";
				ofile << "Linear," ;
				output_result(ofile, &linear_result[i][j]);

				ofile << tc[i][j].name << "," << tc[i][j].n << "," << tc[i][j].N << "," << tc[i][j].alpha << ",";
				ofile << "Cuckoo," ;
				output_result(ofile, &cuckoo_result[i][j]);

				ofile << tc[i][j].name << "," << tc[i][j].n << "," << tc[i][j].N << "," << tc[i][j].alpha << ",";
				ofile << "Coalesced," ;
				output_result(ofile, &coalesced_result[i][j]);
			}
		}
	}

	ofile.close();
}

#endif
