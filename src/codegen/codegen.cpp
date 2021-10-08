#include "codegen.hpp"
#include "../err_codes.h"
#include "../log.hpp"

codegen::codegen(std::deque<size_t> loop_stack_) noexcept:
loop_stack(loop_stack_){
    //
}

codegen::~codegen() noexcept{
    //
}