#include "kungfu/yijinjing/journal/common.h"
#include "kungfu/yijinjing/timer.hpp"
#include "perftest.hpp"

uint64_t run_client(
        ky::ThreadUnsafePerCountTimer& data_available_timer,
        ky::ThreadUnsafePerCountTimer& call_next_timer,
        ky::ThreadUnsafePerCountTimer& current_frame_timer,
        ky::ThreadUnsafePerCountTimer& data_as_bytes_timer)
{

    uint32_t dest_id = 0;
    auto location = build_journal_path(dest_id);
    ky::ThreadUnsafeTimer creation_timer;
    ky::ThreadUnsafeTimer join_timer;
    kjournal::reader_ptr reader;

    // The parameter, lazy, is set to true here. However it's not used
    // in any place of reader.
    creation_timer.time_it([&]{
            reader = std::make_shared<kjournal::reader>(true); });

    std::cout
        << "time spent on creation(milliseconds): "
        << creation_timer.total_time_milliseconds()
        << std::endl;

    // This should be correct - the parameter goes to
    // page::find_page_id(). If parameter == 0, it starts from
    // page_ids.front().
    int64_t from_time = 0;
    join_timer.time_it([&]{ reader->join(location, dest_id, from_time); });

    std::cout
        << "time spent on join(milliseconds): "
        << join_timer.total_time_milliseconds()
        << std::endl;

    uint64_t data_count = 0;
    bool has_data = false;
    const char* data = nullptr;
    kjournal::frame_ptr frame;

    data_available_timer.time_it([&]{ has_data = reader->data_available(); });
    while (has_data) {
        current_frame_timer.time_it([&]{ frame = reader->current_frame();});
        uint32_t length = frame->data_length();

        // DON'T USE frame->data_as_string()! It has a bug that it
        // assumes end of data is "\0". This is incorrect because server
        // side (writer->write() => writer-> write_raw()) copies only
        // string content via string::c_str() + string::length().
        // It does not contain trailing '\0'.
        data_as_bytes_timer.time_it([&]{ data = frame->data_as_bytes(); });
        uint32_t data_length = frame->data_length();
        std::string string_data(data, data_length);
        std::cout << string_data << std::endl;
        data_count += 1;

        call_next_timer.time_it([&] { reader->next(); });

        data_available_timer.time_it([&]{ has_data = reader->data_available(); });

        // When data lines written to 1 million (1,000,000), there are
        // data lost - the starting line is 171767. I'm not very clear
        // about the reason. Still need further study.
    }
    return data_count;
}
