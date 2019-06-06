#pragma once
#include <iostream>
#include <cassert>
#include <functional>
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>
#include <cstring>
#include <getopt.h>

namespace dg {

using Handle = std::function<int()>;

class Options {
	public:
		/// @param name		name of the option
		/// @param has_arg	kind of arguments that are used (no_argument, required_argument, optional_argument)
		/// @param val		identifies the option (see getopt documentation)
		/// @param help_text	help text for this option
		/// @param handle	lambda that is invoked when the option occures
		void add(std::string const & name, int has_arg, int val, std::string const & help_text, Handle handle) {
			add(name, has_arg, nullptr, val, help_text, handle);
		}

		/// @param name		name of the option
		/// @param has_arg	kind of arguments that are used (no_argument, required_argument, optional_argument)
		/// @param flag		pointer that is set after the option occured
		/// @param val		value for the flag to be set
		/// @param help_text	help text for this option
		/// @param handle	lambda that is invoked when the option occures
		void add(std::string const & name, int has_arg, int* flag, int val, std::string const & help_text, Handle handle) {
			names.push_back(name);
			// create a new option from the args
			options.push_back({names.back().data(), has_arg, flag, val});
			help_texts.push_back(help_text);

			int index = val; // let val be the option's unique identifier
			if (flag != nullptr) {
				// flag is not null, so the val is not used as identifier
				// so pick another one
				index = num_flags++;
			}
			// store the handle
			handles[index] = handle;
		}

		int process(int argc, char* argv[]) {
			int ret = 0;
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
					return 1;
				} else if (key == ':') {
					return 1;
				} else if (key == 0) {
					// call flag's handle
					ret = handles.at(index)();
				} else {
					// call option's handle
					ret = handles.at(key)();
				}

				if(ret) {
					return ret;
				}
			}

			for (int i = optind; i < argc; ++i) {
				args.push_back(argv[i]);
			}

			// remove terminating option
			options.pop_back();
			assert(options.size() == handles.size());
			assert(options.size() == help_texts.size());
			return 0;
		}

		const std::vector<std::string> arguments() const { return args; }

		void help() {
			std::size_t width = 0;
			for(auto const & opt : options) {
				if(opt.name == nullptr) {
					continue; // skip terminating option
				}
				width = std::max(width, std::strlen(opt.name) + (opt.has_arg != no_argument ? 0 : 3));
			}
			width += 1;

			int index = 0;
			for(auto const & help_text : help_texts) {
				if(index >= (int)options.size()) {
					break;
				}
				auto const & opt = options.at(index);
				std::string args;
				switch(opt.has_arg) {
				case required_argument:
					args = "<x>";
					break;
				case optional_argument:
					args = "[x]";
					break;
				case no_argument:
				default:
					break;
				}

				std::string padding;
				std::size_t pad_size = width - (std::strlen(opt.name) + args.size());
				padding.append(pad_size, ' ');
				if(opt.val >= 33 && opt.val <= 126)
				{
					std::cout << "  -" << static_cast<char>(opt.val) << ", --" << opt.name << " " << args << padding;
				}
				else
				{
					std::cout << "      --" << opt.name << " " << args << padding;
				}

				std::cout << help_text << std::endl;
				++index;
			}
		}

	private:
		std::size_t num_flags{};
		std::vector<option> options{};
		std::deque<std::string> names{};
		std::vector<std::string> help_texts{};
		std::vector<std::string> args;
		std::unordered_map<int, Handle> handles{};
};

}
