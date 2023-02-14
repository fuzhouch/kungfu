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
    // TODO
    // The logic is translated from python/kungfu/yijinjing/journal.py.
    // The layout value is hardcoded to JOURNAL just following Python
    // code. In our C++ code it's called by page::load(), which is also
    // hardcoded.
    //
    // The behavior seems to be a possible problem when list_page_id()
    // is called by other classes, but let's keep it as is.
    std::string layout_dir_path = layout_dir(loc, kdata::layout::JOURNAL);
    std::filesystem::path dir(layout_dir_path);

    std::vector<int> page_ids;
    for (auto const& dir_entry: std::filesystem::directory_iterator(dir))
    {
        // Note that the logic here is weaker than its python brother.
        // We just check file extension instead of checking file path
        // pattern, but so far it should be fine.
        std::string filename(dir_entry.path().filename());
        std::string ext(dir_entry.path().extension());
        if (ext == ".journal") {
            // Filename format looks like this: NNNNNNNN.MM.journal.
            // The page_id we are looking for is MM. Note that MM is
            // always a number (\d+)
            auto first_dot_pos = filename.find(".");
            auto second_dot_pos = filename.find(".", first_dot_pos + 1);
            if (first_dot_pos == std::string::npos ||
                    second_dot_pos == std::string::npos)
            {
                continue;
            }
            std::string page_id_str = filename.substr(
                    first_dot_pos + 1,
                    second_dot_pos - first_dot_pos - 1);
            int page_id = std::stoi(page_id_str);
            page_ids.push_back(page_id);
        }
    }
    return page_ids;
}

} // namespace kungfudemo
