#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

// Color codes for Windows console
const int COLOR_BLACK = 0;
const int COLOR_BLUE = 1;
const int COLOR_GREEN = 2;
const int COLOR_CYAN = 3;
const int COLOR_RED = 4;
const int COLOR_MAGENTA = 5;
const int COLOR_YELLOW = 6;
const int COLOR_WHITE = 7;
const int COLOR_GRAY = 8;
const int COLOR_BRIGHT_BLUE = 9;
const int COLOR_BRIGHT_GREEN = 10;
const int COLOR_BRIGHT_CYAN = 11;
const int COLOR_BRIGHT_RED = 12;
const int COLOR_BRIGHT_MAGENTA = 13;
const int COLOR_BRIGHT_YELLOW = 14;
const int COLOR_BRIGHT_WHITE = 15;

// Default board dimensions
int Board_Width = 20;
int Board_Height = 20;
const int Tetris_Size = 4;

// Tetrimino shapes with different colors
const pair<int, int> Tetrimino_Shape[7][Tetris_Size][Tetris_Size] = {
    {{{0, COLOR_CYAN}, {0, COLOR_CYAN}, {0, COLOR_CYAN}, {0, COLOR_CYAN}}, 
     {{1, COLOR_CYAN}, {1, COLOR_CYAN}, {1, COLOR_CYAN}, {1, COLOR_CYAN}}, 
     {{0, COLOR_CYAN}, {0, COLOR_CYAN}, {0, COLOR_CYAN}, {0, COLOR_CYAN}}, 
     {{0, COLOR_CYAN}, {0, COLOR_CYAN}, {0, COLOR_CYAN}, {0, COLOR_CYAN}}}, // I (Cyan)
     
    {{{0, COLOR_YELLOW}, {0, COLOR_YELLOW}, {0, COLOR_YELLOW}, {0, COLOR_YELLOW}}, 
     {{0, COLOR_YELLOW}, {1, COLOR_YELLOW}, {1, COLOR_YELLOW}, {0, COLOR_YELLOW}}, 
     {{0, COLOR_YELLOW}, {1, COLOR_YELLOW}, {1, COLOR_YELLOW}, {0, COLOR_YELLOW}}, 
     {{0, COLOR_YELLOW}, {0, COLOR_YELLOW}, {0, COLOR_YELLOW}, {0, COLOR_YELLOW}}}, // O (Yellow)
     
    {{{0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}}, 
     {{0, COLOR_MAGENTA}, {1, COLOR_MAGENTA}, {0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}}, 
     {{1, COLOR_MAGENTA}, {1, COLOR_MAGENTA}, {1, COLOR_MAGENTA}, {0, COLOR_MAGENTA}}, 
     {{0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}, {0, COLOR_MAGENTA}}}, // T (Magenta)
     
    {{{0, COLOR_GREEN}, {0, COLOR_GREEN}, {0, COLOR_GREEN}, {0, COLOR_GREEN}}, 
     {{0, COLOR_GREEN}, {1, COLOR_GREEN}, {1, COLOR_GREEN}, {0, COLOR_GREEN}}, 
     {{1, COLOR_GREEN}, {1, COLOR_GREEN}, {0, COLOR_GREEN}, {0, COLOR_GREEN}}, 
     {{0, COLOR_GREEN}, {0, COLOR_GREEN}, {0, COLOR_GREEN}, {0, COLOR_GREEN}}}, // S (Green)
     
    {{{0, COLOR_RED}, {0, COLOR_RED}, {0, COLOR_RED}, {0, COLOR_RED}}, 
     {{1, COLOR_RED}, {1, COLOR_RED}, {0, COLOR_RED}, {0, COLOR_RED}}, 
     {{0, COLOR_RED}, {1, COLOR_RED}, {1, COLOR_RED}, {0, COLOR_RED}}, 
     {{0, COLOR_RED}, {0, COLOR_RED}, {0, COLOR_RED}, {0, COLOR_RED}}}, // Z (Red)
     
    {{{0, COLOR_BLUE}, {0, COLOR_BLUE}, {0, COLOR_BLUE}, {0, COLOR_BLUE}}, 
     {{1, COLOR_BLUE}, {0, COLOR_BLUE}, {0, COLOR_BLUE}, {0, COLOR_BLUE}}, 
     {{1, COLOR_BLUE}, {1, COLOR_BLUE}, {1, COLOR_BLUE}, {0, COLOR_BLUE}}, 
     {{0, COLOR_BLUE}, {0, COLOR_BLUE}, {0, COLOR_BLUE}, {0, COLOR_BLUE}}}, // J (Blue)
     
    {{{0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}}, 
     {{0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}, {1, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}}, 
     {{1, COLOR_BRIGHT_YELLOW}, {1, COLOR_BRIGHT_YELLOW}, {1, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}}, 
     {{0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}, {0, COLOR_BRIGHT_YELLOW}}}  // L (Orange/Bright Yellow)
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class Tetromino {
public:
    pair<int, int> shape[Tetris_Size][Tetris_Size]; // first: block exists, second: color
    int PieceX, PieceY;
    int shapeIndex;

    Tetromino() {
        shapeIndex = rand() % 7;
        for (int i = 0; i < Tetris_Size; i++)
            for (int j = 0; j < Tetris_Size; j++)
                shape[i][j] = Tetrimino_Shape[shapeIndex][i][j];
        PieceX = Board_Width / 2 - Tetris_Size / 2;
        PieceY = 0;
    }

    void rotate() {
        pair<int, int> temp[Tetris_Size][Tetris_Size];
        for (int i = 0; i < Tetris_Size; i++)
            for (int j = 0; j < Tetris_Size; j++)
                temp[j][Tetris_Size - 1 - i] = shape[i][j];
        for (int i = 0; i < Tetris_Size; i++)
            for (int j = 0; j < Tetris_Size; j++)
                shape[i][j] = temp[i][j];
    }
};

class Tetris {
private:
    vector<vector<pair<int, int>>> Board; // first: block exists, second: color
    Tetromino currentPiece;
    vector<Tetromino> upcomingPieces;
    int score;
    bool firstDraw;
    bool paused;
    string playerName;

    void initializeBoard() {
        Board.clear();
        Board.resize(Board_Height, vector<pair<int, int>>(Board_Width, {0, COLOR_WHITE}));
    }

    bool isMoveValid(int newX, int newY) {
        for (int i = 0; i < Tetris_Size; i++) {
            for (int j = 0; j < Tetris_Size; j++) {
                if (currentPiece.shape[i][j].first) {
                    int PieceX = newX + j;
                    int PieceY = newY + i;
                    if (PieceX < 0 || PieceX >= Board_Width || PieceY >= Board_Height || (PieceY >= 0 && Board[PieceY][PieceX].first))
                        return false;
                }
            }
        }
        return true;
    }

    void mergePiece() {
        for (int i = 0; i < Tetris_Size; i++) {
            for (int j = 0; j < Tetris_Size; j++) {
                if (currentPiece.shape[i][j].first) {
                    Board[currentPiece.PieceY + i][currentPiece.PieceX + j] = {
                        1, 
                        currentPiece.shape[i][j].second
                    };
                }
            }
        }
    }

    void clearLines() {
        for (int i = Board_Height - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < Board_Width; j++) {
                if (!Board[i][j].first) {
                    full = false;
                    break;
                }
            }
            if (full) {
                Board.erase(Board.begin() + i);
                Board.insert(Board.begin(), vector<pair<int, int>>(Board_Width, {0, COLOR_WHITE}));
                score += 100;
            }
        }
    }

    void generateUpcomingPieces() {
        while (upcomingPieces.size() < 3) {
            upcomingPieces.push_back(Tetromino());
        }
    }

    void drawBoard() {
        system("cls");
        
        // Draw upcoming pieces
        setColor(COLOR_BRIGHT_WHITE);
        cout << "Upcoming Pieces:\n";
        for (int i = 0; i < 4; i++) {
            cout << "  ";
            for (int p = 0; p < 3; p++) {
                if (p < upcomingPieces.size()) {
                    for (int j = 0; j < 4; j++) {
                        if (upcomingPieces[p].shape[i][j].first) {
                            setColor(upcomingPieces[p].shape[i][j].second);
                            cout << "#";
                            setColor(COLOR_BRIGHT_WHITE);
                        } else {
                            cout << " ";
                        }
                    }
                    cout << "  ";
                }
            }
            cout << endl;
        }
        
        // Draw score
        setColor(COLOR_BRIGHT_WHITE);
        cout << playerName << "'s Score: " << score << "\n";
        
        // Draw top border
        cout << "+";
        for (int j = 0; j < Board_Width; j++) cout << "-";
        cout << "+\n";

        // Draw the board with current piece
        for (int i = 0; i < Board_Height; i++) {
            cout << "|";
            for (int j = 0; j < Board_Width; j++) {
                bool isCurrentPiece = false;
                int color = Board[i][j].second;
                
                // Check if this position is part of the current falling piece
                for (int pi = 0; pi < Tetris_Size; pi++) {
                    for (int pj = 0; pj < Tetris_Size; pj++) {
                        if (currentPiece.shape[pi][pj].first && 
                            currentPiece.PieceY + pi == i && 
                            currentPiece.PieceX + pj == j) {
                            isCurrentPiece = true;
                            color = currentPiece.shape[pi][pj].second;
                            break;
                        }
                    }
                    if (isCurrentPiece) break;
                }
                
                if (Board[i][j].first || isCurrentPiece) {
                    setColor(color);
                    cout << "#";
                    setColor(COLOR_BRIGHT_WHITE);
                } else {
                    cout << " ";
                }
            }
            cout << "|\n";
        }

        // Draw bottom border
        cout << "+";
        for (int j = 0; j < Board_Width; j++) cout << "-";
        cout << "+\n";

        if (paused) {
            gotoxy(2, Board_Height / 2 - 1);
            setColor(COLOR_BRIGHT_RED);
            cout << "Paused";
            gotoxy(2, Board_Height / 2);
            setColor(COLOR_BRIGHT_WHITE);
            cout << "Press 'r' to Resume";
            gotoxy(2, Board_Height / 2 + 1);
            cout << "Controls: a/d/x/w/s/p/r";
        }
    }

    void showGameOver() {
        gotoxy(0, Board_Height + 7);
        setColor(COLOR_BRIGHT_RED);
        cout << "Game Over, " << playerName << "! Final Score: " << score << endl;
        setColor(COLOR_BRIGHT_WHITE);
        cout << "Press 'r' to Restart or 'q' to Quit: ";
    }

    void getPlayerSettings() {
        system("cls");
        setColor(COLOR_BRIGHT_WHITE);
        cout << "Welcome to Colorful Tetris!\n\n";
        cout << "Enter your name: ";
        getline(cin, playerName);
        
        cout << "\nEnter board width (10-30, default 20): ";
        string widthInput;
        getline(cin, widthInput);
        if (!widthInput.empty()) {
            try {
                int w = stoi(widthInput);
                if (w >= 10 && w <= 30) Board_Width = w;
            } catch (...) {}
        }
        
        cout << "Enter board height (15-30, default 20): ";
        string heightInput;
        getline(cin, heightInput);
        if (!heightInput.empty()) {
            try {
                int h = stoi(heightInput);
                if (h >= 15 && h <= 30) Board_Height = h;
            } catch (...) {}
        }
        
        cout << "\nControls:\n";
        cout << "  a - Move Left\n";
        cout << "  d - Move Right\n";
        cout << "  x - Soft Drop (faster down)\n";
        cout << "  w - Rotate Clockwise\n";
        cout << "  s - Rotate Counterclockwise\n";
        cout << "  p - Pause\n";
        cout << "  r - Resume/Restart\n";
        cout << "  q - Quit\n";
        cout << "\nPress any key to start...";
        _getch();
    }

public:
    Tetris() : score(0), firstDraw(true), paused(false) {
        srand(time(0));
        getPlayerSettings();
        initializeBoard();
        generateUpcomingPieces();
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            int newX = currentPiece.PieceX, newY = currentPiece.PieceY;
            switch (tolower(key)) {
                case 'a': newX--; break; // Move Left
                case 'd': newX++; break; // Move Right
                case 'x': newY++; break; // Soft Drop
                case 'w': { // Rotate Clockwise
                    Tetromino temp = currentPiece;
                    temp.rotate();
                    if (isMoveValid(temp.PieceX, temp.PieceY)) {
                        currentPiece.rotate();
                    }
                    break;
                }
                case 's': { // Rotate Counterclockwise
                    Tetromino temp = currentPiece;
                    temp.rotate();
                    temp.rotate();
                    temp.rotate(); // Three clockwise rotations = one counterclockwise
                    if (isMoveValid(temp.PieceX, temp.PieceY)) {
                        currentPiece = temp;
                    }
                    break;
                }
                case 'p': paused = true; break; // Pause
                case 'r': paused = false; break; // Resume
                case 'q': // Quit
                    score = -1; // Signal to quit
                    return;
            }
            if (!paused && isMoveValid(newX, newY)) {
                currentPiece.PieceX = newX;
                currentPiece.PieceY = newY;
            }
        }
    }

    bool update() {
        if (paused) return true;
        if (isMoveValid(currentPiece.PieceX, currentPiece.PieceY + 1)) {
            currentPiece.PieceY++;
        } else {
            mergePiece();
            clearLines();
            if (upcomingPieces.empty()) {
                currentPiece = Tetromino();
                generateUpcomingPieces();
            } else {
                currentPiece = upcomingPieces[0];
                upcomingPieces.erase(upcomingPieces.begin());
                generateUpcomingPieces();
            }
            if (!isMoveValid(currentPiece.PieceX, currentPiece.PieceY)) return false;
        }
        return true;
    }

    void run() {
        while (true) {
            while (update()) {
                drawBoard();
                handleInput();
                if (score == -1) return; // Quit signal
                Sleep(200);
            }
            
            drawBoard();
            showGameOver();
            
            // Wait for restart or quit
            while (true) {
                if (_kbhit()) {
                    char key = tolower(_getch());
                    if (key == 'r') {
                        // Restart game
                        score = 0;
                        initializeBoard();
                        upcomingPieces.clear();
                        generateUpcomingPieces();
                        currentPiece = Tetromino();
                        firstDraw = true;
                        paused = false;
                        break;
                    } else if (key == 'q') {
                        return; // Quit game
                    }
                }
                Sleep(100);
            }
        }
    }
};

int main() {
    // Enable ANSI color codes (not needed for Windows console API but kept for compatibility)
    system("color");
    
    // Hide cursor
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
    
    Tetris game;
    game.run();
    
    // Show cursor before exiting
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(out, &cursorInfo);
    
return 0;
}