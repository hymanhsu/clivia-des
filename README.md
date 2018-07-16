Encrypt & Decrypt based on DES， including JNI and Python extension


Preparations For CentOS7.4

---INSTALL Python3
yum install epel-release
yum install https://centos7.iuscommunity.org/ius-release.rpm
yum install python36u
yum install -y python36u-devel
ln -s /bin/python3.6 /bin/python3
yum install python36u-pip
ln -s /bin/pip3.6 /bin/pip3

---INSTALL JAVA8 and Cmake3
yum groupinstall "Development Tools"
yum -y install java-1.8.0-devel
yum -y install cmake3 openssl-devel
ln -s /usr/bin/cmake3 /usr/bin/cmake

