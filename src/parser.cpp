#include <set>
#include <map>

#include "parser.hpp"
#include "log.hpp"
#include "err_codes.h"
#include "codegen.hpp"
#include "colours.h"

#define ERR_PREFIX "parser -> "

parser::parser(std::string output_filename_, uint8_t optimization_level_) noexcept:
output_filename(output_filename_), optimization_level(optimization_level_){
    if(!optimization_level)
        stdlog() << "Skipping optimizations" << std::endl;
}

parser::~parser() noexcept{
    //
}

//maps token char to bf instruction enum
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

//tokenizes stream of characters
void parser::tokenize(std::istream* input_stream) {
    stdlog() << "Constructing ASC" << std::endl;

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
        stdlog.err() << ERR_PREFIX << "Unexpected loop ending" << std::endl;
        throw PARSE_ERR;
    }
    else if(unmatched_loop > 0){
        stdlog.err() << ERR_PREFIX << "Unmatched loop start" << std::endl;
        throw PARSE_ERR;
    }
}

//maps bf instructions to strings
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

//dumps IR to an output stream
void parser::dump_ir(std::ostream* output_stream){
    if(!output_stream) {
        stdlog.err() << "Could not dump IR - nullptr output stream" << std::endl;
        throw MEM_ERR;
    }
    stdlog() << "Dumping IR" << std::endl;
    asc_node* next_node = root_node->next.get();
    size_t indent_level = 0;
    while(next_node){
        *output_stream << std::string(indent_level, '\t')
        << reverse_token_map[next_node->node_type] << "*" << next_node->iterations << std::endl;
        if(next_node->node_type == loop_start) indent_level++;
        else if(next_node->node_type == loop_end) indent_level--;
        next_node = next_node->next.get();
    }
}

//resolves repeating nodes of the same type into single nodes with >1 iteration values
void parser::contract_repeating_nodes(){
    asc_node* target_node = root_node.get();
    if(!target_node) {
        stdlog.err() << "Null ASC" << std::endl;
        throw OPTIMIZE_ERR;
    }
    asc_node* next_node = target_node->next.get();
    bf_instr target_node_type = target_node->node_type;
    bool opposing_ops = false;
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

//cancels opposing operations, like +*3 -*2 -> +*1
void parser::cancel_opposing_ops(){
    asc_node* prior_node = root_node.get();
    asc_node* target_node = root_node.get();
    if(!target_node) {
        stdlog.err() << "Null ASC" << std::endl;
        throw OPTIMIZE_ERR;
    }
    asc_node* next_node = target_node->next.get();
    bf_instr target_node_type = target_node->node_type;
    bool opposing_ops = false;
    bool null_next = false;
    while(next_node){
        //check if opposing operations (+/-, >/<)
        opposing_ops =
           (target_node->node_type == dec_ptr && next_node->node_type == inc_ptr)
        || (target_node->node_type == inc_ptr && next_node->node_type == dec_ptr)
        || (target_node->node_type == dec_val && next_node->node_type == inc_val)
        || (target_node->node_type == inc_val && next_node->node_type == dec_val);
        if(opposing_ops){
            opposing_ops = false;
            //if the iterations fully cancel, ditch the target and next nodes
            if(target_node->iterations == next_node->iterations){
                if(!next_node->next) break;
                prior_node->next = std::move(next_node->next);
                target_node = prior_node;
                next_node = prior_node->next.get();
            }
            //subtract the iterations
            else if(target_node->iterations > next_node->iterations){
                target_node->iterations -= next_node->iterations;
            }
            //defer to the next node, subtract target iterations
            else {
                target_node->iterations = next_node->iterations-target_node->iterations;
                target_node->location = next_node->location;
                target_node->node_type = next_node->node_type;
            }
            if(!next_node->next) break;
            //ditch the next node
            target_node->next = std::move(next_node->next);
        }

        //move along the asc
        prior_node = target_node;
        target_node = target_node->next.get();
        if(!target_node->next) break;
        next_node = target_node->next.get();
    }
}


//reduces three asc nodes to the first node, with a target type
inline bool reduce_instr_triple(asc_node** prior_node, asc_node** current_node, asc_node** next_node, bf_instr reductant){
    (*prior_node)->node_type = reductant;
    //move next_node->next into prior_node->next
    (*prior_node)->next = std::move((*next_node)->next);
    //current node is next_node->next
    (*current_node) = (*prior_node)->next.get();
    if(!(*current_node)) return false;
    //next node is current_node->next
    (*next_node) = (*current_node)->next.get();
    if(!(*next_node)) return false;
    return true;
}

//flattens zero assignment loops, i.e. [-] or [+] -> *bf_ptr = 0, and zero scan loops (to be implemented)
void parser::zero_loop(){
    asc_node* prior_node = root_node.get();
    if(!prior_node) {
        stdlog.err() << "Null ASC" << std::endl;
        throw OPTIMIZE_ERR;
    }
    asc_node* current_node = prior_node->next.get();
    if(!current_node) return;
    asc_node* next_node = current_node->next.get();
    bool end_of_chain = false;
    //jump size will be utilized when zero scan loop optimization is added
    size_t jump_size = 0;
    while(next_node){
        //if chain context matches three node [*] pattern
        if(prior_node->node_type == loop_start && next_node->node_type == loop_end){
            switch(current_node->node_type){
                //reduce chain context to one zero assign instruction
                case dec_val:
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_assign);
                break;

                //reduce chain context to one zero assign instruction
                case inc_val:
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_assign);
                break;

                //currently not properly implemented in any of the backends, should be soon
                case dec_ptr:
                jump_size = current_node->iterations;
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_scan_left);
                prior_node->iterations = jump_size;
                break;

                //currently not properly implemented in any of the backends, should be soon
                case inc_ptr:
                jump_size = current_node->iterations;
                end_of_chain = reduce_instr_triple(&prior_node, &current_node, &next_node, zero_scan_right);
                prior_node->iterations = jump_size;
                break;
                
                //not a zero loop
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

//run the optimizations
void parser::optimize_asc(){
    if(optimization_level > 0) {
        stdlog() << "Contracting repeating nodes" << std::endl;
        contract_repeating_nodes();
    }
    if(optimization_level > 1){
        stdlog() << "Cancelling opposing operations" << std::endl;
        cancel_opposing_ops();
    }
    if(optimization_level > 2) {
        stdlog() << "Flattening zero assignment loops" << std::endl;
        zero_loop();
    }
}

std::map<compile_target, std::string> target_name_map = {
    {target_c, "c"},
    {target_ada, "ada"},
    {target_fortran, "fortran 90"},
    {target_js, "javascript"},
    {target_rust, "rust"}
};


//pipe asc through a codegen backend
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

    stdlog() << "Piping ASC through " << target_name_map[code_lang] << " backend" << std::endl;
    asc_node* next_node = root_node.get();
    while(next_node){
        //shouldn't be any iters = 0 nodes in the chain, just a failsafe
        if(next_node->iterations > 0)
            backend->generate(next_node);
        next_node = next_node->next.get();
    }
    backend->dump_code(output_stream);
}