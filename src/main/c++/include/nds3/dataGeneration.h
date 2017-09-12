/*
 * Nominal Device Support v.3 (NDS3)
 *
 * By GMV & UPM
 */

#ifndef NDSDATAGENERATION_H
#define NDSDATAGENERATION_H

/**
 * @file dataGeneration.h
 * @brief Defines the nds::DataGeneration node, which provides basic services for
 *  AWG support.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * This is a node that supplies data generation PVs and few control
 * PVs configure and control an standard AWG.
 *
 * It also provides a state machine that allows to start/stop the data generation.
 *
 * The user of a DataGeneration class must declare few delegate functions that
 *  specify the actions to perform when the AWG node's state changes.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::double
 *            - std::vector<std::int8_t>
 *            - std::vector<std::uint8_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<double>
 *            - std::string
 *
 */
template <typename T>
class NDS3_API DataGeneration: public Node
{
public:
    /**
     * @brief Initializes an empty data generation node.
     *
     * You must assign a valid DataGeneration node before calling initialize().
     */
    DataGeneration();

    /**
     * @brief Copies a data generation reference from another object.
     *
     * @param right a data generation holder from which the reference to
     *        the generation object implementation is copied
     */
    DataGeneration(const DataGeneration<T>& right);

    DataGeneration& operator=(const DataGeneration<T>& right);

    /**
     * @brief Constructs the data generation node.
     *
     */
    DataGeneration( const std::string& name,                  ///< The node's name
					size_t maxElements,                       ///< Maximum size of the array. Set to 1 for scalar values
					stateChange_t switchOnFunction,           ///< Delegate function that performs the actions to switch the node on
					stateChange_t switchOffFunction,          ///< Delegate function that performs the actions to switch the node off
					stateChange_t startFunction,              ///< Delegate function that performs the actions to start the data generation (usually launches the gneration thread)
					stateChange_t stopFunction,               ///< Delegate function that performs the actions to stop the data generation(usually stops the generation thread)
					stateChange_t recoverFunction,            ///< Delegate function to execute to recover from an error state
					allowChange_t allowStateChangeFunction,   ///< Delegate function that can deny a state change. Usually just returns true
					writerDouble_t PV_Frequency_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Frequency_Reader,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_RefFrequency_Writer,    ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_RefFrequency_Reader,    ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Amp_Writer,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Amp_Reader,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Phase_Writer,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Phase_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_UpdateRate_Writer,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_UpdateRate_Reader,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_DutyCycle_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_DutyCycle_Reader,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Gain_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Gain_Reader,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Offset_Writer,          ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Offset_Reader,          ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Bw_Writer,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Bw_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Resolution_Writer,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Resolution_Reader,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Impedance_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_Impedance_Reader,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_Coupling_Writer,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_Coupling_Reader,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SignalRef_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_SignalRef_Reader,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SignalType_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_SignalType_Reader,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_Ground_Writer,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_Ground_Reader);          ///< Delegate function setter/getter to interact to the Low Level Driver API

    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the data Generation starts.
     *
     * @param
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup datareadwrite
     * @brief Push acquired data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data acquisition thread in order to push the acquired data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void write(const timespec& timestamp, const T& data);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at the moment of the start of the generation.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the generation started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @ingroup datareadwrite
     * @brief Push acquired data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data acquisition thread in order to push the acquired data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the DataAcquisition constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();

    /**
     * @brief Retrieve the desidered signal type to be generated.
     *
     * @return the signalType value
     */
    size_t getSignalType();
};

}
#endif // NDSDATAGENERATION_H

