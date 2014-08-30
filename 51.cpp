//编写new和delete时需固守常规

void* operator new(std::size_t size) throw(std::bad_alloc)
{
	using namespace std;
	if (size == 0)  //如果是0 byte, 则当成1byte
		size = 1;

	//循环尝试分配size bytes;
	while (true)
	{
        尝试分配size bytes;
		if (分配成功)
			return (一个指针，指向分配得来的内存);

		//分配失败，找出目前的new-handling函数，因为没有直接获得该值的函数，
		//所以使现在的handler=null,利用其返回值返回以前的handler;
		new_handler globalHandler = set_new_handler(0); 
		set_new_handler(globalHandler);  //安装

		//如果非空，则调用，失败则抛出异常
		if (globalHandler)
			(*globalHandler)();
		else
			throw std::bad_alloc();
	}
}

class Base
{
public:
	static void* operator new(std::size_t size) throw(std::bad_alloc);
	static void* operator delete(void* rawMemory, std::size_t size) throw();
	...
};

//假设类未声明operator new,则会继承基类的operator new
class Derived : public Base
{
	...
};

Derived* p = new Derived; //调用基类的operator new,但基类专属的operator new并非用来对付上述这种大小的对象

//解决方案，operator new类版本
void* Base::operator new(std::size_t size) throw(sts::bad_alloc)
{
	if (size != sizeof(Base))   //如果大小错误，则调用标准的operator new,这里省略了对0的判断，因为sizeof(类)不会返回0
		return ::operator new(size);
	...
}

//定制operator delete
void operator delete(void* rawMemory) throw()
{
	if (rawMemory == 0) //如果被删除的是个null指针，则什么都不做
		return;
	以下归还rawMemory内存
}

//operator delete类版本
void Base::operator delete(void* rawMemory, std::size_t size) throw()
{
	if (rawMemory == 0)  //如果为null，什么都不做
		return;
	if (size != sizeof(Base))  //如果大小错误，调用标准版
	{
		::operator delete(rawMemory);
		return;
	}

	现在，归还rawMemory所指内存
	return;
}
