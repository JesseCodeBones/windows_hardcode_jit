#include <iostream>
#include <Windows.h>
#include <array>
#include <string>
#include <tuple>


enum JIT : uint8_t{
    MOV_EAX=0xb8
};

class JITInstruction {
public:
  static  std::array<uint8_t, 5> generateMovEAX(uint32_t value) {
        uint8_t value0 = value & 0b11111111;
        uint8_t value1 = value >> 8 & 0b11111111;
        uint8_t value2 = value >> 16 & 0b11111111;
        uint8_t value3 = value >> 24 & 0b11111111;
        return { JIT::MOV_EAX, value0, value1, value2, value3 };
    }
  static  std::array<uint8_t, 1> generateRet() {
      return { 0xc3 };
  }
};

int main(int, char**){
    void * addr = VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    std::array<uint8_t, 6> executable {};
    const auto jit1 = JITInstruction::generateMovEAX(0xffff);
    uint32_t pos = 0;
    for (uint8_t code : jit1) {
        executable[pos] = code;
        pos++;
    }
    const auto jit2 = JITInstruction::generateRet();
    for (uint8_t code : jit2) {
        executable[pos] = code;
        pos++;
    }

    printf("%p\n", addr);
    if (addr != nullptr) {
        std::memcpy(addr, executable.data(), executable.size());
    }
    typedef uint32_t(*f)();
    f executableFunc = (f)addr;
    uint32_t jitValue = executableFunc();
    std::cout << "Hello, from JIT="<<jitValue<<std::endl;
}
