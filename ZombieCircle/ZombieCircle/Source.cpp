#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include "windows.h"
#include "Header.h"
#include <vector>
#include <time.h>

string getAppPath();
string getAssetsPath();

int main()
{	
	// Texte

	string numb = "level : 1";
	sf::Font arial;
	arial.loadFromFile(getAssetsPath() + "\\arial.ttf");
	sf::Text text;
	text.setFont(arial);	
	text.setCharacterSize(40);
	text.setPosition(20.f, 10.f);
	

	srand(time(NULL));
	// Fen�tre

	RenderWindow window(VideoMode(1200, 950), "ZombieCircle");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	float xwindow = window.getSize().x;
	float ywindow = window.getSize().y;

	
	// Player

	CircleShape player;
	player.setRadius(25.f);
	player.setFillColor(Color::Magenta);
	player.setPosition(xwindow / 2, ywindow);

	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	// Projectiles

	Bullet b1;
	shooterBullet shooterB1;
	shooterBullet bBoss;


	vector<Bullet> bullets;
	vector<shooterBullet> shooterBullets;

	//Munition

	int maxammo = 25;
	int currentammo = 5;
	float reloadtime = 1.f;
	cout << "Ammo : " << currentammo << endl;

	bool canR = true;
	float rCooldownMax = 150.f;
	float rCooldown = 0.f;

	//Affichage Munition

	Text currentammotext;
	string ammostri = to_string(currentammo);
	currentammotext.setString(ammostri);
	currentammotext.setFont(arial);
	currentammotext.setFillColor(Color::White);
	currentammotext.setCharacterSize(25);
	currentammotext.setPosition(500.f, 10.f);

	Text maxammotext;
	string maxammostri = to_string(maxammo);
	maxammotext.setString(maxammostri);
	maxammotext.setFont(arial);
	maxammotext.setCharacterSize(25);
	maxammotext.setPosition(1000.f, 10.f);

	

	



	//Cooldown

	bool canAttack = true;
	float attackCooldownMax = 10.f;
	float attackCooldown = 10.f;

	// Player en vie 

	bool isAlive = true;

	// Enemies

	bool shooterSpawned = false;

	std::vector<Enemy> rusherEnemy = SpawnEnemyRusher(4);
	std::vector<Enemy> rusherEnemy2 = SpawnEnemyRusher(6);
	std::vector<Enemy> rusherEnemy3 = SpawnEnemyRusher(8);
	std::vector<Enemy> rusherEnemy4 = SpawnEnemyRusher(8);
	std::vector<Shooter> shooterEnemy = SpawnEnemyShooter(2);

	// BOSS
	Boss boss;
	const double Pi = 3.14159265359;
	// Background

	RectangleShape bg(Vector2f(1200, 900));
	FloatRect bgRect = bg.getLocalBounds();
	bg.setOrigin(bgRect.left + bgRect.width / 2.0f, bgRect.top + bgRect.height / 2.0f);
	bg.setPosition(window.getView().getCenter());
	bg.setFillColor(Color::Black);
	Clock clocked;

	// Levels

	int completion = 0;
	bool level1 = true;
	bool level2 = false;
	bool level3 = false;
	bool level4 = false;
	bool levelBoss = false;
	bool hasAppeared = false;

	//Ammo
	Ammo ammo;
	std::vector<Ammo> ammoBox;
	int ammoSpawnPourcent = 25;
	
	/*ammoBox.push_back(ammo);*/
	
	CircleShape zebi;
	zebi.setFillColor(Color::Red);
	zebi.setRadius(20);

	while (window.isOpen())
	{

		// Inputs
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;

			default:
				break;

			}
		}


		// Logique

		Time elapsedTime = clocked.restart(); //< Calcul du temps ecoule depuis la derniere boucle

		// Mise � jour 
		// Vecteurs
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / static_cast<float>(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

		

		//D�placement du joueur 
		if (Keyboard::isKeyPressed(Keyboard::Q) && isAlive == true)
			player.move(-7.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::D) && isAlive == true)
			player.move(7.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::Z) && isAlive == true)
			player.move(0.f, -7.f);
		if (Keyboard::isKeyPressed(Keyboard::S) && isAlive == true)
			player.move(0.f, 7.f);

		// Mort du joueur

		if (isAlive == false) {
			player.setFillColor(Color::Transparent);
		}

		//Tir du joueur 
		if (Mouse::isButtonPressed(Mouse::Left) && canAttack && currentammo > 0 && isAlive) {
			if (Mouse::isButtonPressed(Mouse::Left)) {
				b1.shape.setPosition(playerCenter);
				b1.currVelocity = aimDirNorm * b1.maxSpeed;

				bullets.push_back(Bullet(b1));
				currentammo--;

				canAttack = false;
				cout << "Ammo : " << currentammo << " / " << "MaxAmmo : " << maxammo << endl;
				ammostri = to_string(currentammo);
				
			}

		}

		// Cooldown d'attaque ( en n'attaquant pas pendant un moment , la prochaine attaque enverra 2 bullets d'affilé ) 
		if (attackCooldown >= attackCooldownMax) {
			attackCooldown = 0.f;
			canAttack = true;
		}
		else
			attackCooldown += 0.5f;

		
		// Cooldown reload

		if (rCooldown >= rCooldownMax) {
			rCooldown = 0.f;
			canR = true;
		}
		else
			rCooldown += 0.5f;


		//Munition
		if (Keyboard::isKeyPressed(Keyboard::Key::R) && maxammo > 0 && canR)
		{

			Reload(currentammo, maxammo);
			canR = false;			
			maxammostri = to_string(maxammo);
			ammostri = to_string(currentammo);
			
		}
		window.clear();
		
												// Affichage de différents éléments 
		window.draw(bg);		
		window.draw(text);
		window.draw(player);
		
		//boucle pour chaque ennemis
			//RUSHER

														// LEVEL 1 


		if (level1 == true) {
			for (int i = 0; i < rusherEnemy.size(); i++)
			{
				//fonctions general du rusher
				RusherParameters(rusherEnemy[i], player);
				// Mort du joueur en contact d'un ennemi 
				if ((player.getPosition().x < rusherEnemy[i].enemyCircleShape.getPosition().x + rusherEnemy[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().y < rusherEnemy[i].enemyCircleShape.getPosition().y + rusherEnemy[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().x > rusherEnemy[i].enemyCircleShape.getPosition().x - rusherEnemy[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().y > rusherEnemy[i].enemyCircleShape.getPosition().y - rusherEnemy[i].enemyCircleShape.getRadius()))
				{
					if (!rusherEnemy[i].isDead)
					{
						isAlive = false;
					}
				}
				//Affiche les ennemis s'ils sont pas morts
				//pour les rusher
				if (!rusherEnemy[i].isDead || (rusherEnemy[i].isReviving < rusherEnemy[i].respawnPourcentage))
				{
					window.draw(rusherEnemy[i].enemyCircleShape);
				}
				else
				{				
					rusherEnemy.erase(rusherEnemy.begin() + i);
					ammostri = to_string(currentammo);
					completion += 1;
					cout << completion << "\n";
					Ammo(window, currentammo, ammoBox, rusherEnemy);					
					int ammoIsSpawning = rand() % 100;
					if (ammoIsSpawning < ammoSpawnPourcent)
					{
						CreateAmmo(window, currentammo, rusherEnemy, ammoBox);
					}

				}
			}

			if (completion >= 4) {

				level1 = false;
				level2 = true;
				cout << "level 2 : " << level2 << "\n";
				completion = 0;


			}
		}

														// LEVEL 2

		if (level2 == true) {

			numb = "level : 2";
			for (int i = 0; i < rusherEnemy2.size(); i++)
			{
				//fonctions general du rusher
				RusherParameters(rusherEnemy2[i], player);
				// Mort du joueur en contact d'un ennemi 
				if ((player.getPosition().x < rusherEnemy2[i].enemyCircleShape.getPosition().x + rusherEnemy2[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().y < rusherEnemy2[i].enemyCircleShape.getPosition().y + rusherEnemy2[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().x > rusherEnemy2[i].enemyCircleShape.getPosition().x - rusherEnemy2[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().y > rusherEnemy2[i].enemyCircleShape.getPosition().y - rusherEnemy2[i].enemyCircleShape.getRadius()))
				{
					if (!rusherEnemy2[i].isDead)
					{
						isAlive = false;
					}
				}
				//Affiche les ennemis s'ils sont pas morts
				//pour les rusher
				if (!rusherEnemy2[i].isDead || (rusherEnemy2[i].isReviving < rusherEnemy2[i].respawnPourcentage))
				{
					window.draw(rusherEnemy2[i].enemyCircleShape);
				}
				else
				{
					rusherEnemy2.erase(rusherEnemy2.begin() + i);
					ammostri = to_string(currentammo);
					completion += 1;
					cout << completion << "\n";
					Ammo(window, currentammo, ammoBox, rusherEnemy);
					int ammoIsSpawning = rand() % 100;
					if (ammoIsSpawning < ammoSpawnPourcent)
					{
						CreateAmmo(window, currentammo, rusherEnemy2, ammoBox);
					}
				}
			}


			if (completion >= 6) {

				level2 = false;
				level3 = true;
				cout << "level3 : " << level4 << "\n";
				completion = 0;


			}

		}		

													// LEVEL 3

		if (level3 == true) {
			numb = "level : 3";

			for (int i = 0; i < rusherEnemy3.size(); i++)
			{
				rusherEnemy3[i].rusherSpeed = rand() % 3 + 3;
				rusherEnemy3[i].respawnPourcentage = 50;
				//fonctions general du rusher
				RusherParameters(rusherEnemy3[i], player);
				// Mort du joueur en contact d'un ennemi 
				if ((player.getPosition().x < rusherEnemy3[i].enemyCircleShape.getPosition().x + rusherEnemy3[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().y < rusherEnemy3[i].enemyCircleShape.getPosition().y + rusherEnemy3[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().x > rusherEnemy3[i].enemyCircleShape.getPosition().x - rusherEnemy3[i].enemyCircleShape.getRadius()) 
					&& (player.getPosition().y > rusherEnemy3[i].enemyCircleShape.getPosition().y - rusherEnemy3[i].enemyCircleShape.getRadius()))
				{
					if (!rusherEnemy3[i].isDead)
					{
						isAlive = false;
					}
				}
				//Affiche les ennemis s'ils sont pas morts
				//pour les rusher
				if (!rusherEnemy3[i].isDead || (rusherEnemy3[i].isReviving < rusherEnemy3[i].respawnPourcentage))
				{
					window.draw(rusherEnemy3[i].enemyCircleShape);
				}
				else
				{
					rusherEnemy3.erase(rusherEnemy3.begin() + i);
					ammostri = to_string(currentammo);
					completion += 1;
					cout << completion << "\n";
					Ammo(window, currentammo, ammoBox, rusherEnemy);

					if (completion >= 8) {

						level3 = false;
						level4 = true;
						cout << "level 4 : " << level4 << "\n";
						completion = 0;



					}

				}
			}
		}
		

											// LEVEL 4

		if (level4 == true) {
			numb = "level : 4";
			if (!shooterSpawned)
			{
					shooterEnemy = SpawnEnemyShooter(2);
					shooterSpawned = true;
			}
				//SHOOTER
			for (int i = 0; i < shooterEnemy.size(); i++)
			{
					
				window.draw(shooterEnemy[i].shooterShape);

				ShooterParameters(shooterEnemy[i], player, shooterBullets);
				if (!shooterEnemy[i].isDead || (shooterEnemy[i].isReviving < shooterEnemy[i].respawnPourcentage))
				{
						window.draw(shooterEnemy[i].shooterShape);
				}
				else
				{
						shooterEnemy.erase(shooterEnemy.begin() + i);
						ammostri = to_string(currentammo);
						completion += 1;
				}
			}

				for (int i = 0; i < rusherEnemy4.size(); i++)
				{
					rusherEnemy4[i].rusherSpeed = rand() % 3 + 3;
					rusherEnemy4[i].respawnPourcentage = 50;
					//fonctions general du rusher
					RusherParameters(rusherEnemy4[i], player);
					// Mort du joueur en contact d'un ennemi 
					if ((player.getPosition().x < rusherEnemy4[i].enemyCircleShape.getPosition().x + rusherEnemy4[i].enemyCircleShape.getRadius()) 
						&& (player.getPosition().y < rusherEnemy4[i].enemyCircleShape.getPosition().y + rusherEnemy4[i].enemyCircleShape.getRadius())
						&& (player.getPosition().x > rusherEnemy4[i].enemyCircleShape.getPosition().x - rusherEnemy4[i].enemyCircleShape.getRadius())
						&& (player.getPosition().y > rusherEnemy4[i].enemyCircleShape.getPosition().y - rusherEnemy4[i].enemyCircleShape.getRadius()))
					{
						if (!rusherEnemy4[i].isDead)
						{
							isAlive = false;
						}
					}
					//Affiche les ennemis s'ils sont pas morts
					//pour les rusher
					if (!rusherEnemy4[i].isDead || (rusherEnemy4[i].isReviving < rusherEnemy4[i].respawnPourcentage))
					{
						window.draw(rusherEnemy4[i].enemyCircleShape);
					}
					else
					{
						rusherEnemy4.erase(rusherEnemy4.begin() + i);
						int ammoIsSpawning = rand() % 100;
						if (ammoIsSpawning < ammoSpawnPourcent)
						{
							CreateAmmo(window, currentammo, rusherEnemy3, ammoBox);
						}
						completion += 1;
						cout << completion << "\n";
						Ammo(window, currentammo, ammoBox, rusherEnemy);

						if (completion >= 10) {

							level4 = false;
							levelBoss = true;
							completion = 0;
							levelBoss = true;
							currentammo += 1000;

						}

					}
				}
				//mort du joueur par rapport au balles enemis
				for (int i = 0; i < shooterBullets.size(); i++)
				{
					window.draw(shooterBullets[i].bulletShape);
					shooterBullets[i].bulletShape.move(shooterBullets[i].playerPosition * shooterBullets[i].bulletSpeed);
					if ((player.getPosition().x < shooterBullets[i].bulletShape.getPosition().x + shooterBullets[i].bulletShape.getRadius()) && (player.getPosition().y < shooterBullets[i].bulletShape.getPosition().y + shooterBullets[i].bulletShape.getRadius()) && (player.getPosition().x > shooterBullets[i].bulletShape.getPosition().x - shooterBullets[i].bulletShape.getRadius()) && (player.getPosition().y > shooterBullets[i].bulletShape.getPosition().y - shooterBullets[i].bulletShape.getRadius()))
					{
						isAlive = false;
					}
				}

				//tir des shooter
				for (int j = 0; j < shooterEnemy.size(); j++)
				{
					if (shooterEnemy[j].canShoot)
					{
						shooterEnemy[j].shooted = clock() / CLOCKS_PER_SEC;
						shooterB1.bulletShape.setRadius(5.f);
						shooterB1.bulletShape.setFillColor(Color::Blue);
						shooterB1.bulletShape.setPosition(shooterEnemy[j].shooterShape.getPosition());
						if (player.getPosition().x < shooterEnemy[j].shooterShape.getPosition().x)
						{
							shooterB1.playerPosition.x = (player.getPosition().x + 0.01f) - shooterEnemy[j].shooterShape.getPosition().x;
						}
						else
						{

							shooterB1.playerPosition.x = -(shooterEnemy[j].shooterShape.getPosition().x - (player.getPosition().x + 0.01f));
						}
						if (player.getPosition().y < shooterEnemy[j].shooterShape.getPosition().y)
						{
							shooterB1.playerPosition.y = (player.getPosition().y + 0.01f) - shooterEnemy[j].shooterShape.getPosition().y;
						}
						else
						{
							shooterB1.playerPosition.y = -(shooterEnemy[j].shooterShape.getPosition().y - (player.getPosition().y + 0.01f));
						}
						shooterBullets.push_back(shooterBullet(shooterB1));
						shooterEnemy[j].canShoot = false;
					}
					else
					{
						shooterEnemy[j].shootMaxCoolDown = shooterEnemy[j].shooted + 2.f;
						if (shooterEnemy[j].shootCoolDown < shooterEnemy[j].shootMaxCoolDown)
						{
							shooterEnemy[j].shootCoolDown = clock() / CLOCKS_PER_SEC;
						}
						else
						{
							shooterEnemy[j].canShoot = true;
						}

					}

				}
			}

				//BOSS
			if (levelBoss)
			{
				
				if (!boss.isDead)
				{
					window.draw(boss.bossShape);
					//cout << boss.bossBullets.size() << endl;
					//cout << boss.bulletSpawnerPosition[0].getPosition().x << " " << boss.bulletSpawnerPosition[0].getPosition().y << endl;
					for (int i = 0; i < sizeof(boss.bulletSpawnerPosition) / sizeof(*boss.bulletSpawnerPosition); i++)
					{
						boss.bulletSpawnerPosition[i].setRotation(boss.bulletSpawnerPosition[i].getRotation() + .7f);
						window.draw(boss.bulletSpawnerPosition[i]);
						//cout << boss.bulletSpawnerPosition[0].getRotation() << endl;
					}
					BossParameter(boss, bBoss);
					for (int i = 0; i < boss.bossBullets.size(); i++)
					{
						window.draw(boss.bossBullets[i].bulletShape);
						//boss.bossBullets[i].bulletShape.rotate(5.f);
						boss.bossBullets[i].bulletShape.move(Vector2f(((cos(boss.bossBullets[i].bulletShape.getRotation()) / 2 *Pi) * 360 )* boss.bossBullets[i].bulletSpeed, (sin(boss.bossBullets[i].bulletShape.getRotation() / 2 * Pi) * 360 )* boss.bossBullets[i].bulletSpeed));
						if (boss.bossBullets[i].bulletShape.getPosition().x < 0 || boss.bossBullets[i].bulletShape.getPosition().x > window.getSize().x || boss.bossBullets[i].bulletShape.getPosition().y < 0 || boss.bossBullets[i].bulletShape.getPosition().y > window.getSize().y)
						{
							boss.bossBullets.erase(boss.bossBullets.begin() + i);
						}
					}
					//mort du joueur contre les balles du boss
					for (int i = 0; i < boss.bossBullets.size(); i++)
					{
						if ((player.getPosition().x < boss.bossBullets[i].bulletShape.getPosition().x + boss.bossBullets[i].bulletShape.getRadius()) && (player.getPosition().y < boss.bossBullets[i].bulletShape.getPosition().y + boss.bossBullets[i].bulletShape.getRadius()) && (player.getPosition().x > boss.bossBullets[i].bulletShape.getPosition().x - boss.bossBullets[i].bulletShape.getRadius()) && (player.getPosition().y > boss.bossBullets[i].bulletShape.getPosition().y - boss.bossBullets[i].bulletShape.getRadius()))
						{
							isAlive = false;
						}
					}
				}

				
				if (completion == 1) {
					levelBoss = false;
					completion = 0;
				}
			}

			// Tir des projectiles 
			for (size_t i = 0; i < bullets.size(); i++) {
				window.draw(bullets[i].shape);
				bullets[i].shape.move(bullets[i].currVelocity);

				// Mort des ennemis 
				// 
				//RUSHERS
				for (int j = 0; j < rusherEnemy.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < rusherEnemy[j].enemyCircleShape.getPosition().x + rusherEnemy[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y < rusherEnemy[j].enemyCircleShape.getPosition().y + rusherEnemy[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().x > rusherEnemy[j].enemyCircleShape.getPosition().x - rusherEnemy[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y > rusherEnemy[j].enemyCircleShape.getPosition().y - rusherEnemy[j].enemyCircleShape.getRadius()))
					{
						rusherEnemy[j].enemyCircleShape.setFillColor(Color::White);
						rusherEnemy[j].isDead = true;

					}
				}
				for (int j = 0; j < rusherEnemy2.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < rusherEnemy2[j].enemyCircleShape.getPosition().x + rusherEnemy2[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y < rusherEnemy2[j].enemyCircleShape.getPosition().y + rusherEnemy2[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().x > rusherEnemy2[j].enemyCircleShape.getPosition().x - rusherEnemy2[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y > rusherEnemy2[j].enemyCircleShape.getPosition().y - rusherEnemy2[j].enemyCircleShape.getRadius()))
					{

						rusherEnemy2[j].enemyCircleShape.setFillColor(Color::White);
						rusherEnemy2[j].isDead = true;

					}
				}
				for (int j = 0; j < rusherEnemy3.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < rusherEnemy3[j].enemyCircleShape.getPosition().x + rusherEnemy3[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y < rusherEnemy3[j].enemyCircleShape.getPosition().y + rusherEnemy3[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().x > rusherEnemy3[j].enemyCircleShape.getPosition().x - rusherEnemy3[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y > rusherEnemy3[j].enemyCircleShape.getPosition().y - rusherEnemy3[j].enemyCircleShape.getRadius()))
					{
						rusherEnemy3[j].enemyCircleShape.setFillColor(Color::White);
						rusherEnemy3[j].isDead = true;

					}
				}

				for (int j = 0; j < rusherEnemy4.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < rusherEnemy4[j].enemyCircleShape.getPosition().x + rusherEnemy4[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y < rusherEnemy4[j].enemyCircleShape.getPosition().y + rusherEnemy4[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().x > rusherEnemy4[j].enemyCircleShape.getPosition().x - rusherEnemy4[j].enemyCircleShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y > rusherEnemy4[j].enemyCircleShape.getPosition().y - rusherEnemy4[j].enemyCircleShape.getRadius()))
					{
						rusherEnemy4[j].enemyCircleShape.setFillColor(Color::White);
						rusherEnemy4[j].isDead = true;

					}
				}
				
				//SHOOTER
				for (int j = 0; j < shooterEnemy.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < shooterEnemy[j].shooterShape.getPosition().x + shooterEnemy[j].shooterShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y < shooterEnemy[j].shooterShape.getPosition().y + shooterEnemy[j].shooterShape.getRadius()) 
						&& (bullets[i].shape.getPosition().x > shooterEnemy[j].shooterShape.getPosition().x - shooterEnemy[j].shooterShape.getRadius()) 
						&& (bullets[i].shape.getPosition().y > shooterEnemy[j].shooterShape.getPosition().y - shooterEnemy[j].shooterShape.getRadius()))
					{
						shooterEnemy[j].shooterShape.setFillColor(Color::White);
						shooterEnemy[j].isDead = true;

					}
				}

				//AMMO
				for (Ammo& amo : ammoBox)
				{
					if ((player.getPosition().x < amo.ammoShape.getPosition().x + amo.ammoShape.getSize().x)
						&& (player.getPosition().y < amo.ammoShape.getPosition().y + amo.ammoShape.getSize().y)
						&& (player.getPosition().x > amo.ammoShape.getPosition().x - amo.ammoShape.getSize().x)
						&& (player.getPosition().y > amo.ammoShape.getPosition().y - amo.ammoShape.getSize().y))
					{
						amo.isPicked = true;
						if (amo.isPicked == true)
						{
							int i = ammoBox.size() - 1;
							maxammo += 5;
							amo.isPicked = false;
							ammoBox.erase(ammoBox.begin() + i);	
						}
					}
				}

				//BOSS
				if ((bullets[i].shape.getPosition().x < boss.bossShape.getPosition().x + boss.bossShape.getRadius())
					&& (bullets[i].shape.getPosition().y < boss.bossShape.getPosition().y + boss.bossShape.getRadius())
					&& (bullets[i].shape.getPosition().x > boss.bossShape.getPosition().x - boss.bossShape.getRadius())
					&& (bullets[i].shape.getPosition().y > boss.bossShape.getPosition().y - boss.bossShape.getRadius()))
				{
					if (boss.damaged)
					{
						boss.damaged = false;
					}
					else
					{
						boss.BossDamaged(1);
						boss.damaged = true;
					}
				}

				//Suppression des projectiles en dehors de l'écran 
				if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
					bullets.erase(bullets.begin() + i);

				}
			}

			//Deplacement des tirs des ennemis shooter
			for (int i = 0; i < shooterBullets.size(); i++)
			{
				window.draw(shooterBullets[i].bulletShape);
				shooterBullets[i].bulletShape.move(shooterBullets[i].playerPosition * shooterBullets[i].bulletSpeed);
				if ((player.getPosition().x < shooterBullets[i].bulletShape.getPosition().x + shooterBullets[i].bulletShape.getRadius())
					&& (player.getPosition().y < shooterBullets[i].bulletShape.getPosition().y + shooterBullets[i].bulletShape.getRadius())
					&& (player.getPosition().x > shooterBullets[i].bulletShape.getPosition().x - shooterBullets[i].bulletShape.getRadius())
					&& (player.getPosition().y > shooterBullets[i].bulletShape.getPosition().y - shooterBullets[i].bulletShape.getRadius()))
					isAlive = false;
				

			}

			
			//Limitations de la bordure d'écran
			if (player.getPosition().x < 0.f)
				player.setPosition(0.f, player.getPosition().y);

			if (player.getPosition().y < 0.f)
				player.setPosition(player.getPosition().x, 0.f);

			if (player.getPosition().x + player.getGlobalBounds().width > xwindow)
				player.setPosition(xwindow - player.getGlobalBounds().width, player.getPosition().y);

			if (player.getPosition().y + player.getGlobalBounds().height > ywindow)
				player.setPosition(player.getPosition().x, ywindow - player.getGlobalBounds().height);

			// Le nombre de niveau
			text.setString(numb);
			currentammotext.setString("Current ammo: " + ammostri);
			maxammotext.setString("Max ammo: " + maxammostri);
			window.draw(maxammotext);
			window.draw(currentammotext);
			for (Ammo& amo : ammoBox) 
			{
				window.draw(amo.ammoShape);
			}
			window.display();

			// Esc pour quitter le jeu 
			if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
				window.close();
		}


}

	// Le spawner des ennemis coureurs

vector<Enemy> SpawnEnemyRusher(int number)
{
	vector<Enemy> rusher(number);
	for (int i = 0; i < rusher.size(); i++)
	{
		int enemyStartPosX = rand() % 1100;
		int enemyStartPosY = rand() % 700;
		rusher[i].enemyCircleShape.setRadius(15.f);
		rusher[i].enemyCircleShape.setFillColor(Color::Green);
		rusher[i].enemyCircleShape.setPosition(enemyStartPosX, enemyStartPosY);
	}
	return rusher;
}

// Spawner des ennemis shooters

vector<Shooter> SpawnEnemyShooter(int number)
{
	vector<Shooter> shooter(number);
	for (int i = 0; i < shooter.size(); i++)
	{
		int enemyStartPosX = rand() % 1100;
		int enemyStartPosY = rand() % 900;
		shooter[i].shooterShape.setRadius(20.f);
		shooter[i].shooterShape.setPointCount(3);
		shooter[i].shooterShape.setFillColor(Color::Red);
		shooter[i].shooterShape.setPosition(enemyStartPosX, enemyStartPosY);
	}
	return shooter;
}

// Ajout d'Arial pour le texte

string getAppPath() {
	char cExeFilePath[256];
	GetModuleFileNameA(NULL, cExeFilePath, 256);
	string exeFilePath = cExeFilePath;
	int exeNamePos = exeFilePath.find_last_of("\\/");
	string appPath = exeFilePath.substr(0, exeNamePos + 1);
	return appPath;
}

string getAssetsPath() {
	string assetsPath = getAppPath() + "\Assets";
	return assetsPath;
}



												// FONCTIONS

// Fct pour l'ennemi rusher
void BossParameter(Boss& boss, shooterBullet& bBoss)
{
	//si le boss n'est pas encore apparu, apparait
	if (!boss.hasAppeared)
	{
		//Spawn le boss a un endroit avec des valeurs defini
		int enemyStartPosX = rand() % 1100;
		int enemyStartPosY = rand() % 900;
		//preparametre du boss
		boss.bossShape.setPosition(enemyStartPosX, enemyStartPosY);
		boss.bossShape.setRadius(30.f);
		boss.bossShape.setFillColor(Color::Yellow);
		boss.bossShape.setOrigin(boss.bossShape.getRadius(), boss.bossShape.getRadius());
		
		//pre parametres des balles
		bBoss.bulletShape.setFillColor(Color::Blue);
		bBoss.bulletSpeed = 0.005f;
		bBoss.bulletShape.setRadius(7.f);


		//preparametre des spawners 
		float spawnerRotation = 0.f;
		for (int i = 0; i < sizeof(boss.bulletSpawnerPosition) / sizeof(*boss.bulletSpawnerPosition); i++)
		{
			boss.bulletSpawnerPosition[i].setPosition(boss.bossShape.getPosition().x, boss.bossShape.getPosition().y);
			boss.bulletSpawnerPosition[i].setRadius(5.f);
			boss.bulletSpawnerPosition[i].setRotation(spawnerRotation);
			spawnerRotation += 45.f;
		}

		boss.hasAppeared = true;
	}
	if (!boss.hasShooted)
	{
		boss.bossBulletShooted = (float)clock() / CLOCKS_PER_SEC;
		for (int i = 0; i < sizeof(boss.bulletSpawnerPosition) / sizeof(*boss.bulletSpawnerPosition); i++)
		{
			bBoss.bulletShape.setPosition(boss.bulletSpawnerPosition[i].getPosition().x, boss.bulletSpawnerPosition[i].getPosition().y);
			bBoss.bulletShape.setRotation(boss.bulletSpawnerPosition[i].getRotation());
			boss.bossBullets.push_back(bBoss);
			//cout << boss.bulletSpawnerPosition[7].getRotation() << endl;
			//cout << bBoss.bulletShape.getPosition().x << " " << bBoss.bulletShape.getPosition().y << endl;
		}
		boss.hasShooted = true;
	}
	else
	{
		boss.bossBulletCooldown = (float)clock() / CLOCKS_PER_SEC;
		if (boss.bossBulletCooldown >= boss.bossBulletShooted + boss.speedSpawningBullet)
		{
			boss.hasShooted = false;
		}
	}
	
}



void RusherParameters(Enemy& rusher, CircleShape& player)
{
	if (!rusher.isDead)
	{
		//mouvements des ennemis
		if ((rusher.enemyCircleShape.getPosition().x > player.getPosition().x))
		{
			rusher.enemyCircleShape.move(-rusher.rusherSpeed, 0.f);
		}
		if ((rusher.enemyCircleShape.getPosition().y > player.getPosition().y))
		{
			rusher.enemyCircleShape.move(0.f, -rusher.rusherSpeed);
		}
		if ((rusher.enemyCircleShape.getPosition().x < player.getPosition().x))
		{
			rusher.enemyCircleShape.move(rusher.rusherSpeed, 0.f);
		}
		if ((rusher.enemyCircleShape.getPosition().y < player.getPosition().y))
		{
			rusher.enemyCircleShape.move(0.f, rusher.rusherSpeed);
		}

		rusher.timeOfDeath = clock() / CLOCKS_PER_SEC;
	}
	else
	{
		//quand mort s'il peut revivre, attend son cooldown avant de revivre
		if (rusher.isReviving < rusher.respawnPourcentage)
		{
			if (rusher.reviveTime < rusher.timeOfDeath + rusher.timeBeforeRevive)
			{
				rusher.reviveTime = clock() / CLOCKS_PER_SEC;
			}
			else
			{
				rusher.isDead = false;
				rusher.isReviving = rand() % 100;
				rusher.enemyCircleShape.setFillColor(Color::Green);
			}

		}
	}

}

// Fct pour l'ennemi shooter
void ShooterParameters(Shooter& shooter, CircleShape& player, vector<shooterBullet>& shooterBullets)
{
	if (!shooter.isDead)
	{
		//mouvements des ennemis
		if (!((shooter.shooterShape.getPosition().x < player.getPosition().x + (player.getRadius() * 25)) 
			&& (shooter.shooterShape.getPosition().y < player.getPosition().y + (player.getRadius() * 25)) 
			&& (shooter.shooterShape.getPosition().x > player.getPosition().x - (player.getRadius() * 25)) 
			&& (shooter.shooterShape.getPosition().y > player.getPosition().y - (player.getRadius() * 25))))
		{
			if ((shooter.shooterShape.getPosition().x > player.getPosition().x))
			{
				shooter.shooterShape.move(-shooter.shooterSpeed, 0.f);
			}
			if ((shooter.shooterShape.getPosition().y > player.getPosition().y))
			{
				shooter.shooterShape.move(0.f, -shooter.shooterSpeed);
			}
			if ((shooter.shooterShape.getPosition().x < player.getPosition().x))
			{
				shooter.shooterShape.move(shooter.shooterSpeed, 0.f);
			}
			if ((shooter.shooterShape.getPosition().y < player.getPosition().y))
			{
				shooter.shooterShape.move(0.f, shooter.shooterSpeed);
			}
		}
		shooter.timeOfDeath = clock() / CLOCKS_PER_SEC;
	}
	else
	{
		//quand mort s'il peut revivre, attend son cooldown avant de revivre
		if (shooter.isReviving < shooter.respawnPourcentage)
		{
			if (shooter.reviveTime < shooter.timeOfDeath + shooter.timeBeforeRevive)
			{
				shooter.reviveTime = clock() / CLOCKS_PER_SEC;
			}
			else
			{
				shooter.isDead = false;
				shooter.isReviving = rand() % 100;
				shooter.shooterShape.setFillColor(Color::Red);
			}

		}
	}
}

// Fct pour reload
void Reload(int& currentammo, int& maxammo)
{
	cout << "Reloading.." << endl;
	if (maxammo < 0)
	{
		maxammo = 0;
	}
	if (currentammo < 0)
	{
		currentammo = 0;
	}
	currentammo += 5;
	maxammo -= 5;

	cout << "Ammo : " << currentammo << " / " << "MaxAmmo : " << maxammo << endl;
	

}



//condition pour aimanter un element
// variable etant pour le shooter
//if(shooter.shooterShape.getPosition().x < player.getPosition().x + (player.getRadius() * 2)) && (shooter.shooterShape.getPosition().y < player.getPosition().y + (player.getRadius() * 2)) && (shooter.shooterShape.getPosition().x > player.getPosition().x - (player.getRadius() * 2)) && (shooter.shooterShape.getPosition().y > player.getPosition().y - (player.getRadius() * 2))

// Fct des boites de munitions 
void Ammo(RenderWindow& window, int& currentAmmo, CircleShape& ammoBox, vector<Enemy> enemies) {

	currentAmmo += 3;
	window.draw(ammoBox);
}
void CreateAmmo(RenderWindow& window, int& currentAmmo, vector<Enemy>& enemies, vector<Ammo>& ammoBox) {

	Ammo ammo;
	ammoBox.push_back(ammo);
	int i = ammoBox.size() - 1;
	int j = enemies.size() - 1;

	int ammoBoxPosX = enemies[j].enemyCircleShape.getPosition().x;
	int ammoBoxPosY = enemies[j].enemyCircleShape.getPosition().y;

	ammoBox[i].ammoShape.setSize(Vector2f(20, 15));
	ammoBox[i].ammoShape.setFillColor(Color::Blue);
	ammoBox[i].ammoShape.setPosition(ammoBoxPosX, ammoBoxPosY);	

	cout << "aapparu" << endl;
	return;
}