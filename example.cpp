#include <iostream>

#include "getoptpp.hpp"

int main(int argc, char* argv[]) {
	int verbose_flag;

	dg::Options opt;
	opt.add("verbose", no_argument, &verbose_flag, 1,
	        "Be verbose.",
	        [&]() {
		        std::cout << "verbose: " << verbose_flag << "\n";
		        return 0;
	        });
	opt.add("brief", no_argument, &verbose_flag, 0,
	        "Be brief.",
	        [&]() {
		        std::cout << "brief: " << verbose_flag << "\n";
		        return 0;
	        });
	opt.add("add", no_argument, 'a',
	        "Add an entry.",
	        [&]() {
		        std::cout << "add\n";
		        return 0;
	        });
	opt.add("append", no_argument, 'b',
	        "Append an entry.",
	        [&]() {
		        std::cout << "append\n";
		        return 0;
	        });
	opt.add("delete", optional_argument, 'd',
	        "Delete an entry.",
	        [&]() {
		        std::cout << "delete: " << optarg << "\n";
		        return 0;
	        });
	opt.add("create", required_argument, 'c',
	        "Create an entry.",
	        [&]() {
		        std::cout << "create: " << optarg << "\n";
		        return 0;
	        });
	opt.add("file", required_argument, 'f',
	        "Some file argument.",
	        [&]() {
		        std::cout << "file: " << optarg << "\n";
		        return 0;
	        });
	opt.add("help", no_argument, 'h',
	        "Print this help text.",
	        [&]() {
		        std::cout << "usage stuff\n";
		        opt.help();
		        return 0;
	        });

	opt.process(argc, argv);
	for(auto const &arg : opt.arguments())
	{
		std::cout << arg << std::endl;
	}
}
