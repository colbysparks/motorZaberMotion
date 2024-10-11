#include "zaberController.h"
#include <epicsExport.h>
#include <epicsThread.h>
#include <iocsh.h>
#include <iostream>
#include <zaber/motion/dto/ascii/device_identity.h>

int zaberController::zaberTotalControllerNumber = 0;

/**
 * Creates a new zaberController object.
 * \param[in] portName          The name of the asyn port that will be created for this driver
 * \param[in] numAxes           The number of axes that this controller supports
 * \param[in] serialPort        The name of the system serial port that the controller is connected to
 * \param[in] movingPollPeriod  The time between polls when any axis is moving
 * \param[in] idlePollPeriod    The time between polls when no axis is moving
 */
zaberController::zaberController(const char *portName, const char *serialPort, int numAxes,
    double movingPollPeriod, double idlePollPeriod) :
        asynMotorController(portName, numAxes, 0, 0, 0, ASYN_CANBLOCK | ASYN_MULTIDEVICE, 1 /* autoconnect */, 0, 0) {
    try {
        connection_ = zaber::motion::ascii::Connection::openNetworkShare(std::string(serialPort), 11421);
        device_ = connection_.getDevice(1);
        device_.identify();
        connection_.setDisconnectedCallback([](const std::shared_ptr<zaber::motion::exceptions::MotionLibException> &) {
            std::cout << "zaberController: disconnected" << std::endl;
        });
    } catch(const std::exception &e) {
        std::cerr << "zaberController: Connection failed: " << e.what() << std::endl;
    }
    for(int i = 0; i < numAxes; i++) {
        new zaberAxis(this, i);
    }
    startPoller(static_cast<double>(movingPollPeriod) / 1000.0, static_cast<double>(idlePollPeriod) / 1000.0, 2);
}

void zaberController::report(FILE *fp, int level) { std::cout << "zaberController: Report called" << std::endl; }

zaber::motion::ascii::Axis zaberController::getDeviceAxis(int axisNo) { return device_.getAxis(axisNo + 1); }

zaberAxis *zaberController::getAxis(asynUser *pasynUser) {
    return static_cast<zaberAxis *>(asynMotorController::getAxis(pasynUser));
}

zaberAxis *zaberController::getAxis(int axisNo) {
    return static_cast<zaberAxis *>(asynMotorController::getAxis(axisNo));
}

/* Profile Moves -- TBD if we want these */
asynStatus zaberController::initializeProfile(size_t maxPoints) {
    std::cout << "zaberController: Profile initialized" << std::endl;
    return asynSuccess;
}

asynStatus zaberController::buildProfile() {
    std::cout << "zaberController: Profile built" << std::endl;
    return asynSuccess;
}

asynStatus zaberController::executeProfile() {
    std::cout << "zaberController: Profile executed" << std::endl;
    return asynSuccess;
}

asynStatus zaberController::abortProfile() {
    std::cout << "zaberController: Profile aborted" << std::endl;
    return asynSuccess;
}

asynStatus zaberController::readbackProfile() {
    std::cout << "zaberController: Profile readback" << std::endl;
    return asynSuccess;
}

/* Code for iocsh registration */

int zaberMotionCreateController(const char *portName, /* ZaberMotion Motor Asyn Port name */
    const char *serialPortName, /* ZaberMotion Serial Asyn Port name */
    int numAxes, /* Number of axes this controller supports */
    int movingPollPeriod, /* Time to poll (msec) when an axis is in motion */
    int idlePollPeriod) /* Time to poll (msec) when an axis is idle. 0 for no polling */
{
    // int priority = epicsThreadPriorityMedium;
    // int stackSize = epicsThreadGetStackSize(epicsThreadStackMedium);
    zaberController *pController = new zaberController(portName, serialPortName, numAxes, static_cast<double>(movingPollPeriod), static_cast<double>(idlePollPeriod));
    (void)pController;
    return (asynSuccess);
}

/* zaberMotionConfig */
static const iocshArg zaberMotionConfigArg0 = {"asyn motor port name", iocshArgString};
static const iocshArg zaberMotionConfigArg1 = {"serial/tcp port name", iocshArgString};
static const iocshArg zaberMotionConfigArg2 = {"number of axes", iocshArgInt};
static const iocshArg zaberMotionConfigArg3 = {"moving poll rate", iocshArgInt};
static const iocshArg zaberMotionConfigArg4 = {"idle poll rate", iocshArgInt};

static const iocshArg *const zaberMotionConfigArgs[5] = {&zaberMotionConfigArg0, &zaberMotionConfigArg1,
    &zaberMotionConfigArg2, &zaberMotionConfigArg3, &zaberMotionConfigArg4};

static const iocshFuncDef zaberMotionControllerDef = {"ZaberMotionCreateController", 5, zaberMotionConfigArgs};

static void zaberMotionCreateControllerCallFunc(const iocshArgBuf *args) {
    zaberMotionCreateController(args[0].sval, args[1].sval, args[2].ival, args[3].ival, args[4].ival);
}

static void zaberMotionAsynRegister(void) {
    iocshRegister(&zaberMotionControllerDef, zaberMotionCreateControllerCallFunc);
}

extern "C" {
epicsExportRegistrar(zaberMotionAsynRegister);
}