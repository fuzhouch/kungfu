#include <memory>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>
#include <kungfu/yijinjing/time.h>
#include <kungfu/yijinjing/journal/journal.h>
#include "mimiclocator.hpp"
#include "consoleprintpublisher.hpp"
#include "perftest.hpp"

int main(int argc, char* argv[])
{
    if (argc == 0)
    {
        std::cout << "ERROR: missing workmode. Usage: "
            << argv[0] << ": <server|client>"
            << std::endl;
    }
    auto writer = run_server(1000000);
    print_timer_data("writer->open_frame_timer", writer->open_frame_timer);
    print_timer_data("writer->mmap_copy_timer", writer->mmap_copy_timer);
    print_timer_data("writer->close_frame_timer", writer->close_frame_timer);

    return 0;
}

