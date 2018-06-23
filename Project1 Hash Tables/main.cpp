#include "unit_test.h"
#include "complete_test.h"

using namespace std;

int main(int argc, char *argv[])
{
	string r;
	r.assign(argv[1]);
	bool gen_case = value((double)atoi(argv[2]), 0, 1);

	complete_test(r, gen_case); //test the whole process in my project

	return 0;
}
