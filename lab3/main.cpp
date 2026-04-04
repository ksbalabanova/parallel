#include <iostream>
#include <string>

bool check_bit(int value, int bit) {
    return (value & (1 << bit)) != 0;
}

int get_item(int value, int low_bit, int width) {
    return (value >> low_bit) & ((1 << width) - 1);
}

void cpuid(int info[4], int leaf) {
    asm volatile("cpuid"
        : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3])
        : "a"(leaf), "c"(0));
}

void cpuidex(int info[4], int leaf, int subleaf) {
    asm volatile("cpuid"
        : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3])
        : "a"(leaf), "c"(subleaf));
}

int main() {
    int info[4];

    // EAX=0
    cpuid(info, 0);
    int max_func = info[0];

    char vendor[13] = {};
    *(int*)(vendor + 0) = info[1];
    *(int*)(vendor + 4) = info[3];
    *(int*)(vendor + 8) = info[2];

    std::cout << "EAX=0" << std::endl;
    std::cout << " Vendor: " << vendor << std::endl;
    std::cout << " Max basic function: " << max_func << std::endl;

    // EAX=1
    cpuid(info, 1);
    int eax = info[0], ebx = info[1], ecx = info[2], edx = info[3];

    std::cout << std::endl << "EAX=1" << std::endl;
    std::cout << "EAX = 0x" << std::hex << eax << std::dec << std::endl;
    std::cout << "Stepping ID: " << get_item(eax, 0, 4) << std::endl;
    std::cout << "Model: " << get_item(eax, 4, 4) << std::endl;
    std::cout << "Family: " << get_item(eax, 8, 4) << std::endl;
    std::cout << "Processor type: " << get_item(eax, 12, 2) << std::endl;
    std::cout << "Extended model: " << get_item(eax, 16, 4) << std::endl;
    std::cout << "Extended family: " << get_item(eax, 20, 8) << std::endl;

    std::cout << std::endl << " EBX:" << std::endl;
    std::cout << "Local APIC ID: " << get_item(ebx, 24, 8) << std::endl;
    std::cout << "Max logical proc/core: " << get_item(ebx, 16, 8) << std::endl;

    std::cout << std::endl << "EDX flags:" << std::endl;
    std::cout << "FPU: " << check_bit(edx, 0) << std::endl;
    std::cout << "TSC: " << check_bit(edx, 4) << std::endl;
    std::cout << "MMX: " << check_bit(edx, 23) << std::endl;
    std::cout << "SSE: " << check_bit(edx, 25) << std::endl;
    std::cout << "SSE2: " << check_bit(edx, 26) << std::endl;
    std::cout << "HTT: " << check_bit(edx, 28) << std::endl;

    std::cout << std::endl << "ECX flags:" << std::endl;
    std::cout << "SSE3: " << check_bit(ecx, 0) << std::endl;
    std::cout << "SSSE3: " << check_bit(ecx, 9) << std::endl;
    std::cout << "FMA3: " << check_bit(ecx, 12) << std::endl;
    std::cout << "SSE4.1: " << check_bit(ecx, 19) << std::endl;
    std::cout << "SSE4.2: " << check_bit(ecx, 20) << std::endl;
    std::cout << "AVX: " << check_bit(ecx, 28) << std::endl;

    // EAX=4
    std::cout << std::endl << "EAX=4: caches" << std::endl;
    for (int i = 0; i < 10; i++) {
        cpuidex(info, 4, i);
        int cache_type = get_item(info[0], 0, 5);
        if (cache_type == 0) break;

        std::string type_name;
        if (cache_type == 1) type_name = "Data";
        else if (cache_type == 2) type_name = "Instruction";
        else type_name = "Unified";

        int level = get_item(info[0], 5, 3);
        int threads_per_cache = get_item(info[0], 14, 12) + 1;
        int line_size = get_item(info[1], 0, 12) + 1;
        int partitions= get_item(info[1], 12, 10) + 1;
        int ways = get_item(info[1], 22, 10) + 1;
        int sets = info[2] + 1;
        bool inclusive = check_bit(info[3], 1);
        int size_kb = (line_size * partitions * ways * sets) / 1024;

        std::cout << std::endl << "[" << i << "] L" << level << " " << type_name << std::endl;
        std::cout << "Threads per cache: " << threads_per_cache << std::endl;
        std::cout << "Line size: " << line_size << " B" << std::endl;
        std::cout << "Ways: " << ways << std::endl;
        std::cout << "Sets: " << sets << std::endl;
        std::cout << "Size: " << size_kb << " KB" << std::endl;
        std::cout << "Inclusive: " << (inclusive ? "yes" : "no") << std::endl;
    }

    // EAX=7
    if (max_func >= 7) {
        cpuidex(info, 7, 0);
        std::cout << std::endl << "EAX=7, ECX=0" << std::endl;
        std::cout << "AVX2: " << check_bit(info[1], 5) << std::endl;
        std::cout << "AVX512f: " << check_bit(info[1], 16) << std::endl;
        std::cout << "SHA: " << check_bit(info[1], 29) << std::endl;
        std::cout << "GFNI: " << check_bit(info[2], 8) << std::endl;
    }

    // EAX=16h
    if (max_func >= 0x16) {
        cpuid(info, 0x16);
        std::cout << std::endl << "EAX=16h: frequencies" << std::endl;
        std::cout << "Base freq: " << (info[0] & 0xFFFF) << " MHz" << std::endl;
        std::cout << "Max freq: " << (info[1] & 0xFFFF) << " MHz" << std::endl;
        std::cout << "Bus freq: " << (info[2] & 0xFFFF) << " MHz" << std::endl;
    }

    // EAX=80000002h..4h
    char brand[49] = {};
    cpuid((int*)(brand + 0),  0x80000002);
    cpuid((int*)(brand + 16), 0x80000003);
    cpuid((int*)(brand + 32), 0x80000004);
    std::cout << std::endl << "CPU brand: " << brand << std::endl;

    return 0;
}
