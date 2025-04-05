#pragma once

namespace GameMetadata
{
	const int NUM_TILES_WIDTH = 8;
	const int NUM_TILES_HEIGHT = 8;

	const int NUM_CASTLING_MOVES = 4;

	const int STRING_SIZE_WITHOUT_MOVE = GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 9;
	const int STRING_SIZE_WITH_MOVE = GameMetadata::STRING_SIZE_WITHOUT_MOVE + 4;

	const int FREQUENCY_UNTIL_DRAW_REPETITION = 3;

	const int STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY = 6;
	const int STRING_SIZE_CONFIGURATION_AUTOMATE_GAMEPLAY = GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH;
};

