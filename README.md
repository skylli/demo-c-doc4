# demo-c-doc 
##远程计算器
本部分展示了借助 Wilddog C/嵌入式 SDK 通过网页输入两个数，客户端对这两个数进行相加并推送到网页端。这是一个简单的远程加法器，它的输入和输出都是网页端，而控制逻辑也是简单的相加。你可以利用 Arduino 或树莓派 (可以在快速入门了解 Wilddog C/嵌入式 SDK 支持的平台 )，把各类的传感器的输出或者网上的天气等的数据作为其输入，电机驱动或者是 LCD 驱动作为其输出，制作更有实用意义的控制器。

### 克隆代码

    git clone https://github.com/skylli/demo-c-doc4.git

### 使用
把 demo-c-doc.c 拷贝到 wilddog-client-c\examples\linux\ 下，点击此下载[wilddog-client-c SDK](https://github.com/WildDogTeam/wilddog-client-c.git)

修改 Url, 若你还没有应用的 Url 需要先到 wilddog 进行注册

 #define DEMO_YOUR_URL "coap://your_appi.wilddogio.com"   

编译：

	$ make
    $ make examples

使用计算器

    $./bin/demo-c-doc

### 更多示例

这里分类汇总了 WildDog 平台上的示例程序和开源应用，　链接地址：[https://github.com/WildDogTeam/wilddog-demos](https://github.com/WildDogTeam/wilddog-demos)

### 相关文档

* [Wilddog 概览](https://z.wilddog.com/overview/introduction)
* [c SDK快速入门](https://z.wilddog.com/c/quickstart)
* [c SDK API](https://z.wilddog.com/c/api)
* [下载页面](https://www.wilddog.com/download/)
* [Wilddog FAQ](https://z.wilddog.com/questions)

### License
MIT
http://wilddog.mit-license.org/