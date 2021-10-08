#include "js.hpp"

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

const putchar = (char) => {
	process.stdout.write(String.fromCharCode(char));
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
codegen(loop_stack_), indent_level(1){
    //
}

codegen_js::~codegen_js() noexcept{
    //
}

inline std::ostream& codegen_js::code_stream_fmt() noexcept {
    return code_stream << std::string(indent_level, '\t');
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
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "putchar(bf_array[bf_ptr]);\n";
		break;

		case get_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "bf_array[bf_ptr] = await getchar();\n";
		break;

		default:
		break;
    }
}