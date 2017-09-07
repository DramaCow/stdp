#include <iostream>
#include <iomanip>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include <algorithm>

#include "snn.hpp"
#include "eventmanager.hpp"
#include "allevents.hpp"
#include "units.hpp"

#include <typeinfo>

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // figure number as string
  std::string fig_num(argv[1]);

  // example outputting dopamine controlled STDP rules for d1/d2 dopaminergic neurons
  //double d = 10.0;
  //GlutamateSynapse sy_d1(nullptr, nullptr, 0.5*W_MAX, 1.20, 6.00, 1.20, 1.30,  0.00, -0.40, -0.50, d);
  //sy_d1.write(fig_num);

  // global config
  const double duration = 8000;
  const double dt_max = 0.00005;
  double t_sim = 0.0;

  // initialise simulation state
  SNN snn(duration);

  // initialise event queue
  EventManager EM(duration);
  EM.insert(new ProgressEvent(0.0, 0));

  EM.insert(new EpochEvent(0.0, 0));
  EM.insert(new EpochEvent(0.0, 1));
  EM.insert(new RecordEvent(0.0, 0, fig_num));

  // initialise some global recorders
  std::vector<double> s_record;

  std::cout << "SIM " << argv[1] << " (duration=" << EM.duration << "s) :" << std::endl;

  // main loop
  while (t_sim <= EM.duration && EM.size() > 0)
  {
    Event *e = EM.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (t_sim < e->time && !synch_event_inserted)
    {
      double dt = (dt_max <= (e->time-t_sim)) ? dt_max : (e->time-t_sim);
      t_sim += dt;

      for (Neuron *&neuron : snn.sn)
      {
        neuron->step(dt);
        if (neuron->is_spiking())
        {
          EM.insert(new SpikeEvent(t_sim, neuron)); // TODO: why bother inserting, just process it here
          synch_event_inserted = true;

          s_record.push_back(t_sim);
        }
      }
    }

    // event (asynchronous) update
    if (!synch_event_inserted)
    {
      e->process(EM, snn);
      EM.del_min();
    }
  }

  // export results to binary files
  std::cout << " writing results to file...";
  snn.con.write(fig_num);
  {
    FILE* file = fopen((fig_num + "_spiketrains.dat").c_str(), "wb");

    int count = snn.ppn.size() + snn.sn.size();
    fwrite(&count, sizeof(int), 1, file);

    for (PPNeuron *&neuron : snn.ppn)
    {
      count = neuron->spikes.size();
      fwrite(&count, sizeof(int), 1, file);
      fwrite(&neuron->spikes[0], sizeof(double), count, file);
    }

    for (Neuron *&neuron : snn.sn)
    {
      count = neuron->spikes.size();
      fwrite(&count, sizeof(int), 1, file);
      fwrite(&neuron->spikes[0], sizeof(double), count, file);
    }
  }

  std::cout << "\r results written to file!  " << std::endl;

  return 0;
}
