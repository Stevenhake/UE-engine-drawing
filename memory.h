#include "base.h"

template<class T>
T Ram(uintptr_t ptr)
{
	T buff{};
	if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8)) {
		if (IsBadReadPtr((void*)ptr, sizeof(T)) == 0) {

			buff = *reinterpret_cast<T*>(ptr);
		}
	}
	return buff;
}

template<class T>
T WRT(uintptr_t ptr)
{
	T buff{};
	if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8)) {
		if (IsBadWritePtr((void*)ptr, sizeof(T)) == 0) {
			buff = *reinterpret_cast<T*>(ptr);
		}
	}
	return buff;
}
