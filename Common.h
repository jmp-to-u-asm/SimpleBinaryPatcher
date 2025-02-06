#pragma once

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <process.h>
#include <thread>
#include <bitset>
#include <array>
#include <dwmapi.h>
#include <string>



#define pOut(x) printf("[Go]: "); printf(x); printf("\n")

inline int to_lower_imp(int c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';
	else
		return c;
}
inline int strcmpi_imp(const char* s1, const char* s2)
{
	while (*s1 && (to_lower_imp(*s1) == to_lower_imp(*s2)))
	{
		s1++;
		s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

enum patch_status {

    failed = 0,
    succeed,
    perfect,

};

enum x86_instructions {
    NOP = 0x90,        // No operation
    MOV = 0x89,        // Move data
    ADD = 0x01,        // Add
    SUB = 0x29,        // Subtract
    INC = 0x40,        // Increment
    DEC = 0x48,        // Decrement
    JMP = 0xEB,        // Jump //0xE9 if relative addr
    JE = 0x74,         // Jump if Equal (Zero flag set)
    JNE = 0x75,        // Jump if Not Equal (Zero flag not set)
    JZ = 0x74,         // Jump if Zero (Zero flag set)
    JNZ = 0x75,        // Jump if Not Zero (Zero flag not set)
    CALL = 0xE8,       // Call procedure
    RET = 0xC3,        // Return from procedure
    CMP = 0x3D,        // Compare
    TEST = 0xA9,       // Test bits
    AND = 0x21,        // Bitwise AND
    OR = 0x09,         // Bitwise OR
    XOR = 0x31,        // Bitwise XOR
    SHL = 0xD0,        // Shift left
    SHR = 0xD1,        // Shift right
    MOVZX = 0x0F,      // Move with Zero Extend
    MOVSX = 0x0F,      // Move with Sign Extend
    PUSH = 0x68,       // Push onto stack
    POP = 0x8F         // Pop from stack
};



