#pragma once
#include <mutex>

template <typename T>
class Synchronized {
public:

    explicit Synchronized(T initial = T()): value(move(initial)) {}

    template <typename U>
    struct Access {
        U& ref_to_value;
        std::lock_guard<std::mutex> guard;
    };

    Access<T> GetAccess() {
        return {value, lock_guard(m)};
    }

    Access<const T> GetAccess() const {
        return {value, lock_guard(m)};
    }

private:
    T value;
    mutable std::mutex m;
};
