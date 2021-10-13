#include "./fortran.hpp"
#include "../log.hpp"
#include "../err_codes.h"

std::vector<std::string> codegen_fortran::skeleton = {
    "program ",
	R"(
implicit none
	integer, dimension(0:29999) :: bf_array = 0
	integer :: bf_ptr = 0
	character :: input_char_buffer = 'a'
)",
    R"(
end program )"
};

codegen_fortran::codegen_fortran(std::deque<size_t> loop_stack_, std::string output_filename_) noexcept:
codegen(loop_stack_), output_filename(output_filename_) {
    indent_level = 1;
	auto output_prefix_iter = output_filename.find('.');
	if(output_prefix_iter != std::string::npos)
		output_filename = output_filename.substr(0, output_prefix_iter);
}

codegen_fortran::~codegen_fortran() noexcept{
    //
}

void codegen_fortran::dump_code(std::ostream* code_output) const noexcept {
    *code_output << skeleton[0] << output_filename << skeleton[1];
    *code_output << code_stream.rdbuf();
    *code_output << skeleton[2] << output_filename;
}

std::string codegen_fortran::dump_code_string() const noexcept {
    std::stringstream code_output;
    code_output << skeleton[0] << output_filename << skeleton[1];
    code_output << code_stream.rdbuf();
    code_output << skeleton[2] << output_filename;
    return code_output.str();
}

void codegen_fortran::generate(asc_node* node_) {
    switch(node_->node_type){
        case inc_ptr:
        code_stream_fmt() << "bf_ptr = bf_ptr+" << node_->iterations << "\n";
        break;

		case dec_ptr:
		code_stream_fmt() << "bf_ptr = bf_ptr-" << node_->iterations << "\n";
		break;

		case inc_val:
		code_stream_fmt() << "bf_array(bf_ptr) = bf_array(bf_ptr)+" << node_->iterations << "\n";
		break;

		case dec_val:
		code_stream_fmt() << "bf_array(bf_ptr) = bf_array(bf_ptr)-" << node_->iterations << "\n";
		break;

		case loop_start:
		if(open_loop_stack.size() >= open_loop_stack.max_size()){
			stdlog.err() << "Deque stack passed max size (" << open_loop_stack.max_size() << ")" << std::endl;
			throw MEM_ERR;
		}
		open_loop_stack.push_back(loop_stack.back());
		loop_stack.pop_back();
		code_stream_fmt() << "do while (bf_array(bf_ptr) > 0)\n";
		indent_level++;
		break;

		case loop_end:
		indent_level--;
		code_stream_fmt() << "end do\n";
		open_loop_stack.pop_back();
		break;

		case put_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "write(*, fmt=\"(a)\", advance=\"no\") char(bf_array(bf_ptr))\n";
		break;

		case get_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "read(*, '(a)') input_char_buffer\n"
			<< "bf_array(bf_ptr) = iachar(input_char_buffer)\n";
		break;

		case zero_assign:
		code_stream_fmt() << "bf_array(bf_ptr) = 0\n";
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

