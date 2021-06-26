//Any type based on https://habr.com/ru/post/207294/

#pragma once

#include<memory>
#include<stdexcept>

class Any {

public:
    template<typename T>
    Any(const T& t): holder(std::make_unique<T>(t)) {}

    template<typename U>
    U cast() const {
        if(typeid (U) != holder->type_info()) {
            throw std::runtime_error("Bad any cast");
        }
        return static_cast<Holder<U>*>(holder.get())->object;
    }


private:

    struct BaseHolder {
        virtual ~BaseHolder();
        virtual const std::type_info& type_info() const = 0;

    };

    template<typename T>
    struct Holder: BaseHolder {
        Holder(const T& t): object(t) {}

        const std::type_info& type_info() const override {
            return typeid (object);
        }

        T object;
    };

    std::unique_ptr<BaseHolder> holder;
};
