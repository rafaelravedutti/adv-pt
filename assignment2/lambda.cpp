#include <iostream>
#include <functional>
#include <algorithm>

int main(int argc, char** argv) {
	// basic lambda function
	if (1) {
    	auto printFct = [] { std::cout << "message" << std::endl; };
    	printFct ();
	}



	// lambda receiving function parameter
	if (1) {
	    auto printFct = [] ( const std::string& toPrint ) { std::cout << toPrint << std::endl; };
	    printFct("message");
	}



	// lambda with capture
	if (1) {
    	std::string toPrint = "message";
    	//auto printFct = [] { std::cout << toPrint << std::endl; };
    	//auto printFct = [&] { std::cout << toPrint << std::endl; };
    	auto printFct = [=] { std::cout << toPrint << std::endl; };
    	toPrint = "reassigned";
    	printFct();
	}



	// lambda with return function
	if (1) {
    	//auto toPrint = [] { "message"; };
    	auto toPrint = [] () -> const std::string { return "message"; };
    	auto printFct = [&] { std::cout << toPrint() << std::endl; };
    	printFct();
    }



    // 'template' lambda
	if (1) {
	    auto printFct = [] ( auto toPrint ) { std::cout << toPrint << std::endl; };
	    printFct("message");
	    printFct(4.2);
    }



}
