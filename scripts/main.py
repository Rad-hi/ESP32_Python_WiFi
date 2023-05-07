from gui import GUI
from wifi_communicator import WiFiCommunicator

def main():
    communicator = WiFiCommunicator(max_buffer_sz=128)
    gui = GUI(communicator=communicator)
    gui.mainloop()

if __name__ == '__main__':
    main()
