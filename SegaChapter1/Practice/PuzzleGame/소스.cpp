#include <iostream>
#include <fstream>

using namespace std;

// # 벽, _ 공간, . 목표지점, o 상자, p 사람
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

//함수 프로토타입
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main()
{
    /*
    ifstream inputFile("stageData.txt", ifstream::binary);
    inputFile.seekg(0, ifstream::end);
    //위치 = 파일 크기
    int fileSize = static_cast<int>(inputFile.tellg());
    inputFile.seekg(0, ifstream::beg);      //맨처음으로 이동
    char* fileImage = new char[fileSize];   //영역 확보
    inputFile.read(fileImage, fileSize);    //읽기

    cout.write(fileImage, fileSize);        //제대로 읽었는지 표시한다.
    */

    //상태 배열 확보
    Object* state = new Object[gStageWidth * gStageHeight];
    //스테이지 초기화
    initialize(state, gStageWidth, gStageHeight, gStageData);
    //메인루프
    while (true)
    {
        draw(state, gStageWidth, gStageHeight);
        if (checkClear(state, gStageWidth, gStageHeight))
        {
            break;
        }
        //입력값 취득
        cout << "a:left d:right w:up s:down. command?" << endl; //조작설명
        char input;
        cin >> input;
        //갱신
        update(state, input, gStageWidth, gStageHeight);
    }
    //축하 메시지
    cout << "Congratulation's! you win." << endl;
    //뒷정리
    delete[] state; //배열을 new로 생성했으므로 delete가 아니라 delete[]
    state = 0;      //내 규칙

    return 0;
}

void initialize(Object* state, int width, int height, const char* stageData)
{
    const char* d = stageData; //읽기용 포인터
    int x = 0;
    int y = 0;
    while (*d != '\0') //NULL 문자가 아닌 동안
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
            case '\n': //다음 행으로
                x = 0; //x를 왼쪽 끝으로 되돌리기
                ++y;   //y는 한 단계 아래로
                t = OBJ_UNKNOWN; //데이터는 없음
                break;
            default: t = OBJ_UNKNOWN; break; //부정 데이터
        }
        ++d;
        //이 if 문은 모르는 문자면 무시한다.
        if (t != OBJ_UNKNOWN)
        {
            state[y * width + x] = t; //쓰기
            ++x;
        }
    }
}

void draw(const Object* state, int width, int height)
{
    //Object 열거형 순서
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

//첫 번째 인수는 다른 함수에서는 state라고 하지만 자주 사용하므로 짧게 s로 끝낸다.
//w, h도 각각 width, height다.
void update(Object* s, char input, int w, int h)
{
    //이동량으로 변환
    int dx = 0;
    int dy = 0;
    switch (input)
    {
    case 'a': dx = -1; break; //왼쪽
    case 'd': dx = 1; break; //오른쪽
    case 'w': dy = -1; break; //위(Y는 아래가 플러스)
    case 's': dy = 1; break; //아래
    }

    //플레이어 좌표 검색
    int i = -1; //의미는 없지만 뭐가 초기화 하고싶다;ㅋㅋㅋ
    for (i = 0; i < w * h; ++i)
    {
        if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
        {
            break;
        }
    }
    int x = i % w; //x는 폭으로 나눈 나머지
    int y = i / w; //y는 폭으로 나눈 몫

    //이동 후 좌표
    int tx = x + dx;
    int ty = y + dy;
    //좌표에서 최대최소 검사. 벗어나면 허가하지 않는다.
    if (tx < 0 || ty < 0 || tx >= w || ty >= h)
    {
        return;
    }

    // A. 이동할 곳이 빈칸 또는 목적지 ->플레이어 이동
    int p = y * w + x;      //플레이어 현재 위치
    int tp = ty * w + tx;   //목표 위치
    if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
    {
        //이동할 곳이 목적지라면 목적지 위 사람, 아니면 그냥 사람
        s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        //현재 플레이어의 위치가 목적지 위라면 목적지로 아니면 공백으로
        s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
    // B. 이동할 곳이 상자 -> 그 방향 다음 칸이 공백이나 목적지면 이동
    else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
    {
        //2칸 앞이 범위 내인지 검사
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
        {
            return;//밀 수 없다.
        }
        int tp2 = (ty + dy) * w + (tx + dx); //2칸 앞
        if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
        {
            //순차적으로 교체
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