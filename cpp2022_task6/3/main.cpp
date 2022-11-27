#include "intrusive_ptr.h"

int main() {
    
    TIntrusivePtr<TDoc> ptr = nullptr;
    ptr = MakeIntrusive<TDoc>();
    std::cout << ptr.RefCount() << " == 1" << std::endl;
    // ptr.RefCount() == 1
    TIntrusivePtr<TDoc> ptr2 = ptr; // ptr.RefCount() == 2
    std::cout << ptr.RefCount() << " == 2" << std::endl;
    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();
    ptr3.Reset(ptr2);
    // ptr.RefCount() == ptr3.RefCount() == 3
    std::cout << ptr.RefCount() << " == 3" << std::endl;
    ptr3.Reset();
    // ptr.RefCount() == ptr3.RefCount() == 2
    std::cout << ptr.RefCount() << " == 2" << std::endl;
    ptr3.Reset(std::move(ptr2));
    // ptr.RefCount() == ptr3.RefCount() == 2
    std::cout << ptr.RefCount() << " == 2" << std::endl;
    std::cout << sizeof(TDoc*) << " " << sizeof(ptr) << std::endl;
    static_assert(sizeof(TDoc*) == sizeof(ptr));
}