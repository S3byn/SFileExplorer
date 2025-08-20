#pragma once

namespace Core {
	
	enum class Cursor {
		Normal, Help, Working, Busy, Precision, Text,
		Unavailable, VResize, HResize, DResize1, DResize2, Move,
		AltSelect, LinkSelect
	};

	namespace System{

		double GetTime();

		void SetCursor(Cursor cursor);
	}

}
