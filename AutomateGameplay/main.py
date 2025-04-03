import pyautogui as pag
from PIL import Image
import time

import Constants
import Utilities
import PiecesIdentifier


time.sleep(Constants.INITIAL_SLEEP_TIME)

last_screenshot = None
last_bounding_box = None
screen_width = None
screen_height = None

while True:
    current_screenshot = pag.screenshot()
    screen_width = current_screenshot.width
    screen_height = current_screenshot.height
    current_screenshot = current_screenshot.resize((int(current_screenshot.width * Constants.SCREENSHOT_SCALE_WIDTH), int(current_screenshot.height * Constants.SCREENSHOT_SCALE_HEIGHT)), Image.NEAREST)
    current_bounding_box = Utilities.find_bounding_box(current_screenshot)

    if current_bounding_box is not None:
        PiecesIdentifier.find_info_about_board(current_screenshot, current_bounding_box)

        Utilities.show_bounding_box(current_screenshot, current_bounding_box)

        '''
        if last_bounding_box is not None:
            if PiecesIdentifier.has_board_changed(last_screenshot, last_bounding_box, current_screenshot, current_bounding_box):
                pass
                # TODO: de continuat
        '''

    last_screenshot = current_screenshot
    last_bounding_box = current_bounding_box
    time.sleep(Constants.SLEEP_TIME_BETWEEN_SCREENSHOTS)

