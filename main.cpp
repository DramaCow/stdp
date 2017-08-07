#include <iostream>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include "neuron.hpp"
#include "synapse.hpp"
#include "event.hpp"
#include "units.hpp"
#include "SNN.hpp"

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // global config
  const double dt_max = 0.0005;

  // initialise simulation state
  SNN snn;

  // initialise event queue
  EventManager EM;
  EM.insert(new EpochEvent(0.0, 0));
  EM.insert(new EpochEvent(0.0, 1));
  EM.insert(new RecordEvent(0.0, 0));

  // main loop
  while (EM.t_sim <= EM.duration && EM.size() > 0)
  {
    Event *e = EM.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (EM.t_sim < e->time && !synch_event_inserted)
    {
      double dt = (dt_max <= (e->time-EM.t_sim)) ? dt_max : (e->time-EM.t_sim);
      EM.t_sim += dt;

      for (IFNeuron *&neuron : snn.sn)
      {
        neuron->step(dt);
        if (neuron->is_spiking())
        {
          EM.insert(new SpikeEvent(EM.t_sim, neuron));
          synch_event_inserted = true;
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

  // figure number as string
  std::string fig_num(argv[1]);

  // export results to binary files
  {
    IFNeuron *N = snn.sn[0];
    FILE* file = fopen((fig_num + "A.dat").c_str(), "wb");
    int count = N->t_record.size();
    int num_plots = 1;
    double ymin = -74.0*mV, ymax = -54.0*mV;
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    fwrite(&N->t_record[0], sizeof(double), count, file);
    fwrite(&N->V_record[0], sizeof(double), count, file);
    fclose(file);
  }
  {
    IFNeuron *N = snn.sn[0];
    FILE* file = fopen((fig_num + "B.dat").c_str(), "wb");
    int count = N->t_record.size();
    double ymin = 0.0, ymax = 0.75;
    int num_plots = 1;
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    fwrite(&N->t_record[0], sizeof(double), count, file);
    fwrite(&N->g_record[0], sizeof(double), count, file);
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "C.dat").c_str(), "wb");
    int count = snn.an.size();
    int num_plots = 1;
    double ymin = 0.0, ymax = 1.0;
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    for (double id = 0.0; id < snn.an.size(); id+=1.0)
    {
      fwrite(&id, sizeof(double), 1, file); 
    }
    for (std::size_t id = 0; id < snn.an.size(); ++id)
    {
      for (Synapse *&sy : snn.con.out(snn.an[id]))
      {
        double w = sy->get_w()/W_MAX;
        fwrite(&w, sizeof(double), 1, file);
      }
    }
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "D.dat").c_str(), "wb");
    int count = EM.rec_entries;
    int num_plots = 2;
    double ymin = 0.0, ymax = 1.0;
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    fwrite(&EM.t_record[0], sizeof(double), count, file);
    fwrite(&EM.w1_record[0], sizeof(double), count, file);
    fwrite(&EM.w2_record[0], sizeof(double), count, file);
    fclose(file);
  }

  return 0;
}
