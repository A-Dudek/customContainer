#ifndef NOISY_H
#define NOISY_H
#include <iostream>

struct Noisy
{
    int i;
    Noisy()
    {
        i = 1;
        std::cout << "Noisy()" << std::endl;
    }

    Noisy(const Noisy &)
    {
        i = 1;
        std::cout << "Noisy(const Noisy &)" << std::endl;
    }

    Noisy(Noisy &&) noexcept
    {
        i = 1;
        std::cout << "Noisy(Noisy &&)" << std::endl;
    }

    ~Noisy()
    {
        std::cout << "~Noisy()" << std::endl;
    }

    Noisy &operator=(const Noisy &)
    {
        std::cout << "operator= (const Noisy&)" << std::endl;
        return *this;
    }

    Noisy &operator=(Noisy &&)
    {
        std::cout << "operator= (Noisy&&)" << std::endl;
        return *this;
    }

    void foo()
    {
        std::cout << "foo() " << i << std::endl;
    }
};

#endif
