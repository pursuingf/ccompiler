# 同济大学编译原理大作业-词法分析 语法分析

### 项目结构

client  前端，经典三件套

server 后端，基于vs2022开发。用到外包库websocketpp和boost_1_81_0
### 编译运行

后端依赖于websocketpp和boost_1_81_0，在vs2022配置依赖库教程见参考资料第[7]项，然后在vs2022正确添加server/src/下的文件，编译运行即可

### 产品获取

先运行server/build/ccompiler.exe，然后双击client/的index.html文件。在前端输入界面按照提示操作即可。需要注意的是，默认开放端口是3000。若端口被占用修改服务器端的main函数里的参数和客户端client.js中的参数即可。


### 局限和不足

本项目仅仅实现部分类C语法规则，包含过程调用但不包括头文件展开

### 参考资料

[1] 陈火旺等. 程序设计语言编译原理（第 3 版）. 国防工业出版社, 2000

[2] Maoyao233. ToyCC. https://github.com/Maoyao233/ToyCC/

[3] GQT. 词法分析器. 2021

[4] Cplusplus.com. std::stringstream - sstream. 

https://cplusplus.com/reference/sstream/stringstream/

[5] 从头开始写一个类C编译器.

https://www.bilibili.com/video/BV1Jy4y1d7nq/?p=2&vd_source=35a8366f91f3ef012a5bcf44aaefceca

[6] PeachCompiler.

https://github.com/nibblebits/PeachCompiler/tree/main

[7] vs2022 安装boost库并导入websocketpp示例.

https://blog.csdn.net/qq_41250497/article/details/129796751# 同济大学编译原理大作业-词法分析 语法分析

### 项目结构

client  前端，经典三件套

server 后端，基于vs2022开发。用到外包库websocketpp和boost_1_81_0
### 编译运行

后端依赖于websocketpp和boost_1_81_0，在vs2022配置依赖库教程见参考资料第[7]项，然后在vs2022正确添加server/src/下的文件，编译运行即可

### 产品获取

先运行server/build/ccompiler.exe，然后双击client/的index.html文件。在前端输入界面按照提示操作即可。需要注意的是，默认开放端口是3000。若端口被占用修改服务器端的main函数里的参数和客户端client.js中的参数即可。


### 局限和不足

本项目仅仅实现部分类C语法规则，包含过程调用但不包括头文件展开

### 参考资料

[1] 陈火旺等. 程序设计语言编译原理（第 3 版）. 国防工业出版社, 2000

[2] Maoyao233. ToyCC. https://github.com/Maoyao233/ToyCC/

[3] GQT. 词法分析器. 2021

[4] Cplusplus.com. std::stringstream - sstream. 

https://cplusplus.com/reference/sstream/stringstream/

[5] 从头开始写一个类C编译器.

https://www.bilibili.com/video/BV1Jy4y1d7nq/?p=2&vd_source=35a8366f91f3ef012a5bcf44aaefceca

[6] PeachCompiler.

https://github.com/nibblebits/PeachCompiler/tree/main

[7] vs2022 安装boost库并导入websocketpp示例.

https://blog.csdn.net/qq_41250497/article/details/129796751
