import pyautogui as pag
from PIL import Image
import socket
import time

import Constants
import Utilities
import PiecesIdentifier


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # IPv4, TCP

while True:
    try:
        sock.connect((Constants.HOST, Constants.PORT))
        print('Connection successful')
        break
    except ConnectionRefusedError:
        print('Connection refused. Retrying...')
        time.sleep(Constants.SLEEP_TIME_BETWEEN_CONNECTION_RETRIES)

time.sleep(Constants.SLEEP_TIME_AFTER_CONNECT)

current_bounding_box = None

while True:
    current_screenshot = pag.screenshot()
    current_screenshot = current_screenshot.resize((int(current_screenshot.width * Constants.SCREENSHOT_SCALE_WIDTH), int(current_screenshot.height * Constants.SCREENSHOT_SCALE_HEIGHT)), Image.NEAREST)
    current_bounding_box = Utilities.find_bounding_box(current_screenshot)  # INFO: E foarte important ca imaginea sa fie color aici.

    if current_bounding_box is not None:

        current_move = PiecesIdentifier.find_info_about_board(current_screenshot, current_bounding_box)
        if current_move is not None:
            sock.send(current_move.encode())
            PiecesIdentifier.should_listen = True
            print('Current move:', current_move)
        print('Current board configuration:', PiecesIdentifier.board_configuration)
        break

    time.sleep(Constants.SLEEP_TIME_BETWEEN_SCREENSHOTS)


while True:

    while PiecesIdentifier.should_listen:
        try:
            data = sock.recv(Constants.STRING_SIZE_MOVE + Constants.STRING_SIZE_CONFIGURATION)

            if data is not None and data.decode() != '':
                print('Received:', data.decode())

                move = data.decode()[:Constants.STRING_SIZE_MOVE]
                configuration = data.decode()[Constants.STRING_SIZE_MOVE:Constants.STRING_SIZE_MOVE + Constants.STRING_SIZE_CONFIGURATION]

                Utilities.apply_move_on_board(current_bounding_box, move)
                PiecesIdentifier.board_configuration = configuration

                PiecesIdentifier.should_listen = False

                time.sleep(Constants.SLEEP_TIME_AFTER_APPLIED_MOVE)

        except BlockingIOError:
            pass

    current_screenshot = pag.screenshot()
    current_screenshot = current_screenshot.resize((int(current_screenshot.width * Constants.SCREENSHOT_SCALE_WIDTH), int(current_screenshot.height * Constants.SCREENSHOT_SCALE_HEIGHT)), Image.NEAREST)
    current_bounding_box = Utilities.find_bounding_box(current_screenshot)  # INFO: E foarte important ca imaginea sa fie color aici.

    if current_bounding_box is not None:
        # PiecesIdentifier.find_info_about_board(current_screenshot, current_bounding_box) # INFO: Se bazeaza pe board_configuration-ul curent. Poate s-a mutat ceva intre timp si vom construi gresit feature-urile pentru piese.

        Utilities.show_pieces_features()  # pentru debug

        # Utilities.show_bounding_box(current_screenshot, current_bounding_box)  # pentru debug

        # Utilities.apply_move_on_board(current_bounding_box, 'Pg7g8B')  # pentru test
        # Utilities.apply_move_on_board(current_bounding_box, 'Pe2e4$')  # pentru test

        current_move = PiecesIdentifier.find_move(PiecesIdentifier.get_changed_board_pos(current_screenshot, current_bounding_box))
        if current_move is not None:
            sock.send(current_move.encode())
            PiecesIdentifier.should_listen = True
            print('Current move:', current_move)

    time.sleep(Constants.SLEEP_TIME_IN_MAIN_LOOP)

    print('Current board configuration:', PiecesIdentifier.board_configuration)


sock.close()








