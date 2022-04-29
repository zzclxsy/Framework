# Application
 Linux开发框架
 
 ## 注意事项
  * 需要自行添加boost库，库头文件路径设置在Framework.pro文件里面
  * XApi，定义了外界调用的抽象接口
  * 任务模块方面，需要继承VXModuleTest类，完成虚函数才能得到框架发送过来的任务
