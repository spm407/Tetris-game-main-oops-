#include <bits/stdc++.h> 
#include <conio.h>       
#include <windows.h>     

using namespace std;

int dropSpeed = 700;
const int WIDTH = 15;
const int HEIGHT = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos = {0, 0};


struct Tetromino {
    vector<vector<vector<int>>> rotations; 
    string emoji; 
    int rotationState; 

    Tetromino(vector<vector<vector<int>>> r, string e) : rotations(r), emoji(e), rotationState(0) {}

    void rotate() {
        rotationState = (rotationState + 1) % rotations.size(); 
        // rotationState = (rotationState - 1 + rotations.size()) % rotations.size();
    }

    vector<vector<int>> getShape() {
        return rotations[rotationState];
    }
};

// Tetromino Definitions
vector<Tetromino> shape = {
    {{{{1}, {1}, {1}, {1}}, {{1, 1, 1, 1}}}, "🟦"}, 
    {{{{1, 1}, {1, 1}}}, "🟨"},                     
    {{{{1, 1, 1}, {0, 1, 0}}, 
      {{0, 1}, {1, 1}, {0, 1}}, 
      {{0, 1, 0}, {1, 1, 1}}, 
      {{1, 0}, {1, 1}, {1, 0}}}, "🟪"},             
    {{{{0, 1, 1}, {1, 1, 0}}, 
      {{1, 0}, {1, 1}, {0, 1}}}, "🟩"},             
    {{{{1, 1, 0}, {0, 1, 1}}, 
      {{0, 1}, {1, 1}, {1, 0}}}, "🟥"},             
    {{{{0, 1}, {0, 1}, {1, 1}}, 
      {{1, 0, 0}, {1, 1, 1}}, 
      {{1, 1}, {1, 0}, {1, 0}}, 
      {{1, 1, 1}, {0, 0, 1}}}, "🟫"},               
    {{{{1, 0}, {1, 0}, {1, 1}}, 
      {{1, 1, 1}, {1, 0, 0}}, 
      {{1, 1}, {0, 1}, {0, 1}}, 
      {{0, 0, 1}, {1, 1, 1}}}, "🟧"}                // L piece
};

vector<vector<string>> board(HEIGHT, vector<string>(WIDTH, "⬜"));
Tetromino currentPiece = shape[0]; 
Tetromino nextPiece = shape[0];    
int pieceX = WIDTH / 2 - 1;
int pieceY = 0;
int score = 0;

void moveCursor(int x, int y) {
    cursorPos.X = x;
    cursorPos.Y = y;
    SetConsoleCursorPosition(hConsole, cursorPos);
}

void displayBoard() {
    system("cls");
    moveCursor(0, 0);
    
    ostringstream output;
    
    
    output << "Next Piece:\n";
    vector<vector<int>> nextShape = nextPiece.getShape();
    for (int i = 0; i < nextShape.size(); i++) {
        for (int j = 0; j < nextShape[i].size(); j++) {
            if (nextShape[i][j] == 1)
                output << nextPiece.emoji;
            else
                output << "  "; 
        }
        output << "\n";
    }
    output << "\n";
    
   
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == "⬜" &&
                pieceY <= i && i < pieceY + currentPiece.getShape().size() &&
                pieceX <= j && j < pieceX + currentPiece.getShape()[0].size() &&
                currentPiece.getShape()[i - pieceY][j - pieceX] == 1) {
                output << currentPiece.emoji;
            } else {
                output << board[i][j];
            }
        }
        output << "\n";
    }
    
    output << "Score: " << score << "\n";
    
    cout << output.str();
}

bool isValidMove(Tetromino &piece, int x, int y) {
    vector<vector<int>> shape = piece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                int newX = x + j, newY = y + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || board[newY][newX] != "⬜") {
                    return false;
                }
            }
        }
    }
    return true;
}

void placePiece() {
    vector<vector<int>> shape = currentPiece.getShape();
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] == 1) {
                board[pieceY + i][pieceX + j] = currentPiece.emoji;
            }
        }
    }
}

void clearLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        if (count(board[i].begin(), board[i].end(), "⬜") == 0) {
            board.erase(board.begin() + i);
            board.insert(board.begin(), vector<string>(WIDTH, "⬜"));
            score += 100;
            dropSpeed = max(100, dropSpeed - 50);
        }
    }
}

void spawnPiece() {
   
    currentPiece = nextPiece;
    pieceX = WIDTH / 2 - 1;
    pieceY = 0;
    
    
    nextPiece = shape[rand() % shape.size()];
    
    if (!isValidMove(currentPiece, pieceX, pieceY)) {
        int maxx = 0;
        maxx = max(maxx, score);
        cout << "Game Over! Final Score: " << score << "\n";
        cout << "Maximum Score: " << maxx << "\n";
        cout << "Press ENTER to play again or ESC to exit...";
        Beep(1000, 800);
        while (true) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 13) {
                    score = 0;
                    board.assign(HEIGHT, vector<string>(WIDTH, "⬜"));
                    spawnPiece();
                    return;
                }
                if (key == 27) {
                    exit(0);
                }
            }
        }
    }
}

void handleInput() {
    while (_kbhit()) {
        char key = _getch();
        int newX = pieceX, newY = pieceY;
        Tetromino newPiece = currentPiece;

        if (key == 75 && isValidMove(currentPiece, pieceX - 1, pieceY)) newX--; 
        if (key == 77 && isValidMove(currentPiece, pieceX + 1, pieceY)) newX++; 
        if (key == 80 && isValidMove(currentPiece, pieceX, pieceY + 1)) newY++; 
        if (key == 72) { 
            newPiece.rotate();
            if (isValidMove(newPiece, pieceX, pieceY))
                currentPiece = newPiece;
        }
        if (key == 32) 
            while (isValidMove(currentPiece, pieceX, newY + 1)) newY++;
        if (key == 27) { 
            Beep(1000, 800);
            exit(0);
        }
        pieceX = newX;
        pieceY = newY;
    }
}

void gameLoop() {
    while (true) {
        handleInput();
        if (isValidMove(currentPiece, pieceX, pieceY + 1)) {
            pieceY++;
        } else {
            placePiece();
            clearLines();
            spawnPiece();
        }
        displayBoard();
        Sleep(dropSpeed);
    }
}

int main() {
    srand(time(0));
    SetConsoleOutputCP(CP_UTF8);

    nextPiece = shape[rand() % shape.size()];
    
    spawnPiece();
    gameLoop();
    return 0;
}
