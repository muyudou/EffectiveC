//在operator=中处理自我赋值

class Bitmap {};

//保存一个指针指向一块动态分配的bitmap
class Widget
{
public:
	//问题的发生：
    Widget& operator=(const Widget& rhs)
	{
		delete pb;
		//如果rhs与this指向同一块内存则错误
		pb = new Bitmap(*rhs.pb);
		return *this;
	}

	//解决方法1：证同测试，
	Widget& operator= (const Widget& rhs)
	{
		if (this == &rhs)
			return *this;
		delete pb;
		//这里可能出现异常问题，如果new发生异常（不论内存不足还是copy构造函数异常）
		//widget会持有一个指针指向一块被删除的bitmap.
		pb = new Bitmap(*rhs.pb);
		return *this;
	}

	//解决方法2：精心安排语句顺序来保证“异常安全”，防止解法1的问题
	Widget& operator=(const Widget& rhs)
	{
		//记住原先指针，再构造一个副本，然后再删除，即删除在构造之后
		Bitmap* porg = pb;
		pb = new Bitmap(*rhs.pb);
		delete porg;
		return *this;
	}

	
	//解法3：2的一个替代方案，即copy and swap技术
	void swap(Widget& rhs)
	{
		//交换数据
	}

	Widget& operator=(const Widget& rhs)
	{
		Widget temp(rhs);
		swap(temp);   //交换*this与temp的数据
		return *this;
	}

	//3的另一个变型解法，依赖以下事实（1） 某类的拷贝赋值操作可能被声明为"by value"方式接受实参
	//（2）以by value方式传递东西会造成另一份副本
	Widget* operator=(Widget rhs)  //这里利用by value构造一个副本
	{
		swap(rhs);   //这里是将*this与副本数据互换，
		return *this;
	}
private:
	Bitmap* pb;  //指向一个从堆上分配的对象
};


