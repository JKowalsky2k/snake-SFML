#include <iostream>
#include <fstream>

#include "engine.h"

Engine::Engine(sf::RenderWindow & win, Snake & snake, Fruit & fruit, Scoreboard & scoreboard)
 : win(win), snake(snake), fruit(fruit), scoreboard(scoreboard) {
	win.create(sf::VideoMode(800, 800), "Snake");
	win.setFramerateLimit(60);
	
	if (!font.loadFromFile("font/ar.ttf")) {
		std::cout << "Nie udalo sie wczytac czcionki\n";
		abort();
	}

	if (!snake_font.loadFromFile("font/snake.ttf")) {
		std::cout << "Nie udalo sie wczytac czcionki\n";
		abort();
	}
	
	scoreboard.loadScoreboardFromFile();
	
	state = MENU;
}

void Engine::run() {
	stateMachine();
}

void Engine::stateMachine() {
	while (state != PERMEND) {
		switch (state) {
			case MENU:
				stateMENU();
			break;
			case PLAY:
				statePLAY();
			break;
			case END:
				stateEND();
			break;
			case SCOREBOARD:
				stateSCOREBOARD();
			break;
		}
	}
}

void Engine::stateMENU() {

	win.setTitle("Menu");

	int option = 0;

	sf::Clock press_delay_clock;
	press_delay_clock.restart();
	sf::Time press_delay;

	sf::Sprite 	map_sprite;
	sf::Texture map_texture[4];
	map_sprite.scale(0.4f, 0.4f);

	sf::RectangleShape cursor;
	cursor.setFillColor(sf::Color::White);

	sf::Text logo_text;
	logo_text.setFont(snake_font);
	logo_text.setFillColor(sf::Color::Magenta);
	logo_text.setCharacterSize(120);
	logo_text.setString("SNAKE");
	logo_text.setOrigin(logo_text.getLocalBounds().width/2, logo_text.getLocalBounds().height/2);
	logo_text.setPosition(sf::Vector2f(win.getSize().x/2, 60));


	sf::Text choose_map_text;
	choose_map_text.setFont(font);
	choose_map_text.setStyle(sf::Text::Bold);
	choose_map_text.setCharacterSize(40);
	choose_map_text.setString("Choose Map");
	choose_map_text.setOrigin(choose_map_text.getLocalBounds().width/2, choose_map_text.getLocalBounds().height/2);
	choose_map_text.setPosition(sf::Vector2f(win.getSize().x/2, 200));


	sf::Color grey (169,169,169);

	sf::Text hint_text;
	hint_text.setFont(font);
	hint_text.setFillColor(grey);
	hint_text.setStyle(sf::Text::Bold);
	hint_text.setCharacterSize(40);
	hint_text.setString("Press A/D to move the cursor");
	hint_text.setOrigin(hint_text.getLocalBounds().width/2, hint_text.getLocalBounds().height/2);
	hint_text.setPosition(sf::Vector2f(win.getSize().x/2, win.getSize().y-300));


	sf::Text confirmation_text;
	confirmation_text.setFont(font);
	confirmation_text.setStyle(sf::Text::Bold);
	confirmation_text.setCharacterSize(40);
	confirmation_text.setString("Press SPACE to play");
	confirmation_text.setOrigin(confirmation_text.getLocalBounds().width/2, confirmation_text.getLocalBounds().height/2);
	confirmation_text.setPosition(sf::Vector2f(win.getSize().x/2, win.getSize().y-230));


	// wczytywanie textur
	for (int i = 0; i < 4; ++i) {
	 	if (!map_texture[i].loadFromFile("img/map"+std::to_string(i+1)+".png")) {
	 		std::cout << "Nie udalo sie wczytac map\n";
			abort();
	 	}
	}	

	// obliczanie przydatnych inforamcji potrzebych do pozycjonowania
	int sprite_size_after_resize = map_sprite.getScale().x*400;
	int gap_between_sprites = (win.getSize().x-4*sprite_size_after_resize)/5;

	// ustawianie wiekosci "kursora"
	cursor.setSize(sf::Vector2f(sprite_size_after_resize, 10));

	while (state == MENU) {
	    sf::Event event;
	    while (win.pollEvent(event)) {
	        if (event.type == sf::Event::Closed) {
	        	scoreboard.saveScoreboardToFile();
	            state = PERMEND;
	            win.close();
	        }

	        press_delay = press_delay_clock.getElapsedTime();
		    if (press_delay >= sf::seconds(0.05f)) {
		        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					if (option == 0) 
						option = 3;
					else
						option--;
		        }

		        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					if (option == 3) 
						option = 0;
					else
						option++;     
		        }

		        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

		        	switch (option) {
		        		case 0: {
		        			reset("map1.txt");
		        		}
		        		break;
		        		case 1: {
							reset("map2.txt");
		        		}
		        		break;
		        		case 2: {
		        			reset("map3.txt");
		        		}
		        		break;
		        		case 3: {
		        			reset("map4.txt");
		        		}
		        		break;
		        	}


					state = PLAY;
		        }

				press_delay_clock.restart();
			}
	    }

	    win.clear();
	    
	    // rysuje grafiki map
	    for (int i = 0; i < 4; ++i) {
	    	map_sprite.setPosition(sf::Vector2f((
	    		i*sprite_size_after_resize+i*gap_between_sprites)+gap_between_sprites, 
	    		win.getSize().y/3));
	    	map_sprite.setTexture(map_texture[i]);
	    	win.draw(map_sprite);
	    }

	    // ustawia "kursor" w odpowiedniej pozycji
	    cursor.setPosition(sf::Vector2f((
	    	option*sprite_size_after_resize+option*gap_between_sprites)+gap_between_sprites, 
	    	win.getSize().y/3+sprite_size_after_resize+20));

	    win.draw(logo_text);
	    win.draw(cursor);
	    win.draw(choose_map_text);
	    win.draw(hint_text);
	    win.draw(confirmation_text);

	    win.display();	
	}
}

void Engine::statePLAY() {

	win.setTitle("Snake");

	sf::Clock update_clock;
	sf::Clock change_direction_clock;

	update_clock.restart();
	change_direction_clock.restart();
	
	sf::Time update_delay;
	sf::Time change_direction_delay;

	while (state == PLAY) {
	    sf::Event event;
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				scoreboard.saveScoreboardToFile();
				state = PERMEND;
				win.close();
			}

			change_direction_delay = change_direction_clock.getElapsedTime();
			if (change_direction_delay >= sf::seconds(one_update_time)) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					snake.changeDirection(Direction::UP);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					snake.changeDirection(Direction::DOWN);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					snake.changeDirection(Direction::LEFT);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					snake.changeDirection(Direction::RIGHT);

				change_direction_clock.restart();
			}
		}		

		// teleportowanie weza na drugi koniec mapy
		if (snake.getHeadCoords().row < 0)
			snake.moveHead(board_height-1, snake.getHeadCoords().col);

		if (snake.getHeadCoords().row >= board_height)
			snake.moveHead(0, snake.getHeadCoords().col);

		if (snake.getHeadCoords().col < 0)
			snake.moveHead(snake.getHeadCoords().row, board_width-1);

		if (snake.getHeadCoords().col >= board_width)
			snake.moveHead(snake.getHeadCoords().row, 0);

		if (snake.getHeadCoords().row == fruit.getFruitCoords().row and 
			snake.getHeadCoords().col == fruit.getFruitCoords().col) {
			snake.grow();
			setFruit();
			score++;
			win.setTitle("Score: "+std::to_string(score));
		}

		//sprawdzanie czy waz udezyl w siebie albo w przeszkode
		if (getGameStatus() == GameStatus::END){
			state = END;
		}

		update_delay = update_clock.getElapsedTime();
		if (update_delay >= sf::seconds(one_update_time)) {
			snake.update();
			update_clock.restart();
		}

		win.clear(sf::Color::Black);
		drawGame();
		win.display();
	}
}

void Engine::stateEND() {
	win.setTitle("Game Over");
	std::string user_str;

	sf::Text top_score_text;
	top_score_text.setFont(snake_font);
	top_score_text.setStyle(sf::Text::Bold);
	top_score_text.setFillColor(sf::Color::Magenta);
	top_score_text.setCharacterSize(120);
	top_score_text.setString("New Top Score");
	top_score_text.setOrigin(top_score_text.getLocalBounds().width/2, top_score_text.getLocalBounds().height/2);
	top_score_text.setPosition(sf::Vector2f(win.getSize().x/2, 100));

	sf::Text player_score_text;
	player_score_text.setFont(font);
	player_score_text.setStyle(sf::Text::Bold);
	player_score_text.setCharacterSize(50);
	player_score_text.setString("Score: "+std::to_string(score));
	player_score_text.setOrigin(player_score_text.getLocalBounds().width/2, player_score_text.getLocalBounds().height/2);
	player_score_text.setPosition(sf::Vector2f(win.getSize().x/2, 260));	

	sf::Text enter_text;
	enter_text.setFont(font);
	enter_text.setStyle(sf::Text::Bold);
	enter_text.setFillColor(sf::Color::Yellow);
	enter_text.setCharacterSize(40);
	enter_text.setString("Enter your name:        ");
	enter_text.setOrigin(enter_text.getLocalBounds().width/2, 0);
	enter_text.setPosition(sf::Vector2f(win.getSize().x/2-40, 370));

	sf::Text user_text;
	user_text.setFont(font);
	user_text.setCharacterSize(40);
	user_text.setString(".....");
	user_text.setPosition(sf::Vector2f(win.getSize().x/2+100, 370));

	sf::Text confirmation_text;
	confirmation_text.setFont(font);
	confirmation_text.setStyle(sf::Text::Bold);
	confirmation_text.setCharacterSize(40);
	confirmation_text.setString("Press SPACE to confirm");
	confirmation_text.setOrigin(confirmation_text.getLocalBounds().width/2, confirmation_text.getLocalBounds().height/2);
	confirmation_text.setPosition(sf::Vector2f(win.getSize().x/2, win.getSize().y-230));

	if (score <= scoreboard.getLastScore()) {
		state = SCOREBOARD;
	}

	while (state == END) {
		sf::Event event;
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				scoreboard.saveScoreboardToFile();
				state = PERMEND;
				win.close();
			}

			if (event.type == sf::Event::TextEntered) {
				int user_input = event.text.unicode;

				// warunek, zebgty wczytywac tylko A-Z i a-z 
				if ((user_input >= 65 and user_input <= 90) or
					(user_input >= 97 and user_input <= 122)) {
					if (user_str.length() < 5) {
						user_str += event.text.unicode;
					}
				}

				// warunek na backspace 
				if (user_input == 8) {
					if (user_str.length() > 0) {
						user_str.erase(user_str.length()-1);
					}
				}

				// warunek na spacje ktora dziala jak enter
				if (user_input == 32) {
					scoreboard.add(user_str ,score);
					state = SCOREBOARD;
				}

				user_text.setString(user_str);
			}
		}

		win.clear();
		win.draw(top_score_text);
		win.draw(player_score_text);
		win.draw(enter_text);
		win.draw(user_text);
		win.draw(confirmation_text);
		win.display();				
	}
}

// Rysuje tylko scoreboard
void Engine::stateSCOREBOARD() {

	win.setTitle("Scoreboard");

	sf::Text table_name_text;
	table_name_text.setFont(font);
	table_name_text.setCharacterSize(50);
	table_name_text.setString("Name");
	table_name_text.setStyle(sf::Text::Bold);
	table_name_text.setPosition(sf::Vector2f(win.getSize().x/2-(table_name_text.getLocalBounds().width+70), 200));

	sf::Text table_score_text;
	table_score_text.setFont(font);
	table_score_text.setCharacterSize(50);
	table_score_text.setString("Score");
	table_score_text.setStyle(sf::Text::Bold);
	table_score_text.setPosition(sf::Vector2f(win.getSize().x/2+70, 200));

	sf::Text score_text;
	score_text.setFont(font);
	score_text.setCharacterSize(40);

	sf::Text confirmation_text;
	confirmation_text.setFont(font);
	confirmation_text.setStyle(sf::Text::Bold);
	confirmation_text.setCharacterSize(40);
	confirmation_text.setString("Press SPACE to restart");
	confirmation_text.setOrigin(confirmation_text.getLocalBounds().width/2, confirmation_text.getLocalBounds().height/2);
	confirmation_text.setPosition(sf::Vector2f(win.getSize().x/2, win.getSize().y-160));

	sf::RectangleShape separator;
	separator.setFillColor(sf::Color::Magenta);
	separator.setSize(sf::Vector2f(500, 3));
	separator.setOrigin(separator.getLocalBounds().width/2, separator.getLocalBounds().height/2);
	separator.setPosition(sf::Vector2f(win.getSize().x/2, 270));

	while (state == SCOREBOARD) {
		sf::Event event;
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				scoreboard.saveScoreboardToFile();
				state = PERMEND;
				win.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				state = MENU;
		}

		win.clear();
		win.draw(table_name_text);
		win.draw(table_score_text);
		win.draw(separator);

		int i = 0;
		/*
		for (size_t i = 0; i < scoreboard.getVector().size(); ++i) {
			score_text.setString(scoreboard.getVector()[i].name);
			score_text.setPosition(table_name_text.getPosition().x, i*50+300);
			win.draw(score_text);
			score_text.setString(std::to_string(scoreboard.getVector()[i].score));
			score_text.setPosition(table_score_text.getPosition().x, i*50+300);
			win.draw(score_text);			
		}
		*/

		std::cout << "b: " << &(*scoreboard.BEGIN()) << "\n";
		std::cout << "e: " << &(*scoreboard.END()) << "\n";	

		/*
		for (auto it = scoreboard.getVector().begin(); it != scoreboard.getVector().end(); ++it) {
			score_text.setString(it->name);
			score_text.setPosition(table_name_text.getPosition().x, i*50+300);
			win.draw(score_text);
			score_text.setString(std::to_string(it->score));
			score_text.setPosition(table_score_text.getPosition().x, i*50+300);
			win.draw(score_text);
			i++;
		}
		*/
		
		for (auto it = scoreboard.BEGIN(); it != scoreboard.END(); ++it) {
			score_text.setString(it->name);
			score_text.setPosition(table_name_text.getPosition().x, i*50+300);
			win.draw(score_text);
			score_text.setString(std::to_string(it->score));
			score_text.setPosition(table_score_text.getPosition().x, i*50+300);
			win.draw(score_text);
			i++;
		}

		win.draw(confirmation_text);
		win.display();	
	}
}

void Engine::drawGame() {
	sf::RectangleShape rect;
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(1.0f);
	rect.setSize(sf::Vector2f(field_size, field_size));
	for (int row = 0; row < board_width; ++row) {
		for (int col = 0; col < board_height; ++col) {
			rect.setPosition(sf::Vector2f(col*field_size, row*field_size));
			if (snake.isPartOfSnake(row, col))
				rect.setFillColor(sf::Color::Green);
			else if (fruit.isFruit(row, col))
				rect.setFillColor(sf::Color::Yellow);
			else if (isWall(row, col))
				rect.setFillColor(sf::Color::Magenta);
			else
				rect.setFillColor(sf::Color::Blue);

			win.draw(rect);
		}
	}
}

void Engine::drawText() {
	for (int row = 0; row < board_width; ++row) {
		for (int col = 0; col < board_height; ++col) {
			if (snake.isPartOfSnake(row, col))
				std::cout << "O";
			else if (fruit.isFruit(row, col))
				std::cout << "F";
			else if (isWall(row, col))
				std::cout << "#";
			else
				std::cout << "_";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

std::string Engine::debugMsg(int row, int col) {
	std::string msg;
	
	if (snake.isPartOfSnake(row, col))
		msg += "[S";
	else 
		msg += "[.";

	if (fruit.isFruit(row, col))
		msg += "F";
	else 
		msg += ".";

	if (isWall(row, col))
		msg += "W]";
	else 
		msg += ".]";		
	
	return msg;
}

void Engine::debugDisplay() {
	std::cout << "\n**** DEBUG MODE ****\n";
	for (int row = 0; row < board_width; ++row) {
		for (int col = 0; col < board_height; ++col) {
			std::cout << debugMsg(row, col);
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Engine::reset(std::string file_name) {
	// wymiary planszy zawsze beda 40x40
	for (int row = 0; row < board_width; ++row) {
		for (int col = 0; col < board_height; ++col) {
			board[row][col] = '_';
		}
	}

	loadMapFromFile(file_name);
	setSnake();
	setFruit();
	score = 0;
	game_status = GameStatus::RUNNING;
}

bool Engine::isInBoard(int row, int col) const {
	if (row < 0 or row >= board_width or col < 0 or col >= board_height)
		return false;
	
	return true;
}

GameStatus Engine::getGameStatus() const {
	if (hitWall())
		return GameStatus::END;

	if (snake.collisionWithOtherPartOfSnake())
		return GameStatus::END;

	return GameStatus::RUNNING;
}

void Engine::setSnake() {
	Coordinates new_snake;
	do {
		new_snake.row = rand()%board_width%2 + board_width/2;
		new_snake.col = rand()%board_height%2 + board_height/2;
	} while (isWall(new_snake.row, new_snake.col));

	snake.reset(new_snake.row, new_snake.col);
}	

void Engine::setFruit() {
	Coordinates new_fruit;
	do {
		new_fruit.row = rand()%board_width;
		new_fruit.col = rand()%board_height;
	} while (snake.isPartOfSnake(new_fruit.row, new_fruit.col) 
		or isWall(new_fruit.row, new_fruit.col));
	fruit.setFruitCoords(new_fruit.row, new_fruit.col);
}

bool Engine::isWall(int row, int col) const {
	if (board[row][col] == '#')
		return true;

	return false;
}

bool Engine::hitWall() const {
	if (isWall(snake.getHeadCoords().row, snake.getHeadCoords().col))
		return true;

	return false;
}

void Engine::loadMapFromFile(std::string file_name) {
	
	std::ifstream snake_map;
	snake_map.open("maps/"+file_name);
	if (!snake_map.good()) {
		std::cout << "Nie udalo sie wczytac mapy!\n";
		abort();
	}

	for (int row = 0; row < board_width; ++row) {
		for (int col = 0; col < board_height; ++col) {
			snake_map >> board[row][col];
		}
	}

	snake_map.close();
}