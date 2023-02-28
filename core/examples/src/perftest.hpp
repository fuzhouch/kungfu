#ifndef _PERFORMANCE_TEST_HPP_
#define _PERFORMANCE_TEST_HPP_

#include <kungfu/yijinjing/timer.hpp>
#include <kungfu/yijinjing/journal/journal.h>
#include <string>

namespace ky = kungfu::yijinjing;
namespace kjournal = kungfu::yijinjing::journal;
namespace kdata = kungfu::yijinjing::data;
namespace kdemo = kungfudemo;

extern kdata::location_ptr build_journal_path(uint32_t& dest_id);
extern kjournal::writer_ptr run_server(uint64_t count);

extern void print_timer_data(const std::string& timer_name,
        const ky::ThreadUnsafePerCountTimer& timer);

#endif // _PERFORMANCE_TEST_HPP_
