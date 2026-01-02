#pragma once

#include <zmq.hpp>
#include <array>
#include <functional>

namespace transport {
  
  template <typename T, std::size_t N>
  class Publisher
  {

    public:

      Publisher()
        : context_(1),
          socket_pub_(context_, zmq::socket_type::pub)
      {

      };

      void create_publisher(const std::string& topic)
      {
        ipc_ = "ipc://@" + topic;
        socket_pub_.bind(ipc_);
      }

      bool publish(const std::array<T,N>& data_array)
      {
        return zmqPub(data_array);
      }

    private:

      zmq::context_t context_;
      zmq::socket_t socket_pub_;
      std::string ipc_;

      bool zmqPub(const std::array<T,N>& data)
      {
        bool sent = false;
        zmq::recv_result_t result;

        zmq::message_t msg(sizeof(T) * N);
        std::memcpy(msg.data(), data.data(), msg.size());

        result = socket_pub_.send(msg, zmq::send_flags::dontwait);

        if(result)
          sent = true;

        return sent;
      }
    
  };


  template <typename T, std::size_t N>
  class Subscriber
  {

    public:

      using Callback = std::function<void(std::array<T,N>&)>;

      Subscriber()
        : context_(1),
          socket_sub_(context_, zmq::socket_type::sub)
      {
        socket_sub_.set(zmq::sockopt::subscribe, "");
      };

      void create_subscriber(const std::string& topic, Callback callback=nullptr)
      {
        ipc_ = "ipc://@" + topic;
        socket_sub_.connect(ipc_);

        callback_ = callback;
      }

      void spin_once(){

        bool received = zmqSub(data_array_);

        if (received)
          callback_(data_array_);
      }

    private:

      zmq::context_t context_;
      zmq::socket_t socket_sub_;
      std::string ipc_;
      std::array<T,N> data_array_;

      Callback callback_;

      bool zmqSub(std::array<T,N>& data_array)
      {

        bool received = false;
        zmq::message_t msg;
        zmq::recv_result_t result;
        
        result = socket_sub_.recv(msg, zmq::recv_flags::dontwait);

        if (result) {

          // Wait for T type with N types
          if (msg.size() != sizeof(T) * N)
          {
            std::cerr << "Unexpected message size\n";
          }

          T* data_ptr = static_cast<T*>(msg.data());

          std::ranges::copy(
            std::span<const T>(data_ptr, N),
            data_array.begin()
          );

          received = true;
        }

        return received;
      }
    
  };

}