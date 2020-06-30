# PKGBUILD-Assistant English Doc
PKGBUILD-Assistant is an auxiliary tool for writing PKGBUILD files developed using the Qt5 framework.



## Availability

| PLATFORM       | LINK                                                         |
| -------------- | ------------------------------------------------------------ |
| AUR            | [pkgbuild-assistant](https://aur.archlinux.org/packages/pkgbuild-assistant/) |
| Github Release | [PKGBUILD-Assistant](https://github.com/skykeyjoker/PKGBUILD-Assistant/releases) |



## Features

### Basic information editing

PKGBUILD-Assistant provides basic software package information editing functions, such as package name `${pkgname}` and package release version `${pkgver}`.



### Dependencies Query

PKGBUILD-Assistant can scan the lib files in the **specified directory** and call the Shell program `pacman -Fx` to query the software packages corresponding to the lib file.



### Package() Implement

PKGBUILD-Assistant can simulate the operation of `Package()` function in **specified directory**, such as copying files, folders, deleting file folders, etc. PKGBUILD-Assistant `Package()` function implementation block **Real-time** maintains two *virtual directory tree structures* of `${srcdir}` and `${pkgdir}`. By referring to the structure of the virtual directory tree, users can write the contents of the `Package()` function more intuitively.



## Related Links:

* [Arch Wiki: PKGBUILD](https://wiki.archlinux.org/index.php/PKGBUILD)





***





# PKGBUILD-Assistant 中文文档

PKGBUILD-Assistant是一款使用Qt5框架进行开发的PKGBUILD文件编写辅助工具。



## 获取程序

| 平台           | LINK                                                         |
| -------------- | ------------------------------------------------------------ |
| AUR            | [pkgbuild-assistant](https://aur.archlinux.org/packages/pkgbuild-assistant/) |
| Github Release | [PKGBUILD-Assistant](https://github.com/skykeyjoker/PKGBUILD-Assistant/releases) |





## 程序功能 

### 基本包信息编辑 

PKGBUILD-Assistant提供了基本的软件包信息编辑功能，如软件包名称`${pkgname}`、软件包发行版本`${pkgver}`等。



### 运行依赖查询

PKGBUILD-Assistant可以通过扫描**指定目录**下的lib文件，调用Shell程序`pacman -Fx`查询lib文件对应的软件包。



### Package()函数实现

PKGBUILD-Assistant可以在**指定目录**模拟`Package()`函数操作，如拷贝文件、文件夹，删除文件文件夹等。PKGBUILD-Assistant `Package()`函数实现版块**实时**维护了`${srcdir}`与`${pkgdir}`两个*虚拟目录树结构*。通过参照虚拟目录树结构，使用者能够比较直观地编写`Package()`函数内容。



## 相关链接

* [Arch Wiki: PKGBUILD](https://wiki.archlinux.org/index.php/PKGBUILD)