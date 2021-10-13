#include <set>
#include <map>

#include "parser.hpp"
#include "log.hpp"
#include "err_codes.h"
#include "codegen.hpp"

#define ERR_PREFIX "parser -> "

parser::parser(std::string output_filename_, uint8_t optimization_level_) noexcept:
output_filename(output_filename_), optimization_level(optimization_level_){
    if(!optimization_level)
        stdlog() << "Skipping optimizations" << std::endl;
}

parser::~parser() noexcept{
    //
}

std::map<char, bf_instr> token_map = {
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
    size_t location = 0;
    root_node = std::make_unique<asc_node>(no_op, location);
    asc_node* next_node = root_node.get();

    size_t unmatched_loop = 0;
    char token;

    while((token = input_stream->get()) != -1){
        auto bf_instr_iter = token_map.find(token);
        if(bf_instr_iter != token_map.end()) {
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
            next_node->next = std::make_unique<asc_node>(bf_instr_iter->second, ++location);
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

std::map<bf_instr, std::string> reverse_token_map = {
    {inc_ptr, ">"},
    {dec_ptr, "<"},
    {inc_val, "+"},
    {dec_val, "-"},
    {loop_start, "["},
    {loop_end, "]"},
    {put_char, "."},
    {get_char, ","},
    {zero_assign, ":= 0"},
    {zero_scan_left, "scan_left"},
    {zero_scan_right, "scan_right"}
};

void parser::dump_ir(std::ostream* output_stream){
    asc_node* next_node = root_node->next.get();
    while(next_node){
        *output_stream << " " << reverse_token_map[next_node->node_type] << "*" << next_node->iterations << " ";
        next_node = next_node->next.get();
    }
}

void parser::contract_repeating_nodes(){
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

            while(target_node->node_type == loop_start || target_node->node_type == loop_end){
                target_node = target_node->next.get();
                if(!target_node) break;
                target_node_type = target_node->node_type;
            }
            if(!target_node) break;
        }
        next_node = target_node->next.get();
    }
}

inline bool reduce_instr_triple(asc_node** prior_node, asc_node** current_node, asc_node** next_node, bf_instr reductant){
    (*prior_node)->node_type = reductant;
    (*prior_node)->next = std::move((*next_node)->next);
    (*current_node) = (*prior_node)->next.get();
    if(!(*current_node)) {
        //stdlog.err() << ERR_PREFIX << "Nullptr current node with prior location " << (*prior_node)->location << std::endl;
        //throw PARSE_ERR;
        return false;
    };
    (*next_node) = (*current_node)->next.get();
    if(!(*next_node)) {
        //stdlog.err() << ERR_PREFIX << "Nullptr next node at current location "  << (*current_node)->location << std::endl;
        //throw PARSE_ERR;
        return false;
    }
    return true;
}


void parser::zero_loop(){
    asc_node* prior_node = root_node.get();
    if(!prior_node) {
        stdlog.err() << "Null ASC" << std::endl;
        throw OPTIMIZE_ERR;
    }
    asc_node* current_node = prior_node->next.get();
    if(!current_node) return;
    asc_node* next_node = current_node->next.get();
    asc_node* node_buffer;
    bool end_of_chain = false;
    size_t jump_size = 0;
    while(next_node){
        //zero loop optimizations
        if(prior_node->node_type == loop_start && next_node->node_type == loop_end){
            switch(current_node->node_type){
                case dec_val:
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_assign);
                break;

                case inc_val:
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_assign);
                break;

                case dec_ptr:
                jump_size = current_node->iterations;
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_scan_left);
                prior_node->iterations = jump_size;
                break;

                case inc_ptr:
                jump_size = current_node->iterations;
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_scan_right);
                prior_node->iterations = jump_size;
                break;
                
                default:
                prior_node = current_node;
                current_node = next_node;
                next_node = current_node->next.get();
                break;
            }
            if(end_of_chain){
                end_of_chain = false;
                break;
            }
        }
        else {
            prior_node = current_node;
            current_node = next_node;
            next_node = current_node->next.get();
        }
    }
}

void parser::optimize_asc(){
    if(optimization_level > 0) contract_repeating_nodes();
    if(optimization_level > 1) zero_loop();
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

        case target_rust:
        backend = std::make_unique<codegen_rust>(loop_stack);
        break;

        case target_ada:
        backend = std::make_unique<codegen_ada>(loop_stack, output_filename);
        break;

        case target_fortran:
        backend = std::make_unique<codegen_fortran>(loop_stack, output_filename);
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