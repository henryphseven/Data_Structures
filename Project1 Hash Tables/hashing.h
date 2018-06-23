#ifndef HASHING_H
#define HASHING_H

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
#include "prime.h"

using namespace std;

typedef struct{

	int k; //key
	int v; //value
	bool valid; //used only in Linear Hashing and Coalesced Hashing
	int prev, next; //used only in Coalesced Hashing

} entry;

class Hashing{

public:
	virtual ~Hashing(){};
	string get_name(void) {return name;}
	int get_element_num(void) {return n;}
	int get_ht_size(void) {return N;}
	int h(int k); //hash function for int keys
	float get_load_factor(void) {return (float)n/N;}

	//return # of instructions in normal case; return -#of instruction in failure
	virtual int set(int k, int v) = 0;
	virtual int search(int k, int *v) = 0;
	virtual int delete_key(int k) = 0;
	virtual void print_ht(void) = 0;
	virtual void print_ht_info(void) = 0;

protected:
	static void set_entry(entry *e, int k, int v, int prev, int next);
	static void move_entry(entry *dest, entry *src);
	static void clear_entry(entry *e);
	virtual int resize(void) = 0;
	int N, n;
	string name;
};

class Chained : public Hashing{

public:
	Chained(int N);
	~Chained();
	virtual int set(int k, int v);
	virtual int search(int k, int *v);
	virtual int delete_key(int k);
	virtual void print_ht(void);
	virtual void print_ht_info(void);

private:
	virtual int resize(void) {return 0;} //chained hashing does not need resizing
	vector<entry> *H;
};

class Linear : public Hashing{

public:
	Linear(int N);
	~Linear();
	virtual int set(int k, int v);
	virtual int search(int k, int *v);
	virtual int delete_key(int k);
	virtual void print_ht(void);
	virtual void print_ht_info(void);

private:
	virtual int resize(void);
	entry *H;
};

class Cuckoo : public Hashing{

public:
	Cuckoo(int N);
	~Cuckoo();
	virtual int set(int k, int v);
	virtual int search(int k, int *v);
	virtual int delete_key(int k);
	virtual void print_ht(void);
	virtual void print_ht_info(void);
	float get_load_factor(void);
	int h1(int k); //hash function for int keys

private:
	virtual int resize(void);
	int set_1(entry *H, entry *H1, int k, int v);
	entry *H, *H1;
};

class Coalesced : public Hashing{

public:
	Coalesced(int M, int N); //M: hash table size, N: hash function
	~Coalesced();
	virtual int set(int k, int v);
	virtual int search(int k, int *v);
	virtual int delete_key(int k);
	virtual void print_ht(void);
	virtual void print_ht_info(void);
	int get_ht_size(void) {return M;}
	float get_load_factor(void);

private:
	virtual int resize(void);
	int set_1(entry *H, int &empty_slot, int &n, int &tomb_num, int k, int v);
	entry *H;
	int M, empty_slot, tomb_num;
	//M: actual number of elements, N: address space of the hash function
	//n: number of entries, tomb_num: number of tombstones (invalid entries in the middle of the chain)
	//n + tomb_num <= M
};

#endif
