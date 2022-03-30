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

enum Objects{
	FIELD,
	BUTTON_DETERMINE,
	BUTTON_CLEAR,
	TEXT_BUTTON_DETERMINE,
	TEXT_BUTTON_CLEAR,
	FIELD_OUT,
	TEXT_DETERMINE
};

Font* fontTextButton;

vector<Drawable*> initObjects(){
	//map<string, Drawable*>* objects = new map<string, Drawable*>;
  
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
  
	RectangleShape* fieldOut = new RectangleShape(Vector2f(392.f, 200.f));
	fieldOut->move(10, 550);
	fieldOut->setFillColor(Color(255, 255, 255));
	fieldOut->setOutlineThickness(1.f);
	fieldOut->setOutlineColor(Color(0, 0, 0));
  
	Text* textDetermine = new Text;
	textDetermine->setFont(*fontTextButton);
	textDetermine->move(20, 560);
	textDetermine->setCharacterSize(30);
	textDetermine->setFillColor(Color(0, 0, 0));
  
	/*map<string, Drawable*>* objects = new map<string, Drawable*>{
		{"field", field}, 
		{"buttonDetermine", buttonDetermine}, 
		{"buttonClear", buttonClear}, 
		{"textButtonDetermine", textButtonDetermine}, 
		{"textButtonClear", textButtonClear}, 
		{"fieldOut", fieldOut}, 
		{"textDetermine", textDetermine}
	};*/
	
	vector<Drawable*> objects;
	
	objects.push_back(field);
	objects.push_back(buttonDetermine);
	objects.push_back(buttonClear);
	objects.push_back(textButtonDetermine);
	objects.push_back(textButtonClear);
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

	network.trainNetwork(60100, 1);

	RenderWindow window(VideoMode(420, 910), "NeuralNetwork");

	bool mousePressed = false;
	vector<CircleShape> circle;
	vector<Drawable*> objects = initObjects();
	
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
			}
			if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(BUTTON_DETERMINE)))) {
				// ��������� ���������� ����� �� ���� (�������) � �������
				Matrix<double> pixels(28, 28);
				for (int i = 0; i < circle.size(); i++) {
					pixels(circle.at(i).getPosition().y / 14 - 1, circle.at(i).getPosition().x / 14 - 1) = 252;
				}

				// ��������� � �������� ��� �������� ������
				Matrix<double> preparedPixels = prepareValues(pixels);

				// ����� ����
				Matrix<double> result = network.query(preparedPixels);
				int maxIndex = 0;
				for (int i = 0; i < result.getSize(); i++) {
					if (result(maxIndex, 0) < result(i, 0)) maxIndex = i;
				}

				cout << maxIndex << endl;

				mousePressed = false;
			}
			if (inObject(touchPosition, static_cast<RectangleShape*>(objects.at(BUTTON_CLEAR)))) {
				circle.clear();
				static_cast<Text*>(objects.at(TEXT_DETERMINE))->setString(string());
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