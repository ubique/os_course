//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
#define HW5_CLIENT_SERVER_INETSOCKETADDRESS_H

#include <string>
#include <netinet/in.h>
#include "InetUtils.h"

using SystemSocketAddress = struct sockaddr_in;

class InetSocketAddress {
 public:
    int port;
    std::string host;
    explicit InetSocketAddress(
            int port, std::string host = InetUtils::kLoopbackAddress
    );
    SystemSocketAddress ToSystemSocketAddress() const;
};

#endif //HW5_CLIENT_SERVER_INETSOCKETADDRESS_H