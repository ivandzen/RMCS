/*
 * slideavgfilter.hpp
 *
 *  Created on: May 14, 2018
 *      Author: ivan
 */

#ifndef DEVICE_SLIDEAVGFILTER_HPP_
#define DEVICE_SLIDEAVGFILTER_HPP_
#include <core/device/oparam.h>
#include <core/device/properties.h>
#include <core/device/defaultnode.h>

template<typename T, uint16_t max_cap>
class SlideAvgFilter :
		DefaultNode
{
public:
	SlideAvgFilter(const char * name, Node * parent) :
		DefaultNode(name, parent),
		_enabled(true),
		_accumulator(0),
		_currentPos(0),
		_cap(128)
	{
		resetFilter();
	}

	inline T exec(T value)
	{
		if(!_enabled)
			return value;

		_accumulator -= _buffer[_currentPos];
		_accumulator += value;
		_buffer[_currentPos] = value;
		++_currentPos;
		if(_currentPos >= _cap)
			_currentPos = 0;
		return T(_accumulator / _cap);
	}

private:
	inline void resetFilter()
	{
		bool lastEn = _enabled;
		_enabled = false;
		_accumulator = 0;
		for(int i = 0; i < max_cap; ++i)
			_buffer[i] = 0;
		_enabled = lastEn;
	}

	BOOL_PROP(Enabled, this, false,
			[this](){ return _enabled; },
			[this](bool value) {
				_enabled = value;
				return true;
			});

	INT_PROP(Capacity, this, max_cap,
			[this](){ return _cap; },
			[this](int value) {
				if((value < 1) || (value > max_cap))
					return false;
				_cap = value;
				resetFilter();
				return true;
			});

	bool	_enabled;
	float 	_accumulator;
	T 		_buffer[max_cap];
	size_t 	_currentPos;
	uint16_t _cap;

};

#endif /* DEVICE_SLIDEAVGFILTER_HPP_ */
