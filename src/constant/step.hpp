#ifndef STEP_HPP
#define STEP_HPP

namespace step
{
	enum {
		NONE = 0,
		STARTLINE = 1,
		HEADERS = 2,
		BODY = 4,
		RESPONSE = 8,
		KEEPALIVE = 16,
		CLOSE = 32
	};
}

#endif // STEP_HPP
