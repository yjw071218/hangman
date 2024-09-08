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

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;

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
        {"lion", "사자의 왕으로 불리는 육식성 동물", "medium"}
    }},
    {"Fruits", {
        {"apple", "나무에서 자라며 다양한 색을 가진 둥근 과일", "easy"},
        {"banana", "열대 식물의 길고 노란 과일", "easy"},
        {"orange", "두꺼운 껍질을 가진 감귤류 과일", "easy"},
        {"grape", "작고 둥글며 포도나무에서 자라는 과일", "easy"},
        {"mango", "달콤하고 즙이 많은 열대 과일", "medium"},
        {"peach", "털이 있는 껍질을 가진 여름철 과일", "easy"},
        {"plum", "달콤하고 씨가 있는 둥근 과일", "medium"},
        {"pear", "부드러운 과육을 가진 원뿔형 과일", "medium"},
        {"kiwi", "작고 갈색 털이 있는 녹색 속의 과일", "medium"},
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
        {"anesthesia", "수술 중 고통을 줄이기 위한 방법", "exteme"},
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




// 콘솔 텍스트 색상을 설정하는 함수
static void set_console_color(int text_color, int bg_color = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text_color + (bg_color << 4));
}

// 콘솔 화면 초기화 함수
static void clear_screen() {
    system("cls");  // 콘솔 창 지우기 (Windows)
}

// 콘솔 화면 크기 얻기 함수
static COORD get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        COORD console_size;
        console_size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        console_size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return console_size;
    }
    else {
        // 기본 값을 반환 (에러 처리)
        COORD default_size = { 80, 25 }; // 일반적인 기본 콘솔 크기
        return default_size;
    }
}

// 테두리를 출력하는 함수
static void print_border(int length, char symbol = '=') {
    // 콘솔 너비에 맞게 테두리 출력
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

// 행맨 그림을 출력하는 함수
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
        set_console_color(attempts_left > 0 ? 10 : 12);  // 초록색(10) 또는 빨간색(12)으로 설정
        print_at_position(start_x, start_y + i, hangman_lines[i]);
    }
    set_console_color(7);  // 기본 색상으로 복원
}


// 콘솔 커서 숨기기 함수
void hide_cursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// 메뉴 항목을 탐색할 수 있는 함수
int navigate_menu(const std::vector<std::string>& menu_items, const std::string& title) {
    int selected_index = 0;
    int menu_size = menu_items.size();

    hide_cursor(); // 커서 숨기기

    while (true) {
        // 화면을 지우고 메뉴를 다시 출력
        clear_screen();

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
                std::cout << ">> " << menu_items[i] << std::endl;
            }
            else {
                set_console_color(7);  // 선택되지 않은 항목은 기본 색상
                std::cout << "   " << menu_items[i] << std::endl;
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

// 텍스트를 우측 상단에 출력하는 함수
static void print_game_info(int attempts_left, const std::string& current_word, const std::set<char>& wrong_guesses, const std::string& difficulty) {
    COORD console_size = get_console_size();
    int hint_position_x = 30;
    int starting_y = 3;

    set_console_color(14);  // 노란색으로 남은 시도 강조
    print_at_position(hint_position_x, starting_y + 2, "Attempts left: " + std::to_string(attempts_left));

    set_console_color(11);  // 파란색으로 현재 단어 상태 강조
    print_at_position(hint_position_x, starting_y + 3, "Current word: " + current_word);

    set_console_color(12);  // 빨간색으로 난이도 강조
    print_at_position(hint_position_x, starting_y + 4, "Difficulty: " + difficulty);

    set_console_color(13);  // 자주색으로 잘못된 추측 강조
    std::string wrong_guesses_text = "Wrong guesses: ";
    for (char c : wrong_guesses) {
        wrong_guesses_text += c;
        wrong_guesses_text += ' ';
    }
    print_at_position(hint_position_x, starting_y + 5, wrong_guesses_text);
    set_console_color(7);  // 기본 색상으로 복원
}

// 단어에서 밑줄을 제거하여 공백으로 변환
static std::string prepare_word(const std::string& word) {
    std::string prepared_word;
    for (char letter : word) {
        if (letter != '_') {
            prepared_word += letter;  // 밑줄이 아닌 글자만 추가
        }
    }
    return prepared_word;
}

// 현재 단어 상태 표시 함수 (밑줄 대신 공백으로 표시)
static std::string get_display_word(const std::string& word, const std::set<char>& correct_guesses) {
    std::string display_word;
    for (char letter : word) {
        if (letter == '_') {
            display_word += ' ';  // 밑줄을 공백으로 표시
        }
        else if (correct_guesses.find(letter) != correct_guesses.end()) {
            display_word += letter;  // 맞춘 글자는 그대로 표시
        }
        else {
            display_word += '_';  // 맞추지 못한 글자는 밑줄로 표시
        }
        display_word += ' ';  // 각 글자 사이에 공백 추가
    }
    return display_word;
}

// 랜덤 단어 선택 함수 (카테고리 기반)
static std::tuple<std::string, std::string> get_random_word(const std::string& category, std::set<std::string>& used_words) {
    srand(static_cast<unsigned int>(time(0)));
    const auto& category_words = word_categories[category];

    // 사용하지 않은 단어 중에서 선택
    std::vector<std::tuple<std::string, std::string, std::string>> available_words;
    for (const auto& word_info : category_words) {
        if (used_words.find(std::get<0>(word_info)) == used_words.end()) {
            available_words.push_back(word_info);
        }
    }

    if (available_words.empty()) {
        return std::make_tuple("", ""); // 사용할 수 있는 단어가 없을 때
    }

    int index = rand() % available_words.size();
    used_words.insert(std::get<0>(available_words[index]));
    return std::make_tuple(std::get<0>(available_words[index]), std::get<2>(available_words[index])); // 단어와 난이도를 반환
}


// 단어에 대한 힌트 제공 함수
static std::string get_hint(const std::string& category, const std::string& word) {
    for (const auto& word_info : word_categories[category]) {
        if (std::get<0>(word_info) == word) {
            return std::get<1>(word_info); // 단어에 대한 힌트 반환
        }
    }
    return ""; // 단어가 없을 경우 빈 문자열 반환
}

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
    if (difficulty == "easy") {
        return 1000;
    }
    else if (difficulty == "medium") {
        return 1500;
    }
    else if (difficulty == "hard") {
        return 2000;
    }
    else if (difficulty == "extreme") {
        return 4000;
    }
    return 0; // Invalid difficulty returns 0
}


// 커서 위치 바로 밑에 출력하는 pause 함수
static void pause() {
    // 콘솔 화면에서 현재 커서 위치 얻기
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    // 현재 커서 위치 가져오기
    int current_cursor_x = csbi.dwCursorPosition.X;
    int current_cursor_y = csbi.dwCursorPosition.Y;

    // 커서 위치 바로 밑에 "계속하려면 아무키나 누르십시오..." 메시지 출력
    print_at_position(current_cursor_x, current_cursor_y + 1, "계속하려면 아무키나 누르십시오...");

    // 사용자로부터 입력을 받음 (두 번 입력 받는 이유는 버퍼 처리를 위해)
    std::cin.get();
    std::cin.get();
}

static bool play_hangman(const std::string& student_number, const std::string& player_name, const std::string& category, std::set<std::string>& used_words, int& attempts_left, int& score, int& bonus_streak, int& skip_chances) {
    clear_screen();

    // 단어와 난이도를 선택
    std::string original_word, difficulty;
    std::tie(original_word, difficulty) = get_random_word(category, used_words);
    std::string word = prepare_word(original_word);

    // 더 이상 사용할 단어가 없을 경우 종료
    if (word.empty()) {
        std::cout << "No more words available in this category!\n";
        return false;
    }

    // 힌트와 단어의 상태 초기화
    std::set<char> correct_guesses;
    std::set<char> wrong_guesses;
    std::string display_word = get_display_word(word, correct_guesses);
    std::string hint = get_hint(category, original_word);

    // 시도 횟수와 스킵 기회를 활용한 게임 진행
    while (attempts_left > 0) {
        clear_screen();
        print_hangman(attempts_left);
        int hint_position_x = 30;
        print_at_position(hint_position_x, 0, "Category: " + category);
        print_at_position(hint_position_x, 1, "Hint: " + hint);
        display_word = get_display_word(word, correct_guesses);
        print_game_info(attempts_left, display_word, wrong_guesses, difficulty); // 난이도 출력
        print_at_position(hint_position_x, 5, "Skip chances left: " + std::to_string(skip_chances)); // 스킵 기회 출력

        // 사용자의 입력을 받음
        char guess;
        COORD console_size = get_console_size();
        int input_position_y = 10;
        print_at_position(hint_position_x, input_position_y, "Guess a letter or type 'skip': ");
        std::string input;
        std::cin >> input;

        // 스킵 기능 처리
        if (input == "skip") {
            if (skip_chances > 0) {
                skip_chances--; // 스킵 기회 소모
                set_console_color(14);  // 노란색으로 텍스트 설정
                std::cout << "You skipped the word! The correct word was: " << word << "\n";
                set_console_color(7);  // 기본 색상으로 복원
                pause();  // 사용자 입력 대기 (텍스트 아래에 출력)
                return true; // 스킵 후 다음 단어로 진행
            }
            else {
                set_console_color(12);  // 빨간색으로 텍스트 설정
                std::cout << "No skip chances left!\n";
                set_console_color(7);  // 기본 색상으로 복원
                pause();  // 사용자 입력 대기 (텍스트 아래에 출력)
                continue;
            }
        }

        // 사용자가 입력한 단어가 유효한지 확인
        if (input.length() == 1) {
            guess = tolower(input[0]);
        }
        else {
            std::cout << "Please enter a valid letter or 'skip'.\n";
            continue;
        }

        // 이미 추측한 문자인지 확인
        if (correct_guesses.find(guess) != correct_guesses.end() || wrong_guesses.find(guess) != wrong_guesses.end()) {
            std::cout << "You already guessed that letter!\n";
            pause();  // 사용자 입력 대기 (텍스트 아래에 출력)
            continue;
        }

        // 단어에 맞는 문자인지 확인
        if (word.find(guess) != std::string::npos) {
            correct_guesses.insert(guess);
            bonus_streak++; // 연속으로 맞춘 경우 보너스 스트릭 증가
            score += bonus_streak * 100; // 보너스 점수 추가
        }
        else {
            wrong_guesses.insert(guess);
            attempts_left--;
            bonus_streak = 0; // 틀렸을 때 보너스 스트릭 초기화
        }

        // 단어가 완성되었는지 확인
        bool is_word_complete = true;
        for (char letter : word) {
            if (correct_guesses.find(letter) == correct_guesses.end() && letter != ' ') {
                is_word_complete = false;
                break;
            }
        }

        // 단어를 맞췄을 때
        if (is_word_complete) {
            clear_screen();
            print_hangman(attempts_left);
            print_game_info(attempts_left, display_word, wrong_guesses, difficulty);

            set_console_color(10);  // 초록색으로 축하 메시지 출력
            std::cout << "\n\nCongratulations, " << player_name << "! You've guessed the word: ";
            set_console_color(11);  // 파란색으로 맞춘 단어 강조
            std::cout << word << std::endl;

            set_console_color(7);  // 기본 색상으로 복원
            int points = get_score_for_difficulty(difficulty);
            score += points;

            std::cout << "\nPress Enter to continue...";
            while (std::cin.get() != '\n');
            std::cin.get();
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

        // "계속하려면 아무 키나 누르십시오..." 메시지를 점수 출력 바로 밑에 출력
        print_at_position(30, 7, "");

        // 사용자로부터 입력을 기다림
        pause();  // 입력을 기다리는 pause 함수 호출

        return false;  // 게임 오버 시 false 반환
    }

    return true;
}



// 로비 화면 출력 함수
static void display_lobby() {
    clear_screen();

    set_console_color(14);  // 노란색 텍스트
    print_border(40);  // 상단 테두리
    set_console_color(11);  // 파란색 텍스트
    std::cout << "        Welcome to Hangman Game        " << std::endl;
    set_console_color(14);  // 노란색 텍스트
    print_border(40);  // 하단 테두리

    set_console_color(7);  // 기본 흰색 텍스트
    std::cout << "1. Play Game\n";
    std::cout << "2. View Rules\n";
    std::cout << "3. View Rankings\n";
    std::cout << "4. Exit\n";
    print_border(40);  // 메뉴 하단에 구분선 추가
}

// 게임 규칙을 표시하는 함수
static void show_rules() {
    clear_screen();
    set_console_color(14);  // 노란색 텍스트
    print_border(40);  // 상단 테두리
    set_console_color(11);  // 파란색 텍스트
    std::cout << "             Game Rules            " << std::endl;
    set_console_color(14);  // 노란색 텍스트
    print_border(40);  // 하단 테두리

    set_console_color(7);  // 기본 흰색 텍스트
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
    std::cout << "   - 글자를 맞출 때마다 bonus_streak 변수가 1씩 증가\n";
    std::cout << "   - 맞출 때마다 bonus_streak * 100 점수가 추가\n";
    std::cout << "   - 틀린 글자를 입력하면 bonus_streak이 0으로 초기화\n";
    print_border(40);  // 하단 구분선 추가

    set_console_color(7);  // 기본 색상 복원
    std::cout << "\nPress any key to return to the lobby...";
    while (std::cin.get() != '\n');  // '\n'이 나올 때까지 계속 버퍼를 비움
}

// 랭킹을 표시하는 함수
static void show_ranking() {
    clear_screen();
    std::ifstream ranking_file("ranking.txt");
    std::string line;

    std::vector<std::tuple<std::string, int, std::time_t>> rankings;
    clear_screen();
    set_console_color(14);  // 노란색 텍스트
    print_border(40);  // 상단 테두리
    set_console_color(11);  // 파란색 텍스트
    std::cout << "           Player Rankings           " << std::endl;
    set_console_color(14);  // 노란색 텍스트
    print_border(40);  // 하단 테두리

    set_console_color(7);  // 기본 흰색 텍스트

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
        // 점수를 기준으로 정렬 (점수가 같으면 날짜가 오래된 순으로 정렬)
        std::sort(rankings.begin(), rankings.end(), [](const auto& a, const auto& b) {
            if (std::get<1>(a) == std::get<1>(b)) {
                return std::get<2>(a) < std::get<2>(b);  // 점수가 같으면 시간으로 정렬
            }
            return std::get<1>(a) > std::get<1>(b);  // 점수로 정렬
            });

        // 순위를 표시하며 출력
        int rank = 1;
        for (const auto& entry : rankings) {
            std::cout << rank << ". " << std::get<0>(entry) << ": " << std::get<1>(entry) << " points\n";
            rank++;
        }
    }

    print_border(40);  // 하단 구분선 추가

    set_console_color(7);  // 기본 색상 복원
    std::cout << "\nPress any key to return to the lobby...";
    while (std::cin.get() != '\n');  // '\n'이 나올 때까지 계속 버퍼를 비움
}


static int navigate_category_menu(const std::set<std::string>& used_words, const std::set<std::string>& completed_categories) {
    std::vector<std::string> categories = {
        "1. Animals",
        "2. Fruits",
        "3. Technology",
        "4. Nature",
        "5. Sports",
        "6. Food",
        "7. History",
        "8. Medicine",
        "9. Vehicles",
        "10.Instruments"
    };

    // 각 카테고리의 남은 단어 수와 난이도별 단어 수를 계산
    std::vector<std::string> category_info;
    for (const auto& category : categories) {
        std::string category_name = category.substr(3);  // "1. " 등 숫자 제거 후 실제 카테고리 이름 추출
        const auto& words = word_categories[category_name];

        int easy_count = 0, medium_count = 0, hard_count = 0, extreme_count = 0, remaining_words = 0;

        // 사용되지 않은 단어를 기준으로 난이도별 단어 수를 계산
        for (const auto& word_info : words) {
            const std::string& word = std::get<0>(word_info);
            const std::string& difficulty = std::get<2>(word_info);

            if (used_words.find(word) == used_words.end()) {
                remaining_words++;
                if (difficulty == "easy") {
                    easy_count++;
                }
                else if (difficulty == "medium") {
                    medium_count++;
                }
                else if (difficulty == "hard") {
                    hard_count++;
                }
                else if (difficulty == "extreme") {
                    extreme_count++;
                }
            }
        }

        // 카테고리 정보에 남은 단어 수와 난이도별 단어 수를 간결하게 표시
        std::stringstream ss;
        ss << category_name << " [" << remaining_words << " words] | "
            << "E: " << easy_count << " M: " << medium_count << " H: " << hard_count << " EX: " << extreme_count;

        // 완료된 카테고리는 회색으로 표시
        if (completed_categories.find(category_name) != completed_categories.end()) {
            ss << " (Completed)";
        }
        category_info.push_back(ss.str());
    }

    // 메뉴 탐색 시 완료된 카테고리는 선택할 수 없도록 처리
    while (true) {
        int choice = navigate_menu(category_info, "   Select a Category");
        std::string category_name = categories[choice].substr(3);  // 숫자를 제외한 카테고리 이름 추출

        if (completed_categories.find(category_name) != completed_categories.end()) {
            std::cout << "You have already completed all the words in this category! Please select another category.\n";
            continue;  // 이미 완료된 카테고리면 다시 선택을 요청
        }
        return choice;
    }
}

// 메인 로비 함수
static void lobby() {
    std::string student_number, player_name;
    std::set<std::string> used_words;
    std::set<std::string> completed_categories;
    int score = 0;
    int bonus_streak = 0;
    int attempts_left = 6;  // 목숨을 유지하기 위한 초기화
    int skip_chances = 3; // 스킵 기회 3번

    std::vector<std::string> menu_items = {
        "1. Play Game",
        "2. View Rules",
        "3. View Rankings",
        "4. Exit"
    };

    while (true) {
        int choice = navigate_menu(menu_items, "Welcome to Hangman Game");  // 타이틀 인수 추가

        switch (choice) {
        case 0:  // Play Game 선택
            std::cout << "Enter your student number: ";
            std::cin >> student_number;
            std::cout << "Enter your name: ";
            std::cin >> player_name;

            // 게임 시작 시 전체 진행 상태 초기화
            used_words.clear();
            completed_categories.clear();
            score = 0;
            bonus_streak = 0;
            attempts_left = 6;
            skip_chances = 3;

            while (true) {
                // 카테고리 선택 시 화살표 키 탐색 가능, 타이틀을 "Select a Category"로 변경
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

                // 이미 완료된 카테고리인지 확인
                if (completed_categories.find(category) != completed_categories.end()) {
                    std::cout << "You have already completed all the words in this category!\n";
                    continue;
                }

                // Hangman 게임 시작
                bool word_guessed = play_hangman(student_number, player_name, category, used_words, attempts_left, score, bonus_streak, skip_chances);

                // 카테고리의 모든 단어가 사용되었는지 확인
                const auto& category_words = word_categories[category];
                bool all_words_used = true;
                for (const auto& word_info : category_words) {
                    if (used_words.find(std::get<0>(word_info)) == used_words.end()) {
                        all_words_used = false;
                        break;
                    }
                }

                // 모든 단어가 사용된 경우 해당 카테고리를 완료된 목록에 추가
                if (all_words_used) {
                    completed_categories.insert(category);
                    std::cout << "\nYou have completed all the words in the " << category << " category!\n";
                }

                // 게임 오버일 경우 목숨 초기화
                if (!word_guessed || attempts_left == 0) {
                    std::cout << "\nYour final score is: " << score << std::endl;
                    save_score(student_number, player_name, score);  // 점수 저장

                    // 모든 상태 초기화
                    used_words.clear();
                    completed_categories.clear();
                    score = 0;
                    bonus_streak = 0;
                    attempts_left = 6;  // 게임 오버 시 목숨 초기화
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


int main() {
    lobby();  // 게임 로비 호출
    return 0;
}
