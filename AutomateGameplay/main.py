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
# - o sa existe 2 fisiere de comunicare intre script si aplicatie, pentru ambele directii
# - atat scriptul, cat si aplicatia ar trb sa se asigure ca fisierele sunt sterse atunci cand isi incep activitatea, pentru a evita scenarii neprevazute (trebuie si cu lock pe fisiere aici ca poate sterg simultan fisierele)
# - cand aplicatia realizeaza mutarea ar fi bine sa trimita si noua configuratie a tablei, ca sa fie mai usor pentru script sa stie board-ul curent
# - aplicatia sln ar trb sa aiba acel if cu verificat daca a primit ceva de la script in clasa BoardVisualizer, inainte sa aplice mutare din click-uri, restul functiei poate merge obisnuit,
# deoarece apelul de mutare schimba si culoarea celui ce joaca (de verificat totusi)

# Posibile Probleme:
# - se poate da lock pe un fisier pentru care nu avem garantia ca exista inca?
# - se poate da unlock pe un fisier dupa ce l-am sters?
# - script-ul inca se bazeaza pe faptul ca prima poza facuta este cea cu board-ul initial, ceea ce creeaza probleme deoarece construim niste feature-uri la piese pe baza asta
# - de mentionat totusi ca refacem acele feature-uri la fiecare screenshot, poate acest fapt ar mitiga problema (?)


