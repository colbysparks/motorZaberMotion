// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#include <string>

#include "zaber/motion/dto/serializable.h"

namespace zaber {
namespace motion {
namespace requests {

class ChannelOn: public Serializable {
public:
    int interfaceId {0};
    int device {0};
    int axis {0};
    bool on {false};

    ChannelOn();

    ChannelOn(
        int p_interfaceId,
        int p_device,
        int p_axis,
        bool p_on
    );

    bool operator==(const ChannelOn& other) const;

    bool operator!=(const ChannelOn& other) const {
        return !(*this == other);
    }

    int getInterfaceId() const;
    void setInterfaceId(int p_interfaceId);

    int getDevice() const;
    void setDevice(int p_device);

    int getAxis() const;
    void setAxis(int p_axis);

    bool getOn() const;
    void setOn(bool p_on);

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
