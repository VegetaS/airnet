#ifndef COMMON_CURRENTTHREAD_H_
#define COMMON_CURRENTTHREAD_H_

#include <stdint.h>

namespace common {
namespace CurrentThread {
	// internal
	extern __thread int t_cachedTid;
	extern __thread char t_tidString[32];
	extern __thread const char * t_threadName;
	void CacheTid();

	inline int tid()
	{
		if (__builtin_expect(t_cachedTid == 0, 0))
		{
			CacheTid();
		}
		return t_cachedTid;
	}

	inline const char* tid_string() // for logging
	{
		return t_tidString;
	}

	inline const char* name()
	{
		return t_threadName;
	}

	bool IsMainThread();

	void SleepUsec(int64_t usec);
}
} // namespace common

#endif // COMMON_CURRENTTHREAD_H_
