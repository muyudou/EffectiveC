//以独立语句将newed对象置入智能指针
//有以下函数
int priority();
void processWidget(std::tr1::shared_ptr<Widget> pw, int priority);

//以下调用形式,不会通过编译，因为tr1::shared_ptr的构造函数是explicit函数
//虽然它接受一个widget的指针，但是不能进行隐式转换，
processWidget(new Widget, priority());

//所以如果使用以下强制转化,可以通过，但这可能有隐式的资源泄漏,在new与调用shared_ptr构造函数之间
//执行priority,如果它发生异常，则资源泄漏
processWidget(std::tr1::shared_ptr<Widget>(new Widget), priority());

//解决方案：
std::tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw, priority());
