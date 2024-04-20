#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include "./console/console.h"
#include <string>
#include <ctime>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
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
  void printBoard();

  // objectTetromino가 board_의 true와 닿았다면 objectTetormino를 board에 쌓는다.
  void buildTetromino();

  // objectTetromino의 그림자의 Y값을 반환한다.
  int getShadowY();

  // objectTetromino가 바닥 혹은 board_의 true 부분에 닿는지 판단한다. 
  bool crashTetromino(int x, int y);

  // objectTetromino의 왼쪽 false 부분들을 보정해주는(무시하게 해주는) leftX를 반환한다.
  int crashLeftTetromino(Tetromino& tetromino);

  // objectTetromino의 오른쪽 false 부분들을 보정해주는(무시하게 해주는) rightX를 반환한다.
  int crashRightTetromino(Tetromino& tetromino);

  // board_를 초기화한다.
  void setBoard();
  
  // objectTetromino가 직선 상에 바로 쌓일 수 있게 한다.
  void hardDrop();

  // 테트로미노를 일정 간격으로 아래로 이동한다.
  void softDrop();

  // board를 체크하여 가로 한 줄이 꽉 찼으면 삭제 후 clearLine에 적용한다.
  void isClearLine();

  // board의 꽉 찬 가로 한 줄을 삭제한다.
  void clearLineOne(int boardH);

  // 게임의 승리 조건
  bool victoryGame();

  // 게임의 패배 조건
  bool overGame();

  // 입력받는 키로 테트로미노를 변화시킨다. (상, 하, 좌, 우 방향키/Z, X키/SPACE키 입력)
  // 하, 좌, 우 방향키 : objectTetromino의 이동
  // 상 방향키 : hardDrop() 실행
  // Z, X키 : objectTetromino를 시계 방향, 반시계 방향으로 회전
  // SPACE키 : hold(임시저장) 및 swap(교환) 기능
  void inputTetromino();

  // tetromino를 생성한다.
  void createTetromino(Tetromino& tetromino);

  // 게임의 진행 시간을 갱신한다.
  void checkTime();
  
public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif