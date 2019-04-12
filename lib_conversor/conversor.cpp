#include "conversor.h"

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