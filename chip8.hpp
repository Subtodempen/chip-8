#include<iostream>

class CPU{
	
	private:
		struct Memory{
			unsigned char ram[4095];
			unsigned char V[15];//stack dont forget

			uint16_t I;

			uint8_t soundTimer;
			uint8_t delayTimer;

			uint16_t PC;
			uint8_t SP;
		}mem;
};
