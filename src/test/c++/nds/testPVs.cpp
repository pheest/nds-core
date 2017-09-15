#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/testDevice.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

//TEST(testPVs, testDelegate)
//{
//    nds::Factory factory("test");
//
//    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());
//
//    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");
//
//    timespec timestamp;
//    timestamp.tv_sec = 2;
//    timestamp.tv_nsec = 12;
//    pInterface->writeCSValue("/rootNode-Channel1.delegateOut", timestamp, std::string("this is a test"));
//
//    std::string readValue;
//    timespec readTimestamp;
//    pInterface->readCSValue("/rootNode-Channel1.delegateIn", &readTimestamp, &readValue);
//    EXPECT_EQ("this is a test", readValue);
//    EXPECT_EQ(2, readTimestamp.tv_sec);
//    EXPECT_EQ(12, readTimestamp.tv_nsec);
//
//    factory.destroyDevice("rootNode");
//
//}

TEST(testPVs, testVariable)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    std::int32_t value_I32;
    double  value_DBL;
    std::vector<std::int8_t> vector_I8;
    std::vector<std::uint8_t> vector_UI8;
    std::vector<std::int32_t> vector_I32;
    std::vector<double> vector_DBL;
    std::string value_string;

    timespec readTimestamp{0,0}, timestamp{0,0};

    {
        pInterface->readCSValue("/rootNode-int32_VariableIn", &readTimestamp, &value_I32);
        EXPECT_EQ(1, value_I32);

        pInterface->readCSValue("/rootNode-double_VariableIn", &readTimestamp, &value_DBL);
        EXPECT_EQ(1, value_DBL);

        pInterface->readCSValue("/rootNode-vectorI8_VariableIn", &readTimestamp, &vector_I8);
    	EXPECT_EQ(1, vector_I8[0]);
    	EXPECT_EQ(1, vector_I8[1]);
    	EXPECT_EQ((unsigned int)2, vector_I8.size());

        pInterface->readCSValue("/rootNode-vectorUI8_VariableIn", &readTimestamp, &vector_UI8);
    	EXPECT_EQ(1, vector_I8[0]);
    	EXPECT_EQ(1, vector_I8[1]);
    	EXPECT_EQ((unsigned int)2, vector_UI8.size());

        pInterface->readCSValue("/rootNode-vectorI32_VariableIn", &readTimestamp, &vector_I32);
    	EXPECT_EQ(1, vector_I8[0]);
    	EXPECT_EQ(1, vector_I8[1]);
    	EXPECT_EQ((unsigned int)2, vector_I32.size());

        pInterface->readCSValue("/rootNode-vectorDBL_VariableIn", &readTimestamp, &vector_DBL);
    	EXPECT_EQ(1, vector_I8[0]);
    	EXPECT_EQ(1, vector_I8[1]);
    	EXPECT_EQ((unsigned int)2, vector_DBL.size());

        pInterface->readCSValue("/rootNode-string_VariableIn", &readTimestamp, &value_string);
        EXPECT_EQ("Initial value", value_string);
    }

    {
    	timestamp.tv_sec =1; timestamp.tv_nsec=1;
        pInterface->writeCSValue("/rootNode-int32_VariableOut", timestamp, (std::int32_t)2);
        pInterface->readCSValue("/rootNode-int32_VariableOut", &readTimestamp, &value_I32);
        EXPECT_EQ(2, value_I32);
        EXPECT_EQ(1, readTimestamp.tv_sec);
        EXPECT_EQ(1, readTimestamp.tv_nsec);

    	timestamp.tv_sec =2; timestamp.tv_nsec=2;
        pInterface->writeCSValue("/rootNode-double_VariableOut", timestamp, (double)2);
        pInterface->readCSValue("/rootNode-double_VariableOut", &readTimestamp, &value_DBL);
        EXPECT_EQ(2, value_DBL);
        EXPECT_EQ(2, readTimestamp.tv_sec);
        EXPECT_EQ(2, readTimestamp.tv_nsec);

    	timestamp.tv_sec =3; timestamp.tv_nsec=3;
        pInterface->writeCSValue("/rootNode-vectorI8_VariableOut", timestamp, std::vector<int8_t> (2,2));
        pInterface->readCSValue("/rootNode-vectorI8_VariableOut", &readTimestamp, &vector_I8);
    	EXPECT_EQ(2, vector_I8[0]);
    	EXPECT_EQ(2, vector_I8[1]);
    	EXPECT_EQ((unsigned int)2, vector_I8.size());
    	EXPECT_EQ(3, readTimestamp.tv_sec);
        EXPECT_EQ(3, readTimestamp.tv_nsec);

    	timestamp.tv_sec =4; timestamp.tv_nsec=4;
        pInterface->writeCSValue("/rootNode-vectorUI8_VariableOut", timestamp,  std::vector<uint8_t> (2,2));
        pInterface->readCSValue("/rootNode-vectorUI8_VariableOut", &readTimestamp, &vector_UI8);
    	EXPECT_EQ(2, vector_UI8[0]);
    	EXPECT_EQ(2, vector_UI8[1]);
    	EXPECT_EQ((unsigned int)2, vector_UI8.size());
    	EXPECT_EQ(4, readTimestamp.tv_sec);
        EXPECT_EQ(4, readTimestamp.tv_nsec);

    	timestamp.tv_sec =5; timestamp.tv_nsec=5;
        pInterface->writeCSValue("/rootNode-vectorI32_VariableOut", timestamp, std::vector<int32_t> (2,2));
        pInterface->readCSValue("/rootNode-vectorI32_VariableOut", &readTimestamp, &vector_I32);
    	EXPECT_EQ(2, vector_I32[0]);
    	EXPECT_EQ(2, vector_I32[1]);
    	EXPECT_EQ((unsigned int)2, vector_I32.size());
    	EXPECT_EQ(5, readTimestamp.tv_sec);
        EXPECT_EQ(5, readTimestamp.tv_nsec);

    	timestamp.tv_sec =6; timestamp.tv_nsec=6;
        pInterface->writeCSValue("/rootNode-vectorDBL_VariableOut", timestamp, std::vector<double> (2,2));
        pInterface->readCSValue("/rootNode-vectorDBL_VariableOut", &readTimestamp, &vector_DBL);
    	EXPECT_EQ(2, vector_DBL[0]);
    	EXPECT_EQ(2, vector_DBL[1]);
    	EXPECT_EQ((unsigned int)2, vector_DBL.size());
    	EXPECT_EQ(6, readTimestamp.tv_sec);
        EXPECT_EQ(6, readTimestamp.tv_nsec);

    	timestamp.tv_sec =7; timestamp.tv_nsec=7;
        pInterface->writeCSValue("/rootNode-string_VariableOut", timestamp, std::string("testing output variable"));
        pInterface->readCSValue("/rootNode-string_VariableOut", &readTimestamp, &value_string);
        EXPECT_EQ("testing output variable", value_string);
        EXPECT_EQ(7, readTimestamp.tv_sec);
        EXPECT_EQ(7, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");
}

TEST(testPVs, testSubscription0)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::parameters_t parameters;
    parameters.push_back("rootNode-Channel1-testVariableIn");
    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("subscribe", "rootNode-Channel1-testVariableOut", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-Channel1.writeTestVariableIn", timestamp, value.str());

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-Channel1.readTestVariableOut", &readTimestamp, &readValue);
        EXPECT_EQ(value.str(), readValue);
        EXPECT_EQ(count, readTimestamp.tv_sec);
        EXPECT_EQ(count + 10, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");
}
//
//TEST(testPVs, testSubscription1)
//{
//    nds::Factory factory("test");
//
//    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());
//
//    {
//        nds::parameters_t parameters;
//        parameters.push_back("rootNode-Channel1-testVariableIn");
//        nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("subscribe", "rootNode-Channel1-testVariableOut", parameters);
//    }
//
//    {
//        nds::parameters_t parameters;
//        parameters.push_back("0");
//        nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("decimation", "rootNode-Channel1-testVariableIn", parameters);
//    }
//
//    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");
//
//    for(std::int32_t count(0); count != 10; ++count)
//    {
//        std::ostringstream value;
//        value << "Test string " << count;
//
//        timespec timestamp;
//        timestamp.tv_sec = count;
//        timestamp.tv_nsec = count + 10;
//        pInterface->writeCSValue("/rootNode-Channel1.pushTestVariableIn", timestamp, value.str());
//
//        {
//            const std::string* readValue;
//            const timespec* readTimestamp;
//            EXPECT_THROW(pInterface->getPushedString("/rootNode-Channel1.pushTestVariableIn", readTimestamp, readValue), std::runtime_error);
//        }
//
//        std::string readValue;
//        timespec readTimestamp;
//        pInterface->readCSValue("/rootNode-Channel1.readTestVariableOut", &readTimestamp, &readValue);
//        EXPECT_EQ(value.str(), readValue);
//        EXPECT_EQ(count, readTimestamp.tv_sec);
//        EXPECT_EQ(count + 10, readTimestamp.tv_nsec);
//    }
//
//    factory.destroyDevice("rootNode");
//}
//
//TEST(testPVs, testReplication)
//{
//    nds::Factory factory("test");
//
//    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());
//
//    nds::parameters_t parameters;
//    parameters.push_back("rootNode-Channel1-testVariableIn");
//    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("replicate", "rootNode-Channel1-delegateIn", parameters);
//
//    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");
//
//    for(std::int32_t count(0); count != 10; ++count)
//    {
//        std::ostringstream value;
//        value << "Test string " << count;
//
//        timespec timestamp;
//        timestamp.tv_sec = count;
//        timestamp.tv_nsec = count + 10;
//        pInterface->writeCSValue("/rootNode-Channel1.pushTestVariableIn", timestamp, value.str());
//
//        const std::string* pReadValue;
//        const timespec* pReadTimestamp;
//        pInterface->getPushedString("/rootNode-Channel1.delegateIn", pReadTimestamp, pReadValue);
//        EXPECT_EQ(value.str(), *pReadValue);
//        EXPECT_EQ(count, pReadTimestamp->tv_sec);
//        EXPECT_EQ(count + 10, pReadTimestamp->tv_nsec);
//    }
//
//    factory.destroyDevice("rootNode");
//}

