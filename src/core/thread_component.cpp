#include "quadruped_locomotion/core/thread_component.hpp"

namespace core
{

ThreadComponent::~ThreadComponent()
{
  stopBackgroundSpin();
}

void ThreadComponent::startBackgroundSpin()
{
  if (transport_thread_.joinable()) {
      throw std::logic_error("Background spin already running");
  }

  transport_thread_ = std::jthread(
    [this](std::stop_token st){
      this->spin(st, 100);
    });
}

void ThreadComponent::stopBackgroundSpin()
{
  if (transport_thread_.joinable())
  {
    transport_thread_.request_stop();
  }

}

void ThreadComponent::spin(std::stop_token st, double frequency){

  using clock = std::chrono::steady_clock;
  auto period = std::chrono::duration_cast<clock::duration>(
      std::chrono::duration<double>(1.0 / frequency)
  );
  auto next = clock::now();

  while(!st.stop_requested())
  {
    next += period;
    spinOnceInternal();
    std::this_thread::sleep_until(next);
  }
}

void ThreadComponent::spinOnce()
{
  if (!transport_thread_.joinable())
    spinOnceInternal();
  else
    throw std::logic_error("spinOnce() not allowed when using background spin");
}

}