#include "game.h"
#include "tetromino.h"
#include "./console/console.h"
#include <string>
// 게임 판을 나타내는 배열
// board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
bool board_[BOARD_WIDTH][BOARD_HEIGHT];

Tetromino objectTetromino = Tetromino::I;
Tetromino originalTetromino = Tetromino::I;
Tetromino nextTetromino = Tetromino::I;

int xTetromino = BOARD_WIDTH/2;
int yTetromino = 1;
int dropTimer = DROP_DELAY;

// board_를 초기화한다.
void Game::setBoard() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            board_[i][j] = false;
        }
    }
}

// 위쪽 방향키를 누르면 테트로미노를 맨 아래로 이동한다. (상 방향키 입력)
void Game::hardDrop() {

}

// 테트로미노를 일정 간격으로 아래로 이동
void Game::softDrop() {
    dropTimer--;
    if (dropTimer == 0) {
        dropTimer = DROP_DELAY;
        yTetromino += 1;
    }
}

// board를 체크하여 가로 한 줄이 꽉 찼으면 삭제 clearLine에 1을 추가
void Game::isClearLine() {

}

// clearLine이 일정 개수에 도달하면 게임 종료 (gameClear = True)
void Game::clearGame() {

}

// 테트로미노를 시계, 반시계 방향으로 회전한다. (Z, X키 입력)
void Game::rotatedTetromino() {
    //moveTetromino()에 임시 통합 (정상 작동 확인)
}

// 테트로미노를 이동한다. (좌, 우, 하 방향키 입력)
void Game::moveTetromino() {
    if (console::key(console::K_LEFT)) {
        xTetromino -= 1;
    }
    if (console::key(console::K_RIGHT)) {
        xTetromino += 1;
    }
    if (console::key(console::K_UP)) {
        //
    }
    if (console::key(console::K_DOWN)) {
        yTetromino += 1;
    }
    if (console::key(console::K_Z)) {
        objectTetromino = objectTetromino.rotatedCW();
    }
    if (console::key(console::K_X)) {
        objectTetromino = objectTetromino.rotatedCCW();
    }
    if (console::key(console::K_SPACE)) {
        createTetromino();
    }
}

// 테트로미노를 생성한다.
void Game::createTetromino() {
    xTetromino = BOARD_WIDTH / 2;
    yTetromino = 1;
    int selectTetromino = rand() % 7;
    if (selectTetromino == 0) objectTetromino = Tetromino::I;
    else if (selectTetromino == 1) objectTetromino = Tetromino::O;
    else if (selectTetromino == 2) objectTetromino = Tetromino::T;
    else if (selectTetromino == 3) objectTetromino = Tetromino::S;
    else if (selectTetromino == 4) objectTetromino = Tetromino::Z;
    else if (selectTetromino == 5) objectTetromino = Tetromino::J;
    else if (selectTetromino == 6) objectTetromino = Tetromino::L;
}

// 게임의 한 프레임을 처리한다.
void Game::update() {
    softDrop();
    moveTetromino();
}

// 게임 화면을 그린다.
void Game::draw() {
    console::drawBox(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1);
    objectTetromino.drawAt(BLOCK_STRING, xTetromino, yTetromino);
    objectTetromino.drawAt(SHADOW_STRING, xTetromino, BOARD_HEIGHT-1);
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit() {
    bool gameOver = false;
    if (console::key(console::K_ESC)) {
        gameOver = true;
    }
    return gameOver; 
}

Game::Game() {
    setBoard();
    createTetromino();
}