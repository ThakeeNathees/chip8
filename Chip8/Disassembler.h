#pragma once
#include "include.h"
#include "gui/conf.h" // for ROM_SIZE

/*
0nnn - SYS addr is not implimented here - it'll be ignored
*/

enum class InstructionType
{
	NONE,				// 0000
	CLS, 				// 00E0 
	RET, 				// 00EE 
	JP, 				// 1nnn 
	CALL, 				// 2nnn 
	SE_Vx, 				// 3xkk 
	SNE_Vx, 			// 4xkk 
	SE_Vx_Vy, 			// 5xy0 
	LD_Vx, 				// 6xkk 
	ADD_Vx, 			// 7xkk 
	LD_Vx_Vy, 			// 8xy0 
	OR_Vx_Vy, 			// 8xy1 
	AND_Vx_Vy, 			// 8xy2 
	XOR_Vx_Vy, 			// 8xy3 
	ADD_Vx_Vy, 			// 8xy4 
	SUB_Vx_Vy, 			// 8xy5 
	SHR_Vx, 			// 8xy6 
	SUBN_Vx_Vy, 		// 8xy7 
	SHL_Vx, 			// 8xyE 
	SNE_Vx_Vy, 			// 9xy0 
	LD_I, 				// Annn 
	JP_V0, 				// Bnnn 
	RND_Vx, 			// Cxkk 
	DRW_Vx_Vy, 			// Dxyn 
	SKP_Vx, 			// Ex9E 
	SKNP_Vx, 			// ExA1 
	LD_Vx_DT, 			// Fx07 
	LD_Vx_K, 			// Fx0A 
	LD_DT_Vx, 			// Fx15 
	LD_ST_Vx, 			// Fx18 
	ADD_I_Vx, 			// Fx1E 
	LD_F_Vx, 			// Fx29 
	LD_B_Vx, 			// Fx33 
	LD_I_Vx, 			// Fx55 
	LD_Vx_I, 			// Fx65 

};

struct Instruction
{
public:
	InstructionType type = InstructionType::NONE;
	unsigned int x, y;			// registers
	unsigned int addr, byte;	// values
	std::string to_string = "---";
};

class Disassembler
{
private:
	unsigned char* m_bytes_array = NULL; // a pointer to bytes array
	unsigned int m_bytes_size = ROM_SIZE;
	std::vector<Instruction> m_instructions;

public:
	void setBytesArray(unsigned char* bytes_array) {
		m_bytes_array = bytes_array;
	}

	Instruction byteToInstruction(unsigned int byte) {
		Instruction ins;

		if (byte == 0x0000) { // NONE 
		}
		else if (byte == 0x00E0) {  // 00E0 - CLS
			ins.type = InstructionType::CLS;
			ins.to_string = "CLS";
		}
		else if (byte == 0x00EE) {  // 00EE - RET
			ins.type = InstructionType::RET;
			ins.to_string = "RET";
		}

		else if ((byte & 0xf000) == 0x1000) { // 1nnn - JP addr
			ins.type = InstructionType::JP;
			ins.addr = byte & 0x0fff;
			ins.to_string = std::string("JP   0x").append(toHexString(ins.addr, 4));
		}

		else if ((byte & 0xf000) == 0x2000) { // 2nnn - CALL addr
			ins.type = InstructionType::CALL;
			ins.addr = byte & 0x0fff;
			ins.to_string = std::string("CALL 0x").append(toHexString(ins.addr, 4));
		}

		else if ((byte & 0xf000) == 0x3000) { // 3xkk - SE Vx, byte
			ins.type = InstructionType::SE_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.byte = byte & 0x00ff;
			ins.to_string = std::string("SE   V").append(std::to_string(ins.x)).append(" 0x").append(std::to_string(ins.byte));
		}

		else if ((byte & 0xf000) == 0x4000) { // 4xkk - SNE Vx, byte
			ins.type = InstructionType::SNE_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.byte = byte & 0x00ff;
			ins.to_string = std::string("SNE  V") .append(std::to_string(ins.x)).append(" ").append( std::to_string(ins.byte) );
		}

		else if ((byte & 0xf00f) == 0x5000) { // 5xy0 - SE Vx, Vy
			ins.type = InstructionType::SNE_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("SE   V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf000) == 0x6000) { // 6xkk - LD Vx, byte
			ins.type = InstructionType::LD_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.byte = byte & 0x00ff;
			ins.to_string = std::string("LD   V").append(std::to_string(ins.x)).append(" ").append(std::to_string(ins.byte));
		}

		else if ((byte & 0xf000) == 0x7000) { // 7xkk - ADD Vx, byte
			ins.type = InstructionType::ADD_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.byte = byte & 0x00ff;
			ins.to_string = std::string("ADD  V").append(std::to_string(ins.x)).append(" ").append(std::to_string(ins.byte));
		}

		else if ((byte & 0xf00f) == 0x8000 ) { // 8xy0 - LD Vx, Vy
			ins.type = InstructionType::LD_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("LD   V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8001 ) { // 8xy1 - OR Vx, Vy
			ins.type = InstructionType::OR_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("OR   V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8002) { // 8xy2 - AND Vx, Vy
			ins.type = InstructionType::AND_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("AND  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8003) { // 8xy3 - XOR Vx, Vy
			ins.type = InstructionType::XOR_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("XOR  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8004) { // 8xy4 - ADD Vx, Vy
			ins.type = InstructionType::ADD_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("ADD  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8005) { // 8xy5 - SUB Vx, Vy
			ins.type = InstructionType::SUB_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("SUB  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8006) { // 8xy6 - SHR Vx {, Vy}
			ins.type = InstructionType::SHR_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("SHR  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x8007) { // 8xy7 - SUBN Vx, Vy
			ins.type = InstructionType::SUBN_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("SUBN V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf00f) == 0x800E) { // 8xyE - SHL Vx {, Vy}
			ins.type = InstructionType::SHL_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("SHL  V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf00f) == 0x9000) { // 9xy0 - SNE Vx, Vy
			ins.type = InstructionType::SNE_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.to_string = std::string("SNE  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y));
		}

		else if ((byte & 0xf000) == 0xA000 ) { // Annn - LD I, addr
			ins.type = InstructionType::LD_I;
			ins.addr = byte & 0x0fff;
			ins.to_string = std::string("LD   I 0x").append(toHexString(ins.addr, 4));
		}

		else if ((byte & 0xf000) == 0xB000 ) { // Bnnn - JP V0, addr
			ins.type = InstructionType::JP_V0;
			ins.addr = byte & 0x0fff;
			ins.to_string = std::string("JP   V0 0x").append(toHexString(ins.addr, 4));
		}

		else if ((byte & 0xf000) == 0xC000 ) { // Cxkk - RND Vx, byte
			ins.type = InstructionType::RND_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.byte = byte & 0x00ff;
			ins.to_string = std::string("RND  V").append(std::to_string(ins.x)).append(" ").append(std::to_string(ins.byte));
		}

		else if ((byte & 0xf000) == 0xD000 ) { // Dxyn - DRW Vx, Vy, nibble
			ins.type = InstructionType::DRW_Vx_Vy;
			ins.x = (byte & 0x0f00) >> 8;
			ins.y = (byte & 0x00f0) >> 4;
			ins.byte = byte & 0x000f;
			ins.to_string = std::string("DRW  V").append(std::to_string(ins.x)).append(" V").append(std::to_string(ins.y)).append(" ").append(std::to_string(ins.byte));
		}

		else if ((byte & 0xf0ff) == 0xE09E ) { // Ex9E - SKP Vx
			ins.type = InstructionType::SKP_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("SKP  V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xE0A1 ) { // ExA1 - SKNP Vx
			ins.type = InstructionType::SKNP_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("SNKP V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF007 ) { // Fx07 - LD Vx, DT
			ins.type = InstructionType::LD_Vx_DT;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   V").append(std::to_string(ins.x)).append(" DT");
		}

		else if ((byte & 0xf0ff) == 0xF00A ) { // Fx0A - LD Vx, K
			ins.type = InstructionType::LD_Vx_K;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   V").append(std::to_string(ins.x)).append(" K");
		}

		else if ((byte & 0xf0ff) == 0xF015 ) { // Fx15 - LD DT, Vx
			ins.type = InstructionType::LD_DT_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   DT V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF018 ) { // Fx18 - LD ST, Vx
			ins.type = InstructionType::LD_ST_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   ST V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF01E ) { // Fx1E - ADD I, Vx
			ins.type = InstructionType::ADD_I_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("ADD  I V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF029 ) { // Fx29 - LD F, Vx
			ins.type = InstructionType::LD_F_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   F V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF033 ) { // Fx33 - LD B, Vx
			ins.type = InstructionType::LD_B_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   B V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF055 ) { // Fx55 - LD [I], Vx
			ins.type = InstructionType::LD_I_Vx;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   I V").append(std::to_string(ins.x));
		}

		else if ((byte & 0xf0ff) == 0xF065 ) { // Fx65 - LD Vx, [I]
			ins.type = InstructionType::LD_Vx_I;
			ins.x = (byte & 0x0f00) >> 8;
			ins.to_string = std::string("LD   V").append(std::to_string(ins.x)).append(" I");
		}

		else {
			// std::exception("unknown instruction");
		}
		
		return ins;
	}

	void disassemble() {
		m_instructions.clear();
		if (m_bytes_array == NULL) throw "m_bytes_array uninitialized";
		if (m_bytes_size % 2 != 0) throw "m_bytes_size % 2 must be 0";

		for (unsigned int i = 0; i < m_bytes_size; ) {
			unsigned int instruction_byte = m_bytes_array[i] << 8 | m_bytes_array[i+1];
			i += 2;
			Instruction instruction = byteToInstruction(instruction_byte);
			m_instructions.push_back(instruction);
		}
	}

	std::vector<Instruction>& getInstructions() {
		return m_instructions;
	}

};
