// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#include <vector>
#include <string>

#include "zaber/motion/dto/serializable.h"

namespace zaber {
namespace motion {
namespace requests {

class IntArrayResponse: public Serializable {
public:
    std::vector<int> values;

    IntArrayResponse();

    IntArrayResponse(
        std::vector<int> p_values
    );

    bool operator==(const IntArrayResponse& other) const;

    bool operator!=(const IntArrayResponse& other) const {
        return !(*this == other);
    }

    std::vector<int> const& getValues() const;
    void setValues(std::vector<int> p_values);

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