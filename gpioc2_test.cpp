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
 * @file gpioc2.cpp
 * @author Jeremy Roy <jeremy.roy@queensu.ca>
 * @copyright (c) Jeremy Roy under the Apache License, version 2.0.
 */

#include "unistd.h"
#include "gpioc2.h"

#include <iostream>

#define DELAY 25000

 int main()
 {
     std::cout << "Instantiating GPIOC2 objects" << std::endl;
     GPIOC2 red(11, OUTPUT);
     GPIOC2 green(13, OUTPUT);
     GPIOC2 blue(15, OUTPUT);

     std::cout << "Flashing lights" << std::endl;
     for (int i = 0; i < 10; i++)
     {
         red.write_value(HIGH);
         usleep(DELAY);
         red.write_value(LOW);
         usleep(DELAY);

         green.write_value(HIGH);
         usleep(DELAY);
         green.write_value(LOW);
         usleep(DELAY);

         blue.write_value(HIGH);
         usleep(DELAY);
         blue.write_value(LOW);
         usleep(DELAY);
     }

     return 0;
 }