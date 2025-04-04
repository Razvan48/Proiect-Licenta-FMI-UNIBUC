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


def find_bounding_box(screenshot):
    left = screenshot.width
    right = 0
    top = screenshot.height
    bottom = 0

    for i in range(screenshot.height):
        for j in range(screenshot.width):
            pixel = screenshot.getpixel((j, i))
            if is_pixel_white_tile(pixel) or is_pixel_black_tile(pixel):
                left = min(left, j)
                right = max(right, j)
                top = min(top, i)
                bottom = max(bottom, i)

    if left == screenshot.width or right == 0 or top == screenshot.height or bottom == 0:
        return None
    return left, right, top, bottom


def show_bounding_box(screenshot, bounding_box):  # pentru debug
    modified_screenshot = screenshot.copy()
    image_draw = ImageDraw.Draw(modified_screenshot)
    image_draw.rectangle((bounding_box[0], bounding_box[2], bounding_box[1], bounding_box[3]), outline="red", width=3)

    modified_screenshot.show()


def show_pieces_features():  # pentru debug
    print('White Pawn Features:', PiecesIdentifier.white_pawn_features)
    print('White Rook Features:', PiecesIdentifier.white_rook_features)
    print('White Knight Features:', PiecesIdentifier.white_knight_features)
    print('White Bishop Features:', PiecesIdentifier.white_bishop_features)
    print('White Queen Features:', PiecesIdentifier.white_queen_features)
    print('White King Features:', PiecesIdentifier.white_king_features)

    print('Black Pawn Features:', PiecesIdentifier.black_pawn_features)
    print('Black Rook Features:', PiecesIdentifier.black_rook_features)
    print('Black Knight Features:', PiecesIdentifier.black_knight_features)
    print('Black Bishop Features:', PiecesIdentifier.black_bishop_features)
    print('Black Queen Features:', PiecesIdentifier.black_queen_features)
    print('Black King Features:', PiecesIdentifier.black_king_features)

    print('Empty Tile Features:', PiecesIdentifier.empty_tile_features)


def apply_move_on_board(bounding_box, move):  #piesa/coloana/linie/coloana/linie/promovare
    if PiecesIdentifier.is_white_above is None:
        return

    tile_width = (bounding_box[1] - bounding_box[0]) // Constants.NUM_TILES_WIDTH
    tile_height = (bounding_box[3] - bounding_box[2]) // Constants.NUM_TILES_HEIGHT

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

    pag.moveTo(bounding_box_scaled[0] + start_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + start_row * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
    pag.mouseDown()
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
    pag.mouseUp()
    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)

    if len(move) == 6:
        piece = move[0]
        promotion_piece = move[5]

        if piece.isupper():
            if PiecesIdentifier.is_white_above:
                if promotion_piece == 'Q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'N':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row - 1) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'R':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row - 2) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'B'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row - 3) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
            else:
                if promotion_piece == 'Q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'N':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row + 1) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'R':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row + 2) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'B'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row + 3) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
        else:
            if PiecesIdentifier.is_white_above:
                if promotion_piece == 'q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'n':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row + 1) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'r':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row + 2) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'b'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row + 3) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
            else:
                if promotion_piece == 'q':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + end_row * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'n':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row - 1) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                elif promotion_piece == 'r':
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row - 2) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                else:  # 'b'
                    pag.moveTo(bounding_box_scaled[0] + end_col * tile_width_scaled + tile_width_scaled // 2, bounding_box_scaled[2] + (end_row - 3) * tile_height_scaled + tile_height_scaled // 2, duration=Constants.MOUSE_MOVE_DURATION)
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)
                    pag.click()
                    time.sleep(Constants.SLEEP_TIME_WHEN_APPLYING_MOVE)



