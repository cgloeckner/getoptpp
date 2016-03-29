#pragma once
#include <cassert>
#include <functional>
#include <vector>
#include <unordered_map>
#include <getopt.h>

namespace dg {

using Handle = std::function<void()>;

class Options {
	public:
		Options();
		
		void add(std::string const & name, int has_arg, int val, Handle handle);
		void add(std::string const & name, int has_arg, int* flag, int val, Handle handle);
		
		void process(int argc, char* argv[]);
		
	private:
		std::size_t num_flags;
		std::vector<option> options;
		std::unordered_map<int, Handle> handles;
};

Options::Options()
	: num_flags{}
	, options{}
	, handles{} {
}

void Options::add(std::string const & name, int has_arg, int val, Handle handle) {
	add(name, has_arg, nullptr, val, handle);
}

void Options::add(std::string const & name, int has_arg, int* flag, int val, Handle handle) {
	options.emplace_back();
	auto& option = options.back();
	option.name = name.c_str();
	option.has_arg = has_arg;
	option.flag = flag;
	option.val = val;
	
	int index = val;
	if (flag != nullptr) {
		index = num_flags++;
	}
	handles[index] = handle;
}

void Options::process(int argc, char* argv[]) {
	std::string shortopts;
	for (auto const & option: options) {
		if (option.flag != nullptr) {
			continue;
		}
		shortopts += static_cast<char>(option.val);
		
		switch (option.has_arg) {
			case no_argument:
				break;
			case required_argument:
				shortopts += ":";
				break;
			case optional_argument:
				shortopts += "::";
				break;
		}
	}
	
	// add termination option
	options.push_back({0, 0, 0, 0});
	
	// handle arguments
	while (true) {
		int index{0};
		int key = getopt_long(argc, argv, shortopts.c_str(), options.data(), &index);
		
		if (key == -1) {
			break;
		} else if (key == 0) {
			// call flag's handle
			handles.at(index)();
		} else {
			// call option's handle
			handles.at(key)();
		}
	}
	
	// remove terminating option
	options.pop_back();
	assert(options.size() == handles.size());
}

}
