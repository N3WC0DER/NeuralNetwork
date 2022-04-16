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

constexpr auto TRAIN = 0;;

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
				pixels(i, j) = getRandomNumber(132, 178);
			}
		}
	}

	//cout << pixels << endl;

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

	//cout << pixels << endl;

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
	FIELD,
	BUTTON_DETERMINE,
	BUTTON_CLEAR,
	BUTTON_TRAIN,
	TEXT_BUTTON_DETERMINE,
	TEXT_BUTTON_CLEAR,
	TEXT_BUTTON_TRAIN,
	FIELD_OUT,
	TEXT_DETERMINE
};

Font* fontTextButton;

vector<Drawable*> initObjects(){
	RectangleShape* field = new RectangleShape(Vector2f(392.f, 392.f));
	field->move(14, 14);
	field->setFillColor(Color(255, 255, 255));
  
	RectangleShape* buttonDetermine = new RectangleShape(Vector2f(392.f, 50.f));
	buttonDetermine->move(10, 430);
	buttonDetermine->setFillColor(Color(255, 255, 255));
	buttonDetermine->setOutlineThickness(1.f);
	buttonDetermine->setOutlineColor(Color(0, 0, 0));
  
	RectangleShape* buttonClear = new RectangleShape(Vector2f(392.f, 50.f));
	buttonClear->move(10, 490);
	buttonClear->setFillColor(Color(255, 255, 255));
	buttonClear->setOutlineThickness(1.f);
	buttonClear->setOutlineColor(Color(0, 0, 0));

	RectangleShape* buttonTrain = new RectangleShape(Vector2f(392.f, 50.f));
	buttonTrain->move(10, 550);
	buttonTrain->setFillColor(Color(255, 255, 255));
	buttonTrain->setOutlineThickness(1.f);
	buttonTrain->setOutlineColor(Color(0, 0, 0));

	fontTextButton = new Font;
#if defined(__ANDROID__)
	if (!fontTextButton->loadFromFile("arialmt.ttf")) {
		exit(EXIT_FAILURE);
	}
#else
	if (!fontTextButton->loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
		exit(EXIT_FAILURE);
	}
#endif
  
	Text* textButtonDetermine = new Text;
	textButtonDetermine->setFont(*fontTextButton);
	textButtonDetermine->move(136, 435);
	textButtonDetermine->setString("Determine");
	textButtonDetermine->setCharacterSize(30);
	textButtonDetermine->setFillColor(Color(0, 0, 0));
  
	Text* textButtonClear = new Text;
	textButtonClear->setFont(*fontTextButton);
	textButtonClear->move(166, 495);
	textButtonClear->setString("Clear");
	textButtonClear->setCharacterSize(30);
	textButtonClear->setFillColor(Color(0, 0, 0));

	Text* textButtonTrain = new Text;
	textButtonTrain->setFont(*fontTextButton);
	textButtonTrain->move(166, 555);
	textButtonTrain->setString("Train");
	textButtonTrain->setCharacterSize(30);
	textButtonTrain->setFillColor(Color(0, 0, 0));
  
	RectangleShape* fieldOut = new RectangleShape(Vector2f(392.f, 200.f));
	fieldOut->move(10, 610);
	fieldOut->setFillColor(Color(255, 255, 255));
	fieldOut->setOutlineThickness(1.f);
	fieldOut->setOutlineColor(Color(0, 0, 0));
  
	Text* textDetermine = new Text;
	textDetermine->setFont(*fontTextButton);
	textDetermine->move(20, 620);
	textDetermine->setCharacterSize(30);
	textDetermine->setFillColor(Color(0, 0, 0));
	
	vector<Drawable*> objects;
	
	objects.push_back(field);
	objects.push_back(buttonDetermine);
	objects.push_back(buttonClear);
	objects.push_back(buttonTrain);
	objects.push_back(textButtonDetermine);
	objects.push_back(textButtonClear);
	objects.push_back(textButtonTrain);
	objects.push_back(fieldOut);
	objects.push_back(textDetermine);
	
	return objects;
}

int main(){
	srand(static_cast<unsigned int>(time(0)));
	rand();

	const int inputNodes = 784,
			  hiddenNodes = 100,
			  outputNodes = 10;
	double learningRate = 0.3;

	NeuralNetwork network(inputNodes, hiddenNodes, outputNodes, learningRate);

	//network.trainNetwork(5, 3);

	RenderWindow window(VideoMode(420, 910), "NeuralNetwork");

	bool mousePressed = false;
	vector<CircleShape> circle;
	vector<Drawable*> objects = initObjects();
	bool isTraining = false;
	
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				for (auto it = objects.begin(); it != objects.end(); it++) {
					delete *it;
				}
				delete fontTextButton;
				window.close();
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
		window.clear(Color(221, 221, 221, 0));

		if (mousePressed) {
		#ifdef __ANDROID__
			Vector2f touchPosition = window.mapPixelToCoords(Touch::getPosition(0, window));
		#else
			Vector2f touchPosition = window.mapPixelToCoords(Mouse::getPosition(window));
		#endif
			if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(FIELD)))) {
				CircleShape tempCircle(5.f);
				tempCircle.move(touchPosition.x, touchPosition.y);
				tempCircle.setFillColor(Color(0, 0, 0));
				circle.push_back(tempCircle);
			} else if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(BUTTON_DETERMINE)))) {
				if (isTraining) {
					Matrix<int> pixels(28, 28);
					for (int i = 0; i < circle.size(); i++) {
						pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
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
						pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
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
				isTraining = true;
				mousePressed = false;
			}
		}
		
		for (auto it = objects.begin(); it != objects.end(); it++) {
			window.draw(**it);
		}
		
		for (int i = 0; i < circle.size(); i++) {
			window.draw(circle.at(i));
		}
		window.display();
	}
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete *it;
	}
	delete fontTextButton;
	return 0;
}