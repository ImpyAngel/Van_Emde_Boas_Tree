#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "veb.h"

template <unsigned int T>
struct VEBTree : public AbstractVEBTree<T> {

private:
    bool empty;
    unsigned long long min, max;
    std::map<unsigned long long, std::shared_ptr<VEBTree<T / 2>>> data;
    unsigned long long higher(unsigned long long value) const {
        return value >> T;
    }
    unsigned long long lower(unsigned long long value) const {
        return ((value >> T) << T)^value  ;
    }

public:
    bool is_empty(){
        return empty;
    }

    VEBTree() {
        empty = true;
    }

    VEBTree(unsigned long long value) {
        empty = false;
        min = max = value;
    }

    void add(unsigned long long value) {
        if (is_empty()) {
            min = max = value;
            empty = false;
            return;
        }
        if (min == max) {
            min = value;
            if (min > max) {
                std::swap(min, max);
            }
            return;
        }
        if (value > max) {
            std::swap(value, max);
        }

        if (value < min) {
            std::swap(value, min);
        }
        if (data.find(higher(value)) == data.end()) {
        data.insert(std::pair<unsigned long long, std::shared_ptr<VEBTree<T/2>>>(higher(value),
                                                                                     std::make_shared<VEBTree<T/2>>(lower(value))));
        } else {
            data.at(higher(value))->add(lower(value));
        }
    }

    void remove(unsigned long long value) {
        if (value == min) {
            if (min == max) {
                empty = true;
                return;
            }
        if (data.empty()) {
            min = max;
            return;
        }
        unsigned long long new_min = data.begin()->second->getMin();
        min = new_min + (data.begin()->first << T);
        data.begin()->second->remove(new_min);
        if (data.begin()->second->is_empty()) {
            data.erase(data.begin());
        }
         return;
        }
        if (value == max) {
            if(data.empty()) {
                max = min;
                return;
            }
            unsigned long long new_max = data.rbegin()->second->getMax();
            max = new_max + (data.rbegin()->first << T);

            data.rbegin()->second->remove(new_max);
            if (data.rbegin()->second->is_empty()) {
                data.erase(data.rbegin()->first);
            }
            return;
        }
        data.at(higher(value))->remove(lower(value));
        if (data.at(higher(value))->is_empty()) {
            data.erase(higher(value));
        }
    }

    unsigned long long next(unsigned long long value) const {
        if (value == min) {
            if (data.empty()) {
                return getMax();
            }
            unsigned long long new_min = data.begin()->second->getMin();
            return new_min + (data.begin()->first << T);
        }
        if (data.at(higher(value))->getMax() == lower(value)) {
            if (data.find(higher(value))->first == data.rbegin()->first) {
                return max;
            }
            auto it = ++data.find(higher(value));
            return it->second->getMin() + (it->first << T);
        }
        return data.at(higher(value))->next(lower(value)) + (higher(value) << T);
    }
    unsigned long long prev(unsigned long long value) const {
        if (value == max) {
            if (data.empty()) {
                return getMin();
            }
            unsigned long long new_max = data.rbegin()->second->getMax();
            return new_max + (data.rbegin()->first << T);
        }
        if (data.at(higher(value))->getMin() == lower(value)) {
            if (data.find(higher(value))->first == data.begin()->first) {
                return min;
            }
            auto it = --data.find(higher(value));
            return it->second->getMax() + (it->first << T);
        }
        return data.at(higher(value))->prev(lower(value)) + (higher(value) << T);
    }

    unsigned long long getMin() const {
        return min;
    }
    unsigned long long getMax() const {
        return max;
    }
};

int main() {
    VEBTree<20> tree;
     tree.add(5);
     tree.add(11);
     tree.add(10);
     std::cout << tree.next(5) << std::endl;
     tree.remove(10);
     std::cout << tree.next(5) << std::endl;
}

