调试运行时，如果出现发送数据长度为0的情况，可能是dbc.ini文件读取失败导致，解决方法如下：

1. 调试运行ZlgCan_Demo.vi前，将bin\dbc.ini文件先拷贝至LabView安装目录，如 D:\Program Files (x86)\National Instruments\LabVIEW 2017。

2. 修改刚才拷贝后的dbc.ini，根据当前使用的是ControlCAN或者ZLGCAN库，修改配置文件参数为vcican或者zlgcan。