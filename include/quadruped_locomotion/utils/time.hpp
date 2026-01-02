#pragma once
#include <chrono>

namespace quadruped_locomotion::utils{

    /// @brief Gets the current time since epoch in the specified duration type.
    /// @tparam T Duration type (e.g., std::chrono::milliseconds).
    /// @return Current time since epoch as an integer count of the specified duration type.
    template <typename T>
    inline int64_t get_time(){

        return std::chrono::duration_cast<T>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();
    }

    /// @brief Gets the current time since epoch in milliseconds.
    /// @return Current time since epoch in milliseconds.
    inline int64_t get_time_ms(){

        return get_time<std::chrono::milliseconds>();
    }

    /// @brief Gets the current time since epoch in microseconds.
    /// @return Current time since epoch in microseconds.
    template <typename Duration>
    inline Duration make_duration(long long v){
        return Duration(v);
    }

    /// @brief Creates a duration in milliseconds.
    /// @param v Value in milliseconds.
    /// @return Duration object representing the specified milliseconds.
    inline auto ms(long long v) {
        return make_duration<std::chrono::milliseconds>(v);
    }

    /// @brief Creates a duration in microseconds.
    /// @param v Value in microseconds.
    /// @return Duration object representing the specified microseconds.
    inline auto us(long long v) {
        return make_duration<std::chrono::microseconds>(v);
    }

    /// @brief Creates a duration in seconds.
    /// @param v Value in seconds.
    /// @return Duration object representing the specified seconds.
    inline auto sec(long long v) {
        return make_duration<std::chrono::seconds>(v);
    }

    /// @brief Template structure to measure elapsed time in a specified duration type.
    /// @tparam T Duration type (e.g., std::chrono::milliseconds).
    template <typename T>
    struct ElapsedTime{
        using Clock = std::chrono::steady_clock;

        Clock::time_point last = Clock::now();

        operator uint64_t() const {

            Clock::time_point now = Clock::now();
            return std::chrono::duration_cast<T>(now-last).count();
        }

        void reset(){
            last = Clock::now();
        }
    };

    /// @brief Type alias for measuring elapsed time in milliseconds.
    using ElapsedMillis = ElapsedTime<std::chrono::milliseconds>;

    /// @brief Type alias for measuring elapsed time in microseconds.
    using ElapsedMicros = ElapsedTime<std::chrono::microseconds>;

    /// @brief Type alias for measuring elapsed time in seconds.
    using ElapsedSeconds = ElapsedTime<std::chrono::seconds>;


};