/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESUPP_H
#define NDSTIMESUPP_H

/**
 * @file timeSupp.h
 * @brief TBD
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

template <typename T>
class NDS3_API TimeSupp: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
	TimeSupp();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	TimeSupp(const TimeSupp<T>& right);

	TimeSupp& operator=(const TimeSupp<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	TimeSupp(const std::string& name,
			size_t maxElements,                           ///< Number of maximum FTEs provided to timing board
            stateChange_t switchOnFunction,               ///< Delegate function that performs the actions to switch the node on
            stateChange_t switchOffFunction,              ///< Delegate function that performs the actions to switch the node off
            stateChange_t startFunction,                  ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
            stateChange_t stopFunction,                   ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
            stateChange_t recoverFunction,                ///< Delegate function to execute to recover from an error state
            allowChange_t allowStateChangeFunction,       ///< Delegate function that can deny a state change. Usually just returns true
			writerInt32_t PV_clkSrc_Writer,				 ///< Delegate function setter/getter to interact to the Low Level Driver API
			writerDouble_t PV_clkFreq_Writer,			///< Delegate function setter/getter to interact to the Low Level Driver API
			writerInt32_t PV_clkMult_Writer,			///< Delegate function setter/getter to interact to the Low Level Driver API
			writerInt32_t PV_MaxSchFTEs_Writer,			///< Delegate function setter/getter to interact to the Low Level Driver API
			writerInt32_t PV_AbortAllFTEs_Writer,		///< Delegate function setter/getter to interact to the Low Level Driver API
			readerTime_t PV_Time_Reader);				///< Delegate function setter/getter to interact to the Low Level Driver API


    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();

    /**
     * @ingroup
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
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;
    /**
     * @brief Retrieve the array of FTEs times
     *
     * @return the array of FTEs times
     */
    std::vector<timespec> getDataFTEs();
    /**
     * @brief Retrieve the Clock Source
     *
     * @return the Clock source
     */
    size_t getClkSrc();
    /**
     * @brief Retrieve the Clock frequency
     *
     * @return the  Clock frequency value
     */
    size_t getClkFreq();
    /**
     * @brief Retrieve the Clock multiplier
     *
     * @return the  Clock multiplier value
     */
    size_t getClkMult();
    /**
     * @brief Retrieve the Clock frequency
     *
     * @return the Clock frequency value
     */
    size_t getSyncStatus();
    /**
     * @brief Retrieve the seconds since last Sync
     *
     * @return the seconds since last Sync
     */
    size_t getSecsSinceSync();
    /**
     * @brief Retrieve the max scheduled FTEs
     *
     * @return the Max scheduled FTEs value
     */
    size_t getMaxSchFTEs();
    /**
     * @brief Retrieve the number of pending FTEs
     *
     * @return the number of pending FTEs value
     */
    size_t getPendingFTEs();
    /**
     * @brief Retrieve the FTE levels
     *
     * @return the FTEs levels
     */
    std::vector<std::int32_t> getFTEsLevels();
    /**
     * @brief Retrieve the status of Abort all FTEs
     *
     * @return the AbortAllFTEs value
     */
    size_t getAbortAllFTEs();
    /**
     * @brief Retrieve the Reference base time
     *
     * @return the Reference base time value
     */
    timespec getRefTimeBase();
    /**
     * @brief Sets the array of the FTEs times
     *
     */
    void setDataFTEs(const timespec& timestamp, const std::vector<timespec>& value);
     /**
     * @brief Sets the value of the Clock Source
     *
     */
    void setClkSrc(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Clock frequency
     *
     */
    void setClkFreq(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the Clock multiplier
     *
     */
    void setClkMult(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Clock frequency
     *
     */
    void setSyncStatus(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the seconds since last Sync
     *
     */
    void setSecsSinceSync(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the max scheduled FTEs
     *
     */
    void setMaxSchFTEs(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the number of pending FTEs
     *
     */
    void setPendingFTEs(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the FTE levels
     *
     */
    void setFTEsLevels(const timespec& timestamp, const std::vector<std::int32_t>& value);
    /**
     * @brief Sets the value of the status of Abort all FTEs
     *
     */
    void setAbortAllFTEs(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Reference base time
     *
     */
    void setRefTimeBase(const timespec& timestamp, const timespec& value);

};

template <typename T>
class NDS3_API TimeStampSupp: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
	TimeStampSupp();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	TimeStampSupp(const TimeStampSupp<T>& right);

	TimeStampSupp& operator=(const TimeStampSupp<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	TimeStampSupp( const std::string& name,
			size_t maxElements,                           ///< Number of maximum timestamps acquired
            stateChange_t switchOnFunction,               ///< Delegate function that performs the actions to switch the node on
            stateChange_t switchOffFunction,              ///< Delegate function that performs the actions to switch the node off
            stateChange_t startFunction,                  ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
            stateChange_t stopFunction,                   ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
            stateChange_t recoverFunction,                ///< Delegate function to execute to recover from an error state
            allowChange_t allowStateChangeFunction,       ///< Delegate function that can deny a state change. Usually just returns true
			writerInt32_t PV_EnableTimeStamp_Writer,
			readerInt32_t PV_EnableTimeStamp_Reader,
			writerDouble_t PV_TimeStampEdge_Writer,
			readerDouble_t PV_TimeStampEdge_Reader);

};

template <typename T>
class NDS3_API TriggerSup: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
	TriggerSup();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	TriggerSup(const TriggerSup<T>& right);

	TriggerSup& operator=(const TriggerSup<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	TriggerSup(const std::string& name,
			writerDouble_t PV_DAQStartABSTime_Writer,
			readerDouble_t PV_DAQStartABSTime_Reader,
			writerDouble_t PV_DAQStartTimeDelay_Writer,
			readerDouble_t PV_DAQStartTimeDelay_Reader,
			writerInt32_t PV_TriggPeriodType_Writer,
			readerInt32_t PV_TriggPeriodType_Reader,
			writerInt32_t PV_EnableSWTrigg_Writer,
			readerInt32_t PV_EnableSWTrigg_Reader,
			writerDouble_t PV_TriggPeriod_Writer,
			readerDouble_t PV_TriggPeriod_Reader,
			writerInt32_t PV_TriggEventType_Writer,
			readerInt32_t PV_TriggEventType_Reader,
			writerInt32_t PV_LevelTrigg_Writer,
			readerInt32_t PV_LevelTrigg_Reader,
			writerInt32_t PV_EdgeTrigg_Writer,
			readerInt32_t PV_EdgeTrigg_Reader,
			writerInt32_t PV_CombineTrigg_Writer,
			readerInt32_t PV_CombineTrigg_Reader,
			writerInt32_t PV_TriggDelay_Writer,
			readerInt32_t PV_TriggDelay_Reader,
			writerInt32_t PV_PreTrigg_Writer,
			readerInt32_t PV_PreTrigg_Reader,
			writerDouble_t PV_SamplesToACQwhenTrigg_Writer,
			readerDouble_t PV_SamplesToACQwhenTrigg_Reader,
			writerDouble_t PV_SecondsToACQwhenTrigg_Writer,
			readerDouble_t PV_SecondsToACQwhenTrigg_Reader);

};

}
#endif // NDSTIMESUPP_H

