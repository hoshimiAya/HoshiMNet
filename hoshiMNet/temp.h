
#include <spdlog/spdlog.h>

class Test
{
public:
    int sum(int a, int b)
    {
        spdlog::info("summing {} and {}", a, b);
        return a + b;
    }
};