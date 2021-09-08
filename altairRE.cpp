#include <bangtal>
using namespace bangtal;

int main() {
	// 1번 방 구성
	auto room1 = Scene::create("거실", "Images/배경-1.png");
	auto door1InRoom1 = Object::create("Images/문-오른쪽-닫힘.png", room1, 800, 270);
	auto door2InRoom1 = Object::create("Images/문-왼쪽-닫힘.png", room1, 300, 300);
	auto isDoor1Open = false;
	auto isDoor2Open = false;

	// 2번 방 구성
	auto room2 = Scene::create("화장실", "Images/배경-2.png");
	auto door1InRoom2 = Object::create("Images/문-왼쪽-열림.png", room2, 320, 270);

	// 1번 문에 맞는 키 생성
	auto key1 = Object::create("Images/열쇠.png", room1, 200, 250);
	key1->setScale(0.2f);
	key1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {
		object->pick();
		return true;
		});

	// 1번 문에 맞지 않는 키를 생성
	auto dummyKey = Object::create("Images/열쇠.png", room1, 500, 100);
	dummyKey->setScale(0.2f);
	dummyKey->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {
		object->pick();
		return true;
		});

	// 1번 문에 맞는 올바른 키는 화분에 가려 놓는다. 
	auto flowerPot = Object::create("Images/화분.png", room1, 150, 250);
	auto isPotMoved = false;
	flowerPot->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {
		if (isPotMoved == false && action == MouseAction::MOUSE_CLICK) {
			showMessage("반쯤 죽어있는 식물이다...");
		}
		if (isPotMoved == false && action == MouseAction::MOUSE_DRAG_LEFT) {
			flowerPot->locate(room1, 50, 250);
			isPotMoved = true;
		}
		else if (isPotMoved == false && action == MouseAction::MOUSE_DRAG_RIGHT) {
			flowerPot->locate(room1, 250, 250);
			isPotMoved = true;
		}
		return true;
		});

	// 1번 방의 1번 문 마우스 인터렉션
	door1InRoom1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {
		if (!isDoor1Open) {
			if (key1->isHanded()) {
				object->setImage("Images/문-오른쪽-열림.png");
				isDoor1Open = true;
				showMessage("열렸다!");
				key1->drop();
				key1->hide();
			}
			else if (dummyKey->isHanded()) {
				showMessage("열쇠가 구멍에 맞지 않는다. \n다른 열쇠를 찾아보자.");
				dummyKey->drop();
				dummyKey->hide();
			}
			else {
				showMessage("잠겨있다...");
			}
		}
		else {
			room2->enter();
		}
		return true;
	});

	// 2번 방의 1번 문 마우스 인터렉션
	door1InRoom2->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		room1->enter();
		return true;
	});

	// 화장실 오브젝트들 구성
	auto endKey = Object::create("Images/열쇠.png", room2, 850, 250);
	endKey->setScale(0.2f);
	auto toilet = Object::create("Images/변기.png", room2, 800, 200);
	toilet->setScale(0.04f);
	auto hammer = Object::create("Images/망치.png", room2, 500, 100);
	hammer->setScale(0.04f);
	hammer->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		object->pick();
		return true;
		});

	// 변기 인터렉션 구성
	toilet->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		if (hammer->isHanded()) {
			toilet->hide();
		}
		else {
			showMessage("세라믹 변기다. 물통 안에 뭐가 들었을까?");
		}
		return true;
		});

	// 변기 속의 키 인터렉션
	endKey->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		object->pick();
		return true;
		});

	// 1번 방의 2번 문 인터렉션
	door2InRoom1->setOnMouseCallback([&](auto object, auto x, auto y, auto action) -> bool {
		if (!isDoor2Open) {
			if (endKey->isHanded()) {
				object->setImage("Images/문-왼쪽-열림.png");
				isDoor2Open = true;
				showMessage("열렸다! \n밖이 보인다!");
			}
			else {
				showMessage("잠겨있다...");
			}
		}
		else {
			endGame();
		}
		return true;
		});

	startGame(room1);
}