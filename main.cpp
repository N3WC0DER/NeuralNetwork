#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include "lib/Matrix.h"

#include "NeuralNetwork.h"
#include "lib/sigmoida.h"
#include "lib/random.h"
#include <SFML/Graphics.hpp>

using namespace sf;

constexpr auto TRAIN = 2;

bool inObject(const Vector2f& mousePosition, const RectangleShape* object) {
	if (mousePosition.x > object->getPosition().x &&
		mousePosition.y > object->getPosition().y &&
		mousePosition.x < object->getPosition().x + object->getSize().x &&
		mousePosition.y < object->getPosition().y + object->getSize().y) {
		return true;
	}
	else return false;
}

Matrix<double> prepareValues(Matrix<double>& pixels) {
	for (int i = 1; i < pixels.getRows() - 1; i++) {
		for (int j = 1; j < pixels.getCols() - 1; j++) {
			if (pixels(i, j) == 0 && (pixels(i + 1, j) == 252 || pixels(i, j + 1) == 252 || pixels(i - 1, j) == 252 || pixels(i, j - 1) == 252)) {
				pixels(i, j) = getRandomNumber(10, 178);
			}
		}
	}

	Matrix<double> inputs(pixels.getSize(), 1);
	for (int i = 0; i < pixels.getSize(); i++) {
		inputs(i, 0) = pixels(i / 28, i % 28);
	}
	pixels();

	for (int i = 0; i < inputs.getSize(); i++) {
		inputs(i, 0) = (inputs(i, 0) / 255 * 0.99) + 0.01;
	}

	return inputs;
}

Matrix<int> prepareValues(Matrix<int>& pixels) {
	for (int i = 1; i < pixels.getRows() - 1; i++) {
		for (int j = 1; j < pixels.getCols() - 1; j++) {
			if (pixels(i, j) == 0 && (pixels(i + 1, j) == 252 || pixels(i, j + 1) == 252 || pixels(i - 1, j) == 252 || pixels(i, j - 1) == 252)) {
				pixels(i, j) = getRandomNumber(132, 178);
			}
		}
	}

	Matrix<int> inputs(pixels.getSize(), 1);
	for (int i = 0; i < pixels.getSize(); i++) {
		inputs(i, 0) = pixels(i / 28, i % 28);
	}
	pixels();
	return inputs;
}

void saveTrainData(Matrix<int>& pixels) {
	ofstream trainData;
	trainData.open("trainData.csv", ios::app);

	if (!trainData.is_open()) {
		exit(EXIT_FAILURE);
	}

	string line = "";
	for (int i = 0; i < pixels.getRows(); i++) {
		for (int j = 0; j < pixels.getCols(); j++) {
			if (i == pixels.getRows() - 1 && j == pixels.getCols() - 1) {
				line += to_string(pixels(i, j));
				continue;
			}
			line += to_string(pixels(i, j));
			line.push_back(',');
		}
	}
	trainData << TRAIN << ",";

	trainData << line;
	trainData << '\n';

	trainData.close();
}

enum Objects{
	FIELD = 1,
	BUTTON_DETERMINE,
	BUTTON_CLEAR,
	BUTTON_TRAIN,
	TEXT_DETERMINE,
	CANVAS,
	FIELD_CANVAS
};

Font* mainFont;
Font* font;
Texture* background;
Texture* menu;
Texture* RedVad;

vector<Drawable*> initObjects(){
	menu = new Texture;
	if (!menu->loadFromFile("resources/menu.png")) {
		exit(EXIT_FAILURE);
	}
	Sprite* s_menu = new Sprite;
	s_menu->setTexture(*menu);
	s_menu->setScale(0.857, 0.815);
	
	RectangleShape* field = new RectangleShape(Vector2f(1080.f, 1080.f));
	field->setFillColor(Color::Transparent);
	
	RectangleShape* buttonDetermine = new RectangleShape(Vector2f(770.f, 160.f));
	buttonDetermine->setFillColor(Color::Transparent);
	buttonDetermine->move(155.f, 1985.f);
	
	RectangleShape* buttonClear = new RectangleShape(Vector2f(770.f, 160.f));
	buttonClear->setFillColor(Color::Transparent);
	buttonClear->move(155.f, 1780.f);
	
	RectangleShape* buttonTrain = new RectangleShape(Vector2f(770.f, 160.f));
	buttonTrain->setFillColor(Color::Transparent);
	buttonTrain->move(155.f, 1580.f);

	mainFont = new Font;
#if defined(__ANDROID__)
	if (!mainFont->loadFromFile("resources/fonts/arialmt.ttf")) {
		exit(EXIT_FAILURE);
	}
#else
	if (!mainFont->loadFromFile("C:\\Windows\\Fonts\\ariblk.ttf")) {
		exit(EXIT_FAILURE);
	}
#endif
	
	font = new Font;
	if (!font->loadFromFile("resources/fonts/teddy-bear.ttf")) {
		exit(EXIT_FAILURE);
	}
	
	Text* textDetermine = new Text;
	textDetermine->setFont(*mainFont);
	textDetermine->setFillColor(Color::White);
	textDetermine->setCharacterSize(50);
	textDetermine->move(210.f, 1150.f);
	
	RedVad = new Texture;
	if (!RedVad->loadFromFile("resources/redvad.png")) {
		exit(EXIT_FAILURE);
	}
	Sprite* s_RedVad = new Sprite;
	s_RedVad->setTexture(*RedVad);
	s_RedVad->setScale(0.857, 0.815);
	
	RectangleShape* canvas = new RectangleShape(Vector2f(1008.f, 1008.f));
	canvas->setFillColor(Color::Transparent);
	canvas->move(36.f, 36.f);
	
	RectangleShape* fieldCanvas = new RectangleShape(Vector2f(28.f, 28.f));
	fieldCanvas->setFillColor(Color::Transparent);
	fieldCanvas->move(36.f, 36.f);
	
	vector<Drawable*> objects;
	
	objects.push_back(s_menu);
	objects.push_back(field);
	objects.push_back(buttonDetermine);
	objects.push_back(buttonClear);
	objects.push_back(buttonTrain);
	objects.push_back(textDetermine);
	objects.push_back(canvas);
	objects.push_back(fieldCanvas);
	objects.push_back(s_RedVad);
	
	return objects;
}

int main(){
	srand(static_cast<unsigned int>(time(0)));
	rand();

	background = new Texture;
	if (!background->loadFromFile("resources/background.png")) {
		exit(EXIT_FAILURE);
	}
	Sprite* s_background = new Sprite;
	s_background->setTexture(*background);
	s_background->setScale(0.857, 0.815);

	const int inputNodes = 784,
			  hiddenNodes = 100,
			  outputNodes = 10;
	double learningRate = 0.3;

	NeuralNetwork network(inputNodes, hiddenNodes, outputNodes, learningRate);

	RenderWindow* window = new RenderWindow(VideoMode::getDesktopMode(), "NeuralNetwork");

	bool mousePressed = false;
	vector<CircleShape> circle;
	vector<Drawable*> objects = initObjects();
	bool isTraining = false;
	
	while (window->isOpen()) {
		Event event;
		while (window->pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window->close();
				break;
#if defined(__ANDROID__)
			case Event::TouchBegan:
				mousePressed = true;
				break;
			case Event::TouchEnded:
				mousePressed = false;
				break;
#else
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					mousePressed = true;
				}
				break;
			case Event::MouseButtonReleased:
				if (event.mouseButton.button == Mouse::Left) {
					mousePressed = false;
				}
				break;
#endif
			}
		}
		window->clear(Color(0x222831));

		if (mousePressed) {
		#ifdef __ANDROID__
			Vector2f touchPosition = window->mapPixelToCoords(Touch::getPosition(0, *window));
		#else
			Vector2f touchPosition = window->mapPixelToCoords(Mouse::getPosition(*window));
		#endif
			if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(FIELD)))) {
				CircleShape tempCircle(50.f);
				tempCircle.move(touchPosition.x, touchPosition.y);
				tempCircle.setFillColor(Color::White);
				circle.push_back(tempCircle);
				
				/*if (!inObject(tempCircle.getPosition(), static_cast<RectangleShape*>(objects.at(FIELD_CANVAS)))) {
					Vector2f extremeNegativePoints = Vector2f(1100.f, 1100.f);
					Vector2f extremePositivePoints;
					for (auto i = circle.begin(); i != circle.end(); i++) {
						if (i->getPosition().x > extremePositivePoints.x) 
							extremePositivePoints.x = i->getPosition().x;
						if (i->getPosition().y > extremePositivePoints.y) 
							extremePositivePoints.y = i->getPosition().y;
						if (i->getPosition().x < extremeNegativePoints.x) 
							extremeNegativePoints.x = i->getPosition().x;
						if (i->getPosition().y < extremeNegativePoints.y) 
							extremeNegativePoints.y = i->getPosition().y;
					}
					
					if (!inObject(extremeNegativePoints, static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))))
						static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))->setPosition(extremeNegativePoints);
					if (!inObject(extremePositivePoints, static_cast<RectangleShape*>(objects.at(FIELD_CANVAS)))) {
						Vector2f newPosition = Vector2f(static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))->getSize().x * 2 + extremeNegativePoints.x - static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))->getPosition().x, static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))->getSize().y * 2 + extremeNegativePoints.y - static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))->getPosition().y);
						static_cast<RectangleShape*>(objects.at(FIELD_CANVAS))->setSize(newPosition);
					}
				}*/
			} else if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(BUTTON_DETERMINE)))) {
				if (isTraining) {
					Matrix<int> pixels(28, 28);
					for (int i = 0; i < circle.size(); i++) {
						int row = circle.at(i).getPosition().y;
						int col = circle.at(i).getPosition().x;
						if (inObject(circle.at(i).getPosition(), static_cast<RectangleShape*>(objects.at(CANVAS))))
							pixels((row - 36) / 36, (col - 36) / 36) = 252;
					}

					Matrix<int> preparedPixels = prepareValues(pixels);
					saveTrainData(preparedPixels);

					ifstream trainData;
					trainData.open("trainData.csv", ios::in);
					if (!trainData.is_open()) {
						exit(EXIT_FAILURE);
					}

					int count = 0;
					while (trainData.ignore(numeric_limits<streamsize>::max(), '\n')) {
						if (!trainData.eof())
							count++;
					}

					ostringstream out;
					out << count;
					static_cast<Text*>(objects.at(TEXT_DETERMINE))->setString(out.str());

					trainData.close();
				} else {
					Matrix<double> pixels(28, 28);
					for (int i = 0; i < circle.size(); i++) {
						int row = circle.at(i).getPosition().y;
						int col = circle.at(i).getPosition().x;
						if (inObject(circle.at(i).getPosition(), static_cast<RectangleShape*>(objects.at(CANVAS))))
							pixels((row - 36) / 36, (col - 36) / 36) = 252;
					}

					Matrix<double> preparedPixels = prepareValues(pixels);
					Matrix<double> result = network.query(preparedPixels);
					int maxIndex = 0;
					for (int i = 0; i < result.getSize(); i++) {
						if (result(maxIndex, 0) < result(i, 0)) maxIndex = i;
					}

					ostringstream out;
					out << "Output network: ";
					out << maxIndex;
					out << " " << result(maxIndex, 0);
					static_cast<Text*>(objects.at(TEXT_DETERMINE))->setString(out.str());
				}
				mousePressed = false;
			} else if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(BUTTON_CLEAR)))) {
				circle.clear();
				if (!isTraining) {
					static_cast<Text*>(objects.at(TEXT_DETERMINE))->setString(string());
					mousePressed = false;
				}
			} else if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(BUTTON_TRAIN)))) {
				ostringstream out;
				out << "Trained...\n Wait, please";
				static_cast<Text*>(objects.at(TEXT_DETERMINE))->setString(out.str());
				out.clear();
				
				network.trainNetwork(3);

				out << "Complete!";
				static_cast<Text*>(objects.at(TEXT_DETERMINE))->setString(out.str());
				mousePressed = false;
			}
		}
		
		window->draw(*s_background);
		
		for (int i = 0; i < circle.size(); i++) {
			window->draw(circle.at(i));
		}
		
		for (auto it = objects.begin(); it != objects.end(); it++) {
			window->draw(**it);
		}
		window->display();
	}
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete *it;
	}
	delete mainFont;
	delete font;
	delete background;
	delete menu;
	delete RedVad;
	delete window;
	return 0;
}