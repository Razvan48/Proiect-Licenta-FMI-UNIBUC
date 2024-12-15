#pragma once

namespace GameMetadata
{
	const int NUM_TILES_WIDTH = 8;
	const int NUM_TILES_HEIGHT = 8;

	const int NUM_CASTLING_MOVES = 4;

	const int STRING_SIZE_WITHOUT_MOVE = GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 9;
	const int STRING_SIZE_WITH_MOVE = GameMetadata::STRING_SIZE_WITHOUT_MOVE + 4;
};

