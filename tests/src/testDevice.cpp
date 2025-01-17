
#include "testDevice.h"
#include <nds3/nds.h>
#include <mutex>
#include <functional>
#include "ndsTestFactory.h"
#include "ndsTestInterface.h"


static std::map<std::string, TestDevice*> m_devicesMap;
static std::mutex m_lockDevicesMap;

TestDevice::TestDevice(nds::Factory &factory, const std::string &parameter): m_name(parameter)
{
    {
        std::lock_guard<std::mutex> lock(m_lockDevicesMap);
        if(m_devicesMap.find(parameter) != m_devicesMap.end())
        {
            throw std::logic_error("Device with the same name already allocated. This should not happen");
        }
        m_devicesMap[parameter] = this;
    }

    nds::Node rootNode(parameter);

    nds::Node channel1 = rootNode.addChild(nds::Port("Channel1"));
    m_variableIn0 = channel1.addChild(nds::PVVariableIn<std::int32_t>("variableIn0"));
    m_variableIn1 = channel1.addChild(nds::PVVariableIn<std::vector<std::int32_t> >("variableIn1"));
    m_dataAcquisition = channel1.addChild(nds::DataAcquisition<std::vector<std::int32_t> >(
        "data",
        10000,
        std::bind(&TestDevice::switchOn, this),
        std::bind(&TestDevice::switchOff, this),
        std::bind(&TestDevice::start, this),
        std::bind(&TestDevice::stop, this),
        std::bind(&TestDevice::recover, this),
        std::bind(&TestDevice::allowChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&TestDevice::PV_DataAcquisition_Gain_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_Offset_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_Bandwidth_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_Resolution_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_Impedance_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_Coupling_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_SignalRefType_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_Ground_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_DMAEnable_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&TestDevice::PV_DataAcquisition_SamplingRate_Writer, this, std::placeholders::_1, std::placeholders::_2)
    ));

    m_numberAcquisitions = channel1.addChild(nds::PVVariableOut<std::int32_t>("numAcquisitions"));

    channel1.addChild(nds::PVDelegateIn<std::string>("delegateIn", std::bind(&TestDevice::readDelegate, this, std::placeholders::_1, std::placeholders::_2)));
    channel1.addChild(nds::PVDelegateOut<std::string>("delegateOut", std::bind(&TestDevice::writeDelegate, this, std::placeholders::_1, std::placeholders::_2)));

    m_testVariableIn = channel1.addChild(nds::PVVariableIn<std::string>("testVariableIn"));
    m_testVariableIn.setValue("Initial value");
    m_testVariableOut = channel1.addChild(nds::PVVariableOut<std::string>("testVariableOut"));

    channel1.addChild(nds::PVDelegateOut<std::string>("writeTestVariableIn", std::bind(&TestDevice::writeTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)));
    channel1.addChild(nds::PVDelegateOut<std::string>("pushTestVariableIn", std::bind(&TestDevice::pushTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)));
    channel1.addChild(nds::PVDelegateIn<std::string>("readTestVariableOut", std::bind(&TestDevice::readTestVariableOut, this, std::placeholders::_1, std::placeholders::_2)));

    m_setCurrentTime = channel1.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));

    channel1.setTimestampDelegate(std::bind(&TestDevice::getCurrentTime, this));

    rootNode.initialize(this, factory);
}

TestDevice::~TestDevice()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

TestDevice* TestDevice::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, TestDevice*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

/*
 * Allocation function
 *********************/
void* TestDevice::allocateDevice(nds::Factory& factory, const std::string& device, const nds::namedParameters_t& /* parameters */)
{
    return new TestDevice(factory, device);
}

/*
 * Deallocation function
 ***********************/
void TestDevice::deallocateDevice(void* device)
{
    delete (TestDevice*)device;
}

void TestDevice::switchOn()
{
    ::sleep(1);
}

void TestDevice::switchOff()
{
    ::sleep(1);
}

void TestDevice::start()
{
    m_acquisitionThread = std::thread(std::bind(&TestDevice::acquire, this, m_numberAcquisitions.getValue(),m_dataAcquisition.getMaxElements()));
}

void TestDevice::stop()
{
    m_acquisitionThread.join();
}

void TestDevice::recover()
{

}

bool TestDevice::allowChange(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}

void TestDevice::acquire(size_t numAcquisition, size_t numSamples)
{
    std::vector<std::int32_t> pushData0(numSamples);
    std::vector<std::int32_t> pushData1(numSamples);

    timespec timestamp = m_dataAcquisition.getStartTimestamp();

    // Initialize vectors
    for(size_t initializeVectors(0); initializeVectors != numSamples; ++initializeVectors)
    {
        pushData0[initializeVectors] = initializeVectors;
        pushData1[initializeVectors] = numSamples - initializeVectors;
    }
    for(size_t acquisitionNumber(0); acquisitionNumber != numAcquisition; ++acquisitionNumber)
    {
        if((acquisitionNumber & 0x1) == 0)
        {
            m_dataAcquisition.push(timestamp, pushData0);
        }
        else
        {
            m_dataAcquisition.push(timestamp, pushData1);
        }
        timestamp.tv_sec++;
        timestamp.tv_nsec++;
    }
}

void TestDevice::readDelegate(timespec* pTimestamp, std::string* pValue)
{
    *pTimestamp = m_timestamp;
    *pValue = m_writtenByDelegate;
}

void TestDevice::writeDelegate(const timespec& timestamp, const std::string& value)
{
    m_timestamp = timestamp;
    m_writtenByDelegate = value;
}

void TestDevice::writeTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.setValue(timestamp, value);
}

void TestDevice::pushTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.push(timestamp, value);
}

void TestDevice::readTestVariableOut(timespec* pTimestamp, std::string* pValue)
{
    m_testVariableOut.getValue(pTimestamp, pValue);
}

timespec TestDevice::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

