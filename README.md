#### 介绍

apollo cyber record读写库，提供一个轻量级库读写百度apollo无人车的record文件，不依赖完整的apollo框架


#### 安装依赖

```
apt-get install protobuf-compiler libprotobuf-dev python3-protobuf libgoogle-glog-dev libprotoc-dev
```

#### 安装 (Python)

```
python3 setup.py install
```

#### 编译JNI库 (Java)

```
export JAVA_HOME=<JDK所在路径>
mkdir build
cd build
cmake .. -DJAVA=1
make
```
