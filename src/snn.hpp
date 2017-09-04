#ifndef SNN_H
#define SNN_H

#include "allneurons.hpp"
#include "synapses/synapse.hpp"
#include "synapsenetwork.hpp"
#include <array>

#define A_NUM 1
#define S_NUM 1

class SNN
{
  public:
    SNN(double duration);
    ~SNN();

    std::array<PPNeuron*, A_NUM> ppn; // poison process neurons
    std::array<Neuron*, S_NUM> sn; // synchronous neurons

    SynapseNetwork con;
};

#endif
