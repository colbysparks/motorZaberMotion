#ifndef CONNECTION_MOCK_H
#define CONNECTION_MOCK_H

#include <functional>
#include <memory>

#define DEFAULT_PORT 11421

namespace zaber {
namespace motion {

namespace exceptions {
class MotionLibException {
    public:
    MotionLibException() {}
    virtual ~MotionLibException() {}
};
} // exceptions

namespace ascii {

struct Connection {
    // static member function mocks
    static std::function<Connection(const std::string &)> openSerialPortMock;
    static std::function<Connection(const std::string &, int)> openTcpMock;
    int interfaceId = 1;

    void identify() {}
    int getInterfaceId() { return interfaceId; }
    void setDisconnectedCallback(const std::function<void(const std::shared_ptr<zaber::motion::exceptions::MotionLibException> &)> &callback) {
        (void)callback;
    }
    static Connection openSerialPort(const std::string &port) {
        return openSerialPortMock(port);
    }
    static Connection openTcp(const std::string &hostnameOrIp, int port = DEFAULT_PORT) {
        return openTcpMock(hostnameOrIp, port);
    }
};

} // ascii
} // motion
} // zaber

#endif
