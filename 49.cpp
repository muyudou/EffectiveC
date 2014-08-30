//了解new-handler的行为

//set_new_handler函数
namespace std {
    typedef void (*new_handler)();
	new_handler set_new_handler(new_handler p) throw();  //异常声明，表示该函数不抛出任何异常
}

//set_new_handler用法
//以下是operator new无法分配内存时，该被调用的函数
void outOfMem()
{
	std::cerr << "Unable to satisfy request for memory\n";
	std::abort();
}

int main()
{
	std::set_new_handler(outOfMem); //安装内存处理函数
	int *pBigDataArray = new int [100000000L];  //当无法分配时，调用outOfMem;
    ...
}


//提供类专属之new-handler，
class Widget
{
public:
	//客户端需要提供一个new-handler函数专门是该类使用的,
	static std::new_handler set_new_handler(std::new_handler p) throw();
    static void* operator new(std::size_t size) throw(std::bad_alloc); //只抛出bad_alloc异常
private:
	static std::new_handler currentHandler;  //指向当前类使用的new-handler,保存它是为了恢复
};

std::new_handler Widget::currentHandler = 0; //初始化0

//客户端需要提供一个new-handler函数专门是该类使用的,该函数获得这个指针并存储起来，然后返回先前存储的指针。
//这也正式标准的set_new_handler的行为
static std::new_handler Widget::set_new_handler(std::new_handler p) throw();
{
    std::new_handler oldHandler = currentHandler;
	currentHandler = p;
	return oldHandler;
}

//来管理Widget的new-handler，保证可以正常恢复原来的handler
class NewHandlerHolder
{
public:
	explicit NewHandlerHolder(std::new_handler nh) //取得目前的new-handler
		: handler(nh)
	{}

    ~NewHandlerHolder()
	{
		std::set_new_handler(handler);  //释放它，其实就是还原了handler
	}
private:
	std::new_handler handler;  //记录下来

	NewHandlerHolder(const NewHandlerHolder&);  //阻止拷贝
	NewHandlerHolder& operator=(const NewHandlerHolder&);
};

//实现了以上类，则就可以实现Widget's operator new,实现很简单
void *Widget::operator new(std::size_t size) throw(std::bad_alloc)
{
   	//安装Widget的new-handler,并且保存原来的handler,如果第一次调用，原来的是null
	NewHandlerHolder h(std::set_new_handler(currentHandler));

	return ::operator new(size);  //分配内存或抛出异常，回复global new-handler(调用NewHandlerHolder的析构函数恢复)       
}


//客户端可能的调用：
void outOfMem();  //为类声明的handler
Widget::set_new_handler(outOfMem); //设置outOfMem为Widget内存分配失败时调用的函数

Widget *pw1 = new Widget;  //如果内存分配失败，调用outOfMem

std::string* ps = new std::string;  //如果内存分配失败，调用global new-handler,如果有的话

Widget::set_new_handler(0);  //设置Widget专属的为NULL
Wdiget* pw2 = new Widget;    //如果内存分配失败，立刻抛出异常

//适用于每个类的方案
template <typename T>
class NewHandlerSupport
{
public:
	static std::new_handler set_new_handler(std::new_handler p) throw();
	static void* operator new(std::size_t size) throw(std::bad_alloc);
private:
	static std::new_handler currentHandler;
};

template <typename T>
std::new_handler
NewHandlerSuppport<T>::set_new_handler(std::new_handler p) throw()
{
	std::new_handler oldHandler = currentHandler;
	currentHandler = p;
	return oldHandler;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) throw(std::bad_alloc)
{
	NewHandlerHolder h(std::set_new_handler(currentHandler));
	
    return ::operator new(size);
}

//以下将每一个currentHandler初始化为NULL
template <typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;

//只要Widget继承自NewHandlerSupprt<Widget>就可以实现为该类提供一个专属的handler的功能了
class Widget : public NewHandlerSupport<Widget>
{
	...
};

//nothrow new
class Widget {...};
Widget* pw1 = new Widget;  //正常的，分配失败返回bad_alloc
if (pw1 == 0) ... //测试失败

Widget* pw2 = new (std::nothrow) Widget; // 如果分配失败，返回0
if (pw2 == 0) ... //测试可能成功. 

