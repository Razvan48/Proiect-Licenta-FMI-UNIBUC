import pyautogui as pag
from PIL import Image
import socket
import time

import Constants
import Utilities
import PiecesIdentifier


time.sleep(Constants.INITIAL_SLEEP_TIME)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((Constants.HOST, Constants.PORT))

while True:
    try:
        sock.connect((Constants.HOST, Constants.PORT))
        print('Connection successful')
        break
    except ConnectionRefusedError:
        print('Connection refused. Retrying...')
        time.sleep(Constants.SLEEP_TIME_BETWEEN_CONNECTION_RETRIES)

sock.setblocking(False)  # INFO: Facem non-blocking dupa ce ne-am conectat.

current_bounding_box = None

while True:
    current_screenshot = pag.screenshot()
    current_screenshot = current_screenshot.resize((int(current_screenshot.width * Constants.SCREENSHOT_SCALE_WIDTH), int(current_screenshot.height * Constants.SCREENSHOT_SCALE_HEIGHT)), Image.NEAREST)
    current_bounding_box = Utilities.find_bounding_box(current_screenshot)

    if current_bounding_box is not None:
        current_move = PiecesIdentifier.find_info_about_board(current_screenshot, current_bounding_box)
        if current_move is not None:
            sock.send(current_move.encode())
            print('Current move:', current_move)
        break

    time.sleep(Constants.SLEEP_TIME_BETWEEN_SCREENSHOTS)


while True:

    while PiecesIdentifier.should_listen:
        try:
            data = sock.recv(Constants.STRING_SIZE_MOVE + Constants.STRING_SIZE_CONFIGURATION)

            if data is not None and data.decode() != '':
                PiecesIdentifier.should_listen = False
                print('Received:', data.decode())

                move = data.decode()[:Constants.STRING_SIZE_MOVE]
                configuration = data.decode()[Constants.STRING_SIZE_MOVE:Constants.STRING_SIZE_MOVE + Constants.STRING_SIZE_CONFIGURATION]

                Utilities.apply_move_on_board(current_bounding_box, move)
                PiecesIdentifier.board_configuration = configuration

        except BlockingIOError:
            pass

    current_screenshot = pag.screenshot()
    current_screenshot = current_screenshot.resize((int(current_screenshot.width * Constants.SCREENSHOT_SCALE_WIDTH), int(current_screenshot.height * Constants.SCREENSHOT_SCALE_HEIGHT)), Image.NEAREST)
    current_bounding_box = Utilities.find_bounding_box(current_screenshot)

    if current_bounding_box is not None:
        PiecesIdentifier.find_info_about_board(current_screenshot, current_bounding_box)

        # Utilities.show_pieces_features()  # pentru debug

        # Utilities.show_bounding_box(current_screenshot, current_bounding_box)  # pentru debug

        # Utilities.apply_move_on_board(current_bounding_box, 'Pg7g8B')  # pentru test
        # Utilities.apply_move_on_board(current_bounding_box, 'Pe2e4$')  # pentru test

        current_move = PiecesIdentifier.find_move(PiecesIdentifier.get_changed_board_pos(current_screenshot, current_bounding_box))
        if current_move is not None:
            sock.send(current_move.encode())
            print('Current move:', current_move)

    time.sleep(Constants.SLEEP_TIME_IN_MAIN_LOOP)


sock.close()








