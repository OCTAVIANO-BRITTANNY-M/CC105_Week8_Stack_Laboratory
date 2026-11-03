
#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <vector>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}
    };

    Node* topNode;
    int stackSize;

public:
    Stack() : topNode(nullptr), stackSize(0) {}

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    ~Stack() {
        clear();
    }

    void push(const T& value) {
        topNode = new Node(value, topNode);
        ++stackSize;
    }

    bool pop(T& value) {
        if (isEmpty()) {
            return false;
        }

        Node* temp = topNode;
        value = temp->data;
        topNode = topNode->next;
        delete temp;
        --stackSize;

        return true;
    }

    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }

        value = topNode->data;
        return true;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    int size() const {
        return stackSize;
    }

    void clear() {
        while (topNode != nullptr) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }

        stackSize = 0;
    }

    void displayBottomToTop() const {
        std::vector<T> values;
        Node* current = topNode;

        while (current != nullptr) {
            values.push_back(current->data);
            current = current->next;
        }

        for (int i = static_cast<int>(values.size()) - 1; i >= 0; --i) {
            std::cout << values[i];

            if (i > 0) {
                std::cout << " ";
            }
        }

        std::cout << '\n';
    }
};

#endif
