#include <memory>
#include <string>
#include <cstdlib>
#include <kungfu/yijinjing/journal/journal.h>
#include "mimiclocator.hpp"

namespace kjournal = kungfu::yijinjing::journal;
namespace kdata = kungfu::yijinjing::data;
namespace kdemo = kungfudemo;

int main(int argc, char* argv[])
{
    kdata::mode m = kdata::mode::DATA;
    kdata::category c = kdata::category::SYSTEM; // What's MD/TD?
    std::string g = "test_group";
    std::string n = "test_name";
    bool writing = true;
    bool lazy = false;

    kdata::locator_ptr locator(new kdemo::MimicLocator("."));
    kdata::location_ptr location(new kdata::location(m, c, g, n, locator));
    kjournal::journal_ptr journal(new kjournal::journal(location, 0, writing, lazy));
    kjournal::writer_ptr writer(new kjournal::writer(location, 0, lazy, nullptr));
    return 0;
}
