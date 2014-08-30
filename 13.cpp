//以对象管理资源

//基类
class Investment
{...}

//程序库通过一个工厂函数产生某个特定的inverstment对象,返回指针
Investment* createInvestment(); 

//这导致调用者需要对这个返回的指针进行删除
void f()
{
	Investment* pInv = createInvestment();
	//这里中间可能过早返回，或抛出异常，则都不会执行delete，也就会发生资源泄漏
	...
	delete pInv;
}

//解决方法1：使用auto_ptr管理对象资源
void f()
{
	//这里是RAII，获得资源后立刻放进管理对象
    std::auto_ptr<Investment> pInv(createInvestment);

	//auto_ptr的复制行为
	std::auto_ptr<Investment> pInv2(pInv);  //pInv2指向对象，pInv=null
	pInv = pInv2;       //pInv指向对象，pInv2=null

}//离开函数的时候，调用auto_ptr的析构函数确保资源被释放

//解决方法2：使用tr1::shared_ptr管理对象资源
void f()
{
	//这里是RAII，获得资源后立刻放进管理对象
    std::tr1::shared_ptr<Investment> pInv(createInvestment);

	//shared_ptr的复制行为
	std::tr1::shared_ptr<Investment> pInv2(pInv);  //pInv2, pInv指向同一对象
	pInv = pInv2;       //pInv，pInv2指向同一对象

}//离开函数的时候，调用shared_ptr的析构函数确保pInv,pInv2被释放

//以下行为虽然可以通过编译器，但是会有资源泄漏，不应该使用
std::auto_ptr<std::string> aps(new std::string[10]);
std::tr1::shared_ptr<std::string> api(new int[1024]);
