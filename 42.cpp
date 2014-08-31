//typename的双重意义

//嵌套从属类型
template <typename C>
print2nd(const C& container)
{
	//以下是一个嵌套从属名称，因为C是模板参数，并且const_iterator被嵌套于
	//这个C中，所以下面需要使用typename,表明它是一个类型
	typename C::const_iterator* x;
}

//typename只被用来验明嵌套从属类型名称，其他名称不该有它存在
template <typename C>   //允许使用typename与class,此中情况下等同
void f(const C& container,  //正常类型，不允许使用typename
		typename C::iterator iter);  //嵌套类型，需要使用typename

//特殊情况，typename不可以出现在基类列表内的嵌套从属类型名称之前，也不可在
//成员初始化列表中作为base class修饰符
template <typename T>
class Derived : public Base<T>::Nested //基类列表中不允许
{
public:
	explicit Derived(int x)
		: Base<T>::Nested(x)   //成员初始化列表中不允许
	{
		typename Base<T>::Nested temp;  //嵌套从属类型加typename
	}
};

//STL中typename
template <typename IterT>
void workWithIterator(IterT iter)
{
	//为嵌套从属类型使用一个类型声明符,value_type就代表这个类型
	typedef typename std::iterator_traits<IterT>::value_type value_type;
	value_type temp(*iter);
}
