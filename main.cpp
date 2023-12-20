#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <math.h>
#include <codecvt>
#include <locale>

using namespace std;
using namespace sf;

class MultiEnString{
public:
    enum EncType{
        GBK,Utf8,Utf16,Unicode
    };
    MultiEnString(string str,EncType);
    MultiEnString(wstring str,EncType);
    string GetUtf8();
    string GetGBK();
    wstring GetUTF16();
    wstring GetUnicode();

    string utf8InnerData;
};

string ANSIToUTF8(string in){
    //��������ֵ����ȡ�䳤��
    string buildRet = "";
    char * input_string=(char *)in.c_str();
    int in_size= strlen(input_string);


    /*ӳ��һ���ַ�����һ�����ַ���unicode�����ַ������ɸú���ӳ����ַ���û��Ҫ�Ƕ��ֽ��ַ��顣
       CP_ACP��ANSI����ҳ����������Windows����ϵͳ�У�ANSI ������� GBK ���룩*/
    //�Ȼ�ȡ���ַ������Ȳ�����������ʵ��ִֵ�к���
    int wide_size=MultiByteToWideChar(CP_ACP, 0, input_string, in_size, NULL, 0);
    wchar_t * wide_string = (wchar_t * ) malloc(wide_size*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, input_string, in_size, wide_string, wide_size);


    /*�ѿ��ַ���ת����ָ�����µ��ַ�������ANSI��UTF8�ȣ����ַ��������Ƕ��ֽ��ַ�����
       CP_UTF8��ʹ��UTF-8ת��*/
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, NULL, 0, NULL, NULL);
    char *utf8_string = (char * ) malloc(utf8_size);
    WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, utf8_string, utf8_size, NULL, NULL);
    free(wide_string);

    buildRet = string(utf8_string);
    free(utf8_string);
    return buildRet;
}

MultiEnString::MultiEnString(string str,EncType e){
    if(e == GBK){
        utf8InnerData = "";
    }else if(e == Utf8)utf8InnerData = str;
}

MultiEnString::MultiEnString(wstring str,EncType e){
    if(e == Unicode){
        ///TODO����Ҫ���� Required to do it well
        utf8InnerData = std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(str);
    }else if(e == Utf16){
        utf8InnerData = std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(str);
    }
}

string MultiEnString::GetUtf8(){
    return utf8InnerData;
}

string MultiEnString::GetGBK(){
    return "";}

wstring MultiEnString::GetUTF16(){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(utf8InnerData);
    return wide_string;
}

wstring MultiEnString::GetUnicode(){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(utf8InnerData);
    return wide_string;
}

HWND GetBackground() {
    //��������û�д�����������֪������������workerW�����и�����Program Maneger������ֻҪ
    //��������workW���͵Ĵ��壬��һ�Ƚ����ĸ������ǲ���Program Manager�Ϳ����ҵ���������
    HWND hwnd = FindWindowA("progman", "Program Manager");
    HWND worker = NULL;
    do {
        worker = FindWindowExA(NULL, worker, "workerW", NULL);
        if (worker != NULL) {
            char buff[200] = {0};
            int ret = GetClassNameA(worker, (PCHAR) buff, sizeof(buff) * 2);
            if (ret == 0) {
                return NULL;
            }
        }
        if (GetParent(worker) == hwnd) {
            return worker;//���ؽ��
        }
    } while (worker != NULL);
    //û���ҵ�
    //������Ϣ����һ��WorkerW����
    SendMessage(hwnd,0x052C,0,0);
    //�ظ����沽��
    do {
        worker = FindWindowExA(NULL, worker, "workerW", NULL);
        if (worker != NULL) {
            char buff[200] = {0};
            int ret = GetClassNameA(worker, (PCHAR) buff, sizeof(buff) * 2);
            if (ret == 0) {
                return NULL;
            }
        }
        if (GetParent(worker) == hwnd) {
            return worker;//���ؽ��
        }
    } while (worker != NULL);
    return NULL;
}

#define PI 3.14159265
#define deg2rad(deg) (deg)*PI/180
#define rad2deg(rad) (rad)*180/PI

Vector2f RotateAround(float ang,float len,Vector2f center){
    float nx = -sin(ang);
    float ny = cos(ang);
    return Vector2f(nx*len,ny*len) + center;
}
Vector2f RotateAround(float ang,Vector2f ori,Vector2f center){
    float x = ori.x - center.x;
    float y = ori.y - center.y;
    float len = sqrt(x*x + y*y);
    return RotateAround(ang,len,center);
}


vector<wstring> motd(ifstream &io,int &size){
    if(!io.good())return {L""};
    io >> size;
    string s0;
    vector<wstring> ss;
    while(!io.eof()){getline(io,s0);ss.push_back(MultiEnString(s0,MultiEnString::Utf8).GetUTF16());}
    return ss;
}

Color ColorMoreXX(Color cl,float mul){
    return Color(cl.r * mul,cl.g * mul,cl.b * mul);
}

int main()
{
    vector<string> kebiao;
    vector<wstring> mtd;
    int idx = 0;
    ifstream kb("kebiao.txt");
    if(!kb.good())return -1;
    string str = "";
    while(!kb.eof()){
            string s = "";
            str = "";
            getline(kb,s);
            str += s + ":";
        for(int i =0;i < 16;++i){
            s = "";
            getline(kb,s);
            str += s + " ";
        }
    cout << str << endl;
        kebiao.push_back(str);
    }
    kb.close();
    SYSTEMTIME st;
    Font ft;
    if(!ft.loadFromFile("f.ttf"))return -1;
    Texture yhy,bgt;
    if(!bgt.loadFromFile("bg.png"))return -1;
    if(!yhy.loadFromFile("yhy.png"))return -1;
    Sprite sun(yhy),bgs(bgt);
    HWND bg = GetBackground();
    RECT r;
    GetWindowRect(bg,&r);
    int cx = r.right - r.left,cy = r.bottom - r.top;
    cx*=3;cy*=3;
    float sx = (float)cx/bgt.getSize().x,sy = (float)cy/bgt.getSize().y;
    bgs.setScale({sx>sy?sx:sy,sx>sy?sx:sy});
    sun.setScale({320.0f/yhy.getSize().x,320.f/yhy.getSize().y});
    RenderWindow window(sf::VideoMode(cx,cy),"",Style::None);
    HWND hWnd = window.getSystemHandle();
    SetParent(hWnd,bg);
    //window.setFramerateLimit(30);
    float rTheta = 120;
    int cycle = 1900,cycle2 = 300;
    Text t("",ft,138);
    t.setOutlineColor(Color::Black);
    t.setFillColor(Color::White);
    t.setOutlineThickness(12);
Text t2(MultiEnString("",MultiEnString::Utf8).GetUTF16(),ft,258);
    t2.setOutlineColor(Color::Black);
    t2.setFillColor(Color::Red);
    t2.setOutlineThickness(12);

    while (window.isOpen())
    {
        rTheta += 0.05;
        if(rTheta>=360)rTheta=0;
        float dec = sin(deg2rad(rTheta / 2));
        //Processing events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed){
                window.close();
                ///Do Nothing
            }
        }
        ++cycle;
        ++cycle2;
        bgs.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
        sun.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
        window.draw(bgs);
        //FloatRect fr = sun.getGlobalBounds();
        //sun.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
        //sun.setPosition(100,100);
        window.draw(sun);sun.setPosition(RotateAround(deg2rad(rTheta),{cx/2+240.0f,0.0f},Vector2f(cx/2,cy)));
        if(cycle2 >= 30*5 && mtd.size()>0){
                cycle2 = 0;
            ++idx;
            if(idx>=(int)mtd.size())idx=0;
            t2.setString(mtd[idx]);
        }
        if(cycle >= 30*60){
                cycle = 0;
            GetLocalTime(&st);t.setString(MultiEnString(kebiao[st.wDayOfWeek]+"\n"+kebiao[(st.wDayOfWeek+1)>6?0:st.wDayOfWeek+1],MultiEnString::Utf8).GetUTF16());

            FloatRect fr = t.getGlobalBounds();
            t.setPosition((cx -fr.width)/2,10);
            kb.open("motd.txt");
            int sz;
            mtd = motd(kb,sz);
            t2.setCharacterSize(sz);
            kb.close();
            }
        window.draw(t);
        FloatRect fr = t2.getGlobalBounds();
        t2.setPosition((cx - fr.width)/2,cy/2 - fr.height/2);
        window.draw(t2);
        window.display();
        Sleep(40);
    }
    return 0;
}
