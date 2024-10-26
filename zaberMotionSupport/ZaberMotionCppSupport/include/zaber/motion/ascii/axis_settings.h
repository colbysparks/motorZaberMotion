﻿// ===== THIS FILE IS GENERATED FROM A TEMPLATE ===== //
// ============== DO NOT EDIT DIRECTLY ============== //

#pragma once

#include <string>
#include <vector>

#include "zaber/motion/units.h"
#include "zaber/motion/ascii/axis.h"
#include "zaber/motion/dto/ascii/get_axis_setting.h"
#include "zaber/motion/dto/ascii/get_axis_setting_result.h"


namespace zaber {
namespace motion {
namespace ascii {

/* Forward Declarations */
class ConversionFactor;


/**
 * Class providing access to various axis settings and properties.
 */
class AxisSettings {
public:
    AxisSettings(Axis axis);

    /**
     * Returns any axis setting or property.
     * For more information refer to the [ASCII Protocol Manual](https://www.zaber.com/protocol-manual#topic_settings).
     * @param setting Name of the setting.
     * @param unit Units of setting.
     * @return Setting value.
     */
    double get(const std::string& setting, Units unit = Units::NATIVE);

    /**
     * Sets any axis setting.
     * For more information refer to the [ASCII Protocol Manual](https://www.zaber.com/protocol-manual#topic_settings).
     * @param setting Name of the setting.
     * @param value Value of the setting.
     * @param unit Units of setting.
     */
    void set(const std::string& setting, double value, Units unit = Units::NATIVE);

    /**
     * Returns any axis setting or property as a string.
     * For more information refer to the [ASCII Protocol Manual](https://www.zaber.com/protocol-manual#topic_settings).
     * @param setting Name of the setting.
     * @return Setting value.
     */
    std::string getString(const std::string& setting);

    /**
     * Sets any axis setting as a string.
     * For more information refer to the [ASCII Protocol Manual](https://www.zaber.com/protocol-manual#topic_settings).
     * @param setting Name of the setting.
     * @param value Value of the setting.
     */
    void setString(const std::string& setting, const std::string& value);

    /**
     * Convert arbitrary setting value to Zaber native units.
     * @param setting Name of the setting.
     * @param value Value of the setting in units specified by following argument.
     * @param unit Units of the value.
     * @return Setting value.
     */
    double convertToNativeUnits(const std::string& setting, double value, Units unit);

    /**
     * Convert arbitrary setting value from Zaber native units.
     * @param setting Name of the setting.
     * @param value Value of the setting in Zaber native units.
     * @param unit Units to convert value to.
     * @return Setting value.
     */
    double convertFromNativeUnits(const std::string& setting, double value, Units unit);

    /**
     * Returns the default value of a setting.
     * @param setting Name of the setting.
     * @param unit Units of setting.
     * @return Default setting value.
     */
    double getDefault(const std::string& setting, Units unit = Units::NATIVE);

    /**
     * Returns the default value of a setting as a string.
     * @param setting Name of the setting.
     * @return Default setting value.
     */
    std::string getDefaultString(const std::string& setting);

    /**
     * Indicates if given setting can be converted from and to native units.
     * @param setting Name of the setting.
     * @return True if unit conversion can be performed.
     */
    bool canConvertNativeUnits(const std::string& setting);

    /**
     * Overrides default unit conversions.
     * Conversion factors are specified by setting names representing underlying dimensions.
     * Requires at least Firmware 7.30.
     * @param conversions Factors of all conversions to override.
     */
    void setCustomUnitConversions(const std::vector<ConversionFactor>& conversions);

    /**
     * Gets many setting values in as few requests as possible.
     * @param axisSettings The settings to read.
     * @return The setting values read.
     */
    std::vector<GetAxisSettingResult> getMany(std::initializer_list<GetAxisSetting> axisSettings);

    template<typename... T>
    std::vector<GetAxisSettingResult> getMany(T&&... axisSettings) {
        return getMany({std::forward<T>(axisSettings)...});
    }

    /**
     * Gets many setting values in the same tick, ensuring their values are synchronized.
     * Requires at least Firmware 7.35.
     * @param axisSettings The settings to read.
     * @return The setting values read.
     */
    std::vector<GetAxisSettingResult> getSynchronized(std::initializer_list<GetAxisSetting> axisSettings);

    template<typename... T>
    std::vector<GetAxisSettingResult> getSynchronized(T&&... axisSettings) {
        return getSynchronized({std::forward<T>(axisSettings)...});
    }

protected:
    Axis getAxis() const;

    Axis _axis;
};


}  // Namespace ascii
}  // Namespace motion
}  // Namespace zaber
