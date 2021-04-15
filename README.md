#                         code-review
[![Build Status](https://travis-ci.com/loodao/helloworld.svg?branch=master)](https://travis-ci.com/loodao/helloworld)

<a href="https://scan.coverity.com/projects/soway-code-direct-sensor">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/20845/badge.svg"/>
</a>
<a href="https://scan.coverity.com/projects/soway-code-direct-sensor">
  <img alt="Coverity Scan Build Status"
       src="https://img.shields.io/coverity/scan/20845.svg"/>
</a>



# training
1)  所有相关与git&amp;github操作的测试在这个仓库练习

2)  相关的git & github的练习操作都可在这仓库进行；包括资料的共享等等

3)  添加了RT-Thread 代码自动审查配置,测试自己编写的rt-thread模块可以进行CI测试


_________________________________________________________________________________________________________________________________
代码合并的要求：
 
   1：项目的每个模块要有流程图及doxygen格式的注释
   
   2：每个函数需要doxygen格式的注释
   
   3：代码经过CI审核之后假如有错误（有可能服务器的设置更新），项目审核人要再次人工审核之后才允许合并
   
 _______________________________________________________________________________________________________________________________
   



##### 目录说明

​	/applications ------应用程序源代码

  /board --------板的支持包

   /documents -----文档目录

​                           ---->GitHub入门与实践

​						   ---->Github进行fork后如何与原仓库同步

​						  ----->代码持续集成，审核及测试方式

​						  ----->培训计划一

​						  -----> Doxygen注释语法

  /img-------链接图片保存

/include  --------头文件

/libraries-------库文件

/setting--------参数设置

/src-----rtthread源代码





##### 练习自己的代码在CI集成下的审查过程。PR代码的时候建议目录组成如下：

/applications  --------->放置用户源代码.c文件及.h文件  （针对用rtthread的系统开发）



