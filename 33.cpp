//避免掩盖继承而来的名称

//如以下类
class Base
{
private:
	int x;
public:
	virtual void mf1()=0;
	virtual void mf1(int);

	virtual void mf2();
	
	void mf3();
	void mf3(double);
};

class Derived : public Base
{
public:
	virtual void mf1();   //这里会将基类的mf1,mf1(int)都隐藏掉，遮盖了名称
	void mf3(); //这里也如mf1一样，将基类的两个mf3遮盖
	void mf4();
};

Derived d;
int x;

d.mf1();    //Derived::mf1
d.mf1(x);   //错误，因为继承类掩盖了基类的mf1(int)
d.mf2();    //Derived::mf2
d.mf3();    //Derived::mf3
d.mf3(x);   //错误，掩盖了基类的mf3(int)

//为了避免名称被掩盖，1 public继承下可以使用using声明式
class Base
{
private:
	int x;
public:
	virtual void mf1()=0;
	virtual void mf1(int);

	virtual void mf2();
	
	void mf3();
	void mf3(double);
};

class Derived : public Base
{
public:
	using Base::mf1;  //让base类名为mf1和mf3的所有东西（函数了变量了）
	using Base::mf3;  //在Derived作用域内都可见（并且是public）
	virtual void mf1(); 
	void mf3();
	void mf4();
};
d.mf1();    //Derived::mf1
d.mf1(x);   //没问题，Base::mf1(int)
d.mf2();    //Derived::mf2
d.mf3();    //Derived::mf3
d.mf3(x);   //没问题，Base::mf3(int)

//为了避免名称被掩盖，2 使用转交函数继承一个函数
class Base
{
private:
	int x;
public:
	virtual void mf1()=0;
	virtual void mf1(int);
    ...
};

class Derived : private Base
{
public:
	virtual void mf1()
	{
		Base::mf1();   //转交函数，暗自成为inline
	}
};
d.mf1();    //Derived::mf1
d.mf1(x);   //错误，名称被掩盖
