#include <iostream>
#include <boost/program_options.hpp>
#include <map>

#include "cli.hpp"
#include "err_codes.h"
#include "log.hpp"

namespace po = boost::program_options;

std::map<std::string, compile_target> target_from_str = {
    {"js", target_js},
    {"c", target_c},
    {"ada", target_ada},
    {"rust", target_rust},
    {"fortran", target_fortran}
};

cli_args::cli_args():
verbose(false),
dump_ir(false),
optimization_level(3),
target(target_c),
source_filename(),
output_filename("out"){
    //
}

cli_args::~cli_args(){
    //
}

cli_args parse_args(int argc, char* argv[]){
    cli_args retval;
    //options
    po::options_description desc("\nBFT - a multi-target optimizing brainf*ck transpiler");
    desc.add_options()
        ("help,h", "show this help message")
        ("verbose,v", "increase verbosity")
        ("dump_ir,d", "dump the ASC IR to the output file (overrides target)")
        ("optimize,O", po::value<int>(), "optimization level (0 - 3)")
        ("output,o", po::value<std::string>(), "output asm file path")
        ("target,t", po::value<std::string>(), "the target language (js, c, rust, c++)")
        ("source,s", po::value<std::string>(), "source bf file path")
    ;

    po::positional_options_description pos_desc;
    pos_desc.add("source", -1);

    po::variables_map vm;
    try{
        po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
        po::notify(vm);
    } catch(po::error& e){
        stdlog.err() << "Invalid command line arguments - " << e.what() << std::endl;
        throw CLI_INVALID_ARGS_ERR;
    }

    if(vm.size() == 0){
        stdlog.err() << "Invalid arguments" << std::endl;
        std::cout << desc << std::endl;
        throw CLI_INVALID_ARGS_ERR;
    }

    if (vm.count("help")){
        std::cout << desc << std::endl;
        throw CLI_INVALID_ARGS_ERR;
    }

    retval.verbose = vm.count("verbose");
    retval.dump_ir = vm.count("dump_ir");

    if(vm.count("source")){
        retval.source_filename = vm["source"].as<std::string>();
    }
    else{
        stdlog.err() << "A source file must be provided" << std::endl;
        throw CLI_INVALID_ARGS_ERR;
    }

    if(vm.count("target")){
        auto target_iter = target_from_str.find(vm["target"].as<std::string>());
        if(target_iter == target_from_str.end()){
            stdlog.err() << "Invalid compilation target" << std::endl;
            std::cout << desc << std::endl;
            throw CLI_INVALID_ARGS_ERR;
        }
        retval.target = target_iter->second;
    }

    if(vm.count("optimize")){
        int opt_level = vm["optimize"].as<int>();
        if(opt_level > 3) {
            stdlog.err() << "Invalid optimization level (accept 0 - 3, got " << opt_level << ")" << std::endl;
            std::cout << desc << std::endl;
            throw CLI_INVALID_ARGS_ERR;
        }
        retval.optimization_level = opt_level;
    }

    if(vm.count("output")){
        retval.output_filename = vm["output"].as<std::string>();
    }

    return retval;
}