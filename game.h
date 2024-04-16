#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include "./console/console.h"
#include <string>
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  // 생성한 테트로미노
  //Tetromino objectTetromino;
  // 생성한 테트리미노의 원본
  //Tetromino originalTetromino;
  // 다음에 생성할 테트리미노
  //Tetromino nextTetromino;
  // 생성한 테트로미노의 x 좌표
  //int xTetromino;
  // 생성한 테트로미노의 y 좌표
  //int yTetromino;

public:
  // board_를 초기화한다.
  void setBoard();
  
  // 위쪽 방향키를 누르면 테트로미노를 맨 아래로 이동한다. (상 방향키 입력)
  void hardDrop();

  // 테트로미노를 일정 간격으로 아래로 이동
  void softDrop();

  // board를 체크하여 가로 한 줄이 꽉 찼으면 삭제 clearLine에 1을 추가
  void isClearLine();

  // clearLine이 일정 개수에 도달하면 게임 종료 (gameClear = True)
  void clearGame();

  // 테트로미노를 시계, 반시계 방향으로 회전한다. (Z, X키 입력)
  void rotatedTetromino();

  // 테트로미노를 이동한다. (좌, 우, 하 방향키 입력)
  void moveTetromino();

  // 테트로미노를 생성한다.
  void createTetromino();

  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif