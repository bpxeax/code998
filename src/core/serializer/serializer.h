#ifndef __COOLMONKEY_SERIALIZER_H__
#define __COOLMONKEY_SERIALIZER_H__

#include <iostream>

namespace CoolMonkey
{
    namespace Serializer
    {
        template<typename T>
        void serialize(const T& ) { static_assert(false, "No implementation of type serializer!"); }

        template<>
        void serialize(const int& value)
        {
            std::cout << "int value: " << value << std::endl;
        }

        template<>
        void serialize(const float& value)
        {
            std::cout << "float value: " << value << std::endl;
        }
    }
}

#endif
