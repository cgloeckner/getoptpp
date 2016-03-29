#include <iostream>

#include "getoptpp.hpp"

int main(int argc, char* argv[]) {
	int verbose_flag;
	
	gopt::Options opt;
	opt.add("verbose", no_argument, &verbose_flag, 1, [&]() {
		std::cout << "verbose: " << verbose_flag << "\n";
	});
	opt.add("brief", no_argument, &verbose_flag, 0, [&]() {
		std::cout << "brief: " << verbose_flag << "\n";
	});
	opt.add("add", no_argument, 'a', [&]() {
		std::cout << "add\n";
	});
	opt.add("append", no_argument, 'b', [&]() {
		std::cout << "append\n";
	});
	opt.add("delete", required_argument, 'd', [&]() {
		std::cout << "delete: " << optarg << "\n";
	});
	opt.add("create", required_argument, 'c', [&]() {
		std::cout << "create: " << optarg << "\n";
	});
	opt.add("file", required_argument, 'f', [&]() {
		std::cout << "file: " << optarg << "\n";
	});
	opt.add("help", no_argument, '?', [&]() {
		std::cout << "usage stuff\n";
	});
	
	opt.process(argc, argv);
}
