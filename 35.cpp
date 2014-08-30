//virtual函数替代方案，例子

//游戏人物的基类，会有不同的继承类
class GameCharacter
{
public:
	//返回人物健康指数,不是纯虚，说明是有一个计算健康指数的缺省算法
	virtual int healthValue() const;  
};

//方案1：NVI手法
class GameCharacter
{
public:
	int healthValue() const
	{
		//可以做一些事前工作，如包括锁定互斥器，制造运转日志记录，验证classu
		//约束条件，验证函数先决条件等等
		...
		int retVal = doHealthValue();
		...
		//调用结束后还可以做一些事后工作，包括互斥器接触锁定，验证函数的事后
		//条件，再次验证类约束条件
		return retVal;
	}
private:
	virtual int doHealthValue() const   //继承类可以重定义它
	{
		...//缺省算法，计算健康指数
	}
};

//2 成员函数指针实现stategy模式
class GameCharactor;  //前置声明
//计算健康指数的缺省算法
int defaultHealthCalc(const GameCharacter& gc);
class GameCharacter
{
public:
	//定义函数类型
	typedef int (*HealthCalcFunc)(const GameCharacter&);
	explicit GameCharacter(HealthCalcFunc hcf=defaultHealthCalc)
		: HealthFunc(hcf)
	{}

	int healthValue() const
	{
		return healthFunc(*this);
	}
private:
	HealthCalcFunc heathFunc;
};
//GameCharacter可提供一个成员函数setHealthCalculator,
//用来替换当前的健康指数计算函数,则函数可以在运行时变更

//同一人物类型的不同实体可以有不同的健康计算函数
class EvilBadGuy : public GameCharacter
{
public:
	explicit EvilBadGuy(HealthCalcFunc hcf=defaultHealthCalc)
		: GameCharacter(hcf)
	{...}
};

//不同的健康指数计算函数
int loseHealthQuickly(const GameCharacter&); 
int loseHealthSlowly(const GameCharacter&);

//相同类型的人物搭配不同的健康计算方式
EvilBadGuy ebg1(loseHealthQuickly);
EvilBadGuy ebg2(loseHealthSlowly);

//方案3：tr1::function函数实现strategy模式
class GameCharactor;  //前置声明
//计算健康指数的缺省算法
int defaultHealthCalc(const GameCharacter& gc);
class GameCharacter
{
public:
	//代表函数是接受一个reference指向const GameCharacter,并返回int,而且这个
	//函数对象可以持有任何与此前面式兼容的可调用物，即参数可被隐式转换为
	//const GameCharacter&,返回值可被隐式转换为int,这个也是与方案2类的唯一
	//区别，不是持有一个指针，而是持有一个tr1::function对象，相当于指向函
	//数的泛化指针
	typedef std::tr1::function<int (const GameCharacter&)> HealthCalcFunc;
	explicit GameCharacter(HealthCalcFunc hcf=defaultHealthCalc)
		: HealthFunc(hcf)
	{}

	int healthValue() const
	{
		return healthFunc(*this);
	}
private:
	HealthCalcFunc heathFunc;
};
//健康计算函数：返回是是non-int,但是可以隐式转换为int,可调用
short calcHealth(const GameCharacter&);
//函数对象
struct HealthCalculator
{
	int operator() (const GameCharacter&) const
	{...}
};

class GameLevel
{
public:
	//成员函数，用于计算健康值，non-int返回类型
    float health(const GameCharacter&) const;
};

//不同的人物类型，构造函数相同
class EvilBadGuy : public GameCharacter
{};
class EyeCanyCharacter : public GameCharacter
{};

EvilBadGuy ebg1(calHealth);  //人物1，使用函数计算
EyeCandyCharacter ecc1(HealthCalculator()); //人物2，使用函数对象计算
//使用类的成员函数计算
GameLevel currentLevel;
EvilBadGuy ebg2(str::tr1::bind(&GameLevel::health, currentLevel, _1));

//方案4：古典的Strategy模式
class GameCharacter;
class HealthCalcGunc
{
public:
	...
	virtual int calc(const GameCharacter& gc) const
	{}
};

class SlowHealthLoser : public HealthCalcFunc
{};
class FastHealthLoser : public HealthCalcFunc
{};

HealthCalcFunc defaultHealthCalc;
class GameCharacter
{
public:
	explicit GameCharacter(HealthCalcFunc* phcf = &defaultHealthCalc)
		: pHealthCalc(phcf)
	{}
	int healthValue() const
	{
		return pHealthCalc->calc(*this);
	}
private:
	HealthCalcFunc* pHealthCalc;
};
//不同的人物类型，构造函数相同
class EvilBadGuy : public GameCharacter
{};
class EyeCanyCharacter : public GameCharacter
{};
