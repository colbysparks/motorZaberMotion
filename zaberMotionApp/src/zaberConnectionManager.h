#ifndef ZABER_CONNECTION_MANAGER_H
#define ZABER_CONNECTION_MANAGER_H

#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <zaber/motion/ascii/connection.h>

namespace zml = zaber::motion;

class ZaberConnectionManager {
public:
    static ZaberConnectionManager& singleton();
    std::shared_ptr<zml::ascii::Connection> tryGetConnection(const std::string& portName);
    void removeConnection(const std::string& portName, int interfaceId);

private:
    std::mutex _mutex;
    std::unordered_map<std::string, std::weak_ptr<zml::ascii::Connection>> _connections;
    ZaberConnectionManager() = default;

};

#endif
