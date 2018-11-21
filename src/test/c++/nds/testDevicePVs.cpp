#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"
#include "../include/testUtils.h"


/**
 * @brief Internal function to test all the data types supported by the NDS PVs.
 * @param testInitializers Flag to set to @c true when the initializers of
 * output PVs shall be tested.
 * As the initialization of output PVs is the only difference between
 * PVTypes and PVTypesInit test, this function shares the full test
 * and the evaluation or not of the initialization features depends on the
 * @a testInitializers flag.
 */
static void commonPVsTest(const bool testInitializers = false);

TEST(testDevicePVs, PVTypes)
{
	commonPVsTest();
}

TEST(testDevicePVs, PVTypesInit)
{
	commonPVsTest(true);
}

static void commonPVsTest(const bool testInitializers) {
	timespec timestamp = {0, 0};
	const timespec* pTimestamp;
	const std::int32_t* pInteger;
	const double* pDouble;
	const std::vector<bool>* pBoolArray;
	const std::vector<std::uint8_t>* pUInt8Array;
	const std::vector<std::uint16_t>* pUInt16Array;
	const std::vector<std::uint32_t>* pUInt32Array;
	const std::vector<std::int8_t>* pInt8Array;
	const std::vector<std::int16_t>* pInt16Array;
	const std::vector<std::int32_t>* pInt32Array;
	const std::vector<double>* pDoubleArray;
	const std::string* pString;
	const timespec* pTimespec;
	const std::vector<timespec>* pTimespecArray;
	const nds::timestamp_t* pTimestampData;


	//--------------------------------------------------------------------------------------
	//VALUES OF THE PVS THAT HAVE BEEN PUSHED AT THE INITIALIZATION
	//--------------------------------------------------------------------------------------
	std::int32_t intData = 1;
	double doubleData = 1.1;
	std::vector<bool> boolArrayData = {true, true, false, true};
	std::vector<std::uint8_t> uInt8ArrayData = {0,1,2};
	std::vector<std::uint16_t> uInt16ArrayData = {3,4};
	std::vector<std::uint32_t> uInt32ArrayData = {5,6,7};
	std::vector<std::int8_t> int8ArrayData = {0,-1,2};
	std::vector<std::int16_t> int16ArrayData = {3,-4};
	std::vector<std::int32_t> int32ArrayData = {-5,6,-7};
	std::vector<double> doubleArrayData = {-0.1,0.2,1.5};
	std::string stringData = "text";
	timespec timespecData = {11111111,2243354};
	std::vector<timespec> timespecArrayData = {{1,2}, {3,4}};
	nds::timestamp_t timestampData = {{10,1}, 0, true};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DevicePVs and name it devicePVs
	nds::namedParameters_t parameters;
	if (testInitializers) {
		parameters["INIT"]="YES";
	}
    factory.createDevice("DevicePVs", "devicePVs", parameters);

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("devicePVs");

    if (testInitializers) { //Test initial value of output PVs.

    	std::int32_t intValue;
    	double doubleValue;
    	std::vector<bool> boolArrayValues;
    	std::vector<std::uint8_t> uInt8ArrayValues;
    	std::vector<std::uint16_t> uInt16ArrayValues;
    	std::vector<std::uint32_t> uInt32ArrayValues;
    	std::vector<std::int8_t> int8ArrayValues;
    	std::vector<std::int16_t> int16ArrayValues;
    	std::vector<std::int32_t> int32ArrayValues;
    	std::vector<double> doubleArrayValues;
    	std::string stringValue ;
    	timespec timespecValue;
    	std::vector<timespec> timespecArrayValues;
    	nds::timestamp_t timestampValue;

    	//--------------------------------------------------------------------------------------
    	//VALUES OF THE PVS THAT HAVE BEEN SET AT THE INITIALIZATION
    	//--------------------------------------------------------------------------------------
    	std::int32_t intDataInit = -2147483648;
    	double doubleDataInit = 5e8;
    	std::vector<bool> boolArrayDataInit(2, true); //Vector with 2 data to true
    	std::vector<std::uint8_t> uInt8ArrayDataInit(5, 255); //Vector with 5 data to 255.
    	std::vector<std::uint16_t> uInt16ArrayDataInit(4, 65535);//Vector with 4 data to 65535.
    	std::vector<std::uint32_t> uInt32ArrayDataInit(3, 4294967295); //Vector with 3 data to 4294967295.
    	std::vector<std::int8_t> int8ArrayDataInit(5, -128); //Vector with 5 data to -128.
    	std::vector<std::int16_t> int16ArrayDataInit(1, -32768); //Vector with 1 data to -32768.
    	std::vector<std::int32_t> int32ArrayDataInit(1, -2147483648); //Vector with 1 data to -2147483648.
    	std::vector<double> doubleArrayDataInit(4, 9e12); //Vector with 4 data to 9e12.
    	std::string stringDataInit = "string";
    	timespec timespecDataInit = {123, 456};
    	std::vector<timespec> timespecArrayDataInit(5, {123, 456}); //Vector with 5 data to {123, 456}.
    	nds::timestamp_t timestampDataInit{{123, 456}, 0, false};

		//--------------------------------------------------------------------------------------
		//TEST THE VALUES OF THE PVS THAT HAVE BEEN INITIALIZED IN THE CONTROL SYSTEM
		//--------------------------------------------------------------------------------------

    	pInterface->readCSValue("/devicePVs-Integer", &timestamp, &intValue);
		EXPECT_EQ((std::int32_t) intDataInit, intValue);

    	pInterface->readCSValue("/devicePVs-Double", &timestamp, &doubleValue);
		EXPECT_EQ((double) doubleDataInit, doubleValue);

    	pInterface->readCSValue("/devicePVs-BoolArray", &timestamp, &boolArrayValues);
		EXPECT_EQ((bool) true, (boolArrayDataInit == boolArrayValues) );

    	pInterface->readCSValue("/devicePVs-UInt8Array", &timestamp, &uInt8ArrayValues);
		EXPECT_EQ((bool) true, (uInt8ArrayDataInit == uInt8ArrayValues) );

    	pInterface->readCSValue("/devicePVs-UInt16Array", &timestamp, &uInt16ArrayDataInit);
		EXPECT_EQ((bool) true, (uInt16ArrayDataInit == uInt16ArrayDataInit) );

    	pInterface->readCSValue("/devicePVs-UInt32Array", &timestamp, &uInt32ArrayDataInit);
		EXPECT_EQ((bool) true, (uInt32ArrayDataInit == uInt32ArrayDataInit) );

    	pInterface->readCSValue("/devicePVs-Int8Array", &timestamp, &int8ArrayValues);
		EXPECT_EQ((bool) true, (int8ArrayDataInit == int8ArrayValues) );

    	pInterface->readCSValue("/devicePVs-Int16Array", &timestamp, &int16ArrayValues);
		EXPECT_EQ((bool) true, (int16ArrayDataInit == int16ArrayValues) );

    	pInterface->readCSValue("/devicePVs-Int32Array", &timestamp, &int32ArrayValues);
		EXPECT_EQ((bool) true, (int32ArrayDataInit == int32ArrayDataInit) );

    	pInterface->readCSValue("/devicePVs-DoubleArray", &timestamp, &doubleArrayValues);
		EXPECT_EQ((bool) true, (doubleArrayDataInit == doubleArrayDataInit) );

    	pInterface->readCSValue("/devicePVs-String", &timestamp, &stringValue);
		EXPECT_EQ(stringDataInit, stringValue);

    	pInterface->readCSValue("/devicePVs-Timespec", &timestamp, &timespecValue);
		EXPECT_EQ(timespecDataInit.tv_sec, timespecValue.tv_sec);
		EXPECT_EQ(timespecDataInit.tv_nsec, timespecValue.tv_nsec);

    	pInterface->readCSValue("/devicePVs-TimespecArray", &timestamp, &timespecArrayValues);
		EXPECT_EQ(timespecArrayDataInit.size(), timespecArrayValues.size());
		for (std::uint32_t i = 0; i < timespecArrayDataInit.size(); i++) {
			EXPECT_EQ(timespecArrayDataInit[i].tv_sec, timespecArrayValues[i].tv_sec);
			EXPECT_EQ(timespecArrayDataInit[i].tv_nsec, timespecArrayValues[i].tv_nsec);
		}

    	pInterface->readCSValue("/devicePVs-Timestamp", &timestamp, &timestampValue);
		EXPECT_EQ(timestampDataInit.timestamp.tv_sec, timestampValue.timestamp.tv_sec);
		EXPECT_EQ(timestampDataInit.timestamp.tv_nsec, timestampValue.timestamp.tv_nsec);
		EXPECT_EQ(timestampDataInit.id, timestampValue.id);
		EXPECT_EQ(timestampDataInit.rising, timestampValue.rising);

		std::cout << "\t-------------------Testing Initial Values-------------------" << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(intDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(doubleDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(boolArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(uInt8ArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(uInt16ArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(uInt32ArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(int8ArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(int16ArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(int32ArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(doubleArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(stringDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(timespecDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(timespecArrayDataInit) << std::endl;
		std::cout << "\t Initial " << TestUtils::getString(timestampDataInit) << std::endl;
		std::cout << "\t------------------------------------------------------------" << std::endl;
    }

    for (int i = 0; i < 2; i++) {

    	if (i == 1) {

    	    //--------------------------------------------------------------------------------------
    	    //CHANGE THE PVS VALUES AND VERIFIES IT THROUGH THE READBACKS
    	    //--------------------------------------------------------------------------------------

    		intData = 5;
    		doubleData = 4.3;
    		boolArrayData = {false, true};
    		uInt8ArrayData = {2,1};
    		uInt16ArrayData = {1,2};
    		uInt32ArrayData = {0,6,5};
    		int8ArrayData = {-1, -2};
    		int16ArrayData = {-3,4};
    		int32ArrayData = {-5,6,32};
    		doubleArrayData = {-1.5, 2.4};
    		stringData = "newText";
    		timespecData = {10,25};
    		timespecArrayData = {{3,4}, {10, 2}};
    		timestampData = {{1000,10}, 1, true};
    		pInterface->writeCSValue("/devicePVs-Integer", timestamp, intData);
    		pInterface->writeCSValue("/devicePVs-Double", timestamp, doubleData);
    		pInterface->writeCSValue("/devicePVs-BoolArray", timestamp, boolArrayData);
    		pInterface->writeCSValue("/devicePVs-UInt8Array", timestamp, uInt8ArrayData);
    		pInterface->writeCSValue("/devicePVs-UInt16Array", timestamp, uInt16ArrayData);
    		pInterface->writeCSValue("/devicePVs-UInt32Array", timestamp, uInt32ArrayData);
    		pInterface->writeCSValue("/devicePVs-Int8Array", timestamp, int8ArrayData);
    		pInterface->writeCSValue("/devicePVs-Int16Array", timestamp, int16ArrayData);
    		pInterface->writeCSValue("/devicePVs-Int32Array", timestamp, int32ArrayData);
    		pInterface->writeCSValue("/devicePVs-DoubleArray", timestamp, doubleArrayData);
    		pInterface->writeCSValue("/devicePVs-String", timestamp, stringData);
    		pInterface->writeCSValue("/devicePVs-Timespec", timestamp, timespecData);
    		pInterface->writeCSValue("/devicePVs-TimespecArray", timestamp, timespecArrayData);
    		pInterface->writeCSValue("/devicePVs-Timestamp", timestamp, timestampData);
    	}

		//--------------------------------------------------------------------------------------
		//TEST THE VALUES OF THE PVS THAT HAVE BEEN PUSHED TO THE CONTROL SYSTEM
		//--------------------------------------------------------------------------------------
		pInterface->getPushedInt32("/devicePVs-Integer_RBV", pTimestamp, pInteger);
		EXPECT_EQ((std::int32_t) intData, *pInteger);
		pInterface->getPushedDouble("/devicePVs-Double_RBV", pTimestamp, pDouble);
		EXPECT_EQ((double) doubleData, *pDouble);
		pInterface->getPushedVectorBool("/devicePVs-BoolArray_RBV", pTimestamp, pBoolArray);
		EXPECT_EQ((size_t)boolArrayData.size(), (size_t)pBoolArray->size());
		for (std::uint32_t i = 0; i < boolArrayData.size(); i++) {
			EXPECT_EQ((bool)boolArrayData[i], pBoolArray->at(i));
		}
		pInterface->getPushedVectorUint8("/devicePVs-UInt8Array_RBV", pTimestamp, pUInt8Array);
		EXPECT_EQ(uInt8ArrayData.size(), pUInt8Array->size());
		for (std::uint32_t i = 0; i < uInt8ArrayData.size(); i++) {
			EXPECT_EQ(uInt8ArrayData[i], pUInt8Array->at(i));
		}
		pInterface->getPushedVectorUint16("/devicePVs-UInt16Array_RBV", pTimestamp, pUInt16Array);
		EXPECT_EQ(uInt16ArrayData.size(), pUInt16Array->size());
		for (std::uint32_t i = 0; i < uInt16ArrayData.size(); i++) {
			EXPECT_EQ(uInt16ArrayData[i], pUInt16Array->at(i));
		}
		pInterface->getPushedVectorUint32("/devicePVs-UInt32Array_RBV", pTimestamp, pUInt32Array);
		EXPECT_EQ(uInt32ArrayData.size(), pUInt32Array->size());
		for (std::uint32_t i = 0; i < uInt32ArrayData.size(); i++) {
			EXPECT_EQ(uInt32ArrayData[i], pUInt32Array->at(i));
		}
		pInterface->getPushedVectorInt8("/devicePVs-Int8Array_RBV", pTimestamp, pInt8Array);
		EXPECT_EQ(int8ArrayData.size(), pInt8Array->size());
		for (std::uint32_t i = 0; i < int8ArrayData.size(); i++) {
			EXPECT_EQ(int8ArrayData[i], pInt8Array->at(i));
		}
		pInterface->getPushedVectorInt16("/devicePVs-Int16Array_RBV", pTimestamp, pInt16Array);
		EXPECT_EQ(int16ArrayData.size(), pInt16Array->size());
		for (std::uint32_t i = 0; i < int16ArrayData.size(); i++) {
			EXPECT_EQ(int16ArrayData[i], pInt16Array->at(i));
		}
		pInterface->getPushedVectorInt32("/devicePVs-Int32Array_RBV", pTimestamp, pInt32Array);
		EXPECT_EQ(int32ArrayData.size(), pInt32Array->size());
		for (std::uint32_t i = 0; i < int32ArrayData.size(); i++) {
			EXPECT_EQ(int32ArrayData[i], pInt32Array->at(i));
		}
		pInterface->getPushedVectorDouble("/devicePVs-DoubleArray_RBV", pTimestamp, pDoubleArray);
		EXPECT_EQ(doubleArrayData.size(), pDoubleArray->size());
		for (std::uint32_t i = 0; i < doubleArrayData.size(); i++) {
			EXPECT_EQ(doubleArrayData[i], pDoubleArray->at(i));
		}
		pInterface->getPushedString("/devicePVs-String_RBV", pTimestamp, pString);
		EXPECT_EQ(stringData, *pString);
		pInterface->getPushedTimespec("/devicePVs-Timespec_RBV", pTimestamp, pTimespec);
		EXPECT_EQ(timespecData.tv_sec, pTimespec->tv_sec);
		EXPECT_EQ(timespecData.tv_nsec, pTimespec->tv_nsec);
		pInterface->getPushedVectorTimespec("/devicePVs-TimespecArray_RBV", pTimestamp, pTimespecArray);
		EXPECT_EQ(timespecArrayData.size(), pTimespecArray->size());
		for (std::uint32_t i = 0; i < timespecArrayData.size(); i++) {
			timespec t = pTimespecArray->at(i);
			EXPECT_EQ(timespecArrayData[i].tv_sec, t.tv_sec);
			EXPECT_EQ(timespecArrayData[i].tv_nsec, t.tv_nsec);
		}
		pInterface->getPushedTimestamp("/devicePVs-Timestamp_RBV", pTimestamp, pTimestampData);
		EXPECT_EQ(timestampData.timestamp.tv_sec, pTimestampData->timestamp.tv_sec);
		EXPECT_EQ(timestampData.timestamp.tv_nsec, pTimestampData->timestamp.tv_nsec);
		EXPECT_EQ(timestampData.id, pTimestampData->id);
		EXPECT_EQ(timestampData.rising, pTimestampData->rising);

		std::cout << "\t--------------------------------------" << std::endl;
		std::cout << "\t" << TestUtils::getString(intData) << std::endl;
		std::cout << "\t" << TestUtils::getString(doubleData) << std::endl;
		std::cout << "\t" << TestUtils::getString(boolArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(uInt8ArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(uInt16ArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(uInt32ArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(int8ArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(int16ArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(int32ArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(doubleArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(stringData) << std::endl;
		std::cout << "\t" << TestUtils::getString(timespecData) << std::endl;
		std::cout << "\t" << TestUtils::getString(timespecArrayData) << std::endl;
		std::cout << "\t" << TestUtils::getString(timestampData) << std::endl;
		std::cout << "\t--------------------------------------" << std::endl;

    }

    // Destroy test device
    factory.destroyDevice("devicePVs");

}





