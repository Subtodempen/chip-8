#include<iostream>

class CPU{
	public:
		Cpu();
		~Cpu();

		void Init();	//sets everyything to zero
		void loadRom(); //loads ROM instructions into 0x200 onwards
		void emuCycle();//emulates a cycle
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
