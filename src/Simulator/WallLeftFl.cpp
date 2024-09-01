#include <iostream>
#include <string>
#include<utility>
#include<queue>

#include "API.h"


int main(int argc, char* argv[]) {
    while (true) {
        if (!API::wallLeft()) {
            API::turnLeft();
        }
        while (API::wallFront()) {
            API::turnRight();
        }
        API::moveForward();
    }
    return 0 ;
}