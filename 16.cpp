typedef std::string AddressLines[4];  //定义了一个AddressLines类型，执行string [4]

std::string* pal = new AddressLines; //这里分配的是数组，相当于new string[4]

delete pal;  //错误，但是可能会发生
delete [] pal; //正确的形式，但与以上new不对称
