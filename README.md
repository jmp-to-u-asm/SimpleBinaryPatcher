
# SimpleBinaryPatcher

**SimpleBinaryPatcher**

Simple Binary Patching on Runtime



Getting started
===============


A Quick Example
===============

Initialize an instance
--------------------



``` c++

auto Instance = std::make_unique<Engine>();		//an instance for target process

while (!Instance->initialize(target_name))		//waiting for target process
	Sleep(1);
```

The `Engine`  class, instances can be created and initialized for each of the target processes.

Patch memory
--------------------



``` c++
//example 

struct HookData {

	uint64_t offset;
	std::vector<BYTE> payload;
	...
}


HookData hook = { 0x4B04E, makebyte::JMP() };
bool result = Instance->patch_byte(hook);

constexpr auto instruction_begin = 0x4B05E;	 //offset
//start address which the instruction will be applied

constexpr auto instruction_end = 0x4B066;
//end address
constexpr auto inst_size = instruction_end - instruction_begin; //calc size

HookData hook = { instruction_begin , makebyte::NOP(inst_size) };
bool result = Instance->patch_byte(hook);
```
The `patch_byte(HookData& in)` function takes a `HookData` as an argument and overwrites the specified ㅁaddress in the target process with the Payload.

Change VirtualProtect properties of memory region and **patch** then restores the original protection properties.


Payload can be 
`madebyte:: {instruction name}` or  `vector <BYTE> payload = {0x90, 0x90, 0x90};`
`vector <BYTE> payload = {MOV, 0xEB, JMP};` ...

and `patch_byte(HookData& in)` returns the patch result, indicates whether the actual memory is manipulated successful.

`HookData.offset (the first element) `  must be the offset. The base address in memory is calculated by initialization. 

 