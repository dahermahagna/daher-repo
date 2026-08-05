#ifndef __HANDLETON__
#define __HANDLETON__
#ifdef I_AM_SHARED_LIBRARY
#include <pthread.h>
#include <stdlib.h>
// in case we are compiling a shared library which instantiate the handleton
template <class T> 
class Handleton {
private:
	Handleton (){}
	~Handleton(){}
public:
	static T *GetInstance () {
		T *temp = s_inst;

		asm volatile("mfence":::"memory"); // Prevent CPU reordering 
		asm volatile("":::"memory"); // Prevent compiler reordering

		if (0 == temp) {
			pthread_mutex_lock(&lock); // preferred solution scope lock
			temp = s_inst;
			if (0 == temp) {
				temp = new T();
				asm volatile("mfence":::"memory"); // Prevent CPU reordering 
				asm volatile("":::"memory"); // Prevent compiler reordering
				::atexit(&Handleton<T>::DestroyInstance);
				s_inst = temp;
			}
			pthread_mutex_unlock(&lock);
		}
		return temp;
	}
private:
	static pthread_mutex_t lock;
	static T *s_inst;
	
	static void DestroyInstance() 
 	 {
		Handleton<T> h;
		T *ptr = s_inst;
		s_inst = reinterpret_cast<T*>(0xdeadbeef);
		asm volatile("mfence":::"memory"); // Prevent CPU reordering 
		asm volatile("":::"memory"); // Prevent compiler reordering
		delete ptr;
	  }
};
	template <class T>
	pthread_mutex_t Handleton<T>::lock;
	
	template <class T>
	T *Handleton<T>::s_inst = 0;

 	

#define INSTANTIATE_HANDLETON(T) template class Handleton<T>; // We need to do instantiation 

#else // in case we need forward declaration of the class (in executable / other shared objects which "uses" the singleton)
template<class T> 
class Handleton {
private:
	Handleton ();
	~Handleton();
public:
	static T *GetInstance ();
};
#endif //I_AM_SHARED_LIBRARY

#endif // __HANDLETON