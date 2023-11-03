#pragma once
#include "archive.hpp"

#include <cstdint>

namespace elastic
{
	struct message_pod
	{};

	template <typename _Message>
	class message_lite
	{
		using message_type = _Message;

	public:
		message_lite() = default;

		virtual ~message_lite() = default;

	public:
		virtual message_pod& internal_type() = 0;

	public:
		template <typename _StreamBuffer>
		bool to_binary(_StreamBuffer& buffer)
		{
			return elastic::to_binary(_this->internal_type(), buffer);
		}

		template <typename _StreamBuffer>
		bool from_binary(_StreamBuffer& buffer)
		{
			return elastic::from_binary(_this()->internal_type(), buffer);
		}

		template <typename _StreamBuffer>
		bool to_json([[maybe_unused]] _StreamBuffer& buffer)
		{
			return true;
		}

		template <typename _StreamBuffer>
		bool from_json([[maybe_unused]] _StreamBuffer& buffer)
		{
			return true;
		}

		template <typename _StreamBuffer>
		bool to_xml([[maybe_unused]] _StreamBuffer& buffer)
		{
			return true;
		}

		template <typename _StreamBuffer>
		bool from_xml([[maybe_unused]] _StreamBuffer& buffer)
		{
			return true;
		}

	private:
		message_type* _this()
		{
			return static_cast<message_type*>(this);
		}
	};
} // namespace elastic