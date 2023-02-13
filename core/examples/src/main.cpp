#include <memory>
#include <string>
#include <cstdlib>
#include <kungfu/yijinjing/journal/journal.h>
#include "mimiclocator.hpp"

namespace kjournal = kungfu::yijinjing::journal;
namespace kdata = kungfu::yijinjing::data;

int main(int argc, char* argv[])
{
    kdata::mode m = kdata::mode::DATA;
    kdata::category c = kdata::category::SYSTEM; // What's MD/TD?
    std::string g = "test_group";
    std::string n = "test_name";
    kdata::locator_ptr l;
    kdata::location_ptr loc(new kdata::location(m, c, g, n, l));
    return 0;
}
