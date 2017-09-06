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
			writerInt32_t PV_clkSrc_Writer,
			readerInt32_t PV_clkSrc_Reader,
			writerDouble_t PV_clkFreq_Writer,
			readerDouble_t PV_clkFreq_Reader,
			writerDouble_t PV_clkMult_Writer,
			readerDouble_t PV_clkMult_Reader,
			readerDouble_t PV_SyncStatus_Reader,
			readerDouble_t PV_SecsSinceSync_Reader,
			readerInt32_t PV_MaxSchFTEs_Reader,
			readerInt32_t PV_PendingFTEs_Reader,
			readerVectorInt32_t  PV_FTElevels_Reader,
			size_t MaxElements,
			writerInt32_t PV_AbortAllFTEs_Writer,
			readerInt32_t PV_AbortAllFTEs_Reader,
			readerInt32_t PV_refTimeBase_Reader,
			readerDouble_t PV_Time_Reader);


    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time.
     *
     * @param timestampDelegate the function that returns the exact starting time o
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);



    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     * @return the time when started.
     */
    timespec getStartTimestamp() const;
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

