#ifndef BFT_ASC
#define BFT_ASC

#include <iostream>
#include <memory>
#include <string>

enum bf_instr {
    no_op,
    inc_ptr,
    dec_ptr,
    inc_val,
    dec_val,
    loop_start,
    loop_end,
    get_char,
    put_char,
    zero_assign,
    zero_scan_left,
    zero_scan_right
};

class asc_node {
    public:
    std::unique_ptr<asc_node> next;
    size_t iterations;
    bf_instr node_type;
    size_t location;

    asc_node(bf_instr node_type_, size_t location_);
    ~asc_node();

    friend inline bool move_node(asc_node* init_node, asc_node* next_node);
};

inline bool move_node(asc_node* init_node, asc_node* next_node){
    if(!init_node || !next_node) return false;

    init_node->location = next_node->location;
    init_node->iterations = next_node->iterations;
    init_node->node_type = next_node->node_type;

    if(!next_node->next) return false;
    init_node->next = std::move(next_node->next);
    
    return true;
}

extern asc_node loop_start_dummy;
extern asc_node loop_end_dummy;
extern asc_node dec_ptr_dummy;
extern asc_node inc_ptr_dummy;

#endif