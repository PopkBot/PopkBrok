#pragma once


#include <SFML/Graphics.hpp>
#include<iostream>
#include <algorithm>
//#include <vector>

#define w 1000
#define h 500
#define lineCount 1

#define maxH 10

using namespace std;
class Graph
{

public:

	sf::Shader shader;

	struct StLines {
		std::vector<sf::Vertex>vertex;
		float x;
		bool bDrawLine = false;
		sf::Text text;

		sf::Vertex posLine_Y[2];
		bool bdraw = false;
	};
	
	
	sf::Font font;
	
	struct Axis {
		std::vector<sf::Vertex>xAxis, yAxis;
		float x0;
		float y0;
	};

	Axis axis;
	Axis axisR;

	sf::Text tmaxV;

	StLines lines[lineCount];
	
	bool bCheck_y = false,
		bScale_y=false;
	float 
		qy = 2,
		dqy=0;


	void initialize() {
		sf::RenderWindow window(sf::VideoMode(w, h), "Tracing", sf::Style::Titlebar | sf::Style::Close); //Titlebar | sf::Style::Close
		window.setFramerateLimit(60);
		window.setPosition(sf::Vector2i(830, 0));

		sf::RenderTexture empityTexture;
		empityTexture.create(w, h);
		sf::Sprite empitySprite = sf::Sprite(empityTexture.getTexture());



		//shader.setUniform("u_resolution", sf::Vector2f(w, h));

		
		if (!font.loadFromFile("arial.ttf")) {
			printf("\nerror=======================================\n");
		}
	

		tmaxV.setPosition(axisR.x0 + 10, 30);
		tmaxV.setFillColor(sf::Color{ 150,150,150 });
		tmaxV.setFont(font);
		tmaxV.setCharacterSize(15);
		

		
		sf::RenderStates states;
		sf::Transform transform;
		states.transform = transform;

		while (window.isOpen()) {


			sf::Event event;

			while (window.pollEvent(event))
			{



				if (event.type == sf::Event::Closed)
				{

					window.close();
				}

				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}
					if (event.key.code == sf::Keyboard::Space) {
						//drawline(10, 0);
					}
					if (event.key.code == sf::Keyboard::W) {

						

					}
					if (event.key.code == sf::Keyboard::S) {

						

						
					}

				}
				else if (event.type == sf::Event::KeyReleased)
				{
					if (event.key.code == sf::Keyboard::Space) {
						
					}
					if (event.key.code == sf::Keyboard::W) {

					}
					

				}

			}


			tmaxV.setString(to_string(axis.y0 / (qy )));


			
			window.clear();
			//boundscheck();

			check_y();
			scale_y_max();
			
			window.draw(&axis.xAxis[0], axis.xAxis.size(), sf::LinesStrip);
			window.draw(&axis.yAxis[0], axis.yAxis.size(), sf::LinesStrip);
			window.draw(&axisR.yAxis[0], axisR.yAxis.size(), sf::LinesStrip);
			window.draw(tmaxV);
			
			for (int i = 0; i < lineCount; i++) {
				if (lines[i].bdraw) {
					window.draw(lines[i].text);
					window.draw(&lines[i].posLine_Y[0], 2, sf::LinesStrip);
					window.draw(&lines[i].vertex[0], lines[i].vertex.size(), sf::LinesStrip);
				}
				
			}
			
			

			
			window.display();
			
			

			

			

			


		}
	}


	void drawline(float ny,int lineN) {
		sf::Vertex vertexs;
		vertexs.position = sf::Vector2f(lines[lineN].x + axis.x0, -ny * qy + axis.y0);
		//printf("\nx= %3f\ty= %3f\n", lines[lineN].x, -ny * 10 + 400.0f);
		vertexs.color = sf::Color::Red;
		
		lines[lineN].bDrawLine = true;
	
		
		

		if (lines[lineN].x + axis.x0 > axisR.x0) {
			
				for (int j = 0; j < (lines[lineN].vertex.size()-1); j++) {
					lines[lineN].vertex[j].position.y = lines[lineN].vertex[j+1].position.y;
				}
				lines[lineN].vertex[lines[lineN].vertex.size() - 1].position.y = vertexs.position.y;
				
				//cout << "\n" << lines[i].vertex.size() << endl;
			
		}
		else {
			lines[lineN].vertex.push_back(vertexs);
		}
		
		
		lines[lineN].posLine_Y[1].position.y = lines[lineN].vertex[lines[lineN].vertex.size() - 1].position.y;
		lines[lineN].posLine_Y[1].position.x = axisR.x0;
		lines[lineN].posLine_Y[1].color = sf::Color{ 50,100,50 };

		lines[lineN].posLine_Y[0].position.x = axis.x0;
		lines[lineN].posLine_Y[0].position.y = lines[lineN].posLine_Y[1].position.y;
		lines[lineN].posLine_Y[0].color = sf::Color{ 50,100,50 };


		
		lines[lineN].text.setPosition(axisR.x0+10, lines[lineN].posLine_Y[1].position.y-15);
		lines[lineN].text.setFillColor(sf::Color{ 150,150,150 });
		lines[lineN].text.setFont(font);
		lines[lineN].text.setCharacterSize(15);
		
	}

	void initializeAxis() {
		axis.x0 = 10;
		axis.y0 = h-10;
		sf::Vertex dumv;

		dumv.color = sf::Color{100,100,100};
		dumv.position = sf::Vector2f(0, axis.y0);
		axis.xAxis.push_back(dumv);
		dumv.position = sf::Vector2f(w, axis.y0);
		axis.xAxis.push_back(dumv);

		dumv.color = sf::Color{ 100,100,100 };
		dumv.position = sf::Vector2f(axis.x0,0);
		axis.yAxis.push_back(dumv);
		dumv.position = sf::Vector2f(axis.x0, h);
		axis.yAxis.push_back(dumv);

		
		axisR.x0 = w - 100;
		axisR.y0 = 0;

		dumv.color = sf::Color{ 100,100,100 };
		dumv.position = sf::Vector2f(axisR.x0, 0);
		axisR.yAxis.push_back(dumv);
		dumv.position = sf::Vector2f(axisR.x0, h);
		axisR.yAxis.push_back(dumv);

	}

	void boundscheck() {

		bool bBoarder = false;
		int ln;
		float dx=1;
		for (int i = 0; i < lineCount & !bBoarder; i++) {
			if (lines[i].x + axis.x0 > w-100) {
				bBoarder = true;
				ln = i;
				dx = (w-axis.x0)/ lines[i].x;
			}
		}

		if (bBoarder) {
			for (int i = 0; i < lineCount; i++) {
				/*
				for (int j = 0; j < lines[i].vertex.size(); j++) {
					lines[i].vertex[j].position.x = (lines[i].vertex[j].position.x-axis.x0)* dx+axis.x0;
					lines[i].x = (lines[i].vertex[j].position.x ) * dx ;
				}
				*/
				
				
				for (int j = 1; j < (lines[i].vertex.size() ); j++) {
					//bufVer.position = lines[i].vertex[j].position;
					lines[i].vertex[j-1].position = lines[i].vertex[j].position;
					
					//lines[i].x = (lines[i].vertex[j].position.x);
				}
				cout << "\n" << lines[i].vertex.size() << endl;
				//lines[i].vertex.pop_back();
				lines[i].x = lines[i].vertex[lines[i].vertex.size()].position.x;
			}
		}



	}


	void scale_y_max() {

		
		float maxY = axis.y0;
		
		for (int i = 0; i < lineCount; i++) {
			
			for (int j = 0; j < (lines[i].vertex.size()); j++) {
				if(lines[i].bdraw)
					if (lines[i].vertex[j].position.y < maxY) {
						maxY = lines[i].vertex[j].position.y;
					}
			}
		}

	

		if (maxY < maxH | bScale_y) {

			bScale_y = true;

			if (dqy < 1) {
				dqy += 0.01f;
			}
			else if (dqy >= 1.0f) {
				//dqy = 1;
				bScale_y = false;
				dqy = 0;
			}

			for (int i = 0; i < lineCount; i++) {

				for (int j = 0; j < lines[i].vertex.size(); j++) {
					lines[i].vertex[j].position.y = axis.y0 * (1 -   (axis.y0 - maxH) / (axis.y0 - maxY))
						+ lines[i].vertex[j].position.y  * (axis.y0 - maxH) / (axis.y0 - maxY);

				}


			}
			qy = qy  * (axis.y0 - maxH) / (axis.y0 - maxY);

		}

	}


	void check_y() {

		
	

		float maxY = lines[0].posLine_Y[1].position.y;
		int maxI = 0;
		for (int i = 1; i < lineCount; i++) {
			if (lines[i].posLine_Y[1].position.y > maxY) {
				maxY = lines[i].posLine_Y[1].position.y;
				maxI = i;
			}

		}

	
		if (lines[maxI].posLine_Y[1].position.y >= axis.y0 - 10 | bCheck_y) {
			
			bCheck_y = true;

			if (dqy < 1) {
				dqy += 0.01f;
			}
			else if (dqy >= 1.0f) {
				//dqy = 1;
				bCheck_y = false;
				dqy = 0;
			}
			dqy = 1;
			for (int i = 0; i < lineCount; i++) {

				for (int j = 0; j < lines[i].vertex.size(); j++) {
					lines[i].vertex[j].position.y = axis.y0 * (1 - dqy * (axis.y0 - h / 2) / (axis.y0 - maxY))
													+ lines[i].vertex[j].position.y * dqy * (axis.y0 - h / 2) / (axis.y0 - maxY);
					
				}


			}
			qy =qy*dqy* (axis.y0 - h / 2) / (axis.y0-maxY);

		}

	}


};

