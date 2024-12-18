#include "zaberAxis.h"
#include "zaberController.h"
#include "zaberUtils.h"

#include <iostream>
#include <string>
#include <vector>

#include <zaber/motion/ascii/axis_settings.h>
#include <zaber/motion/ascii/warning_flags.h>
#include <zaber/motion/ascii/warnings.h>
#include <zaber/motion/dto/ascii/axis_identity.h>
#include <zaber/motion/dto/ascii/axis_type.h>
#include <zaber/motion/dto/ascii/get_axis_setting.h>
#include <zaber/motion/units.h>

#define ZABER_MAX_SPEED = "maxspeed";
#define ZABER_ACCEL = "accel";

const std::unordered_map<std::string, std::string> zaberAxis::ZML_FAULT_TO_MESSAGE = {
    {zml::ascii::warning_flags::CRITICAL_SYSTEM_ERROR, "Critical system error"},
    {zml::ascii::warning_flags::PERIPHERAL_NOT_SUPPORTED, "Peripheral not supported"},
    {zml::ascii::warning_flags::PERIPHERAL_INACTIVE, "Peripheral inactive"},
    {zml::ascii::warning_flags::HARDWARE_EMERGENCY_STOP, "Hardware emergency stop"},
    {zml::ascii::warning_flags::OVERVOLTAGE_OR_UNDERVOLTAGE, "Overvoltage or undervoltage"},
    {zml::ascii::warning_flags::DRIVER_DISABLED_NO_FAULT, "Driver disabled, no fault"},
    {zml::ascii::warning_flags::CURRENT_INRUSH_ERROR, "Current inrush error"},
    {zml::ascii::warning_flags::MOTOR_TEMPERATURE_ERROR, "Motor temperature error"},
    {zml::ascii::warning_flags::DRIVER_DISABLED, "Driver disabled"},
    {zml::ascii::warning_flags::ENCODER_ERROR, "Encoder error"},
    {zml::ascii::warning_flags::INDEX_ERROR, "Index error"},
    {zml::ascii::warning_flags::ANALOG_ENCODER_SYNC_ERROR, "Analog encoder sync error"},
    {zml::ascii::warning_flags::OVERDRIVE_LIMIT_EXCEEDED, "Overdrive limit exceeded"},
    {zml::ascii::warning_flags::STALLED_AND_STOPPED, "Stalled and stopped"},
    {zml::ascii::warning_flags::STREAM_BOUNDS_ERROR, "Stream bounds error"},
    {zml::ascii::warning_flags::INTERPOLATED_PATH_DEVIATION, "Interpolated path deviation"},
    {zml::ascii::warning_flags::LIMIT_ERROR, "Limit error"},
    {zml::ascii::warning_flags::EXCESSIVE_TWIST, "Excessive twist"},
};

zaberAxis::zaberAxis(zaberController *pC, int axisNo) :
        asynMotorAxis(pC, axisNo) {
    pC_ = pC;
    axis_ = pC->getDeviceAxis(axisNo);
    std::function<asynStatus()> action = [this]() {
        asynStatus status = asynSuccess;
        switch(axis_.getAxisType()) {
            case zml::ascii::AxisType::LINEAR:
                lengthUnit_ = zml::Units::LENGTH_MICROMETRES;
                velocityUnit_ = zml::Units::VELOCITY_MICROMETRES_PER_SECOND;
                accelUnit_ = zml::Units::ACCELERATION_MICROMETRES_PER_SECOND_SQUARED;
                break;
            case zml::ascii::AxisType::ROTARY:
                lengthUnit_ = zml::Units::ANGLE_DEGREES;
                velocityUnit_ = zml::Units::ANGULAR_VELOCITY_DEGREES_PER_SECOND;
                accelUnit_ = zml::Units::ANGULAR_ACCELERATION_DEGREES_PER_SECOND_SQUARED;
                break;
            default:
                printf("Zaber Motion Error: Unsupported axis type -- %s\n", zml::ascii::AxisType_toString(axis_.getAxisType()).c_str());
                printf("Please refer to the documentation on supported axis types\n");
                status = asynError;
                break;
        }
        return status;
    };
    zaber::epics::handleException(pC_->pasynUserSelf, action);
}

zaberAxis::~zaberAxis() {}

void zaberAxis::report(FILE *fp, int details) {
    std::function<asynStatus()> action = [this, fp, details]() {
        fprintf(fp, "  Zaber Motion Axis: %d\n", axisNo_);
        fprintf(fp, "    %s\n", axis_.toString().c_str());

        if(details > 0) {
            double velocity, position, acceleration;
            pC_->lock();
            pC_->getDoubleParam(pC_->motorVelocity_, &velocity);
            pC_->getDoubleParam(pC_->motorPosition_, &position);
            pC_->getDoubleParam(pC_->motorAccel_, &acceleration);
            pC_->unlock();

            fprintf(fp, "    Position: %f\n", position);
            fprintf(fp, "    Velocity: %f\n", velocity);
            fprintf(fp, "    Acceleration: %f\n", acceleration);
        }
        return asynSuccess;
    };
    zaber::epics::handleException(pC_->pasynUserSelf, action);
}

/**
 * Perform relative or absolute move.
 *
 * @param position The target position.
 * @param relative If true, the move is relative.
 * @param minVelocity This parameter is ignored.
 * @param maxVelocity Corresponds to `maxspeed` setting in Zaber ASCII protocol.
 * @param acceleration Corresponds to `accel` setting in Zaber ASCII protocol.
 */
asynStatus zaberAxis::move(double position, int relative, double minVelocity, double maxVelocity, double acceleration) {
    (void)minVelocity;

    asynStatus status = asynSuccess;
    if(relative) {
        status = doRelativeMove(position, maxVelocity, acceleration);
    } else {
        status = doAbsoluteMove(position, maxVelocity, acceleration);
    }
    pC_->wakeupPoller();
    return status;
}

/**
 * Move the axis at velocity.
 *
 * @param minVelocity This parameter is ignored.
 * @param maxVelocity Corresponds to maxspeed setting in Zaber ASCII protocol.
 * @param acceleration Corresponds to accel setting in Zaber ASCII protocol.
 */
asynStatus zaberAxis::moveVelocity(double minVelocity, double maxVelocity, double acceleration) {
    (void)minVelocity;

    std::function<asynStatus()> action = [this, maxVelocity, acceleration]() {
        zml::ascii::Axis::MoveVelocityOptions options{
            .acceleration = acceleration,
            .accelerationUnit = accelUnit_};
        axis_.moveVelocity(maxVelocity, velocityUnit_, options);
        return asynSuccess;
    };
    asynStatus status = zaber::epics::handleException(pC_->pasynUserSelf, action);
    pC_->wakeupPoller();
    return status;
}

/**
 * Home the axis.
 *
 * Note that this function ignores the minVelocity, maxVelocity, and acceleration parameters.
 * The homing velocity of a zaber axis is defined as the lesser of `limit.approach.maxspeed` and
 * `maxspeed` settings. If you wish to modify these settings, please do so using Zaber Launcher.
 */
asynStatus zaberAxis::home(double minVelocity, double maxVelocity, double acceleration, int forwards) {
    (void)minVelocity;
    (void)maxVelocity;
    (void)acceleration;
    (void)forwards;

    std::function<asynStatus()> action = [this]() {
        axis_.home(false);
        return asynSuccess;
    };
    asynStatus status = zaber::epics::handleException(pC_->pasynUserSelf, action);
    pC_->wakeupPoller();
    return status;
}

/**
 * Stop the axis with given deceleration.
 *
 * @param acceleration Corresponds to accel setting in Zaber ASCII protocol.
 */
asynStatus zaberAxis::stop(double acceleration) {
    std::function<asynStatus()> action = [this, acceleration]() {
        zml::ascii::Axis::MoveVelocityOptions options{
            .acceleration = acceleration,
            .accelerationUnit = accelUnit_};
        axis_.moveVelocity(0., velocityUnit_, options);
        return asynSuccess;
    };
    asynStatus status = zaber::epics::handleException(pC_->pasynUserSelf, action);
    pC_->wakeupPoller();
    return status;
}

/**
 * Detailed information on polling status updates can be found in project README
 */
asynStatus zaberAxis::poll(bool *moving) {
    std::function<asynStatus()> action = [this, &moving]() {
        *moving = axis_.isBusy();
        setIntegerParam(pC_->motorStatusDone_, static_cast<int>(!*moving));
        setIntegerParam(pC_->motorStatusMoving_, static_cast<int>(*moving));
        setIntegerParam(pC_->motorStatusHomed_, static_cast<int>(axis_.isHomed()));

        double pos = axis_.getPosition(lengthUnit_);
        setDoubleParam(pC_->motorPosition_, pos);
        setIntegerParam(pC_->motorStatusCommsError_, 0);

        // AtHome and Home cannot be inferred from axis state
        setIntegerParam(pC_->motorStatusAtHome_, 0);
        setIntegerParam(pC_->motorStatusHome_, 0);

        // check and clear all flags
        zml::ascii::Warnings warnings = axis_.getWarnings();
        std::unordered_set<std::string> flags = warnings.getFlags();
        warnings.clearFlags();

        if (checkAllFlags(flags)) {
            setIntegerParam(pC_->motorStatusProblem_, 1);
            return asynError;
        }

        setIntegerParam(pC_->motorStatusProblem_, 0);
        return asynSuccess;
    };
    std::function<void()> onError = [this]() {
        setIntegerParam(pC_->motorStatusCommsError_, 1);
        setIntegerParam(pC_->motorStatusProblem_, 1);
    };

    asynStatus status = zaber::epics::handleException(pC_->pasynUserSelf, action, onError);
    callParamCallbacks();
    return status;
}

/**
 * Set the position of the axis. Calls set pos defined in Zaber ASCII protocol.
 *
 * @param position The target position.
 */
asynStatus zaberAxis::setPosition(double position) {
    std::function<asynStatus()> action = [this, position]() {
        axis_.getSettings().set("pos", position, lengthUnit_);
        return asynSuccess;
    };
    return zaber::epics::handleException(pC_->pasynUserSelf, action);
}

/* Private member functions */

asynStatus zaberAxis::doAbsoluteMove(double position, double velocity, double acceleration) {
    std::function<asynStatus()> action = [this, position, velocity, acceleration]() {
        zml::ascii::Axis::MoveAbsoluteOptions options{
            .waitUntilIdle = false,
            .velocity = velocity,
            .velocityUnit = velocityUnit_,
            .acceleration = acceleration,
            .accelerationUnit = accelUnit_};
        axis_.moveAbsolute(position, lengthUnit_, options);
        return asynSuccess;
    };
    return zaber::epics::handleException(pC_->pasynUserSelf, action);
}

asynStatus zaberAxis::doRelativeMove(double distance, double velocity, double acceleration) {
    std::function<asynStatus()> action = [this, distance, velocity, acceleration]() {
        zml::ascii::Axis::MoveRelativeOptions options{
            .waitUntilIdle = false,
            .velocity = velocity,
            .velocityUnit = velocityUnit_,
            .acceleration = acceleration,
            .accelerationUnit = accelUnit_};
        axis_.moveRelative(distance, lengthUnit_, options);
        return asynSuccess;
    };
    return zaber::epics::handleException(pC_->pasynUserSelf, action);
}

bool zaberAxis::checkAllFlags(std::unordered_set<std::string> flags) {
    bool fault = false;
    std::string message = "";

    for (const std::string &flag : flags) {
        if (flag.length() == 0) {
            continue;
        }

        if (flag[0] == 'F') {
            auto it = ZML_FAULT_TO_MESSAGE.find(flag);
            if (it != ZML_FAULT_TO_MESSAGE.end()) {
                message = it->second;
            } else {
                message = "Unrecognized fault";
            }

            fault = true;
            asynPrint(pC_->pasynUserSelf, ASYN_TRACE_ERROR, "Zaber Motion Fault -- %s: %s\n", flag.c_str(), message.c_str());
        } else if (flag == zml::ascii::warning_flags::UNEXPECTED_LIMIT_TRIGGER) {
            fault = true;
            asynPrint(pC_->pasynUserSelf, ASYN_TRACE_WARNING, "Zaber Motion Warning -- %s: %s\n", flag.c_str(), "Limit warning");
        }

        updateStatusFromFlag(flag);
    }
    return fault;
}

void zaberAxis::updateStatusFromFlag(const std::string& flag) {
    if (flag == zml::ascii::warning_flags::STALLED_AND_STOPPED) {
        pC_->setIntegerParam(pC_->motorStatusFollowingError_, 1);
    } else if (flag == zml::ascii::warning_flags::INTERPOLATED_PATH_DEVIATION) {
        pC_->setIntegerParam(pC_->motorStatusFollowingError_, 1);
    } else if (flag == zml::ascii::warning_flags::LIMIT_ERROR) {
        pC_->setIntegerParam(pC_->motorStatusLowLimit_, 1);
        pC_->setIntegerParam(pC_->motorStatusHighLimit_, 1);
    }
}
