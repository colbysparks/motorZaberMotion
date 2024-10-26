// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#include <string>

#include "zaber/motion/dto/serializable.h"

namespace zaber {
namespace motion {
namespace requests {

class StreamSetupStoreArbitraryAxesRequest: public Serializable {
public:
    int interfaceId {0};
    int device {0};
    int streamId {0};
    bool pvt {false};
    int streamBuffer {0};
    int pvtBuffer {0};
    int axesCount {0};

    StreamSetupStoreArbitraryAxesRequest();

    StreamSetupStoreArbitraryAxesRequest(
        int p_interfaceId,
        int p_device,
        int p_streamId,
        bool p_pvt,
        int p_streamBuffer,
        int p_pvtBuffer,
        int p_axesCount
    );

    bool operator==(const StreamSetupStoreArbitraryAxesRequest& other) const;

    bool operator!=(const StreamSetupStoreArbitraryAxesRequest& other) const {
        return !(*this == other);
    }

    int getInterfaceId() const;
    void setInterfaceId(int p_interfaceId);

    int getDevice() const;
    void setDevice(int p_device);

    int getStreamId() const;
    void setStreamId(int p_streamId);

    bool getPvt() const;
    void setPvt(bool p_pvt);

    int getStreamBuffer() const;
    void setStreamBuffer(int p_streamBuffer);

    int getPvtBuffer() const;
    void setPvtBuffer(int p_pvtBuffer);

    int getAxesCount() const;
    void setAxesCount(int p_axesCount);

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
