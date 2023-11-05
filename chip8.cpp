#include "chip8.hpp"

#include <iomanip>
#include <stdlib.h>


unsigned char chip8Fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};


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
	mem.stack.resize(16);

	std::fill(mem.V.begin(), mem.V.end(), 0);
	std::fill(mem.ram.begin(), mem.ram.end(), 0);

	for(int i=0; i < xRes; i++){ //sets display as zero
		for(int z=0; z < yRes; z++){ 
			screen[i][z] = 0;
		}
	}

	for(int i=0; i < 80; i++)
		mem.ram[i] = chip8Fontset[i];
	
	srand(0); //change
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
	
	//convert to big endian
	// littleToBig(dataArray);

	int loadCount = 0x200;
	for (uint8_t value : dataArray){
		//std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value) << " " ;
		
		mem.ram[loadCount] = value;
		loadCount++;
	}

	/*
	for(uint8_t byte : mem.ram){
		std::cout<< std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
	}
	std::cout<<std::endl;
	*/
}


void CPU::emuCycle(){
	uint16_t opcode;
	bool addPC = true;
	
	//fetch
	opcode = static_cast<uint16_t>(mem.ram[mem.PC]) << 8 | mem.ram[mem.PC+1];
	
	//std::cout<< std::hex << std::setw(2)  <<"opcode:"<< static_cast<int>(opcode)<<std::endl;

	//decode
	int x = (opcode & 0x0F00) >> 8; 
	int y = (opcode & 0x00F0) >> 4; 
	
	//execute
	switch((opcode & 0xF000) >> 12){
		case 0:
			if((opcode & 0x000F) == 0x0){

				for(int i=0; i < xRes; i++){ //sets display as zero
					for(int z=0; z < yRes; z++){ 
						screen[i][z] = 0;
					}
				}
			}else if((opcode & 0x000F) == 0xE){
				mem.PC = mem.stack[mem.SP];
				mem.SP--;

				addPC = false;
			}
			
			break;
		case 1:
			mem.PC = (opcode & 0x0FFF);
			
			addPC = false;
			break;
		case 2:
			mem.stack[mem.SP] = mem.PC;
			mem.SP++;

			mem.PC = (opcode & 0x0FFF);
			
			addPC = false;
			break;
		case 3:
			if(mem.V[x] == (opcode & 0x00FF))
				mem.PC += 2;
			
			break;		
		case 4:	
			if(mem.V[x] != (opcode & 0x00FF))
				mem.PC += 2;

			break;	
		case 5:
			if(mem.V[x] == mem.V[y])
				mem.PC += 2;

			break;
		case 6:
			mem.V[x] = (opcode & 0x00FF);
			
			break;
		case 7:
			mem.V[x] = mem.V[x] + (opcode & 0x00FF);
			
			break;
		case 8:
			if((opcode & 0x000F) == 0x0)
				mem.V[x] = mem.V[y];
			
			else if((opcode & 0x000F) == 0x1)
				mem.V[x] = mem.V[x] | mem.V[y];
			
			else if((opcode & 0x000F) == 0x2)
				mem.V[x] = mem.V[x] & mem.V[y];
			
			else if((opcode & 0x000F) == 0x3)
				mem.V[x] = mem.V[x] ^ mem.V[y];
			
			else if((opcode & 0x000F) == 0x4){
				mem.V[x] = mem.V[x] + mem.V[y];

				if(mem.V[x] > 255){//if V[x] is greater than 8 bits
					mem.V[0xf] = 1;
					mem.V[x] = mem.V[x] & 0x0FF;
				}
			
			}else if((opcode & 0x000F) == 0x5){
				if(mem.V[x] > mem.V[y])
					mem.V[0xf] = 1;
				else{
					mem.V[0xf] = 0; //idk ab this 

					mem.V[x] = mem.V[x] - mem.V[y];
				}

			}else if((opcode & 0x000F) == 0x6){
				//idk
				mem.V[0xf] = (mem.V[x] & 0x000F) & 0b0001;

				mem.V[x] = mem.V[x] / 2;
			
			}else if((opcode & 0x000F) == 0x7){
				if(mem.V[y] > mem.V[y])
					mem.V[0xf] = 1;
				else
					mem.V[0xf] = 0;

				mem.V[x] = mem.V[y] - mem.V[x];
			
			}else if((opcode & 0x000F) == 0xE){
				mem.V[0xf] = ((mem.V[x] & 0xF000) >> 12) & 0b1000 >> 3;

				mem.V[x] = mem.V[x] * 2;
			}

			break;
		case 9:
			if(mem.V[(opcode & 0x0F00) >> 8] != mem.V[(opcode & 0x00F0) >> 4])
				mem.PC += 2;
			
			break;
		case 0xA:
			mem.I = (opcode & 0x0FFF);

			break;
		case 0xB:
			mem.PC = (opcode & 0x0FFF) + mem.V[0];

			addPC = false; 
			break;
		case 0xC:
			mem.V[x] = (rand() % 255) & (opcode & 0x00FF);

			break;
		case 0xD:
			{
    		uint8_t xPos = mem.V[x];
    		uint8_t yPos = mem.V[y];

    		for (int i = 0; i < (opcode & 0x000F); i++) {
        		for (int z = 0; z < 8; z++) {
            		int xPixel = (xPos + z) % xRes;
            		int yPixel = (yPos + i) % yRes;
            		
					if(screen[xPixel][yPixel] == false){
						screen[xPixel][yPixel] = (mem.ram[mem.I + i] >> (7 - z)) & 1;//error evisit
						mem.V[0xf] = 0;
					}
					else{
						mem.V[0xf] = 1;
						screen[xPixel][yPixel] = screen[xPixel][yPixel] != (mem.ram[mem.I + i] >> (7 - z)) & 1;
					}

        		}
    		}

    		break;
            }
		case 0XE:
		//need to do E and FX0A also do sound timer
		case 0XF:
			if((opcode & 0x000F) == 7)
				mem.V[x] = mem.delayTimer;
			else if((opcode & 0x000F) == 5){
				if(y == 1)
					mem.delayTimer = mem.V[x];
				else if(y == 5){
					for(int i=mem.I; i < 16; i++){//might giv eroor 
						mem.ram[i] = mem.V[i - mem.I];
					}	
				}
				else if(y == 6){
					for(int i=mem.I; i < 16; i++){//might giv eroor 
						mem.V[i - mem.I] = mem.ram[i];
					}	
				}
			}
			else if((opcode & 0x000F) == 8)
				mem.soundTimer = mem.V[x];
			
			else if((opcode & 0x000F) == 0XE)
				mem.I = mem.I + mem.V[x];
			
			else if((opcode & 0x000F) == 9)
				mem.I = (mem.V[x]) * 5;
			
			else if((opcode & 0x000F) == 3){
				mem.ram[mem.I] = (mem.V[x] / 100) % 10;
				mem.ram[mem.I + 1] = (mem.V[x] / 10) % 10;
				mem.ram[mem.I + 2] = mem.V[x] % 10;
			}
			break;
		default:
			std::cout<<"not recognised: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) <<std::endl;
			break;
	}		
	//add everything(pc, sound timers, delay timers)
	
	if(addPC)
		mem.PC += 2; // add it by 2 bytes 
	
	if(mem.delayTimer != 0)
		mem.delayTimer--;
	else
		mem.delayTimer = 60;

	for (int i = 0; i < yRes; i++) {
    	for (int z = 0; z < xRes; z++) {
        	std::cout << screen[z][i];
    	}
    	std::cout << "\n";
	}

	std::cout<<std::endl;
}
