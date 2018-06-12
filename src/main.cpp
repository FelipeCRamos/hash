#include <iostream>
#include "hashtable.hpp"

int main(void){
	HashTbl<int, int> test(11);

	for( int i = 1; i < 100; i++ ){
		test.insert( (i - 3 * (i % 19)) * (5 % i), 3 * (7 % i) );
	}

	test.print();

	std::cout << "stub" << std::endl;
	return 0;
}
