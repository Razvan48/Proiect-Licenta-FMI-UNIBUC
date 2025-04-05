import pyautogui as pag
from PIL import Image
import time

import Constants
import Utilities
import PiecesIdentifier


time.sleep(Constants.INITIAL_SLEEP_TIME)

last_screenshot = None
last_bounding_box = None

while True:
    current_screenshot = pag.screenshot()
    current_screenshot = current_screenshot.resize((int(current_screenshot.width * Constants.SCREENSHOT_SCALE_WIDTH), int(current_screenshot.height * Constants.SCREENSHOT_SCALE_HEIGHT)), Image.NEAREST)
    current_bounding_box = Utilities.find_bounding_box(current_screenshot)

    if current_bounding_box is not None:
        PiecesIdentifier.find_info_about_board(current_screenshot, current_bounding_box)

        # Utilities.show_pieces_features()  # pentru debug

        # Utilities.show_bounding_box(current_screenshot, current_bounding_box)  # pentru debug

        # Utilities.apply_move_on_board(current_bounding_box, 'Pg7g8B')  # pentru test

        if last_bounding_box is not None:
            print(PiecesIdentifier.get_changed_board_pos(last_screenshot, last_bounding_box, current_screenshot, current_bounding_box))

    last_screenshot = current_screenshot
    last_bounding_box = current_bounding_box
    time.sleep(Constants.SLEEP_TIME_BETWEEN_SCREENSHOTS)

# TODO:
# - readaugam board-ul curent in PiecesIdentifier
# - de implementat metoda care primeste screenshot curent + bounding box curent (si atat) si compara cu board-ul curent stocat in PiecesIdentifier
# - de obtinut din functia de mai sus mutarea facuta (asta pare cea mai grea chestie din todo)
# - nu ar mai fi nevoie de last_screenshot si last_bounding_box

# Posibile Probleme:
# - script-ul inca se bazeaza pe faptul ca prima poza facuta este cea cu board-ul initial, ceea ce creeaza probleme deoarece construim niste feature-uri la piese pe baza asta
# solutie: adaugam boolean in constants




