#include <memory>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <kungfu/yijinjing/time.h>
#include <kungfu/yijinjing/journal/journal.h>
#include "mimiclocator.hpp"
#include "consoleprintpublisher.hpp"
#include "timecounter.hpp"

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
    kdata::location_ptr location = kdata::location::make(
            kdata::mode::DATA, 
            kdata::category::SYSTEM,
            "test_group",
            "experiment",
            locator);
    // location maintains an important concept: the uid_ member. It's
    // hash value of provided file path. Supposed it can be used to
    // represent a set of journal pages.

    // The code below shows a typical approach to get initial dest_id.
    // It's copied from wingchun/src/strategy/context.cpp.
    kdata::location_ptr parent_location = kdata::location::make(
            kdata::mode::DATA, 
            kdata::category::SYSTEM,
            "test_group",
            "parent",
            locator);
    uint32_t dest_id = parent_location->uid;
    std::cout << "parent dest_id" << std::hex << dest_id << std::dec << std::endl;

    // Publisher object is required, or it can cause crash on close_frame().
    ky::publisher_ptr publisher = std::make_shared<kdemo::ConsolePrintPublisher>();
    bool lazy = false;
    kdemo::ThreadUnsafeTimer creation_timer;
    kjournal::writer_ptr writer;
    
    creation_timer.time_it([&]{ writer = std::make_shared<kjournal::writer>(
            location,
            dest_id,
            lazy,
            publisher); });

    std::cout 
        << "time spent on creation(milliseconds): "
        << creation_timer.total_time_milliseconds()
        << std::endl;

    // After writer is created, a filer under current folder is created
    // with name like below:
    // 
    //     system/test_group/test_name/data/{dest_id}.{page_id}.journal
    //
    // Every page can setup multiple frames, every frame represents an
    // event (header+data block).
    //
    // The size of page is hardcoded. It's defined in
    // kungfu/yijinjing/journal/page.h, find_page_size(). Three values:
    //
    // 1. category == MD && dest_id == 0: 128MiB
    // 2. category == TD or strategy && dest_id != 0: 4MiB
    // 3. others: 1MiB

    kdemo::ThreadUnsafePerCountTimer timer(1000000);
    // Now we have a file created (a page). We can write data.
    // writer internally hosts a journal data structure, and journal
    // decides which page to write to. The performance is ensured by
    // operating files via mmap.
    int64_t trigger_time = 0;
    int32_t msg_type = 1; // Biz type in kungfu/wingchun/msg.h
    for (int i = 0; i < 1000000; ++i) {
        std::stringstream data_stream;
        trigger_time = kungfu::yijinjing::time::now_in_nano();

        // Content to write
        data_stream
            << "i=" << i
            << ",time=" << trigger_time
            << ",data=hello";

        timer.time_it([&]{
            writer->write(trigger_time, msg_type, data_stream.str());
        });
    }

    std::cout << "count: " << timer.call_count() << std::endl;
    std::cout << "total(ns): " << timer.total_time_ns() << std::endl;
    std::cout << "avg(ns): " << timer.avg_time_ns() << std::endl;
    std::cout << "max(ns): " << timer.max_time_ns() << std::endl;
    std::cout << "min(ns): " << timer.min_time_ns() << std::endl;
    std::cout << "25tile(ns): " << timer.quantile(0.25) << std::endl;
    std::cout << "50tile(ns): " << timer.quantile(0.50) << std::endl;
    std::cout << "75tile(ns): " << timer.quantile(0.75) << std::endl;
    std::cout << "95tile(ns): " << timer.quantile(0.95) << std::endl;
    std::cout << "99tile(ns): " << timer.quantile(0.99) << std::endl;
    std::cout << "99.9tile(ns): " << timer.quantile(0.999) << std::endl;

    // A second challenge is on reader: It provides a join() operation.
    // How does it use seek_time?
    // Let's pause here.


    // Let's design an example benchmark app. Requirements:
    // 1. One producer, multiple consumers.
    // 2. Consumer should run either in and out of process.
    // 3. Measure end-to-end latency
    //
    // Key question: what's the measuring target?
    // - Chain 1: Writer side: event generation -> arrive disk
    // - Chain 2: Reader side: read disk -> data read
    // - End-to-end: From event generation to data read
    //
    // From API, we can safely assume writer->write() has safely written
    // data into disk.
    //
    // As for reader - Use join to add a journal - It support multiple
    // journals and support sort() API - it checks which journal
    // contains last data.
    
    return 0;
}

