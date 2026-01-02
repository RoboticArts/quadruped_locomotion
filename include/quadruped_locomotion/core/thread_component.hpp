#pragma once

#include <thread>
#include <atomic>

namespace core
{

class ThreadComponent 
{
  public:

    virtual ~ThreadComponent();

    void startBackgroundSpin();
    void stopBackgroundSpin();
    void spinOnce(); 

  protected:

    virtual void spinOnceInternal() = 0;

  private:

    void spin(std::stop_token st, double frequency);

    std::jthread transport_thread_;

};

}