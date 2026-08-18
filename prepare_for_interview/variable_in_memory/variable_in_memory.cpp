#include <bits/stdc++.h>
#include <unistd.h> // for getpid()

// Globals
int g_uninit_global;                    // uninitialized -> BSS
int g_init_global = 42;                 // initialized -> DATA
static int g_static_uninit;             // static uninitialized -> BSS
static int g_static_init = 99;          // static initialized -> DATA
const int g_const_global = 7;           // likely in .rodata
volatile int g_volatile_global = 8;     // volatile global
const char *g_str_literal = "string literal"; // in rodata/text

void sample_function() {}

int &get_static_local()
{
    static int s_local = 12345; // static local -> BSS/DATA
    return s_local;
}

// Try to find which /proc/self/maps line contains the given address
std::string find_mapping(void *p)
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    std::ifstream maps("/proc/self/maps");
    std::string line;
    while (std::getline(maps, line)) {
        unsigned long start = 0, end = 0;
        if (std::sscanf(line.c_str(), "%lx-%lx", &start, &end) == 2) {
            if (addr >= start && addr < end) return line;
        }
    }
    return std::string();
}

void print_addr(const char *label, void *p)
{
    std::cout << std::left << std::setw(30) << label
              << "address=" << p << "  ";
    auto m = find_mapping(p);
    if (!m.empty()) std::cout << "map: " << m;
    else std::cout << "map: <not found>";
    std::cout << '\n';
}

int main()
{
    std::cout << "PID=" << getpid() << "\n";

    // Stack variables
    int stack_var = 1;
    const int stack_const = 2;
    volatile int stack_volatile = 3;

    // Heap allocations
    int *heap_int = new int(55);
    void *heap_malloc = std::malloc(64);

    // Local static (inside function)
    int &static_local = get_static_local();

    // Lambda/static inside lambda to get another static
    static auto lambda_static = [](){ static int v = 777; return &v; }();

    // Print many addresses and try to identify their mappings
    print_addr("g_uninit_global (BSS)", &g_uninit_global);
    print_addr("g_init_global   (DATA)", &g_init_global);
    print_addr("g_static_uninit (BSS)", &g_static_uninit);
    print_addr("g_static_init   (DATA)", &g_static_init);
    print_addr("g_const_global  (RODATA)", (void *)&g_const_global);
    print_addr("g_volatile_global", (void *)&g_volatile_global);
    print_addr("g_str_literal   (RODATA)", (void *)g_str_literal);
    print_addr("sample_function (TEXT)", (void *)&sample_function);
    print_addr("main (TEXT)", (void *)&main);

    print_addr("stack_var (STACK)", &stack_var);
    print_addr("stack_const (STACK)", (void *)&stack_const);
    print_addr("stack_volatile (STACK)", (void *)&stack_volatile);

    print_addr("heap_int (HEAP)", heap_int);
    print_addr("heap_malloc (HEAP)", heap_malloc);

    print_addr("static_local (STATIC LOCAL)", &static_local);
    print_addr("lambda_static (STATIC LOCAL)", lambda_static);

    // show some container internal allocation
    std::vector<int> v;
    v.reserve(4);
    v.push_back(1);
    print_addr("vector data (HEAP)", v.data());

    std::free(heap_malloc);
    delete heap_int;
    return 0;
}

