# CandyCountry
 【Cocos2dx】自制三消游戏-糖果国度

### 项目说明
&emsp;&emsp;此项目是使用Cocos2dx开发的三消游戏，也是我的毕业设计。游戏功能相当完善，包括经典的限时消除玩法和闯关玩法，还加上了一种自己设计的一个游戏模式。具体玩法可以自行尝试，或者看下文档说明，文档很详细，毕竟是毕业设计的文档。  
&emsp;&emsp;其实玩法主要参考《天天爱消除》，而游戏风格则直接照搬《Candy Crush saga（糖果粉碎传奇）》。  
&emsp;&emsp;游戏的界面设置纯靠手敲（当时为了赶时间，没去学习使用CocosStudio进行界面设计），代码的话，还是蛮多的，具体设计还是参考文档，要提一下的是有些名称后缀不同的代码文件的含义：xx_C.cpp代表是经典模式（限时模式）专用代码（Classic），xx_T代表是闯关模式（Tollgate）专用代码，xx_L代表是清除模式（Level）专用代码，别问我为什么这么设计，这么命名...我也不懂当时的想法- -!  
&emsp;&emsp;关卡配置的话，用的是Json文件进行配置，具体直接看文件和文档。  
&emsp;&emsp;另外：可能存在bug，之前玩的时候有时会闪退，后面也没去找问题所在了。（毕竟毕设已经通过了...）

### 开发环境和配置
1. 操作系统：Win7 64位  
2. 开发平台：Microsoft Visual Studio 2013  
3. 游戏引擎：Cocos2d-x-3.0  
4. 其他环境：JDK 1.7.0_79、Python 2.7.6、Android-ndk-r9b、Android-sdk-windows、Apache-ant-1.9.2

### 目录说明
Assets/Classes：存放游戏代码文件。  
Assets/Resources：存放游戏图片和音频资源。  
Document：存放项目文档和打包的安卓APK文件。  

### 运行示例
[运行示例](https://img.wenhairu.com/image/NxZRK)  
![](https://cdn.img.wenhairu.com/images/2020/04/24/NxZRK.th.png)
