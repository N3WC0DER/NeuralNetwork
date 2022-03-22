#include <iostream>
#include <ctime>
#include "lib/Matrix.h"

#include "NeuralNetwork.h"
#include "lib/sigmoida.h"
#include "lib/random.h"
#include <SFML/Graphics.hpp>

using namespace sf;
/*
void trainNetwork(NeuralNetwork &network, const int countIteration){
	ifstream trainDataset;
	trainDataset.open("mnist_dataset/mnist_train.csv", ios::in);
	vector<string> lines;
	lines.resize(countIteration);

	string tempLine;

	trainDataset >> tempLine;

	for(int i = 0; i < countIteration; i++){
		trainDataset >> lines[i];
	}

	vector<double> pixels;
	int index = 0;
	vector<char> buff;
	Matrix<double> targets(network.getOutputNodes(), 1);
	Matrix<double> inputs(network.getInputNodes(), 1);
	for(int i = 0; i < countIteration; i++){
		for(int j = 0; j < lines[i].size(); j++){
			if(isdigit(lines[i][j])){
				if(j == 0){
					index = lines[i][j] - '0';
					continue;
				}
				buff.push_back(lines[i][j]);
				if(j != lines[i].size()-1) continue;
			}
			if(buff.size() == 3) pixels.push_back((buff[0] - '0') * 100 + (buff[1] - '0') * 10 + (buff[2] - '0'));
			else if(buff.size() == 2) pixels.push_back((buff[0] - '0') * 10 + buff[1] - '0');
			else if(buff.size() == 1) pixels.push_back(buff[0] - '0');
			
			buff.clear();
		}

		for(int j = 0; j < pixels.size(); j++){
			pixels[j] = (pixels[j] / 255 * 0.99) + 0.01;
		}

		for(int j = 0; j < network.getOutputNodes(); j++){
			targets(j, 0) = 0.01;
			if(j == index) targets(j, 0) = 0.99;
		}

		for(int j = 0; j < network.getInputNodes(); j++){
			inputs(j, 0) = pixels[j];
		}

		network.train(inputs, targets);
		pixels.clear();
	}

	trainDataset.close();
}

void testNetwork(NeuralNetwork &network, const int countIteration){
	ifstream testDataset;
	testDataset.open("mnist_dataset/mnist_test.csv", ios::in);

	vector<string> lines;
	lines.resize(countIteration);

	string tempLine;

	testDataset >> tempLine;

	for(int i = 0; i < countIteration; i++){
		testDataset >> lines[i];
	}

	vector<double> pixels;
	int index;
	vector<char> buff;
	Matrix<double> targets(1, network.getOutputNodes());
	Matrix<double> inputs(1, network.getInputNodes());
	int result = 0;
	for(int i = 0; i < countIteration; i++){
		for(int j = 0; j < lines[i].size(); j++){
			if(isdigit(lines[i][j])){
				if(j == 0){
					index = lines[i][j] - '0';
					continue;
				}
				buff.push_back(lines[i][j]);
				if(!(j == lines[i].size()-1)) continue;
			}
			if(buff.size() == 3) pixels.push_back((buff[0] - '0') * 100 + (buff[1] - '0') * 10 + (buff[2] - '0'));
			else if(buff.size() == 2) pixels.push_back((buff[0] - '0') * 10 + buff[1] - '0');
			else if(buff.size() == 1) pixels.push_back(buff[0] - '0');
			
			buff.clear();
		}

		for(int j = 0; j < pixels.size(); j++){
			pixels[j] = (pixels[j] / 255 * 0.99) + 0.01;
		}

		for(int j = 0; j < network.getOutputNodes(); j++){
			targets(0, j) = 0.01;
			if(j == index) targets(0, j) = 0.99;
		}

		for(int j = 0; j < network.getInputNodes(); j++){
			inputs(0, j) = pixels[j];
		}

		Matrix<double> outputs = network.query(inputs);
		pixels.clear();
		cout << "------------" << i+1 << " attempts: " << endl;

		int maxIndex = 0;
		for(int j = 0; j < network.getOutputNodes(); j++){
			if(outputs(maxIndex, 0) < outputs(j, 0)) maxIndex = j;
		}

		cout << "Output network: " << maxIndex << endl;
		cout << "Target output: " << index << endl;
		cout << outputs(maxIndex, 0) << endl;
		if(maxIndex == index) result++;
	}

	cout << "Result: " << endl;
	cout << (double) result / (double) countIteration * 100 << "% (" << result << "/" << countIteration << ")" << endl;
	testDataset.close();
}
*/

bool inObject(const Vector2f& mousePosition, const RectangleShape& object) {
	if (mousePosition.x > object.getPosition().x &&
		mousePosition.y > object.getPosition().y &&
		mousePosition.x < object.getPosition().x + object.getSize().x &&
		mousePosition.y < object.getPosition().y + object.getSize().y) {
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

int main(){
	srand(static_cast<unsigned int>(time(0)));
	rand();

	const int inputNodes = 784,
			  hiddenNodes = 100,
			  outputNodes = 10;
	double learningRate = 0.5;

	NeuralNetwork network(inputNodes, hiddenNodes, outputNodes, learningRate);

	//network.trainNetwork(10);

	RenderWindow window(VideoMode(420, 910), "NeuralNetwork");

	bool mousePressed = false;
	vector<CircleShape> circle;

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
				// Переводим координаты точек на поле (рисунка) в матрицу
				Matrix<double> pixels(28, 28);
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
				}

				// Переводим в понятный для нейронки формат
				Matrix<double> preparedPixels = prepareValues(pixels);

				// Опрос сети
				Matrix<double> result = network.query(preparedPixels);
				int maxIndex = 0;
				for (int i = 0; i < result.getSize(); i++) {
					if (result(maxIndex, 0) < result(i, 0)) maxIndex = i;
				}

				cout << maxIndex << endl;
				
				mousePressed = false;
			}
			if (inObject(mousePosition, buttonClear)) {
				circle.clear();
				textDetermine.setString(string());
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