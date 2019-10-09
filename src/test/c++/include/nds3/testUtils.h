
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <nds3/nds.h>

class TestUtils
{
public:

	/**
	 * @brief Get a string by identifying the type of data (std::int32_t) and its value
	 */
	static std::string getString(const std::int32_t & data);

	/**
	 * @brief Get a string by identifying the type of data (float) and its value
	 */
	static std::string getString(const float & data);

	/**
	 * @brief Get a string by identifying the type of data (double) and its value
	 */
	static std::string getString(const double & data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<bool> data) and its value
	 */
	static std::string getString(std::vector<bool> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint8_t>) and its value
	 */
	static std::string getString(std::vector<std::uint8_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint16_t>) and its value
	 */
	static std::string getString(std::vector<std::uint16_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint32_t>) and its value
	 */
	static std::string getString(std::vector<std::uint32_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int8_t>) and its value
	 */
	static std::string getString(std::vector<std::int8_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int16_t>) and its value
	 */
	static std::string getString(std::vector<std::int16_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int32_t>) and its value
	 */
	static std::string getString(std::vector<std::int32_t> data);

	/**
	 * @brief Get a string by identifying the type of data (float) and its value
	 */
	static std::string getString(std::vector<float> data);

	/**
	 * @brief Get a string by identifying the type of data (double) and its value
	 */
	static std::string getString(std::vector<double> data);

	/**
	 * @brief Get a string by identifying the type of data (std::string) and its value
	 */
	static std::string getString(std:: string data);

	/**
	 * @brief Get a string by identifying the type of data (timespec data) and its value
	 */
	static std::string getString(timespec data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<timespec> data) and its value
	 */
	static std::string getString(std::vector<timespec> data);

	/**
	 * @brief Get a string by identifying the type of data (nds::timestamp_t) and its value
	 */
	static std::string getString(nds::timestamp_t data);
};




#endif /* TESTUTILS_H_ */
