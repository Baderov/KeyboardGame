#include "pch.h"
#include "TGUI.h"

GameLanguage tempGameLanguage;
sf::Vector2u tempWindowSize;
unsigned int tempFPSLimiter;
bool tempIsFullscreen, tempIsVsync;

void widgetStateHandler(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	std::string tempResolution = "", width = "", height = "";

	auto resolutionComboBox = gw->pageGUI.get<tgui::ComboBox>("resolutionComboBox");
	auto languageComboBox = gw->pageGUI.get<tgui::ComboBox>("languageComboBox");
	auto fpsComboBox = gw->pageGUI.get<tgui::ComboBox>("fpsComboBox");
	auto fullscreenCheckBox = gw->pageGUI.get<tgui::CheckBox>("fullscreenCheckBox");
	auto vsyncCheckBox = gw->pageGUI.get<tgui::CheckBox>("vsyncCheckBox");

	switch (widgetState)
	{
	case WidgetState::ResolutionComboBoxChanged:
		pageAction = PageAction::Nothing;
		tempResolution = resolutionComboBox->getSelectedItem().trim().toStdString();
		width = tempResolution.substr(0, tempResolution.find('x'));
		height = tempResolution.substr(tempResolution.find('x') + 1, tempResolution.size());
		tempWindowSize = sf::Vector2u(std::atoi(width.c_str()), std::atoi(height.c_str()));
		break;

	case WidgetState::LanguageComboBoxChanged:
		if (languageComboBox->getSelectedItemId() == "0") { tempGameLanguage = GameLanguage::English; }
		else if (languageComboBox->getSelectedItemId() == "1") { tempGameLanguage = GameLanguage::Russian; }
		break;

	case WidgetState::FPSComboBoxChanged:
		if (fpsComboBox->getSelectedItemId() == "0") { tempFPSLimiter = 60; }
		else if (fpsComboBox->getSelectedItemId() == "1") { tempFPSLimiter = 75; }
		else if (fpsComboBox->getSelectedItemId() == "2") { tempFPSLimiter = 100; }
		else if (fpsComboBox->getSelectedItemId() == "3") { tempFPSLimiter = 144; }
		break;

	case WidgetState::FullscreenCheckBoxChanged:
		if (fullscreenCheckBox->isChecked()) { tempIsFullscreen = true; }
		else { tempIsFullscreen = false; }
		break;

	case WidgetState::VsyncCheckBoxChanged:
		if (vsyncCheckBox->isChecked()) { tempIsVsync = true; }
		else { tempIsVsync = false; }
		break;

	}
	widgetState = WidgetState::Nothing;
}

void menuApplyButtonPressed(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	gv->setGameLanguage(std::move(tempGameLanguage));
	gw->setSize(std::move(tempWindowSize));
	gw->setFPSLimiter(std::move(tempFPSLimiter));
	gw->setIsFullscreen(std::move(tempIsFullscreen));
	gw->setIsVsync(std::move(tempIsVsync));

	gw->createWindow(gw->getSize(), gw->getIsFullscreen(), gw->getIsVsync(), gw->getFPSLimiter());
	graphicsSettingsUpdate(gv, gw);

	createVirtualKeyboard(gv, gw);
	createWord(gv, gw);
	createGameInfo(gv, gw);

	if (gameState != GameState::MainMenu) { updateWord(gv, gw); }
}





void homePageUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	gw->pageGUI.removeAllWidgets();

	tgui::Label::Ptr homePageLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { homePageLabel->setText(L"HOME PAGE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { homePageLabel->setText(L"ДОМАШНЯЯ СТРАНИЦА"); }
	homePageLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	homePageLabel->setOrigin(0.5f, 0.5f);
	homePageLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	homePageLabel->setPosition("50%", "10%");
	gw->pageGUI.add(homePageLabel, "homePageLabel");

	tgui::Button::Ptr regButton = tgui::Button::create();
	regButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	regButton->getRenderer()->setTextColor(tgui::Color::Black);
	regButton->setSize("15%", "7%");
	regButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { regButton->setText(L"REGISTRATION"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { regButton->setText(L"РЕГИСТРАЦИЯ"); }
	regButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 66.f)));
	regButton->setPosition("60%", "40%");
	regButton->onPress([] { pageAction = PageAction::OpenRegPage; });
	gw->pageGUI.add(regButton, "regButton");

	tgui::Button::Ptr authButton = tgui::Button::create();
	authButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	authButton->getRenderer()->setTextColor(tgui::Color::Black);
	authButton->setSize("15%", "7%");
	authButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { authButton->setText(L"AUTHORIZATION"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { authButton->setText(L"АВТОРИЗАЦИЯ"); }
	authButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 66.f)));
	authButton->setPosition("40%", "40%");
	authButton->onPress([] { pageAction = PageAction::OpenAuthPage; });
	gw->pageGUI.add(authButton, "authButton");

	tgui::Button::Ptr exitButton = tgui::Button::create();
	exitButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	exitButton->getRenderer()->setTextColor(tgui::Color::Black);
	exitButton->setSize("10%", "7%");
	exitButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { exitButton->setText(L"EXIT"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { exitButton->setText(L"ВЫХОД"); }
	exitButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 45.f)));
	exitButton->setPosition("50%", "80%");
	exitButton->onPress([&gw] { gw->window.close(); });
	gw->pageGUI.add(exitButton, "exitButton");

	tgui::Label::Ptr languageLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { languageLabel->setText(L"Language"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { languageLabel->setText(L"Язык"); }
	languageLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	languageLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	languageLabel->setPosition("70%", "90%");
	gw->pageGUI.add(languageLabel, "languageLabel");

	tgui::ComboBox::Ptr languageComboBox = tgui::ComboBox::create();
	languageComboBox->setSize("10.4%", "3.7%");
	languageComboBox->getRenderer()->setTextColor(tgui::Color::Black);
	languageComboBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
	languageComboBox->addItem(L"English", "0");
	languageComboBox->addItem(L"Русский", "1");
	languageComboBox->setPosition("80%", "90%");
	if (gv->getGameLanguage() == GameLanguage::English) { languageComboBox->setSelectedItemById("0"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { languageComboBox->setSelectedItemById("1"); }
	languageComboBox->onItemSelect([&gv, &gw, languageComboBox]()
		{
			if (languageComboBox->getSelectedItemId() == "0") { gv->setGameLanguage(GameLanguage::English); }
			else if (languageComboBox->getSelectedItemId() == "1") { gv->setGameLanguage(GameLanguage::Russian); }
			homePageUpdate(gv, gw);
		});
	gw->pageGUI.add(languageComboBox, "languageComboBox");
}

void regPageUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	gv->setAvatarNumber(0);
	gv->setNickname(L"");
	gv->setPassword(L"");
	gw->pageGUI.removeAllWidgets();

	tgui::Label::Ptr errorLabel = tgui::Label::create();
	errorLabel->setText(L"");
	errorLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	errorLabel->setOrigin(0.5f, 0.5f);
	errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
	errorLabel->setPosition("50%", "90%");
	gw->pageGUI.add(errorLabel, "errorLabel");

	tgui::Label::Ptr regPageLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { regPageLabel->setText(L"REGISTRATION PAGE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { regPageLabel->setText(L"СТРАНИЦА РЕГИСТРАЦИИ"); }
	regPageLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	regPageLabel->setOrigin(0.5f, 0.5f);
	regPageLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	regPageLabel->setPosition("50%", "10%");
	gw->pageGUI.add(regPageLabel, "regPageLabel");

	tgui::Label::Ptr enterNicknameLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { enterNicknameLabel->setText(L"ENTER NICKNAME"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { enterNicknameLabel->setText(L"ВВЕДИТЕ НИКНЕЙМ"); }
	enterNicknameLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	enterNicknameLabel->setOrigin(0.5f, 0.5f);
	enterNicknameLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	enterNicknameLabel->setPosition("35%", "25%");
	gw->pageGUI.add(enterNicknameLabel, "enterNicknameLabel");

	tgui::EditBox::Ptr nicknameEditBox = tgui::EditBox::create();
	nicknameEditBox->getRenderer()->setTextColor(tgui::Color::Black);
	nicknameEditBox->setSize("25%", "7%");
	nicknameEditBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 35.f)));
	nicknameEditBox->setMaximumCharacters(15);
	nicknameEditBox->setOrigin(0.5f, 0.5f);
	nicknameEditBox->setText("");
	nicknameEditBox->setPosition("35%", "31%");
	nicknameEditBox->onTextChange([&gv, &gw, nicknameEditBox]()
		{
			if (nicknameEditBox->getText().contains(' '))
			{
				nicknameEditBox->setText(nicknameEditBox->getText().substr(0, nicknameEditBox->getText().size() - 1));
			}
			else
			{
				std::wstring tempNick = nicknameEditBox->getText().toWideString();
				tempNick.erase(remove(tempNick.begin(), tempNick.end(), ' '), tempNick.end());
				gv->setNickname(tempNick);
			}

		});
	gw->pageGUI.add(nicknameEditBox, "nicknameEditBox");

	tgui::Label::Ptr enterPasswordLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { enterPasswordLabel->setText(L"ENTER PASSWORD"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { enterPasswordLabel->setText(L"ВВЕДИТЕ ПАРОЛЬ"); }
	enterPasswordLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	enterPasswordLabel->setOrigin(0.5f, 0.5f);
	enterPasswordLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	enterPasswordLabel->setPosition("65%", "25%");
	gw->pageGUI.add(enterPasswordLabel, "enterPasswordLabel");

	tgui::EditBox::Ptr passwordEditBox = tgui::EditBox::create();
	passwordEditBox->getRenderer()->setTextColor(tgui::Color::Black);
	passwordEditBox->setSize("25%", "7%");
	passwordEditBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 35.f)));
	passwordEditBox->setMaximumCharacters(15);
	passwordEditBox->setOrigin(0.5f, 0.5f);
	passwordEditBox->setText("");
	passwordEditBox->setPosition("65%", "31%");
	passwordEditBox->onTextChange([&gv, &gw, passwordEditBox]()
		{
			if (passwordEditBox->getText().contains(' '))
			{
				passwordEditBox->setText(passwordEditBox->getText().substr(0, passwordEditBox->getText().size() - 1));
			}
			else
			{
				std::wstring tempPassword = passwordEditBox->getText().toWideString();
				tempPassword.erase(remove(tempPassword.begin(), tempPassword.end(), ' '), tempPassword.end());
				gv->setPassword(tempPassword);
			}
		});
	gw->pageGUI.add(passwordEditBox, "passwordEditBox");


	tgui::Label::Ptr selectAvatarLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { selectAvatarLabel->setText(L"SELECT AVATAR"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { selectAvatarLabel->setText(L"ВЫБЕРИТЕ АВАТАР"); }
	selectAvatarLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	selectAvatarLabel->setOrigin(0.5f, 0.5f);
	selectAvatarLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	selectAvatarLabel->setPosition("35%", "46%");
	gw->pageGUI.add(selectAvatarLabel, "selectAvatarLabel");

	std::vector<tgui::Picture::Ptr> avatarsVec;
	for (size_t i = 0; i < 10; i++)
	{
		std::string avatarPath = "Images\\SmallAvatars\\" + std::to_string(i) + ".png";
		tgui::Texture avatarTexture(avatarPath);
		avatarsVec.emplace_back(tgui::Picture::create(avatarTexture));
		avatarsVec.back()->setSize("15%", "20%");
		avatarsVec.back()->setOrigin(0.5f, 0.5f);
		avatarsVec.back()->setPosition("35%", "60%");
		if (i == 0) { avatarsVec.back()->setVisible(true); }
		else { avatarsVec.back()->setVisible(false); }
		gw->pageGUI.add(avatarsVec.back(), "avatarPicture" + std::to_string(i));
	}

	tgui::Button::Ptr leftArrowButton = tgui::Button::create();
	tgui::Button::Ptr rightArrowButton = tgui::Button::create();

	leftArrowButton->getRenderer()->setBackgroundColorDisabled(tgui::Color(96, 96, 96));
	leftArrowButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	leftArrowButton->getRenderer()->setTextOutlineColor(sf::Color::Black);
	leftArrowButton->getRenderer()->setTextOutlineThickness(1.f);
	leftArrowButton->getRenderer()->setTextColor(tgui::Color::Black);
	leftArrowButton->setSize("5%", "5%");
	leftArrowButton->setOrigin(0.5f, 0.5f);
	leftArrowButton->setText("<-");
	leftArrowButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	leftArrowButton->setPosition("23%", "60%");
	leftArrowButton->setEnabled(false);
	leftArrowButton->onPress([&gv, avatarsVec, leftArrowButton, rightArrowButton, errorLabel]
		{
			if (gv->getAvatarNumber() > 0)
			{
				rightArrowButton->setEnabled(true);
				leftArrowButton->setEnabled(true);

				gv->setAvatarNumber(gv->getAvatarNumber() - 1);
				avatarsVec[gv->getAvatarNumber()]->setVisible(true);

				if (gv->getAvatarNumber() < 10) { avatarsVec[gv->getAvatarNumber() + 1]->setVisible(false); }
				if (gv->getAvatarNumber() == 0) { leftArrowButton->setEnabled(false); }
			}
		});
	gw->pageGUI.add(leftArrowButton, "leftArrowButton");

	rightArrowButton->getRenderer()->setBackgroundColorDisabled(tgui::Color(96, 96, 96));
	rightArrowButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	rightArrowButton->getRenderer()->setTextOutlineColor(sf::Color::Black);
	rightArrowButton->getRenderer()->setTextOutlineThickness(1.f);
	rightArrowButton->getRenderer()->setTextColor(tgui::Color::Black);
	rightArrowButton->setSize("5%", "5%");
	rightArrowButton->setOrigin(0.5f, 0.5f);
	rightArrowButton->setText("->");
	rightArrowButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	rightArrowButton->setPosition("47%", "60%");
	rightArrowButton->onPress([&gv, avatarsVec, leftArrowButton, rightArrowButton]
		{
			if (gv->getAvatarNumber() < 9)
			{
				leftArrowButton->setEnabled(true);
				rightArrowButton->setEnabled(true);

				gv->setAvatarNumber(gv->getAvatarNumber() + 1);
				avatarsVec[gv->getAvatarNumber()]->setVisible(true);

				if (gv->getAvatarNumber() > 1) { avatarsVec[gv->getAvatarNumber() - 1]->setVisible(false); }
				if (gv->getAvatarNumber() == 9) { rightArrowButton->setEnabled(false); }
			}
		});
	gw->pageGUI.add(rightArrowButton, "rightArrowButton");

	tgui::MessageBox::Ptr messageBox = tgui::MessageBox::create();
	messageBox->getSharedRenderer()->setTitleBarColor(tgui::Color(145, 27, 27));
	messageBox->getSharedRenderer()->setBackgroundColor(tgui::Color::Black);
	messageBox->getSharedRenderer()->setBorderColor(tgui::Color::Magenta);
	messageBox->getSharedRenderer()->setTitleColor(tgui::Color::White);
	messageBox->getSharedRenderer()->setTextColor(tgui::Color::Green);
	messageBox->getSharedRenderer()->setBorders({ "2","2","2","2" });
	messageBox->getSharedRenderer()->setTitleBarHeight(round(winSizeX / 50.f));
	messageBox->setSize("35%", "30%");
	messageBox->setOrigin(0.5f, 0.5f);
	messageBox->setPosition("50%", "50%");
	messageBox->setPositionLocked(true);
	messageBox->setTitleAlignment(tgui::ChildWindow::TitleAlignment::Center);
	messageBox->setLabelAlignment(tgui::MessageBox::Alignment::Center);
	messageBox->setButtonAlignment(tgui::MessageBox::Alignment::Center);
	messageBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	messageBox->setTitleTextSize(static_cast<unsigned int>(round(winSizeX / 60.f)));
	if (gv->getGameLanguage() == GameLanguage::English) { messageBox->setTitle(L"Registration"); messageBox->setText(L"Registration completed\n\t successfully!"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { messageBox->setTitle(L"Регистрация"); messageBox->setText(L"Регистрация прошла успешно!"); }
	messageBox->addButton(L"OK");
	messageBox->onButtonPress(([] { pageAction = PageAction::OpenHomePage; }));
	messageBox->setVisible(false);
	gw->pageGUI.add(messageBox, "messageBox");

	tgui::Button::Ptr signUpButton = tgui::Button::create();
	signUpButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	signUpButton->getRenderer()->setTextColor(tgui::Color::Black);
	signUpButton->setSize("10%", "7%");
	signUpButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { signUpButton->setText(L"SIGN UP"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { signUpButton->setText(L"РЕГИСТРАЦИЯ"); }
	signUpButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
	signUpButton->setPosition("59%", "60%");
	signUpButton->onPress([&gv, &gw, &database, &winSizeX, errorLabel, messageBox]
		{
			errorLabel->setText(L"");
			dbError = DBError::NoError;

			if (gv->getNickname().size() < 3)
			{
				if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! The nickname must contain at least 3 characters!"); }
				else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Никнейм должен содержать не менее 3 символов!"); }
			}

			if (gv->getPassword().size() < 6)
			{
				if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! The password must contain at least 6 characters!"); }
				else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Пароль должен содержать не менее 6 символов!"); }
			}

			if (gv->getNickname().size() < 3 && gv->getPassword().size() < 6)
			{
				if (gv->getGameLanguage() == GameLanguage::English)
				{
					errorLabel->setText(L"\t\t\t\t\tError!\nThe nickname must contain at least 3 characters!\nThe password must contain at least 6 characters!");
				}
				else if (gv->getGameLanguage() == GameLanguage::Russian)
				{
					errorLabel->setText(L"\t\t\t\t  Ошибка!\nНикнейм должен содержать не менее 3 символов!\nПароль должен содержать не менее 6 символов!");
				}
			}

			if (errorLabel->getText().toWideString() == L"" && database->userRegist(gv))
			{
				database->insertToConfigTable(gv);

				for (auto& el : gw->pageGUI.getWidgets())
				{
					if (el->getWidgetName() != "messageBox")
					{
						el->setEnabled(false);
						el->getSharedRenderer()->setOpacity(0.5f);
					}
					else
					{
						el->moveToFront();
						el->setVisible(true);
					}
				}
			}

			if (dbError == DBError::UserExists)
			{
				if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! The user exists!"); }
				else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Пользователь существует!"); }
			}
		});
	gw->pageGUI.add(signUpButton, "signUpButton");

	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize("10%", "7%");
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"BACK"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"НАЗАД"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
	backButton->setPosition("71%", "60%");
	backButton->onPress([] { pageAction = PageAction::OpenHomePage; });
	gw->pageGUI.add(backButton, "backButton");

}

void authPageUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	gv->setNickname(L"");
	gv->setPassword(L"");
	gw->pageGUI.removeAllWidgets();

	tgui::Label::Ptr errorLabel = tgui::Label::create();
	errorLabel->setText(L"");
	errorLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	errorLabel->setOrigin(0.5f, 0.5f);
	errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
	errorLabel->setPosition("50%", "90%");
	gw->pageGUI.add(errorLabel, "errorLabel");

	tgui::Label::Ptr authPageLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { authPageLabel->setText(L"AUTHORIZATION PAGE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { authPageLabel->setText(L"СТРАНИЦА АВТОРИЗАЦИИ"); }
	authPageLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	authPageLabel->setOrigin(0.5f, 0.5f);
	authPageLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	authPageLabel->setPosition("50%", "10%");
	gw->pageGUI.add(authPageLabel, "authPageLabel");

	tgui::Label::Ptr enterNicknameLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { enterNicknameLabel->setText(L"ENTER NICKNAME"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { enterNicknameLabel->setText(L"ВВЕДИТЕ НИКНЕЙМ"); }
	enterNicknameLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	enterNicknameLabel->setOrigin(0.5f, 0.5f);
	enterNicknameLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	enterNicknameLabel->setPosition("50%", "30%");
	gw->pageGUI.add(enterNicknameLabel, "enterNicknameLabel");

	tgui::EditBox::Ptr nicknameEditBox = tgui::EditBox::create();
	nicknameEditBox->getRenderer()->setTextColor(tgui::Color::Black);
	nicknameEditBox->setSize("25%", "7%");
	nicknameEditBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 35.f)));
	nicknameEditBox->setMaximumCharacters(15);
	nicknameEditBox->setOrigin(0.5f, 0.5f);
	nicknameEditBox->setText("");
	nicknameEditBox->setPosition("50%", "36%");
	nicknameEditBox->onTextChange([&gv, &gw, nicknameEditBox]()
		{
			if (nicknameEditBox->getText().contains(' '))
			{
				nicknameEditBox->setText(nicknameEditBox->getText().substr(0, nicknameEditBox->getText().size() - 1));
			}
			else
			{
				std::wstring tempNick = nicknameEditBox->getText().toWideString();
				tempNick.erase(remove(tempNick.begin(), tempNick.end(), ' '), tempNick.end());
				gv->setNickname(tempNick);
			}
		});
	gw->pageGUI.add(nicknameEditBox, "nicknameEditBox");

	tgui::Label::Ptr enterPasswordLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { enterPasswordLabel->setText(L"ENTER PASSWORD"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { enterPasswordLabel->setText(L"ВВЕДИТЕ ПАРОЛЬ"); }
	enterPasswordLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 38.f)));
	enterPasswordLabel->setOrigin(0.5f, 0.5f);
	enterPasswordLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	enterPasswordLabel->setPosition("50%", "46%");
	gw->pageGUI.add(enterPasswordLabel, "enterPasswordLabel");

	tgui::EditBox::Ptr passwordEditBox = tgui::EditBox::create();
	passwordEditBox->getRenderer()->setTextColor(tgui::Color::Black);
	passwordEditBox->setSize("25%", "7%");
	passwordEditBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 35.f)));
	passwordEditBox->setMaximumCharacters(15);
	passwordEditBox->setOrigin(0.5f, 0.5f);
	passwordEditBox->setText("");
	passwordEditBox->setPosition("50%", "52%");
	passwordEditBox->onTextChange([&gv, &gw, passwordEditBox]()
		{
			if (passwordEditBox->getText().contains(' '))
			{
				passwordEditBox->setText(passwordEditBox->getText().substr(0, passwordEditBox->getText().size() - 1));
			}
			else
			{
				std::wstring tempPassword = passwordEditBox->getText().toWideString();
				tempPassword.erase(remove(tempPassword.begin(), tempPassword.end(), ' '), tempPassword.end());
				gv->setPassword(tempPassword);
			}
		});
	gw->pageGUI.add(passwordEditBox, "passwordEditBox");


	tgui::Button::Ptr signInButton = tgui::Button::create();
	signInButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	signInButton->getRenderer()->setTextColor(tgui::Color::Black);
	signInButton->setSize("10%", "7%");
	signInButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { signInButton->setText(L"SIGN IN"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { signInButton->setText(L"ВОЙТИ"); }
	signInButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 66.f)));
	signInButton->setPosition("44%", "70%");
	signInButton->onPress([&gv, &database, errorLabel]
		{
			errorLabel->setText(L"");
			if (gv->getNickname().empty())
			{
				if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! The nickname must not be empty!"); }
				else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Никнейм не должен быть пустым!"); }
			}

			if (gv->getPassword().empty())
			{
				if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! The password must not be empty!"); }
				else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Пароль не должен быть пустым!"); }
			}
			if (gv->getNickname().empty() && gv->getPassword().empty())
			{
				if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! Nickname and password must not be empty!"); }
				else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Никнейм и пароль не должны быть пустыми!"); }
			}

			if (errorLabel->getText().toWideString() == L"")
			{
				if (database->userAuth(gv))
				{
					unsigned int userID = database->getUserID(gv, gv->getNickname());
					database->selectConfigTable(gv, userID);
					pageAction = PageAction::OpenMainMenu;
				}
				else
				{
					if (gv->getGameLanguage() == GameLanguage::English) { errorLabel->setText(L"Error! Invalid nickname or password entered!"); }
					else if (gv->getGameLanguage() == GameLanguage::Russian) { errorLabel->setText(L"Ошибка! Введен неверный никнейм или пароль!"); }
				}
			}

		});
	gw->pageGUI.add(signInButton, "signInButton");

	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize("10%", "7%");
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"BACK"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"НАЗАД"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 66.f)));
	backButton->setPosition("56%", "70%");
	backButton->onPress([] { pageAction = PageAction::OpenHomePage; });
	gw->pageGUI.add(backButton, "backButton");
}

void graphicsSettingsUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	tempGameLanguage = gv->getGameLanguage();
	tempWindowSize = gw->getSize();
	tempFPSLimiter = gw->getFPSLimiter();
	tempIsFullscreen = gw->getIsFullscreen();
	tempIsVsync = gw->getIsVsync();

	float winSizeX = static_cast<float>(gw->window.getSize().x);
	float winSizeY = static_cast<float>(gw->window.getSize().y);
	gw->pageGUI.removeAllWidgets();

	tgui::Button::Ptr applyButton = tgui::Button::create();
	applyButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	applyButton->getRenderer()->setTextColor(tgui::Color::Black);
	applyButton->getRenderer()->setBackgroundColorDisabled(tgui::Color(96, 96, 96));
	applyButton->getRenderer()->setTextColorDisabled(tgui::Color::Black);
	applyButton->setSize("14%", "7%");
	applyButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { applyButton->setText(L"Apply"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { applyButton->setText(L"Применить"); }
	applyButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	applyButton->setPosition("50%", "80%");
	applyButton->onPress(&menuApplyButtonPressed, std::ref(gv), std::ref(gw));
	applyButton->setEnabled(false);
	gw->pageGUI.add(applyButton, "applyButton");

	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize("14%", "7%");
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"Back"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"Назад"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	backButton->setPosition("50%", "90%");
	backButton->onPress([] { pageAction = PageAction::OpenSettings; });
	gw->pageGUI.add(backButton, "backButton");

	tgui::Label::Ptr graphicsSettingsLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { graphicsSettingsLabel->setText(L"GRAPHICS SETTINGS"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { graphicsSettingsLabel->setText(L"НАСТРОЙКИ ГРАФИКИ"); }
	graphicsSettingsLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	graphicsSettingsLabel->setOrigin(0.5f, 0.5f);
	graphicsSettingsLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	graphicsSettingsLabel->setPosition("50%", "10%");
	gw->pageGUI.add(graphicsSettingsLabel, "graphicsSettingsLabel");

	tgui::Label::Ptr resolutionLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { resolutionLabel->setText(L"Resolution"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { resolutionLabel->setText(L"Разрешение экрана"); }
	resolutionLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	resolutionLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	resolutionLabel->setPosition("5%", "22%");
	gw->pageGUI.add(resolutionLabel, "resolutionLabel");

	tgui::Label::Ptr fullscreenLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { fullscreenLabel->setText(L"Fullscreen"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { fullscreenLabel->setText(L"Полный экран"); }
	fullscreenLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	fullscreenLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	fullscreenLabel->setPosition("5%", "32%");
	gw->pageGUI.add(fullscreenLabel, "fullscreenLabel");

	tgui::Label::Ptr vsyncLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { vsyncLabel->setText(L"Vsync"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { vsyncLabel->setText(L"Верт. синхронизация"); }
	vsyncLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	vsyncLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	vsyncLabel->setPosition("5%", "42%");
	gw->pageGUI.add(vsyncLabel, "vsyncLabel");

	tgui::Label::Ptr fpsLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { fpsLabel->setText(L"FPS"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { fpsLabel->setText(L"ФПС"); }
	fpsLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	fpsLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	fpsLabel->setPosition("5%", "52%");
	gw->pageGUI.add(fpsLabel, "fpsLabel");

	tgui::Label::Ptr languageLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { languageLabel->setText(L"Language"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { languageLabel->setText(L"Язык"); }
	languageLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	languageLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	languageLabel->setPosition("5%", "62%");
	gw->pageGUI.add(languageLabel, "languageLabel");

	tgui::ComboBox::Ptr resolutionComboBox = tgui::ComboBox::create();
	resolutionComboBox->setSize("10.4%", "3.7%");
	resolutionComboBox->getRenderer()->setTextColor(tgui::Color::Black);
	resolutionComboBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
	int i = 0;
	for (auto& el : sf::VideoMode::getFullscreenModes())
	{
		resolutionComboBox->addItem(std::to_string(el.width) + "x" + std::to_string(el.height), std::to_string(i));
		i++;
	}
	resolutionComboBox->setPosition("30%", "22%");
	resolutionComboBox->setSelectedItem(std::to_string(gw->getSize().x) + "x" + std::to_string(gw->getSize().y));
	resolutionComboBox->onItemSelect([&gv, &gw, applyButton]()
		{
			widgetState = WidgetState::ResolutionComboBoxChanged;
			widgetStateHandler(gv, gw);

			applyButton->setEnabled(true);
		});
	gw->pageGUI.add(resolutionComboBox, "resolutionComboBox");

	tgui::ComboBox::Ptr languageComboBox = tgui::ComboBox::create();
	languageComboBox->setSize("10.4%", "3.7%");
	languageComboBox->getRenderer()->setTextColor(tgui::Color::Black);
	languageComboBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
	languageComboBox->addItem(L"English", "0");
	languageComboBox->addItem(L"Русский", "1");
	languageComboBox->setPosition("30%", "62%");
	if (gv->getGameLanguage() == GameLanguage::English) { languageComboBox->setSelectedItemById("0"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { languageComboBox->setSelectedItemById("1"); }
	languageComboBox->onItemSelect([&gv, &gw, applyButton]()
		{
			widgetState = WidgetState::LanguageComboBoxChanged;
			widgetStateHandler(gv, gw);

			applyButton->setEnabled(true);
		});
	gw->pageGUI.add(languageComboBox, "languageComboBox");

	tgui::ComboBox::Ptr fpsComboBox = tgui::ComboBox::create();
	fpsComboBox->getRenderer()->setTextColorDisabled(tgui::Color::Black);
	fpsComboBox->getRenderer()->setBackgroundColorDisabled(tgui::Color(96, 96, 96));
	fpsComboBox->getRenderer()->setArrowBackgroundColorDisabled(tgui::Color(96, 96, 96));
	fpsComboBox->getRenderer()->setArrowColorDisabled(tgui::Color(96, 96, 96));
	fpsComboBox->setSize("10.4%", "3.7%");
	fpsComboBox->getRenderer()->setTextColor(tgui::Color::Black);
	fpsComboBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
	fpsComboBox->addItem("60", "0");
	fpsComboBox->addItem("75", "1");
	fpsComboBox->addItem("100", "2");
	fpsComboBox->addItem("144", "3");
	fpsComboBox->setPosition("30%", "52%");
	fpsComboBox->setSelectedItem(std::to_string(gw->getFPSLimiter()));
	fpsComboBox->onItemSelect([&gv, &gw, applyButton]()
		{
			widgetState = WidgetState::FPSComboBoxChanged;
			widgetStateHandler(gv, gw);

			applyButton->setEnabled(true);
		});
	if (gw->getIsVsync()) { fpsComboBox->setEnabled(false); }
	else { fpsComboBox->setEnabled(true); }
	gw->pageGUI.add(fpsComboBox, "fpsComboBox");

	tgui::CheckBox::Ptr fullscreenCheckBox = tgui::CheckBox::create();
	fullscreenCheckBox->getRenderer()->setCheckColor(tgui::Color::Black);
	fullscreenCheckBox->setSize("2.6%", "4.6%");
	fullscreenCheckBox->setPosition("30%", "32%");
	if (gw->getIsFullscreen()) { fullscreenCheckBox->setChecked(true); }
	else { fullscreenCheckBox->setChecked(false); }
	fullscreenCheckBox->onChange([&gv, &gw, applyButton]()
		{
			widgetState = WidgetState::FullscreenCheckBoxChanged;
			widgetStateHandler(gv, gw);

			applyButton->setEnabled(true);
		});
	gw->pageGUI.add(fullscreenCheckBox, "fullscreenCheckBox");

	tgui::CheckBox::Ptr vsyncCheckBox = tgui::CheckBox::create();
	vsyncCheckBox->getRenderer()->setCheckColor(tgui::Color::Black);
	vsyncCheckBox->setSize("2.6%", "4.6%");
	vsyncCheckBox->setPosition("30%", "42%");
	if (gw->getIsVsync()) { vsyncCheckBox->setChecked(true); }
	else { vsyncCheckBox->setChecked(false); }
	vsyncCheckBox->onChange([&gv, &gw, applyButton, vsyncCheckBox, fpsComboBox]()
		{
			widgetState = WidgetState::VsyncCheckBoxChanged;
			widgetStateHandler(gv, gw);

			applyButton->setEnabled(true);
			if (vsyncCheckBox->isChecked()) { fpsComboBox->setEnabled(false); }
			else { fpsComboBox->setEnabled(true); }
		});

	gw->pageGUI.add(vsyncCheckBox, "vsyncCheckBox");
}

void settingsUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	float halfWinSizeX = gw->window.getSize().x / 2.f;
	float halfWinSizeY = gw->window.getSize().y / 2.f;
	gw->pageGUI.removeAllWidgets();

	tgui::Button::Ptr graphicsSettingsButton = tgui::Button::create();
	graphicsSettingsButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	graphicsSettingsButton->getRenderer()->setTextColor(tgui::Color::Black);
	graphicsSettingsButton->setSize(round(winSizeX / 6.5f), round(winSizeX / 12.75f));
	graphicsSettingsButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { graphicsSettingsButton->setText(L"GRAPHICS\nSETTINGS"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { graphicsSettingsButton->setText(L"НАСТРОЙКИ\n ГРАФИКИ"); }
	graphicsSettingsButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	graphicsSettingsButton->setPosition(halfWinSizeX, halfWinSizeY - round(winSizeX / 21.f));
	graphicsSettingsButton->onPress([] { pageAction = PageAction::OpenGraphicsSettings; });
	gw->pageGUI.add(graphicsSettingsButton, "graphicsSettingsButton");

	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize(round(winSizeX / 9.5f), round(winSizeX / 19.f));
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"BACK"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"НАЗАД"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	backButton->setPosition(halfWinSizeX, halfWinSizeY + round(winSizeX / 38.f));
	backButton->onPress([] { pageAction = PageAction::OpenMainMenu; });
	gw->pageGUI.add(backButton, "backButton");
}

void highScoresUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	float columnWidth = (0.6f * gw->window.getSize().x) / 2.f;
	float itemAndHeaderHeight = (0.6f * gw->window.getSize().y) / 11.f;
	database->getHS(gv, HighScores::ThirtySeconds);
	gw->pageGUI.removeAllWidgets();

	tgui::Label::Ptr HSLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { HSLabel->setText(L"HIGH SCORES"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { HSLabel->setText(L"РЕКОРДЫ"); }
	HSLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	HSLabel->setOrigin(0.5f, 0.5f);
	HSLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	HSLabel->setPosition("50%", "8%");
	gw->pageGUI.add(HSLabel, "HSLabel");

	tgui::ListView::Ptr HSListView = tgui::ListView::create();
	if (gv->getGameLanguage() == GameLanguage::English)
	{
		HSListView->addColumn(L"Nickname", columnWidth, tgui::ListView::ColumnAlignment::Center);
		HSListView->addColumn(L"Score", columnWidth, tgui::ListView::ColumnAlignment::Center);
	}
	else if (gv->getGameLanguage() == GameLanguage::Russian)
	{
		HSListView->addColumn(L"Никнейм", columnWidth, tgui::ListView::ColumnAlignment::Center);
		HSListView->addColumn(L"Счёт", columnWidth, tgui::ListView::ColumnAlignment::Center);
	}
	HSListView->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	HSListView->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	HSListView->getRenderer()->setHeaderBackgroundColor(tgui::Color::Green);
	HSListView->getRenderer()->setBorderColor(tgui::Color::Black);
	HSListView->getRenderer()->setTextColor(tgui::Color::Black);
	HSListView->getRenderer()->setSeparatorColor(tgui::Color::Black);
	HSListView->setSeparatorWidth(1);
	HSListView->setHeaderSeparatorHeight(1);
	HSListView->setGridLinesWidth(1);
	HSListView->setShowVerticalGridLines(true);
	HSListView->setShowHorizontalGridLines(true);
	HSListView->setSize("60%", "60%");
	HSListView->setHeaderHeight(itemAndHeaderHeight);
	HSListView->setItemHeight(static_cast<unsigned int>(itemAndHeaderHeight));
	HSListView->setTextSize(static_cast<unsigned int>(round(winSizeX / 45.f)));
	HSListView->setOrigin(0.5f, 0.5f);
	HSListView->setPosition("50%", "53%");

	for (size_t i = 0; i < gv->HSVec.size(); i++)
	{
		HSListView->addItem({ gv->HSVec[i].first, std::to_string(gv->HSVec[i].second) });
	}

	gw->pageGUI.add(HSListView, "HSListView");

	tgui::ComboBox::Ptr gameModeComboBox = tgui::ComboBox::create();
	gameModeComboBox->setSize("20%", "8%");
	gameModeComboBox->getRenderer()->setTextColor(tgui::Color::Black);
	gameModeComboBox->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
	gameModeComboBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 60.f)));
	gameModeComboBox->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English)
	{
		gameModeComboBox->addItem(L"30 SECONDS", "0");
		gameModeComboBox->addItem(L"1 MINUTE", "1");
		gameModeComboBox->addItem(L"2 MINUTES", "2");
		gameModeComboBox->addItem(L"ONE MISTAKE", "3");
	}
	else if (gv->getGameLanguage() == GameLanguage::Russian)
	{
		gameModeComboBox->addItem(L"30 СЕКУНД", "0");
		gameModeComboBox->addItem(L"1 МИНУТА", "1");
		gameModeComboBox->addItem(L"2 МИНУТЫ", "2");
		gameModeComboBox->addItem(L"ОДНА ОШИБКА", "3");
	}

	gameModeComboBox->setPosition("50%", "18%");
	gameModeComboBox->setSelectedItemById("0");
	gameModeComboBox->onItemSelect([&gv, &database, gameModeComboBox, HSListView]()
		{
			HSListView->removeAllItems();

			if (gameModeComboBox->getSelectedItemId() == "0")
			{
				database->getHS(gv, HighScores::ThirtySeconds);
			}
			else if (gameModeComboBox->getSelectedItemId() == "1")
			{
				database->getHS(gv, HighScores::OneMinute);
			}
			else if (gameModeComboBox->getSelectedItemId() == "2")
			{
				database->getHS(gv, HighScores::TwoMinutes);
			}
			else if (gameModeComboBox->getSelectedItemId() == "3")
			{
				database->getHS(gv, HighScores::OneMistake);
			}

			for (size_t i = 0; i < gv->HSVec.size(); i++)
			{
				HSListView->addItem({ gv->HSVec[i].first, std::to_string(gv->HSVec[i].second) });
			}

		});
	gw->pageGUI.add(gameModeComboBox, "gameModeComboBox");




	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize(round(winSizeX / 9.5f), round(winSizeX / 19.f));
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"BACK"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"НАЗАД"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	backButton->setPosition("50%", "90%");
	backButton->onPress([] { pageAction = PageAction::OpenMainMenu; });
	gw->pageGUI.add(backButton, "backButton");

}

void menuUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	float halfWinSizeX = static_cast<float>(gw->window.getSize().x) / 2.f;
	float halfWinSizeY = static_cast<float>(gw->window.getSize().y) / 2.f;
	gw->pageGUI.removeAllWidgets();

	tgui::Button::Ptr HSButton = tgui::Button::create();
	HSButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	HSButton->getRenderer()->setTextColor(tgui::Color::Black);
	HSButton->getRenderer()->setBackgroundColorDisabled(tgui::Color(96, 96, 96));
	HSButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
	HSButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { HSButton->setText(L"HIGH SCORES"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { HSButton->setText(L"РЕКОРДЫ"); }
	HSButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	HSButton->setPosition(halfWinSizeX, halfWinSizeY);
	HSButton->onPress([] { pageAction = PageAction::OpenHighScores; });
	if (dbError == DBError::SQLConnectError) { HSButton->setEnabled(false); }
	gw->pageGUI.add(HSButton, "HSButton");

	tgui::Button::Ptr settingsButton = tgui::Button::create();
	settingsButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	settingsButton->getRenderer()->setTextColor(tgui::Color::Black);
	settingsButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
	settingsButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { settingsButton->setText(L"SETTINGS"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { settingsButton->setText(L"НАСТРОЙКИ"); }
	settingsButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	settingsButton->setPosition(halfWinSizeX, halfWinSizeY + round(winSizeX / 18.f));
	settingsButton->onPress([] { pageAction = PageAction::OpenSettings; });
	gw->pageGUI.add(settingsButton, "settingsButton");

	tgui::Button::Ptr exitButton = tgui::Button::create();
	exitButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	exitButton->getRenderer()->setTextColor(tgui::Color::Black);
	exitButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
	exitButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { exitButton->setText(L"EXIT"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { exitButton->setText(L"ВЫХОД"); }
	exitButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	exitButton->setPosition(halfWinSizeX, halfWinSizeY + round(winSizeX / 8.96f));
	exitButton->onPress([&gw] { gw->window.close(); });
	gw->pageGUI.add(exitButton, "exitButton");

	if (gameState == GameState::MainMenu)
	{
		tgui::Button::Ptr startGameButton = tgui::Button::create();
		startGameButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
		startGameButton->getRenderer()->setTextColor(tgui::Color::Black);
		startGameButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
		startGameButton->setOrigin(0.5f, 0.5f);
		if (gv->getGameLanguage() == GameLanguage::English) { startGameButton->setText(L"START THE GAME"); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { startGameButton->setText(L"НАЧАТЬ ИГРУ"); }
		startGameButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
		startGameButton->setPosition(halfWinSizeX, halfWinSizeY - round(winSizeX / 18.f));
		startGameButton->onPress([] { pageAction = PageAction::OpenGameModeSelection; });
		gw->pageGUI.add(startGameButton, "startGameButton");

		if (dbError != DBError::SQLConnectError)
		{
			tgui::Picture::Ptr homePicture = tgui::Picture::create("Images\\home_icon.png");
			homePicture->setSize("8%", "10%");
			homePicture->setOrigin(0.5f, 0.5f);
			homePicture->setPosition("5%", "90%");
			homePicture->onClick([&gv]() { pageAction = PageAction::OpenHomePage; gv->setNickname(L""); gv->setPassword(L""); });
			gw->pageGUI.add(homePicture, "homePicture");

			tgui::Picture::Ptr profilePicture = tgui::Picture::create("Images\\profile_icon.png");
			profilePicture->setSize("6%", "10%");
			profilePicture->setOrigin(0.5f, 0.5f);
			profilePicture->setPosition("95%", "10%");
			profilePicture->onClick([]() { pageAction = PageAction::OpenProfile; });
			gw->pageGUI.add(profilePicture, "profilePicture");
		}

		else if (dbError == DBError::SQLConnectError && gv->getShowDBConnectionError())
		{
			gv->setNickname(L"User");

			for (auto& el : gw->pageGUI.getWidgets())
			{
				el->setEnabled(false);
				el->getSharedRenderer()->setOpacity(0.5f);
			}

			tgui::MessageBox::Ptr messageBox = tgui::MessageBox::create();
			messageBox->getSharedRenderer()->setTitleBarColor(tgui::Color(145, 27, 27));
			messageBox->getSharedRenderer()->setBackgroundColor(tgui::Color::Black);
			messageBox->getSharedRenderer()->setBorderColor(tgui::Color::Magenta);
			messageBox->getSharedRenderer()->setTitleColor(tgui::Color::White);
			messageBox->getSharedRenderer()->setTextColor(tgui::Color::Red);
			messageBox->getSharedRenderer()->setBorders({ "2","2","2","2" });
			messageBox->getSharedRenderer()->setTitleBarHeight(round(winSizeX / 50.f));
			messageBox->setSize("35%", "20%");
			messageBox->setOrigin(0.5f, 0.5f);
			messageBox->setPosition("50%", "50%");
			messageBox->setPositionLocked(true);
			messageBox->setTitleAlignment(tgui::ChildWindow::TitleAlignment::Center);
			messageBox->setLabelAlignment(tgui::MessageBox::Alignment::Center);
			messageBox->setButtonAlignment(tgui::MessageBox::Alignment::Center);
			messageBox->setTextSize(static_cast<unsigned int>(round(winSizeX / 70.f)));
			messageBox->setTitleTextSize(static_cast<unsigned int>(round(winSizeX / 65.f)));
			if (gv->getGameLanguage() == GameLanguage::English) { messageBox->setTitle(L"Database error!"); messageBox->setText(L"Failed to connect to MS SQL Server.\nThe game will launch in demo mode."); }
			else if (gv->getGameLanguage() == GameLanguage::Russian) { messageBox->setTitle(L"Ошибка базы данных!"); messageBox->setText(L"Не удалось подключиться к MS SQL Server.\nИгра будет запущена в демо-режиме."); }
			messageBox->addButton(L"OK");
			messageBox->moveToFront();
			messageBox->onButtonPress(([&gw, &gv, messageBox]
				{
					messageBox->setVisible(false);
					gv->setShowDBConnectionError(false);
					for (auto& el : gw->pageGUI.getWidgets())
					{
						if (el->getWidgetName() != "HSButton") { el->setEnabled(true); }
						el->getSharedRenderer()->setOpacity(1.f);
					}
				}));
			gw->pageGUI.add(messageBox, "messageBox");
		}
	}
	else if (gameState == GameState::GameMenu)
	{
		tgui::Button::Ptr continueButton = tgui::Button::create();
		continueButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
		continueButton->getRenderer()->setTextColor(tgui::Color::Black);
		continueButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
		continueButton->setOrigin(0.5f, 0.5f);
		if (gv->getGameLanguage() == GameLanguage::English) { continueButton->setText(L"CONTINUE"); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { continueButton->setText(L"ПРОДОЛЖИТЬ"); }
		continueButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
		continueButton->setPosition(halfWinSizeX, halfWinSizeY - round(winSizeX / 8.96f));
		continueButton->onPress([] { pageAction = PageAction::ContinueGame; });
		gw->pageGUI.add(continueButton, "continueButton");

		tgui::Button::Ptr backToMenuButton = tgui::Button::create();
		backToMenuButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
		backToMenuButton->getRenderer()->setTextColor(tgui::Color::Black);
		backToMenuButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
		backToMenuButton->setOrigin(0.5f, 0.5f);
		if (gv->getGameLanguage() == GameLanguage::English) { backToMenuButton->setText(L"BACK TO MENU"); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { backToMenuButton->setText(L"НАЗАД В МЕНЮ"); }
		backToMenuButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
		backToMenuButton->setPosition(halfWinSizeX, halfWinSizeY - round(winSizeX / 18.f));
		backToMenuButton->onPress([&gv] { pageAction = PageAction::OpenMainMenu; gameState = GameState::MainMenu; });
		gw->pageGUI.add(backToMenuButton, "backToMenuButton");
	}
	else if (gameState == GameState::GameOver || gameState == GameState::GameResult)
	{
		tgui::Button::Ptr restartButton = tgui::Button::create();
		restartButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
		restartButton->getRenderer()->setTextColor(tgui::Color::Black);
		restartButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
		restartButton->setOrigin(0.5f, 0.5f);
		if (gv->getGameLanguage() == GameLanguage::English) { restartButton->setText(L"RESTART"); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { restartButton->setText(L"ПЕРЕЗАПУСК"); }
		restartButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
		restartButton->setPosition(halfWinSizeX, halfWinSizeY - round(winSizeX / 8.96f));
		restartButton->onPress([&gv] { pageAction = PageAction::RestartGame; });
		gw->pageGUI.add(restartButton, "restartButton");

		tgui::Button::Ptr backToMenuButton = tgui::Button::create();
		backToMenuButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
		backToMenuButton->getRenderer()->setTextColor(tgui::Color::Black);
		backToMenuButton->setSize(round(winSizeX / 4.8f), round(halfWinSizeY / 5.25f));
		backToMenuButton->setOrigin(0.5f, 0.5f);
		if (gv->getGameLanguage() == GameLanguage::English) { backToMenuButton->setText(L"BACK TO MENU"); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { backToMenuButton->setText(L"НАЗАД В МЕНЮ"); }
		backToMenuButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
		backToMenuButton->setPosition(halfWinSizeX, halfWinSizeY - round(winSizeX / 18.f));
		backToMenuButton->onPress([&gv] { pageAction = PageAction::OpenMainMenu; gameState = GameState::MainMenu; });
		gw->pageGUI.add(backToMenuButton, "backToMenuButton");
	}

}

void profileUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	float columnWidth = (0.6f * gw->window.getSize().x) / 4.f;
	float itemAndHeaderHeight = (0.6f * gw->window.getSize().y) / 11.f;
	unsigned int userID = database->getUserID(gv, gv->getNickname());
	database->selectConfigTable(gv, userID);
	gw->pageGUI.removeAllWidgets();

	tgui::Label::Ptr profileLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { profileLabel->setText(L"PROFILE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { profileLabel->setText(L"ПРОФИЛЬ"); }
	profileLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	profileLabel->setOrigin(0.5f, 0.5f);
	profileLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	profileLabel->setPosition("50%", "10%");
	gw->pageGUI.add(profileLabel, "profileLabel");

	tgui::Label::Ptr userNickLabel = tgui::Label::create();
	userNickLabel->setText(gv->getNickname());
	userNickLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 30.f)));
	userNickLabel->setOrigin(0.5f, 0.5f);
	userNickLabel->getRenderer()->setTextColor(tgui::Color::White);
	userNickLabel->setPosition("50%", "20%");
	gw->pageGUI.add(userNickLabel, "userNickLabel");

	std::string avatarPath = "Images\\BigAvatars\\" + std::to_string(gv->getAvatarNumber()) + ".png";
	tgui::Texture avatarTexture(avatarPath);
	tgui::Picture::Ptr avatarPicture = tgui::Picture::create(avatarTexture);
	avatarPicture->setSize("20%", "30%");
	avatarPicture->setOrigin(0.5f, 0.5f);
	avatarPicture->setPosition("50%", "40%");
	gw->pageGUI.add(avatarPicture, "avatarPicture");

	tgui::Label::Ptr personalHSLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { personalHSLabel->setText(L"PERSONAL HIGH SCORES"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { personalHSLabel->setText(L"ЛИЧНЫЕ РЕКОРДЫ"); }
	personalHSLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 40.f)));
	personalHSLabel->setOrigin(0.5f, 0.5f);
	personalHSLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	personalHSLabel->setPosition("50%", "65%");
	gw->pageGUI.add(personalHSLabel, "personalHSLabel");

	tgui::ListView::Ptr personalHSListView = tgui::ListView::create();
	if (gv->getGameLanguage() == GameLanguage::English)
	{
		personalHSListView->addColumn(L"30 SECONDS", columnWidth, tgui::ListView::ColumnAlignment::Center);
		personalHSListView->addColumn(L"1 MINUTE", columnWidth, tgui::ListView::ColumnAlignment::Center);
		personalHSListView->addColumn(L"2 MINUTES", columnWidth, tgui::ListView::ColumnAlignment::Center);
		personalHSListView->addColumn(L"ONE MISTAKE", columnWidth, tgui::ListView::ColumnAlignment::Center);
	}
	else if (gv->getGameLanguage() == GameLanguage::Russian)
	{
		personalHSListView->addColumn(L"30 СЕКУНД", columnWidth, tgui::ListView::ColumnAlignment::Center);
		personalHSListView->addColumn(L"1 МИНУТА", columnWidth, tgui::ListView::ColumnAlignment::Center);
		personalHSListView->addColumn(L"2 МИНУТЫ", columnWidth, tgui::ListView::ColumnAlignment::Center);
		personalHSListView->addColumn(L"ОДНА ОШИБКА", columnWidth, tgui::ListView::ColumnAlignment::Center);
	}


	personalHSListView->addItem({ std::to_string(gv->personalHSVec[0]), std::to_string(gv->personalHSVec[1]),
								 std::to_string(gv->personalHSVec[2]), std::to_string(gv->personalHSVec[3]) });

	personalHSListView->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	personalHSListView->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	personalHSListView->getRenderer()->setHeaderBackgroundColor(tgui::Color::Green);
	personalHSListView->getRenderer()->setBorderColor(tgui::Color::Black);
	personalHSListView->getRenderer()->setTextColor(tgui::Color::Black);
	personalHSListView->getRenderer()->setSeparatorColor(tgui::Color::Black);
	personalHSListView->setSeparatorWidth(1);
	personalHSListView->setHeaderSeparatorHeight(1);
	personalHSListView->setGridLinesWidth(1);
	personalHSListView->setShowVerticalGridLines(true);
	personalHSListView->setShowHorizontalGridLines(true);
	personalHSListView->setSize("60%", "11%");
	personalHSListView->setHeaderHeight(itemAndHeaderHeight);
	personalHSListView->setItemHeight(static_cast<unsigned int>(itemAndHeaderHeight));
	personalHSListView->setTextSize(static_cast<unsigned int>(round(winSizeX / 45.f)));
	personalHSListView->setOrigin(0.5f, 0.5f);
	personalHSListView->setPosition("50%", "75%");
	gw->pageGUI.add(personalHSListView, "personalHSListView");

	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize(round(winSizeX / 9.5f), round(winSizeX / 19.f));
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"BACK"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"НАЗАД"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	backButton->setPosition("50%", "90%");
	backButton->onPress([] { pageAction = PageAction::OpenMainMenu; });
	gw->pageGUI.add(backButton, "backButton");

}

void gameModeSelectionUpdate(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	float winSizeX = static_cast<float>(gw->window.getSize().x);
	float halfWinSizeX = gw->window.getSize().x / 2.f;
	float halfWinSizeY = gw->window.getSize().y / 2.f;
	gw->pageGUI.removeAllWidgets();

	tgui::Label::Ptr gameModeSelectionLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { gameModeSelectionLabel->setText(L"SELECT GAME MODE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { gameModeSelectionLabel->setText(L"ВЫБЕРИТЕ РЕЖИМ ИГРЫ"); }
	gameModeSelectionLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	gameModeSelectionLabel->setOrigin(0.5f, 0.5f);
	gameModeSelectionLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	gameModeSelectionLabel->setPosition("50%", "10%");
	gw->pageGUI.add(gameModeSelectionLabel, "gameModeSelectionLabel");

	tgui::Button::Ptr thirtySecondsMode = tgui::Button::create();
	thirtySecondsMode->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	thirtySecondsMode->getRenderer()->setTextColor(tgui::Color::Black);
	thirtySecondsMode->setSize("25%", "10%");
	thirtySecondsMode->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { thirtySecondsMode->setText(L"30 SECONDS"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { thirtySecondsMode->setText(L"30 СЕКУНД"); }
	thirtySecondsMode->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	thirtySecondsMode->setPosition("50%", "30%");
	thirtySecondsMode->onPress([&gv] { pageAction = PageAction::StartGame; gv->setGameMode(GameMode::ThirtySeconds); });
	gw->pageGUI.add(thirtySecondsMode, "thirtySecondsMode");

	tgui::Button::Ptr oneMinuteMode = tgui::Button::create();
	oneMinuteMode->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	oneMinuteMode->getRenderer()->setTextColor(tgui::Color::Black);
	oneMinuteMode->setSize("25%", "10%");
	oneMinuteMode->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { oneMinuteMode->setText(L"1 MINUTE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { oneMinuteMode->setText(L"1 МИНУТА"); }
	oneMinuteMode->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	oneMinuteMode->setPosition("50%", "41%");
	oneMinuteMode->onPress([&gv] { pageAction = PageAction::StartGame; gv->setGameMode(GameMode::OneMinute); });
	gw->pageGUI.add(oneMinuteMode, "oneMinuteMode");

	tgui::Button::Ptr twoMinutesMode = tgui::Button::create();
	twoMinutesMode->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	twoMinutesMode->getRenderer()->setTextColor(tgui::Color::Black);
	twoMinutesMode->setSize("25%", "10%");
	twoMinutesMode->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { twoMinutesMode->setText(L"2 MINUTES"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { twoMinutesMode->setText(L"2 МИНУТЫ"); }
	twoMinutesMode->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	twoMinutesMode->setPosition("50%", "52%");
	twoMinutesMode->onPress([&gv] { pageAction = PageAction::StartGame; gv->setGameMode(GameMode::TwoMinutes); });
	gw->pageGUI.add(twoMinutesMode, "twoMinutesMode");

	tgui::Button::Ptr mistakeMode = tgui::Button::create();
	mistakeMode->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	mistakeMode->getRenderer()->setTextColor(tgui::Color::Black);
	mistakeMode->setSize("25%", "10%");
	mistakeMode->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { mistakeMode->setText(L"ONE MISTAKE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { mistakeMode->setText(L"ОДНА ОШИБКА"); }
	mistakeMode->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	mistakeMode->setPosition("50%", "63%");
	mistakeMode->onPress([&gv] { pageAction = PageAction::StartGame; gv->setGameMode(GameMode::OneMistake); });
	gw->pageGUI.add(mistakeMode, "mistakeMode");

	tgui::Button::Ptr freeMode = tgui::Button::create();
	freeMode->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	freeMode->getRenderer()->setTextColor(tgui::Color::Black);
	freeMode->setSize("25%", "10%");
	freeMode->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { freeMode->setText(L"FREE"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { freeMode->setText(L"СВОБОДНЫЙ"); }
	freeMode->setTextSize(static_cast<unsigned int>(round(winSizeX / 48.f)));
	freeMode->setPosition("50%", "74%");
	freeMode->onPress([&gv] { pageAction = PageAction::StartGame; gv->setGameMode(GameMode::Free); });
	gw->pageGUI.add(freeMode, "freeMode");

	tgui::Button::Ptr backButton = tgui::Button::create();
	backButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	backButton->getRenderer()->setTextColor(tgui::Color::Black);
	backButton->setSize("14%", "7%");
	backButton->setOrigin(0.5f, 0.5f);
	if (gv->getGameLanguage() == GameLanguage::English) { backButton->setText(L"Back"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { backButton->setText(L"Назад"); }
	backButton->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	backButton->setPosition("50%", "90%");
	backButton->onPress([] { pageAction = PageAction::OpenMainMenu; });
	gw->pageGUI.add(backButton, "backButton");

}





void createGameInfo(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	unsigned int winSizeX = gw->window.getSize().x;
	gw->gameInfoGUI.removeAllWidgets();

	tgui::Label::Ptr enterWordLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { enterWordLabel->setText(L"ENTER A WORD"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { enterWordLabel->setText(L"ВВЕДИТЕ СЛОВО"); }
	enterWordLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 25.f)));
	enterWordLabel->setOrigin(0.5f, 0.5f);
	enterWordLabel->getRenderer()->setTextColor(tgui::Color::Magenta);
	enterWordLabel->setPosition("50%", "10%");
	gw->gameInfoGUI.add(enterWordLabel, "enterWordLabel");


	tgui::Label::Ptr correctlyLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { correctlyLabel->setText(L"CORRECTLY!"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { correctlyLabel->setText(L"ПРАВИЛЬНО!"); }
	correctlyLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 40.f)));
	correctlyLabel->setOrigin(0.5f, 0.5f);
	correctlyLabel->getRenderer()->setTextColor(tgui::Color::Green);
	correctlyLabel->setPosition("50%", "30%");
	correctlyLabel->setVisible(false);
	gw->gameInfoGUI.add(correctlyLabel, "correctlyLabel");


	tgui::Label::Ptr wrongLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { wrongLabel->setText(L"WRONG!"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { wrongLabel->setText(L"НЕПРАВИЛЬНО!"); }
	wrongLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 40.f)));
	wrongLabel->setOrigin(0.5f, 0.5f);
	wrongLabel->getRenderer()->setTextColor(tgui::Color::Red);
	wrongLabel->setPosition("50%", "30%");
	wrongLabel->setVisible(false);
	gw->gameInfoGUI.add(wrongLabel, "wrongLabel");


	tgui::Label::Ptr gameModeLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English)
	{
		std::wstring tempGameMode = L"";

		if (gv->getGameMode() == GameMode::ThirtySeconds) { tempGameMode = L"30 Seconds"; }
		else if (gv->getGameMode() == GameMode::OneMinute) { tempGameMode = L"1 Minute"; }
		else if (gv->getGameMode() == GameMode::TwoMinutes) { tempGameMode = L"2 Minutes"; }
		else if (gv->getGameMode() == GameMode::OneMistake) { tempGameMode = L"One Mistake"; }
		else if (gv->getGameMode() == GameMode::Free) { tempGameMode = L"Free"; }

		gameModeLabel->setText(L"Game mode: " + tempGameMode);
	}
	else if (gv->getGameLanguage() == GameLanguage::Russian)
	{
		std::wstring tempGameMode = L"";

		if (gv->getGameMode() == GameMode::ThirtySeconds) { tempGameMode = L"30 Секунд"; }
		else if (gv->getGameMode() == GameMode::OneMinute) { tempGameMode = L"1 Минута"; }
		else if (gv->getGameMode() == GameMode::TwoMinutes) { tempGameMode = L"2 Минуты"; }
		else if (gv->getGameMode() == GameMode::OneMistake) { tempGameMode = L"Одна ошибка"; }
		else if (gv->getGameMode() == GameMode::Free) { tempGameMode = L"Свободный"; }

		gameModeLabel->setText(L"Режим игры: " + tempGameMode);
	}
	gameModeLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	gameModeLabel->setOrigin(0.5f, 0.5f);
	gameModeLabel->getRenderer()->setTextColor(tgui::Color::Green);
	gameModeLabel->setPosition("85%", "40%");
	gw->gameInfoGUI.add(gameModeLabel, "gameModeLabel");


	if (gv->getGameMode() != GameMode::Free)
	{
		tgui::Label::Ptr scoreLabel = tgui::Label::create();
		if (gv->getGameLanguage() == GameLanguage::English) { scoreLabel->setText(L"Score: " + std::to_wstring(gv->getScore())); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { scoreLabel->setText(L"Счёт: " + std::to_wstring(gv->getScore())); }
		scoreLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
		scoreLabel->setOrigin(0.5f, 0.5f);
		scoreLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
		scoreLabel->setPosition("85%", "55%");
		gw->gameInfoGUI.add(scoreLabel, "scoreLabel");
	}

	if (gv->getGameMode() != GameMode::Free)
	{
		tgui::Label::Ptr enteredWordsLabel = tgui::Label::create();
		if (gv->getGameLanguage() == GameLanguage::English) { enteredWordsLabel->setText(L"Entered words: " + std::to_wstring(gv->getNumOfEnteredWords())); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { enteredWordsLabel->setText(L"Введённые слова: " + std::to_wstring(gv->getNumOfEnteredWords())); }
		enteredWordsLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
		enteredWordsLabel->setOrigin(0.5f, 0.5f);
		enteredWordsLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
		enteredWordsLabel->setPosition("85%", "60%");
		gw->gameInfoGUI.add(enteredWordsLabel, "enteredWordsLabel");
	}


	if (gv->getGameMode() != GameMode::OneMistake && gv->getGameMode() != GameMode::Free)
	{
		tgui::Label::Ptr mistakesLabel = tgui::Label::create();
		if (gv->getGameLanguage() == GameLanguage::English) { mistakesLabel->setText(L"Mistakes: " + std::to_wstring(gv->getNumOfMistakes())); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { mistakesLabel->setText(L"Ошибки: " + std::to_wstring(gv->getNumOfMistakes())); }
		mistakesLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
		mistakesLabel->setOrigin(0.5f, 0.5f);
		mistakesLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
		mistakesLabel->setPosition("85%", "65%");
		gw->gameInfoGUI.add(mistakesLabel, "mistakesLabel");
	}


	if (gv->getGameMode() != GameMode::OneMistake && gv->getGameMode() != GameMode::Free)
	{
		tgui::Label::Ptr timeLabel = tgui::Label::create();
		if (gv->getGameLanguage() == GameLanguage::English) { timeLabel->setText(L"Time: " + std::to_wstring(gv->getGameTime())); }
		else if (gv->getGameLanguage() == GameLanguage::Russian) { timeLabel->setText(L"Время: " + std::to_wstring(gv->getGameTime())); }
		timeLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
		timeLabel->setOrigin(0.5f, 0.5f);
		timeLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
		timeLabel->setPosition("85%", "70%");
		gw->gameInfoGUI.add(timeLabel, "timeLabel");
	}

	tgui::Label::Ptr nicknameLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { nicknameLabel->setText(L"Nickname: " + gv->getNickname()); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { nicknameLabel->setText(L"Никнейм: " + gv->getNickname()); }
	nicknameLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 50.f)));
	nicknameLabel->setOrigin(0.5f, 0.5f);
	nicknameLabel->getRenderer()->setTextColor(tgui::Color::White);
	nicknameLabel->setPosition("85%", "90%");
	gw->gameInfoGUI.add(nicknameLabel, "nicknameLabel");

}

void createVirtualKeyboard(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	unsigned int winSizeX = gw->window.getSize().x;
	gw->virtualKeyboardGUI.removeAllWidgets();
	gv->virtualKeyboardVec.clear();

	size_t delimiter = 0;
	std::string posX = "";
	std::string posY = "";

	for (size_t symbolNum = 0; symbolNum < gv->symbols.size(); ++symbolNum)
	{
		posY = "65%";
		if (symbolNum >= 10 && symbolNum < 19) { posY = "74%"; }
		else if (symbolNum >= 19) { posY = "83%"; }
		if (symbolNum == 10 || symbolNum == 19) { delimiter = 0; }
		posX = std::to_string(20 + delimiter) + "%";

		gv->virtualKeyboardVec.emplace_back(tgui::Button::create());
		gv->virtualKeyboardVec[symbolNum]->getRenderer()->setBackgroundColorHover(tgui::Color::White);
		gv->virtualKeyboardVec[symbolNum]->getRenderer()->setTextColor(tgui::Color::Black);
		gv->virtualKeyboardVec[symbolNum]->setSize("5%", "8.89%");
		gv->virtualKeyboardVec[symbolNum]->setTextSize(winSizeX / 42);
		gv->virtualKeyboardVec[symbolNum]->setText(gv->symbols.substr(symbolNum, 1));
		gv->virtualKeyboardVec[symbolNum]->setOrigin(0.5f, 0.5f);
		gv->virtualKeyboardVec[symbolNum]->setPosition(posX.c_str(), posY.c_str());
		gw->virtualKeyboardGUI.add(gv->virtualKeyboardVec[symbolNum], gv->symbols[symbolNum]);
		delimiter += 5;
	}
}

void createWord(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	unsigned int winSizeX = gw->window.getSize().x;
	gw->wordsGUI.removeAllWidgets();
	gv->wordsVec.clear();

	size_t delimiter = 0;
	std::string posX = "50%";
	std::string posY = "20%";

	for (size_t symbolNum = 0; symbolNum < gv->dictionaryVector[gv->getNumOfEnteredWords()].size(); ++symbolNum)
	{
		if (gv->dictionaryVector[gv->getNumOfEnteredWords()].size() % 2 == 0)
		{
			posX = std::to_string((47 - ((gv->dictionaryVector[gv->getNumOfEnteredWords()].size() / 2) * 5)) + 5 + delimiter) + "%";
		}
		else { posX = std::to_string((50 - (((gv->dictionaryVector[gv->getNumOfEnteredWords()].size() / 2) + 1) * 5)) + 5 + delimiter) + "%"; }


		gv->wordsVec.emplace_back(tgui::Button::create());
		gv->wordsVec[symbolNum]->getRenderer()->setBackgroundColorHover(tgui::Color::White);
		gv->wordsVec[symbolNum]->getRenderer()->setTextColor(tgui::Color::Black);

		if (symbolNum < gv->getSymbolNumber())
		{
			gv->wordsVec[symbolNum]->getRenderer()->setBackgroundColor(tgui::Color::Green);
		}
		else if (gameEvent != GameEvent::NewWord && symbolNum == gv->getSymbolNumber() && gv->getPressedKey() != gv->wordsVec[symbolNum]->getText())
		{
			gv->wordsVec[symbolNum]->getRenderer()->setBackgroundColor(tgui::Color::Red);
		}

		gv->wordsVec[symbolNum]->setSize("5%", "8.89%");
		gv->wordsVec[symbolNum]->setTextSize(winSizeX / 42);
		gv->wordsVec[symbolNum]->setText(gv->dictionaryVector[gv->getNumOfEnteredWords()].substr(symbolNum, 1));
		gv->wordsVec[symbolNum]->setOrigin(0.5f, 0.5f);
		gv->wordsVec[symbolNum]->setPosition(posX.c_str(), posY.c_str());
		gw->wordsGUI.add(gv->wordsVec[symbolNum], gv->wordsVec[symbolNum]->getText());

		delimiter += 5;
	}

}

void createGameResult(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw)
{
	unsigned int winSizeX = gw->window.getSize().x;
	gw->gameInfoGUI.removeAllWidgets();

	tgui::Label::Ptr gameResultLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { gameResultLabel->setText(L"GAME RESULT"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { gameResultLabel->setText(L"РЕЗУЛЬТАТ ИГРЫ"); }
	gameResultLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 20.f)));
	gameResultLabel->setOrigin(0.5f, 0.5f);
	gameResultLabel->getRenderer()->setTextColor(tgui::Color::Magenta);
	gameResultLabel->setPosition("50%", "10%");
	gameResultLabel->setVisible(true);
	gw->gameInfoGUI.add(gameResultLabel, "gameResultLabel");

	tgui::Label::Ptr gameInfoLabel = tgui::Label::create();

	if (gv->getGameMode() == GameMode::OneMistake)
	{
		if (gv->getGameLanguage() == GameLanguage::English)
		{
			gameInfoLabel->setText(
				L"SCORE: " + std::to_wstring(gv->getScore()) +
				L"\nENTERED WORDS: " + std::to_wstring(gv->getNumOfEnteredWords()));
		}
		else if (gv->getGameLanguage() == GameLanguage::Russian)
		{
			gameInfoLabel->setText(
				L"СЧЁТ: " + std::to_wstring(gv->getScore()) +
				L"\nВВЕДЁННЫЕ СЛОВА: " + std::to_wstring(gv->getNumOfEnteredWords()));
		}
	}
	else
	{
		if (gv->getGameLanguage() == GameLanguage::English)
		{
			gameInfoLabel->setText(
				L"SCORE: " + std::to_wstring(gv->getScore()) +
				L"\nENTERED WORDS: " + std::to_wstring(gv->getNumOfEnteredWords()) +
				L"\nMISTAKES: " + std::to_wstring(gv->getNumOfMistakes()));
		}
		else if (gv->getGameLanguage() == GameLanguage::Russian)
		{
			gameInfoLabel->setText(
				L"СЧЁТ: " + std::to_wstring(gv->getScore()) +
				L"\nВВЕДЁННЫЕ СЛОВА: " + std::to_wstring(gv->getNumOfEnteredWords()) +
				L"\nОШИБКИ: " + std::to_wstring(gv->getNumOfMistakes()));
		}
	}


	gameInfoLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 30.f)));
	gameInfoLabel->setOrigin(0.5f, 0.5f);
	gameInfoLabel->getRenderer()->setTextColor(tgui::Color::Yellow);
	gameInfoLabel->setPosition("50%", "50%");
	gameInfoLabel->setVisible(true);
	gw->gameInfoGUI.add(gameInfoLabel, "gameInfoLabel");

	tgui::Label::Ptr pressEscapeLabel = tgui::Label::create();
	if (gv->getGameLanguage() == GameLanguage::English) { pressEscapeLabel->setText(L"PRESS ESCAPE TO RETURN TO THE MENU"); }
	else if (gv->getGameLanguage() == GameLanguage::Russian) { pressEscapeLabel->setText(L"НАЖМИТЕ ESCAPE, ЧТОБЫ ВЕРНУТЬСЯ В МЕНЮ"); }
	pressEscapeLabel->setTextSize(static_cast<unsigned int>(round(winSizeX / 30.f)));
	pressEscapeLabel->setOrigin(0.5f, 0.5f);
	pressEscapeLabel->getRenderer()->setTextColor(tgui::Color::Cyan);
	pressEscapeLabel->setPosition("50%", "90%");
	pressEscapeLabel->setVisible(true);
	gw->gameInfoGUI.add(pressEscapeLabel, "pressEscapeLabel");
}