


//   PARA COMPILAR

//   g++ GraphicMode.cpp -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio; ./a.out

//==================================================================================


#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<time.h>

using namespace std;

const float pA = 0.9, pB = 0.9, P = 0.5; // pA e pB porcentagem de tolerancia de elementos diferentes e P é a porcentagem da matriz q sera ocupada
const int T = 100, POP = ( ((T*T) * P ) / 2), Max = 1000;
int ocupacaoA = 0, ocupacaoB = 0, t1=0, k=0;


int Sortear(){
	return rand() % 2;
}

int realocar(){
	return rand() % (T-2) + 1;
}

void zerarMatriz(int matriz[T][T]){
	int i, j;
	for(i = 0; i<T; i++){
		for(j = 0; j<T; j++){
			matriz[i][j] = 0;
		}
	}
}

void imprimirMatriz(int matriz[T][T]){
	int i, j;
	for(i = 0; i<T; i++){
		for(j = 0; j<T; j++){
			cout << matriz[i][j];
		}
		printf("\n");
	}
}

int contarIguais (int matriz[T][T], int px, int py){
	int counter = -1;
	if ( matriz[px][py] == 0 ){
        return 0;
    }

	int valor = matriz[px][py];
	for (int i = px-1; i < px+2; i++){
		for (int j = py-1; j < py+2; j++){
			if(matriz[i][j] == valor){
                counter++;
			}
		}
	}
	return counter;
}

int contarTodos (int matriz[T][T], int px, int py){
	int counter = -1;
	if ( matriz[px][py] == 0 ){
        return 0;
	}
	for (int i = px-1; i < px+2; i++){
		for (int j = py-1; j < py+2; j++){
			if( matriz[i][j] != 0){
                counter++;
			}
		}
	}
	return counter;
}

void regra(int matriz[T][T]){
	bool teste = 0;
	int same, all, a, b;
	k = 0;

	for(int x = 1; x < T-1; x++){
		for(int y = 1; y < T-1; y++){
			if (matriz[x][y] == 0)
                teste = 1;

            same = contarIguais(matriz, x, y);
			all = contarTodos(matriz, x, y);

			if (all == 0)
                all = 1;

			if (matriz[x][y] == 1){ //vermelho
				if ( (same / all) >= (1-pA) ){
                    teste = 1;
				}
			}

			if (matriz[x][y] == 2){ //azul
				if( (same / all) >= (1-pB) ){
                    teste = 1;
				}
			}

			//realocacao
			while(teste == 0){
				a = realocar();
				b = realocar();
				if( matriz[a][b] == 0){
					matriz[a][b] = matriz[x][y];
					matriz[x][y] = 0;
					teste = 1;
					k++;
				}
			}

		    teste = 0;
		}
	}
}

void ocuparMatriz(int matriz[T][T]){
	int i, j, sorteio;
	while( (ocupacaoA < POP) || (ocupacaoB < POP) ){
	    for(i = 1; i < T-1; i++){
	        for(j = 1; j < T-1; j++){
	            sorteio = Sortear();
	            if (sorteio == 1){ // se sim, vai ocupar a matriz
	                sorteio = Sortear();
	                if( (ocupacaoA < POP) && (sorteio == 1) ){
	                	matriz[i][j] = 1;
	                	ocupacaoA++;
	                }
	                if( (ocupacaoB < POP) && (sorteio != 1) ){
	                	matriz[i][j] = 2;
	                	ocupacaoB++;
	                }
	            }
	        }
	    }
	}
}

//-----------------------------------------------------------------------

void Pause (int milliseconds) {
	struct timespec ts;
	ts.tv_sec = 0;//milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void show (sf::RenderWindow *window, int matriz[T][T]) {

	sf::RectangleShape rect (sf::Vector2f (3.5, 3.5));
	
	for (int i = 0; i < T; i++){
		for (int j = 0; j < T; j++) {
			if (matriz[i][j]==0)
				rect.setFillColor (sf::Color(255, 255, 255));
			if (matriz[i][j]==1)
				rect.setFillColor (sf::Color(255, 0, 0));
			if (matriz[i][j]==2)
				rect.setFillColor (sf::Color(0, 0, 255));
			rect.setPosition  (i*5, j*5);         
			window->draw (rect);
	    }
	}
}

int main() {

	srand(time(NULL));
	int m[T][T], L = 0;
	sf::RenderWindow window(sf::VideoMode(500, 500), "Simulação de Torcida!");

	zerarMatriz(m);
	
	ocuparMatriz(m);
	
	while (window.isOpen() && (L < Max) ) {
		sf::Event event;
		while (window.pollEvent(event)) 
			if( (event.type == sf::Event::Closed) || ( (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) ) )
				return 0;
			
		regra(m);

		if (k==0)
			break;
		
		window.clear();
			
		show (&window, m);

		window.display();

		Pause (100);
		L++;
		//printf("%d\n", L );		
		
	}
	printf("\n%d\n", L);
	while (window.isOpen() || L == Max){
		sf::Event event;
		while (window.pollEvent(event)) 
			if( (event.type == sf::Event::Closed) || ( (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) ) )
				return 0;
		
		show (&window, m);
		
	}


	return 0;
}
