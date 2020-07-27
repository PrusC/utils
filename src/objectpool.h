#pragma once

#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <memory>

template <class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();

    void Deallocate(T* object);

    ~ObjectPool();

private:
    std::queue<std::unique_ptr<T>> free;
    std::unordered_map<T*, std::unique_ptr<T>> allocated;
};

template <typename T>
T* ObjectPool<T>::Allocate() {
    if (free.empty()) {
        free.push(std::make_unique<T>());
    }
    auto ptr = move(free.front());
    free.pop();
    T* ret = ptr.get();
    allocated[ret] = move(ptr);
    return ret;
}

template <typename T>
T* ObjectPool<T>::TryAllocate() {
    if (free.empty()) {
        return nullptr;
    }
    return Allocate();
}

template <typename T>
void ObjectPool<T>::Deallocate(T* object) {
    auto it = allocated.find(object);
    if (it == allocated.end()) {
        throw std::invalid_argument("");
    }
    free.push(move(it->second));
    allocated.erase(it);
}

template <typename T>
ObjectPool<T>::~ObjectPool() {
    for (auto x : allocated) {
        delete x;
    }
    while (!free.empty()) {
        auto x = free.front();
        free.pop();
        delete x;
    }
}
