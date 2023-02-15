#ifndef _CONSOLE_PRINT_PUBLISHER_H_
#define _CONSOLE_PRINT_PUBLISHER_H_

#include <string>
#include "kungfu/yijinjing/common.h"

namespace kungfudemo
{

class ConsolePrintPublisher: public kungfu::yijinjing::publisher
{
public:
    ConsolePrintPublisher() = default;
    virtual ~ConsolePrintPublisher() = default;

    int notify() override;

    int publish(const std::string& json_message) override;
};

} // namespace kungfudemo


#endif // _CONSOLE_PRINT_PUBLISHER_H_
