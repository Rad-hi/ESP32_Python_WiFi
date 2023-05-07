import time
import threading
import tkinter as tk
from tkinter import ttk, font

# Ref: https://github.com/rdbende/Sun-Valley-ttk-theme
# Install: pip install sv-ttk
import sv_ttk

from wifi_communicator import WiFiCommunicator, OutMessage


class GUI(tk.Tk):
    '''
    '''

    ON_BUTTON_STR = 'ON'
    OFF_BUTTON_STR = 'OFF'
    ON_COLOR = 'green'
    OFF_COLOR = 'red'

    def __init__(self, communicator: WiFiCommunicator, *, title: str = 'Test GUI', min_size: 'tuple[int, int]' = (300, 100)) -> None:
        '''
        '''
        super().__init__()

        # The wifi communicator object
        self._communicator = communicator


        # Initialize the application
        self.__set_style_and_configure_font(dark=True)
        self.__initialise_window(title, min_size)
        self.__create_widgets()

        # Bind the click on (X) button event to the __on_closing_cb
        self.protocol("WM_DELETE_WINDOW", self.__on_closing_cb)

        # Incoming messages handler
        self._end_signal = False
        self._msgs_thread_handle = threading.Thread(target=self.__messages_receiving_thread, daemon=True)
        self._msgs_thread_handle.start()
        
        # Keep alive the GUI and do whatever periodic update to the screen
        self.after(1, self.__update)

    def __set_style_and_configure_font(self, dark: bool = True):
        '''
        sets the app style and font size
        '''
        sv_ttk.set_theme("dark" if dark else 'light')
        font.nametofont('TkDefaultFont').configure(size=9)
        ttk.Style().configure('.', font=(None, 10))

    def __initialise_window(self, title, min_size):
        self.title(title)
        self.minsize(*min_size)
        
    def __create_widgets(self):
        '''
        Create all graphical elements
        '''
        self.__create_led_button()
        self.__create_received_msg_label()

    def __create_led_button(self):
        self._btn_state_txt = tk.StringVar(value=self.ON_BUTTON_STR)
        self._on_off_btn = tk.Button(self, textvariable=self._btn_state_txt, bg=self.ON_COLOR, command=self.__on_led_btn_click_cb)
        self._on_off_btn.place(relx=0.3, rely=0.2, relwidth=0.4)

    def __create_received_msg_label(self):
        self._received_msg_label = tk.StringVar(value='No messages!')
        tk.Label(self, textvariable=self._received_msg_label).place(relx=0.01, rely=0.7, relwidth=0.98)

    # -------------------- #
    #  On events callbacks #
    # -------------------- #

    def __on_closing_cb(self):
        '''
        Callback to the click on (X) event
        '''
        self._end_signal = True
        self._msgs_thread_handle.join(0.1)
        self._communicator.destroy()
        self.destroy()

    def __on_led_btn_click_cb(self, *args):
        '''
        '''
        led_on = self._btn_state_txt.get() == self.OFF_BUTTON_STR
        # Update the button text
        self._btn_state_txt.set(self.ON_BUTTON_STR if led_on else self.OFF_BUTTON_STR)
        self._on_off_btn['bg'] = self.ON_COLOR if led_on else self.OFF_COLOR

        # This should be in accordance to what the other end is expecting
        # (there must be a defined protocol for what each command means)
        msg = OutMessage(data='f' if led_on else 'n')
        self._communicator.send_message(msg)

    # ------------------------------- #
    #  Update the screen periodically #
    # ------------------------------- #

    def __messages_receiving_thread(self):
        '''
        '''
        while not self._end_signal:
            message = self._communicator.get_message()

            if message.require_acknowledgment:
                msg = OutMessage(data='A')
                self._communicator.send_message(msg)

            self._received_msg_label.set(message.data)

            time.sleep(0.001)

    def __update(self):
        '''
        This is called each 1ms to update the GUI elements and do periodic actions
        '''
        return