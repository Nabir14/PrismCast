#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;

int main(){
	cout << "PrismCast v1.0.0" << endl;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	double posX = 4;
        double posY = 4;
        double dirX = -1;
        double dirY = 0;
        double cameraPlaneX = 0;
        double cameraPlaneY = 0.66;
        double moveSpeed = 0.1;
        double cameraRotSpeed = 0.1;
        int windowWidth = 640;
        int windowHeight = 480;
	int map[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 0, 0, 1, 1},
	{1, 0, 1, 0, 0, 0, 1, 1},
	{1, 0, 1, 1, 0, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
	};
	SDL_Window* window = SDL_CreateWindow("PrismCast", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* icon = IMG_Load("../icon.png");
	SDL_SetWindowIcon(window, icon);
	bool quit = true;
        while(quit){
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_Event e;
                while(SDL_PollEvent(&e)){
	        	if(e.type == SDL_QUIT){
        	        	quit = false;
                	}else if(e.type == SDL_KEYDOWN){
                		if(e.key.keysym.sym == SDLK_UP){
           		                if(map[int(posX + dirX * moveSpeed)][int(posY)] == false){
                         	               posX += dirX * moveSpeed;
                           	        }
	                                if(map[int(posX)][int(posY + dirY * moveSpeed)] == false){
                                               posY += dirY * moveSpeed;
                                        }
              	    		}
				if(e.key.keysym.sym == SDLK_DOWN){
					if(map[int(posX - dirX * moveSpeed)][int(posY)] == false){
                                               posX -= dirY * moveSpeed;
                     	                }
                                        if(map[int(posX)][int(posY - dirY * moveSpeed)] == false){
                                               posY -= dirY * moveSpeed;
					}
				}
                  	        if(e.key.keysym.sym == SDLK_RIGHT){
	                                double oldDirX = dirX;
                                        dirX = dirX * cos(-cameraRotSpeed) - dirY * sin(-cameraRotSpeed);
                                        dirY = oldDirX * sin(-cameraRotSpeed) + dirY * cos(-cameraRotSpeed);
                                        double oldPlaneX = cameraPlaneX;
                       	                cameraPlaneX = cameraPlaneX * cos(-cameraRotSpeed) - cameraPlaneY * sin(-cameraRotSpeed);
                           	        cameraPlaneY = oldPlaneX * sin(-cameraRotSpeed) + cameraPlaneY * cos(-cameraRotSpeed);
                                }
                                if(e.key.keysym.sym == SDLK_LEFT){
          	                        double oldDirX = dirX;
                                        dirX = dirX * cos(cameraRotSpeed) - dirY * sin(cameraRotSpeed);
                      	                dirY = oldDirX * sin(cameraRotSpeed) + dirY * cos(cameraRotSpeed);
                             	        double oldPlaneX = cameraPlaneX;
                                    	cameraPlaneX = cameraPlaneX * cos(cameraRotSpeed) - cameraPlaneY * sin(cameraRotSpeed);
                                        cameraPlaneY = oldPlaneX * sin(cameraRotSpeed) + cameraPlaneY * cos(cameraRotSpeed);
                                }
			}
		}
		for(int x = 0; x < windowWidth; x++){
  	              double cameraX = 2 * x / double(640) - 1;
     		      double rayDirX = dirX + cameraPlaneX * cameraX;
		      double rayDirY = dirY + cameraPlaneY * cameraX;
		      int mapX = int(posX);
		      int mapY = int(posY);
		      double sideDistX;
	       	      double sideDistY;
	              double deltaDistX = abs(1 / rayDirX);
		      double deltaDistY = abs(1 / rayDirY);
	              double perpWallDist;
	              int stepX;
                      int stepY;
  	              int hit = 0;
  	 	      int side;
	              if(rayDirX < 0){
		      		stepX = -1;
                                sideDistX = (posX - mapX) * deltaDistX;
                      }else{
               	                stepX = 1;
                       	        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
                      }
                      if(rayDirY < 0){
	                        stepY = -1;
	                        sideDistY = (posY - mapY) * deltaDistY;
          	      }else{
	                   	stepY = 1;
		                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
		      }
                      while(hit == 0){
                      		if(sideDistX < sideDistY){
	            	 	        sideDistX += deltaDistX;
	                                mapX += stepX;
	               	           	side = 0;
		       	        }else{
		                        sideDistY += deltaDistY;
                	                mapY += stepY;
					side = 1;
	              		}
			      	if(map[mapX][mapY] > 0){
			                hit = 1;
	     		        }
	       	      }
		      if(side == 0){
		          	perpWallDist = (sideDistX - deltaDistX);
	              }else{
		                perpWallDist = (sideDistY - deltaDistY);
	              }
		      int h = windowHeight;
		      int lineHeight = (int)(h / perpWallDist);
		      int drawStart = -lineHeight / 2 + h / 2;
		      if(drawStart < 0){
		                drawStart = 0;
	              }
	       	      int drawEnd = lineHeight / 2 + h / 2;
		      if(drawEnd >= h){
		                drawEnd = h - 1;
	              }
		      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		      if(side == 1){
		                SDL_SetRenderDrawColor(renderer, 255 / 2, 255 / 2, 255 / 2, 255);
		      }
		      for(int y = drawStart; y < drawEnd; y++){
	                        SDL_RenderDrawPoint(renderer, x, y);
	       	      }
	       	}
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(icon);
        window = NULL;
        renderer = NULL;
	icon = NULL;
	IMG_Quit();
        SDL_Quit();
};
