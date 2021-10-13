#include "asc.hpp"

asc_node::asc_node(bf_instr node_type_, size_t location_):
node_type(node_type_),
next(nullptr),
iterations(1),
location(location_){
    //
}

asc_node::~asc_node(){
    //
}

asc_node loop_start_dummy = asc_node(loop_start, 0);
asc_node loop_end_dummy = asc_node(loop_end, 0);
asc_node dec_ptr_dummy = asc_node(dec_ptr, 0);
asc_node inc_ptr_dummy = asc_node(inc_ptr, 0);