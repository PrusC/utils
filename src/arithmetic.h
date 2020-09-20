#pragma once

#include <memory>
#include <stack>

namespace arithmetic {


struct Node {
    virtual int Evaluate () const = 0;
};


struct Value : public Node {
    Value(char digit) : _value(digit - '0') {}
    int Evaluate () const override {
        return _value;
    }

private:
    const uint8_t _value;
};


struct Variable: public Node {
    Variable(const int &x): _x(x) {}
    int Evaluate() const override {
        return _x;
    }

private:
        const int &_x;
};


struct Operation: public Node {

    Operation(char value):
        _op(value),
        _precedence([value] {
            if(value == '*' || value == '/') {
                return 2;
            } else {
                return 1;
            }
        }()) {}

    int Evaluate() const override {
        if (_op == '*') {
            return _left->Evaluate() * _right->Evaluate();
        }
        else if(_op == '/') {
            return _left->Evaluate() / _right->Evaluate();
        }
        else if(_op == '+') {
            return _left->Evaluate() + _right->Evaluate();
        }
        else if(_op == '-') {
            return _left->Evaluate() - _right->Evaluate();
        }
        return 0;
    }


    void setLeft(std::shared_ptr<Node> node) {
        _left = node;
    }

    void setRight(std::shared_ptr<Node> node) {
        _right = node;
    }

    uint8_t getPrecedence() const {
        return _precedence;
    }

private:
    const char _op;
    const uint8_t _precedence;
    std::shared_ptr<const Node> _left, _right;
};


template <class Iterator>
std::shared_ptr<Node> Parse(Iterator token, Iterator end, const int& x) {
    if(token == end) {
        return std::make_shared<Value>('0');
    }

    std::stack<std::shared_ptr<Node>> values;
    std::stack<std::shared_ptr<Operation>> operations;

    auto popOperations = [&values, &operations] (int precedence) {
        while(!operations.empty() && operations.top()->getPrecedence() >= precedence) {
            auto value1 = values.top();
            values.pop();
            auto value2 = values.top();
            values.pop();
            auto op = operations.top();
            operations.pop();
            op->setRight(value1);
            op->setLeft(value2);
            values.push(op);
        }
    };

    while(token != end) {
        const auto &value = *token;
        if (value >= '0' && value <= '9') {
            values.push(std::make_shared<Value>(value));
        }
        else if(value == '*' || value == '/') {
            popOperations(2);
            operations.push(std::make_shared<Operation>(value));
        }
        else if(value == '+' || value == '-') {
            popOperations(1);
            operations.push(std::make_shared<Operation>(value));
        }
        else if(value == 'x') {
            values.push(std::make_shared<Variable>(x));
        }
        ++ token;
    }

    while(!operations.empty()) {
        popOperations(0);
    }

    return values.top();
}


}
