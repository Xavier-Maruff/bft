#include "rust.hpp"
#include "../log.hpp"
#include "../err_codes.h"

std::pair<std::string, std::string> codegen_rust::skeleton = {
    R"(
fn main() {
	let mut bf_array: [i32; 30000] = [0; 30000];
	let mut bf_ptr: usize = 0;
	unsafe {
)",
    R"(
	}
}
	)"
};

codegen_rust::codegen_rust(std::deque<size_t> loop_stack_) noexcept:
codegen(loop_stack_) {
    indent_level = 2;
}

codegen_rust::~codegen_rust() noexcept{
    //
}

void codegen_rust::dump_code(std::ostream* code_output) const noexcept {
    *code_output << skeleton.first;
    *code_output << code_stream.rdbuf();
    *code_output << skeleton.second;
}

std::string codegen_rust::dump_code_string() const noexcept {
    std::stringstream code_output;
    code_output << skeleton.first;
    code_output << code_stream.rdbuf();
    code_output << skeleton.second;
    return code_output.str();
}

void codegen_rust::generate(asc_node* node_) {
    switch(node_->node_type){
        case inc_ptr:
        code_stream_fmt() << "bf_ptr += " << node_->iterations << ";\n";
        break;

		case dec_ptr:
		code_stream_fmt() << "bf_ptr -= " << node_->iterations << ";\n";
		break;

		case inc_val:
		code_stream_fmt() << "bf_array[bf_ptr] += " << node_->iterations << ";\n";
		break;

		case dec_val:
		code_stream_fmt() << "bf_array[bf_ptr] -= " << node_->iterations << ";\n";
		break;

		case loop_start:
		if(open_loop_stack.size() >= open_loop_stack.max_size()){
			stdlog.err() << "Deque stack passed max size (" << open_loop_stack.max_size() << ")" << std::endl;
			throw MEM_ERR;
		}
		open_loop_stack.push_back(loop_stack.back());
		loop_stack.pop_back();
		code_stream_fmt() << "while bf_array[bf_ptr] > 0 {\n";
		indent_level++;
		break;

		case loop_end:
		indent_level--;
		code_stream_fmt() << "}\n";
		open_loop_stack.pop_back();
		break;

		case put_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "libc::putchar(bf_array[bf_ptr]);\n";
		break;

		case get_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "bf_array[bf_ptr] = libc::getchar();\n";
		break;

		case zero_assign:
		code_stream_fmt() << "bf_array[bf_ptr] = 0;\n";
		break;

		case zero_scan_left:
		//TODO: actually implement this
		generate(&loop_start_dummy);
		dec_ptr_dummy.iterations = node_->iterations;		
		generate(&dec_ptr_dummy);
		generate(&loop_end_dummy);
		break;

		case zero_scan_right:
		//TODO: actually implement this
		generate(&loop_start_dummy);
		inc_ptr_dummy.iterations = node_->iterations;
		generate(&inc_ptr_dummy);
		generate(&loop_end_dummy);
		break;

		default:
		break;
    }
}

