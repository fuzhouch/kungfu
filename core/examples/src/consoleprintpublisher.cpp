#include <iostream>
#include "consoleprintpublisher.hpp"

namespace kungfudemo
{

int ConsolePrintPublisher::notify()
{
    // In journal writer context, notify() is called as last step of
    // writer->close_frame(). However it seems it can't do much since
    // there's no parameter passed in.
    //
    // A production implementation is in cpp/yijinjing/src/io/io.cpp.
    // It simply decides whether we calls publish("{}") to publish an
    // empty json, or just return 0.
    //
    // What the biz value of this function?
    std::cout << "ConsolePrintPublisher::notify(): called by writer->close_frame()"
        << std::endl;
    return 0;
}

int ConsolePrintPublisher::publish(const std::string& json_message)
{
    // In production cases, publish() method controls IO. In Kungfu
    // contxt, see kungfu::yijinjing::io_device
    std::cout << "ConsolePrintPublisher::publish(): json data: "
        << json_message << std::endl;
    return 0;
}

} // namespace kungfudemo
