#include "unit_test.h"
#include "complete_test.h"

using namespace std;

int main(int argc, char *argv[])
{
	string r;
	r.assign(argv[1]);
	bool gen_case = value((double)atoi(argv[2]), 0, 1);

	complete_test(r, gen_case); //test the whole process in my project

	/*
	for(int i = 0; i < 10; i++){

		test_generator_1(test_name("1", i), test_name("2", i));
	}
	*/

	//basic_test_all("test_1_4.txt");

	//simple_test();

	return 0;
}
