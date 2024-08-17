#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;

#define windowWidth 640
#define windowHeight 480
#define tWidth 16
#define tHeight 16
int map[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 2, 2, 2, 2, 2, 1},
	{1, 0, 2, 0, 0, 0, 2, 1},
	{1, 0, 2, 0, 0, 0, 2, 1},
	{1, 0, 2, 2, 0, 2, 2, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

int main(){
	cout << "PrismCast v1.1" << endl;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	double posX = 4;
        double posY = 4;
        double dirX = -1;
        double dirY = 0;
        double cameraPlaneX = 0;
        double cameraPlaneY = 0.66;
        Uint32 sBuffer[windowHeight][windowWidth];
        vector texture[8];
	for(int i = 0; i < 8; i++){
		texture.resize[tWidth * tHeight];
	}
	for(int x = 0; x < tWidth; t++){
		for(int y = 0; y < tHeight; y++){
			texture[0][tWidth * y + x] = 65536 + 254 * (x != y && x != tWidth - y);
		}
	}
        double moveSpeed = 0.1;
        double cameraRotSpeed = 0.1;
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
	 /*             SDL_Color wallColor;
	              switch(map[mapX][mapY]){
	              	        case 1:
	              	        	wallColor.r = 255;
	              	        	wallColor.g = 255;
	              	        	wallColor.b = 255;
					break;
				case 2:
	              	        	wallColor.r = 255;
	              	        	wallColor.g = 0;
	              	        	wallColor.b = 255;
					break;
				default:
	              	        	wallColor.r = 255;
	              	        	wallColor.g = 255;
	              	        	wallColor.b = 255;
	              }
	*/
		      int texNum = map[mapX][mapY] - 1;
		      double wallX;
		      if(side == 0){
		            wallX = posY + perpWallDist * rayDirY;
		      }else{
			    wallX = posX + perpWallDist * rayDirX;
		      }
		      int texX = int(wallX * double(texWidth));
		      if(side == 0 && rayDirX > 0){
			    texX = texWidth - texX - 1;
		      }else if(side == 1 && rayDirY){
	      		    texX = texWidth - texX - 1;
		      }
		      double step = 1.0 * texHeight / lineHeight;
		      double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
		      for(int y = drawStart; y < drawEnd; y++){
		      	    int texY = (int)texPos & texHeight - 1;
			    texPos += step;
			    Uint32 color = texture[texNum][texHeight * texY + texX];
			    if(side == 1){
			          color = (color >> 1) & 8355711;
			    }
			    buffer[y][x] = color;
		      }

		      SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, 255);
		      if(side == 1){
		                SDL_SetRenderDrawColor(renderer, wallColor.r / 2, wallColor.g / 2, wallColor.b / 2, 255);
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
