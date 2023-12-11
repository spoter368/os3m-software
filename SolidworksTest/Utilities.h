/*
Copyright(C) 2023 Colton Baldridge

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#pragma once
#include "hidapi.h"

#define TRANSLATE_X_MULTIPLIER 0.00004  // Shifting left/right on the screen
#define TRANSLATE_Y_MULTIPLIER 0.00020  // Zoom
#define TRANSLATE_Z_MULTIPLIER 0.000007  // Shifting up/down on the screen
#define ROTATE_X_MULTIPLIER 0.00009  // Roll about x-axis
#define ROTATE_Y_MULTIPLIER 0.00006  // Roll about y-axis
#define ROTATE_Z_MULTIPLIER 0.0002  // Roll about z-axis

// Function to read OS3M Mouse USB HID data
void get_hid_data(double data[6], hid_device* handle);

// Function to check if all elements of the HID data array are zero
bool checkZeroes(double data[6]);
