#ifndef HYPERSONIC_TESTS_H
#define HYPERSONIC_TESTS_H

#ifdef USE_TESTS

bool unit_tests();
namespace cgame
{
    bool unit_tests_common();
}

#else

bool unit_tests() {return true;}

namespace cgame
{
    bool unit_tests_common() {return true;}
}

#endif

#endif
