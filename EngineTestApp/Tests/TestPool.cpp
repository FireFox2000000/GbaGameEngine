#include "TestPool.h"
#include <iostream>
#include <vector>
#include "engine/base/core/stl/Pool.h"

void TestPool::TestIteration()
{
    Pool<int, 4> pool;

    auto PrintPoolContents = [&pool]() {
        std::cout << "Pool contents: " << std::endl;

        for (int item : pool)
        {
            std::cout << item << std::endl;
        }
    };

    auto PrintConstPoolContents = [](const Pool<int, 4>& pool) {
        std::cout << "Const Pool contents: " << std::endl;

        for (const int& item : pool)
        {
            std::cout << item << std::endl;
        }
    };

    PrintPoolContents();

    int* a = pool.Create(0);
    int* b = pool.Create(1);

    PrintPoolContents();

    pool.Free(a);
    pool.Free(b);

    PrintPoolContents();

    std::vector<int*> items;
    for (int i = 0; i < 16; ++i)
    {
        items.emplace_back(pool.Create(i));
    }

    PrintPoolContents();

    PrintConstPoolContents(pool);
}
