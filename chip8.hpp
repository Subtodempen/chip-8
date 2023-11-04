#include<iostream>
#include<fstream>
#include<vector>
#include<cstdint>


class CPU{
	public:
		CPU();
		~CPU();

		void Init();	//sets everyything to zero
		void loadRom(std::string Fname); //loads ROM instructions into 0x200 onwards
	 	void emuCycle();//emulates a cycle
	private:
		struct Memory{
			std::vector<uint8_t> ram;
			std::vector<uint8_t> V;//stack dont forget

			uint16_t I;

			uint8_t soundTimer;
			uint8_t delayTimer;

			uint16_t PC;
			uint8_t SP;
		}mem;

		void littleToBig(std::vector<uint8_t> &vec);
};
