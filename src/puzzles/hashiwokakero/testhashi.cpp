#include "hashiboard.h"
#include <iostream>
int main( int argc, char *argv[] )
{
	HashiBoard *b = new HashiBoard(argv[1]);
	std::cout << b->AsString();
	std::cout << "**********" << std::endl;
	b->SetEdgeSite(9, 2);
	std::cout << b->AsString();
	std::cout << "**********" << std::endl;
	std::cout << b->ConnectedComponents() << std::endl;
	return 0;
}