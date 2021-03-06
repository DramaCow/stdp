/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef DOPAMINERGICSYNAPSE_H
#define DOPAMINERGICSYNAPSE_H

#include "synapse.hpp"

class DopaminergicSynapse : public Synapse
{
  public:
    DopaminergicSynapse(Neuron *pre, Neuron *post, double w, 
      double rho, double theta, double a0, double k_p_hi, double k_n_hi, double k_p_lo, double k_n_lo, double &d);

    virtual void pre_spike(double t);
    virtual void post_spike(double t);

    virtual void write(std::string fig);

  //private:
  public:
    double alpha(double d);
    double z_p(double d, double dt);
    double z_n(double d, double dt);

    double x;   // presynaptic trace
    double t_x; // last presynaptic trace update
    double y;   // postsynaptic trace
    double t_y; // last postsynaptic trace update

    double rho;   // Naka-Rushton exponent
    double theta; // Nake-Rushton threshold
    double a0;    // maximum value
    double tau_p; // kernel p time const.
    double tau_n; // kernel n time const.
    double tau_g; // eligibility time const.
    double mu;    // learn rate

    double k_p_hi;
    double k_n_hi;
    double k_p_lo;
    double k_n_lo;

    const double &d; 
};

#endif
