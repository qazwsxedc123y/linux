#include <stdio.h>
#include <add.h> // 引用我们刚才打包的静态库

int main()
{
	int x = 20;
	int y = 10;
	int z = my_add(x, y);
	printf("%d + %d = %d\n", x, y, z);
	return 0;
}