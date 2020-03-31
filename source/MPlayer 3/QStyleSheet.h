#ifndef QSTYLESHEET_H
#define QSTYLESHEET_H
#include<QString>
/*
 * 存储样式表。后期可以放置不同的样式置换主题
*/
inline QString initQStyle()
{
    return (

                //默认按钮样式
            "QToolButton{"
               "background-color:transparent"  //透明背景
//               "background-color:rgba(100,225,100,30);"//背景色（也可以设置图片）
//               "border-style:outset;"                  //边框样式（inset/outset）
//               "border-width:4px;"                     //边框宽度像素
//               "border-radius:10px;"                   //边框圆角半径像素
//               "border-color:rgba(255,255,255,30);"    //边框颜色
//               "font:bold 10px;"                       //字体，字体大小
//               "color:rgba(0,0,0,100);"                //字体颜色
//               "padding:6px;"                          //填衬
//               "}"
//               //鼠标按下样式
//            "QToolButton:pressed{"
//               "background-color:rgba(100,255,100,200);"
//               "border-color:rgba(255,255,255,30);"
//               "border-style:inset;"
//               "color:rgba(0,0,0,100);"
//               "}"
//               //鼠标悬停样式
//            "QToolButton:hover{"
//               "background-color:rgba(100,255,100,100);"
//               "border-color:rgba(255,255,255,200);"
//               "color:rgba(0,0,0,200);"
               "}"

            "QListWidget{"
               "background-color:white;"
                "border-style:outset;"                  //边框样式（inset/outset）
                "border-width:10px;"                     //边框宽度像素
                "border-radius:10px;"                   //边框圆角半径像素
                "border-color:rgba(255,255,255,30);"    //边框颜色
                "}"

             "QScrollBar:vertical{" //垂直滚动条
                "width:8px;"
                "background-color:transparent;"
                "margin:0px,0px,0px,0px;"
                "}"
            "QScrollBar:horizontal{" //水平滚动条
               "height:8px;"
               "background-color:transparent;"
               "margin:0px,0px,0px,0px;"

               "}"
            //进度条，零状态不和谐。。
            "QSlider::add-page:Horizontal{" //底部样式
                "background-color: rgba(245, 255, 255,150);"
                "height:4px; "
                "} "
            "QSlider::sub-page:Horizontal{  "  //滚动覆盖样式
                "background-color: rgb(255, 0, 0);"
                "height:4px;"
                "}"
            "QSlider::groove:Horizontal{ "
                "background:transparent; "
                "height:4px;"   //高度
                "}  "
            "QSlider::handle:Horizontal{ "
                "height:16px; "
                "width:16px; "
                "border-image:url(:/image/image/sliderHandle.png);"//小红点
                "margin: -6 0px; "
                "}"


            );


}
#endif // QSTYLESHEET_H
