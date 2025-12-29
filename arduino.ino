#include "move_table.h"
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int btnBack = 7, btnSelect = 8, btnNext = 9;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int cursor = 0;
bool playerTurn = false;

void setup() {
  pinMode(btnBack, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  pinMode(btnNext, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.print("AI Tic-Tac-Toe");
  delay(1000);
  // seed PRNG (floating analog pin gives some entropy)
  randomSeed(analogRead(A0));
  drawBoard();
}

int checkWinner() {
  int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
  for (int i = 0; i < 8; i++) {
    if (board[wins[i][0]] != 0 && 
        board[wins[i][0]] == board[wins[i][1]] && 
        board[wins[i][0]] == board[wins[i][2]]) {
      return board[wins[i][0]];
    }
  }
  for (int i = 0; i < 9; i++) if (board[i] == 0) return 0;
  return 3; // Draw
}

void drawBoard() {
  lcd.clear();
  renderSegment(0, 0, 0);         
  renderSegment(3, 0, 1); 
  renderSegment(6, 7, 0);
}

void renderSegment(int startIdx, int x, int y) {
  lcd.setCursor(x, y);
  for (int i = 0; i < 3; i++) {
    int currentIdx = startIdx + i;
    char icon = getChar(board[currentIdx]);
    if (currentIdx == cursor && board[currentIdx] == 0) icon = '*';
    lcd.print(icon);
    if (i < 2) lcd.print(" ");
  }
}

char getChar(int val) {
  if (val == 1) return 'X';
  if (val == 2) return 'O';
  return '.';
}

int getPossibleMoves(int possible[]) {
  int count = 0;
  for (int i = 0; i < 9; i++) {
    if (board[i] == 0) possible[count++] = i;
  }
  return count;
}

void flipBoardMarkers() {
    for (int i = 0; i < 9; i++) {
        if (board[i] == 1) board[i] = 2;
        else if (board[i] == 2) board[i] = 1;
    }
}

int getAIMove(int aiMarker) {
    int possible[9];
    int possible_count = getPossibleMoves(possible);
    if (possible_count == 0) return 0;

    int best_moves[9];
    int best_count = 0;
    int8_t best_value = INT8_MIN;

    if (aiMarker != 1) flipBoardMarkers();
        
    for (int pi = 0; pi < possible_count; pi++) {
        int m = possible[pi];
        board[m] = 1;

        // compute base-3 hash of the resulting board
        unsigned int hash = 0;
        unsigned int p3 = 1;
        for (int i = 0; i < 9; i++) {
        hash += (unsigned int)board[i] * p3;
        p3 *= 3;
        }

        // read stored value from PROGMEM
        uint8_t raw = pgm_read_byte(&(moveTable[hash]));
        int8_t val;
        if (raw == 255) {
        // 255 denotes "no data" in the table; treat as very bad
        val = INT8_MIN;
        } else {
        val = (int8_t)raw;
        }

        // undo move
        board[m] = 0;

        // track best moves (maximize for AI)
        if (val > best_value) {
            best_value = val;
            best_count = 0;
            best_moves[best_count++] = m;
        } else if (val == best_value) {
        best_moves[best_count++] = m;
        }
    }

    if (aiMarker != 1) flipBoardMarkers();

    // if no best move found (shouldn't happen), pick first possible
    if (best_count == 0) return possible[0];

    // choose a random move among the best
    int choice = (int)random(best_count);
    return best_moves[choice];
}

void endGame(int winner) {
  lcd.setCursor(7, 1);
  if (winner == 1) lcd.print("AI WINS!");
  else if (winner == 2) lcd.print("YOU WIN!");
  else lcd.print("DRAW!");
  for (int i = 0; i < 9; i++) board[i] = 0;
  playerTurn = false;
  delay(3000);
}

void makePlayerMove(int marker) {
bool move = false;
    while(!move){
        if (digitalRead(btnNext) == LOW) {
            cursor = (cursor + 1) % 9;
            drawBoard();
            while(digitalRead(btnNext) == LOW);
            delay(50); 
        }
        if (digitalRead(btnBack) == LOW) {
            cursor = (cursor + 8) % 9;
            drawBoard();
            while(digitalRead(btnBack) == LOW);
            delay(50);
        }
        if (digitalRead(btnSelect) == LOW) {
            if (board[cursor] == 0) {
                board[cursor] = marker;
                move = true;
            }
            while(digitalRead(btnSelect) == LOW);
            delay(50);
        }
    }
}

int playerMarker = 2;
int aiMarker = 1;
void loop() {
    if(playerTurn) {
      makePlayerMove(playerMarker);
      playerTurn = false;
    } 
    else {
        int aiMove = getAIMove(aiMarker);
        board[aiMove] = aiMarker;
        playerTurn = true;
    }
    drawBoard();

    int winStatus = checkWinner();
    if (winStatus != 0) endGame(winStatus);
}
