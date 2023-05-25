
#define CHAR_CANVOS_WIDTH 160
#define CHAR_CANVOS_HEIGHT 70

enum type_backcolor {bblack=40,bred=41,bgreen=42,byellow=43,bblue=44,bpurple=45,bdarkgreen=46,bwhite=47};
enum type_fontcolor {fblack=30,fred=31,fgreen=32,fyellow=33,fblue=34,fpurple=35,fdarkgreen=36,fwhite=37};
struct charpoin{
    int x,y;
};

int scanKeyboard();
void setCursorPos(int,int);
void moveCursorPosByY(int);
void moveCursorPosByX(int);
void clearScreen();
void hideCursor();
void showCursor();
void setBackColor(type_backcolor);
void setFontColor(type_fontcolor);
void setPoint(charpoin);
void lineto(charpoin);