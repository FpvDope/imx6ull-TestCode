01-helloworld
	第一个测试程序
	
02-makefile-test001
	基本规则：
	 目标:依赖
	 tab 命令
	 
03-makefile-test002
	%  通配符
	$@ 表示目标文件
	$^ 表示所有的依赖文件
	$< 表示第一个依赖文件
	
04-makefile-test003
	假想目标的使用
		|
		|---使用Makefile，make后面不加参数，默认处理第一个目标
		|---使用Makefile，make后面加参数，处理参数指定的目标，例如：make clean
					|
					|-但如果当前存在名为clean的文件(>clean)，make clean会异常，这时就需要使用.PHONY:clean指定clean为假想目标