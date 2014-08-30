//资源管理类中提供对原始资源的访问

class Font
{
public:
	explicit Font(FontHandle fh) : f(fh)
	{}

	//可以提供两种访问原始资源的方式：
	//1 显示转换函数,优点是安全，缺点是每次调用都需要访问该函数
	FontHandle get() const
	{
		return f;
	}

	//2 隐式转换，重载转换操作符,优点不需显示调用，会隐式执行，缺点：容易出错
	operator FontHandle() const
	{
		return f;
	}

	~Font()
	{
		releaseFont(f);
	}
private:
	FontHandle f;  //管理的原始字体资源
};

Font f1(getFont());
FontHandle f2 = f1;  //本来是想拷贝Font对象，却隐式转换f1为FontHandle,然后进行了复制

