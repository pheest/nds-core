#include <cmath>
#include <iostream>
#include "wavesignals.h"

std::vector<double> getDataBlock_sin(double amplitude, double frequency, std::int32_t nElements, 
                                     std::int32_t ClkFrequency, std::int32_t edge,
                                     double SamplingRate) {
  static double t = 0.0;
  double increment;
  if (edge == 2) {
    increment = SamplingRate/((double)ClkFrequency * 2.0);
  } else if (edge == 0) {
    increment = SamplingRate/(double)ClkFrequency;
    t += increment/2.0; 
  } else if (edge == 1) {
    increment = SamplingRate/(double)ClkFrequency;
  } else {
     std::cout  << "WARNING: edge should be 0(RAISING),1(FALLING),2(ANY)\n";
     std::cout  << "         Empty vector returned\n";
     std::vector<double> data(nElements, 0);
     return data; 
  }   
  std::vector<double> data(nElements);
  for (auto it = data.begin(); it != data.end(); ++it) {
     *it  = amplitude * sin(2.0 * M_PI * frequency * t);
     t += increment; 
  }
  if (edge == 1) {
    t += increment/2.0;
  } 
  return data;
}
