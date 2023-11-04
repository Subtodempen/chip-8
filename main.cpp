#include"chip8.hpp"

int main(){
	CPU cpu;

	cpu.Init();
	cpu.loadRom("rom.ch8");

	cpu.emuCycle();
	cpu.emuCycle();
}
