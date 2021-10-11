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
    zero_scan_right,
    zero_scan_left
};

class asc_node {
    public:
    std::unique_ptr<asc_node> next;
    size_t iterations;
    bf_instr node_type;

    asc_node(bf_instr node_type_);
    ~asc_node();
};

#endif