// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#pragma push_macro("error")
#undef error

#include <optional>
#include <string>
#include <vector>

#include "zaber/motion/dto/serializable.h"
#include "zaber/motion/dto/ascii/can_set_state_axis_response.h"

namespace zaber {
namespace motion {
namespace ascii {

/**
 * An object containing any setup issues that will prevent setting a state to a given device.
 */
class CanSetStateDeviceResponse: public Serializable {
public:
    std::optional<std::string> error;
    std::vector<CanSetStateAxisResponse> axisErrors;

    CanSetStateDeviceResponse();

    CanSetStateDeviceResponse(
        std::optional<std::string> p_error,
        std::vector<CanSetStateAxisResponse> p_axisErrors
    );

    CanSetStateDeviceResponse(
        std::vector<CanSetStateAxisResponse> p_axisErrors
    );

    bool operator==(const CanSetStateDeviceResponse& other) const;

    bool operator!=(const CanSetStateDeviceResponse& other) const {
        return !(*this == other);
    }

    /**
     * The error blocking applying this state to the given device.
     */
    std::optional<std::string> const& getError() const;
    void setError(std::optional<std::string> p_error);

    /**
     * A list of errors that block setting state of device's axes.
     */
    std::vector<CanSetStateAxisResponse> const& getAxisErrors() const;
    void setAxisErrors(std::vector<CanSetStateAxisResponse> p_axisErrors);

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

#pragma pop_macro("error")
