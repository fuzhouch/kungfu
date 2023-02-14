#ifndef _MIMIC_LOCATOR_HPP_
#define _MIMIC_LOCATOR_HPP_

#include <string>
#include <kungfu/yijinjing/common.h>
#include <kungfu/yijinjing/journal/journal.h>

namespace kungfudemo
{

namespace kjournal = kungfu::yijinjing::journal;
namespace kdata = kungfu::yijinjing::data;

// MimicLocator mimics logic of core/python/kungfu/yijinjing/journal.py.
class MimicLocator : public kdata::locator {
public:
    MimicLocator(const std::string& home);
    ~MimicLocator() = default;

    bool has_env(const std::string& name) const override;
    const std::string get_env(const std::string& name) const override;
    const std::string layout_dir(kdata::location_ptr loc,
            kdata::layout l) const override;
    const std::string layout_file(kdata::location_ptr loc,
            kdata::layout l,
            const std::string& name) const override;

    const std::string default_to_system_db(kdata::location_ptr loc,
            const std::string& name) const override;

    const std::vector<int> list_page_id(kdata::location_ptr loc,
            uint32_t dest_id) const override;

private:
    std::string home_;
};

} // namespace kungfudemo

#endif // _MIMIC_LOCATOR_HPP_
