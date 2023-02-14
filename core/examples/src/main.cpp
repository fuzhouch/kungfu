#include <memory>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <kungfu/yijinjing/journal/journal.h>
#include "mimiclocator.hpp"
#include "consoleprintpublisher.hpp"

namespace ky = kungfu::yijinjing;
namespace kjournal = kungfu::yijinjing::journal;
namespace kdata = kungfu::yijinjing::data;
namespace kdemo = kungfudemo;

int main(int argc, char* argv[])
{
    // How to use the fast journal --
    // 1. Main entry is writer. A writer instance creates location, page
    //    and journal.
    // 2. Journal is based on concept of pages and frames. Pages map to
    //    physical files.
    //
    // To avoid breaking big environment, let's start from current
    // working directory. For production use, a better idea is to use
    // $HOME.
    // const char* home_dir_cstr = std::getenv("HOME");
    // const std::string root(home_dir_cstr == nullptr? ".": home_dir_cstr);
    std::string root(std::filesystem::current_path().string());
    kdata::locator_ptr locator = std::make_shared<kdemo::MimicLocator>(root);

    // Locator provides a set of helper functions to build path to
    // underlying maps.
    kdata::location_ptr location = std::make_shared<kdata::location>(
                kdata::mode::DATA, 
                kdata::category::SYSTEM,
                "test_group",
                "test_name",
                locator);

    // Now let's create a writer. On creation, a filer under current
    // folder is created, with name like below:
    // 
    //     system/test_group/test_name/data/00000000.1.journal
    //
    // It stores true contents.
    //
    // Publisher is required, or it can cause crash on close_frame().

    ky::publisher_ptr publisher = std::make_shared<kdemo::ConsolePrintPublisher>();
    uint32_t dest_id = 1;
    bool writing = true;
    bool lazy = false;
    kjournal::writer_ptr writer = std::make_shared<kjournal::writer>(location,
            dest_id, lazy, publisher);
    return 0;
}
