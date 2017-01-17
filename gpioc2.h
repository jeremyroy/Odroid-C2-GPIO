/*
 * Copyright 2016 Jeremy Roy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file gpioc2.h
 * @brief Description of GPIOC2 class.
 * @author Jeremy Roy <jeremy.roy@queensu.ca>
 * @copyright (c) Jeremy Roy under the Apache License, version 2.0.
 */

#include <fstream>
#include <set>
#include <map>
#include <sstream>

#define OUTPUT 0
#define INPUT 1

#define LOW 0
#define HIGH 1

enum pin_mode
{
    DIGITAL,
    I2C,
    PWM,
    UART,
    ADC
};

class GPIOC2_Exception : public std::exception
{
public:
    GPIOC2_Exception(std::string message) : m_message(message) {}
    virtual const char* what() const throw() {return m_message.c_str();}
private:
    std::string m_message;
};

class GPIOC2
{
public:
    GPIOC2(int pin_number, bool direction);
    ~GPIOC2();
    bool write_value(bool value);
    int read_value();
private:
    int m_pin_number;
    bool m_pin_direction;
    int m_export_number;

    static const std::set<int> m_voltage_pins;
    static const std::set<int> m_ground_pins;
    static const std::map<int, int> m_digital_pins;

    bool create_file_access();
    bool set_direction();
};

const std::set<int> GPIOC2::m_voltage_pins = {1, 2, 4, 17, 38};
const std::set<int> GPIOC2::m_ground_pins = {6, 9, 14, 20, 25, 30, 34, 39};
const std::map<int, int> GPIOC2::m_digital_pins = {{7, 249},  {11, 247}, {12, 238}, {13, 239}, 
                                                   {15, 237}, {16, 236}, {18, 233}, {19, 235}, 
                                                   {21, 232}, {22, 231}, {23, 230}, {24, 229},
                                                   {26, 225}, {29, 228}, {31, 219}, {32, 224}, 
                                                   {33, 234}, {35, 214}, {36, 218}};

GPIOC2::GPIOC2(int pin_number, bool direction)
: m_pin_number(pin_number),
  m_pin_direction(direction)
{
    /** Check validity of pin */
    if (pin_number < 1 || pin_number > 40)
    {
        std::stringstream message;
        message << "Invalid pin: " << pin_number << "\tPin of range.";
        throw GPIOC2_Exception(message.str());
    }

    /** Check that pin is of mode DIGITAL */
    auto it = m_digital_pins.find(pin_number);
    if (it == m_digital_pins.end()) 
    {
        std::stringstream message;
        message << "Mode DIGITAL not supported for specified pin: "
                << pin_number;
        throw GPIOC2_Exception(message.str());
    }
    m_export_number = it->second;
    
    /** Pin and mode valid.  Now we must create file access for the pin and set direction */
    if (create_file_access() == false)
    {
        std::stringstream message;
        message << "Unable to create GPIO file access for pin " 
                << pin_number;
        throw GPIOC2_Exception(message.str());
    }

}

GPIOC2::~GPIOC2()
{
    //empty
}

bool GPIOC2::write_value(bool value)
{
    if (m_pin_direction != OUTPUT)
    {
        std::stringstream message;
        message << "Cannot write to pin " 
                << m_pin_number 
                << ".  Pin direction set to INPUT.";
        throw GPIOC2_Exception(message.str());
    }
    std::stringstream value_filename;
    value_filename << "/sys/class/gpio/gpio" 
                   << m_export_number 
                   << "/value";
    std::ofstream value_file(value_filename.str(), std::ios::out & std::ios::trunc);
    if (!value_file.is_open())
    {
        return false;
    }
    value_file << value;
    value_file.close();
    return true;
}

int GPIOC2::read_value()
{
    char value_char;
    std::stringstream value_filename;
    value_filename << "/sys/class/gpio/gpio"
                   << m_export_number
                   << "/value";
    std::ifstream value_file(value_filename.str());
    if (!value_file.is_open())
    {
        return -1;
    }
    value_file.get(value_char);
    return value_char - '0';
}

bool GPIOC2::create_file_access()
{
    std::string export_filename = "/sys/class/gpio/export";
    std::ofstream export_file(export_filename, std::ios::out & std::ios::trunc);
    if (!export_file.is_open())
    {
        return false;
    }
    export_file << m_export_number;
    export_file.close();
    return true;
}

bool GPIOC2::set_direction()
{
    std::stringstream direction_filename;
    direction_filename << "/sys/class/gpio/gpio" 
                       << m_export_number 
                       << "/direction";
    std::ofstream direction_file(direction_filename.str(), std::ios::out & std::ios::trunc);
    if (!direction_file.is_open())
    {
        return false;
    }
    if (m_pin_direction == INPUT)
    {
        direction_file << "in";
    }
    else // (direction == OUTPUT)
    {
        direction_file << "out";
    }
    direction_file.close();
    return true;
}