#pragma once 
 
#include <Engine/Renderer/Animation.h> 

namespace Engine
{
	struct PathEntry
	{
		int32_t		dx = 0, dy = 0;
		uint32_t	frame = 0;
		uint32_t	delay = 0;
		PathEntry(void) = default;
		PathEntry(const PathEntry&) = default;
	}; 


	class MovingPathAnimation : public Animation
	{
	public: 
		using Path = std::vector<PathEntry>;  
		using Me = MovingPathAnimation;

	public:  
		MovingPathAnimation(
			const std::string _id,
			const Path& _path, 
			const uint32_t _reps
		) : m_Path(_path), m_reps(_reps), Animation(_id) { }

		const Path& GetPath(void) const { return m_Path; } 
		Me&			SetPath(const Path& p) { m_Path = p; return *(this); } 
		uint32_t	GetReps(void) const { return m_reps; } 
		Me&			SetReps(const uint32_t p) { m_reps = p; return *(this); } 
		bool		IsForEver(void) const { return !m_reps; }
		Animation* Clone(void) const override
		{
			return new MovingPathAnimation(m_Id, m_Path, m_reps);
		}

	private:
		Path     m_Path; 
		uint32_t m_reps = 1;
	};
}
