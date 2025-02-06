#pragma once

#include "Common.h"

#include <winternl.h>
#include <iostream>

#pragma comment(lib, "ntdll.lib")



typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(
	HANDLE ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength
	);

class Memory {

public:

	inline HANDLE lookup_process(PCSTR process_name) {
		HANDLE process_handle = 0;

		PROCESSENTRY32 entry{};
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (Process32Next(snapshot, &entry))
		{
			if (!strcmpi_imp(entry.szExeFile, process_name))
			{
				process_handle = OpenProcess(PROCESS_ALL_ACCESS, 0, entry.th32ProcessID);
				break;
			}
		}

		CloseHandle(snapshot);

		return process_handle;
	}

	inline uintptr_t get_image_base(HANDLE hProcess) {


		pNtQueryInformationProcess NtQueryInformationProcess =
			(pNtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");

		if (!NtQueryInformationProcess) {
			//std::cerr << "Failed to get NtQueryInformationProcess function." << std::endl;
			return 0;
		}

		PROCESS_BASIC_INFORMATION pbi;
		ULONG len;
		NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);

		if (status != 0) {
			//std::cerr << "NtQueryInformationProcess failed!" << std::endl;
			return 0;
		}

		PEB peb;
		SIZE_T bytesRead;
		if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), &bytesRead)) {
			//std::cerr << "Failed to read PEB!" << std::endl;
			return 0;
		}

		return (uintptr_t)peb.Reserved3[1];  // ImageBaseAddress
	}





	




};

auto Mem = std::make_unique<Memory>();

