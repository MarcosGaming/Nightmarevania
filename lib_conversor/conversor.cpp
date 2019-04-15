#include "conversor.h"
#include <unordered_map>

// Converts key to string
std::string Conversor::KeyboardKeyToString(sf::Keyboard::Key key)
{
	std::string ret;
	#define CONVERT(x) case sf::Keyboard:: ## x : ret = #x; break;
	switch (key)
	{
		CONVERT(Unknown);
		CONVERT(A);
		CONVERT(B);
		CONVERT(C);
		CONVERT(D);
		CONVERT(E);
		CONVERT(F);
		CONVERT(G);
		CONVERT(H);
		CONVERT(I);
		CONVERT(J);
		CONVERT(K);
		CONVERT(L);
		CONVERT(M);
		CONVERT(N);
		CONVERT(O);
		CONVERT(P);
		CONVERT(Q);
		CONVERT(R);
		CONVERT(S);
		CONVERT(T);
		CONVERT(U);
		CONVERT(V);
		CONVERT(W);
		CONVERT(X);
		CONVERT(Y);
		CONVERT(Z);
		CONVERT(Num0);
		CONVERT(Num1);
		CONVERT(Num2);
		CONVERT(Num3);
		CONVERT(Num4);
		CONVERT(Num5);
		CONVERT(Num6);
		CONVERT(Num7);
		CONVERT(Num8);
		CONVERT(Num9);
		CONVERT(Escape);
		CONVERT(LControl);
		CONVERT(LShift);
		CONVERT(LAlt);
		CONVERT(LSystem);
		CONVERT(RControl);
		CONVERT(RShift);
		CONVERT(RAlt);
		CONVERT(RSystem);
		CONVERT(Menu);
		CONVERT(LBracket);
		CONVERT(RBracket);
		CONVERT(SemiColon);
		CONVERT(Comma);
		CONVERT(Period);
		CONVERT(Quote);
		CONVERT(Slash);
		CONVERT(BackSlash);
		CONVERT(Tilde);
		CONVERT(Equal);
		CONVERT(Dash);
		CONVERT(Space);
		CONVERT(Return);
		CONVERT(BackSpace);
		CONVERT(Tab);
		CONVERT(PageUp);
		CONVERT(PageDown);
		CONVERT(End);
		CONVERT(Home);
		CONVERT(Insert);
		CONVERT(Delete);
		CONVERT(Add);
		CONVERT(Subtract);
		CONVERT(Multiply);
		CONVERT(Divide);
		CONVERT(Left);
		CONVERT(Right);
		CONVERT(Up);
		CONVERT(Down);
		CONVERT(Numpad0);
		CONVERT(Numpad1);
		CONVERT(Numpad2);
		CONVERT(Numpad3);
		CONVERT(Numpad4);
		CONVERT(Numpad5);
		CONVERT(Numpad6);
		CONVERT(Numpad7);
		CONVERT(Numpad8);
		CONVERT(Numpad9);
		CONVERT(F1);
		CONVERT(F2);
		CONVERT(F3);
		CONVERT(F4);
		CONVERT(F5);
		CONVERT(F6);
		CONVERT(F7);
		CONVERT(F8);
		CONVERT(F9);
		CONVERT(F10);
		CONVERT(F11);
		CONVERT(F12);
		CONVERT(F13);
		CONVERT(F14);
		CONVERT(F15);
		CONVERT(Pause);
	}
	return ret;
	#undef CONVERT
}

// Converts mouse button to string
std::string Conversor::MouseButtonToString(sf::Mouse::Button button)
{
	std::string ret;
	#define CONVERT(x) case sf::Mouse:: ## x : ret = #x; break;
	switch (button)
	{
	case sf::Mouse::Left:
		ret = "L.Mouse";
		break;
	case sf::Mouse::Right:
		ret = "R.Mouse";
		break;
		CONVERT(Middle);
		CONVERT(XButton1);
		CONVERT(XButton2);
	}
	return ret;
	#undef CONVERT
}

// This mapping only works for XBOX controllers and is only for the buttons of the controller not the axis
std::string Conversor::ControllerButtonToString(int button)
{
	std::string ret;
	switch (button)
	{
	case 0 :
		ret = "A";
		break;
	case 1:
		ret = "B";
		break;
	case 2:
		ret = "X";
		break;
	case 3:
		ret = "Y";
		break;
	case 4:
		ret = "LB";
		break;
	case 5:
		ret = "RB";
		break;
	case 6:
		ret = "Back";
		break;
	case 7:
		ret = "Start";
		break;
	case 8:
		ret = "LS";
		break;
	case 9:
		ret = "RS";
		break;
	}
	return ret;
}

// Converts string to keyboard key
sf::Keyboard::Key Conversor::StringToKeyboardKey(std::string& string)
{
	if (string == "A")
	{
		return sf::Keyboard::A;
	}
	else if (string == "B")
	{
		return sf::Keyboard::B;
	}
	else if (string == "C")
	{
		return sf::Keyboard::C;
	}
	else if (string == "D")
	{
		return sf::Keyboard::D;
	}
	else if (string == "E")
	{
		return sf::Keyboard::E;
	}
	else if (string == "F")
	{
		return sf::Keyboard::F;
	}
	else if (string == "G")
	{
		return sf::Keyboard::G;
	}
	else if (string == "H")
	{
		return sf::Keyboard::H;
	}
	else if (string == "I")
	{
		return sf::Keyboard::I;
	}
	else if (string == "J")
	{
		return sf::Keyboard::J;
	}
	else if (string == "K")
	{
		return sf::Keyboard::K;
	}
	else if (string == "L")
	{
		return sf::Keyboard::L;
	}
	else if (string == "M")
	{
		return sf::Keyboard::M;
	}
	else if (string == "N")
	{
		return sf::Keyboard::N;
	}
	else if (string == "O")
	{
		return sf::Keyboard::O;
	}
	else if (string == "P")
	{
		return sf::Keyboard::P;
	}
	else if (string == "Q")
	{
		return sf::Keyboard::Q;
	}
	else if (string == "R")
	{
		return sf::Keyboard::R;
	}
	else if (string == "S")
	{
		return sf::Keyboard::S;
	}
	else if (string == "T")
	{
		return sf::Keyboard::T;
	}
	else if (string == "U")
	{
		return sf::Keyboard::U;
	}
	else if (string == "V")
	{
		return sf::Keyboard::V;
	}
	else if (string == "W")
	{
		return sf::Keyboard::W;
	}
	else if (string == "X")
	{
		return sf::Keyboard::X;
	}
	else if (string == "Y")
	{
		return sf::Keyboard::Y;
	}
	else if (string == "Z")
	{
		return sf::Keyboard::Z;
	}
	else if (string == "Num0")
	{
		return sf::Keyboard::Num0;
	}
	else if (string == "Num1")
	{
		return sf::Keyboard::Num1;
	}
	else if (string == "Num2")
	{
		return sf::Keyboard::Num2;
	}
	else if (string == "Num3")
	{
		return sf::Keyboard::Num3;
	}
	else if (string == "Num4")
	{
		return sf::Keyboard::Num4;
	}
	else if (string == "Num5")
	{
		return sf::Keyboard::Num5;
	}
	else if (string == "Num6")
	{
		return sf::Keyboard::Num6;
	}
	else if (string == "Num7")
	{
		return sf::Keyboard::Num7;
	}
	else if (string == "Num8")
	{
		return sf::Keyboard::Num8;
	}
	else if (string == "Num9")
	{
		return sf::Keyboard::Num9;
	}
	else if (string == "Escape")
	{
		return sf::Keyboard::Escape;
	}
	else if (string == "LControl")
	{
		return sf::Keyboard::LControl;
	}
	else if (string == "LShift")
	{
		return sf::Keyboard::LShift;
	}
	else if (string == "LAlt")
	{
		return sf::Keyboard::LAlt;
	}
	else if (string == "LSystem")
	{
		return sf::Keyboard::LSystem;
	}
	else if (string == "RControl")
	{
		return sf::Keyboard::RControl;
	}
	else if (string == "RShift")
	{
		return sf::Keyboard::RShift;
	}
	else if (string == "RAlt")
	{
		return sf::Keyboard::RAlt;
	}
	else if (string == "RSystem")
	{
		return sf::Keyboard::RSystem;
	}
	else if (string == "Menu")
	{
		return sf::Keyboard::Menu;
	}
	else if (string == "LBracket")
	{
		return sf::Keyboard::LBracket;
	}
	else if (string == "RBracket")
	{
		return sf::Keyboard::RBracket;
	}
	else if (string == "SemiColon")
	{
		return sf::Keyboard::SemiColon;
	}
	else if (string == "Comma")
	{
		return sf::Keyboard::Comma;
	}
	else if (string == "Period")
	{
		return sf::Keyboard::Period;
	}
	else if (string == "Quote")
	{
		return sf::Keyboard::Quote;
	}
	else if (string == "Slash")
	{
		return sf::Keyboard::Slash;
	}
	else if (string == "BackSlash")
	{
		return sf::Keyboard::BackSlash;
	}
	else if (string == "Tilde")
	{
		return sf::Keyboard::Tilde;
	}
	else if (string == "Equal")
	{
		return sf::Keyboard::Equal;
	}
	else if (string == "Dash")
	{
		return sf::Keyboard::Dash;
	}
	else if (string == "Space")
	{
		return sf::Keyboard::Space;
	}
	else if (string == "Return")
	{
		return sf::Keyboard::Return;
	}
	else if (string == "BackSpace")
	{
		return sf::Keyboard::BackSpace;
	}
	else if (string == "Tab")
	{
		return sf::Keyboard::Tab;
	}
	else if (string == "PageDown")
	{
		return sf::Keyboard::PageDown;
	}
	else if (string == "PageUp")
	{
		return sf::Keyboard::PageUp;
	}
	else if (string == "End")
	{
		return sf::Keyboard::End;
	}
	else if (string == "Home")
	{
		return sf::Keyboard::Home;
	}
	else if (string == "Insert")
	{
		return sf::Keyboard::Insert;
	}
	else if (string == "Delete")
	{
		return sf::Keyboard::Delete;
	}
	else if (string == "Add")
	{
		return sf::Keyboard::Add;
	}
	else if (string == "Subtract")
	{
		return sf::Keyboard::Subtract;
	}
	else if (string == "Multiply")
	{
		return sf::Keyboard::Multiply;
	}
	else if (string == "Divide")
	{
		return sf::Keyboard::Divide;
	}
	else if (string == "Left")
	{
		return sf::Keyboard::Left;
	}
	else if (string == "Right")
	{
		return sf::Keyboard::Right;
	}
	else if (string == "Up")
	{
		return sf::Keyboard::Up;
	}
	else if (string == "Down")
	{
		return sf::Keyboard::Down;
	}
	else if (string == "Numpad0")
	{
		return sf::Keyboard::Numpad0;
	}
	else if (string == "Numpad1")
	{
		return sf::Keyboard::Numpad1;
	}
	else if (string == "Numpad2")
	{
		return sf::Keyboard::Numpad2;
	}
	else if (string == "Numpad3")
	{
		return sf::Keyboard::Numpad3;
	}
	else if (string == "Numpad4")
	{
		return sf::Keyboard::Numpad4;
	}
	else if (string == "Numpad5")
	{
		return sf::Keyboard::Numpad5;
	}
	else if (string == "Numpad6")
	{
		return sf::Keyboard::Numpad6;
	}
	else if (string == "Numpad7")
	{
		return sf::Keyboard::Numpad7;
	}
	else if (string == "Numpad8")
	{
		return sf::Keyboard::Numpad8;
	}
	else if (string == "Numpad9")
	{
		return sf::Keyboard::Numpad9;
	}
	else if (string == "F1")
	{
		return sf::Keyboard::F1;
	}
	else if (string == "F2")
	{
		return sf::Keyboard::F2;
	}
	else if (string == "F3")
	{
		return sf::Keyboard::F3;
	}
	else if (string == "F4")
	{
		return sf::Keyboard::F4;
	}
	else if (string == "F5")
	{
		return sf::Keyboard::F5;
	}
	else if (string == "F6")
	{
		return sf::Keyboard::F6;
	}
	else if (string == "F6")
	{
		return sf::Keyboard::F7;
	}
	else if (string == "F8")
	{
		return sf::Keyboard::F8;
	}
	else if (string == "F9")
	{
		return sf::Keyboard::F9;
	}
	else if (string == "F10")
	{
		return sf::Keyboard::F10;
	}
	else if (string == "F11")
	{
		return sf::Keyboard::F11;
	}
	else if (string == "F12")
	{
		return sf::Keyboard::F12;
	}
	else if (string == "F13")
	{
		return sf::Keyboard::F13;
	}
	else if (string == "F14")
	{
		return sf::Keyboard::F14;
	}
	else if (string == "F15")
	{
		return sf::Keyboard::F15;
	}
	else if (string == "Pause")
	{
		return sf::Keyboard::Pause;
	}
	else
	{
		return sf::Keyboard::Unknown;
	}
}

// Converts string to mouse button
sf::Mouse::Button Conversor::StringToMouseButton(std::string& string)
{
	if (string == "L.Mouse")
	{
		return sf::Mouse::Left;
	}
	else if (string == "R.Mouse")
	{
		return sf::Mouse::Right;
	}
	else if (string == "Middle")
	{
		return sf::Mouse::Middle;
	}
	else if (string == "XButton1")
	{
		return sf::Mouse::XButton1;
	}
	else if (string == "XButton2")
	{
		return sf::Mouse::XButton2;
	}
	else
	{
		return sf::Mouse::Button::ButtonCount;
	}
}

// Converts string to controller button
int Conversor::StringToControllerButton(std::string& string)
{
	std::string ret;
	if (string == "A")
	{
		return 0;
	}
	else if (string == "B")
	{
		return 1;
	}
	else if (string == "X")
	{
		return 2;
	}
	else if (string == "Y")
	{
		return 3;
	}
	else if (string == "LB")
	{
		return 4;
	}
	else if (string == "RB")
	{
		return 5;
	}
	else if (string == "Back")
	{
		return 6;
	}
	else if (string == "Start")
	{
		return 7;
	}
	else if (string == "LS")
	{
		return 8;
	}
	else if (string == "RS")
	{
		return 9;
	}
	else
	{
		return 10;
	}
}