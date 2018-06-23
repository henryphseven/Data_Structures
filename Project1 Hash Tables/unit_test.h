#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "hashing.h"
#define array_size(a) sizeof(a)/sizeof(a[0])

void unit_test(Hashing *ht, int keys[], int values[], int n){

	cout << "Let's test " << ht->get_name() << "...\n";
	int counter = 0, value;

	for(int i = 0; i < n; i++) {

		counter = counter + abs(ht->set(keys[i], values[i]));
		printf("After inserting <%d,%d>...\n", keys[i], values[i]);
		ht->print_ht();
	}

	int count;

	for(int i = 0; i < n; i++) {

		count = ht->search(keys[i], &value);
		if(count > 0){

			if(value != values[i]) printf("Search <%d,%d> failed!\n", keys[i], values[i]);
			else printf("Search <%d,%d> succeeded!\n", keys[i], values[i]);
		}
		counter = counter + abs(count);
	}

	for(int i = 0; i < n; i++) {

		counter = counter + abs(ht->delete_key(keys[i]));
		printf("After deleting <%d,%d>...\n", keys[i], values[i]);
		ht->print_ht();
	}

	printf("Amortized time of the %d operations is %.1f\n\n", 3*n, float(counter)/(3*n));
}

void simple_test(void){

	Chained chained(11);
	int chained_case[] = {36, 48, 72, 29, 55, 14, 70, 83};
	unit_test(&chained, chained_case, chained_case, array_size(chained_case));

	Linear linear(11);
	int linear_case[] = {83, 14, 29, 70, 10, 55, 72};
	unit_test(&linear, linear_case, linear_case, array_size(linear_case));

	Linear linear1(13);
	int linear_case1[] = {18, 41, 22, 44, 59, 32, 31, 73};
	unit_test(&linear1, linear_case1, linear_case1, array_size(linear_case1));

	Cuckoo cuckoo(11);
	int cuckoo_case[] = {12, 26, 92, 23, 28, 94};
	unit_test(&cuckoo, cuckoo_case, cuckoo_case, array_size(cuckoo_case));

	Cuckoo cuckoo_B(11);
	int cuckoo_case_B[] = {12, 26, 92, 23, 28, 94, 15}; //15 causes cycle
	unit_test(&cuckoo_B, cuckoo_case_B, cuckoo_case_B, array_size(cuckoo_case_B));

	Cuckoo cuckoo1(11);
	int cuckoo_case1[] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39};
	unit_test(&cuckoo1, cuckoo_case1, cuckoo_case1, array_size(cuckoo_case1));

	Cuckoo cuckoo1_B(11);
	int cuckoo_case1_B[] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39, 6}; //6 causes cycle
	unit_test(&cuckoo1_B, cuckoo_case1_B, cuckoo_case1_B, array_size(cuckoo_case1_B));

	Coalesced coalesced(10, 10);
	int coalesced_case[] = {8, 6, 2, 5, 18, 28, 7, 1, 38, 16};
	unit_test(&coalesced, coalesced_case, coalesced_case, array_size(coalesced_case));

	Coalesced coalesced1(20, 10);
	unit_test(&coalesced1, coalesced_case, coalesced_case, array_size(coalesced_case));
}

void resize_test(void){

	int linear_case[] = {83, 14, 29, 70, 10, 55, 72};
	Linear linear(array_size(linear_case)/2);
	unit_test(&linear, linear_case, linear_case, array_size(linear_case));

	int linear_case1[] = {18, 41, 22, 44, 59, 32, 31, 73};
	Linear linear1(array_size(linear_case1)/2);
	unit_test(&linear1, linear_case1, linear_case1, array_size(linear_case1));

	int cuckoo_case[] = {12, 26, 92, 23, 28, 94};
	Cuckoo cuckoo(array_size(cuckoo_case)/2);
	unit_test(&cuckoo, cuckoo_case, cuckoo_case, array_size(cuckoo_case));

	Cuckoo cuckoo_B(11);
	int cuckoo_case_B[] = {12, 26, 92, 23, 28, 94, 15}; //15 causes cycle
	unit_test(&cuckoo_B, cuckoo_case_B, cuckoo_case_B, array_size(cuckoo_case_B));

	int cuckoo_case1[] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39};
	Cuckoo cuckoo1(array_size(cuckoo_case1)/2);
	unit_test(&cuckoo1, cuckoo_case1, cuckoo_case1, array_size(cuckoo_case1));

	Cuckoo cuckoo1_B(11);
	int cuckoo_case1_B[] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39, 6}; //6 causes cycle
	unit_test(&cuckoo1_B, cuckoo_case1_B, cuckoo_case1_B, array_size(cuckoo_case1_B));

	int coalesced_case[] = {8, 6, 2, 5, 18, 28, 7, 1, 38, 16};
	Coalesced coalesced(array_size(coalesced_case)/2, array_size(coalesced_case)/2);
	unit_test(&coalesced, coalesced_case, coalesced_case, array_size(coalesced_case));

	Coalesced coalesced1(array_size(coalesced_case)*2, array_size(coalesced_case));
	unit_test(&coalesced1, coalesced_case, coalesced_case, array_size(coalesced_case));
}

#endif
