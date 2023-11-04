#include"chip8.cpp"


int main(){
	CPU cpu;

	cpu.Init();
	cpu.loadRom("rom.ch8");
}
