/*
 * device_simulator.h
 *
 *  Created on: Jan 16, 2017
 *      Author: ebernal
 *      GMV & UPM
 */

#ifndef DEVICESIMULATOR_H_
#define DEVICESIMULATOR_H_

#include <functional>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <vector>

#include <nds3/nds.h>

class ChannelGroup;
class Channel;

class DeviceSimulator
{
public:
	DeviceSimulator (nds::Factory& factory, const std::string& device, const nds::namedParameters_t& parameters);

private:
	std::vector<std::shared_ptr<ChannelGroup> > m_channelgroup;
	std::vector<std::shared_ptr<Channel> > m_channel;
	//Info PV's
    nds::PVVariableIn<std::string> m_ndsversion;
    nds::PVVariableIn<std::string> m_irioversion;
    nds::PVVariableIn<std::string> m_fpgaVIversion;
    nds::PVVariableIn<std::string> m_infostatus;
    nds::PVVariableOut<std::int32_t> m_debugmode;

};

class ChannelGroup
{
public:
	ChannelGroup(const std::string& name, nds::Node& parentNode);

	nds::DataAcquisition<std::vector<std::int32_t> > m_dataAcquisition;

    /*
     * @brief A variable PV that stores the amplitude of the wave.
     */
    nds::PVVariableOut<std::int32_t> m_amplitude;
    void switchOn();
    void switchOff();
    void start();
    void stop();
    void recover();
    bool allowChange(const nds::state_t, const nds::state_t, const nds::state_t);
    void DataAcquisitionLoop();
    volatile bool m_bStopDataAcquisition;
    nds::Thread m_dataAcquisitionThread;
    int samples_per_channelgroup;

};

class Channel
{
public:
	Channel(const std::string& name, nds::Node& parentNode);

	nds::DataAcquisition<std::vector<std::int32_t> > m_dataAcquisition;

    /*
     * @brief A variable PV that stores the amplitude of the wave.
     */
    nds::PVVariableOut<std::int32_t> m_amplitude;
    void switchOn();
    void switchOff();
    void start();
    void stop();
    void recover();
    bool allowChange(const nds::state_t, const nds::state_t, const nds::state_t);
    void DataAcquisitionLoop();
    volatile bool m_bStopDataAcquisition;
    nds::Thread m_dataAcquisitionThread;
    int samples_per_channel;
};

#endif /* DEVICESIMULATOR_H_ */
