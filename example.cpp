#include <iostream>

#include "getoptpp.hpp"

int main(int argc, char* argv[]) {
	int verbose_flag;

	dg::Options opt;
	opt.add("verbose", no_argument, &verbose_flag, 1, [&]() {
		std::cout << "verbose: " << verbose_flag << "\n";
		return 0;
	});
	opt.add("brief", no_argument, &verbose_flag, 0, [&]() {
		std::cout << "brief: " << verbose_flag << "\n";
		return 0;
	});
	opt.add("add", no_argument, 'a', [&]() {
		std::cout << "add\n";
		return 0;
	});
	opt.add("append", no_argument, 'b', [&]() {
		std::cout << "append\n";
		return 0;
	});
	opt.add("delete", required_argument, 'd', [&]() {
		std::cout << "delete: " << optarg << "\n";
		return 0;
	});
	opt.add("create", required_argument, 'c', [&]() {
		std::cout << "create: " << optarg << "\n";
		return 0;
	});
	opt.add("file", required_argument, 'f', [&]() {
		std::cout << "file: " << optarg << "\n";
		return 0;
	});
	opt.add("help", no_argument, '?', [&]() {
		std::cout << "usage stuff\n";
		return 0;
	});

	opt.process(argc, argv);
	for(auto const &arg : opt.arguments())
	{
		std::cout << arg << std::endl;
	}
}
