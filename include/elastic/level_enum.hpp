#pragma once

namespace elastic
{
	enum class level_type
	{
		not_serializable = 0,
		primitive_type = 1,
		object_serializable = 2,
		object_class_info = 3
	};
} // namespace elastic