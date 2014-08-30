//绝不在构造和析构过程中调用virtual函数

//如有以下继承体系，希望每创建一个交易对象，都会有一笔日志记录
class Transaction
{
public:
	Transaction() 
	{
		logTransaction();
	};

	virtual void logTransaction() const = 0;
};

//继承类，需要实现logTransaction()
class BuyTransaction : public Transaction
{
public:
	virtual void logTransaction() const;
};

//继承类，需要实现logTransaction()
class SellTransaction : public Transaction
{
public:
	virtual void logTransaction() const;
};

//解决方案：logTransaction改为非虚，然后继承类构造函数传递必要的信息给基类构造函数
class Transaction
{
public:
	explicit Transaction(const std::string& logInfo)
	{
		logTransaction();
	}

	void logTransaction(const std::string& logInfo) const; //非虚
};

class BuyTransaction : public Transaction
{
public:
	BuyTransaction(parameters) 
		: Transaction(createLogString(parameters))
	{}

private:
	//静态的放置“初期未成熟的buytransaction对象内尚未初始化的成员变量”
	static std::string createLogString(parameters); 
};


