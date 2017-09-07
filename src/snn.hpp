#ifndef SNN_H
#define SNN_H

#include "allneurons.hpp"
#include "synapses/synapse.hpp"
#include "synapsenetwork.hpp"
#include <array>

class SNN
{
  public:
    SNN(double duration);
    ~SNN();

    std::vector<PPNeuron*> ppn; // poison process neurons
    std::vector<Neuron*> sn;    // synchronous neurons

    SynapseNetwork con;

    double d; // dopamine level
};

#endif