#include <termios.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include "header/chargraphic.h"

using namespace std;

charpoin currentpoint;
/**
 * @brief 无需回车，返回键盘按键的ASCII码
 *
 * @return int
 */
int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0, &stored_settings);    //读取终端配置参数
    new_settings = stored_settings;    //复制配置参数
    new_settings.c_lflag &= (~ICANON); //取消标准输入方式（无回车即返回)
    new_settings.c_lflag &= (~ECHO);   //取消立即显示
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    in = getchar();

    tcsetattr(0, TCSANOW, &stored_settings); //恢复原始设置
    return in;
}

/**
 * @brief 配置终端光标显示位置
 *
 * @param x
 * @param y
 */
void setCursorPos(int x, int y)
{
    string x0, y0, s;
    char *lstr = (char *)calloc(100, 0);
    x0 = to_string(x);
    y0 = to_string(y);
    s = "\033[" + y0 + ";" + x0 + "H";
    strcpy(lstr, s.c_str());
    printf(lstr, 0);
    free(lstr);
}
/**
 * @brief 沿Y方向移动光标指定行数
 *
 * @param y
 */
void moveCursorPosByY(int y)
{
    if (y > 0)
        printf(("\033[" + to_string(y) + "B").c_str(), 0);
    else
        printf(("\033[" + to_string(abs(y)) + "A").c_str(), 0);
}
/**
 * @brief 沿X方向移动光标指定行数
 *
 * @param x
 */
void moveCursorPosByX(int x)
{
    if (x > 0)
        printf(("\033[" + to_string(x) + "C").c_str(), 0);
    else
        printf(("\033[" + to_string(abs(x)) + "D").c_str(), 0);
}
/**
 * @brief 清除屏幕并复位光标
 *
 */
void clearScreen()
{
    charpoin p;
    p.x = p.y = 0;
    printf("\033[2J");
    setPoint(p);
}
/**
 * @brief 显示光标
 *
 */
void showCursor()
{
    printf("\033[?25h");
}
/**
 * @brief 隐藏光标
 *
 */
void hideCursor()
{
    printf("\033[?25l");
}
/**
 * @brief 配置终端背景色
 *
 * @param color
 */
void setBackColor(type_backcolor color)
{
    int col = color;
    printf(("\033[" + to_string(abs(col)) + "m").c_str(), 0);
}
/**
 * @brief Set the Font Color object
 *
 * @param color
 */
void setFontColor(type_fontcolor color)
{
    int col = color;
    string s1 = "\033[" + to_string(col) + "m";
    printf("%s", s1.c_str());
}
/**
 * @brief Set the Point object
 *
 * @param p
 */
void setPoint(charpoin p)
{
    if (p.x >= CHAR_CANVOS_WIDTH || p.y >= CHAR_CANVOS_HEIGHT)
        return;
    setCursorPos(p.x, p.y);
    currentpoint = p;
}
/**
 * @brief 从所在点绘制直线到目标点
 *
 * @param p
 */
void lineto(charpoin p)
{
    charpoin startp, endp;
    double K, C;
    if ((p.x == currentpoint.x && p.y == currentpoint.y) || p.x >= CHAR_CANVOS_WIDTH || p.y >= CHAR_CANVOS_HEIGHT)
        return;
    startp = p;
    endp = currentpoint;
    if (p.x == currentpoint.x)
    { //垂线
        if (p.y > currentpoint.y)
        {
            startp = currentpoint;
            endp = p;
        }
        for (int i = startp.y; i <= endp.y; i++)
        {
            setCursorPos(startp.x, i);
            printf("X");
        }
    }
    else
    {
        double bendix = double(p.x - currentpoint.x);
        double bendiy = double(p.y - currentpoint.y);
        //以x轴为自变量确定斜率
        K = bendiy / bendix;
        //确定常量
        C = (double)p.y - K * (double)p.x;
        if (p.x > currentpoint.x)
        {
            startp = currentpoint;
            endp = p;
        }
        for (int i = startp.x; i <= endp.x; i++)
        {
            int tempy = round(K * i + C);
            if (tempy > endp.y)
                tempy = endp.y;
            setCursorPos(i, tempy);
            printf("X");
        }
    }
}

/* MAC & Linux 系统使用终端命令直接控制控制台特性
ANSI控制码:

  \033[0m 关闭所有属性
  \033[1m 设置高亮度
  \03[4m 下划线
  \033[5m 闪烁
  \033[7m 反显
  \033[8m 消隐
  \033[30m  --  \033[37m 设置前景色
  \033[40m  --  \033[47m 设置背景色
  \033[nA 光标上移n行
  \03[nB 光标下移n行
  \033[nC 光标右移n行
  \033[nD 光标左移n行
  \033[y;xH设置光标位置
  \033[2J 清屏
  \033[K 清除从光标到行尾的内容
  \033[s 保存光标位置
  \033[u 恢复光标位置
  \033[?25l 隐藏光标
  \33[?25h 显示光标

设置前景背景色的控制码中中间的数字代表不同的颜色

字体颜色设置对应的具体值:

背景色范围: 40--49                 文字颜色: 30--39

        40:黑                                      30:黑

        41:红                                      31:红

        42:绿                                      32:绿

        43:黄                                      33:黄

        44:蓝                                      34:蓝

        45:紫                                      35:紫

        46:深绿                                  36:深绿

        47:白色                                  37:白色
————————————————
版权声明：本文为CSDN博主「zhou-restart」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/zhou_1999/article/details/81173261*/
