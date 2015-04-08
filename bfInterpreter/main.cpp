#include <iostream>
#include <fstream>
#include <string>

#define TESTING

#define MEM_SIZE 30000

/*++++++++++++++++++++++++++++++++[>+>+<<-]>>+++++++++++++++++++++++++<<++++++++++[>>.-<.<-]*/

typedef char byte;

std::string loadFile(const std::string&);

struct bfState
{
	byte mem[MEM_SIZE];
	unsigned ptr;

	std::string src;
	unsigned instrPtr;
};

bfState createbfState(const std::string& fileName)
{
	bfState newbfState;
	newbfState.ptr = 0;
	newbfState.instrPtr = 0;

	newbfState.src = loadFile(fileName);
	return newbfState;
}
/*
static byte mem[MEM_SIZE];
static unsigned int ptr = 0;

static std::string src;
static unsigned int instrPtr = 0;
*/
std::string loadFile(const std::string& fileName)
{
	std::ifstream file;
	std::string line;
	std::string ret;
	file.open(fileName);

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, line);
			ret.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to open file: " << fileName << std::endl;
	}

	return ret;
}

/*
* Print used memory by printing everything in mem until two 0's have been found
* Assumes that memory is used contiguously
*/
void printUsedMem(const bfState& state)
{
	bool lastZ = false;
	for (int i = 0; i < MEM_SIZE; i++)
	{
		if (state.mem[i] == 0)
		{
			if (lastZ)
			{
				std::cout << "Approximate used memory : " << i << " bytes" << std::endl;
				return;
			}
			else
			{
				std::cout << "0" << std::endl;
				lastZ = true;
			}
		}
		else
		{
			std::cout << (int)state.mem[i] << std::endl;
			lastZ = false;
		}
	}
}

void findMatchingBrace(const char symbol, bfState& state)
{
	bool foundMatch = false;
	if (symbol == ']')
	{
		for (unsigned i = state.instrPtr; i < state.src.size(); i++)
		{
			if (state.src[i] == symbol)
			{
				state.instrPtr = i;
				foundMatch = true;
				break;
			}
		}
	}
	else
	{
		for (unsigned i = state.instrPtr; i > 0; i--)
		{
			if (state.src[i] == symbol)
			{
				state.instrPtr = i;
				foundMatch = true;
				break;
			}
		}
	}
	if (!foundMatch)
	{
		std::cout << "Error: missing '" << symbol << "' at" << std::endl;
		std::cout << state.src << std::endl;
		for (int i = 0; i < state.instrPtr; i++)
		{
			std::cout << " ";
		}
		std::cout << "^" << std::endl;
		system("pause");
	}
}

bool processInstruction(bfState& state)
{
	switch (state.src[state.instrPtr])
	{
	case '>':
		state.ptr++;
		break;
	case '<':
		state.ptr--;
		break;
	case '+':
		state.mem[state.ptr]++;
		break;
	case '-':
		state.mem[state.ptr]--;
		break;
	case '.':
		std::cout << state.mem[state.ptr];
		break;
	case ',':
		std::cin >> state.mem[state.ptr];
		break;
	case '[':
		// is the while condition true? (while (mem[ptr]))
		if (state.mem[state.ptr] == 0)
		{
			// find the closing ']' and jump to it
			findMatchingBrace(']', state);
		}
		break;
	case ']':
		// loop?
		if (state.mem[state.ptr] != 0)
		{
			// find the opening '[' and jump to it
			findMatchingBrace('[', state);
			state.instrPtr--;
		}
		break;
	case '\n':
		return false;
		break;
	default:
		std::cout << "Unknown instruction: " << state.src[state.instrPtr] << std::endl;
		return false;
		break;
	}
	state.instrPtr++;
	return true;
}

int main(int argc, char **argv)
{
	bfState bfState;
	if (argc > 1)
	{
		bfState = createbfState(argv[1]);
	}
	else
	{
#ifdef TESTING
		bfState = createbfState("test.txt");
#else
		std::cout << "usage: bfInterpreter [sourceFile]" << std::endl;
		return -1;
#endif
	}

	while (bfState.src.size() > bfState.instrPtr)
	{
		if (!processInstruction(bfState))
		{
			break;
		}
	}
	std::cout << std::endl;
	system("pause");
	return 0;
}