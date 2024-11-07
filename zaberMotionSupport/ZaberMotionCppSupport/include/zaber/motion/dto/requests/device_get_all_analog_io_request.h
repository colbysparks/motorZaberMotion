// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#include <string>

#include "zaber/motion/dto/serializable.h"

namespace zaber {
namespace motion {
namespace requests {

class DeviceGetAllAnalogIORequest: public Serializable {
public:
    int interfaceId {0};
    int device {0};
    std::string channelType;

    DeviceGetAllAnalogIORequest();

    DeviceGetAllAnalogIORequest(
        int p_interfaceId,
        int p_device,
        std::string p_channelType
    );

    bool operator==(const DeviceGetAllAnalogIORequest& other) const;

    bool operator!=(const DeviceGetAllAnalogIORequest& other) const {
        return !(*this == other);
    }

    int getInterfaceId() const;
    void setInterfaceId(int p_interfaceId);

    int getDevice() const;
    void setDevice(int p_device);

    std::string const& getChannelType() const;
    void setChannelType(std::string p_channelType);

    /**
     * Convert object to human-readable string format
     */
    std::string toString() const;

#ifdef ZML_SERIALIZATION_PUBLIC
public:
#else
private:
#endif

    std::string toByteArray() const override;
    void populateFromByteArray(const std::string& buffer) override;

};

} // namespace requests
} // namespace motion
} // namespace zaber