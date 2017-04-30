#include <ns_getopt.h>
#include <stdio.h>

std::size_t total_alloc_size = 0;

void* operator new(std::size_t length)
{
	total_alloc_size += length;
	return malloc(length);
}

void my_method(opt::string_view&& arg) {
	std::cout << arg << std::endl;
}

int main(int argc, char* argv[]) {
	//opt::string_view<char> sv(argv[0]);
//	bool do_something = false;

	auto raw_fun = [](opt::string_view&& s) {
		std::cout << "Raw arg : " << s << std::endl;
	};

	auto vec_fun = [](std::vector<opt::string_view>&& v) {
		for (const auto& x : v) {
			std::cout << x << std::endl;
		}
	};

	opt::argument args[] = {
		{"test", opt::type::no_arg,
				[](){ std::cout << "t" << std::endl; },
				"This is a simple flag.", 't'}
		, {"M", opt::type::no_arg,
				[](){ std::cout << "m" << std::endl; },
				"This is a simple flag.", 'M'}
		, {"A", opt::type::no_arg,
				[](){ std::cout << "a" << std::endl; },
				"This is a simple flag.", 'a'}
		, {"requiredarg", opt::type::required_arg,
			[](opt::string_view&& s){ std::cout << s << std::endl; },
				"This argument requires a value."}
		, {"optional", opt::type::optional_arg,
				my_method,
				"This parameter has an optional argument.", 'o'}
		, {"default", opt::type::default_arg,
				my_method,
				"An example of an argument with default value.", 'd',
				"my_default_val"}
		, {"multi", opt::type::multi_arg, vec_fun,
				"This accepts multiple, space seperated, values.\n"
				"You can also have long descriptions that get\n"
				"automatically aligned simply by using \\n in\n"
				"your description.", 'm'}
		, {"in_file", opt::type::raw_arg, raw_fun,
				"Description for file 1.\nIt can be multiple\nlines too."}
		, {"out_file", opt::type::raw_arg, raw_fun,
				"Description for out_file. Raw arguments are optional."}
	};

	opt::options o = {"A wonderful example.\nTalented Author\n"
			, "More info on github.\n", opt::DEFAULT_FLAGS
			, [](opt::string_view&& s) { std::cout << s << std::endl; }
	};

	std::cout << "Total allocation size : " << total_alloc_size << std::endl;
	total_alloc_size = 0;
	bool succeeded = opt::parse_arguments(argc, argv, args, o);

	/* You can print the help whenever you desire. */
	//opt::print_help(args, o, argv[0]);
	std::cout << "Total allocation size : " << total_alloc_size << std::endl;

	if (!succeeded)
		return -1;

	std::cout << "Parsing arguments succeeded!" << std::endl;
	return 0;
}
