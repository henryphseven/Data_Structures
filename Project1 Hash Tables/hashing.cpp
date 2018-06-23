#include "hashing.h"

int Hashing::h(int k){

	return k%N;
}

void Hashing::set_entry(entry *e, int k, int v, int prev, int next){

	e->k = k;
	e->v = v;
	e->valid = 1;
	e->prev = prev;
	e->next = next;
}

void Hashing::move_entry(entry *dest, entry *src){

	dest->k = src->k;
	dest->v = src->v;
	dest->valid = src->valid;
	dest->prev = src->prev;
	dest->next = src->next;
}

void Hashing::clear_entry(entry *e){

	e->k = 0;
	e->v = 0;
	e->valid = 0;
	e->prev = -1;
	e->next = -1; //-1 means null
}

Chained::Chained(int N){

	this->H = new vector<entry>[N];
	this->N = N;
	this->n = 0;
	this->name = "Chained Hashing";
	/*
	 vector<int> arr[N];
It displays an array of vector, each array[i] would have a vector stored in it that can traverse through many values.
It is like a an Array of Linked List where the heads are only stored in array[i] positions.
	 */
}

Chained::~Chained(){

	delete[] H;
}

int Chained::set(int k, int v){

	int i = h(k);

	for(unsigned int j = 0; j < H[i].size(); j++){

		if(H[i].at(j).k == k) {

			H[i].at(j).v = v;
			return j+1+1; //j+1: # of elements in the chain, 1: pointer
		}
	}

	entry e;
	set_entry(&e, k, v, -1, -1);
	H[i].push_back(e);
	n++;
	return H[i].size()+1;
}

int Chained::search(int k, int *v){

	int i = h(k);

	for(unsigned int j = 0; j < H[i].size(); j++){

		if(H[i].at(j).k == k) {

			*v = H[i].at(j).v;
			return j+1+1;
		}
	}

	*v = 0;
	//printf("Search %d failed!\n", k);
	return -(H[i].size()+1);
}

int Chained::delete_key(int k){

	int i = h(k);

	for(unsigned int j = 0; j < H[i].size(); j++){

		if(H[i].at(j).k == k) {

			H[i].erase(H[i].begin()+j);
			n--;
			return j+1+1+1; //j+1: the order in the linked list, 1: pointer in the hash table, 1: change next pointer
		}
	}

	printf("Delete %d failed!\n", k);
	return -(H[i].size()+1);
}

void Chained::print_ht_info(void){

	cout << name;
	printf(": N = %d, n = %d, alpha = %.2f\n", get_ht_size(), get_element_num(), get_load_factor());
}

void Chained::print_ht(void){

	print_ht_info();

	bool empty = 1;

	for(int i = 0; i < N; i++){

		if(H[i].size() > 0){

			empty = 0;

			printf("H[%d]:[ ", i);

			for(unsigned int j = 0; j < H[i].size(); j++){

				printf("<%d(%d),%d> ", H[i].at(j).k, h(H[i].at(j).k), H[i].at(j).v);
			}

			printf("]\n");
		}
	}

	if(empty) printf("Hash table is empty\n");
}

Linear::Linear(int N){

	this->H = new entry[N]; //H is an array of N entries
	this->N = N;
	this->n = 0;
	this->name = "Linear Hashing";

	for(int i = 0; i < N; i++){

		clear_entry(&H[i]);
	}
}

Linear::~Linear(){

	delete[] H;
}

int Linear::resize(void){

	int N_1 = N;
	N = 2*N_1;

	entry *H_1;
	H_1 = new entry[N];

	for(int i = 0; i < N; i++){

		clear_entry(&H_1[i]);
	}

	int counter = 0;

	//copy H to H_1
	for(int i = 0; i < N_1; i++){

		counter++;

		if(H[i].valid == 1){

			int i_1 = h(H[i].k);
			counter++;

			while(H_1[i_1].valid == 1){

				i_1 = (i_1+1)%N;
				counter++;
			}

			set_entry(&H_1[i_1], H[i].k, H[i].v, -1, -1);
		}
	}

	delete[] H;
	H = H_1;
	//printf("After resizing hash table from %d to %d...\n", N_1, N);
	//print_ht();

	return counter;
}

int Linear::set(int k, int v){

	int counter = 0;

	if(n == N) {

		counter = counter + resize();
	}

	int i = h(k);
	counter++;

	while(H[i].valid == 1 && H[i].k != k){

		i = (i+1)%N;
		counter++;
	}

	if(H[i].valid == 0){

		set_entry(&H[i], k, v, -1, -1);
		n++;
	}
	else if(H[i].k == k){

		H[i].v = v;
	}
	else printf("Undefined behavior. Please check.\n");

	return counter;
}

int Linear::search(int k, int *v){

	int i = h(k), counter = 1;

	while(H[i].valid == 1 && H[i].k != k && counter <= N){

		i = (i+1)%N;
		counter++;
	}

	if(H[i].valid == 1 && H[i].k == k && counter <= N){

		*v = H[i].v;
		return counter;
	}
	else{

		//printf("Search %d failed!\n", k);
		return -min(counter, N);
	}
}

int Linear::delete_key(int k){ //eager delete

	int i = h(k), counter = 1;

	while(H[i].valid == 1 && H[i].k != k && counter <= N){

		i = (i+1)%N;
		counter++;
	}

	if(H[i].valid == 0 || counter > N){

		printf("Delete %d failed!\n", k);
		return -min(counter, N);
	}

	int j = (i+1)%N, i_1 = i;
	counter++;

	while(H[j].valid == 1 && j != i_1){

		if(j >= i+1){

			if(!(i+1 <= h(H[j].k) && h(H[j].k) <= j)){

				move_entry(&H[i], &H[j]);
				i = j; //i becomes an empty slot
			}
		}
		else{

			if(!(i+1 <= h(H[j].k) || h(H[j].k) <= j)){

				move_entry(&H[i], &H[j]);
				i = j;
			}
		}

		j = (j+1)%N;
		counter++;
	}

	if(j == i_1) counter--;
	clear_entry(&H[i]);
	n--;

	return counter;
}

void Linear::print_ht_info(void){

	cout << name;
	printf(": N = %d, n = %d, alpha = %.2f\n", get_ht_size(), get_element_num(), get_load_factor());
}

void Linear::print_ht(void){

	print_ht_info();

	bool empty = 1;

	for(int i = 0; i < N; i++){

		if(H[i].valid == 1){

			empty = 0;
			printf("H[%d]:<%d(%d),%d> ", i, H[i].k, h(H[i].k), H[i].v);
		}
	}

	if(empty) printf("Hash table is empty");
	printf("\n");
}

Cuckoo::Cuckoo(int N){

	this->H = new entry[N];
	this->H1 = new entry[N];
	this->N = N;
	this->n = 0;
	this->name = "Cuckoo Hashing";

	for(int i = 0; i < N; i++){

		clear_entry(&H[i]);
		clear_entry(&H1[i]);
	}
}

Cuckoo::~Cuckoo(){

	delete[] H;
	delete[] H1;
}

int Cuckoo::h1(int k){

	return (k/N)%N;
}

int Cuckoo::resize(){

	int N_1 = N;
	N = 2*N_1;

	entry *H_1, *H1_1;
	H_1 = new entry[N];
	H1_1 = new entry[N];

	for(int i = 0; i < N; i++){

		clear_entry(&H_1[i]);
		clear_entry(&H1_1[i]);
	}

	int counter = 0;

	//copy H to H_1
	for(int i = 0; i < N_1; i++){

		counter++;
		if(H[i].valid == 1) counter = counter + set_1(H_1, H1_1, H[i].k, H[i].v);
	}

	for(int i = 0; i < N_1; i++){

		counter++;
		if(H1[i].valid == 1) counter = counter + set_1(H_1, H1_1, H1[i].k, H1[i].v);
	}

	delete[] H;
	delete[] H1;
	H = H_1;
	H1 = H1_1;
	//printf("After resizing hash table from %d to %d...\n", N_1, N);
	//print_ht();

	return counter;
}

int Cuckoo::set(int k, int v){

	//first check if key has already been in the hash table...
	int i = h(k);

	if(H[i].valid == 1){

		if(H[i].k == k){

			H[i].v = v;
			return 1;
		}
	}

	i = h1(k);

	if(H1[i].valid == 1){

		if(H1[i].k == k){

			H1[i].v = v;
			return 2;
		}
	}

	//if key is not in the hash table...
	int counter = 2, counter_1;

	counter_1 = set_1(H, H1, k, v);
	counter = counter + abs(counter_1);

	while(counter_1 < 0){

		counter = counter + resize();
		counter_1 = set_1(H, H1, k, v);
		counter = counter + abs(counter_1);
	}

	n++;

	return counter;
}

int Cuckoo::set_1(entry *H, entry *H1, int k, int v){

	int i, counter = 0, set_counter = 0, k_1, v_1, t = 0, k_o = k;

	//Reza: We know that if the while loop executes more than the number of keys in the hash table,
	//then there is a cycle.
	while(1){

		if(k_o == k) set_counter++;
		if(set_counter == 3) { //benefit: recover all keys when finding the cycle

			return -counter;
		}
		counter++;

		if(t == 0){

			i = h(k);

			if(H[i].valid == 0){

				set_entry(&H[i], k, v, -1, -1);
				return counter;
			}
			else{

				k_1 = H[i].k;
				v_1 = H[i].v;

				set_entry(&H[i], k, v, -1, -1);

				t = 1;
				k = k_1;
				v = v_1;
			}
		}
		else{

			i = h1(k);

			if(H1[i].valid == 0){

				set_entry(&H1[i], k, v, -1, -1);
				return counter;
			}
			else{

				k_1 = H1[i].k;
				v_1 = H1[i].v;

				set_entry(&H1[i], k, v, -1, -1);

				t = 0;
				k = k_1;
				v = v_1;
			}
		}
	}
}

int Cuckoo::search(int k, int *v){

	int i = h(k);

	if(H[i].valid == 1){

		if(H[i].k == k){

			*v = H[i].v;
			return 1;
		}
	}

	i = h1(k);

	if(H1[i].valid == 1){

		if(H1[i].k == k){

			*v = H1[i].v;
			return 2;
		}
	}

	//printf("Search %d failed!\n", k);
	*v = 0;
	return -2;
}

int Cuckoo::delete_key(int k){

	int i = h(k);

	if(H[i].valid == 1){

		if(H[i].k == k){

			clear_entry(&H[i]);
			n--;
			return 1;
		}
	}

	i = h1(k);

	if(H1[i].valid == 1){

		if(H1[i].k == k){

			clear_entry(&H1[i]);
			n--;
			return 2;
		}
	}

	printf("Delete %d failed!\n", k);
	return -2;
}

void Cuckoo::print_ht_info(void){

	cout << name;
	printf(": N = %d, n = %d, alpha = %.2f\n", N, n, float(n)/(2*N));
}

void Cuckoo::print_ht(void){

	print_ht_info();
	//Reza: In some of the data structure references, I see load factor is (n/N) for cuckoo hashing,
	//when n is the number of elements and N is size of each table.
	//So for your analysis, you can have this assumption.

	bool empty = 1;

	for(int i = 0; i < N; i++){

		if(H[i].valid == 1){

			empty = 0;
			printf("H0[%d]:<%d(%d),%d> ", i, H[i].k, h(H[i].k), H[i].v);
		}
	}

	if(empty) printf("Hash table H0 is empty");
	printf("\n");

	empty = 1;

	for(int i = 0; i < N; i++){

		if(H1[i].valid == 1){

			empty = 0;
			printf("H1[%d]:<%d(%d),%d> ", i, H1[i].k, h1(H1[i].k), H1[i].v);
		}
	}

	if(empty) printf("Hash table H1 is empty");
	printf("\n");
}

float Cuckoo::get_load_factor(void){

	return (float)n/(2*N);
}

Coalesced::Coalesced(int M, int N){

	this->M = M;
	this->N = min(M, N);
	this->H = new entry[M]; //H is an array of N entries
	this->n = 0;
	this->name = "Coalesced Hashing";
	this->empty_slot = M-1;
	this->tomb_num = 0;

	for(int i = 0; i < M; i++){

		clear_entry(&H[i]);
	}
}

Coalesced::~Coalesced(){

	delete[] H;
}

int Coalesced::resize(void){

	int M_1 = M, N_1 = N;
	M = 2*M_1;
	N = 2*N_1;

	entry *H_1;
	H_1 = new entry[M];

	for(int i = 0; i < M; i++){

		clear_entry(&H_1[i]);
	}

	int counter = 0, empty_slot_1 = M-1, n_1 = 0, tomb_num_1 = 0;

	//copy H to H_1
	for(int i = 0; i < M_1; i++){

		counter++;
		if(H[i].valid == 1) counter = counter + set_1(H_1, empty_slot_1, n_1, tomb_num_1, H[i].k, H[i].v);
	}

	delete[] H;
	H = H_1;
	empty_slot = empty_slot_1;
	n = n_1;
	tomb_num = tomb_num_1;
	//printf("After resizing hash table from %d to %d...\n", M_1, M);
	//print_ht();

	return counter;
}

int Coalesced::set(int k, int v){

	int counter = 0;

	if(n + tomb_num == M){

		if(empty_slot >= 0) printf("Bug: empty_slot should be null!\n");
		resize();
		//counter = counter + resize();
	}

	return counter + set_1(H, empty_slot, n, tomb_num, k, v);
}

int Coalesced::set_1(entry *H, int &empty_slot, int &n, int &tomb_num, int k, int v){

	int i = h(k), prev = -1, counter = 1, tombstone = -1;

	//check if key already exists
	//if there is "tombstone" in the chain, write down for future use
	do{

		if(H[i].valid == 1 && H[i].k == k){

			H[i].v = v;
			return counter;
		}
		else{

			if(H[i].valid == 0 && H[i].next >=0 && tombstone == -1) tombstone = i;

			prev = i;
			i = H[i].next;
			counter++;
		}

	}while(i >= 0);
	//break loop if i < 0, i.e., reach the end of the linked list

	i = h(k);

	if(H[i].valid == 0 && H[i].next < 0 && i != empty_slot){ //the top place to insert key

		counter--;
		set_entry(&H[i], k, v, -1, -1);
		n++;
	}
	else if(tombstone >= 0){ //if there is tombstone => use tombstone first

		set_entry(&H[tombstone], k, v, H[tombstone].prev, H[tombstone].next);
		n++;

		if(tomb_num >= 1){

			tomb_num--;
		}
		else printf("Bug: tombstone num should be at least one!\n");
	}
    else{ //otherwise use empty slot

    	if(empty_slot < 0) {

    		printf("Bug: should have resized hash table in set()!\n");
    		return -counter;
    	}

		H[prev].next = empty_slot;
		set_entry(&H[empty_slot], k, v, prev, -1);
		n++;

		//reset empty_slot
		if(n + tomb_num == M) empty_slot = -1;
		else{

			int j = empty_slot - 1; //empty_slot >= 0
			counter++;

			while(j >= 0){

				if(H[j].valid == 0 && H[j].next < 0){ //avoid tombstone

				    empty_slot = j;
					break;
				}

				j--;
				counter++;
			}

			if(j < 0){

				printf("Bug: should be at least one empty slot!\n");
				empty_slot = -1;
				counter--;
			}
		}
	}

	return counter;
}

int Coalesced::search(int k, int *v){

	int i = h(k), counter = 1;

	while(!(H[i].valid == 1 && H[i].k == k)){

		if(H[i].next >= 0){

			i = H[i].next;
			counter++;
		}
		else{ //H[i].next < 0 => reach the end of the linked list

			//printf("Search %d failed!\n", k);
			return -counter;
		}
	}

	*v = H[i].v;
	return counter;
}

int Coalesced::delete_key(int k){ //use lazy delete

	int i = h(k), counter = 1, prev = -1;

	while(!(H[i].valid == 1 && H[i].k == k)){

		if(H[i].next >= 0){

			prev = i;
			i = H[i].next;
			counter++;
		}
		else{ //H[i].next < 0 => reach the end of the linked list

			printf("Delete %d failed!\n", k);
			return -counter;
		}
	}

	int next = H[i].next;

	if(next < 0){ //if the item to be deleted is at the end of the linked list

		clear_entry(&H[i]);
		n--;

		empty_slot = max(empty_slot, i);

		while(prev >= 0){

			H[prev].next = -1;
			counter++;

			if(H[prev].valid == 0){

				tomb_num--;
				empty_slot = max(empty_slot, prev);
				prev = H[prev].prev;
			}
			else prev = -1;
		}
	}
	else{ 	//if the item is in the middle of the linked list

		clear_entry(&H[i]);
		H[i].prev = prev;
		H[i].next = next;
		n--;
		tomb_num++;
	}

	return counter;

	/*The following eager delete is problematic in some cases

	int next, prev;

	while(H[i].next >= 0){

		next = H[i].next;
		move_entry(&H[i], &H[next]);
		H[i].next = next;

		prev = i;
		i = next;
		counter++;
	}

	H[prev].next = -1;

	*/
}

void Coalesced::print_ht_info(void){

	cout << name;
	printf(": M = %d, N = %d, n = %d, alpha = %.2f, # of tombstones = %d, utilization = %.2f, "
			"empty slot = %d\n", M, N, n, float(n)/M, tomb_num, float(n+tomb_num)/M, empty_slot);
}

void Coalesced::print_ht(void){

	print_ht_info();

	bool empty = 1;

	for(int i = 0; i < M; i++){

		if(H[i].valid == 1){

			empty = 0;
			printf("H[%d]:<%d(%d),%d>(%d,%d) ", i, H[i].k, h(H[i].k), H[i].v, H[i].prev, H[i].next);
		}
		else if(H[i].next >= 0){ //also print out tombstone for checking

			empty = 0;
			printf("H[%d]:<tombstone>(%d,%d) ", i, H[i].prev, H[i].next);
		}
	}

	if(empty) printf("Hash table is empty");
	printf("\n");
}

float Coalesced::get_load_factor(void){

	return (float)n/M;
}

