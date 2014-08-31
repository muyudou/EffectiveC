//明智而审慎的使用private继承

class Timer
{
public:
	explicit Timer(int tickFrequency);
	//定时器每滴答一次，函数就自动被调用
	virtual void onTick() const;       
};

//Widget不是Timer,Timer只是帮助实现它，所以私有继承，函数onTick在Widget变成
//私有的，且把它放在私有域中，如果public则有可能调用阿
class Widget : private Timer
{
private:
	virtual void onTick() const; //查看Widget的数据等等， 
};

//使用复合实现以上方法
class Widget
{
private:
	//使用一个私有的嵌套类，而这个类共有继承Timer，并实现onTick方法
	class WidgetTimer : public Timer
	{
	public:
		virtual void onTick() const;
	};
    WidgetTimer timer;   //在包含一个嵌套类对象，则可以调用了呀
};

//空类例子
class Empty {};   //C++插入一个char成为一个字节的大小

//以下这个类不再是一个int大小，还包括empty的1字节，如果还有对齐，那就会有
//3个padding了，多一个int了，sizeof(HoldAnInt) > sizeof(int)
class HoldsAnInt
{
private:
	int x;
	Empty e;
};

//如果继承一个空类，则有空白基类最优化的情况发生EBO
//以下empty不占空间，sizeof(HoldAnInt) == sizeof(int)
class HoldsAnInt : private Empty
{
private:
	int x;
};
