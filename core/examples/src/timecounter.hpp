#ifndef _TIME_COUNTER_H_
#define _TIME_COUNTER_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace kungfudemo {

// ThreadUnsafePerCountTimer records every single call. It allows
// more computation than ThreadUnsafeTimer, such as percentile
// computation.
class ThreadUnsafePerCountTimer
{
public:
    ThreadUnsafePerCountTimer(uint64_t max_call_count):
        max_call_count_(max_call_count)
        , call_count_(0)
        , call_duration_ns_vec_(max_call_count, 0)
        , max_call_ns_(0)
        , min_call_ns_(0)
    {
        std::fill_n(std::back_inserter(call_duration_ns_vec_), max_call_count, 0);
    }
    ~ThreadUnsafePerCountTimer() = default;

    template<typename FuncT>
    void time_it(FuncT&& func)
    {
        uint64_t begin = kungfu::yijinjing::time::now_in_nano();
        func();
        uint64_t end = kungfu::yijinjing::time::now_in_nano();
        uint64_t duration = (end - begin);
        call_duration_ns_vec_[call_count_] = duration;
        call_count_ += 1;
    }

    uint64_t call_count()
    {
        return call_count_;
    }

    uint64_t total_time_ns()
    {
        return std::accumulate(
                call_duration_ns_vec_.begin(),
                call_duration_ns_vec_.end(),
                0);
    }

    uint64_t max_time_ns()
    {
        return *(std::max_element(
                call_duration_ns_vec_.begin(),
                call_duration_ns_vec_.end()));
    }

    uint64_t min_time_ns()
    {
        return *(std::min_element(
                call_duration_ns_vec_.begin(),
                call_duration_ns_vec_.end()));
    }

    double avg_time_ns()
    {
        if (call_count_ == 0)
        {
            return 0;
        }
        return ((double)total_time_ns() / call_count_);
    }

    double quantile(double ntile)
    {
        if (ntile < 0 || ntile > 1.0) {
            throw std::invalid_argument("bad ntile value");
        }

        if (call_count_ == 0) {
            throw std::logic_error("call_count must > 0");
        }

        std::vector<uint64_t> sorted(call_duration_ns_vec_);
        std::sort(sorted.begin(), sorted.end()); // By operator "<"

        const double id = (sorted.size() * ntile - 1);
        const size_t low = std::floor(id);
        const size_t high = std::ceil(id);

        if (low == high) {
            return sorted[low];
        }
        return (sorted[low] + sorted[high]) / 2.0; // Get average
    }

    double total_time_ms()
    {
        return double(total_time_ns() / 1000000);
    }

    const std::vector<uint64_t>& duration_seq() {
        return call_duration_ns_vec_;
    }

private:
    uint64_t max_call_count_;
    uint64_t call_count_;
    std::vector<uint64_t> call_duration_ns_vec_;
    uint64_t max_call_ns_;
    uint64_t min_call_ns_;
};

class ThreadUnsafeTimer
{
public:
    ThreadUnsafeTimer():
        call_count_(0),
        total_time_ns_(0),
        max_call_ns_(0),
        min_call_ns_(0) {}

    ~ThreadUnsafeTimer() = default;

    template<typename FuncT>
    void time_it(FuncT&& func)
    {
        call_count_ += 1;
        uint64_t begin = kungfu::yijinjing::time::now_in_nano();
        func();
        uint64_t end = kungfu::yijinjing::time::now_in_nano();
        uint64_t duration = (end - begin);
        total_time_ns_ += duration;
        max_call_ns_ = (max_call_ns_ < duration? duration: max_call_ns_);
        min_call_ns_ = (min_call_ns_ > duration? duration: min_call_ns_);
    }

    uint64_t call_count()
    {
        return call_count_;
    }

    uint64_t total_time_ns()
    {
        return total_time_ns_;
    }

    uint64_t max_time_ns()
    {
        return max_call_ns_;
    }

    uint64_t min_time_ns()
    {
        return min_call_ns_;
    }

    double avg_time_ns()
    {
        if (call_count_ == 0)
        {
            return 0;
        }
        return ((double)total_time_ns_ / call_count_);
    }

    double total_time_microseconds()
    {
        return double(total_time_ns_ / 1000000);
    }

private:
    uint64_t call_count_;
    uint64_t total_time_ns_;
    uint64_t max_call_ns_;
    uint64_t min_call_ns_;
};

} // namespace kungfudemo

#endif // _TIME_COUNTER_H_
