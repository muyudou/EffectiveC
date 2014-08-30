//一般所指的placement new
void* operator new(std::size_t, void* pMemory) throw();  

//防止构造函数期间的内存泄漏
class Widget
{
public:
	...
	//一个placement new
	static void* operator new(std::size_t size, std::ostream& logStream)
	    throw(std::bad_alloc);
	//提供placement delete,防止operator new成功，构造函数失败时造成的内存泄漏
	static void* operator delete(std::size_t size, std::ostream& logStream)
	    throw();
	//operator new正常运行时调用的delete
	static void* operator delete(void* pMemory, std::size_t size)
		throw();
};

//以下语句如果引发Widget构造函数抛出异常,对应的placement delete会被调用，不会发生内存泄漏
Widget* pw = new (std::cerr) Widget;

//如果没有抛出异常，客户端有如下代码，则会调用正常的delete
delete pw;

//发生函数隐藏
class Base
{
public:
	...
	static void* operator new(std::size_t size, std::ostream& logStream)
	    throw (std::bad_alloc);   //会掩盖正常的global形式
	...
};

Base* pb = new Base;   //错误！因为正常的operator new被掩盖
Base* pb = new (std::cerr) Base;   //正确，调用Base的placement new

//继承类掩盖全局与基类的new
class Derived : public Base
{
public:
	...
	//重新声明正常形式的new
	static void* operator new(std::size_t size) throw (std::bad_alloc);
    ...
};

Derived* pb = new (std::clog) Derived; //错误！因为被掩盖了
Derived* pb = new Derived;  //正确，调用derived的new


//std中的operator new
void* operator new(std::size_t) throw(std::bad_alloc);  //normal new
void* operator new(std::size_t, void*) throw();         //placement new
void* operator new(std::size_t, const std::nothrow_t&) throw(); //nothrow new

//解决名称掩盖
//提供一个基类内含所有的正常形式的new和delete, 
class StandardNewDeleteForms
{
public:
	//normal new/delete
    static void* operator new(std::size_t size) throw(std::bad_alloc)
	{
		return ::operator new(size); //调用正常的new
	}
	static void* operator delete(void* pMemory) throw()
	{
		return ::operator delete(pMemory); //调用正常的delete
	}

	//placement new/delete
    void* operator new(std::size_t size, void* ptr) throw()
	{
        return ::operator new(size, ptr);
	}
    void* operator delete(void* pMemory, void* ptr) throw()
	{
        return ::operator delete(pMemory, ptr);
	}

	//nothrow new/delete
    void* operator new(std::size_t size, const std::nothrow_t& nt) throw()
	{
		return ::operator new(size, nt)
	}
    void* operator delete(void* pMemory, const std::nothrow_t& nt) throw()
	{
		return ::operator new(pMemroy);
	}
};

//如果想以自定义形式扩充标准形式，则可利用继承机制及using声明式取得标准形式
class Widget : public StandardNewDeleteForms
{
public:
	using StandardNewDeleteForms::operator new;  //使这些形式可见
	using StandardNewDeleteForms::operator delete;

	//添加自定义的placement new/delete,这样就可以在这些函数中调用标准形式的了
	static void* operator new(std::size_t size, std::ostream& logStream)
	    throw(std::bad_alloc);
	static void* operator delete(std::size_t size, std::ostream& logStream)
	    throw();
};
