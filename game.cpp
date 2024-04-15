#include "game.h"
#include "tetromino.h"
#include "./console/console.h"

// 게임 판을 나타내는 배열
// board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
bool board_[BOARD_WIDTH][BOARD_HEIGHT];

Tetromino I = Tetromino::I;
Tetromino O = Tetromino::O;
Tetromino T = Tetromino::T;
Tetromino S = Tetromino::S;
Tetromino Z = Tetromino::Z;
Tetromino J = Tetromino::J;
Tetromino L = Tetromino::L;

// 키를 입력받는다.
void handleInput() {
  if (console::key(console::K_LEFT)) {
    
  }
  if (console::key(console::K_RIGHT)) {
    
  }
  if (console::key(console::K_UP)) {
    
  }
  if (console::key(console::K_DOWN)) {
    
  }
  if (console::key(console::K_Z)) {
    
  }
  if (console::key(console::K_X)) {
    
  }
  if (console::key(console::K_SPACE)) {
    
  }
}

// 게임의 한 프레임을 처리한다.
void Game::update() {
    
}

// 게임 화면을 그린다.
void Game::draw() {
    console::drawBox(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1);
    I.drawAt(BLOCK_STRING, BOARD_WIDTH/2 , 1);
    Tetromino Icw = I.rotatedCW();
    Icw.drawAt(BLOCK_STRING, BOARD_WIDTH/2 , 6);
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
    

}