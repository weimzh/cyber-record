#### Intro

Lightweight library for reading and writing .record files used by Baidu's Apollo Cyber Framework, for Python and Java. 


#### Dependencies

```
apt-get install protobuf-compiler libprotobuf-dev python3-protobuf libgoogle-glog-dev libprotoc-dev
```

#### Installation (Python)

```
python3 setup.py install
```

#### Build JNI Library (Java)

```
export JAVA_HOME=<JDK Path>
mkdir build
cd build
cmake .. -DJAVA=1
make
```
