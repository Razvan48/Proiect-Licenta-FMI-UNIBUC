import pyautogui as pag
from PIL import ImageDraw
import numpy as np
import time

import Constants
import PiecesIdentifier


def is_pixel_white_tile(pixel):
    return (np.abs(np.array(pixel) - np.array(Constants.WHITE_TILE_RGB)) < np.array(Constants.WHITE_TILE_EPS)).all()


def is_pixel_black_tile(pixel):
    return (np.abs(np.array(pixel) - np.array(Constants.BLACK_TILE_RGB)) < np.array(Constants.BLACK_TILE_EPS)).all()


def is_pixel_white_piece(pixel):
    return (np.abs(np.array(pixel) - np.array(Constants.WHITE_PIECE_RGB)) < np.array(Constants.WHITE_PIECE_EPS)).all()


def is_pixel_black_piece(pixel):
    return (np.abs(np.array(pixel) - np.array(Constants.BLACK_PIECE_RGB)) < np.array(Constants.BLACK_PIECE_EPS)).all()


def show_bounding_box(screenshot, bounding_box):  # pentru debug
    modified_screenshot = screenshot.copy()
    image_draw = ImageDraw.Draw(modified_screenshot)
    image_draw.rectangle((bounding_box[0], bounding_box[2], bounding_box[1], bounding_box[3]), outline="red", width=3)

    modified_screenshot.show()


def show_pieces_features():  # pentru debug
    print('show_pieces_features(): white_pawn_features:', PiecesIdentifier.white_pawn_features)
    print('show_pieces_features(): white_rook_features:', PiecesIdentifier.white_rook_features)
    print('show_pieces_features(): white_knight_features:', PiecesIdentifier.white_knight_features)
    print('show_pieces_features(): white_bishop_features:', PiecesIdentifier.white_bishop_features)
    print('show_pieces_features(): white_queen_features:', PiecesIdentifier.white_queen_features)
    print('show_pieces_features(): white_king_features:', PiecesIdentifier.white_king_features)

    print('show_pieces_features(): black_pawn_features:', PiecesIdentifier.black_pawn_features)
    print('show_pieces_features(): black_rook_features:', PiecesIdentifier.black_rook_features)
    print('show_pieces_features(): black_knight_features:', PiecesIdentifier.black_knight_features)
    print('show_pieces_features(): black_bishop_features:', PiecesIdentifier.black_bishop_features)
    print('show_pieces_features(): black_queen_features:', PiecesIdentifier.black_queen_features)
    print('show_pieces_features(): black_king_features:', PiecesIdentifier.black_king_features)

    print('show_pieces_features(): empty_tile_features:', PiecesIdentifier.empty_tile_features)


def apply_move_on_board(bounding_box, move):  #piesa/coloana/linie/coloana/linie/promovare
    if PiecesIdentifier.is_white_above is None:
        return

    tile_width = (bounding_box[1] - bounding_box[0]) / Constants.NUM_TILES_WIDTH
    tile_height = (bounding_box[3] - bounding_box[2]) / Constants.NUM_TILES_HEIGHT

    start_row = int(move[2]) - 1
    start_row = Constants.NUM_TILES_HEIGHT - 1 - start_row
    start_col = ord(move[1]) - ord('a')

    end_row = int(move[4]) - 1
    end_row = Constants.NUM_TILES_HEIGHT - 1 - end_row
    end_col = ord(move[3]) - ord('a')

    if PiecesIdentifier.is_white_above:
        start_row = Constants.NUM_TILES_HEIGHT - 1 - start_row
        start_col = Constants.NUM_TILES_WIDTH - 1 - start_col

        end_row = Constants.NUM_TILES_HEIGHT - 1 - end_row
        end_col = Constants.NUM_TILES_WIDTH - 1 - end_col

    scale_width = 1.0 / Constants.SCREENSHOT_SCALE_WIDTH
    scale_height = 1.0 / Constants.SCREENSHOT_SCALE_HEIGHT

    tile_width_scaled = tile_width * scale_width
    tile_height_scaled = tile_height * scale_height

    bounding_box_scaled = (bounding_box[0] * scale_width, bounding_box[1] * scale_width, bounding_box[2] * scale_height, bounding_box[3] * scale_height)

    pag.moveTo(bounding_box_scaled[0] + start_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + start_row * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
    pag.mouseDown()
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
    pag.mouseUp()
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)

    if move[Constants.STRING_SIZE_MOVE - 1] != '$':
        piece = move[0]
        promotion_piece = move[Constants.STRING_SIZE_MOVE - 1]

        if piece.isupper():
            if PiecesIdentifier.is_white_above:
                if promotion_piece == 'Q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'N':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row - 1) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'R':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row - 2) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'B'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row - 3) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
            else:
                if promotion_piece == 'Q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'N':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row + 1) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'R':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row + 2) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'B'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row + 3) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
        else:
            if PiecesIdentifier.is_white_above:
                if promotion_piece == 'q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'n':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row + 1) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'r':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row + 2) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'b'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row + 3) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
            else:
                if promotion_piece == 'q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'n':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row - 1) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'r':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row - 2) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'b'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled / 2, bounding_box_scaled[2] + (end_row - 3) * tile_height_scaled + tile_height_scaled / 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)


def is_white_piece_on_tile(screenshot, tile_bounding_box):
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0

    for i in np.arange(tile_bounding_box[2], tile_bounding_box[3]):
        for j in np.arange(tile_bounding_box[0], tile_bounding_box[1]):
            pixel = screenshot.getpixel((j, i))
            if is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1

    return num_pixels_white_piece > num_pixels_black_piece and num_pixels_white_piece > 0


def is_black_piece_on_tile(screenshot, tile_bounding_box):
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0

    for i in np.arange(tile_bounding_box[2], tile_bounding_box[3]):
        for j in np.arange(tile_bounding_box[0], tile_bounding_box[1]):
            pixel = screenshot.getpixel((j, i))
            if is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1

    return num_pixels_black_piece > num_pixels_white_piece and num_pixels_black_piece > 0


def is_tile_empty(screenshot, tile_bounding_box):
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0

    for i in np.arange(tile_bounding_box[2], tile_bounding_box[3]):
        for j in np.arange(tile_bounding_box[0], tile_bounding_box[1]):
            pixel = screenshot.getpixel((j, i))
            if is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1

    return num_pixels_white_piece == 0 and num_pixels_black_piece == 0


def show_modules_versions():
    import numpy as np
    import pyautogui as pag
    import PIL
    from PIL import Image
    from PIL import ImageDraw

    import sys
    import platform

    print('Python Version (also version for time and socket modules):', sys.version, platform.python_version())
    print('NumPy Version:', np.__version__)
    print('PyAutoGUI Version:', pag.__version__)
    print('PIL Version:', PIL.__version__)
    print('Image from PIL Version:', Image.__version__)
    # print('ImageDraw from PIL Version:', ImageDraw.__version__)





