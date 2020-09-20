/* Template Synchronized is to provide access to an object under a mutex */


#pragma once
#include <mutex>

template <typename T>
class Synchronized {
public:

    explicit Synchronized(T initial = T()): value(move(initial)) {}

    template <typename U>
    struct Access {
        std::lock_guard<std::mutex> guard;
        U& ref_to_value;
    };

    Access<T> GetAccess() {
        return {value, lock_guard(m)};
    }

    Access<const T> GetAccess() const {
        return {lock_guard(m), value};
    }

private:
    T value;
    mutable std::mutex m;
};
