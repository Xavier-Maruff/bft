#include <set>
#include <unordered_map>

#include "parser.hpp"
#include "log.hpp"
#include "err_codes.h"
#include "codegen.hpp"

#define ERR_PREFIX "parser -> "

parser::parser() noexcept{
    //
}

parser::~parser() noexcept{
    //
}

std::unordered_map<char, bf_instr> token_map = {
    {'>', inc_ptr},
    {'<', dec_ptr},
    {'+', inc_val},
    {'-', dec_val},
    {'[', loop_start},
    {']', loop_end},
    {'.', put_char},
    {',', get_char},
};

void parser::tokenize(std::istream* input_stream) {
    root_node = std::make_unique<asc_node>(no_op);
    asc_node* next_node = root_node.get();

    size_t unmatched_loop = 0;
    char token;

    while((token = input_stream->get()) != -1){
        auto bf_instr_iter = token_map.find(token);
        if(bf_instr_iter != token_map.end()) {
            //if(bf_instr_iter->second == loop_start)
            //    loop_stack.push_back("loop_"+std::to_string(loop_stack.size()));
            switch(bf_instr_iter->second){
                case loop_start:
                loop_stack.push_back(loop_stack.size());
                unmatched_loop++;
                break;

                case loop_end:
                unmatched_loop--;
                break;

                default:
                break;
            }
            next_node->next = std::make_unique<asc_node>(bf_instr_iter->second);
            next_node = next_node->next.get();
        }
    }

    if(unmatched_loop < 0) {
        stdlog.err() << "Unexpected loop ending" << std::endl;
        throw PARSE_ERR;
    }
    else if(unmatched_loop > 0){
        stdlog.err() << "Unmatched loop start" << std::endl;
        throw PARSE_ERR;
    }
}

//std::set<bf_instr> non_reducables = {loop_start, loop_end, put_char, get_char};

void parser::optimize_asc(){
    //compress repeating asc nodes
    asc_node* target_node = root_node.get();
    if(!target_node) {
        stdlog.err() << "Null ASC" << std::endl;
        throw OPTIMIZE_ERR;
    }
    asc_node* next_node = target_node->next.get();
    bf_instr target_node_type = target_node->node_type;
    while(next_node){
        if(next_node->node_type == target_node_type){
            target_node->iterations++;
            target_node->next = std::move(next_node->next);
        }
        else {
            target_node = target_node->next.get();
            if(!target_node) break;
            target_node_type = target_node->node_type;
            //while(std::find(non_reducables.begin(), non_reducables.end(), target_node_type) != non_reducables.end()){
            //    target_node = target_node->next.get();
            //    if(!target_node) break;
            //    target_node_type = target_node->node_type;
            //}
            //if(!target_node) break;
        }
        next_node = target_node->next.get();
    }
}


void parser::generate_code(compile_target code_lang, std::ostream* output_stream){
    std::unique_ptr<codegen> backend;
    switch(code_lang){
        case target_c:
        backend = std::make_unique<codegen_c>(loop_stack);
        break;

        case target_js:
        backend = std::make_unique<codegen_js>(loop_stack);
        break;

        default:
        stdlog.err() << ERR_PREFIX << "Unrecognised compilation target" << std::endl;
        throw PARSE_ERR;
        break;
    }

    asc_node* next_node = root_node.get();
    while(next_node){
        backend->generate(next_node);
        next_node = next_node->next.get();
    }
    backend->dump_code(output_stream);
}