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
    textd.tex_small = SDL_CreateTextureFromSurface(rend, imagesmall);
    SDL_SetTextureColorMod(textd.tex_small, 0, 0, 0);
    
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
    if (_hud.og.army_listlen > 0)
        info_load_army(gst.army_bp+0, _hud.og.army_list[0]);
    info_load_playername(_hud.og.playername);
    
    float mlast[2] = {0, 0};
    
    // fxs
    fxs fx;
    fx_init(&fx);
    
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
            
            hud_process(&gs, &_hud, &mkb, &info, 
                gst.army_bp+0, &gst.map_editor, &textd, &gst, 
                &netc, &nets, sounds);
                
            gst_process(&gst, &info, &fx, tot_time);
            fx_process(&fx, tot_time);
            
            render = true;
            unprocessed_time -= FRAME_TIME;
            mlast[0] = mkb.mx; mlast[1] = mkb.my;
            mkb_process(&mkb);
        }

        if (render) {
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            SDL_RenderClear(rend);
            
            gst_render(rend, txsprites, &textd, &gst, &info, tot_time);
            
            fx_render(rend, &fx, gst.cam, tot_time);
            
            hud_render(&_hud, rend, &textd, &mkb, &info, txsprites, &gst,
                tot_time);
            
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