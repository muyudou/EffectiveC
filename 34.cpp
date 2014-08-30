//非纯虚函数的缺省实现提供

class Airport {...};
class Airplane
{
public:
	virtual void fly(const Airport& destination);
	...
};

void Airplane::fly(const Airport& destination)
{
	缺省代码，将飞机飞向指定的目的地
}

class ModelA : public Airplane {...};
class ModelB : public Airplane {...};
//以上继承体系，A，B中fly的相同行为可以写入基类的fly，如果两者飞行方式相同，则不需重新定义fly,可以直接继承基类的实现。
//如果有区别可以调用基类的，不同的行为可以再在这个函数中写出来
//如果新增一个飞机C，且飞行方式不一样，为了防止忘记重新定义fly函数，即
//基类实现“提供缺省实现给derived classes”,但除非它们明确提出才能调用缺省实现，一种方法是切断“virtual”函数接口和“实现接口”之间的链接，如下
class Ariplane
{
public:
    //声明为纯虚的保证子类必须被重写
    virtual void fly(const Airplane& destination=0)=0;
	...
protected: //定义为protected是因为只有继承类需要此函数，外部不应调用，也不关心
	//将默认行为移至另外一个函数，使得子类必须显示调用该函数才能继承默认行为
	void defaultFly(const Airport& destination);	
};

void Airplane::defaultFly(const Airport& destination)
{
	提供缺省行为
}

class ModelA : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		defaultFly(destination);
	}
};


class ModelB : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		defaultFly(destination);
	}
};

//这样新增的moduleC不可能意外继承fly的实现版本了，必须自己提供，且如果不继承
//默认行为则要自己写出来
class ModelC : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		//不继承默认行为，自己实现
	}
};

//但以上方法有人认为不应该以不同的方法分别提供接口和实现继承，则可以利用
//纯虚函数必须在derived classes中重新声明，但他们也可拥有自己的实现这个事实
//给基类的纯虚fly函数提供一个缺省实现
class Ariplane
{
public:
    //声明为纯虚的保证子类必须被重写
    virtual void fly(const Airplane& destination=0)=0;
	...
};

void Airplane::fly(const Airport& destination)
{
	提供缺省行为
}

class ModelA : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		Airplane::fly(destination);
	}
};


class ModelB : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		Airplane::fly(destination);
	}
};

//这样新增的moduleC不可能意外继承fly的实现版本了，必须自己提供，且如果不继承
//默认行为则要自己写出来
class ModelC : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		//不继承默认行为，自己实现
	}
};
