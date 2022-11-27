#include <iostream>
template <typename T>
class TBasePtr {
protected:
    T *ptr = nullptr;
public:
    T* operator ->() const {
        return ptr;
    }

    T& operator *() const {
        return *ptr;
    }

    explicit operator bool() const {
        return bool(ptr);
    }
    friend bool operator == (const TBasePtr& left, const TBasePtr& right);
    friend bool operator != (const TBasePtr& left, const TBasePtr& right);
};

template<typename T>
bool operator == (const TBasePtr<T>& left, const TBasePtr<T>& right) {
        return left.ptr == right.ptr;
    }

template<typename T>
bool operator != (const TBasePtr<T>& left, const TBasePtr<T>& right) {
    return left.ptr != right.ptr;
}

template <typename T>
class TRefCounter {
    int counter = 0;
public:
    virtual void Increase() {
        ++counter;
    }
    
    virtual void Decrease() {
        --counter;
    }

    virtual int RefCount() {
        return counter;
    }
};


template <typename T>
class TIntrusivePtr : public TBasePtr<T> {
public:
    TIntrusivePtr() {
        this->ptr = nullptr;
    }

    TIntrusivePtr(std::nullptr_t) {
        this->ptr = nullptr;
    }

    TIntrusivePtr(T* other_ptr) {
        if (other_ptr) {
            this->ptr = other_ptr;
            this->ptr->Increase();
        }
    }

    TIntrusivePtr(TIntrusivePtr& other) {
        if (other.ptr) {
            this->ptr = other.ptr;
            this->ptr->Increase();
        }
    }

    TIntrusivePtr(TIntrusivePtr&& other) {
        if (other.ptr) {
            this->ptr = other.ptr;
            other.ptr = nullptr;    
        }
    }

    T* Get() {
        return this->ptr;
    }

    int UseCount() {
        return RefCount();
    }

    void Release() {
        if (this->ptr) {
            this->ptr->Decrease();
            if (this->ptr->RefCount() == 0) {
                delete(this->ptr);
            }
            this->ptr = nullptr;
        }
    }

    void Reset() {
        Release();
    }

    void Reset(TIntrusivePtr& other) {
        Release();
        if (other) {
            this->ptr = other.Get();
            this->ptr->Increase();
        }
    }

    void Reset(TIntrusivePtr&& other) {
        Release();
        if (other) {
            this->ptr = other.Get();
            other.ptr = nullptr;
        }
    }

    TIntrusivePtr& operator = (TIntrusivePtr& other) {
        Release();
        Reset(other);
        return *this;
    }

    TIntrusivePtr& operator = (TIntrusivePtr&& other) {
        Release();
        Reset(other);
        return *this;
    }

    int RefCount() {
        if (this->ptr) {
            return this->ptr->RefCount();
        }
        return 0;
    }

    ~TIntrusivePtr() {
        Release();
    }
};

template <typename T, typename ...TParams>
TIntrusivePtr<T> MakeIntrusive(TParams... params) {
    return TIntrusivePtr<T>(new T(std::forward<TParams>(params)...));
}

class TDoc: public TRefCounter<TDoc> {
    
};
