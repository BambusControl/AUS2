#include "vector.h"
#include "../ds_routines.h"
#include <cstdlib>
#include <cstring>

namespace structures {

	Vector::Vector(size_t size) :
		memory_(calloc(size, 1)),
		size_(size)
	{
	}

	Vector::Vector(const Vector& other) :
		Vector(other.size_)
	{
		memcpy(memory_, other.memory_, size_);
	}

	Vector::Vector(Vector&& other) :
		// Prevezmem data z docasneho vektora
		memory_(other.memory_),
		size_(other.size_)
	{
		// Docasny vektor vynulujem, aby mi jeho destruktor neuvolnil uz moju pamat
		other.memory_ = nullptr;
		other.size_ = 0;
	}

	Vector::~Vector()
	{
		free(memory_);

		// Programátorská hygiena
		memory_ = nullptr;
		size_ = 0;
	}

	Structure* Vector::clone() const
	{
		return new Vector(*this);
	}

	size_t Vector::size() const
	{
		return size_;
	}

	Structure& Vector::operator=(const Structure& other)
	{
		if (this != &other)
		{
			*this = dynamic_cast<const Vector&>(other);
		}

		return *this;
	}

	Vector& Vector::operator=(const Vector& other)
	{
		if (this != &other)
		{
			size_ = other.size_;
			memory_ = realloc(memory_, size_);
			memcpy(memory_, other.memory_, size_);
		}

		return *this;
	}

	Vector& Vector::operator=(Vector&& other)
	{
		// Vektor prepise referenciu na vlastne data, treba uvolnit pamat
		free(memory_);

		// Presun vektora
		memory_ = other.memory_;
		size_ = other.size_;

		// Vynulovanie docasneho vektora
		other.memory_ = nullptr;
		other.size_ = 0;

		return *this;
	}

	bool Vector::operator==(const Vector& other) const
	{
		// Su identicke alebo maju rovnaku velkost a obsah
		// memcmp -> pouziva bitovy XOR

		return 
			this == &other ||							// Identicke
			size_ == other.size_ &&						// Velkost
			memcmp(memory_, other.memory_, size_) == 0;	// Obsah
	}

	byte& Vector::operator[](const int index)
	{
		DSRoutines::rangeCheckExcept(index, size_, "Vector::operator[] - Invalid index!");
		// Reinterpret nekonvertuje medzi typmi - iba kontroluje velkost 
		return *(reinterpret_cast<byte*>(memory_) + index);
	}

	byte Vector::operator[](const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Vector::operator[] const - Invalid index!");
		return *(reinterpret_cast<byte*>(memory_) + index);
	}

	byte& Vector::readBytes(const int index, const int count, byte& dest)
	{
		DSRoutines::rangeCheckExcept(index + count, size_ + 1, "Vector::readBytes - Invalid count");
		memcpy(&dest, getBytePointer(index), count);
		return dest;
	}

	void Vector::copy(const Vector& src, const int srcStartIndex, Vector& dest, const int destStartIndex, const int length)
	{
		DSRoutines::rangeCheckExcept(srcStartIndex + length, src.size_ + 1, "Vector::copy - Invalid src index!");
		DSRoutines::rangeCheckExcept(destStartIndex + length, dest.size_ + 1, "Vector::copy - Invalid dest index!");

		if (&src == &dest && abs(srcStartIndex - destStartIndex) < length)
		{
			memmove(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
		}
		else
		{
			memcpy(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
		}
	}

	byte* Vector::getBytePointer(const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Vector::getBytePointer - Invalid index!");
		return reinterpret_cast<byte*>(memory_) + index;
	}

}