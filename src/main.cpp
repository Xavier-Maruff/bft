#include <iostream>
#include <fstream>

#include "parser.hpp"
#include "log.hpp"
#include "cli.hpp"
#include "err_codes.h"


int main(int argc, char* argv[]){
    try{
        cli_args args = parse_args(argc, argv);
        stdlog.is_verbose = args.verbose;

        std::ifstream bf_source_file(args.source_filename);
        if(!bf_source_file.is_open()){
            stdlog.err() << "Could not open source file \"" << args.source_filename << "\"" << std::endl;
            throw IO_ERR;
        }

        parser bf_parser;

        bf_parser.tokenize(&bf_source_file);
        bf_source_file.close();

        if(args.optimize) bf_parser.optimize_asc();

        std::ofstream output_file(args.output_filename);
        if(!output_file.is_open()){
            stdlog.err() << "Could not create output file \"" << args.output_filename << "\"" << std::endl;
            throw IO_ERR;
        }

        bf_parser.generate_code(args.target, &output_file);
        output_file.close();

        return EXIT_SUCCESS;
    } catch(const char* err_msg){
        //stdlog.err() << "\"" << err_msg << "\" was thrown" << std::endl;
        return EXIT_FAILURE;
    };
}