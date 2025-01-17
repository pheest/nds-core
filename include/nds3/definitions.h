/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDS3_DEFINITIONS_H
#define NDS3_DEFINITIONS_H


/**
 * @file definitions.h
 * @brief Defines all the enumeration and common types used across the NDS library.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include <cstdint>
#include <functional>
#include <time.h>
#include <string>
#include <list>
#include <vector>
#include <map>

namespace nds
{



/**
 * @brief Available states, ordered by priority (lowest to higher).
 */
enum class state_t : int
{
    unknown,      ///< The state is Unknown
    off,          ///< The device is switched off
    switchingOff, ///< The device is switched on but is in the process of switching off
    initializing, ///< The device is switched off but is in the process of switching on
    on,           ///< The device is switched on
    stopping,     ///< The device is acquiring data but is in the process of stopping the operations
    starting,     ///< The device is switched on and is starting the operations
    running,      ///< The device is operating
    fault,        ///< An error caused the device to go to FAULT mode
    MAX_STATE_NUM ///< Indicate the number of states in the enumeration
};

/**
 * @brief Available levels of automatic change in the State Machines that are following the state of the father Node.
 */
enum class autoEnable_t : int
{
    none=static_cast<int>(state_t::unknown),          ///< The node won't follow its father STM
    off=static_cast<int>(state_t::off),          ///< The node will follow its father until OFF state
    on=static_cast<int>(state_t::on),          ///< The node will follow its father until ON state
    running=static_cast<int>(state_t::running)   ///< The node will follow its father until RUNNING state

};


/**
 * @brief PV data types
 */
enum class dataType_t
{
    /* The value zero is reserved to static asserts to verify that the correct cpp types are being used */
    dataInt32 = 1,    ///< Signed integer, 32 bits
    dataFloat32,      ///< Float, 32 bits
    dataFloat64,      ///< Float, 64 bits
    dataBoolArray,    ///< Array of Bool
    dataUint8Array,   ///< Array of unsigned 8 bit integers
    dataUint16Array,   ///< Array of unsigned 8 bit integers
    dataUint32Array,   ///< Array of unsigned 8 bit integers
    dataInt8Array,    ///< Array of signed 8 bit integers
    dataInt16Array,   ///< Array of signed 16 bit integers
    dataInt32Array,   ///< Array of signed 32 bit integers
    dataFloat32Array, ///< Array of 32 bit floats
    dataFloat64Array, ///< Array of 64 bit floats
    dataString,       ///< String
    dataTimespec,     ///< Timespec
    dataTimespecArray, ///< Array of timespec
    dataTimestamp,  //< Timestamp structure
    dataInt64,  //< Int 64
    dataInt64Array  //< Array of signed 64 bit integers
};

/**
 * @brief Data type to report the status for updating values in Input PVs.
 */
enum class statusPV_t {
  success = 0,     //!< Value updated successfully.
  timeout = 1,     //!< Timeout.
  overflow = 2,    //!< Overflow.
  disconnected = 3,//!< Disconnected.
  disabled = 4,     //!< Disabled.
  error = 5       //!< Any other error not gathered in previous status.
};

/**
 * @brief Specify how to get the PV value.
 */
enum class scanType_t
{
    periodic, ///< The control system polls the value with a specified frequency
    passive,  ///< The control system polls the value only when needed
    interrupt ///< The device server pushes the value to the control system when it changes
};


/**
 * @ingroup logging
 * @brief Defines the severities of the log information.
 */
enum class logLevel_t: std::uint8_t
{
    debug,   ///< Debug information
    info,    ///< Non critical information
    warning, ///< Warning
    error,   ///< Error
    none     ///< Nothing is logger
};

/**
 * @brief Defines in which direction the data is being transferred.
 */
enum class dataDirection_t
{
    input, ///< The data is being written by the control system and read by the device support
    output ///< The data is being written by the device support and read by the control system
};

/**
 * @ingroup naming
 * @brief Defines the nodes' roles in the tree structure: it is used to build the node's
 *        external name when using the @ref naming_rules.
 */
enum class nodeType_t
{
    generic,           ///< The node's role is not specified
    inputChannel,      ///< The node contains mainly input PVs
    outputChannel,     ///< The node contains mainly output PVs
    dataSourceChannel, ///< The node contains a PV that pushes acquired data
    dataSinkChannel,   ///< The node contains a PV that receives pushed data
    stateMachine       ///< The node implements a state machine
};

/**
 * @ingroup naming
 * @brief Defines an input PV's role: this information is used to build the PV's
 *        external name when using the @ref naming_rules.
 */
enum class inputPvType_t
{
    generic,        ///< The PV's role is not specified
    getLocalState,  ///< The PV is used to retrieve the local state of a state machine
    getGlobalState  ///< The PV is used to retrieve the global state
};

/**
 * @ingroup naming
 * @brief Defines an output PV's role: this information is used to build the PV's
 *        external name when using the @ref naming_rules.
 */
enum class outputPvType_t
{
    generic,       ///< The PV's role is not specified
    setLocalState  ///< The PV is used to change the local state of a state machine
};

/**
 * @ingroup logging
 * @brief If the logging is enabled for the specified severity level then the
 *        macro returns the proper logging stream, otherwise it skips the logging
 *        operation.
 */
#define ndsLogStream(node, logLevel) \
    if(!(node).isLogLevelEnabled(logLevel)) {} else (node).getLogger(logLevel)

/**
 * @ingroup logging
 * @brief Log to the debug stream if the debug log level is enabled on the
 *        selected node.
 *
 * The last logged element must be std::endl.
 *
 * Example:
 * @code
 * ndsDebugStream(node) << "The value has been changed from " << value0 << " to value " << value1 << std::endl
 * @endcode
 */
#define ndsDebugStream(node) ndsLogStream(node, nds::logLevel_t::debug)

/**
 * @ingroup logging
 * @brief Log to the info stream if the info log level is enabled on the
 *        selected node.
 *
 * The last logged element must be std::endl.
 *
 * Example:
 * @code
 * ndsInfoStream(node) << "The value has been changed from " << value0 << " to value " << value1 << std::endl
 * @endcode
 */
#define ndsInfoStream(node) ndsLogStream(node, nds::logLevel_t::info)

/**
 * @ingroup logging
 * @brief Log to the warning stream if the warning log level is enabled on the
 *        selected node.
 *
 * The last logged element must be std::endl.
 *
 * Example:
 * @code
 * ndsWarningStream(node) << "The value could not be changed from " << value0 << " to " << value1 << std::endl
 * @endcode
 */
#define ndsWarningStream(node) ndsLogStream(node, nds::logLevel_t::warning)

/**
 * @ingroup logging
 * @brief Log to the error stream if the error log level is enabled on the
 *        selected node.
 *
 * The last logged element must be std::endl.
 *
 * Example:
 * @code
 * ndsErrorStream(node) << "An error has occurred while updating the value to " << value << std::endl
 * @endcode
 */
#define ndsErrorStream(node) ndsLogStream(node, nds::logLevel_t::error)

/**
 * @brief List of strings passed as parameters to nodes' commands.
 */
typedef std::vector<std::string> parameters_t;

/**
 * @brief Type defined for handling a timestamp with its associated parameters
 *
 * It contains fields to represent:
 * - timestamp
 * - counter to identify the timestamp
 * - boolean to indicate whether the timestamp happens with a rising or falling edge
 */
typedef struct {
    timespec timestamp; //Moment at the timestamp happened
    std::int32_t id; //Identifier of the timestamp
    bool edge; //Type of edge that triggers the timestamp (rising=true; falling=false)
} timestamp_t;

/**
 * @brief Definition of a function called to execute a node's command.
 *
 * It receives a vector of string parameters. It is the responsability of
 *  the delegate function to convert the strings to the appropriate data
 *  types.
 *
 * The number of parameters is verified by NDS before calling the delegate
 *  function.
 */
typedef std::function<parameters_t (const parameters_t& parameters)> command_t;

/**
 * @brief Map containing named parameters passed to the device during the
 *        initialization.
 */
typedef std::map<std::string, std::string> namedParameters_t;

class Factory;

/**
 * @brief Definition for the function executed to allocate a driver.
 *
 * Returns a pointer to the allocated driver: the pointer will be passed
 *  to the function defined by deallocateDriver_t.
 *
 * The allocation delegate should create the device and call initialize()
 *  on its root nodes.
 *
 */
typedef std::function<void*(Factory& factory, const std::string& deviceName, const namedParameters_t& parameters)> allocateDriver_t;

/**
 * @brief Definition for the function executed to deallocate a driver.
 *
 * Takes a pointer to the allocated driver.
 *
 */
typedef std::function<void (void*)> deallocateDriver_t;


/**
 * @brief Definition for the function executed during the state transition.
 *
 * An exception thrown during a state transition will cause the StateMachine to
 *  switch to the state_t::fault state, unless the exception StateMachineRollBack
 *  is thrown which cause a rollback to the previous state.
 */
typedef std::function<void ()> stateChange_t;


/**
 * @brief Definition for a function called to allow or deny a state transition.
 *
 * The function receives 3 parameters:
 * - the first one represents the current state
 * - the second one represents the current global state
 * - the last one represents the desired state
 *
 * The function must return true if the transition from the current state to
 *  the desired state is allowed, or false otherwise.
 *
 * This function is called only after the state machine has verified that
 *  the requested transition is legal.
 */
typedef std::function<bool (const state_t, const state_t, const state_t)> allowChange_t;

/**
 * @ingroup timing
 * @brief Definition for a function called to retrieve a time.
 *
 * The function should return the UNIX epoch (seconds and nanoseconds).
 */
typedef std::function<timespec ()> getTimestampPlugin_t;

typedef std::function<void ()> threadFunction_t;


/**
 * @brief List of strings used for enumeration in PVs that support
 *        the enumeration field.
 */
typedef std::list<std::string> enumerationStrings_t;


typedef std::function<void (timespec* time, std::int32_t* val)> readerInt32_t;
typedef std::function<void (timespec* time, std::int64_t* val)> readerInt64_t;
typedef std::function<void (timespec* time, double* val)> readerDouble_t;
typedef std::function<void (timespec* time, std::vector<bool>* val)> readerVectorBool_t;
typedef std::function<void (timespec* time, std::vector<std::uint8_t>* val)> readerVectorUInt8_t;
typedef std::function<void (timespec* time, std::vector<std::uint16_t>* val)> readerVectorUInt16_t;
typedef std::function<void (timespec* time, std::vector<std::uint32_t>* val)> readerVectorUInt32_t;
typedef std::function<void (timespec* time, std::vector<std::int8_t>* val)> readerVectorInt8_t;
typedef std::function<void (timespec* time, std::vector<std::int16_t>* val)> readerVectorInt16_t;
typedef std::function<void (timespec* time, std::vector<std::int32_t>* val)> readerVectorInt32_t;
typedef std::function<void (timespec* time, std::vector<std::int64_t>* val)> readerVectorInt64_t;
typedef std::function<void (timespec* time, std::vector<double>* val)> readerVectorDouble_t;
typedef std::function<void (timespec* time, std::string* val)> readerString_t;
typedef std::function<void (timespec* time, timespec* val)> readerTime_t;
typedef std::function<void (timespec* time, std::vector<timespec>* val)> readerVectorTime_t;
typedef std::function<void (timespec* time, timestamp_t* val)> readerTimestamp_t;

typedef std::function<void (const timespec&, const std::int32_t&)> writerInt32_t;
typedef std::function<void (const timespec&, const std::int64_t&)> writerInt64_t;
typedef std::function<void (const timespec&, const double&)> writerDouble_t;
typedef std::function<void (const timespec&, const std::vector<bool>&)> writerVectorBool_t;
typedef std::function<void (const timespec&, const std::vector<std::uint8_t>&)> writerVectorUInt8_t;
typedef std::function<void (const timespec&, const std::vector<std::uint16_t>&)> writerVectorUInt16_t;
typedef std::function<void (const timespec&, const std::vector<std::uint32_t>&)> writerVectorUInt32_t;
typedef std::function<void (const timespec&, const std::vector<std::int8_t>&)> writerVectorInt8_t;
typedef std::function<void (const timespec&, const std::vector<std::int16_t>&)> writerVectorInt16_t;
typedef std::function<void (const timespec&, const std::vector<std::int32_t>&)> writerVectorInt32_t;
typedef std::function<void (const timespec&, const std::vector<std::int64_t>&)> writerVectorInt64_t;
typedef std::function<void (const timespec&, const std::vector<double>&)> writerVectorDouble_t;
typedef std::function<void (const timespec&, const std::string&)> writerString_t;
typedef std::function<void (const timespec&, const timespec&)> writerTime_t;
typedef std::function<void (const timespec&, const std::vector<timespec>&)> writerVectorTime_t;
typedef std::function<void (const timespec&, const timestamp_t&)> writerTimestamp_t;
/**
 * @brief Reporter function typedef.
 */
typedef std::function<void (FILE*, int)> reporter_t;
typedef std::function<void (void)> dbParser_t;

} // namespace nds

/**
 * @brief Defines the global functions that are called by NDS when the device class needs
 *        to be allocated.
 *
 * @param driverName   a token that specifies the driver name (input has to be valid c symbol name)
 * @param className    the name of the class that implements the driver
 */
#define NDS_DEFINE_DRIVER(driverName, className)\
extern "C" \
{ \
NDS3_HELPER_DLL_EXPORT void* allocateDevice(nds::Factory& factory, const std::string& device, const nds::namedParameters_t& parameters) \
{ \
    return new className(factory, device, parameters); \
} \
NDS3_HELPER_DLL_EXPORT void deallocateDevice(void* device) \
{ \
    delete (className*)device; \
} \
NDS3_HELPER_DLL_EXPORT const char* getDeviceName() \
{ \
    return #driverName; \
} \
NDS3_HELPER_DLL_EXPORT nds::RegisterDevice<className> registerDevice##driverName(#driverName); \
} // extern "C"

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define NDS3_HELPER_DLL_IMPORT __declspec(dllimport)
  #define NDS3_HELPER_DLL_EXPORT __declspec(dllexport)
#else
  #if __GNUC__ >= 4
    #define NDS3_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define NDS3_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define NDS3_HELPER_DLL_IMPORT
    #define NDS3_HELPER_DLL_EXPORT
  #endif
#endif


// NDS3_API is used for the public API symbols

#ifdef NDS3_DLL // defined if NDS3 is compiled as a DLL
  #ifdef NDS3_DLL_EXPORTS // defined if we are building the NDS3 DLL (instead of using it)
    #define NDS3_API NDS3_HELPER_DLL_EXPORT
  #else
    #define NDS3_API NDS3_HELPER_DLL_IMPORT
  #endif // NDS3_DLL_EXPORTS
#else // NDS3_DLL is not defined: this means NDS3 is a static lib.
  #define NDS3_API
#endif // NDS3_DLL

#endif // NDS3_DEFINITIONS_H
