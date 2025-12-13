#ifndef _STACK_AGC_HPP_
#define _STACK_AGC_HPP_

#include <stack>

template<typename T>
class stack_agc : public std::stack<T> {
public:
	auto& gc() {
		return this->c;
	}
};

#endif
