#ifndef COMMON_NONCOPYABLE_H_
#define COMMON_NONCOPYABLE_H_

namespace common {

// 使用时需要private继承这个类，因为是"has-a"的关系

//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

//  Contributed by Dave Abrahams
//
namespace noncopyable_ // protection from unintended ADL
{
	class noncopyable
	{
	protected:
		noncopyable() { }
		~noncopyable() { }
	private:  // emphasize the following members are private
		noncopyable(const noncopyable&);
		const noncopyable& operator=(const noncopyable&);
	};
}

typedef noncopyable_::noncopyable noncopyable;

} // namespace common

#endif  // COMMON_NONCOPYABLE_H_
