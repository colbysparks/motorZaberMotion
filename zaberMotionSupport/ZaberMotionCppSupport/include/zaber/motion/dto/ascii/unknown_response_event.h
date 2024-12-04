// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#include <string>

#include "zaber/motion/dto/serializable.h"
#include "zaber/motion/dto/ascii/message_type.h"

namespace zaber {
namespace motion {
namespace ascii {

/**
 * Reply that could not be matched to a request.
 */
class UnknownResponseEvent: public Serializable {
public:
    int deviceAddress {0};
    int axisNumber {0};
    std::string replyFlag;
    std::string status;
    std::string warningFlag;
    std::string data;
    MessageType messageType {0};

    UnknownResponseEvent();

    UnknownResponseEvent(
        int p_deviceAddress,
        int p_axisNumber,
        std::string p_replyFlag,
        std::string p_status,
        std::string p_warningFlag,
        std::string p_data,
        MessageType p_messageType
    );

    bool operator==(const UnknownResponseEvent& other) const;

    bool operator!=(const UnknownResponseEvent& other) const {
        return !(*this == other);
    }

    /**
     * Number of the device that sent the message.
     */
    int getDeviceAddress() const;
    void setDeviceAddress(int p_deviceAddress);

    /**
     * Number of the axis which the response applies to. Zero denotes device scope.
     */
    int getAxisNumber() const;
    void setAxisNumber(int p_axisNumber);

    /**
     * The reply flag indicates if the request was accepted (OK) or rejected (RJ).
     */
    std::string const& getReplyFlag() const;
    void setReplyFlag(std::string p_replyFlag);

    /**
     * The device status contains BUSY when the axis is moving and IDLE otherwise.
     */
    std::string const& getStatus() const;
    void setStatus(std::string p_status);

    /**
     * The warning flag contains the highest priority warning currently active for the device or axis.
     */
    std::string const& getWarningFlag() const;
    void setWarningFlag(std::string p_warningFlag);

    /**
     * Response data which varies depending on the request.
     */
    std::string const& getData() const;
    void setData(std::string p_data);

    /**
     * Type of the reply received.
     */
    MessageType const& getMessageType() const;
    void setMessageType(MessageType p_messageType);

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

} // namespace ascii
} // namespace motion
} // namespace zaber
