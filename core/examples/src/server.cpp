#include <memory>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <kungfu/yijinjing/time.h>
#include <kungfu/yijinjing/journal/journal.h>
#include "mimiclocator.hpp"
#include "consoleprintpublisher.hpp"
#include "perftest.hpp"

namespace kdemo = kungfudemo;

void print_timer_data(const std::string& timer_name, const ky::ThreadUnsafePerCountTimer& timer)
{
    std::cout << "==== BEGIN: " << timer_name << std::endl;
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
    std::cout << "==== END: " << timer_name << std::endl;
}

kdata::location_ptr build_journal_path(uint32_t& dest_id)
{
    std::string root(std::filesystem::current_path().string());
    kdata::locator_ptr locator = std::make_shared<kdemo::MimicLocator>(root);
    // location maintains an important concept: the uid_ member. It's
    // hash value of provided file path. Supposed it can be used to
    // represent a set of journal pages.
    kdata::location_ptr parent_location = kdata::location::make(
            kdata::mode::DATA, 
            kdata::category::SYSTEM,
            "test_group",
            "parent",
            locator);
    dest_id = parent_location->uid;
    std::cout << "parent dest_id: " << std::hex << dest_id << std::dec << std::endl;

    // Locator provides a set of helper functions to build path to
    // underlying maps.
    kdata::location_ptr location = kdata::location::make(
            kdata::mode::LIVE, 
            kdata::category::SYSTEM,
            "test_group",
            "experiment",
            locator);
    return location;
}

kjournal::writer_ptr run_server(uint64_t count)
{
    uint32_t dest_id = 0;
    auto location = build_journal_path(dest_id);

    // Publisher object is required, or it can cause crash on close_frame().
    ky::publisher_ptr publisher = std::make_shared<kdemo::ConsolePrintPublisher>();
    bool lazy = false;
    ky::ThreadUnsafeTimer creation_timer;
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

    writer->open_frame_timer.set_max_call_count(count);
    writer->mmap_copy_timer.set_max_call_count(count);
    writer->close_frame_timer.set_max_call_count(count);

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

    int64_t trigger_time = 0;
    int32_t msg_type = 1; // Biz type in kungfu/wingchun/msg.h
    for (uint64_t i = 0; i < count; ++i) {
        std::stringstream data_stream;
        trigger_time = kungfu::yijinjing::time::now_in_nano();
        // Content to write
        data_stream
            << "i=" << i
            << ",time=" << trigger_time
            << ",data=hello";

        std::cout << data_stream.str() << ", length = " << data_stream.str().length() << std::endl;
        writer->write(trigger_time, msg_type, data_stream.str());
    }
    return writer;
}
