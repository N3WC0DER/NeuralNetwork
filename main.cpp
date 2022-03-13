#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>

#include "lib/Matrix.h"
#include "NeuralNetwork.h"
#include "lib/sigmoida.h"
#include "lib/random.h"
#include <SFML/Graphics.hpp>

#define TRAI
#define TRAIN_9

using namespace sf;

bool inObject(const Vector2f &mousePosition, const RectangleShape &object) {
	if (mousePosition.x > object.getPosition().x && 
		mousePosition.y > object.getPosition().y && 
		mousePosition.x < object.getPosition().x + object.getSize().x && 
		mousePosition.y < object.getPosition().y + object.getSize().y) {
		return true;
	}
	else return false;
}

#if defined(TRAIN)
void saveTrainData(Matrix<int> &pixels, const NeuralNetwork &network) {
	Matrix<int> inputs = network.prepareValues(pixels);
	ofstream trainData;
	trainData.open("train.csv",ios::app);
	if (!trainData.is_open()) {
		exit(EXIT_FAILURE);
	}

	string line = "";
	for (int i = 0; i < inputs.getRows(); i++) {
		for (int j = 0; j < inputs.getCols(); j++) {
			line += to_string(inputs(i, j));
			line.push_back(',');
		}
	}

#if defined(TRAIN_0)
	trainData << "0,";
#elif defined(TRAIN_1)
	trainData << "1,";
#elif defined(TRAIN_2)
	trainData << "2,";
#elif defined(TRAIN_3)
	trainData << "3,";
#elif defined(TRAIN_4)
	trainData << "4,";
#elif defined(TRAIN_5)
	trainData << "5,";
#elif defined(TRAIN_6)
	trainData << "6,";
#elif defined(TRAIN_7)
	trainData << "7,";
#elif defined(TRAIN_8)
	trainData << "8,";
#elif defined(TRAIN_9)
	trainData << "9,";
#endif

	trainData << line;
	trainData << '\n';
	trainData.close();
}
#endif

int main(){
	srand(static_cast<unsigned int>(time(0)));
	rand();
	const int inputNodes = 784,
			  hiddenNodes = 100,
			  outputNodes = 10;
	double learningRate = 0.5;

	NeuralNetwork network(inputNodes, hiddenNodes, outputNodes, learningRate);
#ifndef TRAIN
	network.trainNetwork(100);
#endif

	RenderWindow window(VideoMode(420, 910), "NeuralNetwork");

	bool mousePressed = false;
	vector<CircleShape> circle;
#ifdef TRAIN
	Matrix<int> pixels(28, 28);
#else
	Matrix<double> pixels(28, 28);
#endif
	RectangleShape field(Vector2f(392.f, 392.f));
	field.move(14, 14);
	field.setFillColor(Color(255, 255, 255));

	RectangleShape buttonDetermine(Vector2f(392.f, 50.f));
	buttonDetermine.move(10, 430);
	buttonDetermine.setFillColor(Color(255, 255, 255));
	buttonDetermine.setOutlineThickness(1.f);
	buttonDetermine.setOutlineColor(Color(0, 0, 0));

	RectangleShape buttonClear(Vector2f(392.f, 50.f));
	buttonClear.move(10, 490);
	buttonClear.setFillColor(Color(255, 255, 255));
	buttonClear.setOutlineThickness(1.f);
	buttonClear.setOutlineColor(Color(0, 0, 0));

	Font fontTextButton;
#if defined(__ANDROID__)
	if (!fontTextButton.loadFromFile("arialmt.ttf")) {
		return EXIT_FAILURE;
	}
#else
	if (!fontTextButton.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
		return EXIT_FAILURE;
}
#endif

	Text textButtonDetermine;
	textButtonDetermine.setFont(fontTextButton);
	textButtonDetermine.move(136, 435);
	textButtonDetermine.setString("Determine");
	textButtonDetermine.setCharacterSize(30);
	textButtonDetermine.setFillColor(Color(0, 0, 0));

	Text textButtonClear;
	textButtonClear.setFont(fontTextButton);
	textButtonClear.move(166, 495);
	textButtonClear.setString("Clear");
	textButtonClear.setCharacterSize(30);
	textButtonClear.setFillColor(Color(0, 0, 0));

	RectangleShape fieldOut(Vector2f(392.f, 200.f));
	fieldOut.move(10, 550);
	fieldOut.setFillColor(Color(255, 255, 255));
	fieldOut.setOutlineThickness(1.f);
	fieldOut.setOutlineColor(Color(0, 0, 0));

	Text textDetermine;
	textDetermine.setFont(fontTextButton);
	textDetermine.move(20, 560);
	textDetermine.setCharacterSize(30);
	textDetermine.setFillColor(Color(0, 0, 0));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
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

	#if defined(__ANDROID__)
		if (mousePressed) {
			Vector2f touchPosition = window.mapPixelToCoords(Touch::getPosition(0, window));
			if (inObject(touchPosition, field)) {
				CircleShape tempCircle(5.f);
				tempCircle.move(touchPosition.x, touchPosition.y);
				tempCircle.setFillColor(Color(0, 0, 0));
				circle.push_back(tempCircle);
			}
			if (inObject(touchPosition, buttonDetermine)) {
			#if defined(TRAIN)
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
				}
				saveTrainData(pixels, network);
				ifstream trainData;
				trainData.open("train.csv", ios::in);
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
				textDetermine.setString(out.str());

				trainData.close();
				mousePressed = false;
			#else
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
				}

				Matrix<double> preparedPixels = network.prepareValues(pixels);
				Matrix<double> outputs = network.query(preparedPixels);
				int maxIndex = 0;
				for (int i = 0; i < network.getOutputNodes(); i++) {
					if (outputs(maxIndex, 0) < outputs(i, 0)) maxIndex = i;
				}

				ostringstream out;
				out << "Output network: ";
				out << maxIndex;
				out << " " << outputs(maxIndex, 0);
				textDetermine.setString(out.str());
				mousePressed = false;
			#endif
			}
			if (inObject(touchPosition, buttonClear)) {
				circle.clear();
				textDetermine.setString(string());
				mousePressed = false;
			}
		}
	#else
		if (mousePressed) {
			Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
			if (inObject(mousePosition, field)) {
				CircleShape tempCircle(5.f);
				tempCircle.move(mousePosition.x, mousePosition.y);
				tempCircle.setFillColor(Color(0, 0, 0));
				circle.push_back(tempCircle);
			}
			if (inObject(mousePosition, buttonDetermine)) {
			#if defined(TRAIN)
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
				}
				saveTrainData(pixels, network);
				ifstream trainData;
				trainData.open("train.csv", ios::in);
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
				textDetermine.setString(out.str());

				trainData.close();
				mousePressed = false;
			#else
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
				}

				Matrix<double> preparedPixels = network.prepareValues(pixels);
				Matrix<double> outputs = network.query(preparedPixels);
				int maxIndex = 0;
				for (int i = 0; i < network.getOutputNodes(); i++) {
					if (outputs(maxIndex, 0) < outputs(i, 0)) maxIndex = i;
				}
				cout << "Output network: " << maxIndex << endl;
				cout << outputs << endl;

				ostringstream out;
				out << maxIndex;
				textDetermine.setString(out.str());
				mousePressed = false;
			#endif
			}
			if (inObject(mousePosition, buttonClear)) {
				circle.clear();
				#ifndef TRAIN
				textDetermine.setString(string());
				#endif
				mousePressed = false;
			}
		}
	#endif

		window.draw(field);
		window.draw(buttonDetermine);
		window.draw(textButtonDetermine);
		window.draw(buttonClear);
		window.draw(textButtonClear);
		window.draw(fieldOut);
		for (int i = 0; i < circle.size(); i++) {
			window.draw(circle.at(i));
		}
		window.draw(textDetermine);
		window.display();
	}
	return 0;
}