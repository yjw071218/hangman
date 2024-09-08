#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <chrono>
#include <thread>

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;

// 타이머 관련 상수
const int TIMER_DURATION = 30;
const int WARNING_THRESHOLD = 10;

// 난이도별 색상 정의
enum DifficultyColor {
    EASY_COLOR = 10,    // 초록색
    MEDIUM_COLOR = 14,  // 노란색
    HARD_COLOR = 12,    // 빨간색
    EXTREME_COLOR = 13  // 보라색
};

// 한영 전환 키 정의
#define TOGGLE_HANGUL_KEY \
    keybd_event(VK_HANGUL, 0, 0, 0); \
    keybd_event(VK_HANGUL, 0, KEYEVENTF_KEYUP, 0);

// 한글 레이아웃 유지
#define KEEP_HANGUL \
    if (GetAsyncKeyState(VK_HANGUL) & 0x8000) { \
        TOGGLE_HANGUL_KEY; \
    }

// 영어 레이아웃 유지
#define KEEP_ENGLISH \
    if (GetAsyncKeyState(VK_HANGUL) & 0x8000) { \
        TOGGLE_HANGUL_KEY; \
    }

// 게임 플레이 중에 한영키가 눌렸을 때 다시 한 번 눌러서 전환을 유지
#define HANDLE_HANGUL_KEY \
    if (GetAsyncKeyState(VK_HANGUL) & 0x8000) { \
        TOGGLE_HANGUL_KEY; \
        std::cout << "\nHangul key detected. Toggling back to the previous input mode.\n"; \
    }

// 이중 버퍼링에 사용할 핸들
HANDLE hMainConsole, hBackBuffer;
HKL hOriginalLayout;

// 백 버퍼 초기화
void initialize_double_buffer() {
    hMainConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hBackBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
}

// 버퍼 교체 함수
void swap_buffers() {
    SetConsoleActiveScreenBuffer(hBackBuffer);
    std::swap(hMainConsole, hBackBuffer);
}

// 백 버퍼에 출력하는 함수
void print_to_back_buffer(int x, int y, const std::string& text) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hBackBuffer, coord);
    std::cout << text;
}

std::map<std::string, std::vector<std::tuple<std::string, std::string, std::string>>> word_categories = {
    {"Animals", {
        {"giraffe", "긴 목과 다리를 가진 아프리카 포유류", "easy"},
        {"dolphin", "고도로 지능적인 바다 포유류", "easy"},
        {"panther", "흑표범이라 불리는 표범의 변종", "hard"},
        {"pelican", "주머니 모양의 부리를 가진 물새", "medium"},
        {"buffalo", "큰 뿔을 가진 북미 초식 동물", "hard"},
        {"elephant", "긴 코와 상아를 가진 가장 큰 육상 동물", "easy"},
        {"zebra", "흑백 줄무늬를 가진 말과 포유류", "easy"},
        {"koala", "유칼립투스 잎을 먹는 호주산 포유류", "easy"},
        {"tiger", "검은 줄무늬를 가진 육식성 대형 고양이과 동물", "easy"},
        {"bear", "대형 육식성 포유류로 종에 따라 다양한 환경에 서식", "easy"},
        {"wolf", "개과에 속하는 대형 야생 동물", "medium"},
        {"penguin", "비행하지 못하고 수영에 특화된 남극의 새", "easy"},
        {"kangaroo", "긴 뒷다리로 뛰며 이동하는 호주 포유류", "medium"},
        {"rabbit", "작고 귀가 긴 초식성 포유류", "easy"},
        {"camel", "등에 혹을 가진 사막의 초식성 동물", "easy"},
        {"monkey", "긴 꼬리를 가진 영장류 동물", "easy"},
        {"sloth", "느린 움직임을 보이는 나무 위에서 사는 포유류", "hard"},
        {"cheetah", "지상에서 가장 빠른 포유류", "medium"},
        {"hippopotamus", "아프리카의 반수생 대형 초식 동물", "extreme"},
        {"lion", "동물의 왕으로 불리는 육식성 동물", "medium"}
    }},
    {"Fruits", {
        {"apple", "나무에서 자라며 다양한 색을 가진 둥근 과일", "easy"},
        {"banana", "열대 식물의 길고 노란 과일", "easy"},
        {"orange", "두꺼운 껍질을 가진 감귤류 과일", "easy"},
        {"grape", "작고 둥글며 포도나무에서 자라는 과일", "easy"},
        {"mango", "달콤하고 즙이 많은 열대 과일", "medium"},
        {"peach", "털이 있는 껍질을 가진 여름철 과일", "easy"},
        {"plum", "달콤하고 큰 씨가 있는 둥근 과일", "medium"},
        {"pear", "부드러운 과육을 가진 원뿔형 과일", "medium"},
        {"kiwi", "작고 갈색 털이 있는 속이 녹색인 과일", "medium"},
        {"pineapple", "가시같은 겉껍질을 가진 열대 과일", "medium"},
        {"melon", "크고 즙이 많은 과육을 가진 과일", "medium"},
        {"cherry", "작고 빨간 색의 과일", "easy"},
        {"blueberry", "작고 파란 색의 과일", "medium"},
        {"pomegranate", "씨앗이 가득한 붉은 과일", "extreme"},
        {"nectarine", "복숭아와 비슷하지만 껍질이 부드러운 과일", "extreme"},
        {"apricot", "작고 오렌지색의 돌과일", "extreme"},
        {"fig", "작고 씨가 많은 달콤한 과일", "extreme"},
        {"grapefruit", "신맛이 나는 감귤류 과일", "extreme"},
        {"tangerine", "작고 달콤한 감귤류 과일", "hard"},
        {"lime", "신맛이 나는 녹색 감귤류 과일", "medium"}
    }},
    {"Technology", {
        {"processor", "컴퓨터에서 데이터를 처리하는 장치", "hard"},
        {"microchip", "전자 장치에서 회로를 구현한 작은 칩", "hard"},
        {"software", "컴퓨터에서 실행되는 프로그램", "medium"},
        {"algorithm", "문제를 해결하는 체계적인 절차", "hard"},
        {"database", "정보를 저장하고 관리하는 시스템", "medium"},
        {"network", "장치들이 서로 연결된 통신 시스템", "medium"},
        {"encryption", "데이터를 보호하기 위해 암호화하는 기술", "extreme"},
        {"server", "데이터와 서비스를 제공하는 컴퓨터", "medium"},
        {"interface", "시스템 간 상호작용을 위한 장치", "hard"},
        {"protocol", "통신을 위한 규칙과 표준", "hard"},
        {"firewall", "네트워크 보안을 위한 방어 시스템", "extreme"},
        {"bandwidth", "데이터 전송 속도를 나타내는 용량", "extreme"},
        {"virtualization", "물리적 자원을 가상 자원으로 변환하는 기술", "medium"},
        {"cloud_computing", "인터넷을 통한 컴퓨터 자원 제공 방식", "medium"},
        {"robotics", "로봇 설계 및 개발에 관한 학문", "hard"},
        {"quantum_computing", "양자역학을 이용한 계산 기술", "medium"},
        {"machine_learning", "데이터 패턴을 학습하여 성능을 개선하는 기술", "medium"},
        {"artificial_intelligence", "기계가 인간의 지능을 모방하는 기술", "medium"},
        {"blockchain", "거래 기록을 분산 원장에 저장하는 기술", "hard"},
        {"cybersecurity", "정보 시스템을 보호하기 위한 기술", "medium"}
    }},
    {"Nature", {
        {"volcano", "용암을 분출하는 지형", "medium"},
        {"glacier", "천천히 이동하는 거대한 얼음 덩어리", "hard"},
        {"tsunami", "지진이나 해저 폭발로 인한 대규모 해일", "easy"},
        {"hurricane", "강력한 바람과 비를 동반한 열대성 폭풍", "medium"},
        {"canyon", "깊고 협곡이 있는 계곡", "hard"},
        {"desert", "비가 거의 내리지 않는 건조한 지역", "easy"},
        {"rainforest", "열대 지역의 울창하고 비가 많은 숲", "medium"},
        {"tundra", "영구 동토층이 있는 추운 지역", "easy"},
        {"savanna", "드문 나무와 넓은 풀밭이 있는 지역", "medium"},
        {"wetland", "물로 포화된 지역, 늪지대", "medium"},
        {"river", "흐르는 큰 물줄기", "easy"},
        {"waterfall", "절벽에서 떨어지는 물줄기", "easy"},
        {"mountain", "높이 솟은 자연적인 돌출부", "easy"},
        {"forest", "나무가 무성하게 자라는 넓은 지역", "easy"},
        {"lake", "육지에 둘러싸인 물의 몸체", "easy"},
        {"island", "물이 둘러싼 육지", "easy"},
        {"bay", "부분적으로 육지에 둘러싸인 물의 몸체", "medium"},
        {"plain", "평평하고 넓은 땅", "easy"},
        {"plateau", "평평하고 높은 고원의 지형", "medium"},
        {"meadow", "초원으로 덮인 평평한 지역", "medium"}
    }},
    {"Sports", {
        {"soccer", "둥근 공을 이용한 팀 스포츠", "easy"},
        {"basketball", "바스켓에 공을 넣어 점수를 내는 스포츠", "easy"},
        {"tennis", "라켓과 작은 공으로 이루어진 스포츠", "easy"},
        {"golf", "공을 홀에 넣는 스포츠", "medium"},
        {"baseball", "두 팀이 배트와 공으로 경기하는 스포츠", "easy"},
        {"volleyball", "네트를 넘겨 공을 쳐내는 팀 스포츠", "easy"},
        {"hockey", "얼음 위에서 스틱과 퍽으로 경기하는 스포츠", "easy"},
        {"swimming", "물속에서 경쟁하는 스포츠", "easy"},
        {"cycling", "자전거로 경쟁하는 스포츠", "easy"},
        {"boxing", "글러브를 끼고 싸우는 격투 스포츠", "easy"},
        {"rugby", "타원형 공을 사용하는 팀 스포츠", "hard"},
        {"cricket", "야구와 비슷한 배트와 공으로 경기하는 팀 스포츠", "hard"},
        {"athletics", "달리기, 점프, 던지기를 포함한 스포츠", "medium"},
        {"wrestling", "상대방을 제압하여 승리하는 격투 스포츠", "medium"},
        {"gymnastics", "운동 능력과 균형을 시험하는 스포츠", "extreme"},
        {"skateboarding", "보드를 타고 트릭을 수행하는 스포츠", "medium"},
        {"fencing", "칼을 사용하여 득점하는 스포츠", "medium"},
        {"karate", "손과 발을 사용하는 무술", "medium"},
        {"judo", "던지기와 잡기를 중심으로 한 일본 무술", "medium"},
        {"taekwondo", "발차기와 타격을 중심으로 한 무술", "medium"}
    }},
    {"Food", {
        {"pizza", "도우 위에 다양한 재료를 올린 구운 음식", "easy"},
        {"burger", "고기 패티를 넣은 샌드위치", "easy"},
        {"pasta", "밀가루로 만든 면을 이용한 음식", "easy"},
        {"salad", "생야채와 드레싱을 곁들인 음식", "easy"},
        {"soup", "액체 형태로 끓인 음식", "easy"},
        {"sandwich", "빵 사이에 다양한 재료를 넣은 음식", "easy"},
        {"cake", "밀가루와 설탕으로 만든 구운 디저트", "easy"},
        {"ice_cream", "우유나 크림으로 만든 냉동 디저트", "easy"},
        {"tacos", "토르티야에 고기, 채소 등을 넣은 멕시코 음식", "medium"},
        {"sushi", "밥과 생선을 사용한 일본 음식", "medium"},
        {"bread", "밀가루로 만든 기본 식품", "easy"},
        {"pancakes", "반죽을 평평하게 구워 만든 음식", "easy"},
        {"chocolate", "카카오로 만든 달콤한 간식", "easy"},
        {"cookies", "작고 달콤하게 구운 간식", "easy"},
        {"donuts", "기름에 튀겨서 만든 반죽 디저트", "easy"},
        {"muffins", "작은 컵 모양으로 구운 빵", "easy"},
        {"waffles", "격자 모양으로 구운 달콤한 음식", "easy"},
        {"bagels", "원형으로 구운 빵", "easy"},
        {"croissant", "버터로 만든 겹겹이 된 프랑스 페이스트리", "medium"},
        {"noodles", "긴 면 형태의 음식", "easy"}
    }},
    {"History", {
        {"pyramid", "고대 이집트의 거대한 석조 무덤", "easy"},
        {"colosseum", "로마 제국의 대형 원형 경기장", "easy"},
        {"renaissance", "유럽의 예술과 학문 부흥기", "medium"},
        {"vikings", "유럽 북부의 항해사와 전사들", "medium"},
        {"industrial_revolution", "18세기 후반 산업화 시대의 변화", "hard"},
        {"world_war_I", "1914년에 시작된 세계적인 전쟁(숫자는 로마자로 표기)", "medium"},
        {"world_war_II", "1939년에 시작된 세계적인 전쟁(숫자는 로마자로 표기)", "medium"},
        {"medieval", "유럽 중세 시대", "hard"},
        {"roman_empire", "로마의 고대 제국", "medium"},
        {"french_revolution", "1789년 프랑스에서 일어난 대혁명", "medium"},
        {"the_great_wall_of_china", "북방 유목민족의 침략을 막기 위해 전국시대부터 건설된 성벽", "medium"},
        {"napoleon", "프랑스의 군사 지도자", "medium"},
        {"ancient_greece", "고대 그리스 문명", "easy"},
        {"cold_war", "미국과 소련 간의 정치적 갈등", "medium"},
        {"independence_day", "미국의 독립 기념일", "medium"},
        {"neil_armstrong", "1969년 달 착륙 계획의 사령관", "medium"},
        {"stonehenge", "잉글랜드의 고대 석조 유적", "hard"},
        {"little_boy", "1945년 8월 6일 일본에 투하된 원자폭탄의 이름", "hard"},
        {"apollo_11", "인류 최초의 달 착륙 임무", "medium"},
        {"suez_canal", "이집트의 중요한 운하", "hard"}
    }},
    {"Medicine", {
        {"vaccine", "질병을 예방하기 위해 사용하는 생물학적 제제", "easy"},
        {"antibiotic", "세균 감염을 치료하는 약물", "hard"},
        {"surgery", "수술로 신체를 치료하는 과정", "easy"},
        {"diagnosis", "질병을 확인하는 과정", "hard"},
        {"symptom", "질병의 징후나 표현", "easy"},
        {"pharmacy", "약을 조제하고 제공하는 장소", "hard"},
        {"therapy", "치료를 위한 방법", "hard"},
        {"prescription", "의사가 작성한 약물 처방 지시서", "hard"},
        {"immunization", "면역력을 주기 위한 예방 접종 과정", "hard"},
        {"stethoscope", "심장 및 폐 소리를 듣는 의료 장치", "extreme"},
        {"xray", "신체 내부를 촬영하는 방사선 검사", "medium"},
        {"ultrasound", "초음파를 사용하여 내부를 검사하는 기술", "hard"},
        {"medication", "질병을 치료하기 위한 약물", "hard"},
        {"treatment", "질병이나 부상에 대한 치료법", "hard"},
        {"clinical_trial", "새로운 치료법의 안전성과 효과를 시험하는 연구", "hard"},
        {"anesthesia", "수술 중 고통을 줄이기 위한 방법", "extreme"},
        {"blood_pressure", "혈관 내 혈압을 측정하는 수치", "medium"},
        {"allergy", "면역 체계가 과민 반응을 일으키는 상태", "easy"},
        {"rehabilitation", "부상이나 질병 후 기능 회복을 위한 과정", "hard"},
        {"dentistry", "치아와 잇몸을 치료하는 의료 분야", "medium"}
    }},
    {"Vehicles", {
        {"car", "일반적으로 4개의 바퀴를 가진 도로용 차량", "easy"},
        {"motorcycle", "두 바퀴로 움직이는 차량", "easy"},
        {"bicycle", "사람이 페달을 밟아 이동하는 두 바퀴 차량", "easy"},
        {"bus", "여러 사람을 태우는 대형 도로 차량", "easy"},
        {"train", "궤도 위를 달리는 대형 교통 수단", "medium"},
        {"airplane", "하늘을 나는 교통 수단", "medium"},
        {"helicopter", "회전 날개를 이용해 하늘을 나는 비행기", "medium"},
        {"boat", "물 위를 떠다니는 작은 교통 수단", "medium"},
        {"ship", "대형 수상 교통 수단", "medium"},
        {"truck", "대형 화물을 운반하는 도로 차량", "medium"},
        {"subway", "도시 내 지하를 달리는 열차", "medium"},
        {"tram", "도시 내 궤도를 달리는 차량", "hard"},
        {"scooter", "소형 오토바이, 작고 가벼운 모터를 장착한 도로 차량", "medium"},
        {"skateboard", "작은 판 위에서 바퀴를 굴려 타는 장비", "medium"},
        {"jet_ski", "물 위에서 달리는 개인용 수상 교통 수단", "hard"},
        {"yacht", "레저용 대형 고급 보트", "hard"},
        {"snowmobile", "눈 위에서 주행하는 차량", "hard"},
        {"hovercraft", "공기 쿠션으로 물과 육지를 이동하는 차량", "hard"},
        {"tanker", "액체 화물을 운반하는 대형 선박", "hard"},
        {"spaceship", "우주를 다니도록 만들어진 탈것", "easy"}
    }},
    {"Instruments", {
        {"guitar", "6개의 줄을 가진 악기", "easy"},
        {"piano", "건반을 눌러 소리를 내는 악기", "easy"},
        {"violin", "활을 사용해 연주하는 현악기", "easy"},
        {"flute", "바람을 불어 연주하는 악기", "medium"},
        {"trumpet", "피스톤으로 조절되는 금관악기", "medium"},
        {"drum", "리듬을 연주하는 타악기", "easy"},
        {"saxophone", "리드를 이용해 소리를 내는 금관악기", "hard"},
        {"cello", "저음역을 내는 현악기(비올라보다 한 옥타브 아래)", "hard"},
        {"harp", "줄을 뜯어 연주하는 발현악기", "hard"},
        {"accordion", "바람을 넣어 소리를 내는 악기", "hard"},
        {"xylophone", "나무 봉으로 쳐서 연주하는 악기", "medium"},
        {"clarinet", "리드를 사용해 소리를 내는 목관악기, 클라리온의 축소명", "medium"},
        {"tambourine", "소형의 한쪽만 가죽막이 있는 북", "medium"},
        {"trombone", "슬라이드를 이용해 음을 조절하는 금관악기", "hard"},
        {"ukulele", "작고 네 줄을 가진 현악기", "easy"},
        {"banjo", "재즈나 민속음악에 쓰이는 기타의 일종으로 독특한 소리를 가진 악기", "extreme"},
        {"oboe", "더블 리드를 사용하는 목관악기", "hard"},
        {"bagpipe", "바람을 불어 연주하는 스코틀랜드 전통 악기", "extreme"},
        {"bassoon", "목관악기 중 하나로, 저음역을 내는 악기", "hard"},
        {"mandolin", "8개의 줄을 가진 작은 현악기", "extreme"}
    }}
};

bool is_alphabet(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

std::string get_input_with_backspace() {
    std::string input;
    char ch;

    while (true) {
        ch = _getch();  // 한 글자씩 입력 받음
        if (ch == '\r') {  // Enter 입력 시 입력 종료
            break;
        }
        else if (ch == '\b') {  // 백스페이스 처리
            if (!input.empty()) {
                // 화면에서 마지막 글자를 지우고 커서 이동
                std::cout << "\b \b";
                input.pop_back();  // 입력 문자열에서도 마지막 글자 삭제
            }
        }
        else {
            input.push_back(ch);
            std::cout << ch;  // 입력된 문자 출력
        }
    }

    return input;
}

// 콘솔 관련 유틸리티 함수들
// ---------------------------------

// 콘솔 텍스트 색상을 설정하는 함수
static void set_console_color(int text_color, int bg_color = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text_color + (bg_color << 4));
}

// 난이도에 따라 색상을 설정하는 함수
void set_difficulty_color(const std::string& difficulty) {
    if (difficulty == "easy") {
        set_console_color(EASY_COLOR);
    }
    else if (difficulty == "medium") {
        set_console_color(MEDIUM_COLOR);
    }
    else if (difficulty == "hard") {
        set_console_color(HARD_COLOR);
    }
    else if (difficulty == "extreme") {
        set_console_color(EXTREME_COLOR);
    }
}

// 콘솔 화면 초기화 함수
static void clear_screen() {
    system("cls");  // 콘솔 창 지우기 (Windows)
}

// 콘솔 화면 크기 얻기 함수
COORD get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hMainConsole, &csbi);
    return csbi.dwSize;
}

void handle_timeout(const std::string& word, int& attempts_left) {
    clear_screen();

    // 타임아웃 메시지 출력
    set_console_color(12);  // 빨간색으로 타임아웃 메시지 표시
    std::cout << "\n\nTime's up! The correct word was: " << word << std::endl;

    // 남은 시도 횟수 출력
    set_console_color(14);  // 노란색으로 남은 시도 횟수 표시
    std::cout << "\nAttempts left: " << attempts_left << std::endl;

    // Enter 키를 눌러서 계속하도록 메시지 출력
    set_console_color(10);  // 초록색으로 안내 메시지 표시
    std::cout << "\nPress Enter to continue..." << std::endl;

    set_console_color(7);  // 기본 색상으로 복원

    // Enter 키를 누를 때까지 대기
    while (_getch() != '\r');
}

// 최소한의 영역만 업데이트 (화면을 지우지 않음)
void update_console(const std::vector<std::string>& content, int start_x, int start_y) {
    for (int i = 0; i < content.size(); ++i) {
        print_to_back_buffer(start_x, start_y + i, content[i]);
    }
    swap_buffers(); // 백 버퍼와 교체
}

// 테두리를 출력하는 함수
void print_border(int length, char symbol = '=') {
    for (int i = 0; i < length; i++) {
        std::cout << symbol;
    }
    std::cout << std::endl;
}

// 좌표에 텍스트 출력 함수
static void print_at_position(int x, int y, const std::string& text) {
    COORD cursor_position = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_position);
    std::cout << text;
}

// 콘솔 커서 숨기기 함수
void hide_cursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// 메뉴 탐색 함수
int navigate_menu(const std::vector<std::string>& menu_items, const std::string& title) {
    int selected_index = 0;
    int menu_size = menu_items.size();

    while (true) {
        // 화면을 지우고 메뉴를 다시 출력
        system("cls");
        set_console_color(14);
        print_border(40);  // 상단 테두리
        set_console_color(11);  // 파란색 텍스트
        std::cout << "        " << title << "        " << std::endl;
        set_console_color(14);
        print_border(40);  // 하단 테두리

        // 메뉴 항목을 출력
        for (int i = 0; i < menu_size; ++i) {
            if (i == selected_index) {
                set_console_color(11);  // 선택된 항목은 파란색으로 표시
                std::cout << ">> " << menu_items[i] << std::endl;  // 메뉴 항목 출력
            }
            else {
                set_console_color(7);  // 선택되지 않은 항목은 기본 색상
                std::cout << "   " << menu_items[i] << std::endl;  // 메뉴 항목 출력
            }
        }

        // 키 입력을 기다림
        int key = _getch();
        if (key == 0 || key == 224) {  // 화살표 키 입력 시
            key = _getch();  // 실제 화살표 키 값 가져오기
            if (key == KEY_UP) {
                selected_index = (selected_index - 1 + menu_size) % menu_size;  // 위쪽으로 이동
            }
            else if (key == KEY_DOWN) {
                selected_index = (selected_index + 1) % menu_size;  // 아래쪽으로 이동
            }
        }
        else if (key == KEY_ENTER) {
            return selected_index;  // 엔터키를 누르면 선택된 인덱스 반환
        }
    }
}

// 화면 흔들기 애니메이션
void shake_screen() {
    COORD console_size = get_console_size();
    int offset_range = 2;  // 흔들리는 범위 (좌우로 움직이는 범위)

    for (int i = 0; i < 10; ++i) {
        int offset_x = (rand() % (offset_range * 2 + 1)) - offset_range;  // -offset_range에서 +offset_range까지의 값 생성
        int offset_y = 0;  // 상하로는 움직이지 않음

        // 흔들린 위치에 "Time's up!" 메시지를 출력
        print_at_position(console_size.X / 2 + offset_x, console_size.Y / 2 + offset_y, "Time's up!");

        // 약간의 지연을 주어 흔들리는 효과 부여
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        clear_screen();  // 화면을 지운 후 반복하여 흔들림 효과
    }
}

// 점수 애니메이션 (점수가 올라가는 효과)
void score_animation(int current_score, int new_score) {
    int score_increment = (new_score - current_score) / 10;  // 점수를 10 단계로 나누어 상승시킴

    // 점수를 스코어 표시되는 위치에 맞추기 위해 위치 설정
    int score_position_x = 30;  // 스코어 표시 X 위치
    int score_position_y = 9;   // 스코어 표시 Y 위치

    for (int i = 0; i <= 10; ++i) {
        int displayed_score = current_score + score_increment * i;  // 점점 증가하는 점수

        set_console_color(10);  // 초록색으로 점수 표시
        // 기존 스코어 자리에 스코어 애니메이션을 출력하고, "points" 텍스트를 추가
        print_at_position(score_position_x, score_position_y, "Score: " + std::to_string(displayed_score) + " points");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 애니메이션 속도 조절
    }

    // 최종 스코어 출력
    print_at_position(score_position_x, score_position_y, "Score: " + std::to_string(new_score) + " points");

    set_console_color(7);  // 기본 색상으로 복원
}


// 로딩 애니메이션 (점점 길어지는 점 표시)
void loading_animation() {
    std::string loading_text = "Loading";

    for (int i = 0; i < 10; ++i) {
        std::string dots(i % 4, '.');  // 점의 개수가 늘어남
        print_at_position(30, 10, loading_text + dots);  // 로딩 메시지 출력
        std::this_thread::sleep_for(std::chrono::milliseconds(300));  // 지연 시간
        clear_screen();  // 화면을 지우고 반복하여 애니메이션 효과
    }

    set_console_color(7);  // 기본 색상으로 복원
}

// 환영 메시지 애니메이션
void welcome_animation() {
    std::string welcome_message = "Welcome to Hangman Game!";

    for (int i = 0; i <= welcome_message.length(); ++i) {
        print_at_position(30, 10, welcome_message.substr(0, i));  // 한 글자씩 출력
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 지연 시간
    }

    set_console_color(7);  // 기본 색상으로 복원
}

// 타이머 UI를 출력하는 함수
static void print_timer(int remaining_time, int hint_position_y) {
    COORD console_size = get_console_size();
    int timer_position_x = (console_size.X - 60) / 2;
    int timer_position_y = hint_position_y + 2;

    // 여백을 이용해 기존 내용을 덮어씀
    print_at_position(timer_position_x, timer_position_y, "                                                   ");

    if (remaining_time <= WARNING_THRESHOLD) {
        set_console_color(12);  // 적은 시간이 남으면 빨간색
    }
    else {
        set_console_color(10);  // 충분한 시간이 있으면 초록색
    }

    // 남은 시간 표시
    print_at_position(timer_position_x, timer_position_y, "Time left: " + std::to_string(remaining_time) + "s");

    // 진행 바를 출력 (덮어쓰기로 화면 깜빡임 방지)
    std::string progress_bar = "[";
    int total_bars = 30;
    int filled_bars = (remaining_time * total_bars) / TIMER_DURATION;

    for (int i = 0; i < filled_bars; ++i) {
        progress_bar += "#";
    }
    for (int i = filled_bars; i < total_bars; ++i) {
        progress_bar += " ";
    }
    progress_bar += "]";

    set_console_color(11);  // 진행 바를 파란색으로 출력
    print_at_position(timer_position_x, timer_position_y + 1, progress_bar);

    set_console_color(7);  // 기본 색상 복원
}

#include <thread>

//축하 메시지, 점수 애니메이션을 동시에 실행하는 함수
void play_concurrent_celebration(const std::string& player_name, const std::string& word, int current_score, int new_score) {
    // 축하 메시지와 점수 애니메이션 출력
    int score_position_x = 30;  // 스코어 표시 X 위치
    int score_position_y = 5;   // 스코어 표시 Y 위치

    // 축하 메시지 출력
    set_console_color(10);  // 초록색으로 축하 메시지 출력
    print_at_position(score_position_x, score_position_y - 3, "Congratulations, " + player_name + "! You've guessed the word:");
    set_console_color(11);  // 파란색으로 맞춘 단어 강조
    print_at_position(score_position_x, score_position_y - 2, word);

    // 점수 애니메이션 실행
    score_animation(current_score, new_score);

    // "Press Enter to continue..." 메시지를 기다리기
    set_console_color(7);  // 기본 색상으로 복원
    print_at_position(score_position_x, score_position_y + 3, "Press Enter to continue...");

    // Enter 키 입력 대기
    while (_getch() != '\r');

    set_console_color(7);  // 기본 색상 복원
}

// 타이머 흔들리기 애니메이션 함수
static void animate_timer(int remaining_time, int hint_position_y) {
    COORD console_size = get_console_size();
    int timer_position_x = (console_size.X - 60) / 2;  // 타이머 중앙 정렬
    int timer_position_y = hint_position_y + 2;  // 힌트 바로 밑에 타이머 숫자 위치

    // 타이머 숫자만 좌우로 약하게 흔들리도록 설정 (상하로는 움직이지 않음)
    for (int i = 0; i < 10; ++i) {
        // 좌우로만 움직이도록 x 좌표에서 -1 ~ 1 범위로만 흔들림
        int offset_x = (rand() % 3) - 1;  // -1, 0, 1 중 하나 선택 (좌우로만 움직임)

        // 타이머 숫자만 흔들리게 표시 (상하는 움직이지 않음)
        set_console_color(12);  // 빨간색
        print_at_position(timer_position_x + offset_x, timer_position_y, "                     ");
        print_at_position(timer_position_x + offset_x, timer_position_y, "Time left: " + std::to_string(remaining_time) + "s");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 짧은 대기 시간
    }

    // 흔들린 후 타이머 숫자 위치 정리
    print_at_position(timer_position_x, timer_position_y, "                     ");
    print_at_position(timer_position_x, timer_position_y, "Time left: " + std::to_string(remaining_time) + "s");

    set_console_color(7);  // 기본 색상으로 복원
}

// 점수 및 입력 관련 함수들
// ---------------------------------

// 점수를 파일에 저장하는 함수
static void save_score(const std::string& student_number, const std::string& player_name, int score) {
    std::ofstream ranking_file("ranking.txt", std::ios::app);
    if (ranking_file.is_open()) {
        std::time_t now = std::time(nullptr);  // 현재 시간
        ranking_file << student_number << ", " << player_name << ": " << score << " points, " << now << "\n";
        ranking_file.close();
    }
}

// 유효한 숫자 입력을 받을 때까지 반복하는 함수
static int get_valid_number() {
    int num;
    while (true) {
        std::cin >> num;
        if (std::cin.fail()) {
            std::cin.clear(); // 입력 스트림 상태 초기화
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a valid number: ";
        }
        else {
            return num; // 유효한 숫자일 경우 반환
        }
    }
}

// 난이도에 따른 점수 반환 함수
static int get_score_for_difficulty(const std::string& difficulty) {
    if (difficulty == "easy") return 1000;
    else if (difficulty == "medium") return 1500;
    else if (difficulty == "hard") return 2000;
    else if (difficulty == "extreme") return 4000;
    return 0; // Invalid difficulty returns 0
}

// 커서 위치 바로 밑에 출력하는 pause 함수
static void pause() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int current_cursor_x = csbi.dwCursorPosition.X;
    int current_cursor_y = csbi.dwCursorPosition.Y;

    print_at_position(current_cursor_x, current_cursor_y + 1, "계속하려면 아무키나 누르십시오...");
    std::cin.get(); std::cin.get();  // 버퍼 처리를 위해 두 번 입력 받음
}

// 스피너 애니메이션 함수
static void spinner_animation(int duration_seconds) {
    std::vector<std::string> spinner_frames = { "|", "/", "-", "\\" };
    int num_frames = spinner_frames.size();
    set_console_color(11);  // 파란색 스피너

    auto start_time = std::chrono::system_clock::now();
    int frame_index = 0;

    while (true) {
        auto current_time = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (elapsed >= duration_seconds) break;

        clear_screen();
        set_console_color(11);
        print_border(40);
        set_console_color(14);  // 노란색 텍스트
        std::cout << "  Loading... " << spinner_frames[frame_index] << std::endl;
        set_console_color(11);
        print_border(40);

        frame_index = (frame_index + 1) % num_frames;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));  // 프레임 간 대기 시간
    }

    set_console_color(7);
    clear_screen();
}

// 게임 흐름 관련 함수들
// ---------------------------------

bool confirm_info(const std::string& student_number, const std::string& player_name) {
    clear_screen();
    set_console_color(14);
    print_border(40);
    set_console_color(11);
    std::cout << "       Confirm Player Information         " << std::endl;
    set_console_color(14);
    print_border(40);

    set_console_color(7);
    std::cout << "Your entered student number is: " << student_number << std::endl;
    std::cout << "Your entered name is: " << player_name << std::endl;
    std::cout << "Press Enter to confirm, or ESC to return to the lobby." << std::endl;

    while (true) {
        int key = _getch();
        if (key == 13) {  // Enter 키
            return true;  // 정보 확인 후 게임 계속 진행
        }
        else if (key == 27) {  // ESC 키
            return false;  // 로비로 돌아가기
        }
    }
}

// 학번과 이름 입력을 받는 함수
static bool get_student_info(std::string& student_number, std::string& player_name) {
    clear_screen();
    set_console_color(14);
    print_border(40);
    set_console_color(11);
    std::cout << "          Player Registration            " << std::endl;
    set_console_color(14);
    print_border(40);

    set_console_color(7);
    std::cout << "Enter your student number (5 digits): ";
    while (true) {
        std::cin >> student_number;
        if (student_number.length() == 5 && std::all_of(student_number.begin(), student_number.end(), ::isdigit)) break;
        else {
            set_console_color(12);
            std::cout << "Invalid student number! Please enter exactly 5 digits.\n";
            set_console_color(7);
            std::cout << "Re-enter your student number: ";
        }
    }

    // 한영키 전환 (한글로)
    TOGGLE_HANGUL_KEY;

    std::cout << "Enter your name (Korean): ";
    player_name = get_input_with_backspace();  // 백스페이스 처리를 포함한 입력 처리

    // 영어로 전환
    TOGGLE_HANGUL_KEY;

    std::cout << "\nSwitching back to English for gameplay...\n";
    spinner_animation(3);

    // 학번과 이름 확인 단계
    if (!confirm_info(student_number, player_name)) {
        clear_screen();
        set_console_color(12);
        std::cout << "Returning to the lobby..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        return false;  // ESC를 눌렀을 경우 로비로 돌아가도록 false 반환
    }

    clear_screen();
    set_console_color(14);
    print_border(40);
    set_console_color(11);
    std::cout << "Welcome, " << player_name << " (" << student_number << ")!" << std::endl;
    set_console_color(14);
    print_border(40);

    std::cout << "\nPress any key to start the game...";
    _getch();
    return true;  // 정상적으로 정보를 확인했을 경우 true 반환
}

// 단어 선택 및 힌트 제공 함수
static std::tuple<std::string, std::string> get_random_word(const std::string& category, std::set<std::string>& used_words) {
    srand(static_cast<unsigned int>(time(0)));
    const auto& category_words = word_categories[category];

    std::vector<std::tuple<std::string, std::string, std::string>> available_words;
    for (const auto& word_info : category_words) {
        if (used_words.find(std::get<0>(word_info)) == used_words.end()) available_words.push_back(word_info);
    }

    if (available_words.empty()) return std::make_tuple("", "");
    int index = rand() % available_words.size();
    used_words.insert(std::get<0>(available_words[index]));
    return std::make_tuple(std::get<0>(available_words[index]), std::get<2>(available_words[index]));
}

static std::string get_hint(const std::string& category, const std::string& word) {
    for (const auto& word_info : word_categories[category]) {
        if (std::get<0>(word_info) == word) return std::get<1>(word_info);
    }
    return "";
}

// 단어 준비 및 표시 함수
static std::string prepare_word(const std::string& word) {
    std::string prepared_word;
    for (char letter : word) {
        if (letter != '_') prepared_word += letter;
    }
    return prepared_word;
}

static std::string get_display_word(const std::string& word, const std::set<char>& correct_guesses) {
    std::string display_word;
    for (char letter : word) {
        if (letter == '_') display_word += ' ';
        else if (correct_guesses.find(letter) != correct_guesses.end()) display_word += letter;
        else display_word += '_';
        display_word += ' ';
    }
    return display_word;
}

// 행맨 그림 출력 함수
static void print_hangman(int attempts_left) {
    std::vector<std::string> hangman_lines;
    if (attempts_left == 6) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |             ",
            " |             ",
            " |             ",
            " |             ",
            " |             ",
            "_|_            "
        };
    }
    else if (attempts_left == 5) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |         O   ",
            " |             ",
            " |             ",
            " |             ",
            " |             ",
            "_|_            "
        };
    }
    else if (attempts_left == 4) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |         O   ",
            " |         |   ",
            " |             ",
            " |             ",
            " |             ",
            "_|_            "
        };
    }
    else if (attempts_left == 3) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |         O   ",
            " |        /|   ",
            " |             ",
            " |             ",
            " |             ",
            "_|_            "
        };
    }
    else if (attempts_left == 2) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |         O   ",
            " |        /|\\  ",
            " |             ",
            " |             ",
            " |             ",
            "_|_            "
        };
    }
    else if (attempts_left == 1) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |         O   ",
            " |        /|\\  ",
            " |        /    ",
            " |             ",
            " |             ",
            "_|_            "
        };
    }
    else if (attempts_left == 0) {
        hangman_lines = {
            "  _________    ",
            " |         |   ",
            " |         O   ",
            " |        /|\\  ",
            " |        / \\  ",
            " |             ",
            " |    R.I.P.   ",
            "_|_            "
        };
    }

    COORD console_size = get_console_size();
    int start_x = (console_size.X - 105) / 2;
    int start_y = 1;

    for (int i = 0; i < hangman_lines.size(); ++i) {
        set_console_color(attempts_left > 0 ? 10 : 12);
        print_at_position(start_x, start_y + i, hangman_lines[i]);
    }
    set_console_color(7);
}

// 게임 정보 출력 함수
static void print_game_info(int attempts_left, const std::string& current_word, const std::set<char>& wrong_guesses, const std::string& difficulty, int score) {
    COORD console_size = get_console_size();
    int hint_position_x = 30;
    int starting_y = 3;

    set_console_color(14);
    print_at_position(hint_position_x, starting_y + 2, "Attempts left: " + std::to_string(attempts_left));

    set_console_color(11);
    print_at_position(hint_position_x, starting_y + 3, "Current word: " + current_word);

    set_difficulty_color(difficulty);  // 난이도에 따른 색상 설정
    print_at_position(hint_position_x, starting_y + 4, "Difficulty: " + difficulty);  // 난이도 출력

    set_console_color(13);
    std::string wrong_guesses_text = "Wrong guesses: ";
    for (char c : wrong_guesses) {
        wrong_guesses_text += c;
        wrong_guesses_text += ' ';
    }
    print_at_position(hint_position_x, starting_y + 5, wrong_guesses_text);

    // 실시간 점수 출력
    set_console_color(10);  // 초록색으로 점수 출력
    print_at_position(hint_position_x, starting_y + 6, "Score: " + std::to_string(score) + " points");
    set_console_color(7);  // 기본 색상 복원
}

// 카테고리 선택 메뉴 탐색
static int navigate_category_menu(const std::set<std::string>& used_words, const std::set<std::string>& completed_categories) {
    std::vector<std::string> categories = {
        "1. Animals", "2. Fruits", "3. Technology", "4. Nature",
        "5. Sports", "6. Food", "7. History", "8. Medicine",
        "9. Vehicles", "10.Instruments"
    };

    std::vector<std::string> category_info;
    for (const auto& category : categories) {
        std::string category_name = category.substr(3);
        const auto& words = word_categories[category_name];
        int easy_count = 0, medium_count = 0, hard_count = 0, extreme_count = 0, remaining_words = 0;

        for (const auto& word_info : words) {
            const std::string& word = std::get<0>(word_info);
            const std::string& difficulty = std::get<2>(word_info);
            if (used_words.find(word) == used_words.end()) {
                remaining_words++;
                if (difficulty == "easy") easy_count++;
                else if (difficulty == "medium") medium_count++;
                else if (difficulty == "hard") hard_count++;
                else if (difficulty == "extreme") extreme_count++;
            }
        }

        std::stringstream ss;
        ss << category_name << " [" << remaining_words << " words] | ";

        // 각 난이도별로 다른 색상 적용
        set_console_color(10);  // 초록색 (easy)
        ss << "E: " << easy_count << " ";
        set_console_color(14);  // 노란색 (medium)
        ss << "M: " << medium_count << " ";
        set_console_color(12);  // 빨간색 (hard)
        ss << "H: " << hard_count << " ";
        set_console_color(13);  // 보라색 (extreme)
        ss << "EX: " << extreme_count;

        if (completed_categories.find(category_name) != completed_categories.end()) {
            ss << " (Completed)";
        }
        category_info.push_back(ss.str());
    }

    while (true) {
        int choice = navigate_menu(category_info, "   Select a Category");
        std::string category_name = categories[choice].substr(3);

        if (completed_categories.find(category_name) != completed_categories.end()) {
            std::cout << "You have already completed all the words in this category! Please select another category.\n";
            continue;
        }
        return choice;
    }
}


// 실제 행맨 게임 실행
static bool play_hangman(const std::string& student_number, const std::string& player_name, const std::string& category, std::set<std::string>& used_words, int& attempts_left, int& score, int& bonus_streak, int& skip_chances) {
    clear_screen();

    // 단어와 난이도 선택
    std::string original_word, difficulty;
    std::tie(original_word, difficulty) = get_random_word(category, used_words);
    std::string word = prepare_word(original_word);

    // 더 이상 사용할 단어가 없을 경우 종료
    if (word.empty()) {
        std::cout << "No more words available in this category!\n";
        return false;
    }

    // 추측한 글자 저장을 위한 set
    std::set<char> correct_guesses;
    std::set<char> wrong_guesses;
    std::string display_word = get_display_word(word, correct_guesses);
    std::string hint = get_hint(category, original_word);

    // 힌트가 출력될 위치 설정
    int hint_position_x = 30;  // 게임 정보 및 힌트 출력 위치 설정
    int hint_position_y = 1;   // 힌트의 y 좌표 설정

    // 시도 횟수가 남아있는 동안 게임 진행
    while (attempts_left > 0) {
        int remaining_time = TIMER_DURATION;  // 타이머 시작
        auto start_time = std::chrono::system_clock::now();

        // 타이머 동작
        while (remaining_time > 0) {
            // 한영키 처리 (게임 중에 한영키가 눌리면 다시 눌러서 원래 상태 유지)
            HANDLE_HANGUL_KEY;

            // 경과된 시간 계산
            auto current_time = std::chrono::system_clock::now();
            remaining_time = TIMER_DURATION - std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

            // 화면을 지우고 현재 상태 출력
            clear_screen();
            print_hangman(attempts_left);
            print_at_position(hint_position_x, 0, "Category: " + category);
            print_at_position(hint_position_x, 1, "Hint: " + hint);
            display_word = get_display_word(word, correct_guesses);
            print_game_info(attempts_left, display_word, wrong_guesses, difficulty, score);
            print_at_position(hint_position_x, 5, "Skip chances left (press 'Tab' to skip): " + std::to_string(skip_chances));
            print_timer(remaining_time, hint_position_y);

            // 남은 시간이 10초 이하일 때 타이머 애니메이션
            if (remaining_time <= WARNING_THRESHOLD) {
                animate_timer(remaining_time, hint_position_y);
            }

            // 사용자가 키를 눌렀을 경우
            if (_kbhit()) {
                int key = _getch();  // 첫 번째 키 입력

                // 화살표 키나 특수 키는 두 번의 입력으로 이루어짐 (첫 번째 입력이 224 또는 0)
                if (key == 0 || key == 224) {
                    _getch();  // 두 번째 키 입력을 처리하고 무시
                    continue;  // 화살표 키는 무시
                }

                char guess = tolower(key);  // 입력된 문자를 소문자로 변환

                // 영어 레이아웃으로 유지 (한영키가 눌렸을 경우)
                KEEP_ENGLISH;

                // 스킵을 위한 입력 (Tab 키를 사용)
                if (guess == 9 && skip_chances > 0) {  // Tab 키의 ASCII 값은 9입니다
                    skip_chances--;  // 스킵 기회를 차감

                    // 스킵 메시지를 행맨 그림 아래에 출력
                    int skip_message_position_y = 10;  // 행맨 그림 아래로 설정 (행맨 그림의 높이에 맞춤)
                    int skip_message_position_x = 1;  // 적절한 x 좌표 설정

                    set_console_color(14);  // 노란색으로 스킵 메시지 출력
                    print_at_position(skip_message_position_x, skip_message_position_y, "You skipped the word!");
                    print_at_position(skip_message_position_x, skip_message_position_y + 1, "The correct word was: " + word);  // 정답 출력
                    print_at_position(skip_message_position_x, skip_message_position_y + 2, "Press Enter to continue...");
                    set_console_color(7);  // 기본 색상으로 복원

                    // Enter 키를 누를 때까지 대기
                    while (_getch() != '\r');

                    return true;  // 스킵 시 단어를 맞춘 것으로 처리하고 다음 단어로 이동
                }

                // 입력된 문자가 알파벳인지 확인
                if (is_alphabet(guess)) {
                    // 이미 추측한 문자인지 확인
                    if (correct_guesses.find(guess) == correct_guesses.end() && wrong_guesses.find(guess) == wrong_guesses.end()) {
                        if (word.find(guess) != std::string::npos) {
                            correct_guesses.insert(guess);  // 맞춘 문자는 저장
                            bonus_streak++;  // 보너스 스트릭 증가

                            // 남은 시간에 기반한 추가 보너스 점수 계산
                            int time_bonus = static_cast<int>((static_cast<double>(remaining_time) / TIMER_DURATION) * 100);

                            // 기본 점수 + (보너스 스트릭 점수 + 시간 보너스)
                            score += (bonus_streak * 100) + time_bonus;  // 각 맞춤에 대한 점수는 단어 난이도에 따라 달라질 수 있음

                            start_time = std::chrono::system_clock::now();  // 타이머 초기화
                            break;  // 타이머 초기화 후 루프 탈출
                        }
                        else {
                            wrong_guesses.insert(guess);  // 틀린 문자는 저장
                            attempts_left--;  // 목숨 하나 줄어듦
                            bonus_streak = 0;  // 보너스 스트릭 초기화
                            break;  // 루프 탈출
                        }
                    }
                }
                else {
                    // 알파벳이 아닌 문자가 입력되면 무시
                    set_console_color(12);  // 빨간색으로 경고 메시지 출력
                    print_at_position(hint_position_x, hint_position_y + 3, "Invalid input! Please enter only alphabets.");
                    set_console_color(7);  // 기본 색상으로 복원
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // 타이머 0.2초마다 갱신
        }

        // 타이머 종료 시 목숨 감소
        if (remaining_time <= 0) {
            clear_screen();
            set_console_color(12);  // 타임 아웃 메시지의 색상을 변경
            shake_screen();  // 타임아웃 시 화면 흔들기 애니메이션 실행
            print_at_position(30, 10, "Time's up! The correct word was: " + word);  // 타임아웃 메시지 출력
            std::cout << "Press Enter to continue...";
            while (_getch() != '\r');  // Enter 키를 누를 때까지 대기
            attempts_left--;  // 목숨 하나 줄어듦
            break;
        }

        // 단어가 완성되었는지 확인
        bool is_word_complete = true;
        for (char letter : word) {
            if (correct_guesses.find(letter) == correct_guesses.end() && letter != ' ') {
                is_word_complete = false;
                break;
            }
        }

        // 단어가 완성되었을 경우
        if (is_word_complete) {
            clear_screen();
            print_hangman(attempts_left);
            print_game_info(attempts_left, display_word, wrong_guesses, difficulty,score);

            set_console_color(10);  // 초록색으로 축하 메시지 출력

            // 축하 메시지를 두 줄 더 내리기 위해 앞에 줄바꿈 추가
            std::cout << "\n\n\nCongratulations, " << player_name << "! You've guessed the word: ";
            set_console_color(11);  // 파란색으로 맞춘 단어 강조
            std::cout << word << std::endl;

            // "Press Enter to continue..." 메시지를 바로 밑에 출력
            set_console_color(7);  // 기본 색상으로 복원
            std::cout << "\nPress Enter to continue...";

            // 스코어 애니메이션을 네 줄 밑으로 내리기 위해 줄바꿈 추가
            std::cout << "\n\n\n\n";
            int points = get_score_for_difficulty(difficulty);
            int new_score = score + points;
            score_animation(score, new_score);  // 점수 상승 애니메이션 실행
            score = new_score;

            while (_getch() != '\r');

            return true;
        }
    }

    // 시도 횟수가 모두 소진되었을 때
    if (attempts_left == 0) {
        clear_screen();
        print_hangman(0);  // 마지막 행맨 그림 출력
        print_at_position(30, 0, "====================");
        print_at_position(30, 1, "      GAME OVER     ");
        print_at_position(30, 2, "====================");

        // 정답 단어 출력
        print_at_position(30, 4, "The correct word was: " + word);

        // 점수 출력
        print_at_position(30, 5, "Your final score is: " + std::to_string(score) + " points.");

        pause();  // 게임 오버 후 대기
        return false;  // 게임 오버 시 false 반환
    }

    return true;
}


// 게임 메인 로비 및 흐름 제어
// ---------------------------------

static void display_lobby() {
    clear_screen();
    set_console_color(14);
    print_border(40);
    set_console_color(11);
    std::cout << "        Welcome to Hangman Game        " << std::endl;
    set_console_color(14);
    print_border(40);

    set_console_color(7);
    std::cout << "1. Play Game\n";
    std::cout << "2. View Rules\n";
    std::cout << "3. View Rankings\n";
    std::cout << "4. Exit\n";
    print_border(40);
}

// 게임 규칙을 표시하는 함수
static void show_rules() {
    clear_screen();
    set_console_color(14);
    print_border(40);
    set_console_color(11);
    std::cout << "              Game Rules            " << std::endl;
    set_console_color(14);
    print_border(40);

    set_console_color(7);
    std::cout << "1. 단어를 한 글자씩 추측하세요.\n";
    std::cout << "2. 틀린 추측을 6번 할 수 있습니다. attenmps left가 다 없어지면 게임 오버됩니다.\n";
    std::cout << "3. 틀린 추측을 할 때마다 행맨의 일부가 그려집니다.\n";
    std::cout << "4. 스킵기회는 총 3번 있습니다.\n";
    std::cout << "5. 띄어쓰기는 생략합니다.\n";
    std::cout << "6. 힌트가 주어지니까 현명하게 활용해서 문제를 해결하세요.\n";
    std::cout << "7. 단어의 난이도에 따라 점수가 부여됩니다:\n";
    std::cout << "   - 쉬움: 1000점\n";
    std::cout << "   - 보통: 1500점\n";
    std::cout << "   - 어려움: 2000점\n";
    std::cout << "   - 익스트림: 4000점\n";
    std::cout << "8. 연속으로 철자를 맞출 경우 추가로 보너스를 받습니다.\n";
    print_border(40);

    set_console_color(7);
    std::cout << "\nPress any key to return to the lobby...";
    while (std::cin.get() != '\n');
}

// 랭킹을 표시하는 함수
static void show_ranking() {
    clear_screen();
    std::ifstream ranking_file("ranking.txt");
    std::string line;

    std::vector<std::tuple<std::string, int, std::time_t>> rankings;
    clear_screen();
    set_console_color(14);
    print_border(40);
    set_console_color(11);
    std::cout << "            Player Rankings           " << std::endl;
    set_console_color(14);
    print_border(40);

    set_console_color(7);
    if (ranking_file.is_open()) {
        bool has_content = false;
        while (getline(ranking_file, line)) {
            has_content = true;
            std::istringstream iss(line);
            std::string student_number, player_name;
            std::string score_str, points_str, timestamp_str;
            int score;
            std::time_t timestamp;

            if (std::getline(iss, student_number, ',') &&
                std::getline(iss, player_name, ':') &&
                (iss >> score >> points_str) &&
                std::getline(iss, timestamp_str)) {
                timestamp = std::stoll(timestamp_str);
                rankings.emplace_back(student_number + ", " + player_name, score, timestamp);
            }
        }
        ranking_file.close();

        if (!has_content) {
            std::cout << "No rankings available.\n";
        }
    }
    else {
        std::cout << "Failed to open ranking file.\n";
    }

    if (!rankings.empty()) {
        std::sort(rankings.begin(), rankings.end(), [](const auto& a, const auto& b) {
            if (std::get<1>(a) == std::get<1>(b)) return std::get<2>(a) < std::get<2>(b);
            return std::get<1>(a) > std::get<1>(b);
            });

        int rank = 1;
        for (const auto& entry : rankings) {
            std::cout << rank << ". " << std::get<0>(entry) << ": " << std::get<1>(entry) << " points\n";
            rank++;
        }
    }

    print_border(40);
    set_console_color(7);
    std::cout << "\nPress any key to return to the lobby...";
    while (std::cin.get() != '\n');
}

// 메인 로비 함수
static void lobby() {
    loading_animation();

    std::string student_number, player_name;
    std::set<std::string> used_words;
    std::set<std::string> completed_categories;
    int score = 0;
    int bonus_streak = 0;
    int attempts_left = 6;
    int skip_chances = 3;

    std::vector<std::string> menu_items = {
        "1. Play Game", "2. View Rules", "3. View Rankings", "4. Exit"
    };

    while (true) {
        int choice = navigate_menu(menu_items, "Welcome to Hangman Game");
        switch (choice) {
        case 0:
            if (!get_student_info(student_number, player_name)) {
                // ESC가 눌렸을 경우 즉시 로비로 돌아감
                break;
            }
            used_words.clear();
            completed_categories.clear();
            score = 0;
            bonus_streak = 0;
            attempts_left = 6;
            skip_chances = 3;

            while (true) {
                int category_choice = navigate_category_menu(used_words, completed_categories);
                std::string category;
                switch (category_choice) {
                case 0: category = "Animals"; break;
                case 1: category = "Fruits"; break;
                case 2: category = "Technology"; break;
                case 3: category = "Nature"; break;
                case 4: category = "Sports"; break;
                case 5: category = "Food"; break;
                case 6: category = "History"; break;
                case 7: category = "Medicine"; break;
                case 8: category = "Vehicles"; break;
                case 9: category = "Instruments"; break;
                default:
                    std::cout << "Invalid choice! Returning to lobby.\n";
                    continue;
                }

                if (completed_categories.find(category) != completed_categories.end()) {
                    std::cout << "You have already completed all the words in this category!\n";
                    continue;
                }

                bool word_guessed = play_hangman(student_number, player_name, category, used_words, attempts_left, score, bonus_streak, skip_chances);
                const auto& category_words = word_categories[category];
                bool all_words_used = true;
                for (const auto& word_info : category_words) {
                    if (used_words.find(std::get<0>(word_info)) == used_words.end()) {
                        all_words_used = false;
                        break;
                    }
                }

                if (all_words_used) {
                    completed_categories.insert(category);
                    std::cout << "\nYou have completed all the words in the " << category << " category!\n";
                }

                if (!word_guessed || attempts_left == 0) {
                    std::cout << "\nYour final score is: " << score << std::endl;
                    save_score(student_number, player_name, score);
                    used_words.clear();
                    completed_categories.clear();
                    score = 0;
                    bonus_streak = 0;
                    attempts_left = 6;
                    skip_chances = 3;
                    break;
                }
            }
            break;
        case 1:
            show_rules();
            break;
        case 2:
            show_ranking();
            break;
        case 3:
            std::cout << "Exiting game. Goodbye!\n";
            return;
        default:
            std::cout << "Invalid choice! Try again.\n";
        }
    }
}

// 메인 함수
int main() {
    // 이중 버퍼링 초기화
    initialize_double_buffer();

    // 환영 메시지 애니메이션 실행
    welcome_animation();


    lobby();  // 게임 로비 호출
    return 0;
}
