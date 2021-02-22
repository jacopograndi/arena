//#define LCB_NO_CONSOLE

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <math.h>

#include <jsonparse.h>
#include <render_text.h>
#include <graphicsettings.h>
#include <button.h>
#include <mkb.h>
#include <vec.h>
#include <net.h>

#include <units.h>
#include <map.h>
#include <info.h>
#include <hud.h>
#include <gst.h>

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *sounds[16];


int main( int argc, char* args[] ) {   
    net_init();
    
    graphic_settings gs = { 1250, 700 };
    
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
    SDL_Renderer* rend = NULL;

	SDL_Init(SDL_INIT_VIDEO);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    
    window = SDL_CreateWindow("DESCRIPT", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, gs.resx, gs.resy, SDL_WINDOW_SHOWN 
        | SDL_WINDOW_RESIZABLE);
    screenSurface = SDL_GetWindowSurface(window);
    
    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // render text init
    txtd textd; char_width_init(textd.cw);
    SDL_Surface* image = SDL_LoadBMP("content/gf.bmp");
    textd.tex = SDL_CreateTextureFromSurface(rend, image);
    SDL_SetTextureColorMod(textd.tex, 0, 0, 0);
    
    SDL_Surface* imagesmall = SDL_LoadBMP("content/gfsmall.bmp");
    SDL_Texture* txsmall = SDL_CreateTextureFromSurface(rend, imagesmall);
    SDL_SetTextureColorMod(txsmall, 0, 0, 0);
    
    SDL_Surface* sprites = SDL_LoadBMP("content/sprites.bmp");
    SDL_Texture* txsprites = SDL_CreateTextureFromSurface(rend, sprites);
    
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    #define SOUND_MOUSE_OVER 0
    sounds[SOUND_MOUSE_OVER] = Mix_LoadWAV(
        "content/sounds/mouse_over.wav" );
    #define SOUND_MOUSE_CLICK_0 1
    sounds[SOUND_MOUSE_CLICK_0] = Mix_LoadWAV( 
        "content/sounds/mouse_click_0.wav" );
    #define SOUND_MOUSE_WHEEL 2
    sounds[SOUND_MOUSE_WHEEL] = Mix_LoadWAV(
        "content/sounds/mouse_wheel.wav" );
    #define SOUND_SUCCESS 3
    sounds[SOUND_SUCCESS] = Mix_LoadWAV( 
        "content/sounds/success.wav" );
    Mix_Volume(-1,MIX_MAX_VOLUME/16);
    // Mix_PlayChannel( -1, mouse_over, 0 );
    
    // frame timing
    const double FRAME_TIME = (double)1/60; // delta time for 60 FPS
    double last_time = SDL_GetTicks();
    double frame_counter = 0;
    double tot_time = 0;
    double unprocessed_time = 0;
    int frames = 0;
    
    // mouse and keyboard
    MKb mkb;
    mkb_init(&mkb);
    
    
    // info
    infos info;
    info_load(&info);
    
    gamestate gst;
    gst_init(&gst);
    gst.cam[0] = -gs.resx/2+gst.map_editor.sx*gst.map_editor.ts/2;
    gst.cam[1] = -gs.resy/2+gst.map_editor.sy*gst.map_editor.ts/2;

    // hud
    hud _hud;
    hud_init(&gs, &_hud, &textd);
    _hud.og.army_listlen = info_army_get_list(_hud.og.army_list);
    info_load_army(gst.army_bp+0, _hud.og.army_list[0]);
    info_load_playername(_hud.og.playername);
    
    float mlast[2] = {0, 0};
    
    // sockets
    net_client netc;
    net_server nets;
    int server = 0;
    
    bool quit = false;
    SDL_Event e;
    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            if(e.type == SDL_WINDOWEVENT
            && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
                gs.resx = e.window.data1; gs.resy = e.window.data2;
                SDL_SetWindowSize(window, gs.resx, gs.resy);
                hud_resize(&gs, &_hud, &textd);
            }
            mkb_event(&mkb, &e);
        }
        
        bool render = false;

        double startTime = SDL_GetTicks();
        double passedTime = (startTime - last_time)/1000;
        last_time = startTime;
        
        unprocessed_time += passedTime;
        frame_counter += passedTime;
        tot_time += passedTime;

        if (frame_counter >= 1.0) {
            //printf("FPS: %i | %f ms\n", frames, 1000.0 / ((double)frames));
            frames = 0;
            frame_counter = 0;
        }

        while (unprocessed_time > FRAME_TIME) {
            float mp[2] = { mkb.mx, mkb.my };
            if (mkb.mclick[1] == 1) {
                float delta[2] = { mkb.mx-mlast[0], mkb.my-mlast[1] };
                vec2_sub(gst.cam, gst.cam, delta);
            }
            /*
            if (mkb_search(&mkb, SDL_SCANCODE_E)) {
                map *m; army *ar;
                gst_get_maparmy(&gst, &m, &ar);
                army_move(&info, ar, m);
                army_fire(&info, ar, m);
            }*/
            
            hud_process(&gs, &_hud, &mkb, &info, 
                gst.army_bp+0, &gst.map_editor, &textd, &gst, 
                &netc, &nets, sounds);
                
            if (gst.state == 1) {
                gst_process(&gst, &info, tot_time);
            }
            
            render = true;
            unprocessed_time -= FRAME_TIME;
            mlast[0] = mkb.mx; mlast[1] = mkb.my;
            mkb_process(&mkb);
        }

        if (render) {
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            SDL_RenderClear(rend);
            
            float ts = 32;
            int posx = gst.cam[0];
            int posy = gst.cam[1];
            
            map *m; army *ar;
            gst_get_maparmy(&gst, &m, &ar);
            
            // render map
            for (int y=0; y<m->sy; y++) {
                for (int x=0; x<m->sx; x++) {
                    float px = x*ts;
                    float py = y*ts;
                    SDL_Rect srcRect = { 1*ts, 0*ts, ts, ts };
                    SDL_Rect dstRect = { (int)px-posx, (int)py-posy, ts, ts };
                    SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
                }
            }
            
            // render enemies
            for (int i=0; i<ar->uslen; i++) {
                if (ar->us[i].hp <= 0) continue;
                float x = ar->us[i].pos[0];
                float y = ar->us[i].pos[1];
                SDL_Rect srcRect = { ar->us[i].info.chassis*ts, ts, ts, ts };
                SDL_Rect dstRect = { (int)x-posx, (int)y-posy, ts, ts };
                SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
                
                float amt = ar->us[i].hp 
                    / info_unit_get_health(&info, &ar->us[i].info);
                SDL_Rect hprect = { 
                    (int)x-posx, (int)y-posy+ts-5, 
                    ts*amt, 6 };
                int sw = 1 ? ar->us[i].owner : 0;
                SDL_SetRenderDrawColor(rend, 255*sw, 255*(1-sw), 0, 255);
                SDL_RenderFillRect(rend, &hprect);
                
                SDL_SetTextureColorMod(txsmall, sw*100, 100*(1-sw), 0);
                char shp[32]; sprintf(shp, "%.0f", ar->us[i].hp);
                float php[2] = { (int)x-posx, (int)y-posy+ts-5 };
                render_text_small(rend, shp, php, txsmall);
                
                SDL_SetTextureColorMod(txsmall, 255, 160, 0);
                char sch[32]; sprintf(sch, "%.0f", ar->us[i].charge);
                float pch[2] = { (int)x-posx, (int)y-posy+ts+1 };
                render_text_small(rend, sch, pch, txsmall);
                SDL_SetTextureColorMod(txsmall, 0, 0, 0);
            }
            
            hud_render(&_hud, rend, &textd, &mkb, &info, txsprites);
            
            SDL_RenderPresent(rend);
            frames++;
        }
        else {
            SDL_Delay(1);
        }
    }

    Mix_FreeMusic(gMusic);
	SDL_DestroyWindow(window);
    Mix_Quit();
	SDL_Quit();
    net_client_close(&netc);
    gst_destroy(&gst);

	return 0;
}