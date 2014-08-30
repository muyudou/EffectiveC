//不要重写定义继承而来的缺省参数值

//以下有缺省参数值调用时可不指定参数
class Shape
{
public:
	enum ShapeColor {Red, Green, Blue};
	virtual void draw(ShapeColor color = Red) const = 0;
};

class Rectangle : public Shape
{
public:
	//这里赋予了不同的参数值，很糟糕，虽可以不带参数调用，却会造成不同
	virtual void draw(ShapeColor color = Green) const;
};

class Circle : public Shape
{
public:
	//客户端如果调用下面这个函数，有两种情况：1 以对象调用，需指名参数值，
	//因为对象调用是静态绑定并不会从基类继承缺省参数值 2 以指针或reference
	//调用，可以不指定参数值，因为动态绑定下这个函数会从其基类继承缺省参数值
	virtual void draw(ShapeColor color) const;
};

//以下静态类型都是Shape*
Shape* ps;                //动态：未确定，未指向任何对象
Shape* pc = new Circle;   //动态类型是Circle, 
Shape* pr = new Rectangle;//动态类型是Rectangle

//动态类型在程序执行过程中可以改变
ps = pc;
ps = pr;

pc->draw(Shape::Red)  //Circle::Draw(Shape::Red)
pr->draw(Shape::Red)  //Rectange::Draw(Shape::Red)

//因为Reactangle的draw有缺省参数值，所以这个调用正确，但是因为pr的静态值是
//Shape*,而缺省参数是静态调用的，所以这个的默认参数不是Rectangle中的Green,
//而是Shape中draw的Red,结果造成这个函数基类与继承类各出一半力，奇怪的行为
pr->draw();

//NVI手法实现缺省参数值，防止虚函数这种表现出异常的行为
class Shape
{
public:
	enum ShapeColor {Red, Green, Blue};
	//非虚调用一个虚函数完成人物，提供缺省参数
	void draw(ShapeColor color = Red) const = 0
	{
		doDraw(color);
	}
private:
	//继承类需提供实现
	virtual void doDraw(ShapeColor color) const = 0;
};

class Rectangle : public Shape
{
public:
	//这里不指定缺省参数值
	virtual void doDraw(ShapeColor color) const;
};
