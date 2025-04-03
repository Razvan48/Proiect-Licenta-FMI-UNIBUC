import numpy as np
from PIL import ImageDraw

import Constants


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


def show_bounding_box(screenshot, bounding_box):
    modified_screenshot = screenshot.copy()
    image_draw = ImageDraw.Draw(modified_screenshot)
    image_draw.rectangle((bounding_box[0], bounding_box[2], bounding_box[1], bounding_box[3]), outline="red", width=3)

    modified_screenshot.show()

