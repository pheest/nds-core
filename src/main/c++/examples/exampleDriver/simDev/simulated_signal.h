#include <cstdint>
#include <vector>
/**
 * @brief function that returns a DataBlock with nElements of a sinusoidal wave
 *
 * */
std::vector<double> getDataBlock_sin(double amplitude, double frequency, std::int32_t nElements, 
                                     std::int32_t ClkFrequency, std::int32_t edge, 
                                     double SamplingRate); 
