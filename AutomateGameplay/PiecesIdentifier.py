from PIL import Image
import numpy as np

import Constants
import Utilities
import Model

from PIL import ImageDraw


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

empty_tile_features = None

all_pieces_features = None
all_pieces_labels = None

board_configuration = None

should_listen = Constants.START_BY_LISTENING

tile_bounding_boxes = [[None for _ in range(Constants.NUM_TILES_WIDTH)] for _ in range(Constants.NUM_TILES_HEIGHT)]


def find_bounding_box(screenshot):
    left = screenshot.width
    right = 0
    top = screenshot.height
    bottom = 0

    for i in range(screenshot.height):
        for j in range(screenshot.width):
            pixel = screenshot.getpixel((j, i))
            if Utilities.is_pixel_white_tile(pixel) or Utilities.is_pixel_black_tile(pixel):
                left = min(left, j)
                right = max(right, j)
                top = min(top, i)
                bottom = max(bottom, i)

    if left == screenshot.width or right == 0 or top == screenshot.height or bottom == 0:
        for tile_i in range(Constants.NUM_TILES_HEIGHT):
            for tile_j in range(Constants.NUM_TILES_WIDTH):
                tile_bounding_boxes[tile_i][tile_j] = None
        return None

    color_changed_row_pos = []
    color_changed_column_pos = []

    tile_width = (right - left + 1) // Constants.NUM_TILES_WIDTH
    tile_height = (bottom - top + 1) // Constants.NUM_TILES_HEIGHT

    color_changed_row_pos.append(top)
    color_changed_column_pos.append(left)

    last_pixel_rgb_row = screenshot.getpixel((left, top))
    for i in range(top + tile_height // 2, bottom + 1 - tile_height // 2):
        pixel_rgb_row = screenshot.getpixel((left, i))
        if pixel_rgb_row != last_pixel_rgb_row:  # and i - color_changed_row_pos[-1] > tile_height // 2:
            color_changed_row_pos.append(i)
        last_pixel_rgb_row = pixel_rgb_row
    color_changed_row_pos.append(bottom + 1)

    last_pixel_rgb_column = screenshot.getpixel((left, top))
    for j in range(left + tile_width // 2, right + 1 - tile_width // 2):
        pixel_rgb_column = screenshot.getpixel((j, top))
        if pixel_rgb_column != last_pixel_rgb_column:  # and j - color_changed_column_pos[-1] > tile_width // 2:
            color_changed_column_pos.append(j)
        last_pixel_rgb_column = pixel_rgb_column
    color_changed_column_pos.append(right + 1)

    for i in range(len(color_changed_row_pos) - 1):
        if color_changed_row_pos[i + 1] - color_changed_row_pos[i] == 1:
            color_changed_row_pos[i] = -1
    for i in range(len(color_changed_column_pos) - 1):
        if color_changed_column_pos[i + 1] - color_changed_column_pos[i] == 1:
            color_changed_column_pos[i] = -1
    color_changed_row_pos = [pos for pos in color_changed_row_pos if pos != -1]
    color_changed_column_pos = [pos for pos in color_changed_column_pos if pos != -1]

    print('color_changed_row_pos:', color_changed_row_pos)
    print('color_changed_column_pos:', color_changed_column_pos)

    '''
    screenshot_copy = screenshot.copy()
    draw = ImageDraw.Draw(screenshot_copy)
    for i in range(len(color_changed_row_pos)):
        draw.ellipse((color_changed_column_pos[0], color_changed_row_pos[i], color_changed_column_pos[0], color_changed_row_pos[i]), outline='red', width=2)
    for i in range(len(color_changed_column_pos)):
        draw.ellipse((color_changed_column_pos[i], color_changed_row_pos[0], color_changed_column_pos[i], color_changed_row_pos[0]), outline='red', width=2)
    screenshot_copy.show()
    '''

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_bounding_boxes[tile_i][tile_j] = (color_changed_column_pos[tile_j], color_changed_column_pos[tile_j + 1], color_changed_row_pos[tile_i], color_changed_row_pos[tile_i + 1])

    return left, right + 1, top, bottom + 1


def find_if_white_above(screenshot):
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0

    bounding_box = tile_bounding_boxes[0][0]

    for i in np.arange(bounding_box[2], bounding_box[3]):
        for j in np.arange(bounding_box[0], bounding_box[1]):
            pixel = screenshot.getpixel((j, i))
            if Utilities.is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif Utilities.is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1

    if num_pixels_white_piece == 0 and num_pixels_black_piece == 0:
        return None
    return num_pixels_white_piece > num_pixels_black_piece


def find_initial_board_configuration(screenshot):
    global is_white_above
    global board_configuration

    if is_white_above is None:
        return None

    board_configuration = Constants.INITIAL_BOARD_CONFIGURATION

    piece_color_on_board = ''

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_left = tile_bounding_boxes[tile_i][tile_j][0]
            tile_right = tile_bounding_boxes[tile_i][tile_j][1]
            tile_top = tile_bounding_boxes[tile_i][tile_j][2]
            tile_bottom = tile_bounding_boxes[tile_i][tile_j][3]

            num_pixels_white_piece = 0
            num_pixels_black_piece = 0

            for i in np.arange(tile_top, tile_bottom):
                for j in np.arange(tile_left, tile_right):
                    pixel = screenshot.getpixel((j, i))
                    if Utilities.is_pixel_white_piece(pixel):
                        num_pixels_white_piece += 1
                    elif Utilities.is_pixel_black_piece(pixel):
                        num_pixels_black_piece += 1

            if num_pixels_white_piece == 0 and num_pixels_black_piece == 0:
                piece_color_on_board += '.'
            elif num_pixels_white_piece > num_pixels_black_piece:
                piece_color_on_board += 'O'
            else:
                piece_color_on_board += 'o'

    if not is_white_above:
        piece_color_on_board = piece_color_on_board[::-1]

    if piece_color_on_board[:2 * Constants.NUM_TILES_WIDTH].count('O') < 2 * Constants.NUM_TILES_WIDTH:
        if piece_color_on_board[1] == '.':
            if piece_color_on_board[2 * Constants.NUM_TILES_WIDTH] == 'O':
                board_configuration = board_configuration[0:1] + '.' + board_configuration[2:2 * Constants.NUM_TILES_WIDTH] + 'N' + board_configuration[2 * Constants.NUM_TILES_WIDTH + 1:]
                return 'Ng1h3$'
            else:  # piece_color_on_board[2 * Constants.NUM_TILES_WIDTH + 2] == 'O'
                board_configuration = board_configuration[0:1] + '.' + board_configuration[2:2 * Constants.NUM_TILES_WIDTH + 2] + 'N' + board_configuration[2 * Constants.NUM_TILES_WIDTH + 3:]
                return 'Ng1f3$'
        elif piece_color_on_board[6] == '.':
            if piece_color_on_board[2 * Constants.NUM_TILES_WIDTH + 5] == 'O':
                board_configuration = board_configuration[0:6] + '.' + board_configuration[7:2 * Constants.NUM_TILES_WIDTH + 5] + 'N' + board_configuration[2 * Constants.NUM_TILES_WIDTH + 6:]
                return 'Nb1c3$'
            else:  # piece_color_on_board[2 * Constants.NUM_TILES_WIDTH + 7] == 'O'
                board_configuration = board_configuration[0:6] + '.' + board_configuration[7:2 * Constants.NUM_TILES_WIDTH + 7] + 'N' + board_configuration[3 * Constants.NUM_TILES_WIDTH:]
                return 'Nb1a3$'
        else:
            for i in range(Constants.NUM_TILES_WIDTH):
                if piece_color_on_board[Constants.NUM_TILES_WIDTH + i] == '.':
                    column = Constants.NUM_TILES_WIDTH - 1 - i
                    column = chr(ord('a') + column)
                    if piece_color_on_board[Constants.NUM_TILES_WIDTH + i + Constants.NUM_TILES_WIDTH] == 'O':
                        board_configuration = board_configuration[:Constants.NUM_TILES_WIDTH + i] + '.' + board_configuration[Constants.NUM_TILES_WIDTH + i + 1:2 * Constants.NUM_TILES_WIDTH + i] + 'P' + board_configuration[2 * Constants.NUM_TILES_WIDTH + i + 1:]
                        return 'P' + column + '2' + column + '3$'
                    elif piece_color_on_board[Constants.NUM_TILES_WIDTH + i + 2 * Constants.NUM_TILES_WIDTH] == 'O':
                        board_configuration = board_configuration[:Constants.NUM_TILES_WIDTH + i] + '.' + board_configuration[Constants.NUM_TILES_WIDTH + i + 1:3 * Constants.NUM_TILES_WIDTH + i] + 'P' + board_configuration[3 * Constants.NUM_TILES_WIDTH + i + 1:]
                        return 'P' + column + '2' + column + '4$'
    else:
        return None


def find_pieces_features(screenshot):
    global is_white_above
    global board_configuration

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

    global all_pieces_features
    global all_pieces_labels

    if is_white_above is None:
        return

    white_pawn_features = []
    white_rook_features = []
    white_knight_features = []
    white_bishop_features = []
    white_queen_features = []
    white_king_features = []

    black_pawn_features = []
    black_rook_features = []
    black_knight_features = []
    black_bishop_features = []
    black_queen_features = []
    black_king_features = []

    empty_tile_features = []

    all_pieces_features = []
    all_pieces_labels = []

    print('Board Configuration in find_pieces_features:', board_configuration)

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_left = tile_bounding_boxes[tile_i][tile_j][0]
            tile_right = tile_bounding_boxes[tile_i][tile_j][1]
            tile_top = tile_bounding_boxes[tile_i][tile_j][2]
            tile_bottom = tile_bounding_boxes[tile_i][tile_j][3]

            if is_white_above:
                pos_in_board = tile_i * Constants.NUM_TILES_WIDTH + tile_j
            else:
                pos_in_board = (Constants.NUM_TILES_HEIGHT - 1 - tile_i) * Constants.NUM_TILES_WIDTH + tile_j

            piece_data = screenshot.crop((tile_left, tile_top, tile_right, tile_bottom))
            piece_data = piece_data.convert('RGB')
            piece_data = piece_data.resize((Constants.TEMPLATE_WIDTH, Constants.TEMPLATE_HEIGHT), Image.NEAREST)
            # piece_data.save(f'test/{np.random.random()}.png')
            piece_data = np.array(piece_data)
            # piece_data = ((np.array(piece_data).reshape(Constants.TEMPLATE_HEIGHT, Constants.TEMPLATE_WIDTH, Constants.NUM_COLOR_CHANNELS) / float(Constants.MAX_VALUE_PIXEL)) - 0.5) * 2.0
            # piece_data = np.array(piece_data).reshape(Constants.TEMPLATE_HEIGHT, Constants.TEMPLATE_WIDTH, Constants.NUM_COLOR_CHANNELS)
            # INFO: np.array(piece_data) dupa conversia la grayscale sau RGB are shape-ul (height, width), nu (width, height).

            # INFO: Ce urmeaza este abordarea veche.
            tile_area = Constants.TEMPLATE_WIDTH * Constants.TEMPLATE_HEIGHT

            num_pixels_white_piece = 0
            num_pixels_black_piece = 0
            for i in range(Constants.TEMPLATE_HEIGHT):
                for j in range(Constants.TEMPLATE_WIDTH):
                    pixel = piece_data[i][j]
                    if Utilities.is_pixel_white_piece(pixel):
                        num_pixels_white_piece += 1
                    elif Utilities.is_pixel_black_piece(pixel):
                        num_pixels_black_piece += 1
            print('find_piece_features(): num_pixels_white_piece:', num_pixels_white_piece, 'num_pixels_black_piece:', num_pixels_black_piece, 'tile_area:', tile_area)
            piece_features = [num_pixels_white_piece / tile_area, num_pixels_black_piece / tile_area]
            print('find_piece_features(): piece_features:', piece_features, 'piece should be', board_configuration[pos_in_board])


            # INFO: Atentie la faptul ca dataset-ul trebuie sa fie balansat.
            if board_configuration[pos_in_board] == 'P':
                white_pawn_features.append(piece_features)
                # white_pawn_features = piece_features
            elif board_configuration[pos_in_board] == 'R':
                white_rook_features.append(piece_features)
                # white_rook_features = piece_features
            elif board_configuration[pos_in_board] == 'N':
                white_knight_features.append(piece_features)
                # white_knight_features = piece_features
            elif board_configuration[pos_in_board] == 'B':
                white_bishop_features.append(piece_features)
                # white_bishop_features = piece_features
            elif board_configuration[pos_in_board] == 'Q':
                white_queen_features.append(piece_features)
                # white_queen_features = piece_features
            elif board_configuration[pos_in_board] == 'K':
                white_king_features.append(piece_features)
                # white_king_features = piece_features
            elif board_configuration[pos_in_board] == 'p':
                black_pawn_features.append(piece_features)
                # black_pawn_features = piece_features
            elif board_configuration[pos_in_board] == 'r':
                black_rook_features.append(piece_features)
                # black_rook_features = piece_features
            elif board_configuration[pos_in_board] == 'n':
                black_knight_features.append(piece_features)
                # black_knight_features = piece_features
            elif board_configuration[pos_in_board] == 'b':
                black_bishop_features.append(piece_features)
                # black_bishop_features = piece_features
            elif board_configuration[pos_in_board] == 'q':
                black_queen_features.append(piece_features)
                # black_queen_features = piece_features
            elif board_configuration[pos_in_board] == 'k':
                black_king_features.append(piece_features)
                # black_king_features = piece_features
            else:
                empty_tile_features.append(piece_features)
                # empty_tile_features = piece_features

            '''
            if board_configuration[pos_in_board] != '.':
                all_pieces_features.append(piece_data)
                all_pieces_labels.append(Constants.FROM_PIECE_TO_LABEL[board_configuration[pos_in_board]])
            '''

    white_pawn_features = np.array(white_pawn_features)
    white_rook_features = np.array(white_rook_features)
    white_knight_features = np.array(white_knight_features)
    white_bishop_features = np.array(white_bishop_features)
    white_queen_features = np.array(white_queen_features)
    white_king_features = np.array(white_king_features)

    black_pawn_features = np.array(black_pawn_features)
    black_rook_features = np.array(black_rook_features)
    black_knight_features = np.array(black_knight_features)
    black_bishop_features = np.array(black_bishop_features)
    black_queen_features = np.array(black_queen_features)
    black_king_features = np.array(black_king_features)

    empty_tile_features = np.array(empty_tile_features)

    '''
    all_pieces_features = np.array(all_pieces_features)
    all_pieces_labels = np.array(all_pieces_labels)

    Model.build_dataset_and_dataloader(all_pieces_features, all_pieces_labels)
    Model.train_model()
    '''


def calculate_distance(current_piece_features, template_features):
    if current_piece_features is None or template_features is None:
        return float('inf')

    template_features_mean = np.mean(template_features, axis=0)
    template_features_mean = template_features_mean.reshape(-1)

    print('current_piece_features shape', np.array(current_piece_features).shape)
    print('template_features_mean shape', template_features_mean.shape)

    '''
    reshaped_current_piece_features = current_piece_features.reshape(1, -1)
    reshaped_template_features = template_features.reshape(template_features.shape[0], -1)

    differences = reshaped_current_piece_features - reshaped_template_features
    distances = np.linalg.norm(differences, axis=1)

    return np.mean(distances)
    '''
    return np.abs(current_piece_features[0] - template_features_mean[0]) + np.abs(current_piece_features[1] - template_features_mean[1])


def find_piece_on_board(screenshot, tile_i, tile_j):
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

    tile_bounding_box = tile_bounding_boxes[tile_i][tile_j]

    piece_data = screenshot.crop((tile_bounding_box[0], tile_bounding_box[2], tile_bounding_box[1], tile_bounding_box[3]))
    piece_data_empty_tile_test = piece_data.copy()  # INFO: Doar pentru cele 2 for-uri de mai jos.
    piece_data = piece_data.convert('RGB')
    piece_data = piece_data.resize((Constants.TEMPLATE_WIDTH, Constants.TEMPLATE_HEIGHT), Image.NEAREST)
    # piece_data.save(f'test/{np.random.random()}.png')
    piece_data = np.array(piece_data)
    # piece_data = (np.array(piece_data).reshape(Constants.TEMPLATE_HEIGHT, Constants.TEMPLATE_WIDTH, Constants.NUM_COLOR_CHANNELS) / float(Constants.MAX_VALUE_PIXEL) - 0.5) * 2.0
    # piece_data = np.array(piece_data).reshape(Constants.TEMPLATE_HEIGHT, Constants.TEMPLATE_WIDTH, Constants.NUM_COLOR_CHANNELS)
    # INFO: np.array(piece_data) dupa conversia la grayscale sau RGB are shape-ul (height, width), nu (width, height).

    piece_data_empty_tile_test = piece_data_empty_tile_test.convert('RGB')
    piece_data_empty_tile_test = piece_data_empty_tile_test.resize((Constants.TEMPLATE_WIDTH, Constants.TEMPLATE_HEIGHT), Image.NEAREST)
    num_pixels_white_piece = 0
    num_pixels_black_piece = 0
    for i in range(Constants.TEMPLATE_HEIGHT):
        for j in range(Constants.TEMPLATE_WIDTH):
            pixel = piece_data[i][j]
            if Utilities.is_pixel_white_piece(pixel):
                num_pixels_white_piece += 1
            elif Utilities.is_pixel_black_piece(pixel):
                num_pixels_black_piece += 1
    tile_area = Constants.TEMPLATE_WIDTH * Constants.TEMPLATE_HEIGHT
    print('num_pixels_white_piece:', num_pixels_white_piece, 'num_pixels_black_piece:', num_pixels_black_piece, 'tile_area:', tile_area)
    piece_features = [num_pixels_white_piece / tile_area, num_pixels_black_piece / tile_area]

    global is_white_above
    pos_on_board = tile_i * Constants.NUM_TILES_WIDTH + tile_j
    if not is_white_above:
        pos_on_board = (Constants.NUM_TILES_HEIGHT - 1 - tile_i) * Constants.NUM_TILES_WIDTH + tile_j
    print('piece_features find_piece_on_board():', piece_features, 'piece should be', board_configuration[pos_on_board])

    # if num_pixels_white_piece == 0 and num_pixels_black_piece == 0:
    #    return '.'

    # return Model.inference(piece_data)

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

    print('piece is:', piece)

    return piece


def find_info_about_board(screenshot):  # left, right, top, bottom
    global is_white_above

    current_move = None
    if is_white_above is None:  # INFO: Facem asta deoarece nu luam in calcul si posibilitatea ca tabla sa dea flip in timpul jocului.
        is_white_above = find_if_white_above(screenshot)
        current_move = find_initial_board_configuration(screenshot)
        find_pieces_features(screenshot)

    return current_move


def get_board_configuration(screenshot):
    global is_white_above

    if is_white_above is None:
        return None

    board_configuration = ''
    color_configuration = ''

    for tile_i in range(Constants.NUM_TILES_HEIGHT):
        for tile_j in range(Constants.NUM_TILES_WIDTH):
            tile_left = tile_bounding_boxes[tile_i][tile_j][0]
            tile_right = tile_bounding_boxes[tile_i][tile_j][1]
            tile_top = tile_bounding_boxes[tile_i][tile_j][2]
            tile_bottom = tile_bounding_boxes[tile_i][tile_j][3]

            piece = find_piece_on_board(screenshot, tile_i, tile_j)

            color = None
            if Utilities.is_tile_empty(screenshot, (tile_left, tile_right, tile_top, tile_bottom)):
                color = '.'
            elif Utilities.is_white_piece_on_tile(screenshot, (tile_left, tile_right, tile_top, tile_bottom)):
                color = 'O'
            elif Utilities.is_black_piece_on_tile(screenshot, (tile_left, tile_right, tile_top, tile_bottom)):
                color = 'o'

            board_configuration += piece
            color_configuration += color

    if not is_white_above:
        board_configuration = board_configuration[::-1]
        color_configuration = color_configuration[::-1]

    return board_configuration, color_configuration


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


def get_changed_board_pos(current_screenshot):
    global is_white_above
    global board_configuration

    if is_white_above is None:
        return None

    current_board_configuration, current_color_configuration = get_board_configuration(current_screenshot)

    print('Current board configuration:', current_board_configuration)
    print('Current color configuration:', current_color_configuration)

    changed_board_pos = []

    for i in range(len(board_configuration)):
        if (board_configuration[i] != '.' and current_color_configuration[i] == '.') \
                or (board_configuration[i] == '.' and current_color_configuration[i] != '.') \
                or (board_configuration[i].isupper() and current_color_configuration[i] == 'o') \
                or (board_configuration[i].islower() and current_color_configuration[i] == 'O'):
            changed_board_pos.append([i, convert_index_to_pos(i), board_configuration[i], current_color_configuration[i]])

    # board_configuration = current_board_configuration  # INFO: Nu ar fi nevoie, deoarece nu este mereu calculata corect si oricum primim board-ul de la aplicatie ulterior.

    print('Initial changed board positions:', changed_board_pos)

    num_white_pieces = 0
    num_black_pieces = 0
    for i in range(len(changed_board_pos)):
        if changed_board_pos[i][2].isupper():
            num_white_pieces += 1
        elif changed_board_pos[i][2].islower():
            num_black_pieces += 1
        if changed_board_pos[i][3].isupper():
            num_white_pieces += 1
        elif changed_board_pos[i][3].islower():
            num_black_pieces += 1

    if num_white_pieces > num_black_pieces:  # Mutare Alb
        if len(changed_board_pos) == 2:
            white_piece = None
            white_piece_pos = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][2].isupper():  # INFO: Pe pozitia 2 nu ar trebuie sa fie niciodata 'O'.
                    white_piece = changed_board_pos[i][2]
                    white_piece_pos = changed_board_pos[i][1]
                    break

            if white_piece == 'P' and white_piece_pos[1] == '7':
                for i in range(len(changed_board_pos)):
                    if changed_board_pos[i][3] == 'O':
                        changed_board_pos[i][3] = current_board_configuration[changed_board_pos[i][0]]

            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][3] == 'O':
                    changed_board_pos[i][3] = white_piece

        elif len(changed_board_pos) == 3:  # En Passant, stim sigur ca e vorba de un P.
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][3] == 'O':
                    changed_board_pos[i][3] = 'P'

        else:  # Rocade
            left_side_castling = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][1][0] == 'h':
                    left_side_castling = True
                    break
                elif changed_board_pos[i][1][0] == 'a':
                    left_side_castling = False
                    break

            if left_side_castling:
                for i in range(len(changed_board_pos)):
                    if changed_board_pos[i][3] == 'O':
                        if changed_board_pos[i][1][0] == 'g':
                            changed_board_pos[i][3] = 'K'
                        elif changed_board_pos[i][1][0] == 'f':
                            changed_board_pos[i][3] = 'R'
            else:
                for i in range(len(changed_board_pos)):
                    if changed_board_pos[i][3] == 'O':
                        if changed_board_pos[i][1][0] == 'c':
                            changed_board_pos[i][3] = 'K'
                        elif changed_board_pos[i][1][0] == 'd':
                            changed_board_pos[i][3] = 'R'

    else:  # Mutare Negru
        if len(changed_board_pos) == 2:
            black_piece = None
            black_piece_pos = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][2].islower():  # INFO: Pe pozitia 2 nu ar trebuie sa fie niciodata 'o'.
                    black_piece = changed_board_pos[i][2]
                    black_piece_pos = changed_board_pos[i][1]
                    break

            if black_piece == 'p' and black_piece_pos[1] == '2':
                for i in range(len(changed_board_pos)):
                    if changed_board_pos[i][3] == 'o':
                        changed_board_pos[i][3] = current_board_configuration[changed_board_pos[i][0]]

            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][3] == 'o':
                    changed_board_pos[i][3] = black_piece

        elif len(changed_board_pos) == 3:  # En Passant, stim sigur ca e vorba de un p.
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][3] == 'o':
                    changed_board_pos[i][3] = 'p'

        else:  # Rocade
            left_side_castling = None
            for i in range(len(changed_board_pos)):
                if changed_board_pos[i][1][0] == 'h':
                    left_side_castling = True
                    break
                elif changed_board_pos[i][1][0] == 'a':
                    left_side_castling = False
                    break

            if left_side_castling:
                for i in range(len(changed_board_pos)):
                    if changed_board_pos[i][3] == 'o':
                        if changed_board_pos[i][1][0] == 'g':
                            changed_board_pos[i][3] = 'k'
                        elif changed_board_pos[i][1][0] == 'f':
                            changed_board_pos[i][3] = 'r'
            else:
                for i in range(len(changed_board_pos)):
                    if changed_board_pos[i][3] == 'o':
                        if changed_board_pos[i][1][0] == 'c':
                            changed_board_pos[i][3] = 'k'
                        elif changed_board_pos[i][1][0] == 'd':
                            changed_board_pos[i][3] = 'r'

    print('Changed board positions at end of get_changed_board_pos():', changed_board_pos)

    return changed_board_pos


def find_move(changed_board_pos):
    if len(changed_board_pos) == 0:
        return None

    print('Changed board positions in find_move():', changed_board_pos)

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
        else:
            print('find_move: Invalid move')
            return None

    elif len(changed_board_pos) == 3:  # en passant

        white_pawn_freq = 0
        black_pawn_freq = 0
        for i in range(len(changed_board_pos)):
            if changed_board_pos[i][2] == 'P':
                white_pawn_freq += 1
            elif changed_board_pos[i][3] == 'P':
                white_pawn_freq += 1
            if changed_board_pos[i][2] == 'p':
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

        for i in range(len(changed_board_pos)):
            if changed_board_pos[i][2] == 'K':
                white_king_appeared = True
            elif changed_board_pos[i][3] == 'K':
                white_king_appeared = True

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
        print('find_move: Invalid move')
        return None




