#include"chip8.hpp"


int main(int argc, char **argv){
	//cli input cycles and fname
    if (argc != 3 || std::string(argv[2]) == "h") {
        std::cout << "Need more args\n";
        std::cout << "First argument - number of cycles.\nSecond argument - ROM file name\n";
        
		return -1;
    }
	
	CPU cpu;

	cpu.Init();
	cpu.loadRom(std::string(argv[2]));

	for(int i=0; i < std::stoi(argv[1]); i++) //change
		cpu.emuCycle();

	return 0;
}
