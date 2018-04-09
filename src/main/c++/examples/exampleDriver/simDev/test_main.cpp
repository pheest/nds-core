#include <iostream>
#include "wavesignals.h"

int main() {

  double amplitude = 3.4; 
  double frequency = 0.5; 
  int32_t nElements = 100; 
  int32_t ClkFrequency = 100;
  double SamplingRate = 1.0;
  std::vector<double> datos = getDataBlock_sin(amplitude, frequency, 
                              nElements, ClkFrequency, 1, SamplingRate); 
  for (auto it = datos.begin(); it != datos.end(); it++) {
    std::cout << *it << std::endl;
  }
  return 0; 

}
