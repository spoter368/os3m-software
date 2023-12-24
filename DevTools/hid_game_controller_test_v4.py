"""
hid_game_controller_test_v4.py
12/24/2023
Contributors: Levi Purdy (lpurdy01)

This script is designed to interface with a 3D mouse using Python, utilizing the tkinter
library for the user interface and pywinusb for handling HID (Human Interface Device)
communication. It is only designed to work on Windows.

Dependencies:
- tkinter: A standard Python interface to the Tk GUI toolkit.
- pywinusb: A Python library to handle Windows HID devices.
- collections: Provides a deque (double-ended queue) data structure.
- time: Standard Python module for time-related tasks.

Features:
- Detects and connects to the specified HID device using its Vendor ID (VID) and Product ID (PID).
- Provides a real-time graphical user interface (GUI) to display the status of joystick axes.
- Continuously monitors the connection status of the HID device and handles reconnection.

Main Components:
- `is_device_connected`: Checks if the specified HID device is currently connected.
- `init_hid_device`: Initializes and opens the HID device for communication.
- `JoystickApp`: A tkinter-based application that displays the status of each axis of the joystick.
- `handle_data`: Processes incoming data packets from the HID device and updates the data deques.

This script was designed to work with the os3m mouse at firmware version:
https://github.com/spoter368/os3m-firmware/commit/fd2047c9115564218a8faca43a733f6c8e141dcb

And hardware version:
https://github.com/spoter368/os3m-hardware/commit/92e4db07d4cdb2534f60af1b8b6087584a9d66ae

Note:
- This script is specifically tailored for a 3D mouse with 6 axes.
- The `FINE` constant determines the range of values displayed in the GUI.
- The `buffer_size` is configurable and affects the responsiveness of the UI.
- The script is slow to detect disconnects, so to reset it is best to hold the reset button until the
 disconnected message appears.
"""

import tkinter as tk
from pywinusb import hid
from collections import deque
import time

# Set your VID and PID here
VID = 0x0483
PID = 0x572b

# Check if the device is connected
def is_device_connected(vid, pid):
    target_filter = hid.HidDeviceFilter(vendor_id=vid, product_id=pid)
    devices = target_filter.get_devices()
    return len(devices) > 0

# Initialize the HID device
def init_hid_device(vid, pid):
    target_filter = hid.HidDeviceFilter(vendor_id=vid, product_id=pid)
    devices = target_filter.get_devices()
    if devices:
        device = devices[0]
        device.open()
        return device
    else:
        return None

class JoystickApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Joystick Axes")
        self.hid_device = None

        self.axis_bars = []
        self.axis_labels = []
        for i in range(6):

            label = tk.Label(self, text=f"Axis {i}:")
            label.grid(row=i, column=0)

            canvas = tk.Canvas(self, width=200, height=20)
            canvas.grid(row=i, column=1)
            bar = canvas.create_rectangle(0, 0, 100, 20, fill="blue")
            self.axis_bars.append((canvas, bar))

            axis_label = tk.Label(self, text="0")
            axis_label.grid(row=i, column=2)
            self.axis_labels.append(axis_label)

        self.after(100, self.update_ui)
        self.check_device_connection()

    def update_ui(self):
        if self.hid_device:
            axis_values_str = "Axes: "
            for i, dat in enumerate(data):
                canvas, bar = self.axis_bars[i]
                new_width = (dat[-1] / FINE + 1) * 100
                canvas.coords(bar, 0, 0, new_width, 20)

                # Update axis value label
                self.axis_labels[i].config(text=str(dat[-1]))

                # Append to the axis values string
                axis_values_str += f"{dat[-1]:>6}, "

            print("\r" + axis_values_str, end="")  # Print the axis statuses on the same line
        self.after(100, self.update_ui)

    def check_device_connection(self):
        if not is_device_connected(VID, PID):
            if self.hid_device:
                self.hid_device.close()
                self.hid_device = None
                print("HID device disconnected.")
            time.sleep(1)  # Delay to avoid high CPU usage
        else:
            if not self.hid_device:
                self.hid_device = init_hid_device(VID, PID)
                if self.hid_device:
                    self.hid_device.set_raw_data_handler(handle_data)
                    print("HID device reconnected.")
        self.after(1000, self.check_device_connection)  # Check connection every second

    def on_closing(self):
        if self.hid_device:
            self.hid_device.close()
        self.destroy()

# Setup deques to store the data for plotting
buffer_size = 12  # Reduced buffer size for UI responsiveness
data = [deque([0] * buffer_size, maxlen=buffer_size) for _ in range(6)]
FINE = 10000  # Set range of view

def handle_data(data_packet):
    values = [int.from_bytes(data_packet[i * 2 + 1:i * 2 + 3], byteorder='little', signed=True) for i in range(6)]
    for val, dat in zip(values, data):
        dat.append(val)

if __name__ == "__main__":
    app = JoystickApp()
    app.protocol("WM_DELETE_WINDOW", app.on_closing)
    app.mainloop()
