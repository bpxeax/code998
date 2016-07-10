#include <iostream>

extern "C"
{
#include "uv.h"
}

long long counter = 0;

void idle_handler(uv_idle_t* idler)
{
    counter++;

    if (counter >= 10e6)
    {
        uv_idle_stop(idler);
    }
}

int main()
{
    uv_idle_t idler;

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, idle_handler);

    std::cout << "Idling..." << std::endl;

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_loop_close(uv_default_loop());

    return 0;
}