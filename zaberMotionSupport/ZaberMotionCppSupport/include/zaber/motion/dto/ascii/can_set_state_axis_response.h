// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#pragma push_macro("error")
#undef error

#include <optional>
#include <string>

#include "zaber/motion/dto/serializable.h"

namespace zaber {
namespace motion {
namespace ascii {

/**
 * An object containing any setup issues that will prevent setting a state to a given axis.
 */
class CanSetStateAxisResponse: public Serializable {
public:
    std::optional<std::string> error;
    int axisNumber {0};

    CanSetStateAxisResponse();

    CanSetStateAxisResponse(
        std::optional<std::string> p_error,
        int p_axisNumber
    );

    CanSetStateAxisResponse(
        int p_axisNumber
    );

    bool operator==(const CanSetStateAxisResponse& other) const;

    bool operator!=(const CanSetStateAxisResponse& other) const {
        return !(*this == other);
    }

    /**
     * The error blocking applying this state to the given axis.
     */
    std::optional<std::string> const& getError() const;
    void setError(std::optional<std::string> p_error);

    /**
     * The number of the axis that cannot be set.
     */
    int getAxisNumber() const;
    void setAxisNumber(int p_axisNumber);

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
