#pragma once

#include "MemoryAPI.hpp"

struct HookData {

	uint64_t offset;
	std::vector<BYTE> payload;

	void debug() {
		
		for (BYTE& b : payload)
		{
			printf("%02X ", b);
		}
		printf(", offset = 0x%llX \n", offset);
	}


};

namespace makebyte {


	inline std::vector<BYTE> NOP(int count) {
		std::vector<BYTE> nops(count, x86_instructions::NOP);
		return nops;
	}

	inline std::vector<BYTE> JMP() {
		std::vector<BYTE> JMP(x86_instructions::JMP);
		return JMP;
	}
	
	inline std::vector<BYTE> JNZ() {
		std::vector<BYTE> JNZ(x86_instructions::JNZ);
		return JNZ;
	}

	inline std::vector<BYTE> JZ() {
		std::vector<BYTE> JZ(x86_instructions::JZ);
		return JZ;
	}


}


#define debug_log

class Engine {
public:
	bool initialize(PCSTR process_name) {

		procname = process_name;

		process = Mem->lookup_process(process_name);

		imagebase = Mem->get_image_base(process);

		if (imagebase)
		{
			debug();

			return 1;
		}

		return 0;


		
	}

	void debug() {

		printf("[Target] process handle = 0x%llX \n", (uint64_t)process);
		printf("[Target] base address = 0x%llX \n", (uint64_t)imagebase);
		printf("[Target] process name = %s \n", procname.c_str());


	}

	
	HANDLE get_process() {
		return process;
	}

	uintptr_t get_imagebase() {
		return imagebase;
	}

	std::string get_name() {
		return procname;
	}



	

	bool patch_byte(const HookData& in) {
		DWORD pBackup;

		SIZE_T size = in.payload.size();
		LPVOID target_ptr = (LPVOID)((uintptr_t)imagebase + in.offset);

#ifdef debug_log
		HookData original;
		original.offset = in.offset;
		original.payload = get_bytes_of(target_ptr, size);

		printf("\noriginal bytes: ");
		original.debug();
#endif // debug_log


		if (VirtualProtectEx(process, target_ptr, size, PAGE_EXECUTE_READWRITE, &pBackup)) {

			bool result = WriteProcessMemory(process, target_ptr, in.payload.data(), size, nullptr);

			VirtualProtectEx(process, target_ptr, size, pBackup, &pBackup);

			HookData patched;
			patched.offset = in.offset;
			patched.payload = get_bytes_of(target_ptr, size);


#ifdef debug_log
			printf("patched bytes: ");
			patched.debug();
#endif // debug_log

			return patched.payload == in.payload;
		}

		return false; 
	}

	bool SetThreadState(bool suspend = 0) {
		THREADENTRY32 te32;
		te32.dwSize = sizeof(THREADENTRY32);  

		
		HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hThreadSnap == INVALID_HANDLE_VALUE) 
			return false;

		
		if (!Thread32First(hThreadSnap, &te32)) {
			CloseHandle(hThreadSnap); 
			return false; 
		}

		
		do {
			if (te32.th32OwnerProcessID == GetProcessId(process)) {  
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
				if (hThread != NULL) {
					if (suspend)
						SuspendThread(hThread);
					else
						ResumeThread(hThread);

					CloseHandle(hThread);  
				}
			}
		} while (Thread32Next(hThreadSnap, &te32));  

		CloseHandle(hThreadSnap); 
		return true;  
	}







private:

	std::vector<BYTE> get_bytes_of(LPCVOID address, SIZE_T size) {
		std::vector<BYTE> buffer(size);  // size만큼 공간 할당

		SIZE_T bytesRead;
		if (!ReadProcessMemory(process, address, buffer.data(), size, &bytesRead)) 
			return {};

		return buffer; // 읽은 데이터 반환
	}

	uintptr_t imagebase = 0;
	HANDLE process = 0;
	std::string procname = "";



};

auto Instance = std::make_unique<Engine>();
