/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef FILTERING_H
#define FILTERING_H

/**
 * @file FILTERING.h
 * @brief Defines the nds::filtering node, which provides basic services
 * for digital filtering.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * This is a node that supplies a data PV and few control
 * PV that specifies how the filtering should be performed.
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
class NDS3_API Filtering: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
    Filtering();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a data holder from which the reference to
     *        the object implementation is copied
     */
    Filtering(const Filtering<T>& right);

    Filtering& operator=(const Filtering<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
    Filtering( const std::string& name,                  	   ///< The node's name
					size_t maxElements,                            ///< Maximum size of the array. Set to 1 for scalar values
					stateChange_t switchOnFunction,                ///< Delegate function that performs the actions to switch the node on
					stateChange_t switchOffFunction,               ///< Delegate function that performs the actions to switch the node off
					stateChange_t startFunction,                   ///< Delegate function that performs the actions to start the data generation (usually launches the gneration thread)
					stateChange_t stopFunction,                    ///< Delegate function that performs the actions to stop the data generation(usually stops the generation thread)
					stateChange_t recoverFunction,                 ///< Delegate function to execute to recover from an error state
					allowChange_t allowStateChangeFunction,        ///< Delegate function that can deny a state change. Usually just returns true
					writerInt32_t PV_EnableFilter_Writer,          ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_FilterType_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerVectorInt32_t PV_FilterParams_Writer    ///< Delegate function setter/getter to interact to the Low Level Driver API
					);

    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when tstarts.
     *
     * @param timestampDelegate the function that returns the exact starting
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup datareadwrite
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();

    /**
     * @ingroup timing
     * @brief Returns the timestamp at start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Retrieve the state of the Filter Enable/Disable
     *
     * @return the state of the Filter Enable/Disable
     */
    size_t getEnableFilter();
    /**
     * @brief Retrieve the Filter type
     *
     * @return the Filter type
     */
    size_t getFilterType();
    /**
     * @brief Retrieve the vector of polynomial Coefficients
     *
     * @return the vector of polynomial Coefficients
     */
    std::vector<std::int32_t> getFilterParams();
    /**
     * @brief Sets the state of the Filter Enable/Disable
     *
     */
    void setEnableFilter(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Filter type
     *
     */
    void setFilterType(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the vector of polynomial Coefficients
     *
     */
    void setFilterParams(const timespec& timestamp, const std::vector<std::int32_t>& value);
};

}
#endif // FILTERING_H

