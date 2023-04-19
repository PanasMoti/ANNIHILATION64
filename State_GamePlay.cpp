//
// Created by juicylucy on 4/16/23.
//

#include "State_GamePlay.h"
#include <iostream>

const char* arr[] = {
        "Forward_Press",
        "Left_Press",
        "Right_Press",
        "Backward_Press",
// -----------------------------
        "Forward_Release",
        "Left_Release",
        "Right_Release",
        "Backward_Release"
};

State_GamePlay::State_GamePlay(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_GamePlay::OnCreate() {
//    m_stateMgr->GetContext()->window->create_buffer();
    keys_state[119] = false;
    keys_state[97] = false;
    keys_state[100] = false;
    keys_state[115] = false;
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    for(int i = 0; i < 8; i++) {
        if(i < 4) {
            evMgr->AddCallback(StateType::GamePlay,arr[i],&State_GamePlay::KeyPressed,this);
        } else {
            evMgr->AddCallback(StateType::GamePlay,arr[i],&State_GamePlay::KeyReleased,this);
        }
    }
}

void State_GamePlay::OnDestroy() {
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;

    for(int i = 0; i < 8; i++) {
        evMgr->RemoveCallback(StateType::GamePlay,arr[i]);
    }
}

void State_GamePlay::Activate() {
    //TODO: process the level into the map;
    std::cout << GameData::self().level << std::endl;
    m_stateMgr->GetContext()->window->create_buffer();
    GameData& gameData = GameData::self();
    Map map;
    std::string level = gameData.level;
    auto find_str = [](std::string s,std::string del) {
        int end = s.find(del);
        std::vector<std::string> vec;
        while(end != -1) {
            // Loop until no delimiter is left in the string
            vec.push_back(s.substr(0,end));
            s.erase(s.begin(),s.begin()+end+1);
            end = s.find(del);
        }
        std::cout << s.substr(0,end) << std::endl;
        return vec;
    };
    auto vec = find_str(level,"\n");
    auto trim = [](std::string& str) {
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        str.erase(str.begin(),str.begin() + str.find("=") + 1);
    };
    std::for_each(vec.begin(),vec.end(),trim);
//    std::for_each(vec.begin(),vec.end(),[](std::string& str){printf("[%s]\n",str.c_str());});
    //vec[0] = PASSWORD
    //vec[1] = WIDTH
    //vec[2] = HEIGHT
    //vec[3] = MAP
    //vec[4] = HP
    //vec[5] = AMMO
    int width = std::stoi(vec[1]);
    int height = std::stoi(vec[2]);
    int hp = std::stoi(vec[4]);
    int ammo = std::stoi(vec[5]);
    map.SetSize(width,height);
    map.SetMap(vec[3]);

    gameData.SetMap(map);

}

void State_GamePlay::Deactivate() {

}

void State_GamePlay::Update(float dt) {
//    std::cout << dt << std::endl;
    float moveSpeed = 5.0f*dt;
    float rotSpeed = 3.0f*dt;
    GameData& data = GameData::self();
    float dirX = data.player.dir.x;
    float dirY = data.player.dir.y;
    float planeX = data.player.plane.x;
    float planeY = data.player.plane.y;
    if(keys_state[SDLK_w]) {
        data.player.pos+=data.player.dir*moveSpeed;
    }
    if(keys_state[SDLK_s]) {
        data.player.pos-=data.player.dir*moveSpeed;
    }
    if(keys_state[SDLK_a]) {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        data.player.plane = {planeX,planeY};
        data.player.dir = {dirX,dirY};
    }
    if(keys_state[SDLK_d]) {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        data.player.plane = {planeX,planeY};
        data.player.dir = {dirX,dirY};
    }
}

void State_GamePlay::Draw() {
    GameData& data = GameData::self();
    RenderWindow* window = m_stateMgr->GetContext()->window;
    int w = data.res.x; int h = data.res.y;

    window->clear_buffer();

    for(int x = 0; x < w; x++) {
        float camX = 2*x/float(w) - 1; // x coordinate in camera space
        float2 raydir = data.player.dir + data.player.plane*camX;
        //which box of the map we're in
        int mapX = data.map.GetPlayerSpawn().x;
        int mapY = data.map.GetPlayerSpawn().y;
        //length of ray from current position to next x or y-side
        float2 sideDist;
        //length of ray from one x or y-side to next x or y-side
        float2 deltaDist = {
                (raydir.x == 0) ? 1e30 : std::abs(1/raydir.x),
                (raydir.y == 0) ? 1e30 : std::abs(1/raydir.y)
        };
        float perpWallDist;
        //what direction to step in x or y-direction (either +1 or -1)
        int2 step;
        int hit = 0; // was there a wall hit?
        int side; // was a NS or an EW wall hit?
        //calculate step and initial sideDist
        if(raydir.x < 0) {
            step.x = -1;
            sideDist.x = (data.player.pos.x -mapX)*deltaDist.x;
        } else {
            step.x = 1;
            sideDist.x = (mapX + 1.0 - data.player.pos.x)*deltaDist.x;
        }
        if(raydir.y < 0) {
            step.y = -1;
            sideDist.y = (data.player.pos.y - mapY)*deltaDist.y;
        } else {
            step.y = 1;
            sideDist.y = (mapY + 1.0 - data.player.pos.y)*deltaDist.y;

        }
        //perform DDA
        while(hit == 0) {
            //jump to next map square, either in x-direction, or in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapX += step.x;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapY += step.y;
                side = 1;
            }
            //Check if ray has hit a wall
            if (data.map(mapX,mapY) != CellType::Empty) hit = 1;

            //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
            if(side == 0) perpWallDist = (sideDist.x - deltaDist.x);
            else          perpWallDist = (sideDist.y - deltaDist.y);

            //Calculate height of line to draw on screen
            int lineHeight = (int)(h / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + h / 2;
            if(drawStart < 0)drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if(drawEnd >= h)drawEnd = h - 1;
            SDL_Color color = ToColor(data.map(mapX,mapY));
            window->render_vertical_line(x,drawStart,drawEnd,color);
        }

    }
    window->update();
}

void State_GamePlay::KeyPressed(EventDetails *l_details) {
    int keycode = l_details->keyCode;
    keys_state[keycode] = true;


}

void State_GamePlay::KeyReleased(EventDetails *l_details) {
    int keycode = l_details->keyCode;
    keys_state[keycode] = false;
}
