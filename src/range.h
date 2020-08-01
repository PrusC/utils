#pragma once

template <typename It>
class Range {
private:
    It _begin;
    It _end;

public:
    Range(It begin, It end): _begin(begin), _end(end) {}

    It begin() const {
        return _begin;
    }

    It end() const {
        return _end;
    }

};
