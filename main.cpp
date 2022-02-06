#include <iostream>
#include <ctime>
#include <fstream>

#include "lib/Matrix.h"
#include "NeuralNetwork.h"
#include "lib/sigmoida.h"
#include <SFML/Graphics.hpp>

using namespace sf;

bool inObject(const Vector2f& mousePosition, const RectangleShape& object) {
	if (mousePosition.x > object.getPosition().x && mousePosition.y > object.getPosition().y && mousePosition.x < object.getPosition().x + object.getSize().x && mousePosition.y < object.getPosition().y + object.getSize().y) {
		return true;
	}
	else return false;
}

void trainNetwork(NeuralNetwork &network, const int countIteration){
	ifstream trainDataset;
	trainDataset.open("mnist_dataset/mnist_train.csv", ios::in);
	vector<string> lines;
	lines.resize(countIteration);

	string tempLine;

	trainDataset >> tempLine;

	vector<double> pixels;
	int index = 0;
	Matrix<double> targets(network.getOutputNodes(), 1);
	Matrix<double> inputs(network.getInputNodes(), 1);
	for(int i = 0; i < countIteration; i++){
		cout << i << endl;

		string line = "";
		getline(trainDataset, line, ',');
		index = stoi(line);
		cout << index << endl;

		line = "";
		for (int j = 0; j < network.getInputNodes(); j++) {
			getline(trainDataset, line, ',');
			pixels.push_back(stoi(line));
		}
		cout << pixels.size() << endl;

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

	cout << "Train completed!" << endl;

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

Matrix<double> prepareValues(Matrix<double> &pixels) {
	cout << pixels << endl;
	for (int i = 1; i < pixels.getRows()-1; i++) {
		for (int j = 1; j < pixels.getCols()-1; j++) {
			if (pixels(i, j) == 0 && pixels(i+1, j) == 255 || pixels(i, j+1) == 255 || pixels(i-1, j) == 255 || pixels(i, j-1) == 255) {
				pixels(i, j) = 176;
			}
		}
	}
	cout << pixels << endl;

	for (int i = 0; i < pixels.getRows(); i++) {
		for (int j = 0; j < pixels.getCols(); j++) {
			pixels(i, j) = (pixels(i, j) / 255 * 0.99) + 0.01;
		}
	}
	//cout << pixels << endl;

	Matrix<double> inputs(1, pixels.getSize());
	for (int i = 0; i < pixels.getSize(); i++) {
		inputs(0, i) = pixels(i % 28, i / 28);
	}
	pixels();
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

	RenderWindow window(VideoMode(420, 910), "NeuralNetwork");

	bool mousePressed = false;
	std::vector<CircleShape> circle;
	Matrix<double> pixels(28, 28);

	while (window.isOpen()) {
		Vector2i mousePixelPos = Mouse::getPosition(window);
		Vector2f mousePosition = window.mapPixelToCoords(mousePixelPos);

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
		if (!fontTextButton.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
			return EXIT_FAILURE;
		}

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

		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
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
			}
		}
		window.clear(Color(250, 240, 240, 0));

		if (mousePressed) {
			if (inObject(mousePosition, field)) {
				CircleShape tempCircle(5.f);
				tempCircle.move(mousePosition.x, mousePosition.y);
				tempCircle.setFillColor(Color(0, 0, 0));
				circle.push_back(tempCircle);
			}
			if (inObject(mousePosition, buttonDetermine)) {
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 255;
				}
				Matrix<double> preparedPixels = prepareValues(pixels);
				Matrix<double> outputs = network.query(preparedPixels);
				int maxIndex = 0;
				for (int i = 0; i < network.getOutputNodes(); i++) {
					if (outputs(maxIndex, 0) < outputs(i, 0)) maxIndex = i;
				}
				cout << "Output network: " << maxIndex << endl;
				cout << outputs(maxIndex, 0) << endl;

				textDetermine.setFont(fontTextButton);
				textDetermine.move(20, 560);
				textDetermine.setString(String(to_string(maxIndex)));
				textDetermine.setCharacterSize(30);
				textDetermine.setFillColor(Color(0, 0, 0));
				mousePressed = false;
			}
			if (inObject(mousePosition, buttonClear)) {
				circle.clear();
				mousePressed = false;
			}
		}

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
	system("pause");
	return 0;
}