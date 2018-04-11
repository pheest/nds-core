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
 * @file triggering.h
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
class NDS3_API Triggering: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
	Triggering();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	Triggering(const Triggering<T>& right);

	Triggering& operator=(const Triggering<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	Triggering(const std::string& name,
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

