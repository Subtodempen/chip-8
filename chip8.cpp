#include"chip8.hpp"


CPU::CPU(){}
CPU::~CPU(){}

void CPU::Init(){
	mem.PC = 0x200;
	mem.SP = 0;

	mem.soundTimer = 0;
	mem.delayTimer = 0;
	
	mem.I = 0;

	std::fill(mem.V.begin(), mem.V.end(), 0);
	std::fill(mem.ram.begin(), mem.ram.end(), 0);
}

void CPU::loadRom(std::string Fname){
	std::ifstream input(Fname, std::ios::binary);	
	
	std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
}
