#ifndef __COOL_MONKEY_BUILD_CONFIG_H__
#define __COOL_MONKEY_BUILD_CONFIG_H__

#if defined (COOL_MONKEY_METADATA_PARSER)
    #define MetaData(...) __attribute__((annotate(#__VA_ARGS__)))
#else
    #define MetaData(...)
#endif

#endif