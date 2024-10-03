#include "API.h"

#include <cstdlib>
#include <iostream>


int API::mazeWidth() {
    return 0 ;
}

int API::mazeHeight() {
    return 0 ;
}

bool API::wallFront() {
    return wallFront ;
}

bool API::wallRight() {
    return wallRight ;
}

bool API::wallLeft() {
    return wallLeft ;
}

void API::moveForward(int distance) {
  moveForward() ;
}

void API::turnRight() {
  turnRight() ;
}

void API::turnLeft() {
  turnLeft();
}

void API::setWall(int x, int y, char direction) {
    std::cout << "setWall " << x << " " << y << " " << direction << std::endl;
}

void API::clearWall(int x, int y, char direction) {
    std::cout << "clearWall " << x << " " << y << " " << direction << std::endl;
}

void API::setColor(int x, int y, char color) {
    std::cout << "setColor " << x << " " << y << " " << color << std::endl;
}

void API::clearColor(int x, int y) {
    std::cout << "clearColor " << x << " " << y << std::endl;
}

void API::clearAllColor() {
    std::cout << "clearAllColor" << std::endl;
}

void API::setText(int x, int y, const std::string& text) {
    std::cout << "setText " << x << " " << y << " " << text << std::endl;
}

void API::clearText(int x, int y) {
  
}

void API::clearAllText() {
}

bool API::wasReset() {
  return true ;
}

void API::ackReset() {
}