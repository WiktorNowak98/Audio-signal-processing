#pragma once

#include "Libraries.h"
#include "SignalProcessing.h"

class WindowApp {

private:

	SignalProc Proc;	// Creating instance of WindowApp class creates instance of SignalProc class

public:

	/* Windows */

	int Main_window();		// Main thread

	int Echo_window(tgui::ChatBox::Ptr chat);

	int Save_window(tgui::ChatBox::Ptr chat);

	int Amplify_window(tgui::ChatBox::Ptr chat);

	int FadeOut_window(tgui::ChatBox::Ptr chat);

	int Noise_gate_window(tgui::ChatBox::Ptr chat);

	int FadeIn_window(tgui::ChatBox::Ptr chat);

	/* Background threads - lag/crash avoidance */

	void Echo_window_thread(tgui::ChatBox::Ptr chat);

	void Save_window_thread(tgui::ChatBox::Ptr chat);

	void Amplify_window_thread(tgui::ChatBox::Ptr chat);

	void FadeOut_window_thread(tgui::ChatBox::Ptr chat);

	void Noise_gate_window_thread(tgui::ChatBox::Ptr chat);

	void FadeIn_window_thread(tgui::ChatBox::Ptr chat);

	/* Processing and debugging */

	void Load(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);		// Loading wav file

	void Save_as(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);		// Saving wav file with new name

	void Inverse(tgui::ChatBox::Ptr chat);

	void Amplify(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);

	void Echo(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);

	void FadeIn(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);

	void FadeOut(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);

	void NoiseGate(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat);
};

/************************************************* Windows *************************************************/

int WindowApp::Main_window() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Definitely not audacity");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "60%","5%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("Path to .wav file...");
		gui.add(editBox);

		auto chatbox = tgui::ChatBox::create();
		chatbox->setRenderer(theme.getRenderer("ChatBox"));
		chatbox->setSize({"35%","40%"});
		chatbox->setTextSize(18);
		chatbox->setPosition({"30%","25%"});
		chatbox->setLinesStartFromTop();
		gui.add(chatbox);

		auto chatbox1 = tgui::ChatBox::create();
		chatbox1->setRenderer(theme.getRenderer("ChatBox"));
		chatbox1->setSize({ "35%","10%" });
		chatbox1->setTextSize(20);
		chatbox1->setPosition({ "30%","70%" });
		chatbox1->setLinesStartFromTop();
		gui.add(chatbox1);
		
		auto button = tgui::Button::create("Echo");
		button->setSize({ "15%", "5%" });
		button->setPosition({ "5%","25%" });
		button->connect("pressed", &WindowApp::Echo_window_thread, this, chatbox1);
		gui.add(button);

		auto button1 = tgui::Button::create("Load");
		button1->setSize({ "15%", "5%" });
		button1->setPosition({ "75%","5%" });
		button1->connect("pressed", &WindowApp::Load, this, editBox, chatbox);
		gui.add(button1);

		auto button2 = tgui::Button::create("Fade out");
		button2->setSize({ "15%", "5%" });
		button2->setPosition({ "5%","35%" });
		button2->connect("pressed", &WindowApp::FadeOut_window_thread, this, chatbox1);
		gui.add(button2);

		auto button3 = tgui::Button::create("Inverse");
		button3->setSize({ "15%", "5%" });
		button3->setPosition({ "5%","45%" });
		button3->connect("pressed", &WindowApp::Inverse, this, chatbox1);
		gui.add(button3);

		auto button4 = tgui::Button::create("Amplify");
		button4->setSize({ "15%", "5%" });
		button4->setPosition({ "5%","55%" });
		button4->connect("pressed", &WindowApp::Amplify_window_thread, this, chatbox1);
		gui.add(button4);

		auto button5 = tgui::Button::create("Noise gate");
		button5->setSize({ "15%", "5%" });
		button5->setPosition({ "5%","65%" });
		button5->connect("pressed", &WindowApp::Noise_gate_window_thread, this, chatbox1);
		gui.add(button5);

		auto button6 = tgui::Button::create("Fade In");
		button6->setSize({ "15%", "5%" });
		button6->setPosition({ "5%","75%" });
		button6->connect("pressed", &WindowApp::FadeIn_window_thread, this, chatbox1);
		gui.add(button6);

		auto button7 = tgui::Button::create("Save");
		button7->setSize({ "15%", "5%" });
		button7->setPosition({ "75%","25%" });
		button7->connect("pressed", &WindowApp::Save_window_thread, this, chatbox1);
		gui.add(button7);
	}
	catch (const tgui::Exception& e) {

		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

int WindowApp::Echo_window(tgui::ChatBox::Ptr chat) {

	sf::RenderWindow window(sf::VideoMode(300, 200), "Echo");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "90%","15%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("Echo by...");
		gui.add(editBox);

		auto button = tgui::Button::create("Save");
		button->setSize({ "25%", "15%" });
		button->setPosition({ "20%","45%" });
		button->connect("pressed", &WindowApp::Echo, this, editBox, chat);
		button->connect("pressed", [&]() {window.close(); });
		gui.add(button);

		auto button1 = tgui::Button::create("Cancel");
		button1->setSize({ "25%","15%" });
		button1->setPosition({ "55%","45%" });
		button1->connect("pressed", [&]() { window.close(); });
		gui.add(button1);
	}
	catch (const tgui::Exception& e) {
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

int WindowApp::Save_window(tgui::ChatBox::Ptr chat) {

	sf::RenderWindow window(sf::VideoMode(300, 200), "Save");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "90%","15%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("Save as...");
		gui.add(editBox);

		auto button = tgui::Button::create("Save");
		button->setSize({ "25%", "15%" });
		button->setPosition({ "20%","45%" });
		button->connect("pressed", &WindowApp::Save_as, this, editBox, chat);
		button->connect("pressed", [&]() {window.close(); });
		gui.add(button);

		auto button1 = tgui::Button::create("Cancel");
		button1->setSize({ "25%","15%" });
		button1->setPosition({ "55%","45%" });
		button1->connect("pressed", [&]() { window.close(); });
		gui.add(button1);
	}
	catch (const tgui::Exception& e) {
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

int WindowApp::Amplify_window(tgui::ChatBox::Ptr chat) {

	sf::RenderWindow window(sf::VideoMode(300, 200), "Amplify");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "90%","15%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("Amplify...");
		gui.add(editBox);

		auto button = tgui::Button::create("Amplify");
		button->setSize({ "25%", "15%" });
		button->setPosition({ "20%","45%" });
		button->connect("pressed", &WindowApp::Amplify, this, editBox, chat);
		button->connect("pressed", [&]() {window.close(); });
		gui.add(button);

		auto button1 = tgui::Button::create("Cancel");
		button1->setSize({ "25%","15%" });
		button1->setPosition({ "55%","45%" });
		button1->connect("pressed", [&]() { window.close(); });
		gui.add(button1);
	}
	catch (const tgui::Exception& e) {
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

int WindowApp::FadeOut_window(tgui::ChatBox::Ptr chat) {

	sf::RenderWindow window(sf::VideoMode(300, 200), "Fade Out");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "90%","15%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("Length of fade...");
		gui.add(editBox);

		auto button = tgui::Button::create("Fade out");
		button->setSize({ "25%", "15%" });
		button->setPosition({ "20%","45%" });
		button->connect("pressed", &WindowApp::FadeOut, this, editBox, chat);
		button->connect("pressed", [&]() {window.close(); });
		gui.add(button);

		auto button1 = tgui::Button::create("Cancel");
		button1->setSize({ "25%","15%" });
		button1->setPosition({ "55%","45%" });
		button1->connect("pressed", [&]() { window.close(); });
		gui.add(button1);
	}
	catch (const tgui::Exception& e) {
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

int WindowApp::Noise_gate_window(tgui::ChatBox::Ptr chat) {

	sf::RenderWindow window(sf::VideoMode(300, 200), "Noise gate");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "90%","15%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("% of noise...");
		gui.add(editBox);

		auto button = tgui::Button::create("Reduce noise");
		button->setSize({ "25%", "15%" });
		button->setPosition({ "20%","45%" });
		button->connect("pressed", &WindowApp::NoiseGate, this, editBox, chat);
		button->connect("pressed", [&]() {window.close(); });
		gui.add(button);

		auto button1 = tgui::Button::create("Cancel");
		button1->setSize({ "25%","15%" });
		button1->setPosition({ "55%","45%" });
		button1->connect("pressed", [&]() { window.close(); });
		gui.add(button1);
	}
	catch (const tgui::Exception& e) {
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

int WindowApp::FadeIn_window(tgui::ChatBox::Ptr chat) {

	sf::RenderWindow window(sf::VideoMode(300, 200), "Fade in");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	try {
		tgui::Theme theme{ "themes/Black.txt" };
		auto picture = tgui::Picture::create("themes/bg.png");
		picture->setSize({ "100%","100%" });
		gui.add(picture);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize({ "90%","15%" });
		editBox->setTextSize(18);
		editBox->setPosition({ "5%","5%" });
		editBox->setDefaultText("Length of fade...");
		gui.add(editBox);

		auto button = tgui::Button::create("Fade in");
		button->setSize({ "25%", "15%" });
		button->setPosition({ "20%","45%" });
		button->connect("pressed", &WindowApp::FadeIn, this, editBox, chat);
		button->connect("pressed", [&]() {window.close(); });
		gui.add(button);

		auto button1 = tgui::Button::create("Cancel");
		button1->setSize({ "25%","15%" });
		button1->setPosition({ "55%","45%" });
		button1->connect("pressed", [&]() { window.close(); });
		gui.add(button1);
	}
	catch (const tgui::Exception& e) {
		std::cerr << "Failed to load TGUI widgets: " << e.what() << std::endl;
		return 1;
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				gui.setView(window.getView());
			}
			gui.handleEvent(event);
		}
		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;
}

/***************************** Creating background threads for new popup windows *****************************/

void WindowApp::Echo_window_thread(tgui::ChatBox::Ptr chat) {

	std::thread t(&WindowApp::Echo_window, this, chat);
	t.detach();
}

void WindowApp::Save_window_thread(tgui::ChatBox::Ptr chat) {

	std::thread t(&WindowApp::Save_window, this, chat);
	t.detach();
}

void WindowApp::Amplify_window_thread(tgui::ChatBox::Ptr chat) {

	std::thread t(&WindowApp::Amplify_window, this, chat);
	t.detach();
}

void WindowApp::FadeOut_window_thread(tgui::ChatBox::Ptr chat) {

	std::thread t(&WindowApp::FadeOut_window, this, chat);
	t.detach();
}

void WindowApp::Noise_gate_window_thread(tgui::ChatBox::Ptr chat) {

	std::thread t(&WindowApp::Noise_gate_window, this, chat);
	t.detach();
}

void WindowApp::FadeIn_window_thread(tgui::ChatBox::Ptr chat) {

	std::thread t(&WindowApp::FadeIn_window, this, chat);
	t.detach();
}

 /************************************************* Processing *************************************************/

void WindowApp::Load(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	std::string name = box->getText().toAnsiString();
	bool isok = Proc.GetWavFile(name);

	if (isok) {

		chat->addLine("File name: " + name, tgui::Color::White);
		chat->addLine("Sample rate: " + std::to_string(Proc.Return_SampleRate()), tgui::Color::White);
		chat->addLine("Bit depth: " + std::to_string(Proc.Return_bitDepth()), tgui::Color::White);
		chat->addLine("Length: " + std::to_string(Proc.Return_lengthInSeconds()) + " sec", tgui::Color::White);
		chat->addLine("Channels: " + std::to_string(Proc.Return_numChannels()), tgui::Color::White);
		chat->addLine("Samples: " + std::to_string(Proc.Return_numSamp()), tgui::Color::White);
	}
	else {
		chat->addLine("File not found, try again.", tgui::Color::White);
	}
}

void WindowApp::Save_as(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		std::string Save = box->getText().toAnsiString();
		Proc.SaveWavFile(Save);
		chat->addLine("Saved as: " + Save, tgui::Color::White);
	}
	else {
		chat->addLine("Nothing to save", tgui::Color::White);
	}
}

void WindowApp::Inverse(tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		Proc.Inverse();
		chat->addLine("Inverted", tgui::Color::White);
	}
	else {
		chat->addLine("Nothing to invert", tgui::Color::White);
	}
}

void WindowApp::Amplify(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		std::string s = box->getText().toAnsiString();
		double amount = std::stod(s);
		Proc.Amplify(amount);
		chat->addLine("Amplified by: " + s, tgui::Color::White);
	}
	else {
		chat->addLine("Nothing to amplify", tgui::Color::White);
	}
}

void WindowApp::Echo(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		std::string s = box->getText().toAnsiString();
		double amount = std::stod(s);
		if (Proc.Return_lengthInSeconds() > amount) {
			Proc.Echo(amount);
			chat->addLine("Echoed by: " + s + " sec", tgui::Color::White);
		}
		else {
			chat->addLine("Wrong value, can't apply echo", tgui::Color::White);
		}
	}
	else {
		chat->addLine("Nothing to echo", tgui::Color::White);
	}
}

void WindowApp::FadeIn(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		std::string s = box->getText().toAnsiString();
		double amount = std::stod(s);
		if (amount < Proc.Return_lengthInSeconds()) {
			Proc.FadeIn(amount);
			chat->addLine("Fade in complete", tgui::Color::White);
		}
		else {
			chat->addLine("Too long fade", tgui::Color::White);
		}
	}
	else {
		chat->addLine("Nothing to Fade in", tgui::Color::White);
	}
}

void WindowApp::FadeOut(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		std::string s = box->getText().toAnsiString();
		double amount = std::stod(s);
		if (amount < Proc.Return_lengthInSeconds()) {
			Proc.FadeOut(amount);
			chat->addLine("Fade in complete", tgui::Color::White);
		}
		else {
			chat->addLine("Too long fade", tgui::Color::White);
		}
	}
	else {
		chat->addLine("Nothing to fade out", tgui::Color::White);
	}
}

void WindowApp::NoiseGate(tgui::EditBox::Ptr box, tgui::ChatBox::Ptr chat) {

	chat->removeAllLines();
	if (Proc.Return_numSamp() > 0) {
		std::string s = box->getText().toAnsiString();
		double amount = std::stod(s);
		if (amount >= 0 and amount <= 100) {
			Proc.Noise_Gate(amount);
			chat->addLine("Noise reduced", tgui::Color::White);
		}
		else {
			chat->addLine("Wrong value", tgui::Color::White);
		}
	}
	else {
		chat->addLine("Nothing to process", tgui::Color::White);
	}
}