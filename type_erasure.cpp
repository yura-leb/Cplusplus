#include <iostream>
#include <typeinfo>

class IValue {
public:
    virtual const std::type_info& type_info() const = 0;
    virtual ~IValue(){}
};

template<typename T> 
class TValue : public IValue
{
public:
    T value;
    TValue(const T& t) : value(t){}
    const std::type_info& type_info() const
    {
        return typeid(value);
    }
};

class Any {
private:
	IValue* val;
public:

	template<typename T>
	Any(const T& t) : val(new TValue<T>(t)){}
	~Any(){ delete val; }

	template<typename U>
	U get() const
	{
		if(typeid(U) != val->type_info())
			throw std::runtime_error("Bad Any cast");
		return static_cast<TValue<U>* >(val)->value;
    }

};


int main() {
    Any a(5);
    std::cout << a.get<int>() << std::endl; // 5
    std::cout << a.get<std::string>() << std::endl;
}