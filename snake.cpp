#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<thread>
#include<chrono>
using namespace std;

const int BOARD_WIDTH = 20;
const int BOARD_HEIGHT = 20;

enum Direction {
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
};

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
};

class Snake {
private:
    vector<vector<char>> board;
    vector<Position> snake;
    Position food;
    Direction currentdir;
    int score;
    int highscore;
    bool gameover;
    bool foodeaten;

    void GotoXY(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void HideCursor()
    {
        CONSOLE_CURSOR_INFO cursorinfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
        cursorinfo.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
    }

    void ClearScreen()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordScreen = {0, 0};
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, coordScreen, &cCharsWritten);
        SetConsoleCursorPosition(hConsole, coordScreen);
    }

    void GenerateFood()
    {
        do
        {
            food.x = rand() % BOARD_WIDTH;
            food.y = rand() % BOARD_HEIGHT;
        } while(isSnakePosition(food));
    }

    bool isSnakePosition(const Position& pos)
    {
        for(const auto& seg : snake)
        {
            if(seg == pos)
            {
                return true;
            }
        }
        return false;
    }

    bool isValidMove(const Position& newhead)
    {
        if(newhead.x < 0 || newhead.x >= BOARD_WIDTH || newhead.y < 0 || newhead.y >= BOARD_HEIGHT)
        {
            return false;
        }
        for(int i=0; i < snake.size() - (foodeaten ? 0 : 1); i++)
        {
            if(snake[i] == newhead)
            {
                return false;
            }
        }
        return true;
    }

public:
    Snake()
    {
        board.resize(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' '));
        currentdir = RIGHT;
        score = 0;
        highscore = 0;
        gameover = false;
        foodeaten = false;
        snake.push_back(Position(BOARD_WIDTH / 2, BOARD_HEIGHT / 2));
        snake.push_back(Position(BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2));
        snake.push_back(Position(BOARD_WIDTH / 2 - 2, BOARD_HEIGHT / 2));
        srand(time(0));
        GenerateFood();
        HideCursor();
    }

    void HandleInput()
    {
        if(_kbhit())
        {
            char key = _getch();
            key = tolower(key);
            switch(key)
            {
                case 'w':
                    if(currentdir != DOWN)
                    {
                        currentdir = UP;
                    }
                    break;
                case 's':
                    if(currentdir != UP)
                    {
                        currentdir = DOWN;
                    }
                    break;
                case 'a':
                    if(currentdir != RIGHT)
                    {
                        currentdir = LEFT;
                    }
                    break;
                case 'd':
                    if(currentdir != LEFT)
                    {
                        currentdir = RIGHT;
                    }
                    break;
                case 'q':
                    gameover = true;
                    break;
            }
        }
    }

    void Update()
    {
        if(gameover)
        {
            return;
        }
        Position head = snake[0];
        Position newhead = head;
        switch(currentdir)
        {
            case UP:
                newhead.y--;
                break;
            case DOWN:
                newhead.y++;
                break;
            case LEFT:
                newhead.x--;
                break;
            case RIGHT:
                newhead.x++;
                break;
        }
        if(!isValidMove(newhead))
        {
            gameover = true;
            if(score > highscore)
            {
                highscore = score;
            }
            return;
        }
        snake.insert(snake.begin(), newhead);
        foodeaten = (newhead == food);
        if(foodeaten)
        {
            score++;
            GenerateFood();
        }
        else
        {
            snake.pop_back();
        }
    }

    void Draw()
    {
        GotoXY(0, 0);
        for(int i=0; i < BOARD_HEIGHT; i++)
        {
            for(int j=0; j < BOARD_WIDTH; j++)
            {
                board[i][j] = '.';
            }
        }
        for(int i=0; i < snake.size(); i++)
        {
            if(i == 0)
            {
                board[snake[i].y][snake[i].x] = 'O';
            }
            else
            {
                board[snake[i].y][snake[i].x] = 'o';
            }
        }
        board[food.y][food.x] = '*';
        cout << "+";
        for(int j=0; j < BOARD_WIDTH; j++)
        {
            cout << "-";
        }
        cout << "+" << endl;
        for(int i=0; i < BOARD_HEIGHT; i++)
        {
            cout << "|";
            for(int j=0; j < BOARD_WIDTH; j++)
            {
                cout << board[i][j];
            }
            cout << "|";
            if(i == 2)
            {
                cout << "    Score: " << score;
            }
            else if(i == 4)
            {
                cout << "    High score: " << highscore;
            }
            else if(i == 6)
            {
                cout << "    Length: " << snake.size();
            }
            else if(i == 9)
            {
                cout << "    Controls:";
            }
            else if(i == 10)
            {
                cout << "    W: Up";
            }
            else if(i == 11)
            {
                cout << "    S: Down";
            }
            else if(i == 12)
            {
                cout << "    A: Left";
            }
            else if(i == 13)
            {
                cout << "    D: Right";
            }
            else if(i == 14)
            {
                cout << "    Q: Quit";
            }
            else if(i == 17)
            {
                cout << "    O: Head, o: Body, *: Food";
            }
            cout << endl;
        }
        cout << "+";
        for(int j=0; j < BOARD_WIDTH; j++)
        {
            cout << "-";
        }
        cout << "+" << endl;
        if(gameover)
        {
            cout << endl << "*** Game Over ***" << endl;
            cout << "Final score: " << score << endl;
            cout << "Snake length: " << snake.size() << endl;
            if(score == highscore)
            {
                cout << "NEW HIGH SCORE!" << endl;
            }
            cout << "Press R to restart or Q to quit..." << endl;
        }
    }

    bool isGameOver() const
    {
        return gameover;
    }

    void Restart()
    {
        snake.clear();
        snake.push_back(Position(BOARD_WIDTH / 2, BOARD_HEIGHT / 2));
        snake.push_back(Position(BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2));
        snake.push_back(Position(BOARD_WIDTH / 2 - 2, BOARD_HEIGHT / 2));
        score = 0;
        gameover = false;
        foodeaten = false;
        currentdir = RIGHT;
        GenerateFood();
    }

    int GetSpeed()
    {
        int basespeed = 200;
        int speedincrease = score / 50;
        return max(80, basespeed - speedincrease * 20);
    }

    void Run()
    {
        auto lastupdate = chrono::steady_clock::now();
        while(true)
        {
            HandleInput();
            if(gameover)
            {
                Draw();
                if(_kbhit())
                {
                    char key = _getch();
                    if(tolower(key) == 'r')
                    {
                        Restart();
                        ClearScreen();
                        continue;
                    }
                    else if(tolower(key) == 'q')
                    {
                        break;
                    }
                }
                this_thread::sleep_for(chrono::milliseconds(100));
                continue;
            }
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - lastupdate);
            if(elapsed.count() >= GetSpeed())
            {
                Update();
                lastupdate = now;
            }
            Draw();
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    }
};

int main()
{
    cout << "Welcome to SNAKE!" << endl;
    cout << "Controls:" << endl;
    cout << "W - Up" << endl;
    cout << "S - Down" << endl;
    cout << "A - Left" << endl;
    cout << "D - Right" << endl;
    cout << "Q - Quit" << endl;
    cout << "Press any key to start..." << endl;
    _getch();
    system("cls");
    Snake snake;
    snake.Run();
    return 0;
}