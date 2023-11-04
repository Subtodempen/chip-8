#include"chip8.hpp"
#include <iomanip>

CPU::CPU(){}
CPU::~CPU(){}

void CPU::littleToBig(std::vector<uint8_t> &vec){
	uint8_t temp;

	for(int i=1; i < vec.size(); i++){
		//swap
		temp = vec[i-1];
		vec[i-1] = vec[i];
		vec[i] = temp;
		
		i++;
	}
}

void CPU::Init(){
	mem.PC = 0x200;
	mem.SP = 0;

	mem.soundTimer = 0;
	mem.delayTimer = 0;
	
	mem.I = 0;
	
	mem.ram.resize(4095);
    mem.V.resize(15);

	std::fill(mem.V.begin(), mem.V.end(), 0);
	std::fill(mem.ram.begin(), mem.ram.end(), 0);
}

void CPU::loadRom(std::string Fname){
	std::ifstream input(Fname, std::ios::binary);	
	
	if (!input) {
        // Handle file open error
        std::cerr << "Error opening file: " << Fname << std::endl;
        return;
    }
	
	std::vector<uint8_t> dataArray;
    char byte;

	while (input.get(byte)) 
        dataArray.push_back(static_cast<uint8_t>(byte));
	//in little endian
	
	//convert to big endian
	littleToBig(dataArray);
	
	int loadCount = 0x200;
	for (uint8_t value : dataArray){
		mem.ram[loadCount] = value;
		loadCount++;
	}
		
	//prints ram
	//for(uint8_t val: mem.ram)
	//	std::cou << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(val) << " " ;
	
	//std::cout<<std::endl;
	
}


void CPU::emuCycle(){
	uint16_t opcode;
	
	//fetch
	opcode = static_cast<uint16_t>(mem.ram[mem.PC]) << 8 | mem.ram[mem.PC+1];
	
	//decode
	opcode = (opcode & 0xF000) >> 12;
	
	//execute
	switch(opcode){
		case 0:
			std::cout<<"zero good";
			break;
		default:
			std::cout<<"not recognised";
			break;
	}	
	
	std::cout<<"\nopocde="<< std::hex << std::setw(1) << std::setfill('0') << static_cast<int>(opcode)<<std::endl;
	//add everything(pc, sound timers, delay timers)
	mem.PC += 2; // add it by 2 bytes 
}
