#pragma once
#include <iostream>
#include <cassert>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <getopt.h>

namespace dg {

using Handle = std::function<void()>;

class Options {
	public:
		/// @param name		name of the option
		/// @param has_arg	kind of arguments that are used (no_argument, required_argument, optional_argument)
		/// @param val		identifies the option (see getopt documentation)
		/// @param handle	lambda that is invoked when the option occures
		void add(std::string const & name, int has_arg, int val, Handle handle) {
			add(name, has_arg, nullptr, val, handle);
		}

		/// @param name		name of the option
		/// @param has_arg	kind of arguments that are used (no_argument, required_argument, optional_argument)
		/// @param flag		pointer that is set after the option occured
		/// @param val		value for the flag to be set
		/// @param handle	lambda that is invoked when the option occures
		void add(std::string const & name, int has_arg, int* flag, int val, Handle handle) {
			// create a new option from the args
			options.push_back({name.data(), has_arg, flag, val});

			int index = val; // let val be the option's unique identifier
			if (flag != nullptr) {
				// flag is not null, so the val is not used as identifier
				// so pick another one
				index = num_flags++;
			}
			// store the handle
			handles[index] = handle;
		}

		bool process(int argc, char* argv[]) {
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
				int index = 0;
				int key = getopt_long(argc, argv, shortopts.data(), options.data(), &index);

				if (key == -1) {
					break;
				} else if (key == '?') {
					return false;
				} else if (key == ':') {
					return false;
				} else if (key == 0) {
					// call flag's handle
					handles.at(index)();
				} else {
					// call option's handle
					handles.at(key)();
				}
			}
			for (int i = optind; i < argc; ++i) {
				args.push_back(argv[i]);
			}

			// remove terminating option
			options.pop_back();
			assert(options.size() == handles.size());
			return true;
		}

		const std::vector<std::string> arguments() const { return args; }

	private:
		std::size_t num_flags{};
		std::vector<option> options{};
		std::vector<std::string> args;
		std::unordered_map<int, Handle> handles{};
};

}
