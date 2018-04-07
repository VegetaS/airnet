#ifndef COMMON_BASE_DEFINE_H_
#define COMMON_BASE_DEFINE_H_

//#include��·����˳��-��׼д��
//Names and Order of Includes
//1.preferred location(dir2/foo2.h in foo2.cpp)
//2.C system files
//3.C++ system files
//4.Other libraries'.h files
//5.Your project's .h files



// ��ֹʹ�ÿ������캯���� operator= ��ֵ�����ĺ�
// Ӧ����� private: ��ʹ��
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
			TypeName(const TypeName&); \
			void operator=(const TypeName&)


///
/// safe delete
///
#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) if(NULL != (ptr)) { delete (ptr); (ptr)=NULL; }
#endif // SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(ptr) if(NULL != (ptr)) { delete[] (ptr); (ptr)=NULL; }
#endif // SAFE_DELETE_ARRAY

#ifndef DELETE_SET_NULL
#define DELETE_SET_NULL(ptr) { delete (ptr); (ptr)=NULL; }
#endif // DELETE_SET_NULL


/*
// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.
#define CHECK_NOTNULL(val) \
	::common::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
namespace common {
	template <typename T>
	T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr) {
		if (ptr == NULL) {
			Logger(file, line, Logger::FATAL).stream() << names;
		}
		assert(NULL != ptr); //new add by yc
		return ptr;
	}
}
*/


// game logic related
#define TICK_PER_SEC        20      //һ�����ж��ٸ�tick�����ֵ��������䶯


#endif // COMMON_BASE_DEFINE_H_
