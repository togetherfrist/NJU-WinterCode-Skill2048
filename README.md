# 南京大学 Winter Code 2026 基础赛道项目部分留档

这次 winter code 的内容是写一个 2048 或者复刻一个广告游戏。

于是写了十几天的 2048 。

## 学习笔记

### cpp 继承

[C++ 继承 | 菜鸟教程](https://www.runoob.com/cplusplus/cpp-inheritance.html)

概括：

语法为 class 派生类名: 访问修饰符 基类名

访问修饰符一般写 `public` ，此时派生类继承基类的公有成员和保护成员且访问类型不变，派生类不能直接访问基类的私有成员。

一个子类可以有多个父类。

如果你想重写基类的函数，请使用虚函数或纯虚函数 [C++ 虚函数和纯虚函数的区别 | 菜鸟教程](https://www.runoob.com/w3cnote/cpp-virtual-functions.html)

含有纯虚函数的类被称为抽象类，如果想储存多个抽象类类型的对象，可以使用智能指针。

### SFML 库
这个项目中使用 SFML 库实现了 GUI 。SFML 比较轻量，可以很快看完文档，但是大部分内容需要自己实现。

### vscode 配置

vscode 使用第三方库时需要将路径加进 includePath 来消除报错以及获得补全。

（更新中...