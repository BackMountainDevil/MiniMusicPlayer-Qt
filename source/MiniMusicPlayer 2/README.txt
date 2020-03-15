介绍：
一款简易的绿色音乐播放器，无需安装，用完即走，不留痕迹。
使用说明：
从左至右三个按纽分别为 选择文件、播放/暂停、查看/隐藏歌单，将鼠标停留在按钮上会显示提示。
1.添加音乐文件：点击选择文件按钮进行单个或多个选择即可，也可以直接拖放文件到软件窗口中自动识别添加文件，添加完成后会自动循环播放刚才添加的音乐；
2.调节音量：将鼠标停留在播放器界面时，滚动鼠标滚轮即可调节音量，默认70%的音量。
3.暂停播放：点击暂停按钮可以暂停播放，再次点击可恢复播放
4.点击歌单按钮可以查看播放列表
5.退出：点击右上角关闭按钮（“ X ”）

资源
工程文件源码在source文件夹，可以下载用Qt打开编辑
安装包在release文件夹，采用windeployqt+Enigma Virtual Box 发布
V2.0 25.5M不能上传到Github，资源链接
https://www.jianguoyun.com/p/DSzZOZUQkLOaCBjh-_QC (访问密码：ybiV0Y)

开发环境
Qt Creator 4.11.1 + Qt 5.14.1 (MSVC 2017, 32 bit)

状态
2020.3.14 V1.0 完成
若图片加载失败，可访问我的博客查看：
https://zhuanlan.zhihu.com/p/113188097
2020.3.16 V2.0 完成
V1.0新增需求文档
1.支持播放mp3、wav格式的音频；
2.可以选择要播放的本地音频；
3.支持暂停和重新开始播放；
V2.0新增需求文档
界面ui设计
增加可隐藏的播放列表
支持拖动添加多个文件并筛选掉不符合格式的文件
选取文件对话框增加支持的格式mpga
支持一次选取多个音乐文件，播放完后自动播放下一曲
增加音量通过鼠标调节功能，默认音量70%，滚一次变化5%
V2.0已攻克bug
播放完成后按钮状态修正
从从暂停的位置恢复播放而不是从头播放
再次添加文件会从头播放修正为继续当前的播放
V3.0新增需求文档
添加到系统托盘图标，有关闭后最小化到系统托盘的选项
重置播放器功能（情况列表和歌单）
增加通过歌单切歌和按钮切歌
增加单曲循环、列表循环、随机播放
播放进度条
歌单窗口大小跟随
文件名识别更准确


协议
遵守GNU GPL v3.0协议

Introduction
An light MusicPlayer
Contact with E-mail: kearneyback@gmail.com


Status
2020.3.14 V1.0 released


Installtion
V2.0：
https://www.jianguoyun.com/p/DSzZOZUQkLOaCBjh-_QC
 (Password：ybiV0Y)    by 2021.3.16

Resources
You can get the source code from this website:


License
This package is released under GNU General Public License v3.0(https://opensource.org/licenses/LGPL-3.0)

