// Brainlike Interpreter Version 1100
// -DVERSION=1100
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <stack>
#include <random>

template<typename T>
class stack_agc : public std::stack<T> {
public:
	auto& gc() {
		return this->c;
	}
};

std::string read_code_file(std::string filename) {
	std::string program;
	std::string back_s;
	for (size_t i = 0; i < filename.size(); i++) {
		if (filename[i] == '.') {
			back_s = filename.substr(i);
		}
	}
	if (back_s != ".blc") throw std::runtime_error("file type error : not .blc");
	std::ifstream in(filename);
	if (!in.is_open()) {
		throw std::runtime_error("failed to open file: " + filename);
	}
	while (!in.eof()) {
		std::string str;
		in >> str;
		program += str;
	}
	in.close();
	return program;
}

std::unordered_map<int, int> preprocessBrackets(const std::string& program) {
	std::unordered_map<int, int> bracketMap;
	std::stack<int> st;
	for (size_t i = 0; i < program.size(); ++i) {
		if (program[i] == '[') {
			st.push(i);
		} else if (program[i] == ']') {
			if (st.empty()) {
				throw std::runtime_error("Mismatched brackets at position: " + std::to_string(i));
			}
			int matchPos = st.top();
			st.pop();
			bracketMap[matchPos] = i;
			bracketMap[i] = matchPos;
		}
	}
	if (!st.empty()) {
		throw std::runtime_error("Unclosed brackets: " + std::to_string(st.size()));
	}
	return bracketMap;
}

int run_program(std::string& program, bool nV = false) {
	std::unordered_map<int, int> bracketMap = preprocessBrackets(program);
	unsigned exitCode = 0;
	size_t ptr = 0;
	std::vector<unsigned> memory(1, 0);
	stack_agc<unsigned> stack_mem;
	std::string logOutputBuffer;
	std::random_device rd;
	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<unsigned> dist(0,(unsigned)(-1));

	for (size_t i = 0; i < program.size(); ++i) {
		bool skipPrint = false;
		switch (program[i]) {
			case '+':
				memory[ptr]++;
				break;
			case '-':
				memory[ptr]--;
				break;
			case '<':
				if (ptr == 0) {
					throw std::runtime_error("Pointer underflow at position: " + std::to_string(i));
				}
				ptr--;
				break;
			case '>':
				ptr++;
				if (ptr == memory.size()) {
					memory.push_back(0);
				}
				break;
			case '.':
				std::cout << char(memory[ptr]);
				skipPrint = true;
				break;
			case ',':
				memory[ptr] = getchar();
				skipPrint = true;
				break;
			case '[':
				if (memory[ptr] == 0)
					i = bracketMap[i];
				break;
			case ']':
				if (memory[ptr] != 0)
					i = bracketMap[i];
				break;
			// Extra grammars start point
			case ':':
				std::cout << memory[ptr];
				skipPrint = true;
				break;
			case ';':
				std::cin >> memory[ptr];
				skipPrint = true;
				break;
			case '^':
				i = memory[ptr];
				break;
			case '!':
				exitCode = memory[ptr];
				break;
			case '~':
				if (memory[ptr] >= 1) memory[ptr] = 1;
				memory[ptr] = !memory[ptr];
				break;
			case '\'':
				if (!memory.empty())
					memory.pop_back();
				if (ptr >= memory.size() && !memory.empty())
					ptr = memory.size() - 1;
				break;
			case '&':
				memory[ptr] = ptr;
				break;
			case '$':
				ptr = memory[ptr];
				while (memory.size() <= ptr)
					memory.push_back(0);
				break;
			case '*':
				memory[ptr] *= memory[ptr];
				break;
			case '\"':
				logOutputBuffer.push_back(memory[ptr]);
				if (program.size() == i + 1 || (program[i + 1] != '\"' && program[i + 1] != '<' && program[i + 1] != '>')) {
					std::ofstream log_out("blpg.log", std::ios_base::out | std::ios_base::app);
					log_out << logOutputBuffer;
					log_out.close();
					logOutputBuffer.clear();
				}
				break;
			case '\\':
				stack_mem.push(memory[ptr]);
				memory[ptr] = 0;
				break;
			case '/':
				if (!stack_mem.empty()) {
					memory[ptr] = stack_mem.top();
					stack_mem.pop();
				}
				break;
			case '_':
				memory[ptr] = dist(gen);
				break;
			default:
				skipPrint = true;
				break;
		}
		if (nV && !skipPrint) {
			std::cout << "Memory: ";
			for (size_t j = 0; j < memory.size(); j++)
				std::cout << memory[j] << ' ';
			std::cout << " | Pointer: " << ptr << '\n';
			std::cout << "Stack(BTT): ";
			for (unsigned& i : stack_mem.gc()) {
				std::cout << i << ' ';
			}
			std::cout << '\n';
		}

	}
	return exitCode;
}

int main(int argc, char** argv) {
	if (argc == 1) {
HELP_INFO:
		std::cout << "Brainlike Code Interpreter Version " << VERSION << '\n'
		          << "    --Feel free in symbols\n"
		          << "Usage:\n"
		          << "    -h : Get this help\n"
		          << "    [FILENAME(.blc)] : Run Code\n"
		          << "    -d : Run and print the memory array \n         after every commands (expect for ';',',',':','.')\n";
		return 0;
	}
	bool debugMode = false;
	std::string filename;
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-h") {
			goto HELP_INFO;
		} else if (arg == "-d") {
			debugMode = true;
		} else if (filename.empty()) {
			filename = arg;
		} else {
			std::cerr << "Invalid argument: " << arg << std::endl;
			return 1;
		}
	}
	if (filename.empty()) {
		std::cerr << "Error: No filename provided!" << std::endl;
		return 1;
	}
	try {
		std::string program = read_code_file(filename);
		return run_program(program, debugMode);
	} catch (const std::runtime_error& e) {
		std::cerr << "Runtime Error: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown Error!" << std::endl;
		return 1;
	}
}
