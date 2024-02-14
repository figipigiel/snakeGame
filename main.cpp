#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <conio.h>
#include <windows.h>

enum Direction { // definicja zmiennej wyliczeniowej odpowiadajcej za kierunek, wartosc poczatkowa = STOP
	STOP,LEFT,RIGHT,UP,DOWN
};

int width=30,height=15,playerX=15,playerY=7,score=0; // width,height to wymiary planszy, playerX,playerY to wspolrzedne glowy weza
bool isOver=false; // zmienna bool odpowiadajaca za zakonczenie gry
Direction dir,prevDir; // zmienna Direction odpowiadajaca za kierunek weza
std::vector<std::array<int,2>> tail= {{playerX,playerY}}; // wektor przechowujący koordynaty wszystkich czesci ogona

std::array<int,2> genFruitCoords() { // funkcja zwracajaca dwuelementowa tablice zawierajaca wygenerowane losowe wymiary owocu
	std::array<int,2> result;
	result[0]=(rand()%(width-1));
	if(result[0]==0){
		result[0]=1;
	}
	result[1]=(rand()%(height-1));
	if(result[1]==0){
		result[1]=1;
	}
	return result;
}

std::array<int,2> fruit=genFruitCoords(); // tablica zawierajaca koordynaty owocu

bool checkFruitPos(){
	for(int i=1; i<tail.size(); i++){
		if(tail[i][0]==fruit[0] && tail[i][1]==fruit[1]){
			return true;
		}
	}
	return false;
}

void screen() { //funkcja odpowiadajaca za wyswietlenie planszy, w tym granic, owocu, glowy oraz ogona weza
	system("cls"); // Przed wyswietleniem czegokolwiek konsola zostaje wyczyszczone
	std::cout<<fruit[0]<<","<<fruit[1]<<std::endl<<std::endl;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			if(i==0 || i==height-1) {
				std::cout<<"#";
			}
			else{
				if(j==0 || j==width-1){
					std::cout<<"#";
					continue;
				}
				if(i==fruit[1] && j==fruit[0]){
					std::cout<<"f";
					continue;
				}
				bool print=false;
				for(int k=0; k<tail.size(); k++){
					if(tail[k][0]==j && tail[k][1]==i){
						std::cout<<"*";
						print=true;
					}
				}
				if(!print){
					std::cout<<" ";
				}
			}
		}
		std::cout<<std::endl;
	}
	std::cout<<"Wynik: "<<score<<std::endl;
}

void input() { // Funkcja przypisujaca odpowiedni kierunek dir ze wzgledu na input
	if(_kbhit()) {
		switch(_getch()) {
			case 'w':
				dir=UP;
				break;
			case 'a':
				dir=LEFT;
				break;
			case 's':
				dir=DOWN;
				break;
			case 'd':
				dir=RIGHT;
				break;
			case 'x':
				isOver=true;
				break;
			case 'k':
				dir=STOP;
				break;
		}
	}
}


void move() { // Funkcja przesuwa koordynaty glowy weza ze wzgledu na dir oraz ustawia prevX,prevY
	switch(dir) {
		case LEFT:
			if(prevDir!=RIGHT || tail.size()<2){
				LEFT:
				tail.insert(tail.begin(),{tail[0][0]-1,tail[0][1]});
				tail.pop_back();
				if(tail[0][0]==0) {
					tail[0][0]=width-2;
				}
				prevDir=LEFT;
			}
			else if(prevDir==RIGHT && tail.size()>1){
				goto RIGHT;
			}
			break;
		case RIGHT:
			if(prevDir!=LEFT || tail.size()<2){
				RIGHT:
				tail.insert(tail.begin(),{tail[0][0]+1,tail[0][1]});
				tail.pop_back();
				if(tail[0][0]==width-1) {
					tail[0][0]=1;
				}
				prevDir=RIGHT;
			}
			else if(prevDir==LEFT && tail.size()>1){
				goto LEFT;
			}
			break;
		case UP:
			if(prevDir!=DOWN || tail.size()<2){
				UP:
				tail.insert(tail.begin(),{tail[0][0],tail[0][1]-1});
				tail.pop_back();
				if(tail[0][1]==0){
					tail[0][1]=height-2;
				}
				prevDir=UP;
			}
			else if(prevDir==DOWN && tail.size()>1){
				goto DOWN;
			}
			break;
		case DOWN:
			if(prevDir!=UP || tail.size()<2){
				DOWN:
				tail.insert(tail.begin(),{tail[0][0],tail[0][1]+1});
				tail.pop_back();
				if(tail[0][1]==height-1){
					tail[0][1]=1;
				}
				prevDir=DOWN;
			}
			else if(prevDir==UP && tail.size()>1){
				goto UP;
			}
			break;
	}
	for(int i=1; i<tail.size()-1; i++){ // Sprawdzenie czy gracz uderzyl we wlasny ogon
		if(tail[0][0]==tail[i][0] && tail[0][1]==tail[i][1]){
			isOver=true;
		}
	}
	if(tail[0][0]==fruit[0] && tail[0][1]==fruit[1]) { // Sprawdzenie czy gracz znajduje sie na polu owocu
		tail.push_back({playerX,playerY});
		fruit=genFruitCoords();
		score++;
		while(checkFruitPos()){
			fruit=genFruitCoords();
		}
	}
	
}

int main() {
	while(!isOver){
		screen();
		input();
		move();
		Sleep(100);
	}
	if(isOver){
		std::cout<<"Game Over!"<<std::endl;
	}
	return 0;
}

// Faktyczne wymiary: 1-28 w poziomie, 1-13 w pionie