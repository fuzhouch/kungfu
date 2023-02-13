#include "mimiclocator.hpp"
#include <filesystem>
#include <string>

namespace kungfudemo
{

MimicLocator::MimicLocator(const std::string& home): home_(home) {}

bool MimicLocator::has_env(const std::string& name) const
{
    return std::getenv(name.c_str()) != nullptr;
}

const std::string MimicLocator::get_env(const std::string& name) const
{
    const char* found = std::getenv(name.c_str());
    return found == nullptr? "": std::string(found);
}

const std::string MimicLocator::layout_dir(kdata::location_ptr loc,
        kdata::layout l) const
{

    std::string m = kungfu::yijinjing::data::get_mode_name(loc->mode);
    std::string c = kungfu::yijinjing::data::get_category_name(loc->category);
    std::string la = kungfu::yijinjing::data::get_layout_name(l);
    std::filesystem::path home(home_);
    std::filesystem::path group(loc->group);
    std::filesystem::path name(loc->name);
    std::filesystem::path mode(m);
    std::filesystem::path category(c);
    std::filesystem::path layout(la);
    std::filesystem::path full_path = home / category / group / name / layout / mode;
    std::filesystem::create_directories(full_path);
    return full_path.string();
}

const std::string MimicLocator::layout_file(kdata::location_ptr loc,
        kdata::layout l, const std::string& name) const
{
    std::string layoutdir = layout_dir(loc, l);
    std::string la = kungfu::yijinjing::data::get_layout_name(l);
    std::string filename_pattern = name + "." + la;
    std::filesystem::path layoutfilepath(layoutdir);
    std::filesystem::path layoutfilename(filename_pattern);
    std::filesystem::path full_path = layoutfilepath / layoutfilename;
    return full_path.string();
}

const std::string MimicLocator::default_to_system_db(
        kdata::location_ptr loc,
        const std::string& name) const
{
    std::string layoutdir = layout_dir(loc, kdata::layout::SQLITE);
    std::string filename_pattern = name + "." + 
        kdata::get_layout_name(kdata::layout::SQLITE);
    std::filesystem::path layoutfilepath(layoutdir);
    std::filesystem::path layoutfilename(filename_pattern);
    std::filesystem::path full_path = layoutfilepath / layoutfilename;
    return full_path.string();
    // NOTE: This function does not have fallback logic of creating
    // default /etc/kungfu/live/system/{name}.sqlite file.
    // Make sure you create it. 
}

const std::vector<int> MimicLocator::list_page_id(kdata::location_ptr loc,
        uint32_t dest_id) const
{
    // TODO: Check python/kungfu/yijinjing/journal.py
    // Appears it enumerates all .journal files from given location,
    // but why do we need to do that?
    return std::vector<int>();
}

} // namespace kungfudemo
