//
// Created by juicylucy on 4/16/23.
//

#include "State_GamePlay.h"
#include <iostream>
#include "gfx/GameEntities.h"

const char* arr[] = {
        "Forward_Press",
        "Left_Press",
        "Right_Press",
        "Backward_Press",
        "Shoot_Press",
        "GamePlay_Interact_Press",
// -----------------------------
        "Forward_Release",
        "Left_Release",
        "Right_Release",
        "Backward_Release",
        "Shoot_Release",
        "GamePlay_Interact_Release"
};

State_GamePlay::State_GamePlay(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_GamePlay::OnCreate() {
//    ZBuffer.resize(GameData::self().res.x);
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    for(int i = 0; i < 12; i++) {
        if(i  < 6) {
            evMgr->AddCallback(StateType::GamePlay,arr[i],&State_GamePlay::KeyPressed,this);
        } else {
            evMgr->AddCallback(StateType::GamePlay,arr[i],&State_GamePlay::KeyReleased,this);
        }
    }
    evMgr->AddCallback(StateType::GamePlay,"DEBUG_LOSE_HP",&State_GamePlay::DebugLoseHP,this);
    evMgr->AddCallback(StateType::GamePlay,"DEBUG_GAIN_HP",&State_GamePlay::DebugGainHP,this);
    RenderWindow* window = m_stateMgr->GetContext()->window;
    doorTexture = GameTexture("assets/door.png");
    wallTexture = GameTexture("assets/redbrick.png");
    heart_img = new Texture(window->loadTexture("assets/heart.png"));
    heart_img->Scale(10.5f);
    bullet_img = new Texture(window->loadTexture("assets/bullet.png"));
    bullet_img->Scale(2.0f);
    info_font = window->loadFont("assets/YosterIsland.ttf");
    info_text = "^use [WASD] to move\n^use [F] to shoot\n^use [E] to interact\n^the barrels will restore your ammo\n#~~GOAL~~#\n *kill all the enemies\n *FIND:\n  -green british cat flower";
    gunSprite = new Sprite("assets/pistol", window->GetRenderer());
    gunSprite->SetSize(500, 250);
    gunSprite->time_between_frames = 0.1f;
    ZBuffer.resize(GameData::self().res.x);


}

void State_GamePlay::OnDestroy() {
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;

    for(int i = 0; i < 6; i++) {
        evMgr->RemoveCallback(StateType::GamePlay,arr[i]);
    }
    evMgr->RemoveCallback(StateType::GamePlay,"DEBUG_LOSE_HP");
    evMgr->RemoveCallback(StateType::GamePlay,"DEBUG_GAIN_HP");
}

void State_GamePlay::Activate() {
    player_shot = false;
    //TODO: process the level into the map;
//    std::cout << GameData::self().level << std::endl;
    m_stateMgr->GetContext()->window->create_buffer();
    GameData& gameData = GameData::self();
    hit_cooldown = {0.0f,1.0f};
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
    gameData.score = 0;
    gameData.SetMap(map);
    gameData.player.hp = hp;
    gameData.player.ammo = ammo;
    GameEntities& sprite = GameEntities::self();
    sprite.AddTexture("assets/sprite/barrel.png");
    sprite.AddTexture("assets/sprite/greenlight.png");
    sprite.AddTexture((vec[0] == "FNAF") ? "assets/sprite/DONOTUSE.png" : "assets/sprite/enemy1.png");
    sprite.AddTexture("assets/sprite/level_end.png");
    auto enemy_vec = gameData.map.GetEnemiesSpawn();
    for(auto enemy: enemy_vec) {
        sprite.AddEntity(static_cast<float>(enemy.x) + 0.5,static_cast<float>(enemy.y) + 0.5,2);
    }
    auto item_vec = gameData.map.GetItemsSpawn();
    for(auto item: item_vec) {
        sprite.AddEntity(static_cast<float>(item.x) + 0.5,static_cast<float>(item.y) + 0.5,0);
    }
    auto p = gameData.map.GetPlayerSpawn();
    sprite.AddEntity(static_cast<float>(p.x) -0.5,static_cast<float>(p.y)-0.5,1);
    auto l = gameData.map.GetLevelEnd();
    sprite.AddEntity(static_cast<float>(l.x)-0.5,static_cast<float>(l.y)-0.5,3);

    gameOver =false;

}

void State_GamePlay::Deactivate() {
    GameEntities& sprite = GameEntities::self();
    sprite.entity.clear();
    sprite.entities.clear();
    sprite.textures.clear();
    gameOver = false;
    keys_state.clear();
}

void State_GamePlay::Update(float dt) {
//    std::cout << dt << std::endl;
    if(player_shot) player_shot = false;
    auto& sprite = GameEntities::self();
    GameData& data = GameData::self();


    if(data.player.hp <= 0) {
        gameOver = true;
    }
    hit_cooldown.x+=dt;


    auto dist = [](float x1,float y1,float x2,float y2) {
        float dx = x2-x1;
        float dy = y2-y1;
        return sqrtf((dx*dx)+(dy*dy));
    };
    int i = 0; int count_enemies = 0;
    for(auto ent : sprite.entity) {
        if(dist(data.player.pos.x,data.player.pos.y,ent.x,ent.y) <= 0.5f) {
            PlayerEntityCollision(i);

        }
        if(ent.texture == 2) count_enemies++;
        i++;
    }
    if(count_enemies == 0) data.map.FinishLevel();


    if(gameOver) GameOver();

    gunSprite->Update(dt);
    float moveSpeed = 5.0f*dt;
    float rotSpeed = 3.0f*dt;
    float dirX = data.player.dir.x;
    float dirY = data.player.dir.y;
    float planeX = data.player.plane.x;
    float planeY = data.player.plane.y;
    if(keys_state[SDLK_w]) {
        int m0x = static_cast<int>(data.player.pos.x);
        int m0y = static_cast<int>(data.player.pos.y);
        int mx = static_cast<int>(data.player.pos.x + dirX*moveSpeed);
        int my = static_cast<int>(data.player.pos.y + dirY*moveSpeed);
        if(data.map(mx,m0y) != CellType::Wall && data.map(mx,m0y) != CellType::Door) {
            data.player.pos.x += data.player.dir.x*moveSpeed;
        }
        if(data.map(m0x,my) != CellType::Wall && data.map(m0x,my) != CellType::Door) {
            data.player.pos.y += data.player.dir.y*moveSpeed;
        }
    }
    if(keys_state[SDLK_s]) {
        int m0x = static_cast<int>(data.player.pos.x);
        int m0y = static_cast<int>(data.player.pos.y);
        int mx = static_cast<int>(data.player.pos.x - dirX*moveSpeed);
        int my = static_cast<int>(data.player.pos.y - dirY*moveSpeed);
        if(data.map(mx,m0y) != CellType::Wall && data.map(mx,m0y) != CellType::Door ) {
            data.player.pos.x -= data.player.dir.x*moveSpeed;
        }
        if(data.map(m0x,my) != CellType::Wall && data.map(m0x,my)!= CellType::Door) {
            data.player.pos.y -= data.player.dir.y*moveSpeed;
        }
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
    if(keys_state[SDLK_f]) {
        if(data.player.cooldown == SHOOTINGCOOLDOWN) {
            if(!gunSprite->isPlaying()) {
                gunSprite->StartPlaying();
                if(--data.player.ammo < 0) data.player.ammo = 0;
            }
            player_shot = true;
        }
        data.player.cooldown-=dt;

        if(data.player.cooldown <= 0 ) {
            if(!gunSprite->isPlaying()) {
                if(--data.player.ammo < 0) data.player.ammo = 0;
                data.player.cooldown = SHOOTINGCOOLDOWN;
                gunSprite->StartPlaying();
            }
            player_shot = true;
        }
    }
    if(!keys_state[SDLK_f]) {
        if(data.player.cooldown < SHOOTINGCOOLDOWN) {
            data.player.cooldown-=dt;
        }
    }
    if(keys_state[SDLK_e]) {
        int m0x = static_cast<int>(data.player.pos.x);
        int m0y = static_cast<int>(data.player.pos.y);
        int mx = static_cast<int>(data.player.pos.x + dirX*moveSpeed);
        int my = static_cast<int>(data.player.pos.y + dirY*moveSpeed);
        if(data.map(mx,my) == CellType::Door) {
            data.map(mx,my) = CellType::Empty;
        }
    }
    EnemyAI(dt);

}

void State_GamePlay::Draw() {
    RenderBuffer();
    RenderHud();
}

void State_GamePlay::KeyPressed(EventDetails *l_details) {
    int keycode = l_details->keyCode;
    keys_state[keycode] = true;


}

void State_GamePlay::KeyReleased(EventDetails *l_details) {
    int keycode = l_details->keyCode;
    keys_state[keycode] = false;
}

void State_GamePlay::RenderBuffer() {
    GameData& data = GameData::self();
    RenderWindow* window = m_stateMgr->GetContext()->window;
    int w = data.res.x; int h = data.res.y;
    auto& sprite = GameEntities::self();
    window->clear_buffer();
    // WALL CASTING
    // -------------------------
    for(int x = 0; x < w; x++) {
        float camX = 2*x/float(w) - 1; // x coordinate in camera space
        float2 raydir = data.player.dir + data.player.plane*camX;
        //which box of the map we're in
        int mapX = static_cast<int>(data.player.pos.x);
        int mapY = static_cast<int>(data.player.pos.y);
        //length of ray from current position to next x or y-side
        float2 sideDist;
        //length of ray from one x or y-side to next x or y-side
        float2 deltaDist = {
                (raydir.x == 0) ? static_cast<float>(1e30) : std::abs(1/raydir.x),
                (raydir.y == 0) ? static_cast<float>(1e30) : std::abs(1/raydir.y)
        };
        float perpWallDist;
        //what direction to step in x or y-direction (either +1 or -1)
        int2 step;
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
        while(true) {
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
            if (data.map(mapX,mapY) == CellType::Wall || data.map(mapX,mapY) == CellType::Door) break;

        } // DDA
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
        auto cell = data.map(mapX,mapY);
        if(cell == CellType::Door || cell == CellType::Wall) {
            GameTexture& texture = (cell == CellType::Door) ? doorTexture : wallTexture;
            float wallX;
            if (side == 0) wallX = data.player.pos.y+perpWallDist*raydir.y;
            else           wallX = data.player.pos.x+perpWallDist*raydir.x;
            wallX -= floor((wallX));

            int texWidth = texture.Width(); int texHeight = texture.Height();
            int texX = static_cast<int>(wallX*float(texWidth));
            if (side == 0 && raydir.x > 0) texX = texWidth - texX - 1;
            if (side == 1 && raydir.y < 0) texX = texWidth - texX - 1;
            float Tstep = 1.0 * texHeight / lineHeight;
            float texPos = (drawStart-h/2+lineHeight/2)*Tstep;
            for(int y = drawStart; y < drawEnd; y++) {
                int texY = static_cast<int>(texPos) % (texHeight);
                texPos += Tstep;
                uint32_t color = texture[texX + texY*texHeight];

                if(side == 1) color = (color >> 1) & 8355711;
                window->render(x,y,color);
            }

        }

        ZBuffer[x] = perpWallDist;
    } // WALL CASTING
    // -------------------------

    // SPRITE CASTING
    // -------------------------
    //sort sprites from far to close
    float posX = data.player.pos.x; float posY = data.player.pos.y; float planeX = data.player.plane.x;
    float planeY = data.player.plane.y; float dirX = data.player.dir.x; float dirY = data.player.dir.y;
    int numSprites = sprite.numEntities();
    sprite.SyncSize();
    for(int i = 0; i < numSprites; i++)
    {
        sprite.entities[i].first = i;
        sprite.entities[i].second = ((posX - sprite.entity[i].x) * (posX - sprite.entity[i].x) + (posY - sprite.entity[i].y) * (posY - sprite.entity[i].y)); //sqrt not taken, unneeded
    }
    sprite.SortSprites();

    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < numSprites; i++)
    {
        //translate sprite position to relative to camera
        double spriteX = sprite.entity[sprite.entities[i].first].x - posX;
        double spriteY = sprite.entity[sprite.entities[i].first].y - posY;

        //transform sprite with the inverse camera matrix
        // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        // [ planeY   dirY ]                                          [ -planeY  planeX ]

        double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

        int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

        //calculate height of the sprite on screen
        int spriteHeight = abs(int(h / (transformY))); //using 'transformY' instead of the real distance prevents fisheye
        //calculate lowest and highest pixel to fill in current stripe
        int drawStartY = -spriteHeight / 2 + h / 2;
        if(drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + h / 2;
        if(drawEndY >= h) drawEndY = h - 1;

        //calculate width of the sprite
        int spriteWidth = abs( int (h / (transformY)));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if(drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if(drawEndX >= w) drawEndX = w - 1;

        //loop through every vertical stripe of the sprite on screen
        for(int stripe = drawStartX; stripe < drawEndX; stripe++) {
            int texWidth = sprite.textures[sprite.entity[sprite.entities[i].first].texture].Width();
            int texHeight = sprite.textures[sprite.entity[sprite.entities[i].first].texture].Height();
            int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
            //the conditions in the if are:
            //1) it's in front of camera plane so you don't see things behind you
            //2) it's on the screen (left)
            //3) it's on the screen (right)
            //4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe]) {
                if(player_shot && sprite.entity[sprite.entities[i].first].texture == 2 && abs(w/2-stripe) <= 1) {
                    sprite.RemoveEntity(sprite.entities[i].first);
                    stripe = drawEndX;
                    player_shot = false;
                    data.score++;
                }
                else {
                    for (int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
                    {
                        int d = (y) * 256 - h * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
                        int texY = ((d * texHeight) / spriteHeight) / 256;
                        Uint32 color = sprite.textures[sprite.entity[sprite.entities[i].first].texture][
                                texWidth * texY +
                                texX]; //get current color from the texture
                        if ((color & 0x00FFFFFF) != 0)
                            window->render(stripe, y, color);
                    }
                }
            }
        }
    }
    // -------------------------
     window->update();
}

void State_GamePlay::RenderHud() {
    RenderWindow* window = m_stateMgr->GetContext()->window;
    GameData& data = GameData::self();
    int& hp = data.player.hp;
    int& ammo = data.player.ammo;
    auto screen_size = window->GetScreenSize();
    window->draw(info_text,info_font,0,0,600,clLIME);
    for(int i = 0; i < hp; i++) {
        window->draw(*heart_img,i*heart_img->GetWidth(),screen_size.y-heart_img->GetHeight());
    }
    int y = 0;
    for(int i = 0; i < ammo; i++) {
        if(i%10 == 0) y++;

        window->draw(*bullet_img,screen_size.x-(i%10)*bullet_img->GetWidth(),screen_size.y-y*bullet_img->GetHeight());
    }
    auto center = window->ScreenCenter();
    window->draw(gunSprite, center.x - gunSprite->GetWidth() / 2, center.y * 2 - gunSprite->GetHeight(), gunSprite->GetCurrentFrameIndex());
}

void State_GamePlay::DebugLoseHP(EventDetails *l_details) {
    GameData& data = GameData::self();
    if(--data.player.hp < 0) data.player.hp = 0;
}

void State_GamePlay::DebugGainHP(EventDetails *l_details) {
    GameData& data = GameData::self();
    if(++data.player.hp > 5) data.player.hp = 5;

}

void State_GamePlay::GameOver() {
    m_stateMgr->SwitchTo(StateType::GameOver);
}

void State_GamePlay::PlayerEntityCollision(int index) {
    GameEntities& sprite = GameEntities::self();
    GameData& data = GameData::self();
    auto& ent = sprite.entity[index];
    switch (ent.texture) {
        case 0:
            // item
//            std::cout << "item" << std::endl;
            if(data.player.ammo != 50) {
                data.player.ammo+=10;
                if(data.player.ammo > 50) data.player.ammo = 50;
                sprite.RemoveEntity(index);
            }
            break;
        case 1:
            // player spawn -- ignore
            break;
        case 2:
            if(hit_cooldown.x >= hit_cooldown.y) {
                if(--data.player.hp < 0) data.player.hp = 0;
                hit_cooldown.x = 0.0f;
            }
            break;
        case 3:
            // level end -- game over
            gameOver = true;
            break;
        default:
            // nothing currently
            break;
    }
}

void State_GamePlay::EnemyAI(float dt) {
    auto& sprite = GameEntities::self();
    std::vector<BaseEntity*> enemies;
    auto& data = GameData::self();
    for(auto& ent : sprite.entity) {
        if(ent.texture == 2) enemies.push_back(&ent);
    }
    for(auto* enemy: enemies) {
        float2 player_pos = GameData::self().player.pos;
        float2 enemy_pos = {enemy->x,enemy->y};
        float2 ray = player_pos-enemy_pos;
        int mapX = static_cast<int>(enemy_pos.x);
        int mapY = static_cast<int>(enemy_pos.y);
        float2 v = linalg::normalize(ray);
        float2 sideDist;
        //length of ray from one x or y-side to next x or y-side
        float2 deltaDist = {
                (v.x == 0) ? static_cast<float>(1e30) : std::abs(1/v.x),
                (v.y == 0) ? static_cast<float>(1e30) : std::abs(1/v.y)
        };
        bool is_seeing_player = true;
        //what direction to step in x or y-direction (either +1 or -1)
        int2 step;
        //calculate step and initial sideDist
        if(v.x < 0) {
            step.x = -1;
            sideDist.x = (enemy_pos.x -mapX)*deltaDist.x;
        } else {
            step.x = 1;
            sideDist.x = (mapX + 1.0 - enemy_pos.x)*deltaDist.x;
        }
        if(v.x < 0) {
            step.y = -1;
            sideDist.y = (enemy_pos.y - mapY)*deltaDist.y;
        } else {
            step.y = 1;
            sideDist.y = (mapY + 1.0 - enemy_pos.y)*deltaDist.y;
        }
        //perform DDA
        while(linalg::length(sideDist) < linalg::length(ray)) {
            //jump to next map square, either in x-direction, or in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapX += step.x;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapY += step.y;
            }
            //Check if ray has hit a wall
            if (data.map(mapX,mapY) == CellType::Wall || data.map(mapX,mapY) == CellType::Door) { is_seeing_player = false;break; }

        } // DDA
        auto l = linalg::length(ray);
        if(is_seeing_player) {
            enemy->x += v.x*0.3f*dt*l;
            enemy->y += v.y*0.3f*dt*l;
        }
    }
}


