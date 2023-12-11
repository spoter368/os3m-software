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
