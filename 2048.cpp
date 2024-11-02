#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <cmath>
#include <sstream>
using namespace std;

void hsl_to_rgb(float h, float s, float l, int &r, int &g, int &b) {
    float c = (1 - std::fabs(2 * l - 1)) * s;
    float x = c * (1 - std::fabs(fmod(h / 60.0, 2) - 1));
    float m = l - c / 2;
    float r_, g_, b_;

    if (0 <= h && h < 60) { r_ = c, g_ = x, b_ = 0; }
    else if (60 <= h && h < 120) { r_ = x, g_ = c, b_ = 0; }
    else if (120 <= h && h < 180) { r_ = 0, g_ = c, b_ = x; }
    else if (180 <= h && h < 240) { r_ = 0, g_ = x, b_ = c; }
    else if (240 <= h && h < 300) { r_ = x, g_ = 0, b_ = c; }
    else { r_ = c, g_ = 0, b_ = x; }

    r = (r_ + m) * 255;
    g = (g_ + m) * 255;
    b = (b_ + m) * 255;
}


std::string get_styled_text(int value) {
    int r, g, b;
    float hue = std::fmod(std::log2(value) * 40, 360);
    hsl_to_rgb(hue, 1.0, 0.5, r, g, b);

    std::ostringstream oss;
    oss << "\033[1m"
        << "\033[38;2;" << r << ";" << g << ";" << b << "m"
        << value
        << "\033[0m";
    return oss.str();
}


std::string get_bold_text(string value) {
    std::ostringstream oss;
    oss << "\033[1m"
        << value
        << "\033[0m";
    return oss.str();
}

vector<vector<int>> grid = {
{0,0,0,0},
{0,0,0,0},
{0,0,0,0},
{0,0,0,0}};

long score=0;

vector<tuple<string, int>> scores = {};

ifstream file;

void clr(){
    cout << "\033[2J\033[1;1H";
}


char getch(void){
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}


vector<vector<int>> find_zeros(){
    vector<vector<int>> positions_of_zeros = {};
    for(int x=0;x<4;x++){
        for(int y=0;y<4;y++){
            if(grid[x][y]==0) positions_of_zeros.push_back({x,y});
        }
    }
    return positions_of_zeros;
}

void end(){
    clr();
    cout<<" ________  ________  _____ ______   _______           ________  ___      ___ _______   ________"<<endl;     
    cout<<"|\\   ____\\|\\   __  \\|\\   _ \\  _   \\|\\  ___ \\         |\\   __  \\|\\  \\    /  /|\\  ___ \\ |\\   __  \\"<<endl;    
    cout<<"\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\\\\\__\\ \\  \\ \\   __/|        \\ \\  \\|\\  \\ \\  \\  /  / | \\   __/|\\ \\  \\|\\  \\"<<endl;   
    cout<<" \\ \\  \\  __\\ \\   __  \\ \\  \\\\|__| \\  \\ \\  \\_|/__       \\ \\  \\\\\\  \\ \\  \\/  / / \\ \\  \\_|/_\\ \\   _  _\\"<<endl;  
    cout<<"  \\ \\  \\|\\  \\ \\  \\ \\  \\ \\  \\    \\ \\  \\ \\  \\_|\\ \\       \\ \\  \\\\\\  \\ \\    / /   \\ \\  \\_|\\ \\ \\  \\\\  \\|"<<endl; 
    cout<<"   \\ \\_______\\ \\__\\ \\__\\ \\__\\    \\ \\__\\ \\_______\\       \\ \\_______\\ \\__/ /     \\ \\_______\\ \\__\\\\ _\\"<<endl;
    cout<<"    \\|_______|\\|__|\\|__|\\|__|     \\|__|\\|_______|        \\|_______|\\|__|/       \\|_______|\\|__|\\|__|"<<endl;
    cout<<endl<<endl<<"Score : "<<score<<endl;
    string name;
    cout<<"enter name : ";
    cin>>name;
    cout<<endl;
    file.open("scores.txt");
    while(1){
        string actual_score;
        string actual_name;
        tuple<string, int> scoring;
        getline(file, actual_name);
        getline(file, actual_score);
        if(actual_name=="-1" || actual_score=="-1") break;
        scores.push_back({actual_name,stoi(actual_score)});
    }
    file.close();
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
    return std::get<1>(a) > std::get<1>(b);
    });
    for(int i=0; i<scores.size();i++){
        if(i<10){
            if(get<0>(scores[i])==name && get<1>(scores[i])==score){
                cout<<i+1<<". "<<get_bold_text(get<0>(scores[i]))<<": "<<get<1>(scores[i])<<endl;
            }else{
                cout<<i+1<<". "<<get<0>(scores[i])<<": "<<get<1>(scores[i])<<endl;
            }
        }else{
            if(get<0>(scores[i])==name && get<1>(scores[i])==score){
                cout<<endl;
                cout<<i+1<<". "<<get_bold_text(get<0>(scores[i]))<<": "<<get<1>(scores[i])<<endl;
            }
        }
          
    }
    ofstream f;
    f.open("scores.txt");
    string line;
    for(int i=0;i<scores.size();i++){
        f<<get<0>(scores[i])<<endl;
        f<<get<1>(scores[i])<<endl;
    }
    f<<"-1";
    f.close();
}

void summon_two(){
    vector<vector<int>> zeros = find_zeros();
    if(zeros.size()==0){
        end();
    }
    int new_two = rand() % zeros.size();
    grid[zeros[new_two][0]][zeros[new_two][1]] = 2;
}


void print_grid(){
    cout<<"╔═══════╤═══════╤═══════╤═══════╗"<<endl;
    for(int i=0;i<4;i++){
        cout<<"║       │       │       │       ║"<<endl<<"║";
        for(int j=0;j<4;j++){
            if(grid[i][j]!=0 && grid[i][j]<10){
                cout<<"   "<<get_styled_text(grid[i][j])<<"   ";
            }else if(grid[i][j]!=0 && grid[i][j]<100){
                cout<<"   "<<get_styled_text(grid[i][j])<<"  ";
            }else if(grid[i][j]!=0 && grid[i][j]<1000){
                cout<<"  "<<get_styled_text(grid[i][j])<<"  ";
            }else if(grid[i][j]!=0 && grid[i][j]<10000){
                cout<<"  "<<get_styled_text(grid[i][j])<<" ";
            }else if(grid[i][j]!=0){
                cout<<" "<<get_styled_text(grid[i][j])<<" ";
            }else{
                cout<<"       ";
            }
            if(j==3){
                cout<<"║"<<endl;
            }else{
                cout<<"│";
            }
        }
        cout<<"║       │       │       │       ║"<<endl;
        if(i!=3){
            cout<<"╟───────┼───────┼───────┼───────╢"<<endl;
        }else{
            cout<<"╚═══════╧═══════╧═══════╧═══════╝"<<endl;
        }
    }
    score+=2;
    cout<<endl<<"Score : "<<score<<endl;

}



void move_up(vector<int> &v1, int i){
    for(int j=3;j>-1;j--){
        if(grid[j][i]==0){
            v1.push_back(0);
        }else{
            v1.insert(v1.begin(), grid[j][i]);
        }
    }
}

void move_down(vector<int> &v1, int i){
    for(int j=0;j<4;j++){
        if(grid[j][i]==0){
            v1.insert(v1.begin(), 0);
        }else{
            v1.push_back(grid[j][i]);
        }
    }
}

void move_right(vector<int> &v1, const vector<int> &v2){
    for(int j=0;j<4;j++){
        if(v2[j] == 0){
            v1.insert(v1.begin(), 0);
        } else {
            v1.push_back(v2[j]);
        }
    }
}

void move_left(vector<int> &v1, const vector<int> &v2){
    for(int j=3;j>-1;j--){
        if(v2[j] == 0){
            v1.push_back(0);
        } else {
            v1.insert(v1.begin(), v2[j]);
        }
    }
}

void move_numbers(char s){
    switch(s){
        case 'U':
            for(int i=0;i<4;i++){
                vector<int> line = {};
                move_up(line, i);

                for(int j=0;j<3;j++){
                    if(line[j]==line[j+1]){
                        line[j] *= 2;
                        line[j+1] = 0;
                    }
                }

                vector<int> line2 = {};
                move_left(line2, line);
                
                for(int j=0;j<4;j++){
                    grid[j][i]=line2[j];
                }
            }
            break;
        case 'D':
            for(int i=0;i<4;i++){
                vector<int> line = {};
                move_down(line, i);

                for(int j=3;j>0;j--){
                    if(line[j]==line[j-1]){
                        line[j] *= 2;
                        line[j-1] = 0;
                    }
                }

                vector<int> line2 = {};
                move_right(line2, line);

                for(int j=0;j<4;j++){
                    grid[j][i]=line2[j];
                }
            }
            break;
        case 'R':
            for(int i=0;i<4;i++){
                vector<int> line = {};
                move_right(line, grid[i]);

                for(int j=3;j>0;j--){
                    if(line[j] == line[j-1] && line[j] != 0){
                        line[j] *= 2;
                        line[j-1] = 0;
                    }
                }

                vector<int> line2 = {};
                move_right(line2, line);
            
                grid[i] = line2;
            }
            break;
        case 'L':
            for(int i=0;i<4;i++){
                vector<int> line = {};
                move_left(line, grid[i]);

                for(int j=0;j<3;j++){
                    if(line[j]==line[j+1]){
                        line[j] *= 2;
                        line[j+1] = 0;
                    }
                }

                vector<int> line2 = {};
                move_left(line2, line);

                grid[i] = line2;
            }
            break;
    }

    clr();
    summon_two();
    print_grid();
}



void print_menu(bool p){
    clr();
    cout<<"  _______  ________  ___   ___  ________"<<endl;     
    cout<<" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\"<<endl;    
    cout<<"/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\"<<endl;   
    cout<<"|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\"<<endl;  
    cout<<"    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\|\\  \\"<<endl; 
    cout<<"   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\"<<endl;
    cout<<"    \\|_______|\\|_______|      \\|__|\\|_______|"<<endl;
    cout<<endl<<"by fraise"<<endl<<endl<<endl;
    cout<<"press RIGHT to confirm"<<endl<<endl;
    if(p==false){
        cout<<"> Start Game"<<endl;
        cout<<"  See Best Scores"<<endl;
    }else{
        cout<<"  Start Game"<<endl;
        cout<<"> See Best Scores"<<endl;
    }
}

void show_scores(){
    bool b=false;
    string line;
    clr();
    file.open("scores.txt");
    for(int i=1;i<51;i++){
        getline(file, line);
        if(line=="-1") break;
        cout<<i<<". "<<line<<": ";
        getline(file, line);
        cout<<line<<endl;
    }
    file.close();
    cout<<endl<<endl<<"press UP or DOWN to escape"<<endl;
}


 
void play(){
    move_numbers('X');
    while(1){
        if (getch() == '\033'){
            getch();
            switch(getch()){
                case 'A':
                    move_numbers('U');
                    break;
                case 'B':
                    move_numbers('D');
                    break;
                case 'C':
                    move_numbers('R');
                    break;
                case 'D':
                    move_numbers('L');
                    break;
            }
        }
    }
}


void start(){
    bool menu_position = false;
    print_menu(false);
    while(1){
        if (getch() == '\033'){
            getch();
            switch(getch()){
                case 'A':
                menu_position=menu_position==false;
                print_menu(menu_position);
                    break;
                case 'B':
                menu_position=menu_position==false;
                print_menu(menu_position);
                    break;
                case 'C':
                    if(menu_position==false){
                        play();
                    }else{
                        show_scores();
                    }
                    break;
            }
        }
    }
}

int main(){
    start();
    return 0;
}