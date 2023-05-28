#include "kernel.h"
//因为我们的操作系统没有TTY IO，所以我们需要重新写一个printf函数
extern "C" void  printf(char *str){
	u_short *monitor_io_memory=(u_short *)0xb8000;//注意！重点来啦！0xb8000内存地址是显示器地址，往这里写数据就直接能够输出到屏幕上
	for(int i=0;str[i]!='';++i){
		//写入字符串，取或0xff00的意思是我们需要把屏幕高四位拉低，否则就是黑色的字体，黑色的字体黑色的屏幕是啥也看不到的
		monitor_io_memory[i]=(monitor_io_memory[i] & 0xff00) | str[i];
	}
}
//操作系统构造函数委托方法
typedef void(*constructor)();
//全局定义构造委托
constructor start_ctors;
//全局定义析构委托
constructor end_ctors;

//轮询函数，并且执行
extern "C" void system_constructors(){
	for(constructor* i=&start_ctors;i!=&end_ctors;i+=1){
		(*i)();
	}
}
//操作系统主启动函数，这里我们打印一个字符串然后让操作系统进入等待
extern "C" void  kernel_main(const void *multiboot_structure,u_int magicnumber){
	printf("Hello MoniOS");
	while(1);
}
