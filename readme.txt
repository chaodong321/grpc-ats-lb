【目录介绍】
conf：配置(暂无配置)
doc：接口说明文档
service：centos7系统服务脚本
proto：grpc接口定义
src：c++接口实现

【注意事项】
1、编译环境前需安装proto3.0版本grpc
2、若grpc安装后库路径不在/lib或/usr/lib中，则需添加共享库搜索路径(例：/usr/local/lib）

【编译】
make build：编译ralt grpc接口并生成可执行文件
make clean：清除make build生成的文件
make install-svr：编译并安装RALT的GRPC服务(安装的目录为/opt/reyzar/ralt-svr)，
make uninstall-svr：卸载RALT的GRPC服务
make install-agent：编译并安装RALT的GRPC代理(安装的目录为/opt/reyzar/ralt-agent)
make rpm-svr：编译RALT的GRPC服务并打包为rpm文件
mkae rpm-agent：编译RALT的GRPC代理并打包为RPM文件
注：可通过makefile文件查看