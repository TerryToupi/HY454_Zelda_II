#pragma once 

#include "Engine/Assert/Assert.h"
#include <string> 
#include <cstring>  
#include <stdint.h> 
#include <vector> 
#include <stdlib.h> 
#include <assert.h> 

namespace Engine {
	template<typename T>
	class MemoryTank {
	public:
		MemoryTank() = default;
		MemoryTank(const MemoryTank&) = default; 

		void Init(uint64_t capacity) {
			this->size = sizeof(T);
			this->capacity = capacity;

			Allocate();
			segmentTank();
			return;

		}

		void freeCell(T* cell) 
		{
			memset(cell, 0, sizeof(T));
			freeList.push_back(cell);
			return;
		};

		T* cell() 
		{ 
			if (freeList.empty())
			{
				ENGINE_CORE_ERROR("Memory tank overflow");
				assert(!freeList.empty());
			}

			void* cell = freeList.back();
			freeList.pop_back();
			return (T*)cell;
		}

	protected:
		void Allocate() 
		{
			data = (T*)malloc(sizeof(T) * this->capacity);
			return;
		}

		void segmentTank() 
		{ 
			for (uint64_t i = 0; i < this->capacity; i++) 
			{
				freeList.push_back(data + i);
			}

			return;
		}

	private:
		uint64_t size = 0;
		uint64_t capacity = 0;

		T* data;
		std::vector<T*> freeList;
	};
};
