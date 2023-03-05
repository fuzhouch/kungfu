#include <memory>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>
#include <kungfu/yijinjing/time.h>
#include <kungfu/yijinjing/journal/journal.h>
#include "kungfu/yijinjing/timer.hpp"
#include "mimiclocator.hpp"
#include "consoleprintpublisher.hpp"
#include "perftest.hpp"


int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "ERROR: missing workmode. Usage: "
            << argv[0] << ": <server|client>"
            << std::endl;
        return 1;
    }

    std::string workmode(argv[1]);
    uint64_t loop_count = 1000000;
    if (workmode == "server")
    {
        auto writer = run_server(loop_count);
        print_timer_data("writer->open_frame_timer", writer->open_frame_timer);
        print_timer_data("writer->mmap_copy_timer", writer->mmap_copy_timer);
        print_timer_data("writer->close_frame_timer", writer->close_frame_timer);
    }
    else if (workmode == "client")
    {
        ky::ThreadUnsafePerCountTimer data_available_timer(loop_count);
        ky::ThreadUnsafePerCountTimer call_next_timer(loop_count);
        ky::ThreadUnsafePerCountTimer current_frame_timer(loop_count);
        ky::ThreadUnsafePerCountTimer data_as_bytes_timer(loop_count);
        uint64_t read_data_timer = run_client(data_available_timer, call_next_timer, current_frame_timer, data_as_bytes_timer);
        std::cout << "reader: data count: " << read_data_timer << std::endl;
        print_timer_data("reader->data_available_timer", data_available_timer);
        print_timer_data("reader->call_next_timer", call_next_timer);
        print_timer_data("reader->current_frame_timer", current_frame_timer);
        print_timer_data("reader->data_as_bytes_timer", data_as_bytes_timer);
    }
    else
    {
        std::cout << "ERROR: Unknown workmode: " << workmode << std::endl;
        return 2;
    }

    return 0;
}

