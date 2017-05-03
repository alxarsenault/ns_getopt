#include <ns_getopt.h>

struct global_options {
	bool has_color = false;
};

struct print_options {
	std::string str;
	int n = 1;
};

struct list_options {
	bool m = false;
	bool f = false;
};

void cmd_list(opt::CommandArgs cmd_arg, global_options& g_opts);
void cmd_print(opt::CommandArgs cmd_arg, global_options& g_opts);

int main(int argc, char* argv[]) {

	global_options g_opts;

	std::vector<opt::argument> opt_args = {
		{"color", opt::type::no_arg, [&g_opts]() { g_opts.has_color = true; }, "Print with color.", 'c'},
		{"list", opt::type::cmd_arg, [&g_opts](opt::CommandArgs cmd_arg) { cmd_list(cmd_arg, g_opts); }, "List some stuff."},
		{"print", opt::type::cmd_arg, [&g_opts](opt::CommandArgs cmd_arg) { cmd_print(cmd_arg, g_opts); }, "Print some stuff."}
	};

	opt::options options = {"Multi layer example."
		, ""
		, opt::flag::none
	};

	if(!opt::parse_arguments(argc, argv, opt_args, options)) {
		return -1;
	}

	return 0;
}

void cmd_list(opt::CommandArgs cmd_arg, global_options&) {
	list_options l_opts;

	std::vector<opt::argument> opt_args = {
		{"your_mom", opt::type::no_arg, [&l_opts]() { l_opts.m = true; }, "Print your mom.", 'm'},
		{"your_face", opt::type::no_arg, [&l_opts]() { l_opts.f = true; }, "Print your face.", 'f'}
	};

	opt::options options = {"Multi layer example."
		, ""
		, opt::flag::arguments_are_optional
	};

	if(!opt::parse_arguments(cmd_arg.argc, cmd_arg.argv, opt_args, options)) {
		return;
	}

	if(l_opts.m) {
		std::cout << "Your mom." << std::endl;
	} else if(l_opts.f) {
		std::cout << "Your face." << std::endl;
	} else {
		std::cout << "Nothing to list." << std::endl;
	}
}

void cmd_print(opt::CommandArgs cmd_arg, global_options&) {
	print_options p_opts;

	std::vector<opt::argument> opt_args = {
		{"str", opt::type::raw_arg, [&p_opts](std::string&& s) { p_opts.str = s; }, "Print something n times."},
		/// @todo Could be nice to auto check type.
		{"n_times", opt::type::required_arg, [&p_opts](std::string&& s) { p_opts.n = std::stoi(s); }, "N times.", 'n'}
	};

	opt::options options = {"Multi layer example."
		, ""
		, opt::flag::none
	};

	if(!opt::parse_arguments(cmd_arg.argc, cmd_arg.argv, opt_args, options)) {
		return;
	}

	for(int i = 0; i < p_opts.n; i++) {
		std::cout << p_opts.str << std::endl;
	}
}
