//资源管理类的copying行为

class Lock
{
public:
	explicit Lock(Mutex* pm) : mutexPtr(pm)
	{
		lock(mutexPtr); //构造函数锁住资源
	}

	~Lock()
	{
		unlock(mutexPtr);  //析构函数释放资源
	}
};

//客户端的用法
Mutex m;
Lock ml(&m);  //锁定，执行关键区域内的操作
...

//如果进行拷贝,可以发生以下情况：
Lock ml1(&m);
Lock ml2(ml1);

//情况1：禁止复制，参考条款6的做法，
//1声明为私有，不定义，如果复制发生链接错误
//2建一个不能拷贝的基类，并私有继承于它,这将错误移到编译期
class Lock : private Uncopyable
{
};

//情况2：引用计数法，例如shared_ptr的行为，但当资源变为0时，它的默认行为是
//删除所指物，而如果我们只是释放锁定，这种情况下可以利用shared_ptr中可以指定“删除器”
//的一个函数或函数对象，当引用计数为0时，调用此函数，这个参数对它的构造函数是可有可无的第二个参数
class Lock
{
public:
	explicit Lock(Mutex* pm) : mutexPtr(pm, unlock) //unlock即为指定的删除器
	{
		lock(mutexPtr.get());
	}
private:
	std::tr1::shared_ptr<Mutex> mutexPtr; //用shared_ptr管理这个对象，并且指定删除器，自定义行为
};
//而且以上不再需要析够函数，因为析构函数会在引用计数为0时自动调用shared_ptr的删除器

//情况3：复制底部资源，进行深度拷贝，例如string类，

//情况4：转移底部所有权，如auto_ptr的行为，资源所有权从被复制物转移到目标物


