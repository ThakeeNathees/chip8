#pragma once
#include "include.h"
#include "Disassembler.h"

// one instruction = 2 byte
#define ONE_INSTRUCTION 2

#define NUM_SPR_SIZE 5
#define NUM_SPR_0 { 0xF0, 0x90, 0x90, 0x90, 0xF0 }
#define NUM_SPR_1 { 0x20, 0x60, 0x20, 0x20, 0x70 }
#define NUM_SPR_2 { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }
#define NUM_SPR_3 { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }
#define NUM_SPR_4 { 0x90, 0x90, 0xF0, 0x10, 0x10 }
#define NUM_SPR_5 { 0xF0, 0x80, 0xF0, 0x10, 0xF0 }
#define NUM_SPR_6 { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }
#define NUM_SPR_7 { 0xF0, 0x10, 0x20, 0x40, 0x40 }
#define NUM_SPR_8 { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }
#define NUM_SPR_9 { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }
#define NUM_SPR_A { 0xF0, 0x90, 0xF0, 0x90, 0x90 }
#define NUM_SPR_B { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }
#define NUM_SPR_C { 0xF0, 0x80, 0x80, 0x80, 0xF0 }
#define NUM_SPR_D { 0xE0, 0x90, 0x90, 0x90, 0xE0 }
#define NUM_SPR_E { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }
#define NUM_SPR_F { 0xF0, 0x80, 0xF0, 0x80, 0x80 }


typedef bool(*pixel_array)[WIDTH_PIX];


class Emulator
{
public:
	Emulator() {
		m_disassembler.setBytesArray(m_rom);
		m_disassembler.disassemble();
		mapKeys();
		setNumSprite();
	}

	void mapKeys() {

		//  1	2	3	C		1	2	3	4
		//  4	5	6	D   --> Q	W	E	R
		//  7	8	9	E   -->	A	S	D	F
		//  A	0	B	F		Z	X	C	V


		m_key[0x1] = sf::Keyboard::Num1;
		m_key[0x2] = sf::Keyboard::Num2;
		m_key[0x3] = sf::Keyboard::Num3;

		m_key[0x4] = sf::Keyboard::Q;
		m_key[0x5] = sf::Keyboard::W;
		m_key[0x6] = sf::Keyboard::E;

		m_key[0x7] = sf::Keyboard::A;
		m_key[0x8] = sf::Keyboard::S;
		m_key[0x9] = sf::Keyboard::D;

		m_key[0xa] = sf::Keyboard::Z;
		m_key[0x0] = sf::Keyboard::X;
		m_key[0xb] = sf::Keyboard::C;

		m_key[0xc] = sf::Keyboard::Num4;
		m_key[0xd] = sf::Keyboard::R;
		m_key[0xe] = sf::Keyboard::F;
		m_key[0xf] = sf::Keyboard::V;
	}
	void setNumSprite() {
		unsigned int pos;
		pos = ROM_SIZE - 1 - 0x10 * NUM_SPR_SIZE;

		for (unsigned char byte : NUM_SPR_0) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_1) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_2) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_3) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_4) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_5) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_6) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_7) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_8) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_9) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_A) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_B) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_C) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_D) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_E) m_rom[pos++] = byte;
		for (unsigned char byte : NUM_SPR_F) m_rom[pos++] = byte;

	}
	unsigned short getNumSprAddr(unsigned char num) {
		num &= 0xf;
		return ROM_SIZE - 1 - 0x10 * NUM_SPR_SIZE + (num * NUM_SPR_SIZE);
	}

	std::vector<Instruction>& getInstructions() {
		return m_disassembler.getInstructions();
	}

	const pixel_array getPixels() { // to draw on window
		return m_pixels;
	}

	const unsigned char* getBytes() {
		return m_rom;
	}
	unsigned char* getBytesToWrite() {
		return m_rom + PROGRAMME_OFFSET;
	}

	unsigned short getPc() { return m_pc; }
	unsigned short getI() { return m_I; }
	unsigned char getDt() { return m_dt; }
	unsigned char getSt() { return m_st; }
	unsigned char getSp() { return m_sp; }
	unsigned char getV(unsigned char x){ 
		return m_V[ x & 0xf ]; 
	}
	unsigned short getStack(unsigned char x) { return m_stack[x & 0xf]; }
	const unsigned short* getStack() { return m_stack; }

	/////////////////////////////////////////////////////////////////////////
	void degreaseDt() {
		if (m_dt > 0) m_dt--;
	}
	void degreaseSt() {
		if (m_st > 0) {
			// TODO: buzz sound
			m_st--;
		}
	}
	void clearScreen() {
		for (unsigned int h = 0; h < HEIGHT_PIX; h++) {
			for (unsigned int w = 0; w < WIDTH_PIX; w++) {
				m_pixels[h][w] = false;
			}
		}
	}
	unsigned short restart() {
		m_pc = 0x200;
		m_sp = 0;
		m_dt = 0;
		m_st = 0;
		for (int x = 0; x < 0x10; x++) {
			m_V[x] = 0;
		}
		for (int x = 0; x < 0x10; x++) {
			m_stack[x] = 0;
		}
		clearScreen();
		setNumSprite();
		return m_pc;
	}

	void setDispPixel(unsigned int x, unsigned y, bool on = true) {
		if (y < HEIGHT_PIX && x < WIDTH_PIX)
			m_pixels[y][x] = on;
	}

	void disassemble() {
		m_disassembler.disassemble();
	}

	unsigned short step() { // return pc
		// BUG! TODO: end condition 
		Instruction ins = m_disassembler.getInstructions()[m_pc/2];
		return exec(ins);
	}

	unsigned short exec(const Instruction& ins) {

		if (ins.type == InstructionType::NONE) {
			m_pc += ONE_INSTRUCTION;
		}

		else if (ins.type == InstructionType::CLS) { // Clear the display.
			clearScreen();
			m_pc += ONE_INSTRUCTION;
		}

		else if (ins.type == InstructionType::RET) { // Return from a subroutine.
			// The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
			m_pc = m_stack[(--m_sp)&0xf]; // TODO: what if sp = 0;
		}

		else if (ins.type == InstructionType::JP) { // Jump to location nnn.
			// The interpreter sets the program counter to nnn.
			m_pc = ins.addr;
		}

		else if (ins.type == InstructionType::CALL) { // Call subroutine at nnn.
			// The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
			m_stack[(m_sp++)&0xf] = m_pc+ ONE_INSTRUCTION;
			m_pc = ins.addr;
		}

		else if (ins.type == InstructionType::SE_Vx) { // Skip next instruction if Vx = kk.
			// The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
			unsigned char vx = m_V[ins.x];
			unsigned char kk = ins.byte;
			if (vx == kk) m_pc += 2 * ONE_INSTRUCTION;
			else m_pc += ONE_INSTRUCTION;
		}

		else if (ins.type == InstructionType::SNE_Vx) { // 4xkk - SNE Vx, byte
			// The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
			unsigned char vx = m_V[ins.x];
			unsigned char kk = ins.byte;
			if (vx != kk) m_pc += 2*ONE_INSTRUCTION;
			else m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::SE_Vx_Vy) { // 5xy0 - SE Vx, Vy
			// The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
			unsigned char vx = m_V[ins.x], vy = m_V[ins.y];
			if (vx == vy) m_pc += 2*ONE_INSTRUCTION;
			else m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_Vx) { // Set Vx = kk.
			// The interpreter puts the value kk into register Vx.
			m_V[ins.x] = ins.byte;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::ADD_Vx) { // Set Vx = Vx + kk.
			// Adds the value kk to the value of register Vx, then stores the result in Vx.
			m_V[ins.x] = m_V[ins.x] + ins.byte;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_Vx_Vy) { // Set Vx = Vy.
			// Stores the value of register Vy in register Vx.
			m_V[ins.x] = m_V[ins.y];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::OR_Vx_Vy) { // Set Vx = Vx OR Vy.
			// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
			// A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, 
			// then the same bit in the result is also 1. Otherwise, it is 0.
			m_V[ins.x] = m_V[ins.x] | m_V[ins.y];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::AND_Vx_Vy) { // Set Vx = Vx AND Vy.
			// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
			// A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, 
			// then the same bit in the result is also 1. Otherwise, it is 0.
			m_V[ins.x] = m_V[ins.x] & m_V[ins.y];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::XOR_Vx_Vy) { // Set Vx = Vx XOR Vy.
			// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
			// An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, 
			// then the corresponding bit in the result is set to 1. Otherwise, it is 0.
			m_V[ins.x] = m_V[ins.x] ^ m_V[ins.y];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::ADD_Vx_Vy) { // Set Vx = Vx + Vy, set VF = carry.
			// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) 
			// VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
			unsigned char vx = m_V[ins.x], vy = m_V[ins.y];
			m_V[vx] =  vx + vy;
			m_V[0xf] = (vx + vy > 0xff) ? 1 : 0;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::SUB_Vx_Vy) { // Set Vx = Vx - Vy, set VF = NOT borrow.
			// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
			unsigned char vx = m_V[ins.x], vy = m_V[ins.y];
			m_V[0xf] = (vx > vy) ? 1 : 0;
			m_V[vx] = vx - vy;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::SHR_Vx) { // Set Vx = Vx SHR 1.
			// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
			unsigned char vx = m_V[ins.x];
			m_V[0xf] = (vx & 0b00000001) ? 1 : 0;
			m_V[ins.x] = vx / 2;
			m_pc += ONE_INSTRUCTION;

		}
		else if (ins.type == InstructionType::SUBN_Vx_Vy) { // Set Vx = Vy - Vx, set VF = NOT borrow.
			// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
			unsigned char vx = m_V[ins.x], vy = m_V[ins.y];
			m_V[0xf] = (vy > vx) ? 1 : 0;
			m_V[vx] = vy - vx;
			m_pc += ONE_INSTRUCTION;

		}
		else if (ins.type == InstructionType::SHL_Vx) { // Set Vx = Vx SHL 1.
			// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
			unsigned char vx = m_V[ins.x];
			m_V[0xf] = (vx & 0b10000000) ? 1: 0;
			m_V[ins.x] = vx * 2;
			m_pc += ONE_INSTRUCTION;

		}
		else if (ins.type == InstructionType::SNE_Vx_Vy) { // Skip next instruction if Vx != Vy.
			// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
			unsigned char vx = m_V[ins.x], vy = m_V[ins.y];
			if (vx != vy) m_pc += 2*ONE_INSTRUCTION;
			else m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_I) { // The value of register I is set to nnn.
			// The value of register I is set to nnn.
			m_I = ins.addr;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::JP_V0) { // Jump to location nnn + V0.
			// The program counter is set to nnn plus the value of V0.
			m_pc = m_V[0] + ins.addr;
		}
		else if (ins.type == InstructionType::RND_Vx) { // Set Vx = random byte AND kk.
			// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. 
			// The results are stored in Vx. See instruction 8xy2 for more information on AND.
			m_V[ins.x] = rand() % (0x100) & ins.byte;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::DRW_Vx_Vy) { // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
			// The interpreter reads n bytes from memory, starting at the address stored in I.
			// These bytes are then displayed as sprites on screen at coordinates(Vx, Vy).Sprites are XORed onto the existing screen.
			// If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned 
			// so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.See instruction

			m_V[0xf] = 0; // no collision
			unsigned char vx = m_V[ins.x], vy = m_V[ins.y];
			for (unsigned int h = 0; h < ins.byte; h++) {
				unsigned char spr = m_rom[m_I+h];
				for (unsigned int w = 0; w < 0x8; w++) {
					bool bit = ( ( spr & (1 << 0x7 - w)) ) >> (0x7 - w) == 1;
					if (m_pixels[vy+h][vx+w] == 1 && bit == 0) m_V[0xf] = 1; // set collision true
					m_pixels[vy+h][vx+w] ^= bit;
				}
			}
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::SKP_Vx) { // Skip next instruction if key with the value of Vx is pressed.
		// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
			if (sf::Keyboard::isKeyPressed( m_key[m_V[ins.x] & 0xf] )) m_pc += 2*ONE_INSTRUCTION;
			else m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::SKNP_Vx) { // Skip next instruction if key with the value of Vx is not pressed.
			// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
			if (!sf::Keyboard::isKeyPressed(m_key[m_V[ins.x] & 0xf])) m_pc += 2*ONE_INSTRUCTION;
			else m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_Vx_DT) { // Set Vx = delay timer value.
			// The value of DT is placed into Vx.
			m_V[ins.x] = m_dt;
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_Vx_K) { // Wait for a key press, store the value of the key in Vx.
			// All execution stops until a key is pressed, then the value of that key is stored in Vx.
			m_V[ins.x] = watiAndGetKey();
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_DT_Vx) { // Set delay timer = Vx.
			// DT is set equal to the value of Vx.
			m_dt = m_V[ins.x];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_ST_Vx) { // Set sound timer = Vx.
			// ST is set equal to the value of Vx.
			m_st = m_V[ins.x];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::ADD_I_Vx) { // Set I = I + Vx.
			// The values of I and Vx are added, and the results are stored in I.
			m_I += m_V[ins.x];
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_F_Vx) { // Set I = location of sprite for digit Vx.
			// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. 
			
			// if Vx = 0x7
			// ****  <-- addr_of_7
			//    *
			//  *
			//  *
			//  *
			// m_I = addr_of_7
			m_I = getNumSprAddr( m_V[ins.x] );
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_B_Vx) { // Store BCD representation of Vx in memory locations I, I+1, and I+2.
			// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, 
			// the tens digit at location I+1, and the ones digit at location I+2.
			unsigned char vx = m_V[ins.x];       // ex: vx = 123
			m_rom[m_I + 2] = vx % 10; vx /= 10;	 // I+2 : 3, vx = 12
			m_rom[m_I + 1] = vx % 10; vx /= 10;	 // I+1 : 2, vx = 1
			m_rom[m_I + 0] = vx % 10;			 // I+0 : 1
			m_pc += ONE_INSTRUCTION;
		} 
		else if (ins.type == InstructionType::LD_I_Vx) { // Store registers V0 through Vx in memory starting at location I.
			// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
			for (unsigned char x = 0; x < 0x10; x++) {
				m_rom[m_I + x] = m_V[x];
			}
			m_pc += ONE_INSTRUCTION;
		}
		else if (ins.type == InstructionType::LD_Vx_I) { // Read registers V0 through Vx from memory starting at location I.
			// The interpreter reads values from memory starting at location I into registers V0 through Vx.
			for (unsigned char x = 0; x < 0x10; x++) {
				m_V[x] = m_rom[m_I + x];
			}
			m_pc += ONE_INSTRUCTION;
		}
		return m_pc;
	}


private:
	unsigned char m_rom[ROM_SIZE] = { 0 };
	unsigned char m_V[0x10] = { 0 };  // 8 bit Vx registers V0 - VF
	unsigned char m_dt, m_st;		// delay timer, sound timer
	unsigned short m_I = 0x000;		// 16 bit address register
	unsigned short m_pc = 0x000;	// 16 bit programme counter

	unsigned short m_stack[0x10];	// 16 16 bit array of stack
	unsigned char m_sp = 0;			// 8 bit stack pointer

	Disassembler m_disassembler;
	bool m_pixels[HEIGHT_PIX][WIDTH_PIX]; // display
	sf::Keyboard::Key m_key[0x10]; // keys 0-f

	unsigned char watiAndGetKey() {
		while (true) {
			if (sf::Keyboard::isKeyPressed(m_key[0x0])) return 0x0;
			if (sf::Keyboard::isKeyPressed(m_key[0x1])) return 0x1;
			if (sf::Keyboard::isKeyPressed(m_key[0x2])) return 0x2;
			if (sf::Keyboard::isKeyPressed(m_key[0x3])) return 0x3;
			if (sf::Keyboard::isKeyPressed(m_key[0x4])) return 0x4;
			if (sf::Keyboard::isKeyPressed(m_key[0x5])) return 0x5;
			if (sf::Keyboard::isKeyPressed(m_key[0x6])) return 0x6;
			if (sf::Keyboard::isKeyPressed(m_key[0x7])) return 0x7;
			if (sf::Keyboard::isKeyPressed(m_key[0x8])) return 0x8;
			if (sf::Keyboard::isKeyPressed(m_key[0x9])) return 0x9;
			if (sf::Keyboard::isKeyPressed(m_key[0xa])) return 0xa;
			if (sf::Keyboard::isKeyPressed(m_key[0xb])) return 0xb;
			if (sf::Keyboard::isKeyPressed(m_key[0xc])) return 0xc;
			if (sf::Keyboard::isKeyPressed(m_key[0xd])) return 0xd;
			if (sf::Keyboard::isKeyPressed(m_key[0xe])) return 0xe;
			if (sf::Keyboard::isKeyPressed(m_key[0xf])) return 0xf;
		}
	}

};