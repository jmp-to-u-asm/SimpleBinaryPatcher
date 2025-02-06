#include "Core.hpp"



constexpr auto target_name = "target.exe";


int main()
{


	pOut("byte patcher ... waiting for target process");
	
	while (!Instance->initialize(target_name))
		Sleep(1);




	
	{
	/*	4B05E                 call    is_auth
		.text:000000000004B063                 test    al, al
		.text : 000000000004B065*/


		//Instance->SuspendAllThreads();

		constexpr auto instruction_begin = 0x4B05E;
		constexpr auto instruction_end = 0x4B066;

		constexpr auto inst_size = instruction_end - instruction_begin;

		HookData first_hook = { instruction_begin , makebyte::NOP(inst_size) };
		bool is_patched = Instance->patch_byte(first_hook);

		printf("patch status = %d \n", is_patched);

		//Instance->ResumeAllThreads();

		getchar();
	}

	






	













	
}

