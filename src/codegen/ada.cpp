#include "ada.hpp"
#include "../log.hpp"
#include "../err_codes.h"

std::vector<std::string> codegen_ada::skeleton = {
    R"(
with Ada.Text_IO; use Ada.Text_IO;

procedure )",
R"( is
   type Bf_Array_Index is range 0 .. 29999;
   type Bf_Array_Type is array (Bf_Array_Index) of Integer;
   Bf_Array : Bf_Array_Type := (others => 0);
   Bf_Ptr: Bf_Array_Index := 0;
   Input_Char_Buffer: Character := 'a';
begin
)",
    R"(
end )",
";"
};

codegen_ada::codegen_ada(std::deque<size_t> loop_stack_, std::string output_filename_) noexcept:
codegen(loop_stack_), output_filename(output_filename_) {
    indent_level = 1;
	auto output_prefix_iter = output_filename.find('.');
	if(output_prefix_iter != std::string::npos)
		output_filename = output_filename.substr(0, output_prefix_iter);
}

codegen_ada::~codegen_ada() noexcept{
    //
}

void codegen_ada::dump_code(std::ostream* code_output) const noexcept {
    *code_output << skeleton[0] << output_filename << skeleton[1];
    *code_output << code_stream.rdbuf();
    *code_output << skeleton[2] << output_filename << skeleton[3];
}

std::string codegen_ada::dump_code_string() const noexcept {
    std::stringstream code_output;
    code_output << skeleton[0] << output_filename << skeleton[1];
    code_output << code_stream.rdbuf();
    code_output << skeleton[2] << output_filename << skeleton[3];
    return code_output.str();
}

void codegen_ada::generate(asc_node* node_) {
    switch(node_->node_type){
        case inc_ptr:
        code_stream_fmt() << "Bf_Ptr := Bf_Ptr+" << node_->iterations << ";\n";
        break;

		case dec_ptr:
		code_stream_fmt() << "Bf_Ptr := Bf_Ptr-" << node_->iterations << ";\n";
		break;

		case inc_val:
		code_stream_fmt() << "Bf_Array(Bf_Ptr) := Bf_Array(Bf_Ptr)+" << node_->iterations << ";\n";
		break;

		case dec_val:
		code_stream_fmt() << "Bf_Array(Bf_Ptr) := Bf_Array(Bf_Ptr)-" << node_->iterations << ";\n";
		break;

		case loop_start:
		if(open_loop_stack.size() >= open_loop_stack.max_size()){
			stdlog.err() << "Deque stack passed max size (" << open_loop_stack.max_size() << ")" << std::endl;
			throw MEM_ERR;
		}
		open_loop_stack.push_back(loop_stack.back());
		loop_stack.pop_back();
		code_stream_fmt() << "while Bf_Array(Bf_Ptr) > 0 loop\n";
		indent_level++;
		break;

		case loop_end:
		indent_level--;
		code_stream_fmt() << "end loop;\n";
		open_loop_stack.pop_back();
		break;

		case put_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "Put(Character'Val(Bf_Array(Bf_Ptr)));\n";
		break;

		case get_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "Get(Input_Char_Buffer);\n"
			<< "Bf_Array(Bf_Ptr) := Character'Pos(Input_Char_Buffer);";
		break;

		default:
		break;
    }
}

