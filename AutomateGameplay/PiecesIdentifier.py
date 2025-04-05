import numpy as np

import Constants
import Utilities


is_white_above = None

white_pawn_features = None
white_rook_features = None
white_knight_features = None
white_bishop_features = None
white_queen_features = None
white_king_features = None

black_pawn_features = None
black_rook_features = None
black_knight_features = None
black_bishop_features = None
black_queen_features = None
black_king_features = None

empty_tile_features = [0.0, 0.0]

board_configuration = Constants.INITIAL_BOARD_CONFIGURATION

should_listen = Constants.START_BY_LISTENING


def find_if_white_above(screenshot, bounding_box):  # left, right, top, bottom
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0

    for i in range(bounding_box[2], bounding_box[3]):
        for j in range(bounding_box[0], bounding_box[1]):
            pixel = screenshot.getpixel((j, i))
            if Utilities.is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif Utilities.is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1

    if num_pixels_white_piece == 0 and num_pixels_black_piece == 0:
        return None
    return num_pixels_white_piece > num_pixels_black_piece


def find_initial_board_configuration(screenshot, bounding_box):
    global is_white_above
    global board_configuration

    if is_white_above is None:
        return None

    board_configuration = Constants.INITIAL_BOARD_CONFIGURATION

    piece_color_on_board = ''

    tile_width = (bounding_box[1] - bounding_box[0]) // Constants.NUM_TILES_WIDTH
    tile_height = (bounding_box[3] - bounding_box[2]) // Constants.NUM_TILES_HEIGHT

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_left = bounding_box[0] + tile_j * tile_width
            tile_right = bounding_box[0] + (tile_j + 1) * tile_width
            tile_top = bounding_box[2] + tile_i * tile_height
            tile_bottom = bounding_box[2] + (tile_i + 1) * tile_height

            num_pixels_white_piece = 0
            num_pixels_black_piece = 0

            for i in range(tile_top, tile_bottom):
                for j in range(tile_left, tile_right):
                    pixel = screenshot.getpixel((j, i))
                    if Utilities.is_pixel_white_piece(pixel):
                        num_pixels_white_piece += 1
                    elif Utilities.is_pixel_black_piece(pixel):
                        num_pixels_black_piece += 1

            if num_pixels_white_piece == 0 and num_pixels_black_piece == 0:
                piece_color_on_board += '.'
            elif num_pixels_white_piece > num_pixels_black_piece:
                piece_color_on_board += 'w'
            else:
                piece_color_on_board += 'b'

    if not is_white_above:
        piece_color_on_board = piece_color_on_board[::-1]

    if piece_color_on_board[:2 * Constants.NUM_TILES_WIDTH].count('w') < 2 * Constants.NUM_TILES_WIDTH:
        if piece_color_on_board[1] == '.':
            if piece_color_on_board[2 * Constants.NUM_TILES_WIDTH] == 'w':
                board_configuration = board_configuration[0:1] + '.' + board_configuration[2:2 * Constants.NUM_TILES_WIDTH] + 'N' + board_configuration[2 * Constants.NUM_TILES_WIDTH + 1:]
                return 'Ng1h3$'
            elif piece_color_on_board[2 * Constants.NUM_TILES_WIDTH + 2] == 'w':
                board_configuration = board_configuration[0:1] + '.' + board_configuration[2:2 * Constants.NUM_TILES_WIDTH + 2] + 'N' + board_configuration[2 * Constants.NUM_TILES_WIDTH + 3:]
                return 'Ng1f3$'
        elif piece_color_on_board[6] == '.':
            if piece_color_on_board[2 * Constants.NUM_TILES_WIDTH + 5] == 'w':
                board_configuration = board_configuration[0:6] + '.' + board_configuration[7:2 * Constants.NUM_TILES_WIDTH + 5] + 'N' + board_configuration[2 * Constants.NUM_TILES_WIDTH + 6:]
                return 'Nb1c3$'
            elif piece_color_on_board[2 * Constants.NUM_TILES_WIDTH + 7] == 'w':
                board_configuration = board_configuration[0:6] + '.' + board_configuration[7:2 * Constants.NUM_TILES_WIDTH + 7] + 'N' + board_configuration[3 * Constants.NUM_TILES_WIDTH:]
                return 'Nb1a3$'
        else:
            for i in range(Constants.NUM_TILES_WIDTH):
                if piece_color_on_board[Constants.NUM_TILES_WIDTH + i] == '.':
                    column = Constants.NUM_TILES_WIDTH - 1 - i
                    column = chr(ord('a') + column)
                    if piece_color_on_board[Constants.NUM_TILES_WIDTH + i + Constants.NUM_TILES_WIDTH] == 'w':
                        board_configuration = board_configuration[:Constants.NUM_TILES_WIDTH + i] + '.' + board_configuration[Constants.NUM_TILES_WIDTH + i + 1:2 * Constants.NUM_TILES_WIDTH + i] + 'P' + board_configuration[2 * Constants.NUM_TILES_WIDTH + i + 1:]
                        return 'P' + column + '2' + column + '3$'
                    elif piece_color_on_board[Constants.NUM_TILES_WIDTH + i + 2 * Constants.NUM_TILES_WIDTH] == 'w':
                        board_configuration = board_configuration[:Constants.NUM_TILES_WIDTH + i] + '.' + board_configuration[Constants.NUM_TILES_WIDTH + i + 1:3 * Constants.NUM_TILES_WIDTH + i] + 'P' + board_configuration[3 * Constants.NUM_TILES_WIDTH + i + 1:]
                        return 'P' + column + '2' + column + '4$'
    else:
        return None


def find_pieces_features(screenshot, bounding_box):
    global is_white_above

    global white_pawn_features
    global white_rook_features
    global white_knight_features
    global white_bishop_features
    global white_queen_features
    global white_king_features

    global black_pawn_features
    global black_rook_features
    global black_knight_features
    global black_bishop_features
    global black_queen_features
    global black_king_features


    tile_width = (bounding_box[1] - bounding_box[0]) // Constants.NUM_TILES_WIDTH
    tile_height = (bounding_box[3] - bounding_box[2]) // Constants.NUM_TILES_HEIGHT

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_left = bounding_box[0] + tile_j * tile_width
            tile_right = bounding_box[0] + (tile_j + 1) * tile_width
            tile_top = bounding_box[2] + tile_i * tile_height
            tile_bottom = bounding_box[2] + (tile_i + 1) * tile_height
            tile_area = (tile_right - tile_left) * (tile_bottom - tile_top)

            num_pixels_white_piece = 0
            num_pixels_black_piece = 0

            for i in range(tile_top, tile_bottom):
                for j in range(tile_left, tile_right):
                    pixel = screenshot.getpixel((j, i))
                    if Utilities.is_pixel_white_piece(pixel):
                        num_pixels_white_piece += 1
                    elif Utilities.is_pixel_black_piece(pixel):
                        num_pixels_black_piece += 1

            pixels_white_piece_ratio = num_pixels_white_piece / tile_area
            pixels_black_piece_ratio = num_pixels_black_piece / tile_area

            if is_white_above:
                pos_in_board = tile_i * Constants.NUM_TILES_WIDTH + tile_j
            else:
                pos_in_board = (Constants.NUM_TILES_HEIGHT - 1 - tile_i) * Constants.NUM_TILES_WIDTH + tile_j

            if board_configuration[pos_in_board] == 'P':
                white_pawn_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'R':
                white_rook_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'N':
                white_knight_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'B':
                white_bishop_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'Q':
                white_queen_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'K':
                white_king_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'p':
                black_pawn_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'r':
                black_rook_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'n':
                black_knight_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'b':
                black_bishop_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'q':
                black_queen_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]
            elif board_configuration[pos_in_board] == 'k':
                black_king_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]


def calculate_distance(features0, features1):
    if features0 is None or features1 is None:
        return float('inf')

    return np.linalg.norm(np.array(features0) - np.array(features1))


def find_piece_on_board(screenshot, tile_bounding_box):  # left, right, top, bottom
    global white_pawn_features
    global white_rook_features
    global white_knight_features
    global white_bishop_features
    global white_queen_features
    global white_king_features

    global black_pawn_features
    global black_rook_features
    global black_knight_features
    global black_bishop_features
    global black_queen_features
    global black_king_features

    global empty_tile_features

    tile_area = (tile_bounding_box[1] - tile_bounding_box[0]) * (tile_bounding_box[3] - tile_bounding_box[2])
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0

    for i in range(tile_bounding_box[2], tile_bounding_box[3]):
        for j in range(tile_bounding_box[0], tile_bounding_box[1]):
            pixel = screenshot.getpixel((j, i))
            if Utilities.is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif Utilities.is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1

    pixels_white_piece_ratio = num_pixels_white_piece / tile_area
    pixels_black_piece_ratio = num_pixels_black_piece / tile_area
    piece_features = [pixels_white_piece_ratio, pixels_black_piece_ratio]

    piece = None
    min_distance = float('inf')

    if white_pawn_features is not None:
        distance = calculate_distance(piece_features, white_pawn_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'P'
    if white_rook_features is not None:
        distance = calculate_distance(piece_features, white_rook_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'R'
    if white_knight_features is not None:
        distance = calculate_distance(piece_features, white_knight_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'N'
    if white_bishop_features is not None:
        distance = calculate_distance(piece_features, white_bishop_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'B'
    if white_queen_features is not None:
        distance = calculate_distance(piece_features, white_queen_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'Q'
    if white_king_features is not None:
        distance = calculate_distance(piece_features, white_king_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'K'
    if black_pawn_features is not None:
        distance = calculate_distance(piece_features, black_pawn_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'p'
    if black_rook_features is not None:
        distance = calculate_distance(piece_features, black_rook_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'r'
    if black_knight_features is not None:
        distance = calculate_distance(piece_features, black_knight_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'n'
    if black_bishop_features is not None:
        distance = calculate_distance(piece_features, black_bishop_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'b'
    if black_queen_features is not None:
        distance = calculate_distance(piece_features, black_queen_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'q'
    if black_king_features is not None:
        distance = calculate_distance(piece_features, black_king_features)
        if distance < min_distance:
            min_distance = distance
            piece = 'k'
    if empty_tile_features is not None:
        distance = calculate_distance(piece_features, empty_tile_features)
        if distance < min_distance:
            min_distance = distance
            piece = '.'
    return piece


def find_info_about_board(screenshot, bounding_box):  # left, right, top, bottom
    global is_white_above

    tile_width = (bounding_box[1] - bounding_box[0]) // Constants.NUM_TILES_WIDTH
    tile_height = (bounding_box[3] - bounding_box[2]) // Constants.NUM_TILES_HEIGHT

    current_move = None
    if is_white_above is None:  # INFO: Facem asta deoarece nu luam in calcul si posibilitatea ca tabla sa dea flip in timpul jocului.
        is_white_above = find_if_white_above(screenshot, (bounding_box[0], bounding_box[0] + tile_width, bounding_box[2], bounding_box[2] + tile_height))
        current_move = find_initial_board_configuration(screenshot, bounding_box)

    if is_white_above is not None:
        find_pieces_features(screenshot, bounding_box)

    return current_move


def get_board_configuration(screenshot, bounding_box):
    global is_white_above

    if is_white_above is None:
        return None

    tile_width = (bounding_box[1] - bounding_box[0]) // Constants.NUM_TILES_WIDTH
    tile_height = (bounding_box[3] - bounding_box[2]) // Constants.NUM_TILES_HEIGHT

    board_configuration = ''

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_left = bounding_box[0] + tile_j * tile_width
            tile_right = bounding_box[0] + (tile_j + 1) * tile_width
            tile_top = bounding_box[2] + tile_i * tile_height
            tile_bottom = bounding_box[2] + (tile_i + 1) * tile_height

            piece = find_piece_on_board(screenshot, (tile_left, tile_right, tile_top, tile_bottom))

            board_configuration += piece

    if not is_white_above:
        board_configuration = board_configuration[::-1]

    return board_configuration


def convert_index_to_pos(index):
    global is_white_above

    if is_white_above is None:
        return None

    row = index // Constants.NUM_TILES_WIDTH
    column = index % Constants.NUM_TILES_WIDTH
    column = Constants.NUM_TILES_WIDTH - 1 - column

    if not is_white_above:
        row = Constants.NUM_TILES_HEIGHT - 1 - row
        column = Constants.NUM_TILES_WIDTH - 1 - column

    row_char = chr(ord('1') + row)
    column_char = chr(ord('a') + column)

    return column_char + row_char


def get_changed_board_pos(current_screenshot, current_bounding_box):  # left, right, top, bottom
    global is_white_above
    global board_configuration

    if is_white_above is None:
        return None

    current_board_configuration = get_board_configuration(current_screenshot, current_bounding_box)

    changed_board_pos = []

    for i in range(len(board_configuration)):
        if board_configuration[i] != current_board_configuration[i]:
            changed_board_pos.append((i, convert_index_to_pos(i), board_configuration[i], current_board_configuration[i]))

    board_configuration = current_board_configuration

    return changed_board_pos


def find_move(changed_board_pos):
    if len(changed_board_pos) == 0:
        return None

    if len(changed_board_pos) == 2:  # mutari simple, capturi simple, promovari simple sau promovari prin captura

        if changed_board_pos[0][2] != '.' and changed_board_pos[0][3] == '.' and changed_board_pos[1][2] == '.' and changed_board_pos[1][3] == changed_board_pos[0][2]:  # mutare simpla
            return changed_board_pos[0][2] + changed_board_pos[0][1] + changed_board_pos[1][1] + '$'
        elif changed_board_pos[1][2] != '.' and changed_board_pos[1][3] == '.' and changed_board_pos[0][2] == '.' and changed_board_pos[0][3] == changed_board_pos[1][2]:  # mutare simpla
            return changed_board_pos[1][2] + changed_board_pos[1][1] + changed_board_pos[0][1] + '$'
        elif changed_board_pos[0][2] != '.' and changed_board_pos[0][3] == '.' and changed_board_pos[1][2] != '.' and changed_board_pos[1][3] == changed_board_pos[0][2]:  # captura simpla
            return changed_board_pos[0][2] + changed_board_pos[0][1] + changed_board_pos[1][1] + '$'
        elif changed_board_pos[1][2] != '.' and changed_board_pos[1][3] == '.' and changed_board_pos[0][2] != '.' and changed_board_pos[0][3] == changed_board_pos[1][2]:  # captura simpla
            return changed_board_pos[1][2] + changed_board_pos[1][1] + changed_board_pos[0][1] + '$'
        elif changed_board_pos[0][2] != '.' and changed_board_pos[0][3] == '.' and changed_board_pos[1][2] == '.' and changed_board_pos[1][3] != changed_board_pos[0][2]:  # promovare simpla
            return changed_board_pos[0][2] + changed_board_pos[0][1] + changed_board_pos[1][1] + changed_board_pos[1][3]
        elif changed_board_pos[1][2] != '.' and changed_board_pos[1][3] == '.' and changed_board_pos[0][2] == '.' and changed_board_pos[0][3] != changed_board_pos[1][2]:  # promovare simpla
            return changed_board_pos[1][2] + changed_board_pos[1][1] + changed_board_pos[0][1] + changed_board_pos[0][3]
        elif changed_board_pos[0][2] != '.' and changed_board_pos[0][3] == '.' and changed_board_pos[1][2] != '.' and changed_board_pos[1][3] != changed_board_pos[0][2]:  # promovare prin captura
            return changed_board_pos[0][2] + changed_board_pos[0][1] + changed_board_pos[1][1] + changed_board_pos[1][3]
        elif changed_board_pos[1][2] != '.' and changed_board_pos[1][3] == '.' and changed_board_pos[0][2] != '.' and changed_board_pos[0][3] != changed_board_pos[1][2]:  # promovare prin captura
            return changed_board_pos[1][2] + changed_board_pos[1][1] + changed_board_pos[0][1] + changed_board_pos[0][3]

    elif len(changed_board_pos) == 3:  # en passant

        white_pawn_freq = 0
        black_pawn_freq = 0
        for i in range(len(changed_board_pos)):
            if changed_board_pos[i][2] == 'P':
                white_pawn_freq += 1
            elif changed_board_pos[i][3] == 'P':
                white_pawn_freq += 1
            elif changed_board_pos[i][2] == 'p':
                black_pawn_freq += 1
            elif changed_board_pos[i][3] == 'p':
                black_pawn_freq += 1

        if white_pawn_freq == 2 and black_pawn_freq == 1:
            start_pos = None
            end_pos = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][2] == 'P':
                    start_pos = changed_board_pos[i][1]
                elif changed_board_pos[i][3] == 'P':
                    end_pos = changed_board_pos[i][1]
            return 'P' + start_pos + end_pos + '$'
        else:  # white_pawn_freq == 1 and black_pawn_freq == 2
            start_pos = None
            end_pos = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][2] == 'p':
                    start_pos = changed_board_pos[i][1]
                elif changed_board_pos[i][3] == 'p':
                    end_pos = changed_board_pos[i][1]
            return 'p' + start_pos + end_pos + '$'

    elif len(changed_board_pos) == 4:  # rocade

        white_king_appeared = False
        black_king_appeared = False

        for i in range(len(changed_board_pos)):
            if changed_board_pos[i][2] == 'K':
                white_king_appeared = True
            elif changed_board_pos[i][3] == 'K':
                white_king_appeared = True
            elif changed_board_pos[i][2] == 'k':
                black_king_appeared = True
            elif changed_board_pos[i][3] == 'k':
                black_king_appeared = True

        if white_king_appeared:
            start_pos = None
            end_pos = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][2] == 'K':
                    start_pos = changed_board_pos[i][1]
                elif changed_board_pos[i][3] == 'K':
                    end_pos = changed_board_pos[i][1]
            return 'K' + start_pos + end_pos + '$'
        else:  # black_king_appeared
            start_pos = None
            end_pos = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][2] == 'k':
                    start_pos = changed_board_pos[i][1]
                elif changed_board_pos[i][3] == 'k':
                    end_pos = changed_board_pos[i][1]
            return 'k' + start_pos + end_pos + '$'

    else:
        return None




