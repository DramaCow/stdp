#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <random>
#include "neuron.hpp"
#include "SNN.hpp"

class EventManager; // forward declaration

class Event
{
  public:
    Event(double time);
    virtual ~Event() {} // necessary for polymorphism
    const double time;

    virtual void process(EventManager &EM, SNN &snn) = 0;
};

class EventManager
{
  public:
    EventManager(double duration);
    ~EventManager();

    void insert(Event *event);
    Event * get_min();
    void del_min();
    int size();

    const double duration;
    const double epoch_freq;
    std::array<double,2> t_epoch;

    std::mt19937 gen; // random number generator

    const double rec_period;

  private:
    std::vector<Event*> event_list;
    int current_size;
    void perc_up(int idx);
    void perc_down(int idx);
    int min_child(int idx);
};

class SpikeEvent : public Event
{
  public:
    SpikeEvent(double time, Neuron *neuron);
    Neuron *const neuron;

    virtual void process(EventManager &EM, SNN &snn);
};

class EpochEvent : public Event
{
  public:
    EpochEvent(double time, int group_id);
    const int group_id;

    virtual void process(EventManager &EM, SNN &snn);
};

class RecordEvent : public Event
{
  public:
    RecordEvent(double time, int idx);
    const int idx;

    virtual void process(EventManager &EM, SNN &snn);
};

// ===

class BaselineEvent : public Event
{
  public:
    BaselineEvent(double time, int it);
    const int it;

    virtual void process(EventManager &EM, SNN &snn);
};

class LearningEvent : public Event
{
  public:
    LearningEvent(double time, int it);
    const int it;

    virtual void process(EventManager &EM, SNN &snn);
};

class IntermissionEvent : public Event
{
  public:
    IntermissionEvent(double time, int it);
    const int it;

    virtual void process(EventManager &EM, SNN &snn);
};

class ExtinctionEvent : public Event
{
  public:
    ExtinctionEvent(double time, int it);
    const int it;

    virtual void process(EventManager &EM, SNN &snn);
};

class PostExtinctionEvent : public Event
{
  public:
    PostExtinctionEvent(double time, int it);
    const int it;

    virtual void process(EventManager &EM, SNN &snn);
};

#endif
