#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDevicePVs, PVTypes)
{
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
	timespec timespecData = {1,2};
	std::vector<timespec> timespecArrayData = {{1,2}, {3,4}};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DevicePVs and name it devicePVs
    factory.createDevice("DevicePVs", "devicePVs", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("devicePVs");

    //--------------------------------------------------------------------------------------
    //TEST THE VALUES OF THE PVS THAT HAVE BEEN PUSHED AT THE INITIALIZATION
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


    //--------------------------------------------------------------------------------------
    //TODO: CHANGE THE PVS VALUES AND VERIFIES IT THROUGH THE READBACKS
    //--------------------------------------------------------------------------------------

    // Destroy test device
    factory.destroyDevice("devicePVs");

}





