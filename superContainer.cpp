#include <iostream>
#include <cstddef>
#include <new>
#include <utility>
#include "noisy.h"

template <typename T>
class superContainer
{
  public:
    superContainer() : m_buff(nullptr), m_capacity(0), m_size(0) {}

    superContainer(std::initializer_list<T> l) : superContainer()
    {
        m_buff = new char[l.size() * sizeof(T)];
        m_capacity = m_size = l.size();

        for (int i = 0; i < l.size(); ++i)
        {
            new (m_buff + i * sizeof(T)) T(*(l.begin() + i));
        }
    }

    explicit superContainer(std::size_t size) : superContainer()
    {
        m_buff = new char[size * sizeof(T)];
        m_capacity = m_size = size;

        for (int i = 0; i < size; ++i)
        {
            T *t = new (m_buff + i * sizeof(T)) T;
        }
    }

    ~superContainer()
    {
        destroy();
    }

    superContainer(const superContainer &rhs) : m_buff(nullptr), m_capacity(rhs.m_capacity), m_size(rhs.m_size)
    {
        m_buff = new char[m_size * sizeof(T)];

        for (int i = 0; i < m_size; ++i)
        {
            new (m_buff + i * sizeof(T)) T(rhs[i]);
        }
    }

    superContainer(superContainer &&rhs) : m_buff(nullptr), m_capacity(rhs.m_capacity), m_size(rhs.m_size)
    {
        std::swap(rhs.m_buff, m_buff);
        rhs.m_capacity = rhs.m_size = 0;
    }

    superContainer &
    operator=(const superContainer &rhs)
    {
        if (this != &rhs)
        {
            destroy();
            m_size = rhs.m_size;
            m_capacity = rhs.m_capacity;

            m_buff = new char[m_size * sizeof(T)];

            for (int i = 0; i < m_size; ++i)
            {
                new (m_buff + i * sizeof(T)) T(rhs[i]);
            }
        }
        return *this;
    }

    superContainer &operator=(superContainer &&rhs)
    {
        if (this != &rhs)
        {
            destroy();
            m_size = rhs.m_size;
            m_capacity = rhs.m_capacity;

            m_buff = rhs.m_buff;
            rhs.m_buff = nullptr;
            rhs.m_capacity = rhs.m_size = 0;
        }
        return *this;
    }

    superContainer &
    operator=(std::initializer_list<T> l)
    {
        destroy();
        m_capacity = m_size = l.size();
        m_buff = new char[l.size() * sizeof(T)];

        for (int i = 0; i < l.size(); ++i)
        {
            new (m_buff + i * sizeof(T)) T(*(l.begin() + i));
        }

        return *this;
    }

    T &
    operator[](std::size_t i)
    {
        return *reinterpret_cast<T *>(m_buff + i * sizeof(T));
    }

    const T &operator[](std::size_t i) const
    {
        return *reinterpret_cast<T *>(m_buff + i * sizeof(T));
    }

    void clear()
    {
        for (int i = 0; i < m_size; ++i)
        {
            reinterpret_cast<T *>(std::launder(m_buff)[i * sizeof(T)])->~T();
        }
        m_size = 0;
    }

    void destroy()
    {
        clear();
        delete[] m_buff;
    }

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity > m_capacity)
        {
            char *buff_tmp = new char[new_capacity * sizeof(T)];
            for (int i = 0; i < m_size; ++i)
            {
                new (buff_tmp + i * sizeof(T)) T(std::move(reinterpret_cast<T *>(m_buff)[i]));

                reinterpret_cast<T *>(m_buff[i * sizeof(T)])->~T();
            }
            delete[] m_buff;
            m_buff = buff_tmp;
            m_capacity = new_capacity;
        }
    }

    superContainer &push_back(const T &element)
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity * 2);
        }
        new (m_buff + sizeof(T) * m_size) T(element);
        m_size++;

        return *this;
    }

  private:
    char *m_buff;
    std::size_t m_capacity;
    std::size_t m_size;
};

void foo(const superContainer<Noisy> &v)
{
    std::cout << v[1].i;
}

int main(int argc, char *argv[])
{
    superContainer<Noisy> v{Noisy(), Noisy()};
    superContainer<Noisy> v2;
    v2 = {Noisy(), Noisy(), Noisy()};
    superContainer<Noisy> v3;
    v3 = v;
    v3.reserve(40);
}
