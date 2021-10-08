#include "asc.hpp"

asc_node::asc_node(bf_instr node_type_):
node_type(node_type_),
next(nullptr),
iterations(1){
    //
}

asc_node::~asc_node(){
    //
}