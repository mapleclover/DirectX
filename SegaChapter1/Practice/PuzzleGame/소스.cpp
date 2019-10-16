#include <iostream>
#include <fstream>

using namespace std;

// # ��, _ ����, . ��ǥ����, o ����, p ���
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object
{
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,

    OBJ_UNKNOWN,
};

//�Լ� ������Ÿ��
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main()
{
    /*
    ifstream inputFile("stageData.txt", ifstream::binary);
    inputFile.seekg(0, ifstream::end);
    //��ġ = ���� ũ��
    int fileSize = static_cast<int>(inputFile.tellg());
    inputFile.seekg(0, ifstream::beg);      //��ó������ �̵�
    char* fileImage = new char[fileSize];   //���� Ȯ��
    inputFile.read(fileImage, fileSize);    //�б�

    cout.write(fileImage, fileSize);        //����� �о����� ǥ���Ѵ�.
    */

    //���� �迭 Ȯ��
    Object* state = new Object[gStageWidth * gStageHeight];
    //�������� �ʱ�ȭ
    initialize(state, gStageWidth, gStageHeight, gStageData);
    //���η���
    while (true)
    {
        draw(state, gStageWidth, gStageHeight);
        if (checkClear(state, gStageWidth, gStageHeight))
        {
            break;
        }
        //�Է°� ���
        cout << "a:left d:right w:up s:down. command?" << endl; //���ۼ���
        char input;
        cin >> input;
        //����
        update(state, input, gStageWidth, gStageHeight);
    }
    //���� �޽���
    cout << "Congratulation's! you win." << endl;
    //������
    delete[] state; //�迭�� new�� ���������Ƿ� delete�� �ƴ϶� delete[]
    state = 0;      //�� ��Ģ

    return 0;
}

void initialize(Object* state, int width, int height, const char* stageData)
{
    const char* d = stageData; //�б�� ������
    int x = 0;
    int y = 0;
    while (*d != '\0') //NULL ���ڰ� �ƴ� ����
    {
        Object t;
        switch (*d)
        {
            case '#': t = OBJ_WALL; break;
            case ' ': t = OBJ_SPACE; break;
            case 'o': t = OBJ_BLOCK; break;
            case 'O': t = OBJ_BLOCK_ON_GOAL; break;
            case '.': t = OBJ_GOAL; break;
            case 'p': t = OBJ_MAN; break;
            case 'P': t = OBJ_MAN_ON_GOAL; break;
            case '\n': //���� ������
                x = 0; //x�� ���� ������ �ǵ�����
                ++y;   //y�� �� �ܰ� �Ʒ���
                t = OBJ_UNKNOWN; //�����ʹ� ����
                break;
            default: t = OBJ_UNKNOWN; break; //���� ������
        }
        ++d;
        //�� if ���� �𸣴� ���ڸ� �����Ѵ�.
        if (t != OBJ_UNKNOWN)
        {
            state[y * width + x] = t; //����
            ++x;
        }
    }
}

void draw(const Object* state, int width, int height)
{
    //Object ������ ����
    const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Object o = state[y * width + x];
            cout << font[o];
        }
        cout << endl;
    }
}

//ù ��° �μ��� �ٸ� �Լ������� state��� ������ ���� ����ϹǷ� ª�� s�� ������.
//w, h�� ���� width, height��.
void update(Object* s, char input, int w, int h)
{
    //�̵������� ��ȯ
    int dx = 0;
    int dy = 0;
    switch (input)
    {
    case 'a': dx = -1; break; //����
    case 'd': dx = 1; break; //������
    case 'w': dy = -1; break; //��(Y�� �Ʒ��� �÷���)
    case 's': dy = 1; break; //�Ʒ�
    }

    //�÷��̾� ��ǥ �˻�
    int i = -1; //�ǹ̴� ������ ���� �ʱ�ȭ �ϰ�ʹ�;������
    for (i = 0; i < w * h; ++i)
    {
        if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
        {
            break;
        }
    }
    int x = i % w; //x�� ������ ���� ������
    int y = i / w; //y�� ������ ���� ��

    //�̵� �� ��ǥ
    int tx = x + dx;
    int ty = y + dy;
    //��ǥ���� �ִ��ּ� �˻�. ����� �㰡���� �ʴ´�.
    if (tx < 0 || ty < 0 || tx >= w || ty >= h)
    {
        return;
    }

    // A. �̵��� ���� ��ĭ �Ǵ� ������ ->�÷��̾� �̵�
    int p = y * w + x;      //�÷��̾� ���� ��ġ
    int tp = ty * w + tx;   //��ǥ ��ġ
    if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
    {
        //�̵��� ���� ��������� ������ �� ���, �ƴϸ� �׳� ���
        s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        //���� �÷��̾��� ��ġ�� ������ ����� �������� �ƴϸ� ��������
        s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
    // B. �̵��� ���� ���� -> �� ���� ���� ĭ�� �����̳� �������� �̵�
    else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
    {
        //2ĭ ���� ���� ������ �˻�
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
        {
            return;//�� �� ����.
        }
        int tp2 = (ty + dy) * w + (tx + dx); //2ĭ ��
        if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
        {
            //���������� ��ü
            s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

bool checkClear(const Object* s, int width, int height)
{
    for (int i = 0; i < width * height; ++i)
    {
        if (s[i] == OBJ_BLOCK)
        {
            return false;
        }
    }
    return true;
}