#include <iostream>
#include <Windows.h>
#include <array>
#include <string>
#include <tuple>

int main(int, char**){
    void * addr = VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    const std::array<uint8_t, 6> executable {
        0xb8, 42, 0, 0, 0, // mov eax, 42
        0xc3,              // ret
    };
    printf("%p\n", addr);
    if (addr != nullptr) {
        std::memcpy(addr, executable.data(), executable.size());
    }
    typedef uint32_t(*f)();
    f executableFunc = (f)addr;
    uint32_t jitValue = executableFunc();
    std::cout << "Hello, from JIT="<<jitValue<<std::endl;
}
