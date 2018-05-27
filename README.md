# 互联网出行项目

数据库训练项目。

## 编译

```
mkdir build
cd build
cmake ..
make
```

## 本地测试

需要调用`create_index`预先生成一个index文件：

```
./create_index ../data ../index.data
```

然后调用main程序进行本地测试：

```
./main ../data ../data/index.data
```

正确的结果是没有输出，正常结束执行。

## 作为服务器运行

需要先生成index.data。

```
cd server
python manage.py runserver [0.0.0.0:PORT]
```

## 依赖

- METIS库，已经包含在thirdparty中。

- GPTree库，已经修改并放在了lib/GPTree.cpp中。