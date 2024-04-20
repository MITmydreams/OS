simple_shell: myshell.c
	gcc -o simple_shell 
	myshell.c

# TODO: 参照 simple_shell，增加编译 testsh 的规则
testsh: testsh.c
	gcc -o testsh testsh.c

.PHONY: test
test: simple_shell testsh
	./testsh ./simple_shell

#TODO:添加自动化测试的lable
.PHONY : clean

clean:
	rm -f testsh simple_shell

#TODO: 参照 clean ，增加自动化测试的规则，注意自动化测试的规则是可以有依赖的
