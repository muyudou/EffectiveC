//定制型operator new

static const int signature = 0xDEADBEEF;
typedef unsigned char Byte;

void* operator new(std::size_t size) throw(std::bad_alloc)
{
	using namespace std;
	size_t realSize = size + 2 * sizeof(int);  //增加大小，使其可以塞入两个signatures

	void* pMem = malloc(realSize);
	if (!pMem)
		throw bad_alloc();

	//将signature写入内存的最前段落与最后段落
	*(static_cast<int*>(pMem)) = signature;
	*(reinterpret_cast<int*>(static_cast<Byte*>(pMem) + realSize - sizeof(int))) = signature;

	//返回指针，指向位于第一个signature之后的内存位置
	return static_cast<Byte*>(pMem) + sizeof(int);
}
