��Ŀ¼���ܡ�
conf������(��������)
doc���ӿ�˵���ĵ�
service��centos7ϵͳ����ű�
proto��grpc�ӿڶ���
src��c++�ӿ�ʵ��

��ע�����
1�����뻷��ǰ�谲װproto3.0�汾grpc
2����grpc��װ���·������/lib��/usr/lib�У�������ӹ��������·��(����/usr/local/lib��

�����롿
make build������ralt grpc�ӿڲ����ɿ�ִ���ļ�
make clean�����make build���ɵ��ļ�
make install-svr�����벢��װRALT��GRPC����(��װ��Ŀ¼Ϊ/opt/reyzar/ralt-svr)��
make uninstall-svr��ж��RALT��GRPC����
make install-agent�����벢��װRALT��GRPC����(��װ��Ŀ¼Ϊ/opt/reyzar/ralt-agent)
make rpm-svr������RALT��GRPC���񲢴��Ϊrpm�ļ�
mkae rpm-agent������RALT��GRPC�������ΪRPM�ļ�
ע����ͨ��makefile�ļ��鿴