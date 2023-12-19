#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

const std::string GPIO_PATH = "/sys/class/gpio/";

void exportGPIO(int pinNumber) {
    std::ofstream exportFile(GPIO_PATH + "export");
    exportFile << pinNumber;
    exportFile.close();
}

void setDirection(int pinNumber, const std::string& direction) {
    std::ofstream directionFile(GPIO_PATH + "gpio" + std::to_string(pinNumber) + "/direction");
    directionFile << direction;
    directionFile.close();
}

int readGPIO(int pinNumber) {
    std::ifstream valueFile(GPIO_PATH + "gpio" + std::to_string(pinNumber) + "/value");
    int value = -1;
    valueFile >> value;
    valueFile.close();
    return value;
}

int main() {
    const int buttonPin = 11; 
    const int ledPins[3] = {2, 3, 4};

    exportGPIO(buttonPin);
    exportGPIO(ledPins[0]);
    exportGPIO(ledPins[1]);
    exportGPIO(ledPins[2]);

    setDirection(buttonPin, "in");
    setDirection(ledPins[0], "out");
    setDirection(ledPins[1], "out");
    setDirection(ledPins[2], "out");

    int ledIndex = 0;

    while (true) {
        int buttonValue = readGPIO(buttonPin);

        if (buttonValue == 1) {
            if(readGPIO(ledPins[ledIndex])!= 1){
                
                std::ofstream ledFile(GPIO_PATH + "gpio" + std::to_string(ledPins[ledIndex]) + "/value");
                ledFile << "1";
                ledFile.close();
            
            }
        } else {
            if(readGPIO(ledPins[ledIndex])!=0){
            std::ofstream ledFile(GPIO_PATH + "gpio" + std::to_string(ledPins[ledIndex]) + "/value");
            ledFile << "0";
            ledFile.close();
                if(ledIndex == 2 ) {
                    ledIndex = 0;
                }
                else{
                    ledIndex++;
                }
            }
        }
usleep(100000);
    }

    return 0;
}