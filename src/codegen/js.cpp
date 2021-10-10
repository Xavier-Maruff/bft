#include "js.hpp"
#include "../log.hpp"
#include "../err_codes.h"

#define BF_ARRAY_SIZE 30000

std::pair<std::string, std::string> codegen_js::skeleton = {
	R"(const bf_array_len = )"+std::to_string(BF_ARRAY_SIZE)+R"(
const readline = require('readline');

const getchar = () => {
    const rl = readline.createInterface({
        input: process.stdin,
        output: undefined,
    });

    return new Promise(resolve => rl.question("", ans => {
        rl.close();
        resolve(ans.charCodeAt(0));
    }))
}

const putchar = (char, iters = 1) => {
	process.stdout.write(String.fromCharCode(char).repeat(iters));
}


const main = async () => {
	let bf_ptr = bf_array_len;
	const bf_array = new Array(bf_array_len);
	while(--bf_ptr) bf_array[bf_ptr] = 0;
	bf_array[0] = 0;
)",
	R"(
}

main();
)"
};

codegen_js::codegen_js(std::deque<size_t> loop_stack_) noexcept:
codegen(loop_stack_){
    indent_level = 1;
}

codegen_js::~codegen_js() noexcept{
    //
}

void codegen_js::dump_code(std::ostream* code_output) const noexcept{
    *code_output << skeleton.first;
    *code_output << code_stream.rdbuf();
    *code_output << skeleton.second;
}

std::string codegen_js::dump_code_string() const noexcept{
	std::stringstream code_output;
    code_output << skeleton.first;
    code_output << code_stream.rdbuf();
    code_output << skeleton.second;
    return code_output.str();
}

void codegen_js::generate(asc_node* node_){
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
		code_stream_fmt() << "while(bf_array[bf_ptr]){\n";
		indent_level++;
		break;

		case loop_end:
		indent_level--;
		code_stream_fmt() << "}\n";
		open_loop_stack.pop_back();
		break;

		case put_char:
		code_stream_fmt() << "putchar(bf_array[bf_ptr], " << node_->iterations << ");\n";
		break;

		case get_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "bf_array[bf_ptr] = await getchar();\n";
		break;

		case zero_assign:
		code_stream_fmt() << "bf_array[bf_ptr] = 0;\n";
		break;

		default:
		break;
    }
}