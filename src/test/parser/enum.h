#include "core/compile/common_macros.h"

enum class enumTest
{
    __state_look,
    __state_run MetaData("enum_field"),
    __state_walk
};

unsigned int test()
{
    enum
    {
        you = 0xffffffff
    };

    return you;
}