#include <iostream>
#include "consoleprintpublisher.hpp"

namespace kungfudemo
{

int ConsolePrintPublisher::notify()
{
    std::cout << "ConsolePrintPublisher::notify(): called by writer->close_frame()"
        << std::endl;
    return 0;
}

int ConsolePrintPublisher::publish(const std::string& json_message)
{
    std::cout << "ConsolePrintPublisher::publish(): json data: "
        << json_message << std::endl;
    return 0;
}

} // namespace kungfudemo
