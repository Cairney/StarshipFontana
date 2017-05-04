#include "SFApp.h"

int score = 0;
int i = 0;
SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetPosition(player_pos);
  
  trip  = make_shared<SFAsset>(SFASSET_TRIP, sf_window);
  auto trip_pos = Point2(320, 240);
  trip->SetPosition(trip_pos);


  //This changes the mother from an array to an object like the player.
  mother  = make_shared<SFAsset>(SFASSET_MOTHER, sf_window);
  auto mother_pos = Point2(canvas_h/2, 400);
  mother->SetPosition(mother_pos);
  mothers.push_back(mother);

  const int number_of_aliens = 8;
  for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i it also helps you place the aliens on the map with the auto pos line.
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2((canvas_w/number_of_aliens) * i, 200.0f);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }
  
  const int number_of_barriers = 9;
  for (int i=0; i<number_of_barriers; i++){
    //This helps place any number of barriers on the map that you as the programmer diesire.
    auto barrier = make_shared<SFAsset>(SFASSET_BARRIER, sf_window);
    auto pos = Point2((canvas_h/number_of_barriers) * (i*2), 100.0f);
    barrier->SetPosition(pos);
    barriers.push_back(barrier);
  }
  const int number_of_meteors = 9;
  for (int i=0; i<number_of_meteors; i++){
    //This helps place any number of barriers on the map that you as the programmer diesire.
    auto meteor = make_shared<SFAsset>(SFASSET_METEOR, sf_window);
    auto pos = Point2((canvas_h/number_of_meteors) * (i*2), 300.0f);
    meteor->SetPosition(pos);
    meteors.push_back(meteor);
  }
   // This is the position of the coin and you can change it by editing the auto pos line.
  auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
  auto pos  = Point2((canvas_h/4) *1, 450.0f);
  coin->SetPosition(pos);
  coins.push_back(coin);
}

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
   COMMENT TO SELF CHANGE THIS WHEN YOU WANT TO MOVE SOMETHING OR MAKE SOMETHING DO SOMETHING!
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
  case SFEVENT_PLAYER_UP:
    player->GoNorth();
    break;
  case SFEVENT_PLAYER_DOWN:
    player->GoSouth();
    break;
  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
    break;
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

int SFApp::Counter(){
i++;
if (i== 1033) {
    i = 0;
  }
}
void SFApp::OnUpdateWorld() {
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

  for(auto c: coins) { // I have left it static because it allows the user to fight through a hord of enemeies to go and collect the survivor in the game to win.
    //c->GoNorth();
  }

  // Update enemy positions
  for(auto m : mothers) {
	Counter();
	if(i >= 500 && i < 530) {
        m ->GoWest2();
        break;
      }
        if (i >= 1000 && i < 1030) {
        m->GoEast2();
        break;
      }
     }
for(auto c : coins) {
	Counter();
	if(i >= 500 && i < 530) {
        c ->GoWest2();
        break;
      }
        if (i >= 1000 && i < 1030) {
        c->GoEast2();
        break;
      }
     }
 // Detect collisions between Projectile and barriers
  for(auto p : projectiles) {
    for(auto b : barriers) {
      if(p->CollidesWith(b)) {
        p->HandleCollision();
        b->HandleCollision();
        std::cout << "You hit a barrier maybe you wanna hit the enemy next time." << std::endl;
      }
    }
  }
// Detect collisions between Projectile and meteor
  for(auto p : projectiles) {
    for(auto m : meteors) {
      if(p->CollidesWith(m)) {
        p->HandleCollision();
        m->HandleCollision();
        std::cout << "AHH! YOU HIT A METEOR" << std::endl;
      }
    }
  }
  // Detect collisions between Projectile and Alien
  for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
	score++;
	std::cout << score << std::endl;
        std::cout << "You shot something do you wanna medal or something, if you complete the game then ill be impressed." << std::endl;
      }
    }
  }
  // Detect collisions between Projectile and Mother
  for(auto p : projectiles) {
      if(p->CollidesWith(mother)) {
        p->HandleCollision();
        mother->HandleCollision();
	score++;
	std::cout << score << std::endl;
        std::cout << "CALL COMMAND, A MOTHERSHIP HAS BEEN DESTROYED!" << std::endl;
      }
  }
 
   // Detect collision between Player and Barrier
   for(auto b : barriers) {
      if( player->CollidesWith(b)) {
        player->SetNotAlive();
        b->HandleCollision();
        std::cout << "YOU'RE A LOSER!" << std::endl;
    }
  }
 // Detect collision between Player and meteor
   for(auto m : meteors) {
      if( player->CollidesWith(m)) {
        player->SetNotAlive();
        m->HandleCollision();
        std::cout << "If you were in armageddon you would suck!" << std::endl;
    }
  }
  // Detect collision between Player and Alien
  for(auto a : aliens) {
    if( player->CollidesWith(a)) {
      player->SetNotAlive();
      a->HandleCollision();
      std::cout << "GAME OVER, YOU SUCK MAN! COME ON! GET GOOD, LOSER!" << std::endl;
    }
  }


  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);
    }
  } 
// This clears the dead alien bodies
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);
  tmp.clear();
  
 // Detect collision between Player and Mother
    if( player->CollidesWith(mother)) {
      player->SetNotAlive();
      mother->HandleCollision();
      std::cout << "GAME OVER!" << std::endl;
    }
  

  // Detects collision between Player and Coin
  for(auto c : coins) {
    if( player->CollidesWith(c)){
      c->SetNotAlive();
      std::cout <<"HE HAS DONE IT HARRY POTTER HAS COLLECTED THE GOLDEN SNITCH OH! wait its just the surviver well done you've completed the mission." << std::endl;
    }
  }


  // removes coin after collection
list<shared_ptr<SFAsset>> tmp3;
  for(auto c : coins){
    if(c->IsAlive()){
      tmp3.push_back(c);
    }
}
  
  coins.clear();
  coins = list<shared_ptr<SFAsset>>(tmp3);
  tmp3.clear();
}

//end of OnUpdateWorld

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());
  
trip->OnRender();
  // draw the player
  if(player->IsAlive()) {player->OnRender();}
 
  // Prints the projectiles on the screen.
  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }
  //Prints the dreaded aliens to the screen.
  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }
  
    if(mother->IsAlive()) {mother->OnRender();}
  
  //Prints the coin which is the wee space man and if you collect him you win.
  for(auto c: coins) {
    if(c->IsAlive()) {c->OnRender();}
  }
 //Prints the annoying blocks that you have to navigate past.
   for(auto b: barriers) {
    if(b->IsAlive()) {b->OnRender();}
  }
//Prints the annoying blocks that you have to navigate past.
   for(auto m: meteors) {
    if(m->IsAlive()) {m->OnRender();}
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());}


void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
