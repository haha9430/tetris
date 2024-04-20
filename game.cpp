#include "game.h"
#include "tetromino.h"
#include "./console/console.h"
#include <string>
#include <ctime>

// 게임 판을 나타내는 배열
// board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
bool board_[BOARD_WIDTH][BOARD_HEIGHT];

// 현재 게임판에서 플레이어가 움직이는 테트로미노
Tetromino objectTetromino = Tetromino::I;
// objectTetromino의 원본(rotated되지 않은 채로 저장)
Tetromino originalTetromino = Tetromino::I;
// objectTetromino의 다음 번 Tetromino
Tetromino nextTetromino = Tetromino::I;
// 임시 보관하는 Tetromino
Tetromino holdTetromino = Tetromino("X", 0, "");
// holdTetromino와 obejctTetromino를 교환할 때 사용되는 보관용 Tetromino
Tetromino saveTetromino = Tetromino("X", 0, "");
// obejectTetromino가 rotated된 상태를 저장할 Tetromino
Tetromino rotatedTetromino = Tetromino::I;
// holdTetromino와 nextTetromino를 저장할 박스의 가로
int smallBoxWidth = 5;
// holdTetromino와 nextTetromino를 저장할 박스의 세로
int smallBoxHeight = 6;
// nextTetromino를 담는 박스의 x 좌표
int nextBoardX = BOARD_WIDTH + 3;
// holdTetromino를 담는 박스의 x 좌표
int holdBoardX = nextBoardX + smallBoxWidth + 1;
// objectTetromino의 x 좌표
int xTetromino = BOARD_WIDTH/2;
// objectTetromino의 y 좌표
int yTetromino = 1;
// softDrop의 주기(DROP_DELAY)
int dropTimer = DROP_DELAY;
// 게임을 승리하기 위해 남은 줄 수(LINES)
int clearLine = LINES;
// hold가 처음인지 구별하기 위한 변수(0이면 두번째 이상)
int hold = 1;
// hold를 할 수 있는지 구별하기 위한 변수(0이면 hold 불가능)
int holdN = 1;
// 게임을 시작을 기준으로 지나는 시간을 출력하기 위한 string 변수
std::string curTime;
// 게임을 시작을 기준으로 지나는 시간
double duration = 0.0;

// board_에 쌓인 Tetromino(true)들을 출력한다.
void Game::printBoard() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (board_[i][j] == true) {
                console::draw(i+1, j+1, BLOCK_STRING);
            }
        }
    }
}


// objectTetromino가 board_의 true와 닿았다면 objectTetormino를 board에 쌓는다.
void Game::buildTetromino() {
    if (crashTetromino(xTetromino, yTetromino)) {
        for (int i = 0; i < objectTetromino.size(); i++) {
            for (int j = 0; j < objectTetromino.size(); j++) {
                if (objectTetromino.check(i, j)) {
                    board_[xTetromino + j - 1][yTetromino + i - 1] = true;
                }
            }
        }
        holdN = 1;
        objectTetromino = nextTetromino;
        originalTetromino = nextTetromino;
        createTetromino(nextTetromino);
    }
}

// objectTetromino의 그림자의 Y값을 반환한다.
int Game::getShadowY() {
    int shadowY = yTetromino;
    for (int i = shadowY; i < BOARD_HEIGHT - 1; i++) {
        if (crashTetromino(xTetromino, i + 1)) {
            shadowY = i + 1;
            break;
        }
    }
    return shadowY;
}

// objectTetromino가 바닥 혹은 board_의 true 부분에 닿는지 판단한다. 
bool Game::crashTetromino(int x, int y) {
    for (int i = 0; i < objectTetromino.size(); i++) {
        for (int j = 0; j < objectTetromino.size(); j++) {
            if (objectTetromino.check(i,j)) {
                int boardX = x + j;
                int boardY = y + i;

                if (boardY == BOARD_HEIGHT || board_[boardX - 1][boardY + 1 - 1]) {
                    return true;
                }
            }
        }
    }

    return false;
}

// objectTetromino의 왼쪽 false 부분들을 보정해주는(무시하게 해주는) leftX를 반환한다.
int Game::crashLeftTetromino(Tetromino& tetromino){
    int leftX = 0;
    int j = 0;
    bool checkTetromino;

    if (tetromino.size() == 4) {
        for (int i = 0; i < tetromino.size(); i++) {
            checkTetromino = (tetromino.check(j, i) == false && tetromino.check(j+1, i) == false) &&
                             (tetromino.check(j+2, i) == false && tetromino.check(j+3, i) == false);
            if (checkTetromino == true && (i == 0 || i == 1)) {
                leftX += 1;
            }   
        }
    } else if (tetromino.size() == 3) {
        for (int i = 0; i < tetromino.size(); i++) {
            checkTetromino = (tetromino.check(j, i) == false && tetromino.check(j+1, i) == false) &&
                              tetromino.check(j+2, i) == false;
            if (checkTetromino == true && (i == 0)) {
                leftX += 1;
            }
        }
    }

    return leftX;
}

// objectTetromino의 오른쪽 false 부분들을 보정해주는(무시하게 해주는) rightX를 반환한다.
int Game::crashRightTetromino(Tetromino& tetromino) {
    int rightX = 0;
    int j = 0;
    bool checkTetromino;

    if (tetromino.size() == 4) {
        for (int i = 0; i < tetromino.size(); i++) {
            checkTetromino = (tetromino.check(j, i) == false && tetromino.check(j+1, i) == false) &&
                             (tetromino.check(j+2, i) == false && tetromino.check(j+3, i) == false);
            if (checkTetromino == true && (i == 2 || i == 3)) {
                rightX += 1;
            }   
        }
    } else if (tetromino.size() == 3) {
        for (int i = 0; i < tetromino.size(); i++) {
            checkTetromino = (tetromino.check(j, i) == false && tetromino.check(j+1, i) == false) &&
                              tetromino.check(j+2, i) == false;
            if (checkTetromino == true && (i == 2)) {
                rightX += 1;
            }
        }
    }
    return rightX;
}

// board_를 초기화한다.
void Game::setBoard() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            board_[i][j] = false;
        }
    }
}

// objectTetromino가 직선 상에 바로 쌓일 수 있게 한다.
void Game::hardDrop() {
    yTetromino = getShadowY();
}

// 테트로미노를 일정 간격으로 아래로 이동한다.
void Game::softDrop() {
    dropTimer--;
    if (dropTimer == 0) {
        dropTimer = DROP_DELAY;
        yTetromino += 1;
    }
}

// board를 체크하여 가로 한 줄이 꽉 찼으면 삭제 후 clearLine에 적용한다.
void Game::isClearLine() {
    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool fullLine = true;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board_[j][i] == false) {
                fullLine = false;
                break;
            }
        }
        if (fullLine == true) {
            clearLineOne(i);
            clearLine -= 1;
        }
    }
}

// board의 꽉 찬 가로 한 줄을 삭제한다.
void Game::clearLineOne(int boardH) {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        board_[i][boardH] = false;
    }
    for (int i = boardH - 1; i >= 0; i--) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board_[j][i + 1] = board_[j][i];
            board_[j][i] = false;
        }
    }
}

// 게임의 승리 조건
bool Game::victoryGame() {
    bool victory = false;
    if (clearLine <= 0) { victory = true; }
    return victory;
}

// 게임의 패배 조건
bool Game::overGame() {
    bool over = false;
    int checkBoardY = 0;
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (board_[i][0] == true) {
                over = true;
            }
        }
    }
    return over;
}

// 입력받는 키로 테트로미노를 변화시킨다. (상, 하, 좌, 우 방향키/Z, X키/SPACE키 입력)
// 하, 좌, 우 방향키 : objectTetromino의 이동
// 상 방향키 : hardDrop() 실행
// Z, X키 : objectTetromino를 시계 방향, 반시계 방향으로 회전
// SPACE키 : hold(임시저장) 및 swap(교환) 기능
void Game::inputTetromino() {
    if (console::key(console::K_LEFT)) {
        if (xTetromino > -(crashLeftTetromino(objectTetromino)) + 1) {
            xTetromino -= 1;
        }
    }
    if (console::key(console::K_RIGHT)) {
        if (xTetromino <= BOARD_WIDTH - objectTetromino.size() + (crashRightTetromino(objectTetromino))) {
            xTetromino += 1;
        }
    }
    if (console::key(console::K_UP)) {
        hardDrop();
    }
    if (console::key(console::K_DOWN)) {
        yTetromino += 1;
    }
    if (console::key(console::K_Z)) {
        rotatedTetromino = objectTetromino.rotatedCW();
        bool CW = xTetromino > -(crashLeftTetromino(rotatedTetromino)) && xTetromino <= BOARD_WIDTH - objectTetromino.size() + (crashRightTetromino(rotatedTetromino)) + 1;
        if (CW) {
            objectTetromino = rotatedTetromino;
        }
    }
    if (console::key(console::K_X)) {
        rotatedTetromino = objectTetromino.rotatedCCW();
        if (xTetromino > -(crashLeftTetromino(rotatedTetromino))) {
            objectTetromino = rotatedTetromino;
        }
        if (xTetromino <= BOARD_WIDTH - objectTetromino.size() + (crashRightTetromino(rotatedTetromino)) + 1) {
            objectTetromino = rotatedTetromino;
        }
    }
    if (console::key(console::K_SPACE)) {
        if (hold == 1 && holdN == 1) {
            holdTetromino = originalTetromino;
            objectTetromino = nextTetromino;
            originalTetromino = nextTetromino;
            createTetromino(nextTetromino);
            holdN -= 1;
            hold -= 1;
        }else if (hold == 0 && holdN == 1) {
            saveTetromino = originalTetromino;
            objectTetromino = holdTetromino;
            holdTetromino = saveTetromino;
            xTetromino = BOARD_WIDTH / 2;
            yTetromino = 1;
            holdN -= 1;
        }
    }
}

// tetromino를 생성한다.
void Game::createTetromino(Tetromino& tetromino) {
    xTetromino = BOARD_WIDTH / 2;
    yTetromino = 1;
    int selectTetromino = rand() % 7;
    if (selectTetromino == 0) tetromino = Tetromino::I;
    else if (selectTetromino == 1) tetromino = Tetromino::O;
    else if (selectTetromino == 2) tetromino = Tetromino::T;
    else if (selectTetromino == 3) tetromino = Tetromino::S;
    else if (selectTetromino == 4) tetromino = Tetromino::Z;
    else if (selectTetromino == 5) tetromino = Tetromino::J;
    else if (selectTetromino == 6) tetromino = Tetromino::L;
}

// 게임의 진행 시간을 갱신한다.
void Game::checkTime() {
    duration += 1.0 / 60.0;
    
    int minute = static_cast<int>(duration / 60);
    int second = static_cast<int>(duration) % 60;
    int millisecond = static_cast<int>((duration - static_cast<int>(duration)) * 100);

    curTime = std::to_string(minute / 10) + std::to_string(minute % 10) + ":" +
              std::to_string(second / 10) + std::to_string(second % 10) + ":" +
              std::to_string(millisecond / 10) + std::to_string(millisecond % 10);
}

// 게임의 한 프레임을 처리한다.
void Game::update() {
    softDrop();
    inputTetromino();
    buildTetromino();
    isClearLine();
    checkTime();
}



// 게임 화면을 그린다.
void Game::draw() {
    console::drawBox(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1);
    console::drawBox(nextBoardX, 0, nextBoardX + smallBoxWidth, smallBoxHeight);
    console::drawBox(holdBoardX, 0, holdBoardX + smallBoxWidth, smallBoxHeight);
    console::draw(nextBoardX + 1, 0, "Next");
    console::draw(holdBoardX + 1, 0, "Hold");
    printBoard();
    objectTetromino.drawAt(SHADOW_STRING, xTetromino, getShadowY());
    objectTetromino.drawAt(BLOCK_STRING, xTetromino, yTetromino);
    nextTetromino.drawAt(BLOCK_STRING, nextBoardX + 1, smallBoxHeight / 2 - nextTetromino.size() / 2);
    holdTetromino.drawAt(BLOCK_STRING, holdBoardX + 1, smallBoxHeight / 2 - holdTetromino.size() / 2);
    //originalTetromino.drawAt(BLOCK_STRING, holdBoardX + 1, smallBoxHeight / 2 - originalTetromino.size() / 2);
    console::draw(0, BOARD_HEIGHT + 2, std::to_string(clearLine) + " lines left");
    console::draw((BOARD_WIDTH + 1) / 2 - curTime.size() / 2 + 1, BOARD_HEIGHT + 3, curTime);
    if (victoryGame()) {
        const std::string m = "You win";
        console::draw((BOARD_WIDTH + 1) / 2 - m.size() / 2 + 1, BOARD_HEIGHT / 2, m);
        console::draw((BOARD_WIDTH + 1) / 2 - curTime.size() / 2 + 1, BOARD_HEIGHT / 2 + 1, curTime);
    }
    if (overGame()) {
        const std::string m = "You lost";
        console::draw((BOARD_WIDTH + 1) / 2 - m.size() / 2 + 1, BOARD_HEIGHT / 2, m);     
    }
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit() {
    bool gameOver = false;
    if (victoryGame() || overGame()) {
        gameOver = true;
    }
    if (console::key(console::K_ESC)) {
        gameOver = true;
    }
    return gameOver; 
}

Game::Game() {
    setBoard();
    createTetromino(nextTetromino);
    createTetromino(objectTetromino);
    originalTetromino = objectTetromino;
}